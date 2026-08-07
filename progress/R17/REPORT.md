# R17：libgcc_s helper 缺口的可用性拆解

## 结论摘要

1. R16 实际比对了 compiler-rt 的静态 `libclang_rt.builtins-*.a`，三架构均用 `nm -g --defined-only` 取全局定义符号；不是共享库导出口径，故不存在该类系统性低估。
2. armv7l 的 `986` 个缺口以定点算术为主：`980/986`（`99.391481%`）是 `__gnu_*` 定点家族；仅 `4/986` 是 `__aeabi_*`，`2/986` 是常规 helper。aarch64 的 `25/25`、x86_64 的 `27/27` 均为常规整数/浮点 helper。
3. “整体覆盖 13.3%”与“`__aeabi_*` 覆盖 62/69”并非同一分母：前者是 1,137 个 `COMPILER_HELPER` 的精确名称覆盖；后者横跨 R16 的 `COMPILER_HELPER` 与 `ARM_EHABI` 类别。缺失的 7 个 `__aeabi_*` 中，3 个 unwind personality 被归为 `ARM_EHABI`，不在 986 个 helper 缺口分母内；其余 4 个读写 helper 在该分母内。
4. 实际引用面不能从现有 R11 数据闭合：R11 仅持久化了名称含 `Unwind` 的 UND，没有保留普通 helper 的逐 ELF UND。按 fail-closed 口径，实际引用 ELF 数、包数、零引用数和“真实缺口”规模全部记为 `NOT_AVAILABLE_R11_UNFILTERED_UND_NOT_RETAINED`，未将缺失记录当作零引用。

## 1. 输入、对象与去重

输入及 SHA256 全表见 `tables/input_files.tsv`。R16 三个静态库对象、RPM NEVRA、对象 SHA256 见 R16 `input_identity.tsv`；本任务保留的 `nm` 原文见 `raw/`。

| 架构 | libgcc_s 定义符号×版本对 | 唯一基础名 | 去除的版本重复对 | 多版本基础名 |
|---|---:|---:|---:|---:|
| armv7l | 1163 | 1163 | 0 | 0 |
| aarch64 | 159 | 159 | 0 | 0 |
| x86_64 | 183 | 177 | 6 | 6 |

x86_64 的六个多版本基础名是 `__divtc3`、`__gttf2`、`__lttf2`、`__multc3`、`__netf2`、`__powitf2`；节点原文见 `tables/libgcc_multiple_version_base_names.tsv`。下文所有 helper 覆盖与缺口数字都按唯一基础名计。

## 2. 缺口家族拆解

| 架构 | 家族 | 符号数 / 缺口分母 | 占缺口比例 | 代表符号 |
|---|---|---:|---:|---|
| armv7l | AEABI | 4/986 | 0.405680% | `__aeabi_uread4`, `__aeabi_uread8`, `__aeabi_uwrite4`, `__aeabi_uwrite8` |
| armv7l | 十进制浮点 | 0/986 | 0.000000% | 无 |
| armv7l | 定点算术 | 980/986 | 99.391481% | `__gnu_addda3`, `__gnu_adddq3`, `__gnu_addha3` 等 |
| armv7l | 常规整数/浮点 | 2/986 | 0.202840% | `__clrsbdi2`, `__clrsbsi2` |
| armv7l | 其他 | 0/986 | 0.000000% | 无 |
| aarch64 | 常规整数/浮点 | 25/25 | 100.000000% | `__clrsbdi2`, `__clrsbti2`, `__divhc3`, `__divmodbitint4`, `__fixdfbitint` 等 |
| aarch64 | 其余四类合计 | 0/25 | 0.000000% | 无 |
| x86_64 | 常规整数/浮点 | 27/27 | 100.000000% | `__clrsbdi2`, `__clrsbti2`, `__divhc3`, `__divmodbitint4`, `__eqhf2` 等 |
| x86_64 | 其余四类合计 | 0/27 | 0.000000% | 无 |

逐家族五行完整汇总见 `tables/missing_helper_family_summary.tsv`，1,038 个缺口成员逐一列于 `tables/missing_helper_family_members.tsv`。

## 3. 实际引用面

R11 全量包分母为 `11,332` 个二进制包实例；其中三项目标架构分别为 armv7l `3,692`、aarch64 `3,756`、x86_64 `3,609`。R11 的运行时 ELF 总分母为 `36,829` 个文件实例；目标架构分别为 armv7l `11,453`、aarch64 `11,531`、x86_64 `10,946`。完整架构口径见 `tables/counting_scope.tsv`。

但这些分母对应的未过滤 UND 明细没有持久化。可复核证据为：

- 扫描器在内存中解析 UND 后，仅保留基础名含 `Unwind` 的记录；见 `commands/02_r11_filter_scope_source.txt`。
- `unwind_undefined_symbols.tsv` 的列和记录均为 unwind 专项，不是全符号 UND 表。
- R11 JSON 分片未提供未过滤 UND 或 raw dynsym 字段；见 `commands/05_r11_retained_schema.txt`。

因此以下项目均为 `NOT_AVAILABLE`，不是 0：

| 架构 | 缺口符号分母 | 有实际引用 | 零引用 | 真实缺口规模 |
|---|---:|---|---|---|
| armv7l | 986 | NOT_AVAILABLE | NOT_AVAILABLE | NOT_AVAILABLE |
| aarch64 | 25 | NOT_AVAILABLE | NOT_AVAILABLE | NOT_AVAILABLE |
| x86_64 | 27 | NOT_AVAILABLE | NOT_AVAILABLE | NOT_AVAILABLE |

逐符号状态见 `tables/missing_helper_actual_reference_status.tsv`。由于不能区分“有引用”与“零引用”，任务书所定义的真实缺口集合不能从允许输入中产生。

## 4. armv7l 专项

R16 的 69 个唯一 `__aeabi_*` 基础名中，compiler-rt 静态 builtins 精确同名命中 `62/69`（`89.855072%`），缺失 `7/69`（`10.144928%`）：

| 符号 | R16 类别 | 在 986 个 helper 缺口中 | 版本节点 | 平台引用面 |
|---|---|---|---|---|
| `__aeabi_unwind_cpp_pr0` | ARM_EHABI | 否，跨类别 | GCC_3.5 | NOT_AVAILABLE |
| `__aeabi_unwind_cpp_pr1` | ARM_EHABI | 否，跨类别 | GCC_3.5 | NOT_AVAILABLE |
| `__aeabi_unwind_cpp_pr2` | ARM_EHABI | 否，跨类别 | GCC_3.5 | NOT_AVAILABLE |
| `__aeabi_uread4` | COMPILER_HELPER | 是 | GCC_3.5 | NOT_AVAILABLE |
| `__aeabi_uread8` | COMPILER_HELPER | 是 | GCC_3.5 | NOT_AVAILABLE |
| `__aeabi_uwrite4` | COMPILER_HELPER | 是 | GCC_3.5 | NOT_AVAILABLE |
| `__aeabi_uwrite8` | COMPILER_HELPER | 是 | GCC_3.5 | NOT_AVAILABLE |

引用面不可得原因与第 3 节相同。完整机器可读表见 `tables/armv7l_missing_aeabi_symbols.tsv`。armv7l 缺口中“有实际引用”的全列也因此为 `NOT_AVAILABLE`，不得由符号存在性推断引用。

## 5. 版本节点

三架构 `libgcc_s.so.1` 全导出面的“版本节点 × R16 类别 × 符号版本对”分布见 `tables/libgcc_version_node_distribution.tsv`。缺口集合的节点分布如下，分母单位是缺口唯一基础符号名；本表中每个缺口只落在一个节点，因此也等于符号版本对数：

| 架构 | 节点 | 缺口符号数 / 缺口分母 |
|---|---|---:|
| armv7l | GCC_3.5 | 4/986 |
| armv7l | GCC_4.3.0 | 980/986 |
| armv7l | GCC_4.7.0 | 2/986 |
| aarch64 | GCC_3.0 | 1/25 |
| aarch64 | GCC_4.7.0 | 2/25 |
| aarch64 | GCC_11.0 | 6/25 |
| aarch64 | GCC_13.0.0 | 6/25 |
| aarch64 | GCC_14.0.0 | 10/25 |
| x86_64 | GCC_4.3.0 | 1/27 |
| x86_64 | GCC_4.7.0 | 2/27 |
| x86_64 | GCC_12.0.0 | 8/27 |
| x86_64 | GCC_13.0.0 | 4/27 |
| x86_64 | GCC_14.0.0 | 12/27 |

由于真实缺口集合不可得，其版本节点分布亦为 `NOT_AVAILABLE`；见 `tables/true_gap_version_nodes.tsv`。

## 实测与不可得边界

实测/复算部分包括：输入身份、静态 builtins 的 `nm` 对象形态、libgcc_s 基础名去重、缺口家族、全缺口版本节点。不可得部分包括：普通 helper 的逐 ELF/逐包引用数、零引用集合、真实缺口集合及其版本节点。本文不包含方案推荐、可行性判定或人日估算。
