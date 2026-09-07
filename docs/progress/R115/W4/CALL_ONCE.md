# call_once 与解除异常规格的关系

结论：**静态核查，本轮回滚名单不包含 call_once**。它自身的等待链不经过本次改动的入口。

核查源码版本：LLVM `5ed6c77278dfa7a470667cf1a137723d3c96fe60`。
完整函数与辅助函数原文见 [原始记录](raw/002_static_inputs.stdout)，文件 SHA256、检索及正向对照见
[边界检索](raw/012_call_once_boundary_search.stdout)。以下行号均指该版本 `codes/llvm/libcxx/`。

| 调用层 | 文件与位置 | 已核实的动作 |
|---|---|---|
| call_once 模板 | include/__mutex/once_flag.h:135；旧语言分支 :145、:153 | 调用 std::__call_once |
| 库实现 | src/call_once.cpp:45–47 | 原生全局 mut 加锁；标志为 Pending 时调用 __libcpp_condvar_wait(&cv, &mut) |
| POSIX 线程辅助函数 | include/__thread/support/pthread.h:120–122 | 直接 return pthread_cond_wait(__cv, __m) |

本次解除的边界是 `condition_variable::wait` 及 system_clock、steady_clock、自定义 Clock 的
`condition_variable::__do_timed_wait`。上述等待调用链没有调用它们，也没有一个“本次将解除”的异常规格。
`_LIBCPP_HIDE_FROM_ABI` 是 ABI 可见性相关宏，不是这里待解除的 `noexcept` 标注。
零命中检索另以 condition_variable.cpp、shared_mutex.cpp 的真实入口/调用作正向对照；结论主要来自逐层函数体核查，而非仅凭词法零命中。

**影响关系（由上述静态调用链得出）**：只解除上述四个入口的规格，不改变 call_once 自身等待路径。
因此它不是“解除后新暴露”的回滚点；本轮没有为它设计或实施守卫。

**单列的既有风险**：等待时持有原生全局 mutex，等待外围没有解锁守卫。
源码中恢复 once_flag 的异常守卫在 :49 才创建，属于执行初始化函数的分支，并不覆盖 :47 的等待者。
取消该等待者是否实际导致全局锁残留，本轮运行结果为 `NOT_OBSERVED`；没有以静态风险冒充复现实测。
若存在该残留，它在解除前后都是既有问题，而非决策一引入。

限定：此处指 call_once **自身等待者路径**。用户初始化回调可以自行调用条件变量；任意用户回调的调用图不能据此判定。
