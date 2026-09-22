"""Assemble evidence-backed reports without changing build inputs."""
import csv,difflib,hashlib,json,datetime
from pathlib import Path
P=Path.cwd(); E=P/'progress/BPF_LIBBPF_0922'; T=P/'tmp/BPF_LIBBPF_0922'
def sha(f):
    with f.open('rb') as s: return hashlib.file_digest(s,'sha256').hexdigest()
results={a:json.loads((E/f'verify-{a}/result.json').read_text()) for a in ('armv7l','aarch64')}
assert all(r['status']=='PASS' for r in results.values())
probe=json.loads((E/'probe/result.json').read_text()); assert probe['passed']==probe['total']==20
identity=[]
for row in json.loads((E/'original-specs.json').read_text()):
    actual=sha(Path(row['path'])); identity.append(dict(**row,actual=actual,match=actual==row['sha256']))
prior=list(csv.DictReader((P/'docs/progress/BPF_W1_0921/ELF_COMPARISON.tsv').open(),delimiter='\t'))
old={}
for a,r in results.items():
    old[a]={}
    for kind in ('main','static','original'):
        row=next(x for x in prior if x['arch']==a and x['path']=='libcxx' and x['kind']==kind)
        actual=sha(Path(row['file'])); identity.append(dict(path=row['file'],sha256=row['sha256'],actual=actual,match=actual==row['sha256']))
        old[a][kind]=row
    f=Path(r['libbpf_archive']['path']); actual=sha(f)
    identity.append(dict(path=str(f),sha256=r['libbpf_archive']['sha256'],actual=actual,match=actual==r['libbpf_archive']['sha256']))
    before=(E/f'snapshots/{a}-ast-CMakeLists.txt').read_text().splitlines(True)
    after=(T/f'rpm-bpf-{a}-libcxx/BUILD/bpftrace-0.24.2/src/ast/CMakeLists.txt').read_text().splitlines(True)
    patch=''.join(difflib.unified_diff(before,after,fromfile='a/src/ast/CMakeLists.txt',tofile='b/src/ast/CMakeLists.txt'))
    (E/f'{a}-static-libbpf.patch').write_text(patch)
assert all(x['match'] for x in identity)
(E/'INPUT_IDENTITY_FINAL.json').write_text(json.dumps(dict(status='PASS',checks=identity),indent=2))
fields=['arch','bytes','sha256','DT_NEEDED','std1','cxx11','libbpf_defined_intersection','path']
with (E/'CANDIDATES.tsv').open('w') as f:
    w=csv.DictWriter(f,fieldnames=fields,delimiter='\t'); w.writeheader()
    for a,r in results.items():
        w.writerow(dict(arch=a,bytes=r['after']['bytes'],sha256=r['after']['sha256'],DT_NEEDED=','.join(r['after']['needed']),std1=r['markers']['std1'],cxx11=r['markers']['cxx11'],libbpf_defined_intersection=r['libbpf_archive']['symbol_intersection'],path=r['after']['path']))
rows='\n'.join(f"| {a} | PASS | {r['after']['bytes']:,} | {r['markers']['std1']:,} / 0 | {r['libbpf_archive']['symbol_intersection']} |" for a,r in results.items())
ids='\n'.join(f"- {a}: `{r['after']['sha256']}`" for a,r in results.items())
archives='\n'.join(f"- {a}: `libbpf-static-1.6.2-1.1.{a}`，`{r['libbpf_archive']['path']}`，SHA256 `{r['libbpf_archive']['sha256']}`。" for a,r in results.items())
sizes='\n'.join(f"| {a} | {old[a]['original']['bytes']} | {old[a]['static']['bytes']} | {r['after']['bytes']} | {r['after']['bytes']-int(old[a]['static']['bytes']):+d} |" for a,r in results.items())
report=f'''# bpftrace static：libbpf 静态链接核验

两架构候选的 **libbpf 链接缺口已闭合**：最终 ELF 均不再依赖 `libbpf.so.1`，只比原资产多预期的 `libgcc_s.so.1`。x86_64 同配置取消探针 20/20 通过。本轮仅修改 tmp 中两份 CMake 的同一行，重链接 static，未重编主包、未改原 spec、未推包仓、未覆盖 Source1002、未上板。

## 结果

| 架构 | ELF 验收 | 字节数（strip -g） | `_ZNSt3__1` / `_ZNSt7__cxx11` 行数 | libbpf 归档与候选的已定义符号交集 |
|---|---|---:|---:|---:|
{rows}

armv7l 的 DT_NEEDED 恰为 `libm.so.6, libgcc_s.so.1, libc.so.6, ld-linux.so.3`；aarch64 恰为 `libm.so.6, libgcc_s.so.1, libc.so.6, ld-linux-aarch64.so.1`。无动态 libc++、libLLVM、libbpf；较宽的 `St7__cxx11` / `std::__cxx11` 检索也为 0。这是本轮产物证据，不推及未测配置。

候选 SHA256：

{ids}

完整路径、大小与依赖：[CANDIDATES.tsv](CANDIDATES.tsv)；原始 readelf/nm/strings 命令及退出码：[ARM](verify-armv7l/commands.json)、[aarch64](verify-aarch64/commands.json)；逐项验收：[ARM](verify-armv7l/result.json)、[aarch64](verify-aarch64/result.json)。

## 为什么以前还依赖 libbpf.so.1

不是缺少静态归档。两架构输入根中均有 `.a`，实际 RPM 归属及 SHA256 见 `cells/input-*/build.log`：

{archives}

缓存已经将 `LIBBPF_LIBRARIES` 指向对应 `/usr/lib[/64]/libbpf.a`。但本地 `src/ast/CMakeLists.txt:86` 的 `STATIC_LINKING` → `LIBCLANG_STATIC_PATH` 扩展又写了裸库名：

```cmake
target_link_libraries(ast PUBLIC ${{LIBCLANG_STATIC_ABS_PATH}} bpf clangIndex clangDriver clangFrontend clangCodeGen)
```

这让实际 driver 行同时含绝对路径 `libbpf.a` 与 `-lbpf`。原 ARM ELF 的动态未定义项包括 `bpf_linker__add_file/finalize/free/new@LIBBPF_0.4.0`（raw/076 输出），不能说原来所有 libbpf 代码都动态链接；可确认的是仍有动态边界。修正为：

```cmake
target_link_libraries(ast PUBLIC ${{LIBCLANG_STATIC_ABS_PATH}} ${{LIBBPF_LIBRARIES}} clangIndex clangDriver clangFrontend clangCodeGen)
```

这复用已经解析为 `.a` 的 CMake 变量，不新建开关。两个副本 diff：[ARM](armv7l-static-libbpf.patch)、[aarch64](aarch64-static-libbpf.patch)。没有新增 libbpf 构建或改其 spec。

## 上游 0.24.2 的实际做法

[根 CMakeLists.txt](https://github.com/bpftrace/bpftrace/blob/v0.24.2/CMakeLists.txt#L81) 在 STATIC_LINKING 时把 `CMAKE_FIND_LIBRARY_SUFFIXES` 设为 `.a`；[FindLibBpf.cmake](https://github.com/bpftrace/bpftrace/blob/v0.24.2/cmake/FindLibBpf.cmake#L23) 通过 `find_library` 设置 `LIBBPF_LIBRARIES`；[src/CMakeLists.txt](https://github.com/bpftrace/bpftrace/blob/v0.24.2/src/CMakeLists.txt#L98) 使用该变量。上游 [src/ast/CMakeLists.txt](https://github.com/bpftrace/bpftrace/blob/v0.24.2/src/ast/CMakeLists.txt#L81) 没有本地 `LIBCLANG_STATIC_PATH` 加裸 `bpf` 的扩展。下载原文与哈希在 `upstream/`、raw/033，差异在 raw/057；未猜测该扩展的作者或引入时间。

## 实际链接命令与构建边界

改前在独立副本中实际执行 `cmake -E cmake_link_script ... --verbose=1`，改后实际执行 `cmake --build .../build-static --target bpftrace --verbose -- -j1`，两架构前后均退出 0。不是只读 link.txt 充当执行记录。

- ARM 完整 driver 原文：[改前](verify-armv7l/link-before.txt)、[改后](verify-armv7l/link-after.txt)、[全部 token 计数差异](verify-armv7l/link-token-diff.json)。
- aarch64 完整 driver 原文：[改前](verify-aarch64/link-before.txt)、[改后](verify-aarch64/link-after.txt)、[全部 token 计数差异](verify-aarch64/link-token-diff.json)。

CMake 重生成还会消除原链接行重复的 libbpf.a/libbcc.a/libbcc_bpf.a 项，不能声称命令行只有一个 token 变化。人工修改仅上述一行；`-shared-libgcc` 保留，`-static-libgcc` 不存在。两架构改后 C/CXX 编译目标公告均为 {results['armv7l']['static_compilation_announcements']} / {results['aarch64']['static_compilation_announcements']}，只进行了 static 重链接。两套保护清单分别 {results['armv7l']['protected_files']} / {results['aarch64']['protected_files']} 项哈希全部一致（含主包构建目录、未改源文件和原副本 CMake）；收尾额外 {len(identity)} 项原 spec、原 ELF 与归档身份核对通过。

原 spec 已有 `_toolchain` 已定义且 clang 才生成 static 的分支。此次局部 CMake 改动只在 STATIC_LINKING 且自定义 libclang 分支内生效；主包 STATIC_LINKING=OFF 不受该行控制。GCC/未定义路径继续安装 Source1002。本轮未重跑这四格，引用前轮 `BPF_W1_0921` 的实测，不把静态核查冒充重测。W4 纳入配方仍需保留 libc++ 条件。

两架构隔离环境 payload 预检实测 `w5.xzdio`（`cells/input-*/build.log`）。**本轮输出是 ELF，不是新 RPM；ELF 本身没有 payload 压缩属性**。既有六格 RPM 不被改写，不能拿旧 RPM 声称已安装本轮新 ELF。是否追加 static-only 写包已询问；未获得确认，未自行扩展为整包重构建。

## 体积对照

| 架构 | 原 Source1002 ELF | 上轮 libc++ static ELF | 本轮 ELF | 较上轮变化 |
|---|---:|---:|---:|---:|
{sizes}

原资产和上轮数字及 SHA 取自 `docs/progress/BPF_W1_0921/ELF_COMPARISON.tsv`，本轮重新校验对应文件身份，见 `INPUT_IDENTITY_FINAL.json`。本轮 strip 使用正式 brp 的 `-g`，不是全剥离；这不是与原资产剥离策略完全等价的证明。每架构只交一个最终 ELF；未剥离与中间全剥离文件保留在 tmp，不冒充最终候选。

## 取消探针

原样使用既有 `condition_cancel_probe.cpp`，源码 SHA `{probe['source_sha256']}`。同套 x86_64 libc++/libc++abi 静态归档、共享 libgcc，附加 `--whole-archive libbpf.a --no-whole-archive`，确保 libbpf 代码实际进入探针；nm 确认 `bpf_object__open` 为已定义符号。四种等待形态各 5 次，20/20 取消穿过、清理恰好一次、join 取得取消值。源文件、编译器、归档身份、完整编译与运行命令/退出码见 `probe/`。

探针依赖 libatomic、libpthread、libm、libgcc_s、libc，不是 bpftrace 候选的精确四依赖契约；该实验仅证明已测 x86_64 配置未重现原静态展开器失败，不能证明 ARM 板上行为或所有取消场景。未运行板子。

## 资源与交接

开工 2026-09-22 13:04 +08，硬截止 17:04。实际内存 cgroup 上限 16536457216 字节、nice19、ionice idle、串行构建；已存在 Makefiles，使用 `-j1`，未改成 Ninja。实际 `/proc/PID/cgroup` 与 `memory.max` 在各 cell events。开工磁盘153GiB，I/O 探测0.175秒；aarch64 前151GiB、0.209秒，无 I/O 暂停。构建目标不足500，没有触发该阈值探测。脚本执行快照及SHA保留。超大 ELF、复制树和完整 nm/strings 输出留 tmp，报告附路径与SHA，不提交平台二进制。

上板输入与缺口见 [W3_INPUTS.md](W3_INPUTS.md)。修法、链接与本轮 ELF 门禁通过，**发布替换仍须新资产写包/安装验证、功能等价和 ARM 物理板验证以及人工审阅**。本轮不启动 QuickBuild。

自行判断和技术性异常完整记录于 [DECISIONS.md](DECISIONS.md)：包括 strip 策略校正、曾在链接未完成时提前读取 ELF 的技术性非零、CMake 去重与 probe 强制保留归档。本轮未把这些记作构建失败。未修订原判定。
'''
(E/'FINAL_RESULT.md').write_text(report)
w3='# W3 上板输入更新（未上板）\n\n本轮两个最终 ELF 已闭合 libbpf 动态依赖；旧 RPM **不含**这些新文件。不能直接把上轮 RPM 当成本轮候选安装。\n\n'
for a,r in results.items():
    w3+=f"## {a}\n\n- 文件：`{r['after']['path']}`\n- 字节：{r['after']['bytes']}\n- SHA256：`{r['after']['sha256']}`\n- DT_NEEDED：`{', '.join(r['after']['needed'])}`\n- 原件对照：`{old[a]['original']['file']}`\n- 原件SHA256：`{old[a]['original']['sha256']}`\n- CMake改动：`{a}-static-libbpf.patch`；完整实际链接命令在 `verify-{a}/link-after.txt`。\n\n"
w3+='''## 仍需准备/验证

- 若采用 RPM 安装路径：先将本轮 ELF 纳入新的 static RPM，保留此前主包/common，核查安装脚本和传输前后 SHA；新 RPM 本轮为 NOT_OBSERVED。已向人工询问是否在本轮追加 static-only 写包，尚未收到答复。
- 原资产、旧 RPM 与旧板端依赖清单仍引用 `docs/progress/BPF_W1_0921/W3_INPUTS.md`，不得与新 ELF 身份混用。主包仍可能需要动态 libbpf，这是原有主包设计，不是本轮 static 失败。
- ARM 物理板取消、--version/--info/probe/脚本原件对照、安装/卸载清理：本轮 NOT_OBSERVED，待 W3 授权。x86_64 probe 20/20 不能代替 ARM 板测。
- 传输前再次核对 CANDIDATES.tsv 的 SHA；不得覆盖 Source1002，不推包仓。
'''
(E/'W3_INPUTS.md').write_text(w3)
(E/'STATUS.md').write_text(f'''# 本轮状态

开工：2026-09-22 13:04 +08；硬截止17:04。报告生成：{datetime.datetime.now().astimezone().isoformat()}。

| 项 | 状态 | 证据 |
|---|---|---|
| armv7l static ELF | PASS，已消除动态libbpf | verify-armv7l/result.json |
| aarch64 static ELF | PASS，已消除动态libbpf | verify-aarch64/result.json |
| x86_64取消探针 | PASS 20/20 | probe/result.json |
| 原spec/主包/来源资产 | 身份核对PASS | INPUT_IDENTITY_FINAL.json与verify-*/unchanged-files.json |
| 新static RPM/上板 | NOT_OBSERVED；本轮ELF不能冒充旧RPM内容 | W3_INPUTS.md |
| 包仓推送/Source1002覆盖 | 未执行 | 本轮限定 |

详见 FINAL_RESULT.md、DECISIONS.md。命令原文及退出码在raw与cells；前次开工只读命令申报保留在raw/080之前的状态读取及raw/082决策记录。
''')
(E/'README.md').write_text('# 阅读顺序\n\n1. FINAL_RESULT.md：根因、修法、结果与范围。\n2. CANDIDATES.tsv、两个static-libbpf.patch、verify-*/link-before/after.txt：逐项复核。\n3. W3_INPUTS.md：新ELF与旧RPM的区别、后续输入。\n4. DECISIONS.md、raw/、cells/、probe/：实际命令、退出码、资源、探针与脚本快照。\n5. upstream/：上游固定版本原文；PUBLICATION_MANIFEST.json：交付材料哈希。\n')
print('REPORTS_GENERATED; INPUT_IDENTITY_PASS',len(identity))
