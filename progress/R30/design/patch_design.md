# R30 补丁设计与语义红项修订

## 共同约束

- 修改范围严格限定为 `src/common/exceptions/TryCatch.h` 与 `src/common/include/utils.h`。
- GNU 专有的 `abi::__forced_unwind` catch 子句仅在 `__GLIBCXX__` 已定义时参与编译；其他 catch 子句的次序、内容与日志行为不变。
- 采用 `#if defined(__GLIBCXX__)` 的正向判据：该宏由 libstdc++ 头定义，恰好证明专有类型可用。没有采用 `#ifndef _LIBCPP_VERSION`，因为“不是 libc++”不能证明一定是 libstdc++，会让未知第三方标准库错误进入 GNU 专有代码。
- 保留 `<cxxabi.h>` include，不扩大改动面；本次错误来自该头在 libc++ 下不声明 `__forced_unwind`，而不是头文件不存在。

## 初始候选的实测否决

初始候选仅隔离 GNU 专有 catch。它虽消除了 libc++ 编译错误，但
armv7l/QEMU 与 x86_64 原生语义用例均以
`FATAL: exception not rethrown`、退出码 134 失败。因此，“libc++
侧移除该 handler 后强制展开不会被 catch-all 吞掉”在本项目实际
`libc++abi + libgcc_s` 组合中不成立；该候选不得提交。

## 诊断性修订设计（亦被实测否决）

保留 GNU 分支原有精确 catch。在 libc++ 分支进入最终 `catch (...)`
后，先调用 GNU 与 LLVM 的 `<cxxabi.h>` 均声明的
`abi::__cxa_current_exception_type()`；若返回空指针则立即重抛，
不执行兜底处理。libc++abi 源码明确对 foreign exception 返回空指针。
普通 C++ 异常具有非空 typeinfo，仍继续原日志与返回值路径。

`_LIBCPP_VERSION` 是“正在使用 libc++”的正向判据，因此不会把上述
libc++abi 行为外推到未知标准库。未知标准库既不会编译 GNU 私有类型，
也不会启用未经验证的 foreign-exception 判别。

该诊断变体在 armv7l 与 x86_64 均实际进入 libc++abi 的重抛路径，
随后报 `terminating due to uncaught foreign exception`、退出 134。
因此它同样被否决并从两个目标文件撤回。源码树和 format-patch 中
保留的是最小候选，且只作为失败证据，不代表已通过设计。

## libcynara-commons

在 `catch (const abi::__forced_unwind &)` 前后增加条件编译。保留 `const`、原注释、`CYNARA_LOGD` 与 `throw`。该子句原位于 `std::bad_alloc`、Cynara 业务异常和 `std::exception` 处理之后、catch-all 之前，位置不调整。

## security-manager

在 `catch (abi::__forced_unwind &)` 前后增加同一条件编译。保留非 `const` 形态与 `throw`。该子句原位于全部普通异常 handler 之前，位置不调整。

## 语义预期与验证方法

待验证预期曾为：libstdc++ 构建由专用 catch 重抛；libc++ 构建识别
foreign exception 后重抛并使 `pthread_join` 返回
`PTHREAD_CANCELED`。两架构实测已否决后半句。

验证以两套标准库分别编译同一最小 pthread 用例，观测 catch、析构、
join 与退出码；结果见 `tables/semantic_results.tsv`。最终状态为
`RED_SEMANTIC_VALIDATION`，没有可申报为通过的源码方案。
