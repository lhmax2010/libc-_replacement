# R28 产物索引

- `REPORT.md`：核查结论。
- `inputs/input_identity.tsv`：冻结输入路径、SHA256、大小。
- `tables/gcc_edge_elf_evidence.tsv`：18 条 gcc/libstdc++ ELF 证据。
- `tables/gcc_symbols_by_consumer_binary.tsv`：986 条逐符号、消费者二进制与架构记录。
- `tables/gcc_distinct_symbol_summary.tsv`：176 个去重符号汇总。
- `tables/gcc_source_identity_distribution.tsv`：gcc 1.13/1.15 的仓与架构分布。
- `tables/permanent_edges_12_consumer_provider.tsv`：12 条永久边。
- `tables/vendor_consumer_distribution.tsv`：4 个厂商源码身份的 provider 分布。
- `tables/corrected_permanent_platform_providers.tsv`：修正后的 4 个平台 provider。
- `tables/corrected_permanent_accounting.tsv`：修正后的 8 个永久源码身份账目。
- `validation.tsv`：机械断言。
- `commands/`：命令原文与退出码。
