# W4 / R48：Chromium 与 AOSP 的 libc++ 本地补丁调研

## 1. 结论摘要

本次只读调研完成，口径与结果如下：

- Chromium：当前 `buildtools` 快照中未找到独立的 libc++/libc++abi/libunwind 源码补丁目录（`NOT_FOUND`）。因此以 3 个可独立追溯的集成提交和 5 个现行集成点为分母，共 8 项；技术适用性分类为 `APPLICABLE 1 / NOT_APPLICABLE 7 / UNCERTAIN 0`。
- AOSP：以 `clang-r596125` 对应的 `PATCHES.json` 97 条记录为总分母。按“在 r596125 生效且直接触及 libcxx/libcxxabi/libunwind”筛得 9 条，再纳入 1 条与标准库系统接口直接相关的 `nl_types` 项，共分析 10 条；分类为 `APPLICABLE 0 / NOT_APPLICABLE 7 / UNCERTAIN 3`。
- 两来源的当前本地改动中，反向检索本项目已知的四类问题均为 `NOT_FOUND`：缺失 forced-unwind 识别类型、foreign rethrow 展开 API、ARM EHABI handler/cleanup 区分、libc++ 内部 forced-unwind 防护。
- 唯一记为 `APPLICABLE` 的 Chromium futex 配置，只表示“Linux/glibc 技术前提与项目相符”；项目是否实际发生相应静态局部初始化锁竞争或重入故障为 `NOT_OBSERVED`。本报告不构成采用建议。

逐项数据见：

- [AOSP 补丁表](tables/android_active_runtime_patches.tsv)
- [Chromium 集成表](tables/chromium_local_integrations.tsv)
- [glibc 相关项](tables/glibc_related.tsv)
- [反向检索表](tables/reverse_search.tsv)

## 2. 数据源、版本与差距

### 2.1 项目比较基准

项目基准来自 `codes/llvm/packaging/SOURCE_PROVENANCE`，SHA256 为 `1cd5a45ea027d8f22b8dc5812eebbcf56f6c52e42446ad531729ce80289e7a37`：LLVM 22.1.8，peeled commit `ca7933e47d3a3451d81e72ac174dcb5aa28b59d1`；运行时组合为 glibc + libgcc_s，`LIBCXXABI_USE_LLVM_UNWINDER=OFF`。

### 2.2 Chromium

使用 [Chromium buildtools 仓](https://chromium.googlesource.com/chromium/src/buildtools/) 的 `refs/heads/main`，本地快照 commit `4277578aa9c45906e51ad33cac1a5a7ad5288010`（2026-08-13）。其滚动 libc++ 快照为 [`97b436da4c33663581d394f4ee0a5977fc38c2f4`](https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx/+/97b436da4c33663581d394f4ee0a5977fc38c2f4)，日期为 2026-07-20。

该快照比 LLVM 22.1.8 的 2026-06-16 发布日晚 34 个自然日，但它是滚动 main 快照而非发布分支；在未取得完整共同历史的条件下，精确提交距离记 `NOT_AVAILABLE`，不得以日期差代替提交差。

Chromium 未保存一组可直接枚举的本地源码 patch 文件。本次因此采用两个互不混淆的口径：

1. 可独立追溯的提交：futex、filesystem clock、static library，共 3 项；
2. 当前 GN/vendor 集成点：ABI namespace、输出库名、custom libunwind 选择、demangle stub、符号可见性，共 5 项。

### 2.3 AOSP

使用 [AOSP llvm_android 仓](https://android.googlesource.com/toolchain/llvm_android/) 中由当前 [clang-r596125 source info](https://android.googlesource.com/platform/prebuilts/clang/host/linux-x86/+/refs/heads/mirror-goog-main-llvm-toolchain-source/clang-r596125/clang_source_info.md) 锁定的 commit `5142d653bd7a62a6e73dee815530af81b318132c`（2026-06-18）。[AndroidVersion.txt](https://android.googlesource.com/platform/prebuilts/clang/host/linux-x86/+/refs/heads/mirror-goog-main-llvm-toolchain-source/clang-r596125/AndroidVersion.txt) 申报 Android 版本 22.0.2、revision r596125、上游基线 `2bc22ea02edda5926f3e53f141def9bf212ac1db`（2025-11-04）。

该版本不是 LLVM 22.1.8 发布分支，版本号也不能用来推出线性提交距离。旧的公开 `refs/heads/main` 快照只有 50 条补丁且时间更早，本次仅保留为口径纠偏证据，不用于最终计数。

全部输入路径、SHA256 与角色见 [input_sha256.tsv](tables/input_sha256.tsv)，完整版本清单见 [source_inventory.tsv](tables/source_inventory.tsv)。

## 3. Chromium 逐项结果

分母为第 2.2 节定义的 8 项。

| 分类 | 数量 | 占比 |
|---|---:|---:|
| APPLICABLE | 1 | 12.5% |
| NOT_APPLICABLE | 7 | 87.5% |
| UNCERTAIN | 0 | 0.0% |
| 合计 | 8 | 100.0% |

关键项：

- C01 [`android: Use futex for __cxa_guard_acquire`](https://chromium.googlesource.com/chromium/src/buildtools/+/0d39be5a3f129cf1f35e7812108a2184e2193315)：实际差异为 1 文件新增 4 行，仅启用上游已存在的 per-guard futex 实现。项目是 Linux/glibc 且当前 spec 未观察到该宏，故技术前提记 `APPLICABLE`；实际故障未观测。
- C02 [`filesystem_clock.cpp`](https://chromium.googlesource.com/chromium/src/buildtools/+/17495e454aae81b581e8b3caccbb53054509b280)：Chromium GN 手工源清单修正；项目使用上游 CMake/runtimes，记 `NOT_APPLICABLE`。
- C03 [`static_library`](https://chromium.googlesource.com/chromium/src/buildtools/+/1df2adafaa36e2bcff31e28cd814505382e04df6)：针对 GN source_set 与 macOS PGO 签名尺寸，项目不具备该交付形态，记 `NOT_APPLICABLE`。
- C04–C08 均为 Chromium vendor/构建集成：两份 libc++ 的 ABI namespace/库名隔离、特定平台的 LLVM libunwind、Android demangle stub、GN 可见性控制。它们与本项目的 libc++/libstdc++ 双运行时边界或 CMake/libgcc_s 配置不同，均记 `NOT_APPLICABLE`。

所有标题、文件、改动量、问题、上游状态和判据均在 [chromium_local_integrations.tsv](tables/chromium_local_integrations.tsv) 中逐项申报；无法从单个提交拆分的现行集成点，行数明确记 `NOT_SEPARATELY_ATTRIBUTABLE`，未作推断补全。

## 4. AOSP 逐项结果

总补丁分母为 97；本报告相关项分母为 10，占总补丁的 10.3%。相关项内部分类如下：

| 分类 | 数量 | 占相关 10 项 |
|---|---:|---:|
| APPLICABLE | 0 | 0.0% |
| NOT_APPLICABLE | 7 | 70.0% |
| UNCERTAIN | 3 | 30.0% |
| 合计 | 10 | 100.0% |

逐项结论：

- A01 AArch64 libunwind execute-only：相应 commit 已是项目 22.1.8 基线祖先，且项目不用 LLVM unwinder，`NOT_APPLICABLE`。
- A02 `__hash_table` erase 修复：相应 commit 已是 22.1.8 基线祖先，`NOT_APPLICABLE`。
- A03 Android `bits/mbstate_t.h`：项目使用 glibc，22.1.8 已探测 `bits/types/mbstate_t.h`，`NOT_APPLICABLE`。
- A04 移出 `__cxa_demangle`：要求 `LIBCXXABI_NON_DEMANGLING_TERMINATE`，项目产品配置为 OFF，`NOT_APPLICABLE`。
- A05 Android i386 charconv 测试禁用：只改测试元数据且目标不匹配，`NOT_APPLICABLE`。
- A06 pre-C++23 `stdatomic.h` revert：不依赖 bionic，但既有数据没有 483 包的使用和 include-order 分布，`UNCERTAIN`。
- A07 `__libcpp_blsr` integer sanitizer：当前 runtimes 未观察到 sanitizer 注入，但不能排除消费者侧配置，`UNCERTAIN`。
- A08 AArch64 LFI：Android 专有大补丁，项目未观察到 LFI 且先导架构为 armv7l，`NOT_APPLICABLE`。
- A09 `__resize_default_init` revert：补丁元数据未记录撤销原因，既有数据无 Android 扩展依赖或等价故障证据，`UNCERTAIN`。
- A10 `nl_types` stub：补 bionic 缺失接口，glibc 已提供，`NOT_APPLICABLE`。

详细版本区间、补丁 SHA256、文件与增删行数见 [android_active_runtime_patches.tsv](tables/android_active_runtime_patches.tsv)。

## 5. glibc 与系统接口核对

分母为 5 个直接涉及 libc/系统展开接口的调研项：`APPLICABLE 1 / NOT_APPLICABLE 3 / UNCERTAIN 1`。

- futex 配置的 Linux/glibc 前提成立，但项目是否实际遇到问题未观测。
- Chromium 的 custom libunwind 选择反而明确区分 Linux 与 Android/Fuchsia；项目使用 glibc + libgcc_s，不能套用 Android 前提。
- AOSP 的 Android mbstate 路径与 nl_types stub 都是 bionic 缺口，项目 glibc 不缺。
- pre-C++23 stdatomic 行为不是 bionic 独有，消费者影响规模不可由现有数据判定。

见 [glibc_related.tsv](tables/glibc_related.tsv)。

## 6. 本项目四类已知问题的反向检索

检索范围是 AOSP r596125 当前生效补丁正文，以及 Chromium buildtools 当前 runtime integration 文件。四类均为 `NOT_FOUND`：

1. `__forced_unwind` 类型补齐；
2. `__cxa_rethrow` 对 foreign exception 的 `_Unwind_Resume_or_Rethrow` 处理；
3. ARM EHABI catching handler 与 cleanup 的区分；
4. libc++ iostream/future/string 内部 forced-unwind 防护。

这表示“在上述当前本地改动范围内未命中”，不表示全部历史、下游分支或 issue 讨论中绝对不存在。逐项关键词与证据边界见 [reverse_search.tsv](tables/reverse_search.tsv)。

## 7. 失败记录与局限

- Chromium 历史/路径日志接口两次请求分别返回 HTTP 500 与 401；未丢弃这些失败记录。三个已知提交随后通过精确 commit URL 取得原文与 diff。
- AOSP source-info 的直接 `?format=TEXT` 请求返回 400；版本身份改由当前 prebuilt 清单和可成功读取的 `AndroidVersion.txt` 交叉确认。失败请求原文仍保留。
- Chromium 不维护本次可见的源码 patch 目录，现行集成点的独立增删行数无法可靠归因，统一记 `NOT_SEPARATELY_ATTRIBUTABLE`。
- AOSP 与 Chromium 都是当前快照；本任务没有把历史上已撤销的全部补丁纳入分母。
- `APPLICABLE` 只表示已观测技术前提相符，不评价收益、风险或是否应采用。

原始命令、退出码与 stdout/stderr 位于 `commands/` 和 `raw/`；官方来源链接汇总见 [source_urls.tsv](tables/source_urls.tsv)。

## 8. 最终事实陈述

当前 Chromium/AOSP 本地改动没有为本项目已解决的四类 forced-unwind 问题提供现成补丁。AOSP 的 10 个相关项中没有可直接判为适用者，3 项因消费者使用数据不足保持 `UNCERTAIN`；Chromium 的 8 个集成项中仅 futex 配置具备相同的 Linux/glibc 技术前提，但实际问题未观测。本任务不作采用、移植或排期建议。
