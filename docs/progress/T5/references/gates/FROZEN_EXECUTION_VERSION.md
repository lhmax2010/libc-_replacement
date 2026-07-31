# Gate tools frozen execution version

状态：`FROZEN_WAVE_EXECUTION_V1_5_LLVM_22`
冻结日期：2026-07-31
验证矩阵：42/42 与预期一致（37 RED，5 GREEN）

| 工具 | 版本 | SHA256 | fixture |
|---|---|---|---:|
| G6 tunnel registry | 1.1.0 | `3feca96a1629ff90c2a0c057b4dd933c7ed43af114d4c82f5dba828984373ccc` | 7 |
| G7 exception gate | 1.1.0 | `eeb0f387e1cd9ec26c8e33ec38e4b7415b75cf9678c3b5d9bcedc9bb1fd4db98` | 13 |
| edge diff gate | 1.1.0 | `3e06d9e593533789360cfe10c0db4a924782f223b9febf37fd6328a9e22c2f74` | 7 |
| promotion ledger gate | 2.1.0 | `245d30e20f451dd5a6954138b06b33ec9c1e27926afc28432bf8eb8fc45b32b2` | 15 |
| libc++ static packaging gate | LLVM_22_ASSERTION_REV1 | `d24f7c234d266bf970dbfd9c24939052681eba2105c186d0002009891c119885` | 0 |

fixture 总数为 42。libc++ static packaging gate 的 fixture 数为 0：T3 已因
版本偏斜红停，T4 没有 LLVM 22 新制品，故本次只修订工具断言，不伪造制品门
运行。42 项矩阵是冻结治理工具的完整负/正 fixture 矩阵，重跑逐项原始结果在
`progress/T5/matrix/`。

## LLVM 22 断言修订

旧工具 SHA256 为
`e37add65648c9afdb11eb320926b2285ae6e6da324c70b9510909b799df42d3e`，
状态为 `SUPERSEDED_BY_LLVM_22`；新工具仅将 `LLVM_21` 字面量替换为
`LLVM_22`。把新工具反向规范化为 `LLVM_21` 后，与旧工具逐字 `cmp` 退出 0。
旧、新登记均保存在 `SYMBOL_ASSERTION_FREEZE.tsv`；v1.4 原登记全文保存在
`FROZEN_EXECUTION_VERSION_V1_4_SUPERSEDED_BY_LLVM_22.md`。

## 冻结对象

- `tools/*.py` 与 `tools/run_static_gates.sh`
- `fixtures/**`
- `census_input_manifest.tsv`
- `stdlib_neutral_registry.tsv`
- `fixture_catalog.tsv`
- `verdict_matrix.tsv`
- `tool_status.tsv`
- `SYMBOL_ASSERTION_FREEZE.tsv`
- 当前与被取代的冻结登记

冻结清单：`FREEZE_MANIFEST.sha256`。清单自身不纳入，避免自引用。

执行纪律：上述任一冻结对象 SHA 变化即自动解除冻结，必须重跑完整 42 项
矩阵，不能只重跑受影响 fixture。旧 v1.4 登记已明确标记
`SUPERSEDED_BY_LLVM_22`，但未删除。
