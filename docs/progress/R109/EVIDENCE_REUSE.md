# 复用材料及输入身份

本轮不重新做扫描或构建，只整理已完成工作的证据。复用范围如下。

| 来源 | 复用内容 | 输入身份与适用范围 |
|---|---|---|
| `docs/progress/R102/tables/base_all_source_packages_rewritten.tsv` | 254 项清单的唯一表格基线 | `Tizen-Base-Toolchain` 源码包口径；R102 沿用 R100/R101 判定。除 R103 已闭合的两项外，本轮不改变其字段 |
| `docs/progress/R103/REPORT.md` 及 `tables/` | `gmp`、`tensorflow2` 的静态归档实际链入结论 | Base Toolchain 的 `gmp@490fe651...`、`tensorflow2@1cdba735...`；消费产物来自已记录的 Unified Toolchain 快照。结论限于报告列明的消费方和快照 |
| `docs/progress/R106/REPORT.md` 及 `tables/` | 两仓固定快照统计、源码包和二进制包集合差异 | `Tizen-Base/tizen-base_20260827.214809` 与 `Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647`，不可外推为滚动仓当前值 |
| `docs/progress/R104/REPORT.md` | 第一批五包提交、LLVM/MLGO 阻断、584/201 条布局敏感边 | 各包既有 `sandbox/lhmax2025/libcxx-migration` 分支；本轮只引用，不访问或修改 Gerrit |
| `docs/progress/R105/REPORT.md` | `boost`、`libsigc++` 提交与验证 | 同上；`tensorflow2` 当时尚未通过门禁，因此不把 R105 的状态当成最终状态 |
| `docs/progress/R108/REPORT.md` | `tensorflow2` 基线对照、最终提交与推送 | 提交 `6d761390...`；armv7l GCC 格按“候选与未改基线同点失败”的非回归判据闭合 |

总部 step0 的 `639/91` 是其文档给出的 `Tizen-Base` 二进制包口径。R106
明确记录该数字来自未固定版本的滚动仓；本轮只解释它与固定快照结果的关系，
不把两种统计方法混成同一口径。

