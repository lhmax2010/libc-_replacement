# T3R2 证据索引

- `T3R2_report.md`：中文主报告。
- `invocation_delta.tsv` / `invocation_commands.tsv` / `invocation_adjustments.tsv`：三次旧调用对照及 r2 修正依据。
- `build_status.tsv` / `red_items.tsv`：逐架构状态与红项。
- `artifact_sha256.tsv` / `toolchain_nevra.tsv`：制品哈希与三架构实际工具链。
- `cross_arch_comparison_status.tsv`：三架构门禁前置条件状态。
- `armv7l/` / `aarch64/` / `x86_64/`：架构级原始证据。
- `commands/`：命令原文、输出与退出码。
- `raw/`：上一轮三次调用上下文取证原文。

本任务最终状态为 `RED_STOP`（armv7l `UNCLASSIFIED`）；aarch64 成功，x86_64 保持不变。
