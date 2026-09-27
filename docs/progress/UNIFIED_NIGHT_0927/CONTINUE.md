# 断点与续跑条件

没有编译断点：gbs build 尚未启动。断点是输入门禁，不是某包编到多少目标。

1. 先读 FINAL_RESULT.md 与 ARCH_INPUT_GATE.json。为拟跑架构补齐缺失的已验证 libc++ RPM（需 SHA、配方/验收来源）；不拿当前 GCC 输出代替、不从旧解包目录重新拼 RPM。若需要补建 Base，先取得新授权，本任务未执行。
2. 核对任务绝对截止 2026-09-28 08:30 +08:00；过期不得自行续跑夜间构建，需人工给新窗口。未启动的工作不因有 CONTINUE.md 自动授权。
3. 解决直接候选 51 个超过上限 30 的取舍；BUILD_SET.tsv 仅发现集合，不是获批构建队列。再次确认运行线没有重型任务/板测竞争。
4. 复用固定 Unified 20260917.132101/Base 20260914.073422 及 metadata 哈希，按 PROVIDER_SOURCE_METADATA.json 的 Gerrit 提交在 tmp 建只读原样 spec 的工作副本；不改 codes/。包源码/SRPM尚未下载、checkout。
5. 两份 project_config 副本在 tmp/UNIFIED_NIGHT_0927/configs/，补丁前后 SHA 见 raw/038；原件没改。完整 GBS 配置、优先 RPM 仓、实际依赖求解、payload 宏预检仍待做。命令必须传 `--define '_binary_payload w5.xzdio'`，禁止 T<数字>，不修改 spec。
6. 开工重新 medium 门禁、df（>=20GiB）、同一 I/O 探测；cgroup实际50%、nice19、ionice idle、串行-j1。resource_entry.py 只适合启动前资源断言，**不是完整构建/截止/I/O监控器**；启动构建前仍需准备并验证守护器，每500目标探测、超30s暂停10min、最多3次、硬截止保存停止、每小时写状态。
7. 成功包逐 ELF 核验，失败 ±30 行原文并分类，依赖连锁不算本包问题；C++与纯 C 判据须有证据，未观测不用0代替。
8. 仅提交推送项目材料与 LINE_STATUS；不推包仓、不上板、不启动 QuickBuild。

脚本说明：metadata.py 会重新联网取相同固定快照但参考 build.xml，scope.py/summarize_inputs.py 会覆盖本轮派生表；续跑应另存日志标签和历史表，不无意覆盖既有证据。inventory.py 补充盘点依赖 RPM_INVENTORY_INITIAL.json，不是通用全盘扫描器。
