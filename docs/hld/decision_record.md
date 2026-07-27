# libc++ 迁移治理裁决记录

裁决人：lhmax2025  
裁决日期：2026-07-27  
数据基线：Base `tizen-base_20260709.234055` + Unified
`tizen-unified_20260723.231947`，三架构对齐 census v2

## D-G1 核心波快照治理

裁决：**方法论冻结、数据现势化**。

- D2、A8、A9 的图修正规则、生产过滤规则、校准门和迁移门禁冻结不变；
- 冻结 armv7l 130 包保留为方法论复现基线和 HQ 报告口径；
- 执行台账不把 130 当作永久成员清单，而是在每次执行时消费执行时点
  快照；校准门精确复现冻结成员集是运行新快照的硬前置；
- 当前 0723 执行参考值为 armv7l/aarch64/x86_64
  **118/121/122**，后续数字变化属于数据现势化，不重开方法论。

数据依据：

- `census_multiarch_v2/METHOD.md`
- `census_multiarch_v2/summary.md`
- `census_multiarch_v2/comparison/regression_assertions.tsv`
- `census_multiarch_v2/comparison/core_v2_arm_reference_counterparts.tsv`
- `census_multiarch_v2/calibration_arm_0716/`

## D-G2 台账口径

裁决：**共享主台账 + per-arch 覆盖层**。

- 分量的成员集和有效边集摘要在三架构上精确相同，才进入共享主台账；
- 任一成员或边差异均进入对应架构覆盖层，不允许按包数、名称相似度或
  TIER1 同构推断 TIER2 同构；
- 判定由 `member_edge_sha256` 机械执行，人工不得改写为“近似共享”。

0723 TIER1 数据给出共享分量 **1520** 个；per-arch 覆盖层分别为
armv7l **43**、aarch64 **46**、x86_64 **41** 个。

数据依据：

- `census_multiarch_v2/comparison/component_isomorphism.tsv`
- `census_multiarch_v2/comparison/shared_ledger_candidates.tsv`
- `census_multiarch_v2/comparison/isomorphism_summary.tsv`
- `census_multiarch_v2/comparison/ledger_model_recommendation.tsv`

## D-G3 Base 先行名单

裁决：

1. `boost-test` 从 Base 先行摘除并转长尾：其名称命中生产过滤规则，
   生产图方法不可判定，且已观测生产入边为 0；
2. `abseil-cpp` 移出独立 Base 先行，与波 2 vision/ML 的 44 包分量同窗；
3. Base 先行波执行名单定为以下 **6 包**：
   `boost-filesystem`、`boost-iostreams`、`boost-program-options`、
   `boost-thread`、`libsigc++`、`taglib`。

## D-G2/D-G3 执行单元勘误

> 执行单元 = 名单包所在 TIER1 分量闭包。MIXED_PACKAGE_LABELS 分量在其任一成员开工前必须收敛为 SINGLE_LABEL:伴随成员改标 <主标签>_COMPANION,或经 D1a/A6 证据把耦合边裁决为 shim 后拆分。波 1 三条件裁决结果必须回写台账与 D-G3。

当前机械消解：

1. `SHARED-T1-0010`：`boost-log` 改标
   `BASE_FIRST_COMPANION`，与 `boost-filesystem`、`boost-thread`
   同窗；分量状态收敛为 `SINGLE_LABEL`。
2. `SHARED-T1-0008`：在正式波 1 S4 结果产生前标为
   `PENDING_WAVE1_S4_OUTCOME`。四包摘除生效时，
   `boost-program-options` 按 `BASE_FIRST` 独立，三包留长尾；四包
   保留时，整个分量服从波 1 条件分支。
3. D-G3 的六包名单是名义执行名单；其中 `boost-program-options`
   的实际执行归属由上述 S4 分支回写后生效。

数据依据：

- `p1/base_unified_split/base_first_8_verification.tsv`
- `p1/base_unified_split/base_first_component_diffs.tsv`
- `p1/base_unified_split/summary.md`
- `p1/base_unified_split/affected_base.tsv`
- `census_multiarch_v2/armv7l/components_production_tier1.tsv`

## 生效与边界

三项裁决自 2026-07-27 起生效。本记录只治理排期、台账分层和数据口径，
不修改冻结 HLD 正文，也不替代每次执行的校准门、门禁或发布签核。

本勘误随 P1 出口评审 C2 生效；S4 结果尚未产生，因此
`SHARED-T1-0008` 不得提前按任一分支记为 PASS。
