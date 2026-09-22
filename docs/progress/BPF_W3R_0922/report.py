"""Generate reports from recorded observations, without changing earlier evidence."""
import datetime,difflib,hashlib,json,math
from pathlib import Path
P=Path.cwd();E=P/'progress/BPF_W3R_0922';B=E/'board-test'
def j(p):return json.loads(p.read_text())
def numbered(path,start,end):
 lines=path.read_text().splitlines();return '\n'.join(f'{i}: {lines[i-1]}' for i in range(start,end+1))
mounts=(B/'mounts-before.txt').read_text().splitlines();resolved={'/':'/','/etc':'/etc','/usr':'/usr','/opt':'/opt','/tmp':'/tmp','/var/tmp':'/opt/var/tmp'}
mt=[]
for original,r in resolved.items():
 m=max((x.split() for x in mounts if r==x.split()[1] or r.startswith(x.split()[1].rstrip('/')+'/')),key=lambda x:len(x[1]));mt.append(dict(path=original,resolved=r,source=m[0],mount=m[1],type=m[2],flags=m[3]))
(E/'MOUNTS.tsv').write_text('path\tresolved\tsource\tmount\ttype\tflags\n'+''.join('\t'.join(x.values())+'\n' for x in mt))
source='https://github.com/rpm-software-management/rpm/blob/bfee1410af51c1cc9724791fb8d985260a62102b/lib/'
origin='''# 一、正常安装预检阻断归因

**已确认的环境条件**：上传目录 `/var/tmp/bpf_static_0922/` 实际为 `/opt/var/tmp/bpf_static_0922/`，位于 `/dev/mmcblk0p3` 的 **rw ext4**；安装目标 `/usr/bin` 与策略文件 `/etc/device-sec-policy` 则位于 `/dev/mmcblk0p2` 的 **ro ext4 根分区**。上传成功与安装预检失败并不矛盾。

| 核对项 | 观测与结论 | 依据 |
|---|---|---|
| 三重身份 | rpi4内核、armv7l、Tizen ID均匹配；不按IP识别 | board-read/IDENTITY.json；board-test/kernel、machine、os.transport.out |
| 根分区空闲 | df仍报1,393,876 KiB；只读并不使df物理空闲变零 | board-root-read/df.out、mounts.out |
| RPM可用块数 | 固定源码在ST_RDONLY时将bavail设为0 | upstream-transaction.c:164 |
| 报错条件 | adj_fs_blocks(bneeded)>bavail，增加RPMPROB_DISKSPACE | upstream-transaction.c:269–272 |
| 显示消息 | needs…filesystem来自RPMPROB_DISKSPACE，向上取整到MiB，文案为MB | upstream-rpmprob.c:143–151 |
| 92MB与Size | Size=90,882,270字节，约86.672MiB；不等于92MiB=96,468,992字节 | board-root-read/rpm-info.out、rpm-size.out |
| 原因匹配 | 已确认只读挂载符合固定源码“按0可用块计算”的条件；不是df所示物理空间不足 | 上述观测+源码 |
| MSM附带报错 | /etc同属ro根分区，不能写入；本轮未追踪MSM内部返回路径，不另行断言插件缺陷 | board-root-read/resolve.out；前轮INSTALL_PREFLIGHT.json |

`adj_fs_blocks` 在 transaction.c:68 对预计用量乘21/20（5%余量），再按文件系统块计数；消息再向上取整。因而92MB不是包头Size的原样打印。按普通文件向上取4096字节块、加5%及MiB向上取整可得到92的量级；这是算术旁证，未声称读取到当时DSI内存值。

## 版本与源码身份边界

任务描述中的“同一份rpm 4.14.1.1源码”需修正：ARM_RPM_DIAG实际固定的是上游 **rpm-4.14.1** 提交 `bfee1410af51c1cc9724791fb8d985260a62102b`，旧INTERIM_RESULT.md:49已注明并非部署Tizen源码的完整身份证明。板上 `rpm --version` 实测也是4.14.1。本轮继续用同一提交，不冒充另外的4.14.1.1 tag；未证明部署二进制与该源码逐字节对应。归因的“已确认”指实际ro条件与指定源码条件对上，不是本轮插桩读到了dsi->bavail。没有重跑安装预检。

## 挂载映射

| 路径 | 实际路径 | 挂载源 / 挂载点 | 类型 | ro/rw |
|---|---|---|---|---|
'''
for x in mt:origin+=f"| {x['path']} | {x['resolved']} | {x['source']} / {x['mount']} | {x['type']} | {x['flags'].split(',')[0]} |\n"
for title,file,a,b in [('DSI初始化','upstream-transaction.c',110,117),('统计与只读处理','upstream-transaction.c',157,170),('问题判定','upstream-transaction.c',260,274),('消息类型与显示单位','upstream-rpmprob.c',143,151),('额外5%余量','upstream-transaction.c',68,68)]:
 origin+=f'\n### {title}\n\n来源：[固定源码]({source+file.removeprefix("upstream-")}#L{a})；本地SHA `{hashlib.sha256((E/file).read_bytes()).hexdigest()}`。\n\n```c\n{numbered(E/file,a,b)}\n```\n'
(E/'INSTALL_CAUSE.md').write_text(origin)
timings=j(E/'history/TIMINGS.json');elf=j(E/'build-audit/ELF_COMPARISON.json')
build='''# 二、构建时长与对象来源核对（未起构建）

**只读纪律违反：** 提取.text使用的llvm-objcopy漏写独立输出ELF，改写了四份已有tmp主包解包副本；发现后未恢复、未再用它们作输入。详见INCIDENT.md与INPUT_ELF_FINAL.json。随后仅在内存从未改变的原RPM读取ELF与.text，复核下表原始SHA和.text比较全部成立（RPM_READONLY_CHECK.json）。这不消除操作违规；四份副本待人工批准恢复。

| 架构/轮次 | 开始 → 结束（+08:00） | 耗时 | 进度公告 |
|---|---|---|---|
'''
for x in timings:build+=f"| {x['arch']} / {x['round']} | {x['start']} → {x['end']} | {x['seconds']:.3f}s（{x['seconds']/60:.2f}min） | {x['announcements']} |\n"
build+='''
**应纠正一处人工侧表述**：ARM确为约119.17分钟降至10.02分钟；aarch64上一轮实际为13.37分钟，本轮10.00分钟，并非同样从约两小时缩短。

两架构各轮840是guard统计的进度公告数，不等于840份独立目标文件。本次另抽取以百分比开头的Building/Linking记录，各274行，两轮逐行相同；history/*-targets.diff为空。既有840计数未改，也不把274与840混用。

## 编译器与缓存

| 项 | 实测/静态核查 | 限制 |
|---|---|---|
| ARM clang/clang++软链接 | /usr/bin与/bin的target-prefixed入口均解析到/usr/bin/clang-22；file为ARM ELF32 | build-audit/root-tools-armv7l.out |
| aarch64入口 | 解析到/usr/bin/clang-22；file为AArch64 ELF64 | build-audit/root-tools-aarch64.out |
| ccache命令 | 两个只读隔离根的PATH中未找到 | 不宣称全宿主所有位置不存在 |
| 四个指定缓存目录 | /home/abuild/.ccache、/root/.ccache、/var/tmp/ccache、$HOME/.cache/ccache均ABSENT；本次HOME=/home/toolchain | 当前根观测，不倒填历史状态；命中统计NOT_OBSERVED，不写0命中 |
| CMake launcher | 两轮两架构main/static CMakeCache中均无CMAKE_CXX_COMPILER_LAUNCHER/ccache条目 | 所查Cache原始输出见build-audit/*-cache-* |
| 旧ARM执行记录 | scheduler为/usr/bin/qemu-arm-static /bin/make等 | history/BPF_W1_0921-armv7l-scheduler.json |
| 新ARM执行记录 | 出现直接/bin/make，以及/usr/bin/qemu-arm binfmt…形式 | history/BPF_STATIC_0922-armv7l-scheduler.json |
| 当前原生加速 | /emul/usr/bin/clang-22为x86-64 ELF、Clang22.1.8；仅--version的只读execve trace实测转至/emul//usr/bin/clang-22 | compiler-read/armv7l.trace:3；不是重跑构建，也不当作历史逐目标证据 |
| 当前QEMU | 输入根为Tizen/Tools QEMU9.2.3，/usr/bin/qemu-arm-static为qemu-arm软链接；宿主同名文件为另一个x86静态ELF | history/*-emul.out；raw/013、012 |
| 历史其它负载 | NOT_OBSERVED：找到的全机进程审计为21:03、21:26，早于ARM22:56开始；小时记录只有本任务进度 | 不把本任务cgroup用量当全机负载；缺同期连续CPU/进程记录 |
| I/O暂停 | 两轮各有成功的探测，均无pause记录 | history/TIMINGS.json，不能据单次探测断言全程无等待 |

**对象来源一句话**：记录支持本轮对象来自新解包源码树内执行的编译/链接流程（%prep删除旧树、解包，随后Building/Linking及命令记录）；在所查路径未观测ccache，当前入口实测能转原生Clang加速。**但缺历史逐编译进程映像及缓存统计，无法把ARM约12倍提速唯一归因于某一种因素，确切原因仍为NOT_OBSERVED。** 没有为了补证而重构、改配置或清缓存。

新版任务log中的%prep及编译记录来源：../BPF_STATIC_0922/cells/bpf-armv7l-libcxx/build.log:4–17及后续Building行。原生加速存在与观察到的入口变化提供解释线索，不等同于本轮已经完成同环境性能归因实验。

## 两轮主包身份

| 架构/轮次 | 主包ELF SHA256 | .text SHA256 |
|---|---|---|
'''
for x in elf:build+=f"| {x['arch']} / {x['round']} | `{x['sha256']}` | `{x['text_sha256']}` |\n"
build+='''
**ARM两项均不同；aarch64两项均相同。** 这不是static ELF的SHA比较。路径、提取.text的实际llvm-objcopy命令及退出码见build-audit/commands.json、ELF_COMPARISON.json。ARM差异未归因，不能以编译目标列表相同推断字节等价，也不能仅凭不同判定功能回归。留W4人工审阅；未修改主包或配方。
'''
(E/'BUILD_TIMING.md').write_text(build)
pairs=j(B/'pairs.json');cancel=j(B/'cancel_summary.json');cl=j(B/'CLEANUP.json')
board='''# 三、物理板可写区对照

工作目录/空间通过：`/var/tmp/bpf_static_0922/` → `/opt/var/tmp/bpf_static_0922/`，可写ext4；精确可用字节见board-test/WORK_DIRECTORY.json，超过500MiB门槛。板端没有cpio，使用已存在ARM构建根的cpio，仅传入本目录并记录源/目标SHA（board-test/transfers.json）；未装系统工具。以rpm2cpio管道解包到新extract/，开启pipefail，实际命令/返回值见commands.json。没有rpm安装、--nodeps、remount或修改MSM。

RPM SHA `672279ea1e93f896c6b82cd1d1b5d557ef6c8a8dcc9079793b2a2efe11f9e756`；解出候选SHA `6e110b9c61a592539e99f5fbfb619211a261552d4bff0de2a7040a39cb4bc1ba`；原件SHA `ade7530b3824dd83eebba837928d3919dcb4cd7708212a8607c3629cb5c55f5b`，均实测相符。仅在工作目录中执行，TMPDIR/HOME/XDG_CACHE_HOME均指该目录；禁止core，单进程有20秒上限，未触发超时。

| 项 | 候选rc / 秒 | 原件rc / 秒 | 结果 | stdout/stderr（board-test/内） |
|---|---|---|---|---|
'''
for x in pairs:
 a=x['candidate'];b=x['original'];board+=f"| {x['name']} | {a['exitcode']} / {a['seconds']:.3f} | {b['exitcode']} / {b['seconds']:.3f} | 双方退出0 | {a['stdout_file']}、{a['stderr_file']}；{b['stdout_file']}、{b['stderr_file']} |\n"
board+='''
耗时为宿主单调时钟包围一次远端调用测得，含SDB调用开销；不包含随后pull输出文件的时间。成对顺序均为候选→原件，参数完全相同，仅执行路径不同。

- --version：均为bpftrace v0.24.2。
- --info：逐行diff仅LLVM 21.1.1→22.1.8；build项与能力项其余行相同，详见info.diff。
- -l：完整输出均保留，选择实际候选列表中的 `tracepoint:sched:sched_switch`，脚本为 `tracepoint:sched:sched_switch { exit(); }`。前50行附后。
- BEGIN脚本各3次均输出ok；选中tracepoint脚本各3次、interval计时退出脚本各3次均退出0。只证明这三个脚本在这块板、此内核的覆盖，不声称所有探针和内置函数均测过。
- 没有候选独有失败，也没有双方失败而删项的情况。

## 取消探针：原样二进制、原断言

| 形态 | 执行数 | 原断言通过 | 退出码 | 结论 |
|---|---:|---:|---|---|
'''
for k,x in cancel.items():board+=f"| {k} | {x['executed']} | {x['passed']}/{x['executed']} | {sorted(set(x['exitcodes']))} | 本轮物理板实测 |\n"
board+='''
四种wait模式各5次。逐次要求wait_confirmed、恰好一次cleanup、cancel_rc=0/join_rc=0/canceled=1/cleanup_count=1；stdout及判断结果见cancel.json。二进制身份沿用../BPF_W1_0921/W3_REFERENCE_INPUTS.json，推前推后SHA核对在transfers.json。

**静态libgcc对照也通过，不按此前x86_64结果预设ARM崩溃。** 因而不能把x86_64的20/20崩推广为所有架构必崩。本轮不追加机制归因，也不据此改回候选的共享libgcc选择。

## 收尾

所有实际测试通过后，已先记录工作目录每个普通文件SHA（preserved-shas.transport.out），再删除该目录文件和空目录。随后独立只读复查目录不存在（raw/020，TASK_DIRECTORY_ABSENT_RC=0）。这是用户允许的本任务目录清理，板上副本已删除；本地原RPM/ELF/探针仍保留，输出已拉回，可重新部署。

rpm查询仍无bpftrace安装；/usr/bin/bpftrace*仍ABSENT；本轮起止/proc/mounts全文逐字节一致；SDB root恢复装前关闭状态。见CLEANUP.json、MOUNTS_COMPARISON.json、ROOT_RESTORED.json。不宣称整个系统所有运行状态都未改变（BPF脚本本来会短暂挂接探针），确认的是未安装包、未改指定系统文件与挂载。

**rpm安装验证留待镜像阶段**：本轮绕开的是安装这个测试步骤，不是绕过RPM门禁执行安装；没有修改安全策略或只读属性。W4需据此前提审阅，不写“安装已通过”。

## -l前50行（完整清单在board-test/list-*.out）
'''
for label in ('candidate','original'):board+='\n### '+label+'\n\n```text\n'+(B/('list-'+label+'-first50.txt')).read_text()+'```\n'
(E/'BOARD_RESULT.md').write_text(board)
final='''# W3R最终结果

**需先申报操作失误：** 本轮llvm-objcopy提取.text时漏指定独立输出ELF，实际改写了四份既有tmp主包解包副本，违反第二部分只读要求。已停止使用、未擅自恢复。原RPM校验未变，随后内存只读解析复核原ELF/.text数据相符；本轮板测static ELF不在受影响四份之内。详见[INCIDENT.md](INCIDENT.md)。任务不能标为无违规完成。

可写区物理板验证通过；此前正常安装预检的只读根分区条件已确认并与固定RPM源码对上。构建时长完成核对，但ARM提速的唯一原因与主包.text差异仍未归因；不得把原生加速线索写成已完成的历史性能归因。**rpm安装验证留待镜像阶段**。本轮不构建、不改spec、不推包仓、不起QuickBuild。

## 一、预检归因

| 项 | 结论 | 证据 |
|---|---|---|
| 上传目录 | /var/tmp→/opt/var/tmp，/dev/mmcblk0p3 rw ext4 | MOUNTS.tsv |
| 安装目标 | /usr、/etc属/dev/mmcblk0p2，ro ext4 | board-root-read/mounts.out |
| df与92MB | 物理空闲不等于RPM可写可用空间；只读时RPM按0块计算 | transaction.c:164、269–272；rpmprob.c:143–151，完整原文见INSTALL_CAUSE.md |
| Size旁证 | 90,882,270字节≠92MiB；5%余量与取整不能省略 | INSTALL_CAUSE.md |
| 源码版本边界 | 实为固定上游4.14.1提交；不冒称部署4.14.1.1源码身份证明 | INSTALL_CAUSE.md |

## 二、构建时长

| 项 | 结论 | 证据 |
|---|---|---|
| ARM两轮 | 119.17min→10.02min，840进度公告相同 | history/TIMINGS.json |
| aarch64两轮 | 13.37min→10.00min，并非两小时→十分钟 | 同上 |
| ccache | 指定目录与PATH未找到，命中数NOT_OBSERVED | build-audit/ |
| 当前原生加速 | 只读--version确实execve至/emul原生Clang；非历史逐目标证明 | compiler-read/ |
| 主包身份 | ARM ELF/.text均不同；aarch64均相同 | BUILD_TIMING.md |
| 完整提速归因/同期其它负载 | NOT_OBSERVED；缺历史编译进程映像、同期连续负载/缓存证据 | BUILD_TIMING.md |

## 三、物理板成对结果与取消

| 项 | 候选/共享 | 原件/静态对照 | 结论 |
|---|---|---|---|
| RPM解包ELF身份 | 匹配指定6e110b9c… | 原件ade7530b…匹配 | PASS |
| --version / --info / -l | 各退出0 | 各退出0 | PASS；info仅LLVM版本不同 |
| BEGIN / tracepoint / interval | 各3/3退出0 | 各3/3退出0 | 已覆盖用例等价 |
| 取消探针 | 共享20/20通过 | 静态20/20通过 | 实际数据；不外推x86_64结果 |
| 清理/系统状态 | 工作目录已删、无安装、/usr/bin/bpftrace*不存在 | 起止挂载一致、root已关闭 | PASS |
| RPM安装 | NOT_OBSERVED（本轮禁止） | — | 留待镜像阶段 |

逐项命令、退出码、stdout/stderr、耗时与前50行见 [BOARD_RESULT.md](BOARD_RESULT.md)。推送前只提交本轮证据和LINE_STATUS，不提交大二进制。方法选择与疑问见 [DECISIONS.md](DECISIONS.md)。W4输入沿用上一轮六份RPM及配方，不重新产出或替换；本轮板测是其中ARM static RPM的实际解包载荷。
'''
(E/'FINAL_RESULT.md').write_text(final)
(E/'STATUS.md').write_text('# 状态\n\n'+datetime.datetime.now().astimezone().isoformat()+'\n\n| 部分 | 状态 | 挂账 |\n|---|---|---|\n| 只读纪律 | VIOLATION | 四份tmp主包副本被objcopy改写，未恢复；见INCIDENT.md |\n| 安装预检归因 | 已确认ro条件匹配 | 源码部署身份证明边界见报告 |\n| 构建时长 | 核对完成，归因PARTIAL | ARM提速唯一原因与.text差异NOT_OBSERVED |\n| 可写区物理板验证 | PASS | RPM安装留镜像阶段 |\n| 板端收尾 | PASS | 目录已删除、挂载不变、root已关闭 |\n')
(E/'W4_INPUTS.md').write_text('''# W4输入

- 配方及diff：../BPF_STATIC_0922/recipe/、SPEC_DIFF.patch；原spec未动。
- 两架构六份RPM的实际路径/SHA/大小：../BPF_STATIC_0922/RPM_INVENTORY.tsv；本轮未重新写包。
- ARM static RPM：`tmp/BPF_STATIC_0922/rpm-bpf-armv7l-libcxx/RPMS/armv7l/bpftrace-static-0.24.2-1.armv7l.rpm`，SHA `672279ea1e93f896c6b82cd1d1b5d557ef6c8a8dcc9079793b2a2efe11f9e756`。
- ARM候选ELF：该RPM解出，SHA `6e110b9c61a592539e99f5fbfb619211a261552d4bff0de2a7040a39cb4bc1ba`。
- ELF依赖和命名空间：前轮verify-armv7l/result.json、verify-aarch64/result.json；本轮验证载荷身份未变。
- 新板测：BOARD_RESULT.md、board-test/pairs.json与cancel_summary.json；12组成对调用退出0、两组取消各20/20通过。
- **rpm安装验证留待镜像阶段**；不能把工作目录执行通过写成RPM安装或%post已验证。
- `-lstdc++fs`既有选择仍由W4裁决，详见前轮STDCXXFS_RESOLUTION.json。
- 新增挂账：ARM主包ELF与.text相比W1均不同；构建时长完整归因尚未取得；静态libgcc对照ARM通过，不外推x86_64崩溃。
- 板上工作目录已按全通过规则清理；需后续验证时从上述本地RPM重新部署，不能引用已删除板端路径作为现存输入。
- 下一步由人工W4审阅签字后决定配方及QuickBuild，不自动推sandbox、不起QuickBuild。
- **先处理只读违规挂账**：四份主包解包副本被本轮objcopy改写，路径与前后SHA在INCIDENT.md/INPUT_ELF_FINAL.json；不得当作原始ELF继续使用。原RPM仍可校验，恢复这些副本需人工批准，本轮未恢复；板测static不在这四份之内。
''')
print('REPORTS_GENERATED')
