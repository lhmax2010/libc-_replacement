# 投影审计交接

状态：**PARTIAL（65/65 全量命中已处理；语义结果未全闭合），待人工审阅**。

| 阶段 | 状态 | 产物 |
| --- | --- | --- |
| 全量命中与原始实参提取 | 65/65；74,954 行 → 30,523 个候选/SHA/行 → 31,586 处 | COVERAGE_INVENTORY.tsv、occurrences/ |
| 结果类型 | 15,780 处无对象；13 处 std 对象；15,793 处未定 | results/、CANDIDATES.md |
| 候选聚合 | 整项无对象 0；含 std 对象 2（与未定重叠）；未定 65 | CANDIDATE_RESULTS.tsv |
| 对象边界 | 13 处均未闭合新的异包边；确认新增 0 | OBJECT_BOUNDARIES.tsv |
| 跨包面 | 已确认 16 包对 / 21 符号边不变（下界） | EDGE_COUNTS_BEFORE_AFTER.json |
| 保护与覆盖自检 | 484 个旧文件未变；6,952 来源路径 SHA 通过 | FINAL_AUDIT.json |

截至汇总生成用时 0.716 小时；原始时间见 TIMING.json。第15/30/45/60项已分别提交推送；末5项与汇总作最终提交。

资源 light、RLIMIT_AS 9,921,875,558 字节（MemTotal 的30%），分析 nice19/ionice3；Git 普通优先级。不用板子、不构建平台包、不改平台代码、不推 Gerrit。串行执行曾有1.834358秒偏差，已重新串行取证，见 FINAL.md；不隐去偏差。

原 BUILTIN_ALIAS/EMPTY_TYPE 结论与 plan.json 不变。完整结果、未定原因及自行判断见 FINAL.md、UNRESOLVED_REASONS.tsv。完成本轮后停止，交人工审阅。
