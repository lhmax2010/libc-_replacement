# security-manager：libc++ 迁移缺陷记录

## 错误现象

armv7l 候选构建的完整诊断原文：

```text
[   78s] In file included from /home/abuild/rpmbuild/BUILD/security-manager-2.4.0/src/common/check-proper-drop.cpp:31:
[   78s] In file included from /home/abuild/rpmbuild/BUILD/security-manager-2.4.0/src/common/include/check-proper-drop.h:28:
[   78s] /home/abuild/rpmbuild/BUILD/security-manager-2.4.0/src/common/include/utils.h:70:19: error: no type named '__forced_unwind' in namespace '__cxxabiv1'
[   78s]    70 |     } catch (abi::__forced_unwind &) {
[   78s]       |              ~~~~~^
[   78s] 1 error generated.
[   78s] make[2]: *** [src/common/CMakeFiles/security-manager-commons.dir/build.make:278: src/common/CMakeFiles/security-manager-commons.dir/check-proper-drop.cpp.o] Error 1
```

证据：`progress/R22/patches/security-manager_error_context.raw.log`；只读来源为 bak 下 `19_security-manager_bc_then_keepgoing.raw.log:549`。

## 触发条件

- 标准库：`-stdlib=libc++`；运行时包 `libc++-21.1.1-2`、`libc++abi-21.1.1-2`。
- 架构：armv7l，`-march=armv7-a -mthumb -mfloat-abi=softfp`。
- 编译器：`clang-22.1.8-18.1`，驱动 `/bin/armv7l-tizen-linux-gnueabi-clang++`。
- 原命令见 `progress/R22/commands/021_defect_evidence_extract.log`。

## 错误分析

`utils.h:32` 包含 `<cxxabi.h>`，第 70 行捕获 `abi::__forced_unwind` 后立即重抛，使强制展开不被后续异常处理吞掉。候选构建实测 libc++abi 的 `__cxxabiv1` 不声明该 libstdc++ C++ ABI 专有类型；原平台发布形态使用 libstdc++。

## 影响面

该文件是 security-manager 内部头，不是已观测到的跨源码包安装头。既有 44 条传播记录中，本头占 30 条，消费者全部是 `security-manager`；逐条文件:行号见 `progress/R22/patches/forced_unwind_propagation.tsv`。

## 解决方案

R30 候选以正向判据 `#if defined(__GLIBCXX__)` 隔离 GNU 专有的
`abi::__forced_unwind` catch。候选 patch 已归档，但因下述语义红项
仅供取证，**不可提交**。

初始候选在 libc++ 下删除精确 catch 后，pthread_cancel 被 catch-all
路径吞入并终止；用 libc++abi 的 current-exception-type 识别 foreign
exception 后重抛，仍以 uncaught foreign exception 终止。诊断变体
已撤出最终候选。

## 兼容性

- libstdc++：armv7l 基线与候选完整 GBS 构建均执行；逐制品对照见
  `progress/R30/tables/`。
- libc++：三架构构建均实际执行；原 `__forced_unwind` 错误消失后，
  仍有 `dpl/fstream_accessors.h` 的 libstdc++ 私有类型、测试源码的
  `std::system_error` 构造差异以及 Boost.Test ABI 链接红项。
- pthread_cancel：GNU 对照通过；libc++ 的 armv7l 与 x86_64 均退出
  134。因此 stdlib-neutral 行为兼容性未通过。

## 验证记录

- 已验证：libc++ armv7l keep-going 构建在该头产生多处同根错误；首条完整诊断见上文。
- 已验证：R1 交叉验证把本包标为 T1_HARD，和 T2 构建失败 MATCH。
- R30 已复核 44/44 传播记录，其中本头 30 条全部 MATCH。
- R30 已实测原编译错误消失，但完整 libc++ 构建被上述无关红项阻断。
- R30 语义矩阵与原文见 `progress/R30/tables/semantic_results.tsv`；
  结论为 RED_SEMANTIC_VALIDATION。

## 提交时机

暂不提交：候选未通过 pthread_cancel 语义门禁，且完整 libc++ 包构建
尚有未裁决红项。人工裁决运行时边界并完成双标准库验证后再评审。
