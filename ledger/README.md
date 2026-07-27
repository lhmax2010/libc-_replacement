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
`affected_unified.tsv`；D-G3 对 abseil-cpp、boost-test 和 Base 五包作
显式覆盖。分量行不擅自消解冲突：

- 一个分量只有一种已知标签：`SINGLE_LABEL`；
- 同分量出现多种包级标签：`MIXED_PACKAGE_LABELS`，保留全部标签供排期；
- split 清单没有标签：`UNASSIGNED_NO_SPLIT_LABEL`，不静默猜成长尾。

`boost-test` 因生产过滤不进入 TIER1 分量台账，但其 D-G3 长尾决策保留在
`package_wave_preassignment.tsv`。Base 先行五包见
`base_first_6.tsv`；文件名为历史兼容名，现行内容以五行台账为准。

## 执行单元消解规则

> 执行单元 = 名单包所在 TIER1 分量闭包。MIXED_PACKAGE_LABELS 分量在其任一成员开工前必须收敛为 SINGLE_LABEL:伴随成员改标 <主标签>_COMPANION,或经 D1a/A6 证据把耦合边裁决为 shim 后拆分。波 1 三条件裁决结果必须回写台账与 D-G3。

0723 台账的两个已知冲突按 C2 处理：

- `SHARED-T1-0010` 已收敛：`boost-log=BASE_FIRST_COMPANION`，与
  `boost-filesystem`、`boost-thread` 同窗，状态为 `SINGLE_LABEL`；
- `SHARED-T1-0008` 尚不能二选一，状态为
  `PENDING_WAVE1_S4_OUTCOME`。S4 摘除生效后，完整四包分量退出波 1，
  作为 Base+Unified 成对晋级的独立跨 repo 原子批次；S4 不满足时，
  完整四包分量进入波 1 的 `ACTIVE_BATCH=23/26` 条件分支。
  `cross_repo_edges.tsv` 第 7/51 行的 CPP_ABI 边在两分支中始终位于
  同一批次，结果产生前禁止开工。

`BASE_FIRST_COMPANION` 在分量归属上归一为主标签 `BASE_FIRST`，但保留
后缀以说明该包不是现行五包名单成员。

## 文件

- `shared_main_tier1.tsv`：1520 个共享分量；
- `overlay_<arch>_tier1.tsv`：每架构差异分量；
- `package_wave_preassignment.tsv`：机械标签及 D-G3 覆盖；
- `validation.tsv`：共享/覆盖互斥、完备和计数硬断言。
- `member_edge_sha256_spec.md`：成员+边摘要的字段、排序、转义和哈希规格。
- `anchor_exemptions_per_arch.tsv`：D2 架构独立锚点豁免覆盖层。

`member_edge_sha256` 必须包含 `edge_class`；跨类负面对照见
`member_edge_sha256_cross_class_fixture.tsv`。新增架构锚点豁免按
HLD errata v2 绑定 source-NEVRA，并作为 9 月开工前置。
