# 回滚验证增量总览

| 事项 | 状态 | 用时 | 产物路径 | 备注 |
|---|---|---|---|---|
| call_once 影响关系 | STATIC_CHECK_COMPLETE | 包含在本轮内 | CALL_ONCE.md | 自身等待链不经过本次解除入口，既有风险单列 |
| 两处写者实验回滚 | ROLLBACK_VIABLE（限定已测范围） | 资源门到双架构矩阵完成约 9 分钟；整理交付另计，精确起止见 raw/*.time.json | REPORT.md、diffs/ | 实验仅在 tmp；无生产改动 |
| 两架构功能矩阵 | MEASURED | 包含在本轮内 | matrix_x86_64.tsv、matrix_armv7l.tsv、cells.tsv | 280 次，所有实际值/状态断言通过 |
| 性能粗测 | MEASURED_WITH_LIMITATIONS | 包含在本轮内 | performance.tsv | 40 次，未断言开销可忽略 |
| 物理板清理 | COMPLETE | 包含在本轮内 | raw/024、raw/025 | 本轮目录及上传包已删，可由主机材料重建 |

所有正式格均 5 次；x86_64 原生、armv7l 物理板。无关历史文件与编译适配线未纳入本次变更。
疑问确认和限定见 [RUNTIME_QUESTIONS_0908b.md](RUNTIME_QUESTIONS_0908b.md)。
最新授权只推送本目录，因此本表作为目录外上一轮总览的增量，不改写其历史交付。
