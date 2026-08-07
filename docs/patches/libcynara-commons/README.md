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

NOT_AVAILABLE：本任务按纪律只建立目录与文档，尚未开发或裁定具体源码修改；不得以未经双标准库构建验证的改写代替方案。

## 兼容性

NOT_AVAILABLE：尚无补丁，因此不存在“修改后在 libstdc++ 与 libc++ 下均可构建”的实测记录。

## 验证记录

- 已验证：原代码在 libc++ 候选构建中于 10 个本包 TU 报同一错误；安装头还阻断 libcynara-dbus 的 TU，详细逻辑诊断位于既有 bak 只读证据。
- 已验证：R1 交叉验证把本包标为 T1_HARD，和 T2 构建失败 MATCH。
- 未验证：补丁内容、libstdc++ 回归构建、libc++ 修复后构建、线程取消运行时语义，均 NOT_AVAILABLE。

## 提交时机

暂不提交：该修改只因 libc++ 迁移需要；待平台 libc++ 上下文建立后依次开发、双标准库验证并提交评审。
