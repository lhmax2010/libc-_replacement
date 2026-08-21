# R55：修复 `std::async` 等待挂起问题

## 1. 结论

结论：`FIXED`。

两架构的修复前全补丁版均稳定挂起；修复后均在 20/20 次运行中唤醒等待方、退出 0，并由 `future.get()` 取得 `future_errc::broken_promise`，与 libstdc++ 参考侧逐格一致。正常返回和真实 C++ 异常经 future 传递均未改变。R51 回归矩阵与 libc++abi 相关上游测试全部通过。

本结论严格限定于 x86_64 原生宿主和 armv7l 真机；未覆盖范围见第 10 节。

## 2. 输入、基线与口径

- llvm-project 基线：见 `raw/upstream_baseline_commit.txt`。
- 层 A、层 B、原层 C 头文件批、层 C 库内批及修正版头文件批 SHA256：见 `raw/identity/baseline_and_patches.txt`。
- 修复对象：层 C 头文件批中的两个 `__async_assoc_state::__execute()` 强制展开 handler；不修改平台源码，不修改 libc++abi，不修改层 C 库内批。
- `async` 专项分母：每架构、每运行时组合 20 次，共 160 格。
- 设施矩阵分母：2 架构 × 7 设施 × 4 组合 = 56 格。本任务书称“7 × 3”，同时又要求修复前全补丁对照；为保留该必要对照，实际按 4 组合记录，未合并口径。
- R51 回归：每架构 18 格；libc++abi 相关测试：每架构 20 格。

资源检查 `medium` 退出 0；构建并行度为 2，未重建 libc++abi。资源原文见 `raw/resources/`。

## 3. 复现与运行时观测

### 3.1 x86_64 复现

R54 全补丁版复现退出 124。独立状态探针取得以下同一时刻事实：

- `pthread_cancel` 返回 0；
- 工作线程的 `/proc/self/task/<tid>` 已不存在，即工作线程已结束；
- `future.wait_for(0)` 返回 `timeout`，即共享状态未 ready；
- 原 R54 等待进程只剩等待线程，内核等待点为 futex。

直接附加调试器受宿主 `ptrace_scope` 限制，记为 `NOT_OBSERVED`；线程状态、task 消失与 future 状态均由无需附加的探针直接观测。原文见 `raw/x86_64/diagnosis/`。

### 3.2 工作线程与 pthread_join 口径

libc++ 的 `__make_async_assoc_state()` 以 `std::thread(...).detach()` 启动内部线程。该线程所有权不暴露给调用方，因此合法的 `pthread_join` 结果为 `NOT_AVAILABLE_STD_ASYNC_OWNS_DETACHED_THREAD`；本任务未对已 detach 的内部线程强行调用 `pthread_join`。线程是否结束由 `/proc` task 消失直接确认。

### 3.3 共享状态

修复前：工作线程结束，但 future 状态为 `timeout`。修复后：`wait()` 返回，`get()` 抛出错误码 4 的 `broken_promise`。因此“共享状态是否 ready”不是源码推断，而是 `wait_for`、`wait` 与 `get` 的直接观测。

## 4. 源码对照与根因

### 4.1 libc++ 修复前

两个 async 关联状态执行函数在普通异常时调用 `set_exception(current_exception())`，该函数会设置 `ready` 并通知条件变量；原层 C 强制展开分支却只执行 `throw;`。工作线程结束后，没有其他对象负责完成该 async 专用共享状态。

等待侧 `__sub_wait()` 在 `ready` 未置位时持续等待条件变量。因此直接挂起原因是：强制展开分支在重抛前没有完成共享状态。

### 4.2 libstdc++

libstdc++ 的 `_Async_state_impl::_M_run()` 在捕获 `__forced_unwind` 后：

1. 若结果对象仍存在，调用 `_M_break_promise()`；
2. 使共享状态 ready；
3. 再重抛强制展开。

实测参考侧 `future.get()` 得到错误码 4 的 `broken_promise`。源码原文见 `raw/source/libstdcxx_future.txt`。

### 4.3 两者差异

| 项目 | libc++ 修复前 | libstdc++ | libc++ 修复后 |
|---|---|---|---|
| 强制展开继续传播 | 是 | 是 | 是 |
| 重抛前完成共享状态 | 否 | 是 | 是 |
| 等待方被唤醒 | 否 | 是 | 是 |
| `future.get()` | 不返回 | `broken_promise` | `broken_promise` |

### 4.4 涉及的 9 处改动

直接涉及两处：

- `__async_assoc_state<_Rp, _Fp>::__execute()`；
- `__async_assoc_state<void, _Fp>::__execute()`。

其余 deferred、packaged_task、async 创建失败回退等 forced-unwind handler 不参与本次挂起。

## 5. `packaged_task` 对照的口径修正

R54 的 `packaged_task` 用例只执行取消与 `pthread_join`，没有取得或等待 future。因此它证明了取消传播不再 abort，但没有证明共享状态已经 ready。

本任务补测：当 `packaged_task` 对象仍存活时，取消后工作线程已结束，但 libc++ 全补丁版与 libstdc++ 的 future 均为 `timeout`。这不是本方案特有回归。`packaged_task` 的状态所有者生命周期与 `std::async` 的 detached async state 不同；本修复没有改变 packaged_task 路径。

## 6. 修复设计与实现

在两个 async 强制展开 handler 中：

1. 调用既有 `std::__throw_future_error(future_errc::broken_promise)` 产生标准 future 错误；
2. 由局部 handler 取得其 `exception_ptr` 并调用既有 `set_exception()`，设置 ready 并通知等待方；
3. 局部 handler 结束后，以外层 `throw;` 继续原强制展开。

该形态的理由：

- 对齐 libstdc++ 的可观测语义；
- 兼容旧语言模式下 `future_error` 构造函数的访问限制；
- 不增加 libc++ 导出符号或虚函数，不改变二进制 ABI；
- 仅增加两个 async 分支各 5 行逻辑；
- 不触碰普通异常、正常返回、deferred 或 packaged_task 路径。

修正版补丁：`patches/0001-Layer-C-header-preserve-forced-unwind-and-ready-async.patch`。补丁相对上游基线为 3 文件、57 行新增；相对 R52 原补丁仅增加上述 10 行。署名未由人工指定，暂记 `NOT_AVAILABLE <not-available@invalid>`，提交前须人工 amend。

## 7. `async` 专项结果

完整表见 `tables/async_specialty_20runs.tsv`。

| 架构 | 组合 | 20 次结果 | 等待返回 | `broken_promise` |
|---|---|---:|---:|---:|
| x86_64 | 未打补丁 | 6×0、14×134 | 16/20 | 0/20 |
| x86_64 | 修复前全补丁 | 20×124 | 0/20 | 0/20 |
| x86_64 | 修复后全补丁 | 20×0 | 20/20 | 20/20 |
| x86_64 | libstdc++ | 20×0 | 20/20 | 20/20 |
| armv7l | 未打补丁 | 20×134 | 20/20 | 0/20 |
| armv7l | 修复前全补丁 | 20×143 | 0/20 | 0/20 |
| armv7l | 修复后全补丁 | 20×0 | 20/20 | 20/20 |
| armv7l | libstdc++ | 20×0 | 20/20 | 20/20 |

基线在 x86_64 的 20 次压力重复中出现 134/0 波动，所有样本均打印 glibc 的强制展开未重抛错误；这不改变修复前全补丁稳定挂起与修复后稳定通过的对照结论。

## 8. 回归验证

### 8.1 设施矩阵

完整表见 `tables/facility_matrix.tsv`。除 async 外，修复前与修复后的退出码逐格相同。`packaged_task` 在两架构均保持 0。

本轮 x86_64 `cout` 参考格再次出现已知超时，而修复前/后全补丁均为 0；该设施不经过本次修改的 future 路径，且任务书明确排除该既知不稳定项，故不纳入 async 修复判据。

### 8.2 正常返回与真实异常

`tables/noncancel_async.tsv` 的 2 架构 × 2 场景 × 4 组合共 16 格全部退出 0：

- 正常 async 返回值保持 42；
- async 中抛出的 `std::runtime_error("async-real-error")` 仍经 future 原样传递。

### 8.3 R51 与 libc++abi 测试

`tables/regression_summary.tsv`：

- R51 回归：x86_64 18/18，armv7l 18/18；
- libc++abi forced-unwind/unwind 相关测试：x86_64 20/20，armv7l 20/20。

### 8.4 R54 future 并发子集

R54 的并发用例源码不包含 `<future>`、`std::async`、`packaged_task` 或任何 future 状态；可重跑的“与 future 相关部分”分母为 0，记 `NOT_AVAILABLE`，没有把无关的 pthread/cout 并发格冒充 future 回归。该事实见 `raw/source/r54_concurrency_probe.txt` 及交付的 `concurrency_probe.cpp`。

## 9. 板端操作与无效首轮

首轮部署把归档传到 `/var/tmp`，板端安全标签拒绝读取；连接工具仍返回 0，导致首轮所有单元实际为目标不存在。该轮完整保留在 `raw/armv7l/invalid_deploy/`，不参与任何计数。

有效重跑改用已验证的共享传输路径，并在执行前核验文件与运行库 SHA。两轮有效板端工作结束后均执行清理。最终只读核验：

- R55 远端目录名匹配数为 0；
- 相关进程数为 0；
- 共享传输归档不存在。

原文见 `commands/023_cleanup_board.log`、`commands/029_cleanup_board_regression.log`、`commands/031_board_final_readonly_verify.log`。

## 10. 未覆盖范围

- aarch64；
- LTO；
- 混合运行时帧穿越；
- 应用自建或第三方 future 实现；
- std::async 内部线程的 `pthread_join` 返回值：线程由 libc++ detach，不向调用方暴露，合法值不可得；
- R54 future 并发子集：原套件中不存在，分母为 0。

## 11. 自行判断与尚存疑问

本任务自行判断：

1. 将“7 设施 × 3 组合”扩展记录为 4 组合，以保留任务书另行要求的“修复前全补丁”对照；没有合并或删除任何格。
2. 未对 std::async 的 detached 内部线程强行执行 `pthread_join`，以 `/proc` task 消失证明线程结束，并明确记 join 为不可得。
3. 采用两个 async handler 内的局部 broken-promise 设置，不新增 out-of-line ABI；这是在“最小改动、更新头文件批、兼容旧语言模式”约束下的唯一直接对齐形态。
4. 不把 R54 无 future 内容的并发套件算作已覆盖；分母明确为 0。

尚存疑问：

- 人工作者 name/email 未提供，补丁署名须在对外提交前 amend。
- 是否需要新增专门的 future 并发回归套件不在本任务授权范围内，本任务没有自行扩展。

本任务未向 Gerrit 或任何外部源码仓推送，未修改平台源码或落章文档。
