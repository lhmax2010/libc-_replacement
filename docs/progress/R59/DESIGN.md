# R59 修复设计（仅设计，未实现）

## F1：`ios.cpp` 两个汇聚函数

现代码先用一次嵌套 `throw;` 分类当前异常，离开内层 `catch (...)` 后再依据异常掩码执行第二次 `throw;`。对非强制外来异常，第一次重抛已完成其 handler 生命周期；第二次裸重抛时不再有活动异常，实测进入 terminate handler。

建议修复文本（badbit；failbit 同构替换位值）：

```cpp
void ios_base::__set_badbit_and_consider_rethrow() {
  __rdstate_ |= badbit;
#if _LIBCPP_HAS_EXCEPTIONS
  try {
    throw;
  } catch (const __cxxabiv1::__forced_unwind&) {
    throw;
  } catch (...) {
    if (__exceptions_ & badbit)
      throw;
  }
#endif
}
```

关键点是：需要传播的普通/外来异常必须在内层 handler 仍活跃时重抛；函数尾部不再执行裸 `throw;`。强制展开仍无条件重抛。逐路径等价性见 `tables/f1_path_equivalence.tsv`。改动局限于两个 out-of-line helper，不改变其他头文件站点。

## F2：`get(basic_streambuf&, delim)`

建议把 typed handler 改为：

```cpp
} catch (const __cxxabiv1::__forced_unwind&) {
  __state |= ios_base::badbit;
  this->__setstate_nothrow(__state);
  throw;
}
```

`__setstate_nothrow` 在重抛前提交 `__state`，避免普通 `setstate` 按异常掩码再抛出另一异常并替换强制展开。该形态与同文件相邻 handler 及 libstdc++ `_M_setstate(badbit); rethrow` 对齐。它只触及该 overload 的 typed 分支，不改变 catch-all 的既有规范行为。

## M1：deferred future（待人工选择）

### 选项 M1-A：恢复 deferred 标记，允许重试

- 在 forced handler 中，通过基类新增的加锁、无抛异常内部操作恢复 deferred 标记，再重抛。
- 后续等待者可再次执行 callable；本次 libstdc++ 参考实测调用次数从 1 增到 2 后得到 42，语义最接近其 `call_once` 失败后可重试的实现。
- 后果：callable 在第一次被取消前可能已经产生外部副作用，重复执行不保证业务安全；C++ 标准没有为线程取消定义此状态策略。
- 实现代价：需要在共享状态锁下原子恢复标记，并处理同时到达的其他 waiter；不能直接无锁写 `__state_`。

### 选项 M1-B：以永久错误完成共享状态

- 在 forced handler 中用无抛异常、幂等的内部完成操作写入一个确定错误并置 ready，随后重抛。
- 后续等待者立即获得错误，不重跑 callable，也不会永久等待。
- 后果：future 永久失效；与 libstdc++ 的可重试行为不一致。需要裁决错误类别（`broken_promise`、现有 future_error 或新增内部状态）。
- 实现代价：需要一个不会因已 ready 而抛异常的原子完成接口，以及错误语义和测试契约。

### 选项 M1-C：执行期间屏蔽取消

- 在 deferred callable 执行窗口禁用线程取消，退出后恢复原状态。
- 后果：避免半执行后的状态选择，但取消会被推迟；callable 若长期不返回，取消请求也无法完成。若 callable 自行重新启用取消，库侧限制还可能失效。
- 与 libstdc++ 不一致，且引入 pthread 平台耦合；必须以取消状态的可观测测试证明约束真实生效。

## M2：async 完成操作不得替换原传播（待人工选择）

### 选项 M2-A：原子的无抛异常 `try_complete_with_error`

- 增加内部接口：持锁检查 ready；未 ready 时写入 broken-promise 类错误并通知，已 ready 时无操作返回；接口保证不抛异常。
- forced handler 无条件在该接口之后执行原 `throw;`。
- 与 libstdc++ 的 `_M_result` 检查加 `_M_break_promise` 方向最接近；本次参考实测完成取消传播，等待方得到 broken promise。
- 不能用“先 `__has_value()` 再 `set_exception()`”的分离检查替代，否则存在 TOCTOU 窗口。

### 选项 M2-B：隔离完成操作的异常，始终恢复原传播

- 在 forced handler 内把完成操作包在独立 try/catch 中；完成操作的任何异常只记录/丢弃，随后执行原 `throw;`。
- 优点是改动集中；已 ready 时 `promise_already_satisfied` 不再替换原传播。
- 后果：未 ready 时若因分配失败等原因未能完成共享状态，等待者可能仍不就绪；因此必须另行定义并测试该失败路径，不能只靠吞异常。

### 选项 M2-C：在库控制的发布窗口施加可验证的取消限制

- 对库控制的共享状态发布临界区禁用取消，并在离开临界区后恢复；以运行时读取取消状态和确定性时序用例验证。
- 后果：平台耦合、推迟取消，并且用户 callable 可自行改变取消状态；要做到“排除异步取消”必须定义库是否覆盖/拒绝用户修改。仅写文档声明不构成限制。
- 与 libstdc++ 当前处理方式不一致。

本报告不在上述 M1/M2 选项中作选择。
