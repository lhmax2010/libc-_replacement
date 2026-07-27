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
3. Base 先行波执行名单修订为以下 **5 包**：
   `boost-filesystem`、`boost-iostreams`、`boost-thread`、`libsigc++`、
   `taglib`；`boost-log` 作为 `BASE_FIRST_COMPANION` 与
   `boost-filesystem`/`boost-thread` 同窗，不计入五包名单。

## D-G4 构建单元与晋级单元分层

三架构冻结 RPM 与 census v2 的 source-unit 反查给出：

- `boost-1.83.0-5.1.src.rpm` 实际输出为 armv7l **31**、
  aarch64 **28**、x86_64 **31** 个；aarch64 不产
  `boost-context/boost-coroutine/boost-fiber`。三架构全部输出的文件清单
  实查 `.a=0`，`boost-* = 1.83.0-5.1` 型精确包锁为 0；
- Base-first 的 Boost TIER1 晋级闭包稳定为
  `boost-filesystem/boost-log/boost-thread` 三包加
  `boost-iostreams` 单点，共四个输出；T1-0008 中 Boost 输出只有
  `boost-program-options`；
- `capi-appfw-capmgr` source 每架构 3 个输出；`security-manager`
  source 每架构 8 个输出，其中另一个已知波次成员只有
  `security-license-manager=WAVE_1`，未发现第三个未登记跨波输出；
  `libsigc++`、`taglib` source 均各 2 个输出（runtime + devel），`.a=0`。

裁决：

> 构建单元与晋级单元分层：D5 构建单元 = source RPM（同源全部输出同一
> stdlib 构建，不可拆）；晋级单元 = TIER1 分量闭包（镜像准入粒度）。
> 每批构建产出逐 RPM 晋级台账：`ADMIT`（分量闭包在批内）/
> `HOLD_SIBLING`（镜像禁入，存量制品 NEVRA+SHA256 保持权威，由后续
> 批次重产晋级）。HOLD 覆盖 devel 与静态归档子包；`.a` 一律随其
> source 的晋级批次走，禁止 libc++ 静态库对 libstdc++ 消费者可见。
> 晋级台账是镜像组装门强制输入；同一 source 构建的每个输出必须恰有一行，
> `ADMIT` 集合的 TIER1 分量闭包由门禁机械断言。

依赖审计未发现要求当前 Boost 输出强制同 NEVRA 合批的精确锁；现有同源
内部 Requires 在包名/`libboost_*.so.1.83.0` 层均可由 ADMIT 或保留的
HOLD 输出满足。`boost-license` 是架构 census 未收录的外部依赖，不属于
source 拆分锁；实际候选仓仍须重跑 solver。候选 Provides/Requires 相对
存量权威不一致、solver 不可解，或执行时新增精确 EVR 锁时，必须把锁定
依赖闭包改为同批 `ADMIT`，否则该批阻塞，不得以 HOLD 绕过。

权威模板为 `ledger/promotion_ledger_template.tsv`；当前取证为
`ledger/boost_source_unit_census.tsv` 与
`ledger/related_source_unit_summary.tsv`。

## D-G2/D-G3/D-G4 执行单元勘误

> 调度/晋级单元 = 名单包所在 TIER1 分量闭包；构建单元另按 D-G4 扩为
> source RPM 全输出。`MIXED_PACKAGE_LABELS` 分量在其任一成员晋级前必须
> 收敛为 `SINGLE_LABEL`：伴随成员改标 `<主标签>_COMPANION`，或经
> D1a/A6 证据把耦合边裁决为 shim 后拆分。波 1 三条件裁决结果必须回写
> 台账与 D-G3/D-G4 晋级台账。

当前机械消解：

1. `SHARED-T1-0010`：`boost-log` 改标
   `BASE_FIRST_COMPANION`，与 `boost-filesystem`、`boost-thread`
   同窗；分量状态收敛为 `SINGLE_LABEL`。
2. `SHARED-T1-0008`：在正式波 1 S4 结果产生前继续标为
   `PENDING_WAVE1_S4_OUTCOME`，但 S4 只决定该完整分量的排期分支，
   不构成拆分依据。

> T1-0008 为跨 repo TIER1 分量（Base 成员
> `boost-program-options` + Unified 成员三包），其第 7/51 行
> `CPP_ABI` 边独立于 UidSandboxing。S4 PASS 时，四包退出波 1，独立批次
> 构建 `boost`、`capi-appfw-capmgr`、`security-manager` 三个 source
> 单元；晋级台账只 ADMIT 四个 T1-0008 成员，其余同源输出
> HOLD_SIBLING。S4 不满足时，四包进入 wave1 23/26 晋级分支；
> `boost` source 加入 wave1 D5 allowlist，23 分支只 ADMIT
> `boost-program-options`，26 分支另 ADMIT
> `boost-filesystem/boost-log/boost-thread`，每个分支其余 Boost 输出
> HOLD_SIBLING。

3. `boost-program-options` 不再属于 Base 先行名单；S4 摘除生效时，
   `boost-program-options`、`capi-appfw-capmgr`、
   `libsecurity-manager-client`、`security-manager` 四包进入独立跨
   repo 原子批次；S4 不满足时四包整体进入波 1 的 23/26 路径。
4. Base-first 的构建形态固定为 `boost`、`libsigc++`、`taglib` 三个
   source 单元；晋级台账 ADMIT
   `boost-filesystem/boost-iostreams/boost-log/boost-thread`、
   `libsigc++`、`taglib`，其余同源输出全部 HOLD_SIBLING。

数据依据：

- `p1/base_unified_split/base_first_8_verification.tsv`
- `p1/base_unified_split/base_first_component_diffs.tsv`
- `p1/base_unified_split/summary.md`
- `p1/base_unified_split/affected_base.tsv`
- `census_multiarch_v2/armv7l/components_production_tier1.tsv`
- `ledger/boost_source_unit_census.tsv`
- `ledger/related_source_unit_summary.tsv`

## 生效与边界

三项裁决自 2026-07-27 起生效。本记录只治理排期、台账分层和数据口径，
不修改冻结 HLD 正文，也不替代每次执行的校准门、门禁或发布签核。

本勘误随 P1 出口评审整改三轮 D-G4 生效；S4 结果尚未产生，因此
`SHARED-T1-0008` 不得提前按任一分支记为 PASS，也不得在任一分支中
拆分四个晋级成员。任何 source 构建若没有覆盖其全部输出的 ADMIT/HOLD
台账，同样不得开工。
