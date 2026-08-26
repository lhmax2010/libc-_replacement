# R71：等待方被取消场景中两套标准库差异来源的澄清

## 1. 范围与结论

本任务未修改平台源码、产品补丁或产品代码，只复用了 R69 的测试件并增加了 GDB 批处理观测脚本。

判定为**可能性 B**：参照实现的实际等待路径上不存在 `__forced_unwind` 防护，差异来自两套标准库对 `std::async` 共享状态等待的结构选择，以及该结构所经过函数的异常规格不同。

- libstdc++ 在共享状态的 `wait()` 中先执行 `_M_complete_async()`，本格因此阻塞在 `std::thread::join()` / `pthread_join()`；该 C++ 调用链没有 `noexcept` 边界，强制展开穿过后等待线程以 `PTHREAD_CANCELED` 结束。
- libc++ 在共享状态的 `wait()` 中阻塞于内部普通 `condition_variable`；`condition_variable::wait` 在声明和定义处均为 `noexcept`。取消从 `pthread_cond_wait()` 发起强制展开，越过该函数时进入 `std::terminate`，最终 abort。

因此，本格不是此前 `__forced_unwind` catch 模式识别漏掉了一处 handler；它暴露的是**防护子句之外的取消兼容性差异**。

## 2. 测试件与观测方法

测试件沿用 `progress/R69/src/r69_new_cases.cpp`，等待线程在第 131 行调用 `waiting_future->wait()`；异步工作线程在主线程释放前保持未完成，确保等待线程确实阻塞。

观测脚本 `progress/R71/code/gdb_waiter_trace.gdb` 在以下位置取所有线程栈：

1. `pthread_cancel` 调用前；
2. `_Unwind_ForcedUnwind` 开始时；
3. `__libc_fatal`（如到达）；
4. inferior 停止或退出时。

GDB 对 `_Unwind_ForcedUnwind` 的两次命中分别对应 glibc 转发层与 libgcc 实现层，是同一次取消的连续路径，不表示发起了两次取消。

二进制与运行库身份见 `commands/017_runtime_identity.log`。基线 libc++ 测试件 SHA256 为 `de34eae46b1f9971015c0b60ecd5ae7fa0342ea785028c7dea33ce6075fe58ad`，参照测试件 SHA256 为 `1b3f1732e65200cc0b73a86a56ef8b4f56a17642f430ddb49fd37b1112d387fe`。

## 3. 源码调用路径逐层对照

完整逐层表见 `tables/call_path_comparison.tsv`。
源码仓版本与逐文件 SHA256 见 `tables/source_identity.tsv`。

### 3.1 libc++

调用序列：

```text
waiting_side (测试件:131)
  -> shared_future<int>::wait (libcxx/include/future:1915)
  -> __assoc_sub_state::wait (libcxx/src/future.cpp:108-111)
  -> __assoc_sub_state::__sub_wait (future.cpp:113-123)
  -> condition_variable::wait (condition_variable.cpp:33-39, noexcept)
  -> __libcpp_condvar_wait (pthread.h:120-123)
  -> pthread_cond_wait
```

`__assoc_sub_state` 在 `libcxx/include/future:523-528` 中持有普通 `condition_variable __cv_`。上述 C++ 层均无 catch；`__assoc_sub_state::wait` 中的 `unique_lock` 是作用域清理构造。决定性异常边界是 `condition_variable::wait`：头文件第 107 行声明为 `_NOEXCEPT`，源文件第 33 行定义为 `noexcept`。

### 3.2 libstdc++

本格的实际调用序列：

```text
waiting_side (测试件:131)
  -> __basic_future<int>::wait (libstdc++-v3/include/std/future:719-724)
  -> _State_baseV2::wait (future:353-361)
  -> _M_complete_async (future:1742)
  -> _M_join (future:1744)
  -> call_once (include/std/mutex:902-918)
  -> thread::join (src/c++11/thread.cc:128-140)
  -> __gthread_join (libgcc/gthr-posix.h:679-683)
  -> pthread_join
```

这些 C++ 函数均未标 `noexcept`，也无 catch。`call_once` 中存在 `_Prepare_execution` RAII 对象，用于清理其线程局部 trampoline 状态；它不捕获或吞掉 forced unwind。

libstdc++ 的 `future` 确实另有三处 `__forced_unwind` handler（`future:1418-1421`、`1441-1444`、`1783-1789`），但都位于工作任务求值/工作线程完成路径，不在本格的等待方路径。故不能把本格成功归因于这些 handler。

### 3.3 普通 condition_variable 的同类上游事实

当前 libstdc++ 的普通 `condition_variable::wait` 也没有 forced-unwind catch，但其声明与定义已不是 `noexcept`（`include/std/condition_variable:97-98`、`src/c++11/condition_variable.cc:38-42`）。GCC 提交 `9e18a25331fa25c3907249fede65a02c6817b06e`（PR103382）明确说明旧的 `noexcept` 会令 NPTL 取消的 `__forced_unwind` 终止进程，并为 GCC 12.1 移除了该标注；完整提交与 diff 见 `commands/016_gcc_pr103382_commit.log`。

这项事实与本次 libc++ 失败机制同类，但参照测试件本格在 `_M_complete_async()` 的 join 阶段已经阻塞，运行时没有走到普通 condition_variable。

## 4. 运行时对照与首处分歧

脱离调试器的直接复现见 `commands/019_direct_runtime.log`：libc++ 基线打印 `terminating due to uncaught foreign exception` 并退出 134；libstdc++ 参照打印 `JOIN_CANCELED=1 VALUE=42` 并退出 0。

### 4.1 libstdc++ 参照

- 取消前，等待线程栈为 `pthread_join -> std::thread::join -> call_once -> _M_join -> _M_complete_async -> _State_baseV2::wait -> __basic_future::wait -> waiting_side`（`raw/x86_64/gdb_reference.log:22-40`）。
- forced unwind 从 `pthread_join` 的取消点开始并沿上述非 `noexcept` 栈回退（同文件 `:81-110`、`:154-183`）。
- 最终输出 `CANCEL_RC=0 JOIN_RC=0 JOIN_CANCELED=1 VALUE=42`，inferior 正常退出（同文件 `:229-230`）。

### 4.2 libc++ 基线

- 取消前，等待线程栈为 `pthread_cond_wait -> condition_variable::wait -> __sub_wait -> __assoc_sub_state::wait -> shared_future::wait -> waiting_side`（`raw/x86_64/gdb_libcxx_baseline.log:16-32`）。
- forced unwind 已从 `pthread_cond_wait` 开始（同文件 `:55-77`、`:102-127`），因此不是“取消未投递”或“展开未开始”。
- 首个造成不同结局的 C++ 边界是 `condition_variable::wait noexcept`。终止栈直接显示 `std::terminate -> __clang_call_terminate -> condition_variable::wait`（同文件 `:149-177`）。

### 4.3 差异来源定性

从 ISO C++ 口径看，这是实现选择：标准要求 async 关联线程的完成与等待满足“as if joined”等效果，但不强制采用 join 或 condition_variable；`condition_variable::wait` 的规范签名没有 `noexcept`，同时写有 `Throws: Nothing`。libc++ 的 noexcept policy 允许把这类函数标为 `noexcept`。POSIX `pthread_cancel` 与 NPTL forced unwind 不属于 ISO C++ 的标准异常语义。

从本项目要求的 glibc/Tizen pthread cancellation 兼容口径看，这是 libc++ 的既有兼容性缺口：强制展开已正常发起，却被库内 `noexcept condition_variable::wait` 终止。GCC PR103382 对普通 condition_variable 的同一机制明确称旧标注不正确并已修复。这里的“缺口”不表示本任务对标准符合性作出否定判断。

## 5. 上游线索

检索表见 `research/upstream_search.tsv`。

- GCC：命中 PR103382，状态为已提交；其回归测试位于 `libstdc++-v3/testsuite/30_threads/condition_variable/members/103382.cc`。
- LLVM：在 llvm-project issue 与 reviews.llvm.org 中，以 `libc++ condition_variable wait noexcept pthread_cancel`、`libc++ pthread_cancel future wait forced unwind` 等组合检索，未找到直接讨论本场景的 issue/评审，记 `NOT_FOUND`。
- 命中的 LLVM noexcept policy 是设计文档，不是该问题的处置记录。

## 6. 对“50 处 + 8 处”识别方法的影响

此前数字仍可作为“显式 catch/forced-unwind 防护站点”的清单与计数；本次没有发现第 51 处 catch 站点，也没有把普通 condition_variable 重新归入那 8 处。

但该口径不能再解释为“标准库内所有取消兼容性影响面”：本次失败不依赖 catch-all，而由**取消点可达路径 + 异常规格 + 不同阻塞原语/控制流**共同形成。因此，需要重新审视的是识别方法的覆盖边界，而不是在无新站点证据时直接改写 50/8 的数字。

这类差异可部分机械检索：

1. 建立 POSIX 取消点及其包装函数种子（如 `pthread_cond_wait`、`pthread_join`、read/write 等）；
2. 构造目标配置下的跨 TU 调用图；
3. 查找从标准库公开/共享状态入口到取消点的路径；
4. 标记路径上 `noexcept`/`_NOEXCEPT` 边界及 catch/cleanup；
5. 对两套标准库比较底层原语和异常规格。

仅靠文本 grep 不足以完备发现：模板实例化、虚调用、平台条件编译、符号版本、优化与外部 glibc 路径都会改变实际调用图。本次 `commands/018_mechanical_scan_seed.log` 只记录可作为种子的直接命中，不把它冒充完整扫描结果；动态栈仍是必要的校验。

结论：**此前“50 处 + 8 处”的站点清单本身没有被本次证据推翻，但若其被用作完整取消影响面的边界，则必须重新审视。**

## 7. 自行判断、疑问与缺口

- 自行判断：把“标准允许”与“本项目 POSIX 取消兼容性缺口”分开表述，避免把非标准取消语义直接上升为 ISO C++ 符合性结论。
- 自行判断：未连接开发板；R70 已表明两架构现象一致，而本任务要求以 x86_64 为主，且 x86_64 的源码与运行栈已足以区分 A/B。
- 尚存疑问：未找到 LLVM 对该精确场景的公开 issue，故无法给出 libc++ 维护者对 pthread cancellation 支持边界的直接表态，记 `NOT_FOUND`。
- 未观测：没有对所有标准库公开 API 执行新的全量调用图扫描；本任务只评估了可机械发现的方法与局限。

## 8. 过程异常申报

两次早期只读检索范围写得过宽并被人工终止：一次误扫 `tmp/R69`，一次执行了耗时的全历史 `git log --all -S`。终止记录保留在 `commands/006_atomic_futex_precise.log` 与 `commands/013_history_blame.log`。二者均未修改源码、补丁或测试结论；后续改用精确路径与精确提交查询。
