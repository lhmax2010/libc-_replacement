# R22 第一部分：分阶段迁移清单修正

## 输入、对象与口径

本轮没有重新扫描平台仓。输入文件及 SHA256 在 `inputs/input_identity.tsv`；仓快照、对象形态与时间口径在 `tables/data_sources.tsv` 和 `tables/download_time_ranges.tsv`。阶段清单单位是精确 NEVRA 形态的源码 RPM 身份：适配集合 483/1,450 个仓内源码 RPM 记录；对应二进制 Name 数按源码身份内去重后合计 1,768。图和阶段分析对象均为 R11 下载的发布形态 RPM，不是开发板已安装的运行形态。

R11 运行时图分母为 36,829 个 ET_EXEC/ET_DYN ELF、11,332 个二进制 RPM 记录。151,403 条 DT_NEEDED 边中 138,236 条唯一解析（91.30%），13,167 条不可唯一解析（8.70%）；不可唯一解析边没有推断补全。R22 针对 1,705 条源码包候选边涉及的 9,234 个本地缓存 ELF 执行定向动态符号提取，9,234/9,234 成功；这不是平台仓重扫。

## 逆向边归因

对每条 A→B 候选边，以具体消费者 ELF 的 UND 动态符号与其 DT_NEEDED 实际解析到的 provider ELF 的 DEF 动态符号求交：交集中至少一个符号以 `_Z` 开头，才记真正 C++ ABI 耦合；交集全为非 `_Z` 则记纯 C 接口；无交集单列，不冒充 C++ ABI 边。逐 ELF/DT_NEEDED/符号证据见大表，逐源码边汇总见 `tables/source_edge_classification.tsv`。

- 全部 1,705 条候选源码边：1,234 条真 C++ ABI 耦合，445 条纯 C 接口，26 条没有符号交集，NOT_AVAILABLE 0。
- R20 的 462 条逆向边：452 条真 C++ ABI 耦合，8 条纯 C 接口误判，2 条没有符号交集，NOT_AVAILABLE 0。完整逐条归因见 `tables/reverse_edge_attribution.tsv`。

## UNKNOWN 归因

R20 的 121/121 个 UNKNOWN 均不是“依赖不可解析、无 ELF或分类文件缺失”，而是同一旧规则缺口：单节点、非插件包只要存在第一阶段之外的 ABI 依赖，就既不满足旧第二阶段条款，也未命中旧第三阶段条款。逐包原理由 `unknown_121_attribution.tsv` 保留；R22 在 SCC 凝聚图上按 provider-first 深度机械分层后，121/121 均获得阶段，保留 UNKNOWN 归属为 0。源码/构建修改需求仍可为 UNKNOWN，未被阶段归属替代。

## 修正规则与结果

1. 从候选图剔除 445 条纯 C 接口边和 26 条无动态符号交集边，仅保留 1,234 条实证 C++ ABI 耦合边。
2. 厂商预编译对象仍以 5 个二进制 Name / 4 个源码 RPM 身份单列；没有把它们塞进迁移阶段。
3. 对其余 479 个源码身份重算 SCC。实测所有 SCC 均为单节点，因此 `coupling_components.tsv` 只有表头。
4. 第一阶段以首批五包和测试框架 gtest 为硬种子，并闭包纳入它们传递依赖的所有 provider SCC，共 13 个源码 RPM。
5. 剩余 SCC 以 A→B 表示消费者依赖 provider，从 provider 端计算拓扑深度；实测最大深度 5。深度 0–2 为第二阶段（450 个），深度 3–5 为第三阶段（16 个）。该规则确保任何较早阶段都不依赖较晚阶段。
6. 厂商单列 4 个；三阶段加单列为 483/483。

汇总见 `tables/stage_summary.tsv`，全清单见 `tables/migration_packages_full.tsv`。修正后真 C++ ABI 逆向边为 0/1,234；`tables/corrected_inverse_edges.tsv` 只有表头。多包 SCC 为 0；插件宿主、测试框架、厂商对象和 Chromium 在 `tables/special_cases.tsv` 单列。

## 交叉校验

`tables/cross_checks.tsv` 的 13 项均 PASS：总数 483/483；首批五包全在第一阶段；`libcynara-commons` 与 `security-manager` 均为源码修改 YES；gtest 在第一阶段；阶段间逆向 ABI 边 0；Chromium 正常归入第二阶段且源码/构建修改需求保持 UNKNOWN；旧 121 个阶段 UNKNOWN 全部闭合。

Chromium 使用 R19 实测事实：42/42 个运行时 ELF 动态依赖 libstdc++，libc++ 依赖 0，`std::__1` 命中 0。它没有因 T1 源码解包缺口被排除；只把源码修改需求和构建系统修改需求标 UNKNOWN。

## 483 与 372 的口径差异

372 是 T1 快照中“二进制 RPM 元数据直接依赖 libstdc++”映射并去重得到的源码包集合；371/372 成功解包扫描，Chromium 1 包失败。483 是较后 R11 快照中“至少一个运行时 ELF 含 Itanium 修饰符号或 C++ 运行时 DT_NEEDED”的源码 RPM 身份集合。前者是直接 libstdc++ 依赖驱动的源码语料分母，后者还纳入静态含 C++ 特征、间接形态以及精确 release 身份，因此两数不能互换。本迁移清单以运行时形态定义的 483 为分母；R1 修改标注只在精确源码 RPM 身份匹配时复用，否则保持 UNKNOWN。

本报告不含排期、人日、优先级建议或风险评级。
