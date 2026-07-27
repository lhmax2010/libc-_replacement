# 三架构波次台账 v1

状态：`INITIAL_0723_DG1_DG2_DG3`

## 快照与方法

- Base：`tizen-base_20260709.234055`
- Unified：`tizen-unified_20260723.231947`
- 图口径：冻结 D2+A8+A9 生产 TIER1；执行前校准门保持硬前置。
- 共享规则：成员集和有效边集摘要 `member_edge_sha256` 三架构精确相同。
  任一差异进入对应架构覆盖层。

## 层次

| 层 | 分量数 | 包数 |
|---|---:|---:|
| 共享主台账 | 1520 | 1587 |
| armv7l 覆盖层 | 43 | 208 |
| aarch64 覆盖层 | 46 | 214 |
| x86_64 覆盖层 | 41 | 208 |

每个架构的共享层与本架构覆盖层互斥且完整覆盖该架构全部生产 TIER1
分量：armv7l 1563、aarch64 1566、x86_64 1561。

## 波次预归属

包级标签继承 `p1/base_unified_split/affected_base.tsv` 与
`affected_unified.tsv`；D-G3 对 abseil-cpp、boost-test 和 Base 六包作
显式覆盖。分量行不擅自消解冲突：

- 一个分量只有一种已知标签：`SINGLE_LABEL`；
- 同分量出现多种包级标签：`MIXED_PACKAGE_LABELS`，保留全部标签供排期；
- split 清单没有标签：`UNASSIGNED_NO_SPLIT_LABEL`，不静默猜成长尾。

`boost-test` 因生产过滤不进入 TIER1 分量台账，但其 D-G3 长尾决策保留在
`package_wave_preassignment.tsv`。Base 先行六包见 `base_first_6.tsv`。

## 文件

- `shared_main_tier1.tsv`：1520 个共享分量；
- `overlay_<arch>_tier1.tsv`：每架构差异分量；
- `package_wave_preassignment.tsv`：机械标签及 D-G3 覆盖；
- `validation.tsv`：共享/覆盖互斥、完备和计数硬断言。
