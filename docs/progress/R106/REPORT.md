# R106：Tizen-Base 与 Tizen-Base-Toolchain 的 RPM 集合差异对比

## 1. 结论摘要

本次只做固定快照的统计、集合对比和既有判定规则核查，没有修改任何判定、配置或平台源码，也没有向 Gerrit 推送。

1. 源码包集合非常接近：两边共有 250 个源码包；仅 `Tizen-Base` 有 1 个（`toybox`）；仅 `Tizen-Base-Toolchain` 有 4 个（`binutils-armv7hl`、`gcc-armv7hl`、`libcxx-runtimes`、`tensorflow2`）。
2. 二进制包名集合共有 638 个；仅 `Tizen-Base` 有 8 个，全部来自 `toybox`；仅 `Tizen-Base-Toolchain` 有 12 个，其中 11 个来自上述 4 个 Toolchain 独有源码包，另 1 个是共同源码包 `bcc-tools` 在 Toolchain 快照中额外产生的 `bcc-tools-examples`。
3. `toybox` 不构成 R100/R101 判定的 C++ 迁移遗漏：R95 的实际 RPM/ELF 扫描对其 8 个二进制包均未检出 C++ runtime ELF，repodata 中没有 `libstdc++.so.6` 依赖；本次取得的源码 RPM 内，上游归档共 383 个条目，其中 236 个 `.c`，按 `.cc/.cpp/.cxx/.c++/.hh/.hpp/.hxx` 后缀检索为 0。这个结论只回答“是否是 libc++ 迁移载体”，不判断它是否应属于 Base 项目。
4. `toybox` 并非在整套 Toolchain 项目中未构建：配对的 `Tizen-Unified-Toolchain/tizen-unified-toolchain_20260829.015247` 含 `toybox-0.6.0-1.src.rpm` 及其二进制包，R98 保存的三架构构建日志状态均为 `OK`。可观测事实是它被放在配对的 Unified Toolchain 分层；为什么采用该分层的政策依据不可得，记为 `NOT_AVAILABLE`。
5. step0 的 `639/91` 不能按其原始执行过程严格复现。Rev 6 自己注明原始数字来自未固定 revision/checksum 的滚动 reference URL。对 R95 固定快照按 primary repodata 直接统计得到 646 个唯一二进制包名、83 个唯一包名声明依赖 `libstdc++.so.6`，分别比 639 多 7、比 91 少 8。R95 的独立 RPM payload/ELF 扫描在排除 `noarch` 与范围外 `riscv64` 后恰好得到 91 个实际含 `DT_NEEDED libstdc++.so.6` 的包名，但它不是文档所述的 repodata 依赖口径，不能拿这个“数值相同”冒充原执行过程复现。
6. 因此，“GCC 仓有而 Base Toolchain 仓没有”的源码包只有 `toybox`；它没有 C++/libstdc++ 迁移载体证据，并在配对的 Unified Toolchain 层存在。就本任务可观测范围而言，没有发现因两个 Base 仓集合差异而漏掉的 libc++ 迁移对象。

## 2. 输入身份

### 2.1 Tizen-Base（GCC 标准仓）

- 快照：`tizen-base_20260827.214809`
- packages URL：`https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base/tizen-base_20260827.214809/repos/standard/packages`
- source URL：`https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base/tizen-base_20260827.214809/repos/standard/source`
- 复用材料：R95 的 `base_binary_records.tsv`、`base_source_records.tsv`、`base_cpp_binary_records_exact.tsv`、`base_cpp_source_records_exact.tsv`。
- 原始 primary XML SHA256：packages `71abb1573e8b9a04ac94dbc23f57088770da4c34fe638dc775938f2f709fa01b`；source `f3bb1479d932801ec35b86a56a06765b33e88a11568db29de85948da73e84278`。

### 2.2 Tizen-Base-Toolchain（LLVM 工具链仓）

- 快照：`tizen-base-toolchain_20260828.101647`
- packages URL：`https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647/repos/standard/packages`
- source URL：`https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647/repos/standard/source`
- 本次重新下载 exact primary metadata；packages SHA256 `ba2bf090e651196a558fe76d2329ea8e236b785cff2657cd3858943bd81355ed`，source SHA256 `eccde3e46dcba072b65538935eb3707f95a35d2d85feaa75b492a23669a72f53`，均与该快照 repomd 中的值一致。
- 复用材料：R101 的 254 包全表，用于“含 C++”的四路候选口径；R98 的配对 Unified Toolchain 元数据和构建日志，仅用于追踪 `toybox` 的项目分层。

完整身份表见 `tables/input_identity.tsv`，下载与校验原文见 `raw/007_fetch_toolchain_metadata.*`、`raw/018_input_checksums.*`。

## 3. 基本盘点与口径对齐

| 仓/固定快照 | binary metadata 记录 | 唯一二进制包名 | source metadata 记录 | 唯一源码包名 | repodata 中声明依赖 libstdc++ 的唯一二进制包名 | 映射后的唯一源码包名 | 已有“含 C++”证据数 |
|---|---:|---:|---:|---:|---:|---:|---:|
| Tizen-Base / 20260827.214809 | 1,710 | 646 | 412 | 251 | 83 | 33 | 38 |
| Tizen-Base-Toolchain / 20260828.101647 | 1,705 | 650 | 292 | 254 | 86 | 35 | 73 |

这里保留了三种不同单位，不把它们混在一起：

- “记录”包含架构和同名包的多个 release；“唯一包名”按 RPM name 去重。
- `83/86` 使用同一个机械口径：primary metadata 的 `Requires` 中出现 `libstdc++.so.6`，再按二进制包名去重。
- `38` 是 R95 的较窄口径：源码包至少产出一个经实际 payload 检查确认的 C++ runtime ELF。
- `73` 是 R100/R101 的较宽四路并集：实际 C++ 构建日志、发布 RPM 的 C++ runtime 依赖、静态归档中的可解码全局 C++ 符号、或对外 C++ 头文件。

因此 `38` 与 `73` 不是可以直接相减的同口径数字。能严格同口径横向比较的是 repodata 代理口径：33 个与 35 个“位于依赖 libstdc++ 的二进制包背后的源码包”。

逐架构统计见 `tables/repository_per_arch_summary.tsv`；完整规范化记录见 `tables/normalized_binary_records.tsv`。

## 4. step0 的 639 与 91

### 4.1 严格结果

不能严格复现原始执行过程，原因有两项：

1. `docs/hq/step0_analysis_rev6.md` Appendix B 明确写明，原始统计引用滚动 reference repository URL，没有保存 snapshot ID 或 checksum，后来无法重新导出同一输入。
2. 在本次固定 Tizen-Base 快照上，primary repodata 的唯一包名/直接依赖口径是 `646/83`，不是 `639/91`。

与总部数字的差值为：二进制总数 `+7`，repodata libstdc++ 依赖包数 `-8`。由于原输入不可得，不能把差值进一步断言为某几个包的增删。

### 4.2 为什么当前材料里仍能看到“91”

R95 还保存了另一套更直接的 payload 证据：逐个解 RPM，对实际 ELF 的 `DT_NEEDED` 做检查。在架构化 payload 中排除范围外 `riscv64` 和异常包含 ELF 的 `noarch` 包后，唯一包名数是 91。

这两种方法的集合并不相同：payload-only 有 10 个 `clang-accel*` / `qemu-accel*` 包；repodata-only 有 `gcc` 和 `libstdc++-devel`。清单见 `tables/step0_dependency_method_delta.tsv`。因此数值 91 是独立观测，不证明它就是 step0 原先 91 的生成方式。

全部显式变体见 `tables/step0_count_variants.tsv`。结论是：**数字可在另一口径中碰到，原 step0 的同输入、同方法复现不可做。**

## 5. 源码包集合差异

| 分类 | 数量 | 清单 |
|---|---:|---|
| 两边共有 | 250 | `tables/source_common.tsv` |
| 仅 Tizen-Base | 1 | `toybox` |
| 仅 Tizen-Base-Toolchain | 4 | `binutils-armv7hl`、`gcc-armv7hl`、`libcxx-runtimes`、`tensorflow2` |

### 5.1 同名版本差异

- 250 个共同源码包中，30 个 EVR 集合完全相同，220 个不同。
- 220 个差异中，219 个仅 release 集合不同；只有 `xdgmime` 的 upstream version 不同：Tizen-Base 为 `0.0.20-1.1`，Base Toolchain 为 `0.0.19-1.1`。
- 这里按每个包在仓中全部架构/记录的 EVR 集合比较，未把 release 漂移写成 upstream source 版本变化。

完整清单见 `tables/source_version_differences.tsv`。

## 6. 仅 Tizen-Base 存在的 `toybox`

### 6.1 C++ 与 libstdc++ 性质

- Tizen-Base source：`toybox-0.6.0-1.1.src.rpm`，本次下载 SHA256 为 `e5894647c5cc2005dfbc53cb712f0f3068768edabb5251f7c0295573173a217e`。
- 产出 8 个唯一二进制包名：`toybox`、`toybox-full`、`toybox-symlinks-dhcp`、`toybox-symlinks-dhcpd`、`toybox-symlinks-full`、`toybox-symlinks-nslookup`、`toybox-symlinks-ping`、`toybox-symlinks-ping6`。
- 8 个包的 repodata 都没有 `libstdc++.so.6` Requires。
- R95 实际 payload/ELF 复用表中，全部 8 个包均为无 C++ runtime ELF 信号。
- source RPM 中的上游 tarball：383 个成员，236 个 `.c`，上述常见 C++ 源码/头文件后缀为 0。该版本 spec 的构建段显式 `export CC="gcc"` 并运行 `make`；这只证明该快照如何编译 C 源码，不把“GCC 构建”误写为“C++ 包”。

按 R101 的判定树，`toybox` 命中的是“没有 C++ 迁移载体”，结果为 `NO_LIBCXX_NEEDED`。这是对本任务新增候选机械套用既有规则，不更改 R100/R101 原表。

### 6.2 为什么 Base Toolchain 没有

可观测结果不是“尚未构建”或“构建失败”：配对的 Unified Toolchain 快照含 `toybox-0.6.0-1.src.rpm`，并有 aarch64、armv7l、x86_64 三个 `succeeded/toybox.buildlog.txt`，R98 下载状态均为 `OK`。所以在现有产物层面，它被放到了 Toolchain 的 Unified 分层，而不是 Base 分层。

配置制定者为何这样分层、是否明确写有排除规则，本次可得材料中没有直接说明，记为 `NOT_AVAILABLE`，不补作意图推断。

逐字段结果见 `tables/tizen_base_only_source_analysis.tsv`，源码包检查见 `analysis/toybox_source_rpm_members.txt`、`analysis/toybox_upstream_members.txt` 及 `raw/013`–`016`。

## 7. 二进制包集合与子包差异

### 7.1 包名集合

| 分类 | 数量 | 内容 |
|---|---:|---|
| 两边共有 | 638 | `tables/binary_common.tsv` |
| 仅 Tizen-Base | 8 | 全部为 `toybox` 的 8 个子包 |
| 仅 Tizen-Base-Toolchain | 12 | `bcc-tools-examples`；`binutils-armv7hl`；`gcc-armv7hl`；4 个 libc++ runtime/devel 包；5 个 tensorflow2 包 |

Toolchain-only 的 libc++ 包为 `libc++`、`libc++-devel`、`libc++abi`、`libc++abi-devel`；tensorflow2 包为 `libtensorflow2-aot-runtime`、`tensorflow2-aot-devel`、`tensorflow2-lite-devel`、`tensorflow2-lite-flatbuf-schema`、`tensorflow2-lite-util`。

完整清单见 `tables/binary_only_tizen_base.tsv` 和 `tables/binary_only_base_toolchain.tsv`。

### 7.2 同一源码包的子包集合

在 250 个共同源码包中，仅 `bcc-tools` 的子包名集合不同：Base Toolchain 多出 `bcc-tools-examples`，共同子包数为 3；其余 249 个共同源码包的二进制子包名集合一致。`bcc-tools` 的源码 EVR 同时不同（Tizen-Base `0.35.0-4.1`，Base Toolchain `0.35.0-2.1`），所以能确认的是“两个固定快照的 source build 输出集合不同”；仅凭 repodata 不能进一步断言是某个 build option 还是 spec revision 造成。

表见 `tables/shared_source_subpackage_differences.tsv`。

### 7.3 同名二进制版本

- 638 个共同二进制包名中，66 个 EVR 集合完全相同，572 个不同。
- 572 个差异中，570 个仅 release 集合不同；2 个存在 upstream version 差异：`xdgmime` 与 `xdgmime-devel`。

完整清单见 `tables/binary_version_differences.tsv`。

## 8. 是否构成 R100/R101 判定遗漏

就两个指定 Base 仓的源码集合差异而言，唯一需要核查的 GCC-only 源码包是 `toybox`。它的实际发布物、依赖和源码归档均给出一致的“非 C++/不依赖 libstdc++”证据；同时它存在于配对 Unified Toolchain 层。因此：

- 没有发现一个“仅 GCC Base 存在且含 C++/依赖 libstdc++”的源码包；
- 没有发现因 R100/R101 以 Base Toolchain 254 包为分母而遗漏的 libc++ 迁移对象；
- 这不等于两个项目的功能集合相同，也不等于对 `toybox` 的项目分层作出政策评价。

## 9. 无法查清的部分

1. step0 原始滚动仓的 revision、repomd checksum 与生成 `639/91` 的完整命令不可得，因此不能对其做同输入复现，也不能把 `+7/-8` 精确归因到包级变动。
2. `toybox` 被放在 Unified Toolchain 而非 Base Toolchain 的政策理由没有直接配置注释或变更记录证据；只能确认实际分层和成功构建状态。
3. 220 个源码 EVR 差异和 572 个二进制 EVR 差异只做了集合统计；本任务没有逐包分析 release 差异的行为含义。
4. 两仓“含 C++”的既有深度扫描定义不同；报告没有把 R95 的窄口径 38 和 R100/R101 的宽口径 73 当作同单位直接比较。统一可比的 repodata 代理数为 33/35。

## 10. 自行判断与尚存疑问

### 自行判断

1. “包总数”主表同时报告 metadata record 数与按 RPM name 去重数；集合比较一律按唯一包名，避免把架构副本当不同包。
2. 版本比较使用每个名称在快照内的完整 EVR 集合；另分 `UPSTREAM_VERSION_DIFFERS` 与 `RELEASE_ONLY_DIFFERS`，避免把 release 漂移误称为 upstream version 变化。
3. 对 step0 同时展示 primary Requires 与实际 ELF `DT_NEEDED` 两种证据，不为了凑出 91 而混写口径。
4. 对 `toybox` 仅应用 R101 已确认的迁移判据，不修改 R100/R101 的既有表，也不对仓分层给出处理建议。

### 尚存疑问

1. 总部 step0 原始统计所用 reference repository 的具体 revision/checksum 和命令是否还能从构建记录恢复？若能提供，才可完成严格复现。
2. `toybox` 的 Base/Unified Toolchain 分层是否有明确设计文档？当前只能从产物证明“确实在 Unified”，不能证明制定者的理由。

## 11. 产物索引

- `tables/repository_summary.tsv`：两仓基本盘点。
- `tables/repository_per_arch_summary.tsv`：逐架构统计。
- `tables/step0_count_variants.tsv`、`tables/step0_dependency_method_delta.tsv`：step0 复现边界。
- `tables/source_common.tsv`、`source_only_*.tsv`、`source_version_differences.tsv`：源码层差异。
- `tables/binary_common.tsv`、`binary_only_*.tsv`、`binary_version_differences.tsv`：二进制层差异。
- `tables/shared_source_subpackage_differences.tsv`：同一源码的子包集合差异。
- `tables/tizen_base_only_source_analysis.tsv`：`toybox` 重点分析。
- `raw/`：命令、stdout、stderr、退出码逐项落盘。
- `code/`：解析、复现变体、机械自检脚本。

