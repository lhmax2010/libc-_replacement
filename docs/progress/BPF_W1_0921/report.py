"""Render current observed results without upgrading unobserved or failed gates."""
import csv,datetime,json,subprocess
from pathlib import Path
p=Path.cwd(); out=p/'progress/BPF_W1_0921'
subprocess.run(['/usr/bin/python3',str(out/'status.py')],check=True)
matrix=json.loads((out/'MATRIX.json').read_text()); packages=[]; binaries=[]; sections=[]
for row in matrix:
    label=f"bpf-{row['arch']}-{row['path']}"; f=out/f'verify-{label}/result.json'
    if not f.exists(): continue
    d=json.loads(f.read_text()); obs=d['observations']
    for r in d['rpms']:
        packages.append(dict(arch=row['arch'],path=row['path'],rpm=r['path'],sha256=r['sha256'],bytes=r['bytes'],payload=r['payload'],file_records=r['file_count']))
    for kind in ('main','static','original'):
        b=obs[kind]; markers=obs.get(kind+'_markers',{})
        binaries.append(dict(arch=row['arch'],path=row['path'],kind=kind,file=b['path'],sha256=b['sha256'],bytes=b['bytes'],DT_NEEDED=','.join(b['needed']),std1=markers.get('std1','NOT_OBSERVED'),cxx11=markers.get('cxx11','NOT_OBSERVED'),cxx11_broad=markers.get('cxx11_broad','NOT_OBSERVED')))
    sections += [f"## {row['arch']} / {row['path']}", '', f"实际写出 {len(d['rpms'])} 个 RPM，摘要校验已执行。综合状态：`{row['status']}`。",'',f"证据：`verify-{label}/result.json`、`verify-{label}/commands.json`、`verify-{label}/all-main-elfs.json`、`audit-{label}/result.json`、`cells/{label}/build.log`。",'', '| 核验项 | 结果 |','|---|---|']
    sections += [f"| {k} | {'PASS' if v else '未闭合'} |" for k,v in d['checks'].items()]
    sections += ['',f"候选 static ELF {obs['static']['bytes']} 字节；原资产 {obs['original']['bytes']} 字节。",f"static 的 DT_NEEDED：`{', '.join(obs['static']['needed'])}`。",f"Source1002 中本架构普通文件 {len(obs['source1002']['arch_regular_files'])} 个；static RPM 解包后普通文件 {len(obs['static_rpm_regular_files'])} 个，完整列表在 result.json，计数不混用 RPM 文件记录（其还可能含目录）。",'']
def table(name,rows):
    if not rows: return
    with (out/name).open('w',newline='') as f:
        w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t'); w.writeheader(); w.writerows(rows)
table('RPM_INVENTORY.tsv',packages); table('ELF_COMPARISON.tsv',binaries)
lines=['# 本轮写包与载荷核验','',f"更新时间：{datetime.datetime.now().astimezone().isoformat()}",'','## 范围与事实边界','','只使用已有候选 spec，不改上游 C++、不新增 spec 改动、不推包仓、不覆盖 Source1002、不上板。编译线项目分支为 codex/r5-ehabi-diagnosis。','', '对账材料原样推送的远端提交为 `c7aa2c5e72f78516e40b7a5423a4b83673608bad`。','', '六格是两架构 × 三路径；现有 spec 每格写 main/common/static 三份 RPM。`RPM_INVENTORY.tsv` 列每份 RPM 的完整 SHA256、大小、压缩格式，`ELF_COMPARISON.tsv` 列 ELF 依赖与字符串痕迹。','',f"截至本次汇总，已核验本轮 RPM {len(packages)} 份；未执行项明确留在 STATUS.md 中。",'', 'RPM 摘要通过、整轮写包通过、静态候选验收通过是三件事。aarch64 libc++ 产物额外依赖 libbpf.so.1 的缺口不会被写包退出 0 覆盖。','', '所有源码/工具脚本执行记录均在 cells、raw 与命令 JSON 中；脚本快照和 SHA256 随记录保存。','',*sections,'## 判断与未闭合项','','- ARM 新旧同 NEVRA 的输入替换需要人工确认；当前进展以 INPUT_STATUS.md 与事务原始退出码为准，不能由拟安装清单推定已安装。','- 既有 libbpf 动态依赖问题未授权修改，本轮只记录实际产物。','- 精确字符串前缀与扩展检索分别报告，零命中不等于原资产不含某套运行时；尤其原 aarch64 资产的扩展检索已有命中。','- 功能等价、取消探针、物理板安装均未在本轮执行。','- 本轮尚不能据写包结果授权替换发布资产或启动 QuickBuild；人工已明确后者等 static 子包问题解决。']
(out/'RESULT.md').write_text('\n'.join(lines)+'\n')
w3=['# W3 输入交接（仅清单，不执行上板）','','必须先由人工审核本轮未闭合项。以下路径只是可定位的本轮产物，不代表已获上板或发布许可。','']
arm=[r for r in packages if r['arch']=='armv7l']
if not any(r['path']=='libcxx' for r in arm):
    w3+=['**armv7l libc++ 新候选主包/static：NOT_AVAILABLE。回退路径 RPM 即使写出，也只作为原资产对照，不能替代新候选输入。**','']
if not arm: w3+=['armv7l 本轮 RPM：NOT_AVAILABLE；本轮尚无已核验 ARM RPM，不以旧轮 18 个 LLVM 输入或旧 bpftrace RPM 冒充。']
for r in arm: w3 += [f"- {r['path']}: `{r['rpm']}`；SHA256 `{r['sha256']}`；{r['bytes']} 字节。"]
w3+=['','还需：同版本原资产对照、共享/静态展开器取消探针、板端依赖与内核/BPF 环境记录、传输前后 SHA256 对照；这些验证不在本轮执行。']
(out/'W3_INPUTS.md').write_text('\n'.join(w3)+'\n')
print('Observed result tables generated; no package or board actions.')
