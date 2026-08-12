# R27 产物索引

- `REPORT.md`：中文主报告。
- `METHODOLOGY.md`：边方向、闭包规则、分母与盲区。
- `inputs/input_identity.tsv`：输入文件路径、SHA256、大小。
- `tables/seed_providers_75.tsv`：75 个种子逐包清单。
- `tables/closure_iterations.tsv`：保守/语义闭包逐轮记录。
- `tables/closure_members.tsv`：最终 76 个成员、阶段、进入轮次与二进制包数。
- `tables/closure_summary.tsv`、`closure_stage_distribution.tsv`：规模和属性分布。
- `tables/closure_propagation_edges.tsv`：闭包成员的已冻结出边。
- `tables/full_graph_semantic_classification.tsv`：完整 1,234 条边的语义分类。
- `tables/closure_semantic_excluded_edges.tsv`、`closure_mode_comparison.tsv`：两种闭包差异来源。
- `tables/permanent_edges_12.tsv`：切面 5 的 12 条边及完整符号。
- `tables/permanent_closure_members.tsv`、`permanent_closure_iterations.tsv`：永久闭包。
- `tables/permanent_closure_propagation_edges.tsv`：永久集合内部依赖。
- `tables/endpoint_accounting.tsv`：E-B 相关单位对齐账目。
- `tables/data_sources.tsv`、`method_and_scope.tsv`：快照与规则申报。
- `validation.tsv`：机械断言结果。
- `commands/`：命令原文与退出码。
- `MANIFEST.sha256`：本目录文件哈希清单。
