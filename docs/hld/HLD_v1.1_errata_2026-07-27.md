# HLD v1.1 冻结版勘误页

适用正文：
`freeze_v1.1/docs/HLD_libcxx_migration_v1.1_frozen.md`  
正文 SHA256：
`cdc048e3c377baa2602c3d0cfa182c434877ad45fd1106d83ba3f5d1c3c57d3f`  
勘误日期：2026-07-27  
依据裁决：D-G1、D-G2

> 本页是治理附页，不回写冻结正文。冻结正文中的 130、402、513 继续
> 作为 0716 armv7l 方法论基线和评审复现读数；执行成员清单由本页规定。

## Errata 1：核心波规模与成员清单

涉及正文 §2、§5、§10 中将“核心 130”用于规划或执行的表述。

勘误后口径：

- **方法论/HQ 报告口径**：冻结 armv7l 核心下界 130，以及其
  UB_direct 402 / UB_closure 513，原样保留；
- **执行口径**：先在冻结快照通过精确成员集校准门，再对执行时点快照
  重算并消费其核心成员清单；
- **0723 当前参考值**：armv7l 118、aarch64 121、x86_64 122；
- 执行时点数字与 130 不同，按“数据现势化”处理，不据此修改 D2/A8/A9
  规则或绕过校准门。

现势数据源：

- `census_multiarch_v2/METHOD.md`
- `census_multiarch_v2/summary.md`
- `census_multiarch_v2/comparison/core_v2_arm_reference_counterparts.tsv`

## Errata 2：波次台账的三架构分层

涉及正文 §5 的波次规划以及 §9 的 allowlist/波次台账。

勘误后口径：

- 共享主台账只接收三架构**成员集和有效边集摘要均精确相同**的分量；
- 任一成员或边差异必须进入 per-arch 覆盖层；
- 共享判定使用 `member_edge_sha256`，不接受规模相同、名称近似或
  TIER1/TIER2 间的类推；
- 0723 TIER1 共享主台账为 1520 分量；覆盖层为
  armv7l 43、aarch64 46、x86_64 41 分量。

现势数据源：

- `census_multiarch_v2/comparison/component_isomorphism.tsv`
- `census_multiarch_v2/comparison/shared_ledger_candidates.tsv`
- `census_multiarch_v2/comparison/isomorphism_summary.tsv`

## 变更记录

| 日期 | 裁决 | 内容 |
|---|---|---|
| 2026-07-27 | D-G1 | 区分冻结方法论基线与执行时点快照 |
| 2026-07-27 | D-G2 | 台账固定为共享主层 + per-arch 覆盖层 |
