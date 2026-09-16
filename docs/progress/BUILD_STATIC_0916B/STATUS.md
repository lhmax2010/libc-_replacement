# bpftrace-static 收尾验证

总状态 **PARTIAL / STATIC_CANCELLATION_GATE=FAIL**。候选当前不能替换发布资产。
开始 2026-09-16T11:01:59+08:00；本次汇总 2026-09-16T11:27:41.297892+08:00；墙钟 1542 秒。
各阶段命令起止时间在 raw/，未耗尽任何阶段时限；因静态行为失败及构建根准备受阻而收口。

| 阶段 | 结果 | 已完成 / 未完成 |
| --- | --- | --- |
| W1 功能等价 | PARTIAL | version/help 与 strip 已实测；info、probe、脚本被 root 检查阻止；内置函数实测清单未知；无 x86_64 原资产 |
| W2 完整 RPM | PARTIAL | aarch64 libcxx-runtimes 四包写出；LLVM 根复制 Permission denied 后停止；libclang RPM、三路径 bpftrace RPM、安装均未闭合 |
| W3 静态取消 | FAIL | 静态四格×5 全部 SIGABRT；动态四格×5 全通过；已得回溯、静态零初始化表与真实指令证据 |
| armv7l | NOT_OBSERVED_BOARD_OFFLINE | 未连接 |

W3 在 W2 的输入核查间隙进行，任何时刻只执行一个编译任务，不并行编译。
资源为 -j1、nice19、ionice3、ulimit -v 16148878 KiB（每进程 VA，不是聚合 RSS）。
RPM 构建监控启动及每500目标做 I/O 检查；实际记录在 runtime-aarch64-rpm.events.jsonl。
监控的小时状态在 HOURLY_STATUS.md；短于一小时的阶段由 raw/ 起止时间与本表交接。
候选和原资产 SHA256 复核未变；原源码树未改。无包仓/Gerrit 推送，无 Source1002 覆盖。
项目证据将仅提交到 codex/r5-ehabi-diagnosis。
