# R13：双 unwinder 共载面测算

状态：`DONE_WITH_UNDETERMINED`。本任务只复用 R11 已落盘的 ELF、DT_NEEDED、RPM 关系与插件数据；本项目 c2 制品的 DT_NEEDED 使用 R12 已落盘补充表。没有重扫平台 RPM/ELF，没有安装、构建或修改既有源码、制品和配置。报告只陈述测量结果与不可判定边界，不做方案推荐、良恶判断或工期估算。

## 1. 输入、快照与计数口径

R11 的平台身份是四组不可变 `repomd revision + SHA256` 构成的复合快照：

| 仓/类型 | revision | repomd SHA256 |
|---|---:|---|
| Base binary | 1785866115 | `1789528921ae67d70ba657c787899ec580ab0b17a8368a0245ec09440710cf16` |
| Unified binary | 1785904307 | `e425b6a91a3cfb2f67cbd73160ac48e2bb776e92866de81d5b97bb25e5ab39c3` |
| Base source | 1785866095 | `cf7fca47147304ec114e8e0c19f6f43353dbf973ee623d3eb5188919c509fe72` |
| Unified source | 1785904240 | `f15c6854277fb975d6d5e52d9a33c120eb85bb5cdc726acdf4243026098fde59` |

实际使用的输入与 SHA256 如下；完整大小和用途字段见 `inputs/input_identity.tsv`：

| 作用 | 文件 | SHA256 |
|---|---|---|
| R11 快照 | `progress/R11/tables/snapshot_identity.tsv` | `03a149f64ece004d059602c55575424d519e3608c68c69b9c7451d5029a36b20` |
| R11 二进制包 | `progress/R11/tables/binary_package_records.tsv` | `7499dd25816f65c10acc8eccca25e339cd35154880a09b242499edcd6bd9e9cc` |
| R11 源码包 | `progress/R11/tables/source_package_records.tsv` | `4ab69af58cbf16111f990a15b510590ec5310a18b91cbcc676ea609ca6134083` |
| R11 ELF | `progress/R11/tables/elf_inventory.tsv` | `64b661853e1d231d0e562faf10b4b043baf1fa31e2e3d057798cc56da729cad3` |
| R11 DT_NEEDED | `progress/R11/tables/dt_needed_edges.tsv` | `410ed99e8df7855d032cbc3b73a346f8700f8f518e14ba5d7d033b03caffb94f` |
| R11 RPM 关系 | `progress/R11/tables/binary_relations.tsv` | `56156849448677df9a4fa4554f6e0bd3f7b4a85e9ac542791c6668bd814cdf93` |
| R11 Unwind UND | `progress/R11/tables/abi_unwind_undefined_symbols.tsv` | `6b4f7f8a6182439ba14838677711acf0228a2571f98b02a5cbc6f8b613e3cd7a` |
| R11 插件包 | `progress/R11/tables/plugin_mechanism_packages.tsv` | `4ec2aa4d7e9912f436f6c7f5738f7d9938124ac33362d958317541412cba389a` |
| R11 c2 ELF | `progress/R11/tables/project_c2_elf_inventory.tsv` | `508445dcca703b7a8e0444f72fe085e67ea21efd032fbbb3dbf5f33a0745d46e` |
| R11 c2 包 | `progress/R11/tables/project_c2_packages.tsv` | `66511ea922355a11074870017ca3f30de5058ae0f28fa08b727d0381f0a8d29c` |
| R11 c2 Unwind UND | `progress/R11/tables/project_c2_unwind_undefined_symbols.tsv` | `419a57d8f92973e687b72cd6da39fbb87e65ec5ae0920b4121bc11161b13fe77` |
| R11 libunwind 依赖 | `progress/R11/tables/libunwind_dependency_metadata.tsv` | `563adfa22310a928a9a3c0b70c6483816730918941436637bdf259cff683226a` |
| R12 c2 DT_NEEDED 补充 | `progress/R12/tables/dt_needed.tsv` | `466a9f7998657b0fe6d8a60a6a65740178d41a28b6e190b7ea94db5eec22a1ae` |
| T1 源码范围 | `progress/T1/source_packages.tsv` | `5609d3424a160e23ec87861c76b6952359554f11b2e9b2f7247d8410356f046f` |
| T1 覆盖 | `progress/T1/coverage_and_occurrence.tsv` | `8a99b4aee8f48fee2ca219acb9e537242c5abd68e861bc47d38dbfd3c243b94d` |
| 5 个厂商锚 | `census/frozen_armv7l/anchors.tsv` | `98e01ce8827c4401f5a572443a8f2a6439c6b39a05a8ecb73fe83a2a13ab9dad` |

后续单位严格分开：平台全量为 11,332 个二进制包记录、1,450 个源码 RPM 身份；本任务闭包主范围为其中拥有运行时 ELF 的 6,064 个二进制包记录和 36,829 个运行时 ELF。源码层含运行时 ELF 的分母为 1,135。一个包记录由 NEVRA/架构/RPM SHA 唯一标识，不把三个架构混成一个包。

## 2. 构图方法、覆盖率与不可判定边界

节点是 R11 的一个 `(rpm_sha256, path)` 运行时 ELF；边是该 ELF 已实测的一个 DT_NEEDED。provider 只用相同解析架构的、无符号版本后缀的 RPM `Provides`，再要求 provider 包内恰有一个 ELF 文件名与 SONAME 完全相同，或恰有一个以 `SONAME.` 开头的实际版本文件。0 个或多个候选均不补全。noarch ELF 只按 R11 实测 machine 映射解析架构。逐条结果见 `tables/dependency_edge_resolution.tsv`，缺边见 `tables/unresolved_dependency_edges.tsv`，规则全文见 `tables/closure_methodology.tsv`。

151,403 条 DT_NEEDED 边中，138,236/151,403（91.303343%）唯一解析；13,167/151,403 未唯一解析：

| 状态 | 边数/151,403 |
|---|---:|
| `UNRESOLVED_NO_UNVERSIONED_SONAME_PROVIDE` | 8,683（5.735025%） |
| `UNRESOLVED_AMBIGUOUS_PROVIDER_ELF` | 2,429（1.604328%） |
| `UNRESOLVED_PROVIDER_HAS_NO_MATCHING_RUNTIME_ELF` | 2,055（1.357305%） |

“全依赖闭包完整”与“libgcc_s 是否可判定”分开申报。完整闭包为 19,656/36,829 ELF（53.370985%）和 2,274/6,064 包记录（37.500000%）。但 DIRECT 是根 ELF 自身的 DT_NEEDED 原文，TRANSITIVE 是一条全部已解析的正向路径；两者即使另有无关缺边，libgcc_s 的“存在”仍已实测确定。只有“没有正向路径且闭包不完整”的对象标为 `UNDETERMINED`。ABSENT 则必须要求完整闭包，绝不从缺边推断。最终 libgcc_s 查询可判定 28,405/36,829 ELF、5,251/6,064 包；不可判定 8,424 ELF、813 包，均不进入 DIRECT/TRANSITIVE/ABSENT 比率分子。

## 3. 闭包分类、干净集与风险集

风险集定义为 DIRECT 或 TRANSITIVE；干净集定义为完整闭包中的 ABSENT。若改用方案 B，风险集对象的既有闭包已有 libgcc_s 正向路径；干净集没有。以下比例同时给全范围分母和可判定分母，未知项单列。

### 3.1 二进制包记录

| 架构 | DIRECT | TRANSITIVE | 风险集 | 干净集 ABSENT | UNDETERMINED | 全范围 / 可判定 |
|---|---:|---:|---:|---:|---:|---:|
| 全部 | 3,607 | 977 | 4,584 | 667 | 813 | 6,064 / 5,251 |
| aarch64 | 856 | 464 | 1,320 | 606 | 100 | 2,026 / 1,926 |
| armv7l | 1,891 | 56 | 1,947 | 47 | 25 | 2,019 / 1,994 |
| x86_64 | 854 | 456 | 1,310 | 12 | 681 | 2,003 / 1,322 |
| noarch | 1 | 1 | 2 | 0 | 0 | 2 / 2 |
| riscv64 | 5 | 0 | 5 | 2 | 2 | 9 / 7 |
| i586 | 0 | 0 | 0 | 0 | 5 | 5 / 0 |

全平台风险集为 4,584/6,064（75.593668%，全范围分母），或 4,584/5,251（87.297658%，仅可判定分母）；干净集为 667/6,064（10.999340%）或 667/5,251（12.702342%）。逐包清单分别见 `tables/risk_package_set.tsv`、`tables/clean_package_set.tsv`；全表和全闭包不完整清单见 `tables/package_closure_classification.tsv`、`tables/incomplete_package_closures.tsv`。

### 3.2 ELF 文件

| 架构 | DIRECT | TRANSITIVE | 风险集 | 干净集 ABSENT | UNDETERMINED | 全范围 / 可判定 |
|---|---:|---:|---:|---:|---:|---:|
| 全部 | 12,722 | 3,390 | 16,112 | 12,293 | 8,424 | 36,829 / 28,405 |
| aarch64 | 2,884 | 1,134 | 4,018 | 6,881 | 632 | 11,531 / 10,899 |
| armv7l | 6,463 | 1,142 | 7,605 | 3,576 | 272 | 11,453 / 11,181 |
| x86_64 | 2,851 | 1,113 | 3,964 | 1,685 | 5,297 | 10,946 / 5,649 |

全平台风险 ELF 为 16,112/36,829（43.748133%），或 16,112/28,405（56.722408%，仅可判定分母）。逐 ELF 见 `tables/elf_closure_classification.tsv`，全架构明细见 `tables/closure_distribution.tsv` 与 `tables/clean_risk_summary.tsv`。

### 3.3 最常见的传递引入中间库

风险集中有 4,186 个包记录至少存在一条“根 ELF → 中间 DSO → libgcc_s”的正向路径。下表按中间 DSO basename 聚合三个架构；同一风险包对同一中间库只计一次，DIRECT 包若另有传递路径也进入本表。

| 排名 | 中间库 | 引入包数/4,186 |
|---:|---|---:|
| 1 | `libstdc++.so.6.0.33` | 2,567 |
| 2 | `libaul.so.0.84.11` | 763 |
| 3 | `libbundle.so.0.18.15` | 738 |
| 4 | `libdlog.so.0.0.0` | 726 |
| 5 | `libglib-2.0.so.0.8000.5` | 710 |
| 6 | `libcynara-client.so.0.26.0` | 577 |
| 7 | `libsystemd.so.0.27.1` | 529 |
| 8 | `libpkgmgr-info.so.0.37.2` | 500 |
| 9 | `libecore.so.1.25.1` | 477 |
| 10 | `libgtest.so.1.15.2` | 471 |

全局和逐架构 top 10、provider 包与代表性最短路径见 `tables/transitive_intermediate_top10.tsv`；4,186 个包与各中间 DSO 的逐路径见 `tables/transitive_intermediate_paths.tsv`。

## 4. 有效风险面细分

包级使用互斥优先级：包内任一 ELF 为 C++ 且有展开表则进 A；否则任一 ELF 为操作性纯 C 且有展开表进 B；否则进 C。展开表沿用 R11 判据 `.eh_frame` 或 `.ARM.exidx`。

| 架构 | A：C++ 且有展开表 | B：操作性纯 C 且有展开表 | C：无展开表 | 风险包分母 |
|---|---:|---:|---:|---:|
| 全部 | 2,424 | 2,160 | 0 | 4,584 |
| aarch64 | 795 | 525 | 0 | 1,320 |
| armv7l | 828 | 1,119 | 0 | 1,947 |
| x86_64 | 795 | 515 | 0 | 1,310 |
| noarch | 1 | 1 | 0 | 2 |
| riscv64 | 5 | 0 | 0 | 5 |

因此按任务给定机械规则，需运行时验证的包数 A+B 为 4,584/4,584 风险包；逐架构同上。ELF 级为 A 8,498、B 7,613、C 1，共 16,112 个风险 ELF；唯一 C 类 ELF 所属包还拥有展开表 ELF，所以包级 C 为 0。逐包、逐 ELF 与汇总见 `tables/risk_package_strata.tsv`、`tables/risk_elf_strata.tsv`、`tables/risk_effective_strata_summary.tsv`。

## 5. 特定形态归属

### 5.1 五个厂商预编译锚

五个冻结 armv7l 锚在 aarch64/armv7l/x86_64 共 15 个包记录中均有直接 `DT_NEEDED libgcc_s.so.1`，15/15 均属风险集：`drm-service-core-tizen`、`hal-api-drm-haltests`、`hal-backend-drm-generic`、`pen-wave`、`pen-wave-devel`。其中多数全闭包仍有其他缺边，但 DIRECT 正向事实不受该缺边影响。逐架构 NEVRA、SHA、DIRECT ELF 数见 `tables/vendor_prebuilt_closure_assignment.tsv`，五包汇总见 `tables/vendor_prebuilt_name_summary.tsv`。

### 5.2 插件宿主近似清单

沿用 R11 的 121 个 plugin/engine/launchpad 元数据候选包记录：风险 90/121（74.380165%），干净 14/121（11.570248%），不可判定 17/121（14.049587%）。aarch64 为风险/干净/未知 25/14/0（分母 39）；armv7l 39/0/0（39）；x86_64 25/0/15（40）；i586 0/0/2（2）；riscv64 1/0/0（1）。该清单仍是 R11 的元数据近似，不证明插件实际被某进程加载。逐包见 `tables/plugin_host_closure_assignment.tsv`，汇总见 `tables/plugin_host_closure_summary.tsv`。

### 5.3 本项目 c2 libc++/libc++abi

R11 记录身份和展开节，R12 已落盘 `dt_needed.tsv` 补充 R11 c2 表未采集的 DT_NEEDED。六个 libc++/libc++abi ELF 均直接需要 `libgcc_s.so.1`，故三架构均为 DIRECT。aarch64 与 armv7l 的全闭包完整；x86_64 的全闭包因替代 ld.so provider 等歧义不完整，但 libgcc_s 存在查询仍由自身 DT_NEEDED 确定。逐 SONAME、RPM SHA 与缺边见 `tables/project_c2_closure_assignment.tsv`。

## 6. armv7l 专项

armv7l 闭包主分母为 2,019 个含运行时 ELF 的包记录、11,453 个运行时 ELF。包级风险/干净/未知为 1,947/47/25；可判定风险比例 1,947/1,994（97.642929%）。ELF 级风险/干净/未知为 7,605/3,576/272；可判定风险比例 7,605/11,181（68.017172%）。风险包有效面为 C+++展开表 828/1,947、操作性纯 C+展开表 1,119/1,947、无展开表 0/1,947。

R11 armv7l 全体有 `_Unwind_VRS*` UND 的对象为 20 个二进制包、51 个 ELF。本次风险集中为 19/20 个包、50/51 个 ELF；其余 1 包/1 ELF 是冻结 Base 的 `libc++-22.1.8-19.1.armv7l`，完整闭包为 ABSENT，VRS 引用无版本，属干净集。20 包全量和 19 包风险子集分别见 `tables/armv7l_vrs_package_assignment.tsv`、`tables/armv7l_risk_vrs_packages.tsv`。

## 7. 工作量机械映射

### 7.1 源码重编口径与 T1 关系

R11 操作性 C++ 判据下，拥有至少一个 C++ 指标运行时 ELF 的源码 RPM 为 483/1,135（42.555066%，分母为有运行时 ELF 的源码包），也可写为 483/1,450（33.310345%，分母为全部 R11 源码记录）。T1 的谓词不同：从“二进制包依赖 libstdc++”映射出 372 个源码包，实际语料扫描 371/372，Chromium 未扫描。两个快照和谓词不能互相替代；按完整 sourcerpm 字符串精确连接，交集为 344/372（92.473118%），没有做包名级推断。逐身份对照见 `tables/r11_cpp_vs_t1_source_scope.tsv`。

### 7.2 文件冲突影响的 7 个包名

复核 R11 的精确包名 Requires/BuildRequires 后，7 个名称为：二进制包名 `coreclr`、`coreclr-diagnostics`、`libunwind-devel`；源码包名 `heaptrack`、`netcoredbg`、`qemu-accel`、`rpi4-linux-kernel`。这是 3 个二进制包名加 4 个源码包名的混合单位，不能给一个共同分母；分别对应 9/11,332 个二进制包记录和 4/1,450 个源码包记录。逐项见 `tables/libunwind_conflict_impact_seven_names.tsv`。

### 7.3 运行时验证包数

按任务定义 A+B：全部 4,584/4,584 风险二进制包记录；aarch64 1,320/1,320，armv7l 1,947/1,947，x86_64 1,310/1,310，noarch 2/2，riscv64 5/5，i586 无可判定风险包。三类工作量映射统一登记在 `tables/workload_mapping.tsv`；没有给出人日、周期或难度估计。

## 8. 实测边界

本报告的 DIRECT 来自 R11 DT_NEEDED 原文；TRANSITIVE 来自逐边唯一解析的正向路径；ABSENT 只来自完整闭包；`UNDETERMINED` 不补全、不进入三分类比率分子。provider 解析未使用动态链接器实际 root、RPATH/RUNPATH、缓存、镜像选包或 dlopen 路径，因此结果是冻结仓的静态闭包测算，不是实际进程映像测量。R12 已证明共载时无版本引用的最终绑定仍是 `UNDETERMINED_BY_STATIC_INSPECTION`；本任务没有把该绑定行为补成结论。

全部命令、PWD 和退出码保存在 `commands/`；首轮因包键不唯一退出 1 的失败原文保留，后续改用关系表完整 package_nevra 键。没有重扫平台仓，没有下载、安装、构建或向外部仓推送。
