# R98 材料索引

- `REPORT.md`：主报告。
- `tables/buildlog_compiler_evidence.tsv`：3,848 条实际 CC/CXX 证据。
- `tables/gcc_package_decision_table.tsv`：11 个 GCC 包的决策用事实表。
- `tables/gcc_inbound_build_dependencies.tsv`：完整构建期依赖。
- `tables/gcc_runtime_dependency_edges.tsv.gz`：包含系统运行时的完整 RPM
  capability 依赖原始表。
- `tables/gcc_runtime_source_pair_classification.tsv`：30 个稳定非系统源码对。
- `symbol_edges/actual_cross_elf_edge_evidence.tsv`：417 条实际 ELF 符号边。
- `tables/gcc_exact_image_membership.tsv`：精确镜像归属。
- `tables/gcc_dlopen_string_evidence.tsv`：动态加载二进制证据。
- `tables/requested_tool_compiler_checks.tsv`：任务书点名工具的编译器核对。
- `inputs/specs/`：11 份精确 spec。
- `raw/`：命令、输出、退出码及技术性恢复记录。
- `code/`：本轮分析脚本。

大体积 RPM 与临时解包树未入库；其 URL、预期/实际 SHA256、下载命令和
分析命令均由表格与 raw ledger 完整记录，可重建。
