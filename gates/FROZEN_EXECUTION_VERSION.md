# Gate tools frozen execution version

状态：`FROZEN_WAVE_EXECUTION_V1_2`
冻结日期：2026-07-28
验证矩阵：33/33 与预期一致（29 RED，4 GREEN）

| 工具 | 版本 | SHA256 | fixture |
|---|---|---|---:|
| G6 tunnel registry | 1.1.0 | `3feca96a1629ff90c2a0c057b4dd933c7ed43af114d4c82f5dba828984373ccc` | 7 |
| G7 exception gate | 1.1.0 | `eeb0f387e1cd9ec26c8e33ec38e4b7415b75cf9678c3b5d9bcedc9bb1fd4db98` | 13 |
| edge diff gate | 1.1.0 | `3e06d9e593533789360cfe10c0db4a924782f223b9febf37fd6328a9e22c2f74` | 7 |
| promotion ledger gate | 1.2.0 | `5933af827664b78dc51f5ec4a62a0f8caf2444f44fd553e11830ed335673e8a7` | 6 |

v1.2 增量：

- 增加逐输出晋级台账门：manifest 的每个架构相关或共享 noarch 输出必须
  恰有一行，缺失与重复均 fail-closed；
- ADMIT 集合必须形成 TIER1 分量闭包；HOLD 镜像实选 SHA 必须等于登记的
  legacy authority；精确 EVR 锁不得跨 ADMIT/HOLD；
- 缺列或空输入返回输入错误码 3，不进入业务判定；
- 新增 1 绿 + 5 红独立 fixture；并按解冻纪律重跑旧三工具全部 27 项，
  完整矩阵实际为 4 绿 + 29 红。

冻结对象：

- `tools/*.py`
- `fixtures/**`
- `fixture_catalog.tsv`
- `verdict_matrix.tsv`
- `tool_status.tsv`

冻结清单：`FREEZE_MANIFEST.sha256`  
冻结清单条目：108。清单自身不纳入，避免自引用；其 SHA256 由仓根
`MANIFEST.sha256` 和整改报告记录。

执行纪律：上述任一冻结对象 SHA 变化即自动解除冻结，必须重跑完整 33 项
矩阵，不能只重跑受影响 fixture。原 v1.1 的 27 项矩阵由本版本整体取代。
