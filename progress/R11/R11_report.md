# R11：平台运行时展开图与 unwinder 依赖实测

状态：`DONE_WITH_NOT_AVAILABLE`。本报告只陈述冻结快照上的实测分布、明确标注的近似和不可得项；不做处置建议、方案推荐或良恶判断。所有分母都随指标写明，源码包、二进制包和 ELF 文件不混用。

## 1. 冻结语料、快照与计数单位

平台未提供一个跨 Base/Unified、binary/source 的单一快照 ID，因此本次用四组 `repomd revision + repomd SHA256` 构成复合快照身份：

| 仓 | 类型 | revision | repomd SHA256 |
|---|---|---:|---|
| Base | binary | 1785866115 | `1789528921ae67d70ba657c787899ec580ab0b17a8368a0245ec09440710cf16` |
| Unified | binary | 1785904307 | `e425b6a91a3cfb2f67cbd73160ac48e2bb776e92866de81d5b97bb25e5ab39c3` |
| Base | source | 1785866095 | `cf7fca47147304ec114e8e0c19f6f43353dbf973ee623d3eb5188919c509fe72` |
| Unified | source | 1785904240 | `f15c6854277fb975d6d5e52d9a33c120eb85bb5cdc726acdf4243026098fde59` |

完整 data href、声明 SHA、时间戳与大小见 `tables/repodata_inventory.tsv`，快照口径见 `tables/snapshot_identity.tsv`。

本次没有抽样：

- 源码包记录：1,450/1,450；全部下载并按 repodata SHA256 验证，失败 0。下载对象登记在 `tables/source_download_status.tsv`，均只落 `tmp/R11/srpms/`。
- 二进制包记录：11,332/11,332；全部下载、校验、解包成功，失败 0。下载对象登记在 `tables/binary_download_status.tsv`，均只落 `tmp/R11/rpms/`。
- ELF：读到 39,166 个；主分母为 36,829 个非 debug 路径的 ET_EXEC/ET_DYN 运行时 ELF，另有 2,337 个 ET_REL/其他 ELF，不进入运行时依赖图；readelf 失败 0。
- 映射：1,450 个 distinct sourcerpm 映射到 11,332 个二进制包记录；其中 1,135 个源码包、6,064 个二进制包拥有至少一个运行时 ELF。逐 ELF 通过其所属 RPM 唯一映射回二进制包，再通过 `rpm:sourcerpm` 映射回源码包。

架构分母为 aarch64 3,756 包/11,531 运行时 ELF，armv7l 3,692/11,453，x86_64 3,609/10,946，noarch 261/35，i586 5/17，riscv64 9/2,847。完整口径见 `tables/counting_units_final.tsv`、`tables/measured_denominators.tsv` 和 `tables/package_universe_summary.tsv`。

源码 spec 提取成功 1,449/1,450；唯一失败是 `chromium-efl-1.1.144-1.src.rpm`，原始错误为 `files over 4GB not supported by cpio, use rpm2archive instead`。它只影响 spec 字面量近似，不影响 11,332 个二进制 RPM/36,829 个运行时 ELF 的全量扫描。

## 2. 实测结果

### 2.1 C/C++ 共存形态

ELF 的操作性 C++ 判据为：动态符号基名存在 `_Z` 前缀，或 DT_NEEDED 含 libstdc++/libc++/libc++abi。未命中者标为 `PURE_C_OPERATIONAL`，该标签仅表示“没有命中 C++ 指标”，不证明所有输入目标文件均由 C 编译。展开表判据为 `.eh_frame` 或 `.ARM.exidx` 存在。

全平台 36,829 个运行时 ELF 的互斥分类为：

| 分类 | ELF 数/36,829 | 二进制包数/11,332（包间类别可重叠） |
|---|---:|---:|
| 操作性纯 C 且有展开表 | 26,816（72.812%） | 3,907（34.478%） |
| 含 C++ 内容 | 9,908（26.903%） | 2,513（22.176%） |
| 操作性纯 C 且无展开表 | 105（0.285%） | 21（0.185%） |

按 RPM 观察“混装”：366/6,064 个拥有运行时 ELF 的二进制包记录，同时含 C++ 指标 ELF 与无 C++ 指标 ELF（6.036%）；按源码包聚合为 192/1,135（16.916%）。这是“同一 RPM 内不同 ELF 的共存”，不是“同一 ELF 内 C 与 C++ 目标文件共存”。逐架构及其余互斥类别见 `tables/c_cpp_package_coexistence.tsv`。

同一 ELF 内的语言共存只用 DWARF `DW_AT_language` 实测。冻结仓只有 43/36,829 个运行时 ELF 可匹配到可用语言证据：43 个均只见 C compile unit，实测 C+C++ 为 0/43；其余 36,786/36,829 为 `UNDETERMINED_NO_DEBUG_INFO`。覆盖率不足，0/43 不外推为平台比例。见 `tables/c_cpp_same_elf_measurement.tsv` 和 `tables/dwarf_language_per_runtime_elf.tsv`。

### 2.2 `_Unwind_*` 引用、版本节点与 DT_NEEDED 扇入

按用户要求还保留了符号名包含 `Unwind` 的字面量全集；它会混入 LLVM API、Rust/Itanium 修饰名等不属于 unwinder ABI 的符号。绑定相关核心统计采用动态 UND 基名以 `_Unwind_` 或 `__gnu_Unwind_` 开头的子集，两个集合分别见 `tables/unwind_undefined_symbols.tsv` 与 `tables/abi_unwind_undefined_symbols.tsv`。

核心 ABI 子集共有 5,767/36,829 个运行时 ELF 引用至少一个符号：

| 范围 | 有 ABI 引用 ELF | 仅版本化 | 仅无版本 | 同一 ELF 混合 |
|---|---:|---:|---:|---:|
| 全平台 | 5,767 | 5,756 | 11 | 0 |
| armv7l | 998 | 996 | 2 | 0 |
| aarch64 | 2,295 | 2,291 | 4 | 0 |
| x86_64 | 2,364 | 2,360 | 4 | 0 |

以 5,767 个引用 ELF 为分母，版本化-only 为 5,756/5,767（99.809%），无版本-only 为 11/5,767（0.191%）。换成全运行时 ELF 分母分别为 5,756/36,829（15.629%）和 11/36,829（0.030%）。包级分布为：含版本化引用 1,870/11,332，含无版本引用 9/11,332，含任一引用 1,875/11,332；有 4 个包在不同 ELF 间同时出现两类，因此包数不可简单相加。逐符号、逐版本节点和逐文件原文均在上述 TSV。

`libgcc_s.so.1` 的直接 DT_NEEDED 扇入为 12,722/36,829 个运行时 ELF、3,607/11,332 个二进制包。armv7l 为 6,463/11,453 ELF、1,891/3,692 包；aarch64 为 2,884/11,531、856/3,756；x86_64 为 2,851/10,946、854/3,609。逐消费者见 `tables/unwinder_dt_needed_consumers.tsv`。

三架构 `libgcc_s.so.1` 的完整 GLOBAL/WEAK 导出与版本节点在 `tables/special_unwinder_exports.tsv`，readelf 原文在 `raw/special_*libgcc_s.so.1.readelf.txt`。摘要实测到：aarch64 18 个 `_Unwind_*` 导出，节点含 GCC_3.0/GCC_3.3/GCC_4.2.0；armv7l 15 个，含 GCC_3.0/GCC_3.3/GCC_3.5/GCC_4.3.0；x86_64 18 个，含 GCC_3.0/GCC_3.3/GCC_4.2.0。

### 2.3 nongnu libunwind 与当前仓内 LLVM libunwind

平台 nongnu 包的 NEVRA 为：

- aarch64/x86_64：`libunwind-1.6.2-1.2`、`libunwind-devel-1.6.2-1.2`；
- armv7l：`libunwind-1.6.2-1.3`、`libunwind-devel-1.6.2-1.3`。

其通用运行时 SONAME 为 `libunwind.so.8`，直接扇入 25/36,829 个运行时 ELF、16/11,332 个二进制包；另有架构、ptrace、coredump、setjmp SONAME。完整文件清单见 `tables/special_unwinder_file_lists.tsv`，导出摘要见 `tables/special_unwinder_export_summary.tsv`。

冻结 Base 同时已有 `libc++-22.1.8-19.1`，其所带 LLVM libunwind SONAME 为 `libunwind.so.1`，直接扇入 6/36,829 个运行时 ELF、3/11,332 个二进制包。nongnu `.so.8` 与 LLVM `.so.1` 的运行时 SONAME 不同，结果登记为 `NO` 冲突，见 `tables/unwinder_soname_comparison.tsv`。

### 2.4 `dlopen` 与插件宿主

对 `dlopen` 的动态 UND 引用命中 1,879/36,829 个运行时 ELF、842/11,332 个二进制包，映射到 235 个源码包。ELF 不记录调用时实际传入的 mode 值，因此平台运行时的 RTLD_LOCAL/RTLD_GLOBAL 分布为 `NOT_AVAILABLE_FROM_ELF`。

源码近似覆盖 235 个目标源码包：234/235 的 SRPM 解包成功，唯一失败仍为 `chromium-efl` 的 cpio >4 GB 限制；该源码包映射到 3/842 个 dlopen 二进制包、11/1,879 个 dlopen ELF。识别出的 400 个源码归档中，358 个完整 `PASS`，42 个为 `PASS_SKIPPED_LINK_OR_UNSAFE_MEMBERS`（合计跳过 336 个链接或不安全成员），归档解压失败 0。

在成功可读的源码文本中得到 21,034 个 `dlopen` 字面量 occurrence：±10 行没有 RTLD scope 字面量 20,009/21,034（95.127%），邻近 `RTLD_GLOBAL` 604/21,034（2.872%），邻近 `RTLD_LOCAL` 390/21,034（1.854%），两者都邻近 31/21,034（0.147%）。按源码包计，GLOBAL 邻近命中 101/235、LOCAL 69/235、两者 14/235、无 scope 邻近 199/235；同一源码包可落入多个类别，不能相加。另有 17/235 个目标源码包在已提取文本中没有 `dlopen` 字面量。以上全部标为 `APPROXIMATION_ONLY`，不替代运行时 mode 实测；逐命中原文和汇总见 `tables/dlopen_source_call_scope_approximation.tsv`、`tables/dlopen_source_analysis_summary.tsv` 和 `tables/dlopen_scope_measurement_limitations.tsv`。

插件/engine/launchpad 清单采用明确标注的元数据近似：路径含 plugin/plugins/engine/engines，或包名为 launchpad 宿主。命中 302/36,829 个运行时 ELF、121/11,332 个二进制包、38/1,450 个源码包；其中同时含 C++ 操作性指标和展开表的为 57/302 个候选 ELF（18.874%）、30/121 个候选二进制包（24.793%）、9/38 个候选源码包（23.684%）。具体机制与证据路径见 `tables/plugin_mechanism_packages.tsv`、`tables/plugin_candidate_elfs.tsv`；这些数字不证明对象实际被加载。

### 2.5 链接驱动形态

冻结 repodata 不含构建日志；1,449 份可提取 spec 中的 `clang`/`clang++` 只是字面量，不能区分编译和链接。字面量分类为：同时含 clang 与 clang++ 12/1,450，仅 clang 51/1,450，无字面量 1,386/1,450，提取不可得 1/1,450。

可得的实际平台候选构建日志只有 R2 的 libcynara-commons 一包，即 1/1,450 个源码包；其中提取出 5 条实际链接命令，5/5 使用 clang++、0/5 使用 clang。这个局部实测不外推。因此平台范围 clang/clang++ 实际链接驱动分布均为 `NOT_AVAILABLE`。搜索范围、日志 SHA 与局部计数见 `tables/link_driver_search_scope.tsv`、`tables/link_driver_available_log_measurement.tsv` 和 `tables/link_driver_measurement_availability.tsv`。

### 2.6 文件路径与 RPM 事务冲突

平台 nongnu `libunwind-devel` 与冻结 Base `libc++-devel-22.1.8-19.1` 在每个主架构有三处同路径：

| 路径 | nongnu | LLVM 侧 | 静态分类 |
|---|---|---|---|
| `/usr/include/libunwind.h` | 普通文件 | 内容不同的普通文件 | 硬冲突 |
| `/usr/include/unwind.h` | 普通文件 | 内容不同的普通文件 | 硬冲突 |
| `/usr/lib*/libunwind.so` | 指向 `.so.8.0.1` 的符号链接 | 指向 `.so.1` 的符号链接 | 目标不同的硬冲突 |

在独立空临时 RPMDB 中执行 `rpm --test --nodeps --noscripts`，armv7l/aarch64/x86_64 均退出 4，并逐条报告上述三处 file conflict；没有修改宿主 RPMDB。因此这两组 devel 包不能在标准 RPM 事务中同时安装。`unwind_arm_ehabi.h`、`unwind_itanium.h`、`libunwind.modulemap`、`__libunwind_config.h`、`mach-o/compact_unwind_encoding.h` 只在冻结 Base LLVM 侧出现，nongnu 侧同路径不存在。

本项目 `artifacts/rebuild_2218_c2/` 当前采用 libgcc_s，三个 `libc++-devel` 制品均不拥有上述 LLVM libunwind 头和 `libunwind.so` 链接。三方路径逐项见 `tables/llvm_libunwind_reference_path_presence.tsv`，RPM test 原文见 `raw/rpm_test_coinstall_*.stderr.txt`。

精确包名依赖的实测为：二进制 Requires `libunwind`/`libunwind-devel` 9/11,332 个包记录（3 个包名：libunwind-devel、coreclr、coreclr-diagnostics）；源码 BuildRequires 4/1,450（heaptrack、netcoredbg、qemu-accel、rpi4-linux-kernel）。另有 10 个二进制包记录要求 `libunwind.so.1` 或 `.so.8` SONAME。完整逐包行见 `tables/libunwind_dependency_metadata.tsv`。

### 2.7 armv7l 专项与本项目制品对照

armv7l 分母为 3,692 个二进制包、11,453 个运行时 ELF：

| 指标 | ELF 数/11,453 | 二进制包数/3,692 |
|---|---:|---:|
| `.ARM.exidx` | 8,136（71.038%） | 1,962（53.142%） |
| `.ARM.extab` | 6,629（57.880%） | 1,652（44.745%） |
| `.eh_frame` | 11,385（99.406%） | 2,012（54.496%） |
| 含 C++ 操作性指标 | 3,118（27.224%） | 832（22.535%） |
| `_Unwind_VRS_Get/Set` 等 EHABI VRS UND | 51（0.445%） | 20（0.542%） |

armv7l 的核心 ABI 引用为 998/11,453 ELF；其中 996 仅版本化、2 仅无版本。逐符号含 `_Unwind_VRS_Get@GCC_3.5`、`_Unwind_VRS_Set@GCC_3.5` 及无版本少数项，见 `tables/abi_unwind_undefined_symbols.tsv`。

本项目 c2 制品的引用形态实测如下：armv7l 有 1 个引用 ELF，节点 GCC_3.0/GCC_3.5；aarch64 有 2 个，均 GCC_3.0；x86_64 有 2 个，均 GCC_3.0。三者都是版本化-only，因此与各架构平台全体的主导引用类别 `MATCH`。冻结 Base 自带的 `libc++-22.1.8-19.1` 则为无版本引用，所以本项目 c2 与该特定包的引用类别为 `MISMATCH`。两个事实同时保留，见 `tables/project_c2_vs_platform_unwind.tsv`；这只是引用形态对照，不是运行时异常展开正确性测试。

## 3. 实测、近似与不可得严格分区

### 3.1 实测

repodata/RPM/ELF 总量与 SHA、ELF 类型和节、动态符号及版本节点、DT_NEEDED、SONAME、RPM payload 路径/类型/hash、空临时 RPMDB 的 `rpm --test`、本项目制品逐 ELF 对照，均为冻结输入上的实测。原始命令、PWD、输出文件和退出码在 `commands/`，逐文件原文在 `raw/`，逐项数据在 `tables/`。

### 3.2 近似（不得读作运行时事实）

- `CPP_CONTENT_OPERATIONAL`：强度中；依据 `_Z`/C++ runtime DT_NEEDED，无法证明同一 ELF 的全部输入语言。
- RPM/源码包“C/C++ 共存”：强度中；表示包内不同 ELF 类别共存，不表示同一 ELF 内 C/C++ 目标共存。
- 插件宿主：强度低到中；依据路径/包名元数据，不证明实际加载。
- RTLD source window：强度只覆盖字面量邻近，不证明运行时值、宏展开或执行分支。
- spec 链接驱动字面量：强度低；不能区分编译与链接。

逐项登记见 `tables/evidence_classification_registry.tsv`。

### 3.3 NOT_AVAILABLE / UNDETERMINED

- 36,786/36,829 个运行时 ELF 无可匹配的 DWARF 语言证据；同一 ELF 内 C/C++ 目标文件共存的平台分布不可得。
- RTLD_LOCAL/RTLD_GLOBAL 是运行时实参，ELF 层不可判定；源码窗口只能作为近似。
- 平台完整构建日志语料不可得；实际 clang/clang++ 链接驱动的平台分布不可得。
- `chromium-efl` spec 因 cpio 的 >4 GB 限制不可提取；已逐包登记，没有补全。

## 4. 失败登记与边界

二进制下载、checksum、RPM 解包和 readelf 分析失败均为 0；源码 RPM 下载失败为 0。源码 spec 提取失败 1/1,450，见第 1 节。`dlopen` 源码近似的逐源码包/归档可得性见 `tables/dlopen_source_analysis_status.tsv` 与 `tables/dlopen_source_archive_status.tsv`。

本次没有安装、构建或改动任何平台包、spec、源码、buildroot 或配置；RPM 冲突测试使用独立临时 RPMDB 且带 `--test`。没有向外部平台仓推送。结果边界是构建产物的静态/动态元数据形态，不覆盖真实进程装载顺序、插件执行路径，也不覆盖 ARM EHABI 或 Itanium/DWARF 的运行时异常展开正确性。
