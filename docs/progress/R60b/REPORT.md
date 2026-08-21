# R60b：已完成部分上传与 libstdc++ broken_promise 来源查证

## 结论摘要

来源性质判定为 **A（更准确地说是 async 的强制展开 handler 主动完成状态）**，不是共享状态析构或引用释放路径的通用兜底。

libstdc++ 的 `_Async_state_impl::_M_run` 在 `catch (__forced_unwind&)` 中显式检查其尚持有的 `_M_result`，调用 `_M_break_promise(std::move(_M_result))`，随后重抛。`_M_break_promise` 把 `broken_promise` 写入 result 对象、交换到共享状态、置 ready 并通知等待方。该路径既能由 `pthread_cancel` 触发，也能由同样采用强制展开的 `pthread_exit` 触发；它不是对 `pthread_cancel` API 名称的特判。

## R60 已上传状态

R60 的当前完整文件、总 diff、两个当前补丁、测试源码、逐项编译链接命令、CMakeCache、基线与产物 SHA256、运行时加载记录、完整矩阵输出和退出码已置于 `docs/progress/R60/`。

- F1：x86_64 定向验证通过，与 libstdc++ 一致。
- F2：x86_64 取消完成且 `badbit=1`，与 libstdc++ 一致。
- M1-A：x86_64 callable 重试次数为 2；4 个并发等待者的当前版与 libstdc++ 对照各 20/20 通过。
- M2：当前仍是“ready 时 no-op”审阅版；R59 定向格超时 124，而 libstdc++ 返回 `broken_promise`、退出 0，因此暂停。
- 两架构库均构建成功，armv7l 测试件已交叉编译；未连接板卡、未运行 armv7l 验证。

本任务没有修改 F1/F2/M1/M2 的任何代码。

## broken_promise 的产生位置

源码基线为 `codes/gcc/libstdc++-v3/include/std/future`。

1. `1412–1427`：`_Task_setter::operator()` 先执行 `(*_M_result)->_M_set((*_M_fn)())`；强制展开被识别后只重抛，并在注释中明确后续将产生 broken promise。
2. `1777–1789`：`_Async_state_impl::_M_run` 的 forced-unwind handler 在 `_M_result` 非空时显式调用 `_M_break_promise(std::move(_M_result))`，然后重抛。
3. `465–481`：`_M_break_promise` 在传入 result 对象中设置 `future_errc::broken_promise`，把它交换进共享状态，将状态置 ready 并通知等待者。
4. `743–751`：`future::get` 的公共取值路径优先检查 `_M_error`；非空时重抛该异常，而不返回值。

这不是析构兜底。`_M_break_promise` 的通用注释描述“最后一个 provider 放弃状态”的契约，但本场景的具体调用者是 `_M_run` 的 forced-unwind catch，调用是显式且可定位的。

## 不使用线程取消的对照

对照探针没有调用 `pthread_cancel`。async callable 返回一个结果对象；该对象已经被移动构造进 libstdc++ 的 result storage 后，源临时对象析构时调用 `pthread_exit`，使工作线程通过另一种强制展开途径退出。与取消场景相同之处是二者都产生 `__forced_unwind`；不同之处是没有取消请求和取消 API。

实测输出：

```text
GET=FUTURE_ERROR CODE=4 BROKEN_PROMISE=1 VALUE_MOVE_CONSTRUCTED=1 SOURCE_DESTRUCTOR_ENTERED=1
```

退出码为 0。运行时实际加载 `/usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.33`，SHA256 为 `1fd75fe70354a416d75aef22bcae68c47bd25d20e2d0568c30b1a9838cf62f11`。

因此，`broken_promise` 不是取消 API 特有行为；它是 libstdc++ 对 async provider 因强制展开未正常完成的统一处置。该实验没有证明所有“非正常退出”都会如此，只证明 `pthread_exit` 这一独立强制展开来源会走相同分支。

## 已设置值的去向

探针的 `VALUE_MOVE_CONSTRUCTED=1` 证明值对象已移动构造进 result storage。源码 `249–283` 显示 `_Result::_M_set` 构造值并把 `_M_initialized` 设为 true。

随后 `_M_break_promise` 并未物理覆盖该对象的存储，而是在同一个 result 对象的 `_M_error` 字段写入 `broken_promise`，再把 result 发布到共享状态。`future::get` 优先检查 `_M_error`，因此已构造的值不再作为结果可见；result 最终销毁时仍按 `_M_initialized` 析构该值。准确表述是“值已构造，但在可观察结果层被错误状态取代，并最终正常析构”。

## 两种候选语义的事实后果

### ready 时 no-op

libc++ 当前在 `set_value` 内即设置 `constructed | ready` 并通知，而 async 的 `__execute` 尚未完全退出。因此 R59 的取消窗口到来时，审阅版接口看到 ready 后保持原值。

这与 libstdc++ 可观察行为不同：libstdc++ 返回 `broken_promise`，审阅版保留值。对普通值，开发者可能观察到“producer 被强制退出但 future 仍返回值”；R59 的特殊结果对象在 `future.get()` 后再次进入其析构控制窗口，表现为永久等待并被 timeout 终止。二者均是应用可观察差异。

### 窗口内转为 broken_promise

若要求对齐 libstdc++，不能只依赖 libc++ 现有 ready 位，因为它同时表示“值已写入”和“对等待者可见”，但不表示 async 执行帧已经完整退出。实现需要额外区分至少两个阶段：

- result storage 已构造或异常已写入；
- async 执行已完整完成，之后该结果才不可被 forced-unwind 处置替换。

可实现的机制类别包括在共享状态锁下增加 execution-complete 状态，或推迟 async 路径的 ready 发布。forced handler 若在中间窗口替换 typed value，还必须由知道 `_Rp` 的派生状态安全析构已构造值、清理 constructed 标志、写入异常并通知；基类接口本身无法析构未知类型的值。这意味着状态位、发布时机、typed/void 路径和并发等待者均需重新验证。这里仅陈述所需性质，不选择实现。

## 自行判断与尚存疑问

- 自行判断：用 `pthread_exit` 构造“不使用线程取消”的对照。它与取消共享强制展开机制，能够直接验证 libstdc++ 的 handler 是否按 API 来源区分；没有使用模拟器或修改运行库。
- 自行判断：将来源归为 A，但把“取消处理逻辑”收窄为“async forced-unwind handler”。源码没有检查取消 API 来源，所以直接称“pthread_cancel 特有分支”不准确。
- 尚存疑问：人工裁决需明确，产品目标是严格执行 ready/no-op 文本，还是严格对齐 libstdc++ 的 provider 未正常完成语义；两者已由实测证明不能同时满足。
- 尚存疑问：若选择对齐 libstdc++，是新增 execution-complete 状态，还是调整 async 的 ready 发布时机；本任务未实现或比较这两类方案。

## 未覆盖范围

- 对照实验仅在 x86_64 原生环境执行。
- 未验证 ARM EHABI 下 `pthread_exit` 对照。
- 未构造非强制展开的语言运行时异常导致 provider 退出；普通 C++ 异常由 task setter 捕获并作为普通异常结果发布，不走本分支。
- 未测试内存分配失败或 `make_exception_ptr` 失败路径。
