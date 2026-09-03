# R103 材料索引

- `REPORT.md`：结论、证据链、版本身份、局限与 R100 分类建议。
- `tables/consumer_linkage_summary.tsv`：逐消费 ELF/架构的精确符号与成员下界。
- `tables/provider_observation_summary.tsv`：三个目标归档的阳性/阴性汇总。
- `tables/representative_archive_members.tsv`：高置信代表成员及匹配符号。
- `tables/archive_symbol_matches.tsv.gz`：完整逐符号匹配表。
- `tables/archive_member_matches.tsv.gz`：完整逐成员匹配表。
- `tables/provider_archive_symbols.tsv.gz`：提供方归档完整符号清单。
- `tables/consumer_elf_inventory.tsv.gz`、`consumer_needed_libraries.tsv.gz`：最终产物与动态依赖。
- `tables/consumer_binary_candidates.tsv.gz`、`consumer_download_status.tsv.gz`：消费 RPM 选择范围与逐包校验状态。
- `inputs/buildlogs/`：15 份公开成功构建日志原文。
- `raw/`：每条命令、stdout、stderr、退出码原文。
- `code/`：下载、解包、扫描及汇总脚本快照。
