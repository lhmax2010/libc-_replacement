# R101 材料索引

- `REPORT.md`：完整核查与结论。
- `SUMMARY_ONE_PAGE.md`：供总部汇报的一页摘要。
- `tables/base_all_source_packages.tsv`：254 个源码包的机器可读完整清单。
- `tables/base_all_source_packages.md`：同一清单的 Markdown 版。
- `tables/implementation_10_packages.tsv` / `.md`：10 个 `NEED_LIBCXX` 包的实施输入。
- `UNDETERMINED.md`：`gmp` 与 `tensorflow2` 的未定原因、闭合方法和临时处理。
- `tables/base_internal_layout_sensitive_edges.tsv`：4 条源码包级 Base 内部布局敏感边。
- `tables/base_internal_layout_sensitive_elf_edges.tsv`：121 条具体 ELF 边。
- `inputs/current_specs/`：2026-09-03 从实际 `tizen_base` HEAD 只读导出的 10 份 spec。
- `code/`：本轮生成与核查脚本。
- `raw/`：命令原文、标准输出、标准错误和退出码。

本任务没有修改配置、平台源码或 R100 判定，没有推送 Gerrit。
