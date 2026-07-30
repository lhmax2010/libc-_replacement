# P1 出口对抗评审结论

- 评审输入：`p1_exit_review_v1.tar.gz`
- 声明 SHA256：`1ca767dfb826291e7e9b118521b59d3595c99db6aeb865a2512a5d83b57712ec`（实测一致）
- 内部 `MANIFEST.sha256`：168 项，全部校验通过
- 评审日期：2026-07-27
- 评审方：reviewer_kimi

## 总判定

**FAIL — P1 不可关账、执行期不可直接开工。**

阻塞项见下文。非阻塞改进项与限制条件写入 `open_questions_2026-07-27.md`。

## 第一层：NEW_SINCE_FREEZE 增量审查

### a. D-G1 快照治理（decision_record.md + HLD_v1.1_errata_2026-07-27.md）

**DISAGREE / 阻塞**

- 证据：
  - `corpus/docs/hld/decision_record.md` 第 8–16 行：将“校准门精确复现冻结成员集”作为“运行新快照的硬前置”，并把 0723 参考值 118/121/122 仅视为“数据现势化”。
  - `corpus/census/multiarch_v2/METHOD.md` 第 20–24 行：A8 仅当“当前快照存在相同 CPP_NOSTL 边”时才提升 33 条冻结布局边；A9 仅当“当前快照存在相同 CPP_NOSTL 边”时才应用 5 条冻结 YES_POINTEE 边；且“不在校准或对齐快照中扩大核心”。
- 反例构造：
  - 假设 0723 之后的新快照出现一条未在冻结 33 条 A8 边 / 5 条 A9 边列表中登记过的 CPP_NOSTL 边 `P → Q`，且该边确实携带 STL 布局。
  - 由于 A8/A9 规则要求“当前快照已存在相同 CPP_NOSTL 边”才能触发提升，而冻结列表中没有这条边，该边不会被提升为 TIER1。
  - 结果：`P` 与 `Q` 被保留在 TIER2 或 unknown，可能在不同波次被迁移，运行期出现 STL 布局/异常跨界断裂。
  - 校准门跑在冻结 0716 快照上，只能证明管线与冻结快照一致，无法发现这条新边；执行时却可能据此做波次划分。
- 结论：治理记录把“数据现势化”等同于“不重开方法论”，但没有为新边/消失边定义机械 triage 规则。校准门是必要前置，不是充分前置。
- 置信度：高

### b. D-G2 台账分层（ledger/README.md 及 TSV）

**AGREE / 非阻塞**

- 证据：
  - `corpus/ledger/shared_main_tier1.tsv` 中仅 2 行 `MIXED_PACKAGE_LABELS`：`SHARED-T1-0008`（boost-program-options/capi-appfw-capmgr/libsecurity-manager-client/security-manager）和 `SHARED-T1-0010`（boost-filesystem/boost-log/boost-thread）。
  - `corpus/docs/hld/decision_record.md` 第 46–55 行 D-G3 已将 boost-filesystem/program-options/thread 列为 Base-first 六包，capi-appfw-capmgr/libsecurity-manager-client/security-manager 进入 `wave1/plan.md` 的条件化台账。
- 结论：当前 MIXED 情况已被 D-G3 与 wave1 计划条件化覆盖；台账“保留全部标签供排期”的口径虽无通用消解规则，但在现有数据集中不造成阻塞。
- 置信度：高

### c. census v2 方法论等价性（METHOD.md / methodology_equivalence.tsv）

**DISAGREE / 与 D-G1 合并为同一阻塞项**

- 证据：
  - `corpus/census/multiarch_v2/methodology_equivalence.tsv` 第 4–7 行证明 A8/A9/TIER1/TIER2 在 0716 校准快照上完全复现。
  - 但 `METHOD.md` 第 20–24 行明确 A8/A9 的提升条件与“当前快照”的 CPP_NOSTL 边存在性绑定。
- 反例：同 D-G1 节。
- 结论：方法论等价性只在校准快照上成立，不能外推到新快照。单独把“校准门复现”作为“管线等价”的判据，会低估新快照的降级/漏升风险。
- 置信度：高

### d. D5 机制（d5_mechanism.md / macro_definition_snippet.conf）

**DISAGREE / 阻塞**

- 证据：
  - `corpus/d5/d5_mechanism.md` 第 3 行状态：`POC_PASS_ADOPTION_BLOCKED_PENDING_CONFIG_DECISION`。
  - 同文件第 222–231 行：PoC 未修改原始 `gbs_llvm.conf`、S6 buildconf 或 `command.txt`；正式采用需要“两处外部状态变化”，当前状态为“在此停下等待裁决”。
  - 同文件第 126–131 行：D5 落地后 S6 必须重新取证并冻结。
- 结论：
  - 设计机制在隔离 buildconf 中 PoC 通过，但尚未进入实际构建系统。
  - 波 1 乃至所有波次都依赖 per-package allowlist（见 `wave1/plan.md` §Step 1）。
  - 在 `gbs_llvm.conf` / S6 project config 未正式修改前，项目无法真正进入“执行期”并开工波 1。因此 D5 落地阻塞是 P1 出口的硬阻塞。
- 置信度：高

### e. 门禁工具（gates/ fixtures + 工具脚本）

**AGREE / 非阻塞**

- 证据：
  - 按 `gates/fixture_catalog.tsv` 13 项矩阵，本地复跑 13 个 fixture，得到 10 RED / 3 GREEN，与 `gates/FROZEN_EXECUTION_VERSION.md` 声明一致。
  - 结果摘要：
    - `edge_diff_check.py`：clean GREEN，new_cpp_abi_edge RED，removed_edge_registry_active RED，soname_change_undeclared RED。
    - `tunnel_registry_check.py`：clean GREEN，unregistered_voidptr_edge RED，registered_signature_changed RED，registry_symbol_disappeared RED。
    - `g7_exception_gate.py`：clean GREEN，escape_unknown RED，rider_callback_count_not_one RED，rider_stale_request RED，rider_terminate_occurred RED。
- 结论：冻结版本声明的 13 项矩阵可机械复现。
- 置信度：高

### f. wave4 通知（base_unified/wave4_notice_*.md）

**AGREE / 非阻塞**

- 证据：
  - 两语言通知均声明 0723 三架构核心为 118/121/122，其中 Base 成员固定为 `jsoncpp`、`libicu`、`libpcrecpp`，与 `base_unified/core_base_members.tsv` 一致。
  - 均注明“执行清单以执行时点快照为准；130 为冻结方法论基线”，与 D-G1 勘误口径一致。
- 结论：通知口径一致，未出现会把 130 当成执行清单的过度承诺。
- 置信度：高

### g. 排期勘误与 packaging 报告（docs/schedule / packaging/multiarch_packaging_report.md）

**DISAGREE / 阻塞**

- 证据：
  - `corpus/docs/schedule/libcxx_migration_schedule.md` 第 34 行：8 月交付项仍写“波 1 cynara（12 包，armv7l 先行）”。
  - `corpus/wave1/plan.md` 第 85–93 行：当前批次为“26 行条件化预台账”，存在 19/23/26 三个分支；原 12 包只是 cynara 双分量，扩展后至少 19 包。
  - `corpus/wave1/startup_conditions.md` 第 103 行：`NOT_READY`，当前唯一红项为 14 项 round3 API 未签核。
- 结论：里程碑排期仍用旧的 12 包口径，而最新 wave1 执行方案已扩展为 19/23/26 条件分支。该口径差异会导致 8 月交付范围、资源与验收清单被低估。属于跨文档不一致，阻塞 P1 出口。
- 置信度：高

## 第二层：全 corpus 一致性

| 数字口径 | 涉及文档 | 结论 |
|---|---|---|
| 核心波下界 130 / 0723 参考 118/121/122 | `decision_record.md`、`HLD_v1.1_errata_2026-07-27.md`、`wave4_notice_*.md`、`libcxx_migration_schedule.md` | 一致，区分方法论基线 vs 执行快照 |
| 三架构 Base 核心 3 包（jsoncpp/libicu/libpcrecpp） | `core_base_members.tsv`、`wave4_notice_*.md` | 一致 |
| 共享台账 1520 + 覆盖层 43/46/41 | `ledger/README.md`、`ledger/validation.tsv`、`decision_record.md`、`HLD_v1.1_errata_2026-07-27.md` | 一致 |
| 波 1 包数 | `libcxx_migration_schedule.md` 写 12；`wave1/plan.md` 写 19/23/26 | **不一致，阻塞** |
| D5 状态 | `d5_mechanism.md` 写 `ADOPTION_BLOCKED`；`libcxx_migration_schedule.md` 写 8 月“buildconf 全局可用” | 风险：排期假设 D5 决策将在 8 月前完成，但 P1 出口未闭合该决策 |

## 阻塞项清单

1. **D5 机制未落地**：PoC 通过但官方 `gbs_llvm.conf`/S6 project config 未修改，`ADOPTION_BLOCKED_PENDING_CONFIG_DECISION`。没有 per-package allowlist，波 1 无法开工。
   - 闭合路径：平台正式裁决并修改 buildconf；重新冻结 S6 命令与 SHA。
2. **D-G1 新快照边界规则缺失**：校准门仅验证管线与冻结 0716 快照等价，未定义新 CPP_NOSTL/A8/A9 边的 triage 规则，存在新快照漏升/误降级风险。
   - 闭合路径：在治理记录中补充“新快照与冻结快照的 diff 必须人工/机械 triage 并更新 A8/A9 边冻结列表”的规则，或把未知边进入核心波必须经过 G6/G7/边差分门作为硬前置。
3. **schedule 与 wave1 plan 的波 1 口径不一致**：里程碑仍写 12 包，实际执行方案为 19/23/26 条件分支。
   - 闭合路径：更新 `libcxx_migration_schedule.md` 第 34 行及对应判据，与 `wave1/plan.md` 的 19/23/26 分支对齐。

## 置信度汇总

- D5 阻塞：高
- D-G1 新快照边界：高
- schedule/wave1 口径不一致：高
- 其他层：高

## 最终判定

**FAIL**
