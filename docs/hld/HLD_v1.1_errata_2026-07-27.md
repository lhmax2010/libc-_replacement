# HLD v1.1 冻结版勘误页 v2

适用正文：
`freeze_v1.1/docs/HLD_libcxx_migration_v1.1_frozen.md`  
正文 SHA256：
`cdc048e3c377baa2602c3d0cfa182c434877ad45fd1106d83ba3f5d1c3c57d3f`  
勘误日期：2026-07-27  
依据裁决：D-G1、D-G2、P1 出口评审 C1/C6

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

## Errata 3：签核继承与 source-NEVRA 绑定

> 已签核裁决(API 签核/G7 台账/锚点豁免)的效力绑定其证据面的 provider/consumer source-NEVRA。执行快照中任一绑定 NEVRA 漂移,对应签核行自动降级 PROPOSED_REVALIDATE,须对新版本重跑该项源码证据面 diff 后重签;边差分门(§9)增加签核绑定包 NEVRA 漂移检测,漂移未处置即红。

机械落点：

- `wave1/api_adjudication_registry_27.tsv` 与
  `wave1/api_adjudication_candidates_round3.tsv` 的 `bound_nevra`；
- `wave1/signoff_nevra_bindings.tsv` 的逐 provider/consumer 绑定行；
- edge diff v1.1 的 `SIGNOFF_NEVRA_DRIFT` 阻塞码。

## Errata 4：执行快照新增/消失边 triage

> 执行时点快照相对上一对齐快照的新增 CPP_NOSTL 边与消失边,必须经机械 diff 清单 + 人工 triage(升格候选 /登记豁免/裁决安全)后方可发波;A6/A9 发现扫描在每次快照对齐后、核心波开工前各重跑一次。

edge diff v1.1 同时阻塞新增 `CPP_ABI`、`CPP_NOSTL` 和已升格布局边；
消失边在登记状态未收敛为 `RETIRED` 时阻塞。

## Errata 5：A8/A9 回放边界

现有 aligned census v2 的 A8/A9 步骤是冻结证据边的条件回放，不是对新
快照重新执行成员布局与 pointee 发现扫描。其校准只证明冻结输入可复现，
不能外推为新快照不存在新增隐藏耦合。实现脚本由 `REF_INDEX.md` 逐文件
SHA256 锚定。

## 变更记录

| 日期 | 裁决 | 内容 |
|---|---|---|
| 2026-07-27 | D-G1 | 区分冻结方法论基线与执行时点快照 |
| 2026-07-27 | D-G2 | 台账固定为共享主层 + per-arch 覆盖层 |
| 2026-07-27 | C1 | 增加签核 source-NEVRA 继承规则和边差分检测 |
| 2026-07-27 | C6 | 增加新边 triage、A6/A9 重跑时点与回放边界 |
