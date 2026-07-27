# Gate tools frozen execution version

状态：`FROZEN_WAVE_EXECUTION_V1_1`
冻结日期：2026-07-27  
验证矩阵：27/27 与预期一致（24 RED，3 GREEN）

| 工具 | 版本 | SHA256 | fixture |
|---|---|---|---:|
| G6 tunnel registry | 1.1.0 | `3feca96a1629ff90c2a0c057b4dd933c7ed43af114d4c82f5dba828984373ccc` | 7 |
| G7 exception gate | 1.1.0 | `eeb0f387e1cd9ec26c8e33ec38e4b7415b75cf9678c3b5d9bcedc9bb1fd4db98` | 13 |
| edge diff gate | 1.1.0 | `3e06d9e593533789360cfe10c0db4a924782f223b9febf37fd6328a9e22c2f74` | 7 |

v1.1 增量：

- edge diff 阻塞新增 `CPP_ABI`、`CPP_NOSTL` 以及
  `A8_LAYOUT_PROMOTED`/`A9_POINTEE_PROMOTED` 升格布局边，并以
  `SIGNOFF_NEVRA_DRIFT` 阻塞签核绑定 source-NEVRA 漂移；
- G7 增加 `cross_dso_unwind_count`、批准 disposition/Q3 校验，并将
  disabled rider 改为 fail-closed；
- G6 补齐未签、过期和歧义登记项负面对照；
- 每个新红分支均有独立 fixture，矩阵实际为 3 绿 + 24 红。

冻结对象：

- `tools/*.py`
- `fixtures/**`
- `fixture_catalog.tsv`
- `verdict_matrix.tsv`
- `tool_status.tsv`

冻结清单：`FREEZE_MANIFEST.sha256`  
冻结清单条目：89。清单自身不纳入，避免自引用；其 SHA256 由仓根
`MANIFEST.sha256` 和整改报告记录。

执行纪律：上述任一冻结对象 SHA 变化即自动解除冻结，必须重跑完整 27 项
矩阵，不能只重跑受影响 fixture。原 v1.0 的 13 项矩阵由本版本整体取代。
