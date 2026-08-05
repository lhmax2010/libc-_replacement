# T4 策展证据索引

- `T4_report.md`：中文主报告；最终状态为 armv7l `RED`、aarch64
  `NOT_AVAILABLE`、x86_64 `PASS`。
- `gate_inventory.tsv` / `no_tool_mapping.tsv`：21.1.1 门项全集、出处与
  T5 145 项冻结登记映射。
- `gate_results.tsv` / `three_arch_gate_comparison.tsv` /
  `architecture_summary.tsv`：逐项结果与三架构结论。
- `artifact_identity.tsv` / `source_input_delta.tsv`：24 个 RPM 的归档
  SHA 核验，以及 armv7l Source0 红项。
- `commands/`：本轮命令原文与退出码 ledger；失败重试原文未删除。
- `<arch>/static/`：门禁结果和为报告保留的关键原始加载证据。
- `EXCLUDED.tsv`：未策展的本地全量文件逐文件登记。

本地全量证据目录：
`/home/toolchain/development/libc++_replacement/progress/T4/`。
本地全量 `MANIFEST.sha256` 共 6075 项；RPM、临时别名载体、解包树和
批量原始输出不上传。
