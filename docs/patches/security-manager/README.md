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

NOT_AVAILABLE：本任务未开发或裁定具体源码修改，且没有可据以证明线程取消语义保持的补丁实测。

## 兼容性

NOT_AVAILABLE：尚无补丁，因此不存在修改后 libstdc++ 与 libc++ 双构建实测。

## 验证记录

- 已验证：libc++ armv7l keep-going 构建在该头产生多处同根错误；首条完整诊断见上文。
- 已验证：R1 交叉验证把本包标为 T1_HARD，和 T2 构建失败 MATCH。
- 未验证：补丁内容、两套标准库回归构建和线程取消运行时语义，均 NOT_AVAILABLE。

## 提交时机

暂不提交：该修改只因 libc++ 迁移需要；待平台 libc++ 上下文建立后依次开发、双标准库验证并提交评审。
