# W1 / R61：M2 归因、探针修正与实现

## 结论

`M2_FIXED`。

- 调用栈坐实 R60 的 124 超时来自夹具残骸在主线程析构时自旋，不是“ready 时无操作”的语义后果。
- 修正夹具后，R60 旧实现的同一格为 `get()` 取得值、退出 0。
- 新实现把 async 结果的构造和 ready 发布拆开；强制展开命中“已构造、未发布”窗口时，销毁暂存值、清除 constructed 标记、发布 `broken_promise`、通知等待者，并重抛原始强制展开。
- x86_64 定向、补充与既有回归均通过；armv7l 留给 W2。

## 1. 挂起归因

原夹具 `AsyncWindowResult(AsyncWindowResult&&)` 会执行 `other.trigger = true`。`future::get()` 从共享状态移动结果时，这会把共享状态中的残骸重新武装。

调试器现场显示工作线程已经退出，主线程位于：

```text
future::get
  -> unique_ptr<__shared_count>::~unique_ptr
  -> __async_assoc_state::__on_zero_shared
  -> __assoc_state<AsyncWindowResult>::__on_zero_shared
  -> AsyncWindowResult::~AsyncWindowResult
  -> r59_probe.cpp:169 的无限循环
```

完整原文：`code/raw/004_gdb_hang_stack.log`。

## 2. 探针修正

修正内容：

1. 以 `m2_window_entered` 单独标记窗口；主线程负责调用 `pthread_cancel`。
2. 工作线程在 callable 中启用异步取消，结果析构只充当一次性窗口哨兵。
3. 移动构造生成的共享状态对象 `hold_worker=false`，且不再修改源对象，避免后续移动重新武装残骸。
4. 所有 `get()` 结果输出显式刷新。
5. 哨兵析构保持 `noexcept(false)`；首次误写为 `noexcept` 的运行退出 86，原始记录保留在 `code/raw/014_run_fixed_m2_once.log`，修正后再执行正式矩阵。

旧实现重跑结果为 `M2_FUTURE_GET=VALUE`、退出 0，见 `code/raw/007_run_corrected_noop.log`。

## 3. 实现

只改实验副本的 `libcxx/include/future`：

- `__assoc_state<_Rp>::__construct_value_unready`：按 `set_value_at_thread_exit` 的持锁模式构造值，只设置 `__constructed`，不设置 `ready`。
- `__async_assoc_state<_Rp, _Fp>::__execute`：先调用上述构造接口，再调用 `__make_ready()`；返回值临时对象的析构位于两个完整表达式之间。
- `__assoc_state<_Rp>::__complete_forced_unwind`：持锁处理未发布状态；若值已构造则销毁并清除标记，再写入 `broken_promise`、设置 `ready`、通知等待者。
- 非 void async 的 forced-unwind 分支使用派生类型可见的完成接口；void 路径保持既有处理。

更新后的层 C 头文件补丁：`code/patches/0001-Layer-C-header-preserve-forced-unwind-and-async-publication.patch`。

## 4. x86_64 验证

### M2 与补充矩阵

口径：每个“修复版/参考版 × 场景”运行 20 次；分母均为 20。结果：

| 场景 | 修复版 | libstdc++ 参考 | 判据 |
|---|---:|---:|---|
| 预发布窗口取消 | 20/20 | 20/20 | `broken_promise`，退出 0 |
| 4 个并发等待者 | 20/20 | 20/20 | 4/4 均收到 `broken_promise` |
| `wait_for` | 20/20 | 20/20 | 首次 timeout，取消后 ready，随后 `broken_promise` |
| void 特化 | 20/20 | 20/20 | `broken_promise`，退出 0 |
| 普通异常 | 20/20 | 20/20 | `runtime_error` 类型与消息一致 |

未打补丁基线的定向格取得原值并由测试返回 2；这是该基线的实际观测，不据此推断其他窗口。

### F1 / F2 / M1

口径：每个“修复版/参考版 × 场景”运行 20 次；分母均为 20。

| 场景 | 修复版 | libstdc++ 参考 | 关键断言 |
|---|---:|---:|---|
| F1 | 20/20 | 20/20 | 外来异常到达顶层，badbit 已提交 |
| F2 | 20/20 | 20/20 | join 为 `PTHREAD_CANCELED`，badbit/failbit 已提交 |
| M1 | 20/20 | 20/20 | callable 调用 2 次，第二等待者取得 42 |

### 设施、R51 与上游测试

- 设施矩阵：`getline` 自由函数、成员 `getline`、`read`、算术输入、`packaged_task`、`async` 的修复版均退出 0，并与参考侧一致。
- `cout` 修复版与参考侧均在进程退出阶段超时 124，属于既已登记的共同夹具现象；取消和 join 已先成功，不计为本次新增回归。
- R51 九类回归格：修复版 9/9、参考版 9/9 退出 0。
- `forced_unwind{1..4}` 与 `unwind_0{1..6}`：修复版 10/10、参考版 10/10 退出 0。

## 5. 构建与身份

- 实验提交：`6cfe8835795d0ea791d68960a721a5b35737b267`。
- x86_64 `libc++.so.1` SHA256：`ca817cc270fdf877c9103ecf098641cbeb10b61df2e90334784a821444e602c4`。
- x86_64 `libc++abi.so.1` SHA256：`30b8d9f8f96b3684657c4c806d2421eed1bbd912cd4637a1d62f17624201f4ed`。
- `LD_DEBUG=libs` 证明测试加载 `tmp/W1/build-x86_64/lib/libc++.so.1` 与同目录 `libc++abi.so.1`，原文见 `code/raw/028_identity.log`。
- 构建成功；被改头文件未产生诊断。日志中的新增可见 warning 仅为既有工具链参数 `--rtlib=libgcc` 在编译阶段未使用。

## 6. 已验证与未覆盖

已验证：x86_64 原生、libgcc_s 展开器、动态 libc++、无 LTO；单等待者、四等待者、`wait_for`、void、普通异常及既有回归。

未覆盖：armv7l（由 W2 执行）、aarch64、LTO、混合运行时帧穿越、动态加载边界、分配失败和结果析构自身抛异常的路径。

本任务没有需要人工裁决的遗留疑问。
