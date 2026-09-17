# 续跑状态

起点 2026-09-17 08:48:19 +08，硬截止 22:48:19 +08。

| 项目 | 状态 | 证据 / 边界 |
| --- | --- | --- |
| 磁盘、资源、同一读写探针 | PASS | 001–004；251 GiB；medium=0；3,932,160 字节探针 0.760745 秒 |
| 五对象原生等价抽查 | 沿用 PASS | 上轮 native-equivalence-summary.json；不是逐目标证明 |
| 历史未上传日志补齐 | 完成并推送 | cc22d4c2296b974a029fa73758668c392c08e62b；269 个归档文件 |
| LLVM aarch64 完整 RPM | PASS | 14:17:21 最终完整 rpmbuild 返回 0；12 个 RPM 全部摘要校验通过；从 RPM 提取的 libclang.a 有 libc++ 痕迹，libLLVM 依赖 libc++、不依赖 libstdc++；见 AARCH64_RPM_RESULT.md |
| LLVM armv7l 静态开发 RPM | PARTIAL / 等待裁决 | 15:47:27 完整 rpmbuild 退出 1（cpio write failed / Function not implemented）；18 个已写出 RPM 单独校验通过，含 libclang.a；不将整轮记 PASS，不自动用于后续 |
| armv7l/aarch64 bpftrace 三路径 RPM | NOT_STARTED / 输入准备中止 | armv7l LLVM 完整写包失败待裁决；aarch64 私有输入根复制校验遇到 EPERM，未将部分复制输入用于构建 |
| armv7l 物理板验证 | NOT_STARTED（已做预检） | raw/222–226 开启板上 root 做只读能力检查，随后恢复 owner；raw/237 保存装包前清单。尚未上传探针、安装 RPM 或执行功能测试 |

## 本轮技术性处理与限制

1. 第一次启动使用上轮较低的内存值，严格相等校验退出 1，尚未启动构建。重新计算并用 16,536,461,312 字节启动。
2. 第二次运行在配置临时 Ninja 的 ionice 返回 `none: prio 0` 时主动中止，不能声称该次资源验收通过。随后独立探针复现活进程为 idle、退出未回收进程为 none。原 PID 当时的存活状态未被采集，故不能将这个解释确认为原 PID 的事实。
3. 新监控同时核对 PID 启动时间与存活状态；退出中进程仅记 NOT_OBSERVED，活 Ninja 仍严格要求 -j1、nice19、idle 和实际 memory.max。失败时仍停止，不放宽任何资源判据。
4. 两次旧脚本快照与旧构建输出均保留。新脚本 SHA256：c93f008a4a3dd23caa7fb21336636a662f190f647a7d5ade40bf04ec4eb84db9。
5. 每 500 目标 I/O 探测、慢时暂停十分钟并最多三次重试；硬截止线程独立发出停止信号。配置准备阶段另有读写复测均未超 30 秒。
6. 此处不把 CMake 可选特性的 Failed/not found 当作构建失败；也不把 CMake 检测 Ninja 的资源证明当作主 Ninja 的证明。

原始输出：raw/、events.jsonl、llvm-aarch64.log。每小时进度：HOURLY_STATUS.md。

## 主构建实测

CMake 配置完成：415.7 秒；生成后主构建实际开始。raw/055、056 记录主 Ninja PID 42296：`-j1 -j 1`，实际 cgroup 为 `.../build-static-0917-resume.scope`，memory.max=16536461312，nice=19，ionice=idle。raw/057 抓到实际 `/emul/usr/bin/aarch64-tizen-linux-gnu-clang++` 编译 LLVMCore/Support 对象命令含 `-stdlib=libc++`，不是只读取 CMake 意图。

当前 aarch64 对象由原生交叉工具集编译；正确性的工具切换依据仍仅是上轮五对象抽查，不作逐目标等价声明。14:17 已取得并核查 LLVM 完整 RPM，但 bpftrace RPM 与板上门禁尚未闭合，不能替换发布资产。

## 10:07 附记

第二个自动 I/O 检查点（1000 目标）通过：实际校验 0.168131 秒，退出 0。读取其摘要的 raw/065 因记录器 PATH 未包含 rg 退出 127；补入已经实际定位的 rg 目录后，raw/067 成功。旧记录器另存 run_logged.before_rg_fix.sh。只改报告记录器的 PATH，不改正在执行的构建脚本、编译参数或隔离根 PATH。

复核发现 raw/043 中进程筛选也受相同 PATH 问题影响；该复合命令最后的压力统计返回 0，不能把其空筛选结果作为“无进程”证据。原 stderr 保留；后续 raw/044、045 已直接查询 scope 和 PID，确认进程存在。本轮开工的 raw/004 为未经过滤的完整 ps 输出，不受此问题影响。

## 12:26 附记

- 已完成十次定期 I/O 探测（500 至 5000 目标），均退出 0，实际读写校验耗时均低于 1 秒；没有触发 I/O 暂停。
- 原 armv7l 构建树仍在，且构建目录归当前用户所有。原 spec 与候选的构建配置差异为条件内打开 LIBCLANG_BUILD_STATIC；另外有 VCS/Release 字段差异。尚未在该树上配置或构建，不将其当作已通过。
- 两份 llvm 源码 tar 的整体 SHA256 不同（raw/086），SOURCE_PROVENANCE 文本相同（raw/087）。随后逐条比较 184,839 个条目的类型、模式、大小、链接目标和常规文件 SHA256：独有条目 0、差异条目 0（raw/092；source_payload_comparison.json）。该比较忽略时间戳、所有者、归档顺序与压缩表示，证明输入有效载荷相同，不声称压缩包逐字节相同。
- 本轮没有新增 sudo、板上操作或包仓推送。只读检查中有个别历史路径猜测错误：原 arm spec 实际位于 SOURCES，CMake 目录为 3.31.2，历史报告文件为 FINAL_REPORT.md；均保留失败输出并定位重查。读取 raw/081 的退出码时误用 .exitcode.txt 后缀，实际文件为 .exitcode（内容 0），不是资源检查失败。

有效载荷检查第一次在 json.dumps 生成大字符串时触发其自设 512 MiB 虚拟内存上限（MemoryError，退出 1）。改成 json.dump 流式写文件后以相同上限重跑通过，耗时 15.25 秒。第一版脚本与输出保留；完整大清单放 tmp/STATIC_0917B/source-payload-audit，报告保留差异结论及清单 SHA256，不把约 80 MiB 的完整清单默认提交。LLVM 构建未被停止或修改。

## 13:44 记录器事故及处置

1. `events.jsonl` 明确记录首轮于 13:37:01.115 返回 0，完成 7546/7546。完整日志自动保留为 `llvm-aarch64.stopped-133701.log`，其中实际写出 12 个 RPM。
2. 13:37:01.279，同一外层记录进程又启动相同 Python 构建监控，监控脚本 SHA256 未变。重复执行到 99 个增量目标后进入打包；13:42:57 对该重复执行的 PGID 115778 发 TERM，退出 -15。外层 `raw/033_build.exitcode` 为 241，**不能把这个外层退出码写成 0**。首轮成功只引用单独的 finished 事件与首轮完整日志。
3. 本轮此前在长命令未结束时改写了正在运行的 Bash 记录器 PATH，这是不应做的。重复执行的直接观测成立；Bash 读取被改写脚本时的具体字节偏移机制尚未另作受控复现，不将其写成已实测机制。今后只用运行前固定的一次性 Python 记录器，任何修改另存新版本，不编辑正在执行的文件。
4. 重复执行已改写 llvm 与 llvm-devel 两个 RPM 文件，且现存集合因此混有两次执行。事后逐条计数进一步澄清：日志只确认 llvm 的 Wrote；llvm-devel 当时还在写入，不能称两个完整包。全部 12 个文件可恢复地移至 `tmp/STATIC_0917B/rpm-aarch64/RPMS_prior_mixed_1343`，不作为最终验收输入。移动前逐个 SHA256 与时间记录于 raw/113、112；未删除任何包或 Source1002。
5. 新记录器 `run_logged_v2.py` 只调用子进程一次。新一轮完整 rpmbuild（仍 --noprep -bb，无 short-circuit）于 13:44:23 启动，复用已经完成的编译树；raw/116 保存独立命令、输出与退出码。重新检查 medium=0（raw/114），memory.max 仍为 16536461312，nice19/idle、-j1 均不放宽。
6. 重复执行约 5 分 56 秒；有意重写包耗时约 32 分 58 秒。首轮结束到最终结束共额外约 40 分 20 秒（包括排查与准备），不能仅报编译用时。重复/最终两轮分别公告 99/98 个增量目标，各含 5 次对象编译公告，合计额外 10 次对象编译，而非 197 个重新编译的对象。14 小时截止仍为 22:48:19，不顺延。首轮打包期间 memory.events 的 oom/oom_kill 均为 0；补做 I/O 探测 1.358 秒通过（raw/106、107）。
7. 截至该附记的 13:44，armv7l 仅准备命令；随后于 14:22:59 启动。用显式原 .rpmrc 与 .rpmmacros 后，展开编译 flags 与保留树 CMAKE_CXX_FLAGS 逐字一致；未加载原 .rpmrc 的前置查询曾返回默认 flags，因此没有用那个不完整环境启动构建。--load 不被该 RPM 支持（raw/096），改用实际支持的 --rcfile/--macros（raw/101），不更改平台宏文件。

## 14:46 附记

- armv7l 123 个增量目标已经完成，正在完整写包；实际编译器仍是 QEMU 入口的目标 Clang，未把仅对 aarch64 获准的原生替换擅自外推到 armv7l。memory.max=16536461312、nice19/idle、Ninja -j1 已核查，当前 oom/oom_kill=0。
- OpenMP 安装的两条符号链接命令报只读文件系统。原 spec:382 将 buildroot 填入 CMAKE_INSTALL_PREFIX，而生成的 openmp/runtime/src/cmake_install.cmake:63–79 将链接工作目录写成 $ENV{DESTDIR}/usr/lib。该轮没有设置 DESTDIR，故被只读旧根阻止。原始输出保留；未擅自修平台打包行为，不以编译完成代替 RPM 检查。
- 为寻找历史脚本而对 tmp/NIGHT_0917 的文件列表检索耗时偏长；确认是本轮只读 rg PID 167188 后停止（raw/160 退出 0），改为检索具体 progress 目录。组合记录 raw/154 的最终退出 0 不表示其中被中止的 rg 完成了全目录扫描。
- 两架构 GCC 与未定义 _toolchain 宏路径已实际展开（bpf_mode_queries.json）：未定义路径确为 defined=0，通过显式指定 GCC 的通用编译器/归档器宏保留 GCC flags；与 GCC 路径 flags 相同，不含 libc++ 参数。这是测试环境定义，不改 spec，不是完整构建通过证明。
- sdb 初次列表为空，直接查询返回 target not found（raw/174=1）；显式 connect 成功（raw/175=0），随后只读板上预检完成（raw/177=0）。任务目录不存在，当前预检身份是 owner；未做板上安装/root 操作，尚无功能验证结论。

## 15:00 压缩并行度纠正

1. armv7l 首轮编译确为 -j1，但 RPM 使用原 .rpmmacros:17 的 `%_binary_payload w5T.xzdio`。14:58 日志实际显示 XZ 请求 20 线程、按内存预算降到 15。**这是本轮资源控制遗漏，不能声称整条流水线均为单并行**。
2. 立即停止该轮 PGID 164290（raw/198 退出 0）；监控/记录器最终退出 241（其构建子进程被 TERM，非正常成功）。原始 arm_llvm/ 日志保留。停止时有 6 个 RPM 文件，其中日志只确认 5 次 Wrote，不能当成完整集合。
3. 这 6 个文件带 SHA256 可恢复地移至 tmp/STATIC_0917B/rpm-arm-llvm/RPMS_stopped_parallel_1459，未删除，也不作为后续输入。
4. 按构建根 usr/lib/rpm/macros:403 的实际说明，命令行覆盖为 `_binary_payload w5T1.xzdio`，保留原 XZ 压缩级别 5，明确线程 1；不修改任何 spec 或平台宏文件。相同隔离环境下 rpm --eval 实际输出 w5T1.xzdio（serial_packaging_correction.json）。后续新 RPM 单元均采用显式单线程压缩。
5. medium 复查退出 0，负载 1.64；15:00:14 重启完整 rpmbuild --noprep --noclean -bb（raw/202），不使用 short-circuit。新的独立日志在 cells/llvm-arm-rpm-serial/，旧日志不覆盖。
6. 新 scope memory.max=16536461312、nice19、idle 已实际核对；截至此附记仍在配置阶段，不能提前写包通过。14 小时截止不顺延。aarch64 先前产物的正确性核查保留；该轮压缩工作线程未单独捕获，记 NOT_OBSERVED，不能用“无 XZ 警告”证明压缩并行度。

## 15:43 进度与验证准备

- 单线程重试已写出主包及部分调试包，正在写 clang-debuginfo；未取得完整退出码前不宣布完成。raw/250、251 记录 memory.current=5052358656，oom/oom_kill=0，磁盘可用 229 GiB。实测 rpmbuild 有三个宿主线程，其中两个睡眠、一个工作；不将宿主线程总数写成一。
- 板上 /usr/bin/bpftrace 与 /usr/bin/bpftrace-static 均不存在，已保存完整安装包清单供恢复核对。/proc/config.gz 与 /boot/config-* 不可得；BTF 和 tracing 列表存在不等于功能测试通过。没有新增 PC sudo。
- 准备取消探针与功能脚本，已做 Python 语法检查，尚未执行。取消脚本逐次核对远端退出码与清理/join 断言；static-libgcc 的 ARM 对照只记录实测，不以 x86_64 结果代替。功能脚本不安装 RPM，只检查已安装且 SHA256 固定的二进制。
- armv7l 仍使用 QEMU 工具入口。aarch64 的原生工具授权与五对象抽查不外推至 armv7l；是否允许 ARM 原生工具抽查的问题已提出，未答复前不切换。

## 15:51 armv7l 写包失败与单独产物核查

- raw/202 实际退出 1；监控 finished 时间为 15:47:27。最后错误为 `create archive failed: cpio: write failed - Function not implemented`。18 次 Wrote 后没有完整结束，不能以 18 个文件替代整轮成功。
- raw/259、260：磁盘可用 229 GiB，inode 可用 88,417,614；该 BUILDROOT 没有大于 1 GiB 的常规文件。具体产生 ENOSYS 的系统调用/源代码位置 NOT_OBSERVED；不能把错误直接归因于磁盘不足、大文件或 QEMU。
- raw/261 是只读产物核查，退出 0：18 个现存 RPM 摘要及 xz/5T1 头字段通过；实际 RPM 提取的 libclang.a SHA256 为 9884348bb092f3d550d2485de9a83dcb130f8b1540e0c4f115b82686e42bc374，std::__1 字符串行 1062、std::__cxx11 0；libLLVM 的动态依赖含 libc++，不含 libstdc++。该检查只说明这些具体 RPM，不关闭整轮门禁。
- 已询问是否允许将独立核验的静态开发 RPM 用于 bpftrace、同时保留 LLVM 整体写包失败。未收到答复前不沿用 armv7l 输入；aarch64 的已通过输入不受该问题影响。
- 未执行的安装脚本从文件名前缀选择改为 RPM NAME 精确选择 libc++、libc++-devel、libc++abi、libc++abi-devel 四包，防止将额外的调试子包误计入四包。只是输入选择工具修正，不改任何 spec/包内容，尚未执行安装。

## 18:52 中断后续跑

- 用户要求继续；原截止 22:48:19 不重置。raw/274 盘点没有旧构建/复制进程，磁盘 227 GiB；原 copy-inputs-aarch64 的最后事件为 15:58:36，raw/269 没有 finished/exitcode，结束原因与退出码均记 NOT_OBSERVED，不补记成成功或某个推测退出码。
- 新 medium 门禁退出 0（raw/275），开工同一读写探针退出 0（raw/276）。系统 uptime 表明系统启动时间已变化，不把这单独作为原中断原因的证明。
- 旧内存上限启动检查 raw/279 退出 1，尚未调用复制子进程。重读 MemTotal=32297772 KiB，50% 按页向下取整为 16536457216 字节，比旧值少 4096 字节。raw/281 以新上限启动，实际 cgroup 与 memory.max、nice19/idle 已核验。
- 复制脚本新增显式 --resume，仅允许当前 aarch64 私有目录；以 rsync --checksum 校验已有文件后补齐，保留原始复制日志。新命令记录另存 -resume-commands.json，不覆盖旧输出。脚本哈希 raw/282。
- 目录盘点 du 较慢且处于 D 状态，补做同一 I/O 探针 raw/284：6.991 秒、退出 0，尚未达到 30 秒暂停阈值。当前未启动新的编译，不以此声称所有 I/O 都正常。

## 19:08 复制校验失败，中止并交人工确认

- du 最终完成，来源 usr=1607950336 字节、RPM 数据库=5455872 字节，合计 1613406208，仍低于 3 GiB。第二次额外 I/O 探针 raw/286 为 1.311 秒、退出 0；没有触发“超 30 秒暂停十分钟”条件。
- rsync --checksum 对旧根 usr/bin/llvm-opt-report 报 `read errors mapping ... Operation not permitted (1)`，两次更新均被其校验拒绝。raw/289 对本轮已核定 PGID 13095 发 TERM，退出 0；raw/281 最终记录器退出 241、监控子进程退出 -15。未把此次中止计为复制成功。
- 源文件 mode=0755、root:root、40896 字节。raw/290 的源文件 SHA256 读取成功，目标不存在，组合命令退出 1。不能将该组合退出 1 解释成源文件 SHA256 失败。
- raw/292 受控单文件诊断：rsync 退出 23，strace 实际 `openat(... O_RDONLY|O_NOFOLLOW)=3` 后 `read(...,40896)=-1 EPERM`；cp 退出 0。普通读取、cp 的 SHA256 均为 55629940b16d1ca7c100f17a41f3e1677f7ec3726449d9d8fa0f462f9b941a35。诊断仅写 tmp/STATIC_0917B/rsync_read_probe 的任务文件，不改源文件，不使用 sudo。
- 产生读取差异的机制仍为 NOT_OBSERVED；未将它推断成普通 I/O 超时、损坏、权限位问题或安全控制。未自行改用 cp 复制整个根，未降低输入校验要求，等待人工确认如何处理。
- 本轮最终 PARTIAL。aarch64 LLVM 的 12 个完整 RPM 及核查结果保留；bpftrace 两架构三路径 RPM、安装及板上功能/取消验证未完成，候选不能据本轮结果替换发布资产。没有推包仓或改 Source1002；没有新增 PC sudo。
