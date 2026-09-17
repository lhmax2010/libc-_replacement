# 续跑状态

起点 2026-09-17 08:48:19 +08，硬截止 22:48:19 +08。

| 项目 | 状态 | 证据 / 边界 |
| --- | --- | --- |
| 磁盘、资源、同一读写探针 | PASS | 001–004；251 GiB；medium=0；3,932,160 字节探针 0.760745 秒 |
| 五对象原生等价抽查 | 沿用 PASS | 上轮 native-equivalence-summary.json；不是逐目标证明 |
| 历史未上传日志补齐 | 完成并推送 | cc22d4c2296b974a029fa73758668c392c08e62b；269 个归档文件 |
| LLVM aarch64 完整 RPM | 首次构建成功；交付集重写包中 | 13:37:01 首次完整 rpmbuild 返回 0（7546/7546、12 RPM）；外层记录器重复执行，污染部分 RPM 后已停止；13:44:23 改用一次性记录器重新完成一致的 RPM 集合 |
| armv7l/aarch64 bpftrace 三路径 RPM | NOT_STARTED | 等待静态开发 RPM |
| armv7l 物理板验证 | NOT_STARTED | 本轮未使用板子 |

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

当前 aarch64 对象由原生交叉工具集编译；正确性的工具切换依据仍仅是上轮五对象抽查，不作逐目标等价声明。尚未取得本轮完整 RPM，不能替换发布资产。

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
4. 重复执行已写出 llvm 与 llvm-devel 两个包，且现存 RPM 集合因此混有两次执行。全部 12 包可恢复地移至 `tmp/STATIC_0917B/rpm-aarch64/RPMS_prior_mixed_1343`，不作为最终验收输入。移动前逐个 SHA256 与时间记录于 raw/113、112；未删除任何包或 Source1002。
5. 新记录器 `run_logged_v2.py` 只调用子进程一次。新一轮完整 rpmbuild（仍 --noprep -bb，无 short-circuit）于 13:44:23 启动，复用已经完成的编译树；raw/116 保存独立命令、输出与退出码。重新检查 medium=0（raw/114），memory.max 仍为 16536461312，nice19/idle、-j1 均不放宽。
6. 重复执行浪费约 5 分 56 秒；重写包额外时间待实际结束后填入。14 小时截止仍为 22:48:19，不顺延。首轮打包期间 memory.events 的 oom/oom_kill 均为 0；补做 I/O 探测 1.358 秒通过（raw/106、107）。
7. armv7l 仅准备命令，尚未运行。用显式原 .rpmrc 与 .rpmmacros 后，展开编译 flags 与保留树 CMAKE_CXX_FLAGS 逐字一致；未加载原 .rpmrc 的前置查询曾返回默认 flags，因此没有用那个不完整环境启动构建。--load 不被该 RPM 支持（raw/096），改用实际支持的 --rcfile/--macros（raw/101），不更改平台宏文件。
