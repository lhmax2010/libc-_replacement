# 表格阅读顺序

- `provider_scope.tsv`：第一阶段完整候选清单，含实际包身份、头文件数、公开/内部配方关联及第二阶段分组建议。
- `provider_category_summary.tsv`：最终五类统计；已排除一个 SDK 聚合根的重复计数，已修正分类规则。
- `sdk_api_groups.tsv`：167 份 public、55 份 internal 及 2 份 C# 配方的逐项头文件交集；未执行条件配方。
- `sdk_header_path_matches.tsv`：每个配方到包内具体头文件的可追踪关系；同一文件可在多个配方出现。
- `proposed_source_groups.tsv`：建议深化的源码 RPM 身份清单；最终是否进入阶段二以人工确认为准。
- `headers.tsv`（封存后为 `.tsv.gz`）：完整包内路径及形态记录；是拥有关系数，不是函数数或去重 API 数。
- `all_binary_records.tsv` / `logical_packages.tsv` / `candidate_packages_before_scan.tsv`：冻结分母与初筛，不是最终应用支持名单。
- `category_summary.tsv`：**初版自动扫描汇总，已被最终 provider_category_summary.tsv 取代，不用于结论。**
- `classification_changes.tsv` / `category_corrections.tsv`：自检纠正的逐项记录，旧计数不得与最终结果混用。
- `managed_sdk_files.tsv`：托管层、绑定层、SDK 根的发布文件列表；不是运行加载或边界行为实测。

`NOT_AVAILABLE`、符号链接未跟随、宏/传递 include 未展开均保留；任何 C++ 信号零命中只在方法定义范围内成立。
