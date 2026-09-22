"""Assemble observed results only; leave absent board observations explicit."""
import datetime,hashlib,json,posixpath
from pathlib import Path
P=Path.cwd(); E=P/'progress/BPF_STATIC_0922'
def load(f,default=None):return json.loads(f.read_text()) if f.exists() else default
def sha(f):
    with f.open('rb') as s:return hashlib.file_digest(s,'sha256').hexdigest()
rows=[]; evidence={}; fs=[]
for arch in ('armv7l','aarch64'):
    r=load(E/f'verify-{arch}/result.json');evidence[arch]=r
    exitfile=E/f'cells/bpf-{arch}-libcxx/exitcode';rc=exitfile.read_text().strip() if exitfile.exists() else 'NOT_OBSERVED'
    rows.append(f"| {arch} 整轮/三RPM | {'PASS' if rc=='0' and r and len(r['rpms'])==3 else 'NOT_OBSERVED'} | 退出{rc}；三份RPM核验见verify-{arch}/commands.json |")
    rows.append(f"| {arch} RPM内static ELF | {r['status'] if r else 'NOT_OBSERVED'} | verify-{arch}/result.json；不是旧ELF |")
    if r:
        f=E/f'verify-{arch}/libbpf-symbol-intersection.txt';content=f.read_text();pretty=content.replace('\\n','\n')
        assert len(pretty.splitlines())==r['observations']['libbpf_archive']['symbol_intersection']
        (E/f'verify-{arch}/libbpf-symbol-intersection-readable.txt').write_text(pretty)
    trace=E/f'link-audit-{arch}/resolution.txt'
    if trace.exists():
        for line in trace.read_text().splitlines():
            name=posixpath.normpath(line.strip());root=P/('tmp/BPF_W1_0921/input-armv7l-origin' if arch=='armv7l' else 'tmp/BPF_W1_0921/input-aarch64-v3')
            f=root/name.lstrip('/');assert f.is_file()
            fs.append(dict(arch=arch,linker_observed_path=line,normalized_guest_path=name,host_path=str(f),bytes=f.stat().st_size,sha256=sha(f)))
(E/'STDCXXFS_RESOLUTION.json').write_text(json.dumps(fs,indent=2))
board=load(E/'board/SUMMARY.json'); install=load(E/'board/INSTALL_RESULT.json'); pre=load(E/'board/INSTALL_PREFLIGHT.json'); cancel=load(E/'board/cancel_summary.json'); cleanup=load(E/'board/CLEANUP.json')
pairs=load(E/'board/pairs.json',[]);selection=load(E/'board/PROBE_SELECTION.json',{})
rows.append(f"| ARM static正常安装预检 | {'PASS' if pre and pre['static']['exitcode']==0 else 'FAIL（已停止）' if pre else 'NOT_OBSERVED'} | board/INSTALL_PREFLIGHT.json；不绕过、不重试 |")
rows.append(f"| ARM正常安装static与%post | {install['status'] if install else 'NOT_OBSERVED'} | board/INSTALL_PREFLIGHT.json；正式安装未执行时无INSTALL_RESULT.json |")
rows.append(f"| ARM候选/原件成对功能 | {'见成对表' if pairs else 'NOT_OBSERVED'} | board/pairs.json仅实际执行后才生成；本轮未执行 |")
rows.append(f"| ARM主包安装预检 | {'PASS' if pre and pre['main']['exitcode']==0 else 'FAIL（非static失败）' if pre else 'NOT_OBSERVED'} | 仅预检，未安装主包；board/INSTALL_PREFLIGHT.json |")
for link in ('shared-gcc','static-gcc'):
    c=cancel.get(link) if cancel else None
    rows.append(f"| ARM取消 {link} | {str(c['passed'])+'/20' if c else 'NOT_OBSERVED'} | 物理板；共享为主证据，静态为对照；不预设崩溃 |")
rows.append(f"| 板端恢复与清理 | {cleanup['status'] if cleanup else 'NOT_OBSERVED'} | board/before.json、after.json、CLEANUP.json |")
pairrows=[]
for x in pairs:
    a,b=x['candidate'],x['original'];pairrows.append(f"| {x['name']} | {a['exitcode']} | {b['exitcode']} | {x['classification']} | {'相同' if x['stdout_equal'] else '有差异'} / {'相同' if x['stderr_equal'] else '有差异'} |")
    import difflib
    for field,suffix in [('stdout','out'),('stderr','err')]:
        delta=''.join(difflib.unified_diff(b[field].splitlines(True),a[field].splitlines(True),fromfile='original/'+x['name'],tofile='candidate/'+x['name']))
        (E/'board'/f"{x['name']}.{suffix}.diff").write_text(delta)
has_candidate_only=any(x['classification']=='CANDIDATE_ONLY_FAILURE' for x in pairs)
status='W2R_COMPLETE' if all(r and r['status']=='PASS' for r in evidence.values()) else 'PARTIAL'
boardstatus='BLOCKED_AT_NORMAL_INSTALL_PREFLIGHT' if pre and pre['static']['exitcode']!=0 else 'NOT_OBSERVED' if not board else 'CANDIDATE_ONLY_FAILURE' if has_candidate_only else 'PAIRED_RESULTS_RECORDED'
identities=[]
for row in load(E/'INPUTS.json',[]):
    f=Path(row['source']);current=sha(f);identities.append(dict(path=str(f),expected=row['sha256'],actual=current,match=current==row['sha256']))
assert all(x['match'] for x in identities)
(E/'ORIGINAL_INPUTS_FINAL.json').write_text(json.dumps(dict(status='PASS',checks=identities),indent=2))
text=f'''# 新配方写包与 ARM 上板结果

本轮状态：**{status} / W3={boardstatus}**。两架构写包与RPM内ELF验收通过；ARM正常安装预检失败，功能对照和取消探针未执行，尚不能确认候选可替换发布资产。不推包仓、不覆盖Source1002、不起QuickBuild。

## 一张表看门禁

| 项 | 结果 | 证据/边界 |
|---|---|---|
{chr(10).join(rows)}

aarch64 不上板，运行功能本轮 NOT_OBSERVED（按任务范围）。两格从源码tar重新执行prep/build/install/write，不复用上轮重链接ELF。六份RPM（每格main/common/static三份）身份、大小、payload和文件数量见 [RPM_INVENTORY.tsv](RPM_INVENTORY.tsv)，依赖见 [RPM_REQUIRES.tsv](RPM_REQUIRES.tsv)。逐份 `rpm -K --nosignature`、文件清单、脚本与提取命令在 verify-*/commands.json。

## 配方与兼容性范围

临时spec只增加Patch0和prep无条件应用，完整差异为 [SPEC_DIFF.patch](SPEC_DIFF.patch)。配方全文与补丁在 recipe/，SHA在recipe/IDENTITY.json。补丁只改变 STATIC_LINKING 且 LIBCLANG_STATIC_PATH 分支的一行，将裸 bpf 改为 `${{LIBBPF_LIBRARIES}}`。主包关闭STATIC_LINKING，GCC/未定义路径不构建static；未添加新开关，未改原spec。GCC/未定义本轮未重构，不把分支核查当作重测。

两架构沿用BPF_W1_0921隔离入口及输入根，只换本轮tmp与SOURCES副本；宏预检均为w5.xzdio，实际RPM payload需与清单相符。j1、nice19、ionice idle、cgroup 16536457216字节均保留；实际PID/cgroup/memory.max在cells事件。开始磁盘143GiB、I/O0.104秒，资源门禁0；每500目标探测和小时状态由guard保留。未并行构建两架构。

## RPM中 ELF 身份

'''
for arch,r in evidence.items():
    if not r:continue
    o=r['observations'];s=o['static'];prior=o['link_only_previous_elf']
    text+=f"### {arch}\n\n- 新RPM内ELF：`{s['sha256']}`，{s['bytes']}字节。\n- 上轮仅重链接ELF：`{prior['sha256']}`，{prior['bytes']}字节。两者只登记，不解释SHA差异。\n- DT_NEEDED：`{', '.join(s['needed'])}`。\n- 命名空间计数：`{json.dumps(o['static_markers'])}`；libbpf已定义符号交集{o['libbpf_archive']['symbol_intersection']}，包含bpf_object__open。\n- ELF类别/架构、压缩包提取后.text/.rodata与本轮构建ELF一致、精确四依赖均见验收JSON。\n\n"
text+='两个新RPM内ELF与上轮重链接ELF的对比：ARM SHA不同；aarch64 SHA相同。只登记本次身份关系，不推广为可重现性证明。\n\n'
text+='## -lstdc++fs 待办（不改）\n\n来自 `src/CMakeLists.txt:188-191` 的Clang/旧GCC文件系统分支，不是spec直接加入。另起独立输出文件，以相同链接参数附加 `-Wl,--trace` 实测解析，未覆盖RPM、构建ELF或原dependency文件；不据此推断所有成员均被拉入。\n\n'
for x in fs:text+=f"- {x['arch']}：`{x['linker_observed_path']}`，静态归档{x['bytes']}字节，SHA `{x['sha256']}`。\n"
text+='\nW4需审阅这项链接选择；本轮不删除、不替换它。原始trace与命令在link-audit-*/。\n\n## ARM成对对照\n\n'
if pre and pre['static']['exitcode']!=0:
    text+='正常 static 预检实际失败，未正式安装，故 `--version`、`--info`、`-l`、三种脚本各3次以及两组取消各20次全部 NOT_OBSERVED（实际执行0次，不是0/20通过或失败）。不得把尚未运行的成对测试说成等价。见 [BOARD_ANALYSIS.md](BOARD_ANALYSIS.md) 的原文与现场状态。\n\n'
text+='| 调用 | 候选退出码 | 原件退出码 | 机械分类（不是功能结论） | stdout / stderr |\n|---|---:|---:|---|---|\n'+('\n'.join(pairrows) if pairrows else '| 全部 | NOT_OBSERVED | NOT_OBSERVED | 未取得板测结果 | 未观测 |')+'\n\n'
if pairs:
    text+=f"内核探针选择：`{selection.get('selected','NOT_OBSERVED')}`；只采用实际-l交集。实际已执行次数和输出以上表及board记录为准。\n\n"
else:
    text+='原计划是每项候选后原件、脚本各3对、取消各20次，并保存-l完整输出/前50行和--info逐行diff；**此次未执行这些步骤，因此不存在这些输出文件，也未选择内核探针**。准备了脚本不等于已执行。\n\n'
text+='## 失败保留、工具问题与下一步\n\nARM第一次核验器正则双重转义误判ELF类别；实际readelf已是ELF32/ARM。首次目录、脚本、退出1记录完整保留；修复核验器后对同一RPM在新解包目录核验通过，不是重跑构建或替换候选。符号交集原文本分隔符问题同样保留原件并提供可读副本，不影响集合与门禁计算。详见 DECISIONS.md。\n\n'
text+='W4所需spec/patch、RPM SHA与板测门禁见 W4_INPUTS.md。只有人工审阅签字后才能推进sandbox配方与QuickBuild；本轮不做这两项。\n'
(E/'FINAL_RESULT.md').write_text(text)
w4='# W4 输入（未授权推包仓）\n\n- 完整候选spec：recipe/bpftrace.spec\n- 完整spec差异：SPEC_DIFF.patch\n- 新Patch：recipe/static-link-libbpf.patch\n- 配方/补丁身份：recipe/IDENTITY.json\n- 六份RPM路径、SHA、大小：RPM_INVENTORY.tsv\n- RPM依赖：RPM_REQUIRES.tsv\n- RPM内新ELF验收：verify-armv7l/result.json、verify-aarch64/result.json\n- 物理板证据/恢复：board/；不得引用旧ELF当成本轮输入\n- -lstdc++fs解析：STDCXXFS_RESOLUTION.json、link-audit-*/；本轮未改\n\n'
w4+=f'当前：{status}，W3={boardstatus}。安装预检阻断、未执行项与尚存门禁见FINAL_RESULT.md与BOARD_ANALYSIS.md。未获得W4人工签字，不推sandbox或包仓，不启动QuickBuild。\n'
if pre and pre['static']['exitcode']!=0:w4+='当前阻断是正常static安装预检退出1（MSM策略写入报错及文件系统92MB提示），不是已证明的候选执行失败。功能对照和两组取消均未执行，须先补齐W3，不能直接以W2R通过签发替换。板上五个上传文件按要求保留，root模式已恢复。\n'
(E/'W4_INPUTS.md').write_text(w4)
(E/'STATUS.md').write_text('# 当前状态\n\n更新：'+datetime.datetime.now().astimezone().isoformat()+'\n\n| 项 | 状态 | 证据 |\n|---|---|---|\n'+'\n'.join(rows)+'\n\n全表与限制见FINAL_RESULT.md。\n')
print(status,boardstatus,'ORIGINAL_INPUTS_UNCHANGED',len(identities))
