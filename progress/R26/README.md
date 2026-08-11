# R26 产物索引

- `REPORT.md`：1,191 条跨界边的语义分类、provider 规模和盲区结论。
- `METHODOLOGY.md`：符号提取、解名、肯定式分类规则及局限。
- `inputs/input_identity.tsv`：复用数据和四个公开头文件 RPM 的 SHA256。
- `tables/edge_semantic_classification.tsv`：逐边分类主表。
- `tables/semantic_evidence_symbols.tsv`：逐边命中的完整原始/解名符号证据；本地保留完整表，策展副本拆分。
- `tables/semantic_class_summary.tsv`、`edge_reason_summary.tsv`：边分类汇总。
- `tables/cut_semantic_distribution.tsv`：切面 1–5 分类分布。
- `tables/provider_semantic_summary.tsv`、`provider_class_summary.tsv`、`provider_top20.tsv`：75 个 provider 全表、三类汇总和前 20。
- `tables/c_only_sample_review.tsv`：C_ONLY 空总体的抽样申报。
- `tables/public_header_reviews.tsv`：四个歧义符号的公开头文件复核。
- `tables/callback_name_candidates.tsv`、`blind_spots.tsv`：回调名称近似与不可见形态。
- `tables/classification_method.tsv`：命令和规则登记。
- `tables/data_sources.tsv`：仓快照和对象形态。
- `tables/CURATION_MAP.tsv`：大表策展拆分映射。
- `commands/`：命令原文、工作目录、输出和退出码。
- `validation.tsv`、`MANIFEST.sha256`：断言和完整性清单。

除特别标注外，边分母为 R25 的 1,191 条去重跨切面源码级边，provider 分母为 75 个精确源码 RPM 身份。
