# M2 超时现象勘误（2026-08-21）

本记录只补充勘误，不修改 `REPORT.md` 原文。

## 勘误内容

- 原报告把退出码 124 的超时记作“ready 时无操作”的语义后果；R61 的挂起现场调用栈证明，超时实际来自测试夹具的伴生效应。
- 原夹具的移动构造函数会无条件把源对象的触发位重新置真。`future::get()` 移走值后，共享状态中的残骸因此被重新武装；主线程在释放共享状态时进入该残骸的析构自旋。
- “libstdc++ 返回 `broken_promise`”应限定为结果已经构造、但尚未发布为 ready 的窗口。结果已经发布后，等待方可取得该值。

## 调用栈依据

挂起现场中工作线程已经退出，主线程的关键栈为：

```text
#0 AsyncWindowResult::~AsyncWindowResult (...) at r59_probe.cpp:169
#1 std::__1::__assoc_state<AsyncWindowResult>::__on_zero_shared (...) at future:631
#2 std::__1::__async_assoc_state<...>::__on_zero_shared (...) at future:900
#3 std::__1::__shared_count::__release_shared (...) at shared_count.h:65
#4 std::__1::__release_shared_count::operator() (...) at future:1019
#5 std::__1::unique_ptr<...>::reset (...) at unique_ptr.h:288
#6 std::__1::unique_ptr<...>::~unique_ptr (...) at unique_ptr.h:254
#7 std::__1::future<AsyncWindowResult>::get (...) at future:1034
#8 run_m2 (...) at r59_probe.cpp:188
```

未经整理的完整调试器输出收录于 `docs/progress/W1/code/raw/004_gdb_hang_stack.log`。

## 修正夹具后的实测

修正夹具把取消动作放到主线程，结果对象中的一次性窗口哨兵只负责暴露“返回值临时对象析构”窗口；移动构造不再重新武装共享状态残骸，并补齐输出刷新。以 R60 的“ready 时无操作”实现重跑：

```text
M2_WINDOW=AFTER_SET_VALUE_BEFORE_TRY_EXIT
M2_CANCEL_RC=0
M2_PROCESS_SURVIVED_CANCEL_PROPAGATION=1
M2_FUTURE_GET=VALUE
EXIT_CODE=0
```

因此，124 超时不是“ready 时无操作”的可观察语义，而是原夹具的生命周期错误。本勘误不改变已经裁决的 M2 方案选择。

