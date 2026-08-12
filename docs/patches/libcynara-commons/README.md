# libcynara-commons：libc++ 迁移缺陷记录

## 错误现象

armv7l 候选构建的完整诊断原文：

```text
[   83s] In file included from /home/abuild/rpmbuild/BUILD/cynara-0.26.0/src/helpers/session/session.cpp:34:
[   83s] /home/abuild/rpmbuild/BUILD/cynara-0.26.0/src/common/exceptions/TryCatch.h:65:25: error: no type named '__forced_unwind' in namespace '__cxxabiv1'
[   83s]    65 |     } catch (const abi::__forced_unwind &) {
[   83s]       |                    ~~~~~^
```

证据：`progress/R22/patches/libcynara-commons_error_context.raw.log`；只读来源为 bak 下 `01_libcynara-commons.raw.log:695`。

## 触发条件

- 标准库：`-stdlib=libc++`；运行时包 `libc++-21.1.1-2`、`libc++abi-21.1.1-2`。
- 架构：armv7l，`-march=armv7-a -mthumb -mfloat-abi=softfp`。
- 编译器：`clang-22.1.8-18.1`，驱动 `/bin/armv7l-tizen-linux-gnueabi-clang++`。
- 原命令见 `progress/R22/commands/021_defect_evidence_extract.log`。

## 错误分析

`TryCatch.h:32` 包含 `<cxxabi.h>`，第 65 行捕获 `abi::__forced_unwind` 并立即重抛。现有源码注释明确该分支用于保持 `pthread_cancel` 的强制展开不被 catch-all 吞掉。候选构建实测 libc++abi 的 `__cxxabiv1` 不声明该类型；R1 将 `__forced_unwind` 以两包真实编译失败为锚归入 T1_HARD。原平台发布包使用 libstdc++，该类型由其 C++ ABI 头提供。

## 影响面

该头由 cynara-devel 安装，是对外公开头。既有 44 条传播记录中，本头占 14 条：libcynara-commons 自身 13 条，libcynara-creds-dbus 1 条；逐条文件:行号见 `progress/R22/patches/forced_unwind_propagation.tsv`。已实测受影响源码包：`libcynara-commons`、`libcynara-creds-dbus`。

## 解决方案

R30 候选以正向判据 `#if defined(__GLIBCXX__)` 隔离 GNU 专有的
`abi::__forced_unwind` catch，未知标准库不会被误判为 libstdc++。
候选 patch 已归档，但因下述语义红项仅供取证，**不可提交**。

初始设计预期 libc++ 的强制展开不会匹配后续 catch-all；armv7l 与
x86_64 实测否决了该预期。进一步使用
`abi::__cxa_current_exception_type()` 识别 foreign exception 后重抛，
两架构仍由 libc++abi 以 uncaught foreign exception 终止。该诊断
变体已撤出最终候选。

## 兼容性

- libstdc++：armv7l 基线与候选完整 GBS 构建均执行；逐制品对照见
  `progress/R30/tables/`。
- libc++：三架构均越过原 `__forced_unwind` 编译错误，但完整包随后
  因 `commandline_options.cpp` 的 nodiscard/Werror 与
  `commands_dispatcher.cpp` 的 libstdc++ 私有 `_S_badbit` 红停。
- pthread_cancel：libstdc++ 的 x86_64 原生与 armv7l/QEMU 用例通过；
  libc++ 的两架构用例均退出 134。故 stdlib-neutral 行为兼容性
  **未通过**，不能以“编译错误消失”替代。

## 验证记录

- 已验证：原代码在 libc++ 候选构建中于 10 个本包 TU 报同一错误；安装头还阻断 libcynara-dbus 的 TU，详细逻辑诊断位于既有 bak 只读证据。
- 已验证：R1 交叉验证把本包标为 T1_HARD，和 T2 构建失败 MATCH。
- R30 已验证 44/44 include 传播记录，当前树逐条 MATCH。
- R30 已验证候选不再产生 `__forced_unwind` 编译诊断，但发现上述
  两类无关源码红项，完整 libc++ 包构建未成功。
- R30 语义矩阵与原始输出见 `progress/R30/tables/semantic_results.tsv`
  及对应命令日志；结论为 RED_SEMANTIC_VALIDATION。

## 提交时机

暂不提交：候选未通过 pthread_cancel 语义门禁，且完整 libc++ 包构建
尚有未裁决红项。人工裁决运行时边界并完成双标准库验证后再评审。
