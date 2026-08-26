# R73 材料说明

## 报告

- `REPORT.md`：四部分分析、结论、盲区、自行判断与尚存疑问。
- `QUESTIONS_AND_JUDGMENTS.md`：自行判断与疑问的独立清单。

## 代码与用例

- `code/refine_facility_hits.py`：把 R72 的宽词法命中精化为显式 `std::`/`using std::` 使用。
- `code/analyze_package_risk.py`：在 R72 命中 source RPM 中枚举 pthread 取消相关 token；输出只供人工复核，不自动判安全。
- `code/r72_noexcept_cancel_samples.cpp`：R72 的动态抽样用例原文，本轮仅作为既有运行证据的代码引用，未重新编译或运行。

## 表格

- `tables/boundary_trigger_conditions.tsv`：5 处实际差异的触发条件、现场和常见性依据。
- `tables/explicit_std_facility_summary.tsv`：显式标准设施使用的 11 source RPM / 9 包名汇总。
- `tables/package_risk_review.tsv`：9 个显式命中包名的上下文复核。
- `tables/package_risk_inventory.tsv`：R72 35 source RPM 宽口径上的取消 token 自动盘点；含大量裸词误命中，不直接作为最终包数。
- `tables/layer_c_overlap.tsv`：边界文件与层 C 文件、机制和交付性质对照。

## 原始材料

- `raw/explicit_std_facility_hits.tsv`：逐行显式使用命中。
- `raw/package_cancel_token_hits.tsv`：逐行取消相关 token 命中。
- `commands/`：本轮 shell 命令、输出和退出码原文。
- `research/upstream_research.tsv`：上游/项目来源与事实。
- `research/search_scope.tsv`：`NOT_FOUND` 的精确检索范围。
- `research/web_research_notes.md`：联网调研的事实整理。

## 复用证据与未观测项

- D01/D03/D04 的动态原始输出、编译链接命令和运行时身份仍以 `docs/progress/R72/` 为原始来源；本任务没有把既有输出重新运行后冒充新材料。
- D02 与 D05 的动态表现仍为 `NOT_OBSERVED`。
- 本任务未连接 armv7l 开发板，也未新增运行用例。
- 联网检索的 `NOT_FOUND` 仅覆盖 `research/search_scope.tsv` 所列查询。
