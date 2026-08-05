# R8：libc++ 构建源基线归属查证报告

## 结论摘要

本次全程只读取既有源码、spec、制品、构建日志和退役 buildroot；未构建，未修改源码、spec、制品或配置。

1. 平台 LLVM 的构建基线结论为 `PLATFORM_BUILDS_FROM_PRISTINE_PLUS_PATCHES`。平台 `llvm.spec` 明文以 `Source0: %{name}-%{version}.tar.gz` 为源码，并声明唯一 `Patch0`，在 `%prep` 中执行 `%setup -q` 后执行 `%patch0 -p1`；不是从 Git 工作树直接构建（`raw/platform_spec_numbered.stdout.txt:39`、`:49`、`:158-161`）。
2. `Patch0` 有 10 个目标文件。把它实际施加到 pristine Source0 后，与 `codes/llvm` 的 `tizen_base@3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b` 比较：7 个逐字相同；另外 3 个包含 Patch0 的内容，但 `tizen_base` 还带有补充下游差异（`platform_patch_correspondence.tsv:2-11`）。
3. 要求补齐的六个目录中，`libcxx/`、`libunwind/`、`runtimes/`、`cmake/`、`third-party/` 退出 0、逐字相同；`libcxxabi/` 退出 1，唯一差异为 `libcxxabi/src/cxa_personality.cpp` 的 EHABI backport（`commands/01_source_tree_differences.log:11-69`、`source_difference_files.tsv:15`）。
4. R6 日志的 78 个实际编译对象已全部映射到源 TU，并结合退役根的 Ninja 头文件依赖数据库和 CMake 重生成输入取交集。14 个已发现差异中，唯一进入 libc++ 构建的是 `libcxxabi/src/cxa_personality.cpp`；`clang/`、`llvm/`、`compiler-rt/` 的 13 个 Tizen 下游差异均为 `NO_INTERSECTION`（`difference_build_intersection_summary.tsv:2-4`）。
5. 三架构子包构成一致，均为 7 个 binary RPM + 1 个 SRPM。消除 lib/lib64、debugsource 架构根和 build-id 哈希路径后，剩余 7 个路径差异全部位于按目标实际依赖生成的 debugsource 源文件集合（`payload_path_comparison_summary.tsv:2-7`）。
6. armv7l 中确有只能由已知差异文件产生的内容：debugsource 内 `cxa_personality.cpp` 含两行 backport，且该 TU 的对象明确进入 `libc++abi.so.1.0` 链接边；armv7l SRPM 内的实际 Source0 也与另两架构不同。未发现任何由 13 个非运行时 Tizen 差异产生的 binary payload 内容（`payload_source_attribution.tsv:3-7`）。
7. 三架构安装进 `libc++-devel` 的 `SOURCE_PROVENANCE` 文本逐字相同，但 armv7l 有两个字段与实际输入不符：`source_tarball_sha256` 和 `source_export`。armv7l 的实际 Source0 SHA256 为 `7d7f2105f1c745754e1318b3d73b632a642e426883fb2d6fa6a5b46f55b5d314`，来源为 `codes/llvm tizen_base@3d3e3da...` 的 GBS Git export；声明值仍指平台 tarball `16a932e1...` 和“verbatim platform SRPM tarball”（`provenance_field_audit.tsv:31`、`:35`）。

## 1. 平台 LLVM 构建源基线

### 1.1 SRPM 身份与 Source/Patch 全集

只读输入为：

- `tmp/corpus/srpms/llvm-22.1.8-18.1.src.rpm`
- SRPM SHA256：`95414ab653e3237125ff9202de108ff3b64f90af8eb7b02f3df16e512d2f38ee`
- spec 的 VCS 字段：`platform/upstream/llvm#c12cbe887ff08f04628e1b1165f8d02ce69a9146`（`raw/platform_spec_numbered.stdout.txt:33`）

全部条目如下（原文见 `raw/platform_source_patch_entries.stdout.txt:1-6`）：

| 类型 | spec 行 | 原文 |
|---|---:|---|
| Source0 | 39 | `Source0: %{name}-%{version}.tar.gz` |
| Source1001 | 40 | `Source1001: llvm.manifest` |
| Source1002 | 45 | `Source1002: mlgo_arm_model.tar.gz` |
| Source1003 | 46 | `Source1003: mlgo_aarch_model.tar.gz` |
| Source1004 | 47 | `Source1004: mlgo_x86_model.tar.gz` |
| Patch0 | 49 | `Patch0: 0001-Apply-Tizen-toolchain-compatibility-fixes.patch` |

`%prep` 原文为 `%setup -q` 和 `%patch0 -p1`（`raw/platform_prep_patch_application.stdout.txt:2-5`）。因此三选一结论是：

`PLATFORM_BUILDS_FROM_PRISTINE_PLUS_PATCHES`

### 1.2 Patch0 与 tizen_base 的内容对应

Patch0 SHA256 为 `509ae752761a32765df63036f1034f94229c3f47d8883e5bccdc469018d7870c`，修改 10 个文件、26 行新增、6 行删除。10 个目标和逐字比较结果完整列于 `platform_patch_correspondence.tsv:2-11`。

- 7 个文件为 `PATCH0_EXACT`。
- `clang/lib/Driver/Driver.cpp`：Patch0 的 `/emul`/`QEMU_ACCEL_DIR` 逻辑存在；`tizen_base` 仅另有该块的缩进/排版差异（`raw/clang_lib_Driver_Driver.cpp.patched_to_tizen.diff:7-17`）。
- `clang/lib/Options/OptionUtils.cpp`：Patch0 的 `CLANG_NO_LIBDIR_SUFFIX` 逻辑存在；`tizen_base` 另加 `<cstdlib>` 并修正一处注释拼写（`raw/clang_lib_Options_OptionUtils.cpp.patched_to_tizen.diff:3-18`）。
- `llvm/lib/TargetParser/Triple.cpp`：Patch0 对该文件的内容存在；`tizen_base` 另加 `getVendorTypeName()` 的 `Tizen -> "tizen"` 分支（`raw/llvm_lib_TargetParser_Triple.cpp.patched_to_tizen.diff:3-9`）。

因此，Patch0 的内容对应 `tizen_base` 的下游适配，但不能把 10 个目标笼统申报为与当前 `tizen_base` 逐字相同。

## 2. runtimes 目录完整核对

比较对象：平台 Source0 解包树与干净的 `codes/llvm` 工作树；后者位于 `tizen_base`，HEAD 为 `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b`。

| 目录 | `diff -qr` 退出码 | 结果 |
|---|---:|---|
| `libcxx/` | 0 | 无差异 |
| `libcxxabi/` | 1 | 唯一差异：`src/cxa_personality.cpp` |
| `libunwind/` | 0 | 无差异 |
| `runtimes/` | 0 | 无差异 |
| `cmake/` | 0 | 无差异 |
| `third-party/` | 0 | 两侧均存在，无差异 |

命令原文和退出码见 `commands/01_source_tree_differences.log:11-69`。唯一完整 diff 见 `raw/cxa_personality.platform_to_tizen.full.diff:1-11`：在 EHABI `#else` 分支增加一行 `__gnu_unwind_frame` 声明及一行空行。重试后的机械断言退出 0（`commands/01a_runtime_scope_assertion_retry.log:1-5`），最终状态为 `RUNTIME_TREE_DIFF=PASS_ONLY_EHABI_BACKPORT`。

作为相关性全集，另对 `clang/`、`compiler-rt/`、`llvm/` 做了全树比较。总差异为 14 个文件：clang 7、compiler-rt 2、llvm 4、libcxxabi 1；逐文件清单见 `source_difference_files.tsv:2-15`。

## 3. 差异文件与 libc++ 构建的相关性实测

### 3.1 实际 TU 全集

R6 成功日志中共有 78 行 `Building CXX/C/ASM object`。每个对象均通过退役根保留的 `build.ninja` 映射到源文件，零未映射；完整清单见 `r6_actual_compiled_tu.tsv:2-79`。原始日志命中、映射命令和断言均退出 0（`commands/02_r6_compile_dependency_intersection.log:6-24`）。

### 3.2 头文件与 CMake 输入

为覆盖“被 include 的头”和“被消费的 CMake 模块”，本次没有仅比较 `.cpp` 名单：

- 读取退役根 `.ninja_deps`，以 `ninja -t deps` 提取这 78 个实际对象的源/头依赖（`commands/02_r6_compile_dependency_intersection.log:26-34`）；
- 以 `ninja -t query build.ninja` 取得 CMake `RERUN_CMAKE` 的实际输入（同文件 `:36-44`）；
- 将直接 TU、include 依赖、CMake 输入统一归一到源码相对路径，再与 14 个差异文件取交集（同文件 `:46-64`）。

实测结果：

| 范围 | 结论 | 命中 |
|---|---|---|
| 14 个全部差异 | `INTERSECTION_FOUND` | `libcxxabi/src/cxa_personality.cpp` |
| 13 个非运行时 Tizen 下游差异 | `NO_INTERSECTION` | 无 |
| runtimes 差异 | `INTERSECTION_FOUND` | `libcxxabi/src/cxa_personality.cpp` |

交集原始行见 `difference_build_intersection.tsv:1-2`，汇总见 `difference_build_intersection_summary.tsv:2-4`。因此对核心问题的回答是：有一个差异文件进入 libc++ 构建，即 EHABI backport；`clang/llvm/compiler-rt` 的差异文件没有任何直接 TU、头文件或 CMake 消费交集。

## 4. 三架构制品内容对照

### 4.1 payload 路径集

三架构均包含相同的 7 个 binary 子包和 1 个 SRPM；每个 RPM 的完整原始 `rpm -qpl` 清单在 `payload_paths/raw/<arch>/`，逐 RPM 项数及 NEVRA 在 `rpm_payload_inventory.tsv:2-25`。

归一化仅做三类可复核的架构字面处理：

1. `/usr/lib` 与 `/usr/lib64` 归一为 `@LIB@`；
2. debugsource 根中的 `.x86_64/.aarch64/.arm` 归一为 `@ARCH@`；
3. build-id 哈希路径归一为 `@BUILD_ID@`。

其余路径不删除。归一化后，运行时、devel、debuginfo 和 SRPM 的路径集均一致；只剩 debugsource 的 7 条目标依赖集合差异：x86_64 对 aarch64 1 条，x86_64 对 armv7l 6 条（`payload_path_differences.tsv:2-8`）。由于 x86_64 与 aarch64 使用同一 Source0 仍出现目标依赖集合差异，且这些路径对应的源文件在两套 runtimes 树中逐字相同，本报告将其严格登记为 `DEBUGSOURCE_TARGET_DEPENDENT_SOURCE_SET_ONLY`，不把它们归因于源码基线差异。

### 4.2 只能由差异文件产生的内容

结论为“存在”，范围如下：

- `libcxx-runtimes-debugsource`：x86_64/aarch64 的 `cxa_personality.cpp` SHA256 都是 `d9d82c76d99df2a9850125287fb0ad2f5e3eef04bb56314210875a6e7d619692`；armv7l 是 `55e136184adbaeee078a339afde2d6da103f4af7cb7c5c31926f2911127eff03`。完整 diff 恰为两行 backport（`raw/debugsource_cxa_x86_vs_arm.diff:1-11`）。
- `libc++abi.so.1`：`cxa_personality.cpp` 是 R6 实际 TU，且该对象位于 `lib/libc++abi.so.1.0` 的 Ninja 链接输入中（`raw/cxa_object_linked_into_libcxxabi.stdout.txt:1`），因此该 DSO 消费了 backport。
- armv7l SRPM：内含 Source0 SHA 为 `7d7f2105...`，另两架构为 `16a932e1...`（`actual_source0_identity.tsv:2-4`）。

对 13 个非运行时 Tizen 差异，78 个 TU、实际 include 依赖和 CMake 输入均无交集，所以结论为 `NO_BINARY_PAYLOAD_CONTENT_ATTRIBUTABLE`。不同 ISA 的 ELF 字节本身没有被用作跨架构因果归属依据（`payload_source_attribution.tsv:3-7`）。

## 5. SOURCE_PROVENANCE 盘点

三份实际文件原文分别位于：

- `SOURCE_PROVENANCE.x86_64.txt:1-13`
- `SOURCE_PROVENANCE.aarch64.txt:1-13`
- `SOURCE_PROVENANCE.armv7l.txt:1-13`

三份文本 SHA256 相同且 `cmp` 均退出 0（`commands/03_payload_and_provenance.log:436-439`）。39 个“架构 × 字段”的逐字段审计见 `provenance_field_audit.tsv:2-40`。

x86_64 与 aarch64 的 13 个字段均与实际输入相符。armv7l 的结论为：

| 字段 | 声明 | 与实际制品 | 正确值/边界 |
|---|---|---|---|
| `source_tarball_sha256` | `16a932e1...d122e` | 不符 | `7d7f2105...5d314` |
| `source_export` | `verbatim platform SRPM tarball; not repacked` | 不符 | `GBS git export`，实际 HEAD `3d3e3da49e00...` |
| `platform_spec_vcs` | `...#c12cbe887f...` | 相符，但只是平台 spec 基线锚 | `c12cbe88` 是实际 HEAD 的祖先；实际 Source HEAD 是 `3d3e3da...` |
| `upstream_tag` / `upstream_peeled_commit` | `llvmorg-22.1.8` / `ca7933e4...` | 相符，但只是上游基线锚 | `ca7933e4` 是实际 HEAD 的祖先；实际 Source HEAD 另含 Tizen 下游提交和 EHABI backport |
| 其余 8 项 | 原声明 | 相符 | 版本、ABI/unwinder 策略和 `LLVM_22` 均与 R6 实际输入/门禁相符 |

祖先关系是 Git 实测，两个 `merge-base --is-ancestor` 均返回 0（`raw/source_git_ancestry.stdout.txt:2-3`）；armv7l 的实际构建模式、提交和 Source0 SHA 原文见 `raw/r6_actual_source_declaration.stdout.txt:1-17`。

## 6. 审计说明与完成状态

两个早期机械断言的非零退出码均被原样保留：

- `00_platform_srpm_and_patch_baseline.sh` 最初要求 Patch0 的 10 个目标与当前 `tizen_base` 全部逐字相同，实际为 7 同、3 含额外下游差异，故该过严断言退出 1；后续脚本按“补丁内容对应”和“完整文件逐字同一”分开申报，退出 0（`commands/00a_platform_patch_correspondence.log:1-33`）。
- `01_source_tree_differences.sh` 的运行时范围断言因命令内正则引号错误退出 2；目录 diff 和完整 diff 证据未受影响。修正为固定字符串断言后退出 0（`commands/01a_runtime_scope_assertion_retry.log:1-5`）。

最终只读核查状态：

- `PLATFORM_BASELINE=PASS_PRISTINE_PLUS_PATCHES_WITH_PARTIAL_TIZEN_BASE_CORRESPONDENCE`
- `RUNTIME_TREE_DIFF=PASS_ONLY_EHABI_BACKPORT`
- `BUILD_INTERSECTION=PASS_ONLY_EHABI_BACKPORT;NONRUNTIME_TIZEN_DELTA=NO_INTERSECTION`
- `PAYLOAD_PROVENANCE_AUDIT=PASS_WITH_DECLARED_ARM_PROVENANCE_MISMATCHES`

本报告仅陈述查证结果，不包含处置建议。
