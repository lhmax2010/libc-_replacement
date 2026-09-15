# 夜间任务状态：源码适配已推送，static 构建未启动

| 任务 | 状态 | 用时 | 产物路径 | 备注 |
| --- | --- | --- | --- | --- |
| W1 推送 bpftrace | COMPLETE | 440 秒（资源门禁至项目材料推送；更早阅读不计） | W1_REPORT.md、PACKAGE_STATUS.*、w1_result.json | f895f8c 已推；源码适配 11/11，static 单列 |
| W2 static 可行性评估 | EVALUATED_WITH_BLOCKERS | 1098 秒（至评估收口） | W2_REPORT.md、static_availability_summary.tsv | 匹配的核心静态输入缺失；完整闭包与可靠估时未取得 |
| W3 static 构建 | NOT_STARTED | 0 | W3_NOT_STARTED.md | 按 W2 第 11.b 条，不启动、不改 spec、不推候选 |

时间戳以 w2_result.json 与 raw 为准，归档/推送在评估收口后继续进行。
未达到任何阶段时限。全任务不足一小时，没有虚构小时心跳或构建目标进度。

资源：并行度 1；nice=19；ionice class=3；ulimit -v 16148878 KiB，为实测
MemTotal 32297756 KiB 的 50%，约 15.40 GiB/进程虚拟地址空间，不是累计 RSS。
两次 medium 门禁均 0；开工磁盘约 264 GiB；4.5 MiB 读写/fsync 探测约 0.105 秒。
本轮没有构建，所以“每 500 目标”与 I/O 暂停机制未触发，不能声称暂停流程已实测。
只清理了当次探测新建的 4.5 MiB 临时文件及其空目录；未删除任何既有资料。
开发板离线，armv7l 板上项目均 NOT_OBSERVED_BOARD_OFFLINE；无连接或等待。

阶段记录：

- 2026-09-15T21:56:02+08:00：W1 资源门禁开始，之后四格/11 spec 复核通过。
- 2026-09-15T22:03:22+08:00：W1 项目材料推送完成；包的推送时间在 w1_result.json。
- 2026-09-15T22:03:39+08:00：W2 资源门禁开始。
- 2026-09-15T22:21:56.568067+08:00：W2 评估收口，W3 NOT_STARTED；随后提交推送 W2/W3 记录。

需后续补齐：匹配版本与 libc++ ABI 的 libclang.a、libc++.a、libc++abi.a，
传递静态依赖闭包，以及有权限环境下原资产的 --info/实际 probe 基准。
这不是永久不可构建结论，也不是新静态构建失败；本轮没有启动新构建。

收尾记录：raw/044 的分支对照误写了既有日志文件名，diff 返回 2（文件不存在），
不是分支变化判据失败；raw/045 用正确路径重跑返回 0，确认 W2 期间所有
bpftrace 远端分支与 W1 推送后记录一致。两次原始命令与输出均保留。
