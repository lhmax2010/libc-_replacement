# 状态：`SUPERSEDED_BY_LLVM_22`（原文以下完整保留）

# Gate tools frozen execution version

状态：`FROZEN_WAVE_EXECUTION_V1_4`
冻结日期：2026-07-28
验证矩阵：42/42 与预期一致（37 RED，5 GREEN）

| 工具 | 版本 | SHA256 | fixture |
|---|---|---|---:|
| G6 tunnel registry | 1.1.0 | `3feca96a1629ff90c2a0c057b4dd933c7ed43af114d4c82f5dba828984373ccc` | 7 |
| G7 exception gate | 1.1.0 | `eeb0f387e1cd9ec26c8e33ec38e4b7415b75cf9678c3b5d9bcedc9bb1fd4db98` | 13 |
| edge diff gate | 1.1.0 | `3e06d9e593533789360cfe10c0db4a924782f223b9febf37fd6328a9e22c2f74` | 7 |
| promotion ledger gate | 2.1.0 | `245d30e20f451dd5a6954138b06b33ec9c1e27926afc28432bf8eb8fc45b32b2` | 15 |

v1.4 neutral 资格信任边界：

- `ADMIT_STDLIB_NEUTRAL` 的资格不再由台账行里的三个 proof 字段声明；
  三列已从工具、实例、fixture 和模板删除；
- authority manifest 由一个固化根认证，且为每个 census 同时登记 TIER1
  成员表与 neutral 登记册两个叶子的 SHA256；
- neutral 行必须以
  `(census_id,target_arch,package,rpm_arch,nevra)+rpm_sha256`
  精确命中认证登记册，否则报红 `NEUTRAL_NOT_IN_TRUSTED_REGISTRY`；
- 当前生产登记册仅有 `boost-license` 三架构行。资格证据为冻结 RPM 的
  `rpm -qpl`、ELF magic 零命中与 RPM/repodata/冻结表 SHA 一致；
- 新增三项红 fixture：普通 fixture 自封 neutral、真实台账
  `boost-program-options` 自封 neutral、neutral 登记册篡改；
- 按解冻纪律重跑四工具全部 42 项，实际为 5 绿 + 37 红。三组生产晋级
  台账另在认证的 0723 成员表和 neutral 登记册上全部 GREEN。

冻结对象：

- `tools/*.py`
- `fixtures/**`
- `census_input_manifest.tsv`
- `stdlib_neutral_registry.tsv`
- `fixture_catalog.tsv`
- `verdict_matrix.tsv`
- `tool_status.tsv`

冻结清单：`FREEZE_MANIFEST.sha256`，共 142 项。清单自身不纳入，避免
自引用；其 SHA256 由仓根 `MANIFEST.sha256` 和整改报告记录。

执行纪律：上述任一冻结对象 SHA 变化即自动解除冻结，必须重跑完整 42 项
矩阵，不能只重跑受影响 fixture。原 v1.3 的 39 项矩阵由本版本整体取代。
