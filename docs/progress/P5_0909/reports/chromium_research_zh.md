# Chromium 的 libc++ 使用方式与 LLVM 展开器对 Tizen 的适用性

证据截至 2026-09-07。用途：总部技术评审；不构成实施或选型建议。

## 摘要

1. **“Chromium／Google 在规范层面统一禁止 `pthread_cancel`”未得到证实。** Chromium 的 `base::Thread` 确实采用请求退出并等待线程结束的协作方式；但本次固定版本主仓扫描，在随仓 gRPC 的 POSIX 实现中发现了真实取消调用。公开 Google C++ Style Guide 的“不使用 C++ 异常”不等于禁止 POSIX 线程取消；内部规范不可得。[线程实现](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/base/threading/thread.cc)、[gRPC 实现](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/third_party/grpc/source/src/core/util/posix/thd.cc)、[公开风格指南](https://google.github.io/styleguide/cppguide.html)。
2. **llvm-libgcc 可以在隔离目录构建，并支持本次 x86_64 强制展开探针；它不是已经验证的 Tizen 即插即用替代库。** 150 次运行区分了 GNU／LLVM 提供方、异常规格保留／解除、写者回滚有／无及 ABI 库修补有／无。换成 LLVM 展开器没有消除 `noexcept` 终止、写者状态残留或未修补 ABI 库的重抛终止。[实测记录](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/MEASUREMENTS.md)、[逐格结果](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/measurement_summary.tsv)。
3. **ChromeOS 的迁移是真实的，但不能当作任意旧二进制兼容的证明。** 正式提交记录显示 2021 年 12 月启用所有板型、更新 SDK 和系统依赖；原始参考提案本身已废弃，不能把它的草案测试栏当成发布验证。公开材料描述受控 SDK 构建前提，不提供 Tizen 外部二进制分母、更新时序或兼容承诺。[全板启用](https://chromium-review.googlesource.com/c/chromiumos/overlays/chromiumos-overlay/+/3335237)、[SDK 更新](https://chromium-review.googlesource.com/c/chromiumos/overlays/chromiumos-overlay/+/3339453)、[原提案](https://reviews.llvm.org/D106703?id=361316)。
4. 本次无 Tizen／ChromeOS 替换后实际镜像；ARM 与 aarch64 的实验展开器运行均为 `NOT_OBSERVED`。八项适用性中，已有的是部分源码、构建和本机探针证据，**没有一项可以外推为整个平台迁移验收通过**。

## 证据口径

“静态核查”指读取源码、构建配置、ELF（可执行文件与共享库格式）或提交；“外部报告”指项目作者报告的结果，不等于本组重现；“实测”只指本次执行并保存原始输出的探针；“分析”明确表示条件性推论。`NOT_OBSERVED` 为未观测，`NOT_AVAILABLE` 为材料不可得。本文不把“没有崩溃”当成对象状态、资源释放或 ABI 正确的证明。

主仓固定在 `ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1`，其 libc++ 依赖固定在 `97b436da4c33663581d394f4ee0a5977fc38c2f4`。本次构建的 LLVM 源树版本另为 `5ed6c77278dfa7a470667cf1a137723d3c96fe60`，**没有把 Chromium 的依赖版本当作本地实验版本**。完整来源与获取失败记录见[引用索引](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/REFERENCES.md)。

## Chromium 如何使用 libc++

### 构建和退出机制：静态核查

默认启用自带 libc++；组件构建时可共享链接，非组件构建采用静态形式，不能概括成“一律静态”。libc++abi 是对应的 C++ ABI 支持库；具体目标类型还随是否由可执行文件导出符号而变化。自带 LLVM libunwind 的默认条件是 Fuchsia 或 Android；**Linux 默认并不是一并使用自带 libunwind**。libc++ ABI 命名空间设置为 `__Cr`、ABI 版本为 2，以隔离标准库符号；这不转换跨边界对象布局。[标准库配置](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/build/config/c++/c++.gni)、[展开器配置](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/build/config/unwind.gni)、[ABI 配置](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/buildtools/third_party/libc++/__config_site)。

`base::Thread::StopSoon()` 投递退出工作；`Stop()` 等待底层线程结束。这证明这一封装的协作退出策略，不证明所有第三方代码都受同一机制约束。[源码](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/base/threading/thread.cc)。

### 使用检索：静态核查，不是产品运行覆盖

归档有 505,965 个普通文件；选择源码、头文件、构建及文档等后缀和指定文件名后扫描 195,796 个文本文件，排除 310,156 个非选定后缀及 13 个含 NUL 文件。不包括另行获取的全部 DEPS 仓库，也不检查宏展开、别名调用或二进制。每个检索词都有独立合成正向对照，另有真实线程源码正向对照。[命中明细](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/chromium_usage.tsv)、[扫描原始输出](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/054_scan_full_archive.stdout)、[正向对照和补查](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/057_supplement.stdout)。

| 检索项 | 结果与解释 |
|---|---|
| `pthread_cancel` | 1 个文件、2 行：gRPC `thd.cc:204` 为调用，206 为日志文字；不是两个调用点。非 Android 的 POSIX `Thread::Kill()` 实现会取消线程。 |
| 间接调用 | `work_stealing_thread_pool.cc:162` 的 `DumpSignalHandler` 调用 `Thread::Kill`；`third_party/grpc/BUILD.gn:2285` 列入 POSIX 源文件。是否被某一最终产品启用／执行仍为 `NOT_OBSERVED`。 |
| `pthread_setcanceltype`、`PTHREAD_CANCEL_ASYNCHRONOUS` | 选定文本范围内均零命中；有正向对照；不等于全依赖、全产品无异步取消。 |
| `std::shared_mutex` | 仅一份性能说明文档的两行文字命中，不是当前实例化代码；`std::shared_timed_mutex` 零命中。不能覆盖别名和独立依赖仓。 |
| `wbuffer_convert` | 一处头文件清单命中，不是使用者；不能据此宣称所有产品无使用者。 |
| `#include <boost/...>` | `third_party/protobuf/third_party/utf8_range/boost.cpp:1` 命中 `boost/locale.hpp`；没有证明该文件进入 Chrome 产品，也不能写“仓内完全没有 Boost”。 |

历史上，Chromium 的 StatisticsRecorder **确实尝试过** `std::shared_mutex`，讨论记录在 2023 年说明已回到 `base::Lock`。理由包括该工作负载的读写锁开销及对原锁使用方式的改进；这不是取消回滚修复，也不是标准库从未使用过的证据。[维护者讨论](https://groups.google.com/a/chromium.org/g/cxx/c/bIlGr1URn8I)。

公开 Google Style Guide 本次文本中，取消相关精确词检索零命中，异常章节提供正向对照；没有找到可引用的全 Google 取消禁令。内部规范为 `NOT_AVAILABLE`，不能从公开异常规则推断其内容。[指南快照](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/sources/google_style.txt)、[原始检索](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/015_chromium_search.stdout)。

### lightweight-web-engine 与 Tizen 的前提

本地源码包 README 将项目标识为 **Starfish**，列出自己的引擎构建及 `liblightweight-web-engine` 产物；它不是一份能证明 Chromium 衍生关系的材料。`WorkerThread.cpp` 在 `OS_POSIX && !STARFISH_ANDROID` 下启用异步取消并调用 `pthread_cancel`，并非仅凭代码条件就能认定为 Tizen 特有追加。私有上游历史不可得，Tizen 公共远端本次返回 HTTP 403；因此“谁、何时引入”和确切派生关系为 `NOT_AVAILABLE`，不能补写为 Chromium 上游行为。[包内 README 与代码快照](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/snapshot_origins.tsv)、[代码上下文](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/049_lwe_context.stdout)、[远端获取结果](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/048_lwe_remote.stderr)。

分析：Tizen 已有取消调用、异步取消组件、跨标准库语义边和未覆盖的外部二进制。它不能仅通过改用 Chromium 的链接方式获得相同使用约束；协作退出只在调用方实际遵守该协议时消除对应取消路径。这是应用语义前提，不是展开器选择带来的效果。

## llvm-libgcc：接口、强制展开及边界

llvm-libgcc 将 compiler-rt 的编译器内建运算支持与 LLVM libunwind 的栈展开实现组合，提供 libgcc 风格文件及符号版本；它不提供 libc++ 容器布局、取消回滚守卫或旧版 libc++ 函数语义。文档明确把完整发行版兼容责任留给集成者，要求显式启用。[项目说明](https://github.com/llvm/llvm-project/blob/main/llvm-libgcc/docs/LLVMLibgcc.md)、[本次构建版本快照](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/snapshots/LLVMLibgcc.rst)。

强制展开是 glibc 取消线程时驱动栈清理的协议，不等于普通 C++ 异常搜索。LLVM 的 DWARF 实现有 `_Unwind_ForcedUnwind`，保存停止回调及其参数并执行带强制展开标志的清理阶段；GNU 也实现该协议。相同入口名不保证两家私有上下文能交叉使用。ARM 的 EHABI（ARM 异常处理 ABI）又有独立的数据结构与继续展开约定，不能用 x86 DWARF 实测代替。[LLVM DWARF 快照](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/snapshots/UnwindLevel1.c)、[LLVM EHABI 快照](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/snapshots/Unwind-EHABI.cpp)、[GNU 快照](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/snapshots/gcc-unwind.inc)。

本地 glibc 2.40 源码通过 `libgcc_s.so.1` 查找 `_Unwind_Backtrace`、`_Unwind_ForcedUnwind`、`_Unwind_GetCFA`、按架构配置的 `_Unwind_GetIP`、`_Unwind_Resume` 和 `__gcc_personality_v0`；另有架构条件扩展，不能将六个入口当成所有架构全集。本次实验 DSO 导出 x86_64 核查所需六项，实际宿主 glibc 的取消和 backtrace 探针通过。**这只确认对应接口子集，不确认整个 GNU libgcc ABI。**[glibc 接口快照](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/snapshots/glibc-unwind-link.c)、[符号核对](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/041_analysis.stdout)。

已知限制包括：混合 GNU／LLVM 展开器的私有上下文交叉使用、ARM 强制展开的历史缺陷、文件别名与 SONAME（共享库内部登记名称）不一致引起的加载问题，以及不同版本的内建运算导出差异。LLVM ARM 修复于 2021 年落地；这反证“他们根本不处理强制展开”的绝对说法，但不是 ChromeOS 全产品取消测试证明。[ARM 修复](https://reviews.llvm.org/D109856)、[混合提供方讨论](https://reviews.llvm.org/D150897)、[发行版取消崩溃报告](https://github.com/llvm/llvm-project/issues/90041)。

## ChromeOS 迁移：已确认的历史及不能补全的部分

Chrome 浏览器改用 libc++ 与 ChromeOS 系统展开器切换是两件事。2017 年的浏览器公告将 C++14 与 CFI（控制流完整性）作为迁移动机之一；不能据此声称整个操作系统的第三方 C++ 库同时换成 libc++。[公告](https://groups.google.com/a/chromium.org/g/chromium-os-dev/c/e8pJRHLisIg)。

| 时间／证据性质 | 可确认事实 |
|---|---|
| 2021 年提案，外部讨论 | D106703 描述受控工具链：ChromeOS SDK 产生相关构建产物，而非任意外部工具链二进制；还讨论了静态 libgcc 与混合展开器问题。提案及关联早期变更已废弃，不能作为最终落地提交。 |
| 2021-10-27，已合入提交 | NaCl 加载故障推动改成真实文件 `libgcc_s.so.1`、`libunwind` 指向它，作者报告 NaCl 测试通过。 |
| 2021-11，已合入提交 | binutils 补显式 pthread 链接；Breakpad 删除阻碍 arm32 上构建 64 位崩溃报告器的硬编码库路径。 |
| 2021-12，已合入提交 | 所有板型默认启用 libunwind，SDK 更新，并加入隐式系统依赖。提交报告多个板型测试；未取得当时完整测试日志或运行镜像。 |
| 2022-02，LLVM 上游 | 后续 llvm-libgcc 项目经 D108416 落地；不能以其上游落地日期替代 ChromeOS 下游启用日期。 |
| 2023-07，已合入提交 | glibc 引导构建因 llvm-libunwind 提前安装而发生问题，调整构建时 sysroot 应用；这不是运行系统全面退回 GNU 展开器。 |

依据：[原提案](https://reviews.llvm.org/D106703?id=361316)、[真实文件修正](https://chromium-review.googlesource.com/c/chromiumos/overlays/chromiumos-overlay/+/3246062)、[pthread 依赖](https://chromium-review.googlesource.com/c/chromiumos/overlays/chromiumos-overlay/+/3298526)、[Breakpad](https://chromium-review.googlesource.com/c/chromiumos/overlays/chromiumos-overlay/+/3292637)、[系统依赖](https://chromium-review.googlesource.com/c/chromiumos/overlays/chromiumos-overlay/+/3343875)、[上游项目](https://reviews.llvm.org/D108416)、[glibc 构建修正](https://chromium-review.googlesource.com/c/chromiumos/overlays/chromiumos-overlay/+/4677913)。各变更原文、状态和 SHA 见[合入记录](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/052_cros_changes.stdout)。

当前 ChromeOS ebuild 仍描述将 builtins 与 libunwind 合成 `libgcc_s.so.1`、配置版本脚本及 frame APIs。它是构建配置证据，**不是本组对当前镜像的观察**。[ebuild](https://chromium.googlesource.com/chromiumos/overlays/chromiumos-overlay/+/main/sys-libs/llvm-libunwind/llvm-libunwind-9999.ebuild)、[抓取快照](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/sources/cros_ebuild.txt)。

关于旧二进制：兼容文件名及 GCC 符号版本、SDK 与镜像构建协调均有证据；任意外部二进制支持策略、旧／新根文件系统切换全过程、所有插件验证分母则为 `NOT_AVAILABLE`。没有依据选择“全量重刷”“全部重编”或“完全靠符号版本”作为唯一答案。GCC 接口的版本节点也不等同于为 libc++ 同一函数保留两套行为。

关于取消：有展开器强制展开实现与 ARM 修复，ChromeOS 作者也报告过混合展开器的真实问题；没有获得 ChromeOS 迁移时按取消方式、库版本、架构划分的完整验收矩阵。不能写“全部验证过”，也不能写“因为完全不用取消所以没有验证”。

## 总部八项适用性核对

DSO 指动态共享对象；下表“镜像”特指集成后的实际系统，不包括本机隔离进程。

| 项 | 配置／静态事实 | 本次实测或实际镜像观察 | Tizen 适用性分析与缺口 |
|---|---|---|---|
| 提供方布局 | 本次 llvm-libgcc 别名最终指向 `libunwind.so.1.0`，SONAME 为 `libunwind.so.1`；ChromeOS 当前构建方式不同。 | 本机每次确认实验路径；Tizen／ChromeOS 替换镜像 `NOT_OBSERVED`。 | 能加载不等于符合安装器、沙箱、加载命名规则；不能直接照搬本次安装布局。 |
| 导出与版本 | Tizen x86 sysroot 198 个已定义符号／版本组合，实验库 231 个；前者有 108 个组合不在后者集合。此口径包含平台表的 15 个 ABS 版本节点名。 | ELF 实查；不是运行镜像符号分母。 | 整体兼容未成立。缺少组合不一定缺同名函数，可能版本不同；须有实际消费者导入集合才能判定受影响产物。不是 108 个缺失函数或 108 个失败消费者。 |
| `pthread_cancel` | glibc 使用强制展开；上层 ABI、异常规格、回滚仍参与。 | 本机两提供方结果一致，见下节；目标镜像 `NOT_OBSERVED`。 | 有限配置可行，不证明吞取消、异步取消、所有标准库内部路径安全。 |
| `backtrace` | glibc 动态取得展开入口。 | 本机主程序及插件栈帧数量断言通过，各提供方 5 次；镜像 `NOT_OBSERVED`。 | 未覆盖信号上下文、优化尾调用、JIT／动态登记帧和生产崩溃报告器。 |
| 跨 DSO | 上下文及 personality 必须形成一致协议。 | 插件抛出整数 42，主程序捕获且清理计数 1，各提供方 5 次；镜像 `NOT_OBSERVED`。 | 同一 ABI 对的异常样本通过；不同标准库对象布局、深绑定、多提供方顺序不在本次覆盖。 |
| `dlopen` 插件 | 动态加载可能引入其他提供方及新版本需求。 | `RTLD_NOW|RTLD_LOCAL` 加载／异常／关闭通过，逐次 maps 核实；镜像 `NOT_OBSERVED`。 | 不包括 `dlmopen`、`RTLD_DEEPBIND`、带静态展开器的插件或旧产品插件。 |
| 预编译产物 | 平台有 GNU 依赖产物；外部产品分母仍不可得。 | 本次只测自建探针，外部预编译集合 `NOT_AVAILABLE`。 | 当前证据不支持承诺兼容；缺 BOM、产物、导入版本、重建声明与更新边界。 |
| armv7l gnu-eabi | 文档列 armv7a gnueabihf；Tizen 宏检查有 ARM EABI/VFP 指令能力，但无 `__ARM_PCS_VFP`。版本脚本判定未检查该调用约定宏。 | 脚本预处理通过、现有 ARM sysroot ELF 已读取；实验库 ARM 运行和替换镜像均 `NOT_OBSERVED`。 | 预处理接受不证明浮点调用约定或 EHABI 兼容；不能把 gnueabi 与 gnueabihf 无条件等同。 |

证据：[实验库身份](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/023_experimental_identity.stdout)、[符号差异](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/symbol_comparison.tsv)、[ARM 宏](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/034_arm_macros.stdout)、[版本脚本](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/snapshots/gcc_s.ver.in)、[ARM ELF 与源码版本](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/053_local_arch_metadata.stdout)、[实测设计](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/MEASUREMENTS.md)。

## 本地展开器对照结果：实测

环境为 x86_64 原生 Ubuntu glibc 2.39，使用 Tizen 编译包装器及既有 libc++／libc++abi 产物；不是 Tizen 原生镜像或 QEMU。实验构建、复制的运行库和探针只在隔离目录；每次正式运行检查 `/proc/self/maps` 和符号实际归属，排除实验组混入系统 GNU 展开器。保留异常规格组的终止处理器输出诊断并以 86 退出；不是把 86 误称成默认 SIGABRT。[宿主身份](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/raw/051_host_environment.stdout)、[测量方法](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/MEASUREMENTS.md)。

| 用例 | GNU 提供方 | LLVM 提供方 | 行为断言 |
|---|---|---|---|
| 保留规格：普通等待及三个时钟变体 | 每格 5/5 终止 | 每格 5/5 终止 | 进入终止处理器，外层清理计数 0。 |
| 解除规格：相同四种等待 | 每格 5/5 取消退出 | 每格 5/5 取消退出 | join 返回取消标记，清理计数 1。内联头文件已随探针重编。 |
| 无回滚：普通／定时写者 | 每格 5/5 残留 | 每格 5/5 残留 | 原读者释放后仍有写者位 2147483648，新读／写获取失败。进程退出 0 表示“预期损坏被准确断言”，不是正常状态。 |
| 有回滚：普通／定时写者 | 每格 5/5 通过 | 每格 5/5 通过 | 清除写者位、保留已有读者，后续读／写真实进展，最终状态 0。 |
| backtrace＋插件跨 DSO 异常 | 5/5 通过 | 5/5 通过 | 栈帧阈值、整数 42、清理 1、关闭成功。 |
| 修补 ABI 对，捕获后重抛取消 | 5/5 通过 | 5/5 通过 | 捕获 1、清理 1、线程取消退出。 |
| 未修补 ABI 对，同一重抛形态 | 5/5 SIGABRT | 5/5 SIGABRT | foreign exception 诊断；未得到干净取消退出。 |

共 30 格 × 5 次。这里没有重新跑全部标准库站点、普通错误未捕获矩阵、所有时序或异步取消；两处写者及模板实例的通过不等于所有模板参数、用户回调都已静态证明或实测覆盖。

## 七类已知问题与四项既定决策

### 已知问题

“对方未观测”不是“对方没有”，更不能补写其未发生原因。公开材料中的接近问题与本次本地复现实验分开列出。

表中 GNU 独有／无同形保护的八个源站点指：实验 executor 四处、GNU vstring 两处、
condition_variable_any 重锁守卫一处、TR2 dynamic_bitset 一处；不是当前 LLVM 下实测的八个故障。
两处既有不可达保护指 `basic_string::shrink_to_fit` 和 `basic_ostream<char>::sentry` 析构的不抛边界，
不能混同于后续已修复的两项代码缺陷。本次未为这些站点新增运行结果。

| 已知问题 | Chromium／ChromeOS 的证据 | 与 Tizen 工作的关系 |
|---|---|---|
| `catch (...)` 吞取消 | 对其完整产品行为 `NOT_OBSERVED`；当前主仓存在取消入口，不能凭协作封装解释所有第三方。 | 本次只测捕获再重抛，不冒充吞取消测试；ABI 修补组仍有价值，换展开器未使其自动多余。 |
| libc++ 内部缺少防护 | 固定 libc++ 源码的写者等待无回滚；其他所有内部 catch 未逐一重审。 | 两写者残留已在 LLVM 提供方复现；需要区分终止边界修正与状态一致性修正。 |
| `noexcept` 边界 | 固定依赖的条件变量等待仍带相关规格。 | GNU／LLVM 均在保留规格时终止；展开器不绕过语言边界。 |
| GNU 独有的八处防护 | 对方逐点处理记录 `NOT_AVAILABLE`，未取得一一对应产品验证。 | 不由 llvm-libgcc 补出 libc++ 缺少的高层路径；八处在新组合下未重测。 |
| 两处防护不生效 | 对方精确同形验证 `NOT_OBSERVED`。 | 没有新实测足以撤销原结论；换展开器后的逐点结果仍缺。 |
| 异步取消绕过防护 | 公开规范禁用前提未成立；Tizen Starfish 实有异步设置。 | 调用时刻与资源原子性问题不是换展开器就消失；本次未新增异步矩阵。 |
| 跨标准库对象布局差异 | Chromium 的 ABI 命名空间隔离及统一构建不是对象转换；旧外部二进制完整策略不可得。 | 换展开器不改变 `std`／Boost 对象布局。本次跨 DSO 整数异常通过不是跨布局通过。 |

### 四项已定处理的对照

| Tizen 已定处理 | Chromium／Google 是否有同一决策点、如何处理 | llvm-libgcc／ChromeOS | 一致性与原因 |
|---|---|---|---|
| 解除等待规格并回滚写者状态 | 所检查依赖仍保留等待规格，写者先置位后等待且无相应回滚；产品是否取消该等待未观测。 | LLVM 展开器本机复现保留规格终止与解除后残留；ChromeOS 精确库补丁证据不可得。 | 不存在“换展开器代替回滚”的实测依据；未发现与 Tizen 同一套已实施改动的证据。 |
| 旧调用方双版本符号 | `__Cr` 命名空间与受控构建不是同一函数的旧／新行为双版本；对任意旧第三方二进制承诺不可得。 | GCC 兼容符号版本和提供方命名有证据，SDK 切换有提交；不是 libc++ 等待函数双语义版本。 | 机制可比较，但不能认为已采用相同方案或解决所有旧内联调用点。 |
| `wbuffer_convert` 析构不再同步 | 所检查头文件析构仍调用 `__close()`，后者调用同步；主仓仅命中头文件清单，不足证明所有产品不用。 | 展开器不改变析构算法；ChromeOS 对该类精确改动记录不可得。 | 当前所查 Chromium 源码与该决策不同；不能以零使用假设解释差异。 |
| Boost 自身迁移以消除跨库语义边 | 自带标准库的第三方构建可保持统一；历史讨论反对整体引入 Boost，但仓中存在 Boost 头引用样本。 | 系统展开器迁移不等于所有第三方改用 libc++；完整库清单不可得。 | 只有同一对象边界两端确实使用兼容 ABI 时才消除布局差异；不能从展开器统一推导 Boost 两端已统一。 |

源码依据：[条件变量实现](https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx/+/97b436da4c33663581d394f4ee0a5977fc38c2f4/src/condition_variable.cpp)、[等待头文件](https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx/+/97b436da4c33663581d394f4ee0a5977fc38c2f4/include/__condition_variable/condition_variable.h)、[写者实现](https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx/+/97b436da4c33663581d394f4ee0a5977fc38c2f4/src/shared_mutex.cpp)、[定时写者头文件](https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx/+/97b436da4c33663581d394f4ee0a5977fc38c2f4/include/shared_mutex)、[转换缓冲析构](https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx/+/97b436da4c33663581d394f4ee0a5977fc38c2f4/include/__locale_dir/wbuffer_convert.h)、[Boost 历史评审](https://codereview.chromium.org/2242483003/)。

## 其他实际问题、可参考方法及盲区

[独立参考问题库](https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/REFERENCE_ISSUES.md)列出 11 项：旧构建产物混用、GPU／ANGLE 集成、静态初始化统计边界、NaCl 文件布局、隐式 pthread 依赖、Breakpad 多架构路径、混合展开器、ARM EHABI、glibc 引导构建顺序、读写锁性能／调度，以及本次发现的版本脚本／符号覆盖差异。每项分别列发现方式、处理证据、已知遗留、Tizen 触发条件、现有测试盲区及可选探测方法。

该清单是有出处的问题库，**不是“历史所有问题已穷尽”**：私有 bug、内部变更、实际旧镜像和完整产品测试不可得；公开检索受范围限制。Tizen 的微型取消及布局探针能够检出部分运行状态问题，但不能检出全量构建依赖、沙箱文件解析、真实 GPU 集成、生产崩溃栈完整性或锁性能退化。

可参考的是固定工具链身份、实际提供方核验、版本化导出比较、干净构建和跨 DSO／插件断言；它们需按 Tizen 架构与产物改造。不可当作现成证明的是“全 Google 禁取消”、任意外部二进制均受受控 SDK 管理、ARM 与 x86 相同、符号名字相同就能混用上下文。这些前提或未成立，或缺证据。

## 为什么 Tizen 不采用 Chromium 的方案

这里的“不采用”是指：**不把 Chromium 的协作退出协议直接当作 Tizen 全平台的取消支持方案**，并非否定其工具链或 ABI 隔离方法。下表区分已证实的前提与尚不能证实的概括；Chromium 一侧的局部做法不能自动成为 Tizen 所有调用方的约束。

| Chromium 的前提／做法 | 证据（URL） | Tizen 的情况及不能直接沿用的原因 | 证据（URL） |
|---|---|---|---|
| 使用 `base::Thread` 的线程，退出走请求停止、处理退出工作、join 的协作协议。 | [固定版本 thread.cc](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/base/threading/thread.cc) | 已检查源码有真实取消调用；lightweight-web-engine 与 OpenMP 的条件路径启用异步模式。不能假定这些线程全部经该封装退出；这里证明的是条件可达，不是每个产品都实际触发。 | [两个组件的源码路径及异步取消实测](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R108/W3_REPORT.md) |
| 协作退出的有效性依赖调用方采用并遵守协议，不能从代码封装推成全仓强制规则。本次主仓选定文本仅发现一处直接 `pthread_cancel` 调用，但它确实存在。 | [gRPC 的实际调用](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/third_party/grpc/source/src/core/util/posix/thd.cc)、[固定范围检索结果](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R116/chromium_usage.tsv) | Tizen 的仓外应用、供应商组件及插件不在完整覆盖范围内。现有材料无法保证它们全部遵守统一退出协议；库更新本身也不能要求不可得的源码一律改用该协议。这不是“永远不能制定产品约束”的断言。 | [外部组件缺口、材料与责任方](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R110/W1_REPORT.md) |
| 统一构建配置及固定依赖使受该构建管理的目标可保持一致；**配置不能证明“monorepo 每次全量重编、完全不考虑旧二进制”**。 | [标准库构建配置](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/build/config/c++/c++.gni)、[固定依赖清单](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/DEPS) | 已确认 5 个只复制安装、不随迁移重建的预编译 DSO；仓外第三方产物和重建策略的分母仍不可得。因此不能把“所有边界两侧都会重编”作为平台前提；也不能给未知外部集合补写数量或确定策略。 | [不随迁移重建组件清单](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R108/W2_REPORT.md)、[外部支持范围缺口](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R110/W1_REPORT.md) |
| ABI 命名空间 `__Cr` 隔离标准库符号；该机制不是跨标准库对象转换。 | [固定版本 __config_site](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/buildtools/third_party/libc++/__config_site) | 布局实例中，libstdc++ 提供方与 libc++ 消费方成功链接／加载，却把同一 `deque` 解释成不同大小和元素数。命名空间不能修复未在接口名中暴露的内部布局；该实例是 x86_64 最小探针，不是断言所有生产边都失败。 | [跨库对象布局实例与原始证据索引](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R80/REPORT.md) |
| 普通 Linux 默认不启用自带 libunwind，而使用系统提供方；系统为 GNU libgcc 时仍走 GNU 展开器。不能把该配置泛化为所有 Linux 产品（包括 ChromeOS）都用 GNU。 | [固定版本 unwind.gni](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/build/config/unwind.gni) | **在同用 GNU 提供方的比较下，展开器不是差异点。** 本地 30 格 GNU／LLVM 对照又表明，换展开器不消除规格终止或写者残留。该实测来自宿主，不冒充 Tizen 镜像验证。 | [30 格对照及环境身份](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R116/MEASUREMENTS.md)、[逐格结果](https://github.com/lhmax2010/libc-_replacement/blob/d211d4453d60545605a6b4398d4fc442aa14ed0a/docs/progress/R116/measurement_summary.tsv) |

下述概括只针对已核查的协作退出路径及 Tizen 已确定的支持目标：它不声称 Chromium 全产品不用取消，也不声称修库后任意调用链、所有异步取消或所有用户回调都安全；本文已有的架构、调用链与未修复路径限制继续成立。

**概括而言，Chromium 的这类方案是“约定 + 在遵守协议的路径上不用取消”，Tizen 的方案是“修库 + 在明确边界内支持取消”；前者依赖调用方遵守协作协议，后者不以所有调用方都放弃取消为前提，两者不是优劣之分，而是前提不同。**

## 适用性结论与既有工作影响

**Chromium：部分做法可参考，不能整体照搬为 Tizen 的取消安全方案。** 已有的协作退出与 ABI 隔离适用范围取决于调用方协议及边界统一；Tizen 的既有取消和外部二进制缺口不随链接配置消失。规范级“免疫前提”未被证实。

**llvm-libgcc：本机有限组合构建及运行可行，平台替换适用性未完成证明。** 直接导出差异、实际提供方布局差异、目标镜像与 ARM 验证缺失、外部预编译分母缺口都仍存在；本次不能给出兼容性验收结论。

如果换展开器，**“需要重验”与“需要重写”不是同一件事**：四个补丁与新提供方组合、五处边界、两个异步取消组件、旧符号调用、析构及 Boost 布局证据都要重新限定覆盖条件；本次只补其中部分 x86 组合。没有证据要求四补丁全重写，也没有证据允许全部撤销。写者回滚的两个实例在 LLVM 下仍必要于本次已观测状态恢复；其他任意模板／回调未证明。对象布局和头文件旧调用元数据不会由替换展开器修正，外部产物材料也仍需由其提供方补齐。

未决材料明确为：Google 内部取消规范；Starfish 历史；ChromeOS 旧二进制／插件与取消完整矩阵；Tizen 外部 BOM 和版本导入；ARM／aarch64 实验提供方及真实目标镜像。本文不据此代替人工决定是否采用、如何部署或先做哪一项。
