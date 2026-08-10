# R24：分阶段划分的可迁移性证明与口径调和

## 结论摘要

本任务只复用既有 R11/R13/R20/R22 与 aligned-0723 数据，没有重扫平台仓、构建、安装或修改既有文档。

1. R22 修正版迁移图为 479 个待迁源码 RPM 节点、1,222 条真 C++ ABI 边；另有 4 个厂商源码 RPM 身份单列，交付总分母 483。
2. 479 个 SCC 全为单节点：非平凡 SCC 0/479，最大 SCC 为 1，非平凡 SCC 覆盖 0/479 个源码包。因不存在非平凡 SCC，“所有非平凡 SCC 均未跨阶段”机械为 PASS，但该事实不能替代阶段内部边检查。
3. 第一、二、三阶段内部真 C++ ABI 边分别为 **35、185、8**；三个阶段都不是“内部独立包集合”。provider-first 分层分别为 **5、3、3 层**，逐层包数为第一阶段 `2/4/3/3/1`、第二阶段 `333/85/32`、第三阶段 `10/5/1`。
4. R20 初版 1,705 条候选边中确实删除过边：445 条纯 C 接口边和 26 条无动态符号交集边被排除出 C++ ABI 图。初版 462 条逆向边中，452 条真边全部保留并通过重新分阶段消除逆向关系；另 8 条纯 C 接口边和 2 条无交集边被删除。
5. aligned-0723 的 **118/121/122 不是 ELF 数**，而是各架构 `tier1_production_0001` 核心 SCC 的生产二进制包 Name 数；对应内部有效边为 198/201/204。将其按同一快照的 `package_presence.tsv` 精确折算为源码构建单元，armv7l/aarch64/x86_64 分别是 **62/65/64 个源码 RPM**；三架构并集为 **69 个源码包 Name、72 个 source-NVR 身份**。
6. R22 第三阶段 16 个源码 RPM 的实际构成为：非平凡 SCC 成员 0、插件宿主 0、两者皆是 0、按 provider-first 深度 3–5 纳入 16。它不是 aligned-0723 核心 SCC 的源码包计数，二者只在每架构重合 7 个源码包 Name（对应 13 个二进制包 Name）。

## 一、分阶段划分的可迁移性证明

### 1. SCC 披露

SCC 由 `tables/scc_membership.tsv` 全量披露，摘要见 `tables/scc_summary.tsv`：

| 指标 | 实测值 | 分母/单位 |
|---|---:|---|
| SCC 总数 | 479 | 479 个待迁源码 RPM |
| 非平凡 SCC | 0 | 479 个 SCC |
| 最大 SCC | 1 | 源码 RPM |
| 非平凡 SCC 覆盖包 | 0 | 479 个待迁源码 RPM |
| 跨阶段非平凡 SCC | 0 | 0 个非平凡 SCC |

`tables/nontrivial_scc.tsv` 只有表头，表示实测零项，不是未执行。不存在必须作为循环整体迁移的多包组，也不存在被阶段边界拆开的 SCC。

### 2. 是否删边

R20 初版以“provider ELF 导出 Itanium 修饰符号，或自身 DT_NEEDED C++ 运行时”为候选边条件，共得到 1,705 条源码边（`progress/R19_21/R20/R20_migration_phases_report.md:9-15`）。R22 对消费者 UND 与 provider DEF 动态符号做实交后重判（`progress/R22/R22_migration_report.md:9-14`）。

| 范围 | 处置 | 边数 | 分母 |
|---|---|---:|---:|
| 全部 R20 候选 | 保留真 C++ ABI 边 | 1,234 | 1,705 |
| 全部 R20 候选 | 删除纯 C 接口边 | 445 | 1,705 |
| 全部 R20 候选 | 删除无动态符号交集边 | 26 | 1,705 |
| 初版 462 条逆向边 | 真边保留并重排阶段 | 452 | 462 |
| 初版 462 条逆向边 | 从 ABI 图删除 | 10 | 462 |

所以不能申报“仅重排与合并，未删边”。准确表述是：**候选图净化删除了 471 条不满足修正版 C++ ABI 判据的边；462 条旧逆向边中的 452 条真边未删除，靠重新分阶段消除逆向关系，10 条非 ABI 边被删除。**

- 471 条完整清单和逐边依据：`tables/deleted_candidate_edges.tsv`。
- 原 462 条逐边去向：`tables/initial_462_edge_disposition.tsv`。

### 3. 阶段内部耦合

| 阶段 | 源码包数/483 | 内部 C++ ABI 边/1,222 | 非平凡 SCC | provider-first 层数 | 准确性质 |
|---|---:|---:|---:|---:|---|
| 第一阶段 | 13/483 | 35/1,222 | 0 | 5 | 有内部依赖，须按层次处理；不是内部独立集 |
| 第二阶段 | 450/483 | 185/1,222 | 0 | 3 | 有内部依赖，不能称为 independent bulk |
| 第三阶段 | 16/483 | 8/1,222 | 0 | 3 | 有内部依赖，须按层次处理；不是内部独立集 |

阶段边矩阵见 `tables/stage_edge_matrix.tsv`：逆向阶段边为 0，但内部边合计 228。这证明“无较早阶段消费者依赖较晚阶段 provider”，不证明同阶段包彼此独立。

第二阶段的完整子序在 `tables/stage_topology_order.tsv`，层摘要在 `tables/stage_topology_layers.tsv`：

- 第 0 层：333/450 个源码 RPM；在第二阶段诱导子图内不依赖仍未处理的第二阶段 provider。
- 第 1 层：85/450。
- 第 2 层：32/450。

第一阶段为 5 层 `2/4/3/3/1`，第三阶段为 3 层 `10/5/1`。这里的层只表示本次实测动态 C++ ABI 图中的 provider-first 偏序，不覆盖方法盲区中的边。

### 4. 初版与修正版规则差异

R20 初版规则原文在 `progress/R19_21/R20/R20_migration_phases_report.md:17-29`：第一阶段使用首批/测试框架和底层节点，第三阶段使用非平凡 SCC、插件宿主或厂商消费者，第二阶段只收无入边或仅依赖第一阶段者，其余记 UNKNOWN。该规则产出 30/311/17/121/4，并留下 462 条逆向边（同报告 `:40-44`）。

R22 实现先净化候选边，再以首批五包和 gtest 的完整 provider 闭包形成第一阶段；其余 SCC 按 provider-first 深度 0–2/3–5 分成第二/三阶段（`progress/R22/tools/rephase_migration.py:59-91`），产出 13/450/16/4。

必须更正一处描述：R22 **实现后的**第三阶段并非“多包 SCC 成员 + 插件宿主”。实测 16/16 全部因深度超过阈值 2 纳入，非平凡 SCC 与插件宿主均为 0/16；逐包见 `tables/phase3_composition.tsv`。

### 5. 边判定与盲区

R22 实际调用 `readelf --dyn-syms -W`，把版本化符号按首个 `@` 去掉版本后缀，再求消费者 UND 与 provider DEF 交集（`progress/R22/tools/classify_abi_edges.py:49-83`）。交集至少有一个 `_Z` 开头符号才保留为 C++ ABI 边。

- 弱 UND：原实现未按 Bind 过滤，与其他 UND 同口径纳入。
- 静态库边：未纳入。
- header-only 接口：未单列；只有落到动态符号/DT_NEEDED 的影响可见。
- `dlopen` 边：未纳入。
- 13,167/151,403 条未唯一解析 DT_NEEDED 边：未推断补齐。
- 无动态 UND 的布局、内联、pointee/tunnel 耦合：未纳入。aligned census 的 A8/A9 提升属于另一图规则。

这些边界在 `METHODOLOGY.md` 单列。因输入图不覆盖上述类别，本报告的“可迁移性证明”严格限定于 R22 已实测动态 C++ ABI 图。

## 二、紧耦合口径调和

### 6. 118/121/122 的原始对象

原始出处为：

- `census/multiarch_v2/summary.md:13-17`：单位明确写为 Production packages；
- `census/multiarch_v2/comparison/core_v2_arm_reference_counterparts.tsv:2-4`：三个 `tier1_production_0001` 的包数与内部边为 118/198、121/201、122/204；
- `ledger/overlay_<arch>_tier1.tsv`：字段名为 `pkg_count` 和 `pkg_list`。

因此“118–122 为 ELF 级测量”的表述与原始文件不符。准确单位是**生产二进制包 Name**。其快照为 Base `tizen-base_20260709.234055` + Unified `tizen-unified_20260723.231947`，图是 D2+A8+A9 修正后的 TIER1 生产包图。

### 7. 第三阶段 16 的构成

| 构成 | 源码包数/16 |
|---|---:|
| 非平凡 SCC 成员 | 0/16 |
| 插件宿主 | 0/16 |
| 两者皆是 | 0/16 |
| R22 实现的深度 3–5 | 16/16 |

所以 118–122 与 16 不是同一对象：前者是旧快照最大核心 SCC 的二进制包成员数，后者是较后 R11/R22 快照上按拓扑深度划出的源码 RPM 阶段。

### 8. 同口径对照

| 架构 | 旧核心二进制包 | 旧核心折算源码 RPM | R22 第三阶段源码 RPM | R22 第三阶段二进制 RPM | R22 第三阶段运行时 ELF | 同名交集：源码/二进制 |
|---|---:|---:|---:|---:|---:|---:|
| armv7l | 118 | 62 | 16 | 88 | 331 | 7/13 |
| aarch64 | 121 | 65 | 16 | 89 | 330 | 7/13 |
| x86_64 | 122 | 64 | 16 | 89 | 330 | 7/13 |

旧核心的运行时 ELF 数在 retained aligned-0723 结论材料中不可得，记 `NOT_AVAILABLE`；未用 R11 的较后 ELF 数反推。逐成员的旧二进制包到旧 source-NVR 映射见 `tables/historical_core_members_source_mapping.tsv`，361/361 行均有来源字段实测映射。

快照、节点和边规则三项均不同，无法把数值差异按原因做可加和归因；该不可分离部分在 `tables/reconciliation_rule_snapshot_differences.tsv` 标为 `NOT_SEPARABLY_IDENTIFIABLE_FROM_RETAINED_INPUTS`。可机械量化的集合差异在 `tables/reconciliation_same_unit_differences.tsv`：每架构只有 7 个源码包 Name、13 个二进制包 Name 重合。

### 9. 11 月核心框架的源码包规模

按 Appendix 所引用的 aligned-0723 核心 `tier1_production_0001` 且将迁移/构建单位统一为源码 RPM：

- armv7l：**62 个源码 RPM**；
- aarch64：**65 个源码 RPM**；
- x86_64：**64 个源码 RPM**；
- 三架构集合并集：**69 个源码包 Name**；若按架构间不同 source-NVR 身份不合并，则为 **72 个 source-NVR 身份**。

这回答的是 0723 参考快照的 11 月核心框架规模。既有执行规则要求开工时对执行快照重算，因此未来执行时点的数字不由本任务外推。R22 第三阶段的 16 个源码包不是这一核心规模的替代口径。

## 三、交付清单

固定交付路径：

- `progress/R24/tables/migration_packages_483.tsv`
- 策展副本：`docs/progress/R24/tables/migration_packages_483.tsv`

文件为 483 行源码 RPM 身份，字段包括包名、阶段、二进制包数、源码修改需求、构建配置修改需求、非平凡 SCC 编号、插件宿主、厂商预编译、双运行时风险集和备注。因为实测无非平凡 SCC，479 个迁移包的 `scc_id` 均为 `-`；4 个厂商单列对象同样为 `-`。

索引为 `tables/migration_packages_483_INDEX.tsv`，阶段汇总为 `tables/migration_packages_483_summary.tsv`。本任务所有单文件均小于 5 MB，无需拆分或排除。

## 数据来源与复现

输入文件 SHA256 在 `inputs/input_identity.tsv`，仓/快照/repomd 在 `tables/data_sources.tsv`，工具和脚本标识在 `tables/tooling_identity.tsv`，命令原文与退出码在 `commands/`。

aligned-0723 旧核心材料保留了 snapshot 名称与 URL，但没有落盘 repomd SHA256，因此该两行明确记 `NOT_AVAILABLE`；R11/R22 的四个 composite repo revision 与 repomd SHA256 均齐备。

本报告不含排期、人日、优先级建议，也没有修改任何既有文档。
