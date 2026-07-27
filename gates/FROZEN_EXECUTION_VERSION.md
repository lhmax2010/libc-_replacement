# Gate tools frozen execution version

状态：`FROZEN_WAVE_EXECUTION_V1`  
冻结日期：2026-07-27  
验证矩阵：13/13 与预期一致（10 RED，3 GREEN）

| 工具 | 版本 | SHA256 |
|---|---|---|
| G6 tunnel registry | 1.0.0 | `2ea62a0a8b7971a8d0343471a0f4d95b79adfd1e420c8a9081b9792ef9896303` |
| G7 exception gate | 1.0.0 | `c804693f729b012e972ea83cde1a6067751dc039790026536f7f3fcd601b4299` |
| edge diff gate | 1.0.0 | `97704e77f2d1f2450e9d57366042e28a22db5922216499006d97b3709cb22d00` |

冻结对象：

- `tools/*.py`
- `fixtures/**`
- `fixture_catalog.tsv`

冻结清单：`FREEZE_MANIFEST.sha256`  
冻结清单 SHA256：
`4f244965a95d71b7d817d149476dc6a658dd6df98598dcdf565d22393ff2f2ea`

执行纪律：上述任一文件 SHA 变化即自动解除冻结，必须重跑完整 13 项矩阵，
不能只重跑受影响 fixture。
