# 引用材料索引 / Evidence index

## 身份、可访问性及保留方式

来源获取日期为 2026-09-07。正式下载的 URL、实际 curl 退出码、解码后快照及 SHA256 逐项见 [source_index.tsv](source_index.tsv)；对应原始 HTTP 状态、stderr、命令原文在 `raw/fetch/`。网上检索及打开结果保存于 `raw/web/`。网页工具打不开但 curl 成功的来源按取得的原始内容核查，不把页面解析失败当作仓库不存在。

固定 Chromium 主仓 `ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1`：[提交元数据](sources/chromium_head.txt)、[固定 DEPS](sources/deps_pinned.txt)。完整归档原始获取命令及成功结果为 `raw/044*`，归档 SHA 为 `0f8ee6bdc0ace963f33e9bd7864d13e9c0bbaca83fb4faead0ebb4b61c97711b`。首次受大小上限限制的下载为失败，未用于完整扫描结论。

Chromium 配置、源文件与依赖仓文件尽量固定到上述主仓及其依赖 SHA；Google 公开指南、当前 ChromeOS ebuild 等 main 页面是获取时快照，不冒称永久固定 URL。完整文档仅作为证据快照，不把其中指令当成本地任务指令。

## Chromium 与 Google

| 主题 | 一手公开出处 | 本地材料 |
|---|---|---|
| C++ 与展开器配置 | [c++.gni](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/build/config/c++/c++.gni)、[unwind.gni](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/build/config/unwind.gni) | sources/cpp_config.txt、unwind_config.txt |
| ABI 命名空间／版本 | [配置头](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/buildtools/third_party/libc++/__config_site) | sources/configsite_actual.txt |
| 协作退出 | [Thread](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/base/threading/thread.cc) | sources/thread.txt；完整归档扫描独立核对 |
| 实际取消入口 | [gRPC POSIX thread](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/third_party/grpc/source/src/core/util/posix/thd.cc) | corpus_matches/e76dddad9f9cc6a7.txt；路径文件和 SHA 相邻保存 |
| 取消间接调用 | [线程池](https://chromium.googlesource.com/chromium/src/+/ec7cb9cc99bba09cdac51acfecc3eff6adba2ac1/third_party/grpc/source/src/core/lib/event_engine/thread_pool/work_stealing_thread_pool.cc) | sources/corpus_matches/aa22d005d835c6aa.txt |
| 公开规则 | [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) | sources/google_style.txt、style.txt；不等于内部规范 |
| 标准读写锁历史 | [维护者讨论，2021—2023](https://groups.google.com/a/chromium.org/g/cxx/c/bIlGr1URn8I) | raw/web/008.json、009.json、013.json、015.json |
| Boost 引入讨论 | [Asio coroutine 评审](https://codereview.chromium.org/2242483003/) | raw/web/；主仓实际 include 另见 chromium_usage.tsv |
| 浏览器迁移公告 | [2017 ChromeOS Chrome libc++](https://groups.google.com/a/chromium.org/g/chromium-os-dev/c/e8pJRHLisIg) | raw/web/ |
| Linux 默认切换／回退 | [回退](https://codereview.chromium.org/2965693002/)、[重新落地](https://codereview.chromium.org/2963223003/) | raw/web/ |
| ANGLE 更新 | [合入评审](https://chromium-review.googlesource.com/557679) | sources/angle_fix.txt；raw/057_supplement.stdout |

固定 libc++ 实现快照为 `sources/pinned_cv.txt`、`pinned_cv_header.txt`、`pinned_shared.txt`、`pinned_shared_header.txt`、`pinned_wbuffer.txt`；其完整 URL 和 SHA 见下载索引。检查对象是固定依赖，不是任意发行版 libc++。

## LLVM、glibc 与 ChromeOS

| 主题 | 一手公开出处 | 证据性质 |
|---|---|---|
| llvm-libgcc 定义／责任边界 | [项目文档](https://github.com/llvm/llvm-project/blob/main/llvm-libgcc/docs/LLVMLibgcc.md) | 当前公开文档；本次旧修订 .rst 另有本地快照 |
| 原提案／受控工具链 | [D106703](https://reviews.llvm.org/D106703?id=361316) | ABANDONED，讨论材料；不是最终落地证明 |
| 后续上游落地 | [D108416](https://reviews.llvm.org/D108416) | 上游项目历史 |
| ARM 强制展开 | [D109856](https://reviews.llvm.org/D109856)、[D111703](https://reviews.llvm.org/D111703) | 修复／相邻 ABI 处理记录，不是本次 ARM 实测 |
| 混合展开器 | [D150897](https://reviews.llvm.org/D150897)、[LLVM 18.1.1 说明](https://releases.llvm.org/18.1.1/projects/libcxx/docs/ReleaseNotes.html)、[发行版故障](https://github.com/llvm/llvm-project/issues/90041) | 项目讨论与外部报告；不混称为同一个 ChromeOS 事件 |
| ChromeOS 当前生成方式 | [llvm-libunwind ebuild](https://chromium.googlesource.com/chromiumos/overlays/chromiumos-overlay/+/main/sys-libs/llvm-libunwind/llvm-libunwind-9999.ebuild) | 构建说明，不是实际镜像 |
| glibc 2.40 动态展开链接 | [glibc 文件](https://sourceware.org/git/?p=glibc.git;a=blob;f=misc/unwind-link.c;hb=glibc-2.40) | 本地镜像提交与快照核查，不当作实际宿主 glibc 版本 |

ChromeOS 精选合入变更：3246062（真实文件布局）、3292637（Breakpad）、3298526（pthread）、3335237／3310954（全板启用）、3339453（SDK）、3343875（系统依赖）、4677913（glibc 引导）、4685180（CMake 构建辅助规则清理）。完整提交消息、状态和 SHA 为 [raw/052_cros_changes.stdout](raw/052_cros_changes.stdout)，差异为 `sources/cros_diff_*.txt`；每个公开 URL 亦在下载索引。4685180 只是 CMake 规则清理，未把它报告成新的运行时故障。

所检索的已合入 ChromeOS 元数据见 `sources/cros_merged_changes.txt`；返回条数和分页限制意味着它不是全项目历史穷举。早期关联变更 2945947 的 ABANDONED 状态与占位测试栏保留于 `sources/cros_change_2021.txt`，不可混作合入证据。

## 本地证据和缺口

- 实验 LLVM 源树提交：`5ed6c77278dfa7a470667cf1a137723d3c96fe60`；glibc 镜像提交：`3d1aed874918c466a4477af1da35983ab036690e`。本次没有修改它们。
- 运行环境身份、实际库映射、构建／探针命令、逐格结果及 ELF 均在 [MEASUREMENTS.md](MEASUREMENTS.md) 链接；原始日志保留实际退出码。
- Starfish README 与 WorkerThread 来自本地给定源码包；[来源表](snapshot_origins.tsv)记录路径及 SHA。无法访问其私有上游历史；公共远端尝试见 `raw/048*`。不把失败网页响应伪装成源码。
- 部分早期抓取使用错误的 BUILD 路径或拼写不完整的 revision 而失败；后续纠正至 buildtools 及正确 SHA。所有失败保留，结论只依赖取得的正确快照。
- 内部规范、私有缺陷细节、实际 ChromeOS 镜像、外部 Tizen 产物清单和实验 ARM 镜像不可得／未观测；本文没有推断补全。
