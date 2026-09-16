# 静态取消归因与动态展开器验证

开工：2026-09-16 13:22 +08:00。总上限十小时；各阶段分别计时。
资源门禁退出 0；并行 1，nice 19、ionice 3，ulimit -v 16148878 KiB（总内存一半，限制虚拟地址空间，非 cgroup RSS）。

| 阶段 | 状态 | 已观测 | 未完成 |
| --- | --- | --- | --- |
| W1 | COMPLETED | GNU 静态组 20/20 SIGABRT；动态组 20/20 通过；GDB 机制与 libc++ 一致 | 无本阶段待跑格；不外推其他架构 |
| W2 | PARTIAL | 原资产身份/依赖/符号与源码调用点核查完成 | 原资产精确展开器及取消行为 NOT_OBSERVED，缺原配方/调试件/触发入口 |
| W3 | COMPLETED_WITH_LIMITS | 同类问题有 GCC/glibc 记录；上游 musl CI / AppImage 区分完成 | 无 bpftrace 官方同机制取消复现；Bugzilla 状态 NOT_AVAILABLE |
| W4 | PASS_X86_64_SCOPED | 新候选重链接；静态 libc++＋动态 libgcc 20/20；额外 GNU 反事实 20/20；回退不变 | 不等于已满足全部发布门禁 |

armv7l：NOT_OBSERVED_BOARD_OFFLINE。不访问开发板，不推送包仓。

核心执行与核查于 2026-09-16 13:38 +08:00 完成，随后整理归档。目录中的 0917 是
任务标识，日志时间按主机实际日期记录，未改写日期。本轮计分重复运行共 80 次，
另有 5 次 GDB 诊断运行（四种形态各一次回溯，加一次 wait 寄存器检查）。
阶段命令的开始/结束时间、持续秒数见 raw/*.started.txt、finished.txt 及逐项 JSON；
无阶段达到小时上限，无需触发每小时进度更新。链接 I/O 探测均小于 30 秒，未暂停。

推送状态以 DELIVERY.md 及推送原始记录为准；本文件不是包仓发布批准。
