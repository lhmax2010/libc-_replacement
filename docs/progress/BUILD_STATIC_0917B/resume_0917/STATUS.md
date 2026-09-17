# 续跑状态

起点 2026-09-17 08:48:19 +08，硬截止 22:48:19 +08。

| 项目 | 状态 | 证据 / 边界 |
| --- | --- | --- |
| 磁盘、资源、同一读写探针 | PASS | 001–004；251 GiB；medium=0；3,932,160 字节探针 0.760745 秒 |
| 五对象原生等价抽查 | 沿用 PASS | 上轮 native-equivalence-summary.json；不是逐目标证明 |
| 历史未上传日志补齐 | 完成并推送 | cc22d4c2296b974a029fa73758668c392c08e62b；269 个归档文件 |
| LLVM aarch64 完整 RPM | RUNNING | 09:47:45 已到 353/7546；完整编译与写包尚未完成 |
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
