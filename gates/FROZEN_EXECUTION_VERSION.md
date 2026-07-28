# Gate tools frozen execution version

状态：`FROZEN_WAVE_EXECUTION_V1_3`
冻结日期：2026-07-28
验证矩阵：39/39 与预期一致（34 RED，5 GREEN）

| 工具 | 版本 | SHA256 | fixture |
|---|---|---|---:|
| G6 tunnel registry | 1.1.0 | `3feca96a1629ff90c2a0c057b4dd933c7ed43af114d4c82f5dba828984373ccc` | 7 |
| G7 exception gate | 1.1.0 | `eeb0f387e1cd9ec26c8e33ec38e4b7415b75cf9678c3b5d9bcedc9bb1fd4db98` | 13 |
| edge diff gate | 1.1.0 | `3e06d9e593533789360cfe10c0db4a924782f223b9febf37fd6328a9e22c2f74` | 7 |
| promotion ledger gate | 2.0.0 | `d09603a49f5ac8cba9ca19f998a6304f5fffbf65107aabea95a2c4c0062bdb00` | 12 |

v1.3 第四门信任边界：

- 逐输出连接键固定为
  `(batch,target_arch,package,rpm_arch,nevra)`；candidate manifest 到
  台账、台账到 manifest 双向完备，分别阻断缺行和幽灵行；
- manifest RPM SHA、ledger candidate SHA、ADMIT 镜像实选 SHA 三方绑定；
  HOLD 继续绑定 legacy authority；
- TIER1 闭包只消费独立的 census 分量成员表。门先验证
  `gates/census_input_manifest.tsv` 自身的冻结 SHA，再验证成员表 SHA；
  台账和 manifest 的自报分量只作交叉核对；
- `ADMIT_STDLIB_NEUTRAL` 只对“不含 ELF、无 C++ 面、内容 SHA 双源复验”
  三项均为 PASS 的输出免闭包，身份三方对账不豁免；
- 新增 1 绿 + 5 红 fixture；按解冻纪律重跑四工具全部 39 项，实际为
  5 绿 + 34 红。三组生产晋级台账另在认证的 0723 census 输入上全部
  GREEN。

冻结对象：

- `tools/*.py`
- `fixtures/**`
- `census_input_manifest.tsv`
- `fixture_catalog.tsv`
- `verdict_matrix.tsv`
- `tool_status.tsv`

冻结清单：`FREEZE_MANIFEST.sha256`，共 130 项。清单自身不纳入，避免
自引用；其 SHA256 由仓根 `MANIFEST.sha256` 和整改报告记录。

执行纪律：上述任一冻结对象 SHA 变化即自动解除冻结，必须重跑完整 39 项
矩阵，不能只重跑受影响 fixture。原 v1.2 的 33 项矩阵由本版本整体取代。
