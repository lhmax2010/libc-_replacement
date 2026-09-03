# R102 材料索引

- `REPORT.md`：改写口径、分组、核查结论、限制与疑问。
- `tables/base_all_source_packages_rewritten.tsv`：254 包机器可读对外说明版。
- `tables/base_all_source_packages_rewritten.md`：同一清单的 Markdown 版。
- `NO_CHANGE_GROUPS.md`：236 个“不需要改”包的共同理由和逐包差异。
- `tables/reason_before_after.tsv` / `.md`：R101 与 R102 的理由逐包对照。
- `INSUFFICIENT_REASONS.md`：证据仍不足以完成最终判定的两个包。
- `code/`：生成、抽查和一致性校验脚本。
- `raw/`：命令原文、标准输出、标准错误与退出码。

本轮只重写说明文字。R101 的包顺序、其余七列和全部 254 项判定均未改变；
未修改配置、平台源码或既有 R101/R100 材料，未推送 Gerrit。
