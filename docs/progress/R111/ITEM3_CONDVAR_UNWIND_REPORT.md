# 解除条件变量异常规格后的展开核查

## 结论

本项为 x86_64 原生实测。对普通等待、system clock 定时等待、steady clock
定时等待、自定义 Clock 定时等待分别执行 5 次：

- 保留异常规格时，取消到达等待边界后 20/20 进入 `std::terminate`；线程内的
  `CleanupProbe` 20/20 未执行，进程由测试 terminate handler 以 86 退出。
- 移除相应异常规格并重编实现及头文件内联调用方后，20/20 强制展开穿过等待
  边界；`CleanupProbe` 20/20 执行，`pthread_join` 20/20 得到
  `PTHREAD_CANCELED`，进程 20/20 正常返回 0。

因此，在本次 x86_64、裸 `pthread_create`、延迟取消用例中，解除规格的目的已被
直接观察到：强制展开能穿过四种被测边界，沿途清理执行，线程可由创建方作为已
取消线程干净回收。这里的“干净”只指这三个被断言的行为，不外推到其他线程状态
或架构。

普通“锁未持有”错误且无人捕获时，两种构建各 10 次（自定义 terminate handler
5 次、默认 handler 5 次）最终都终止；两组的局部 `CleanupProbe` 都是 0/10。
默认 handler 下两组都是 SIGABRT（Python returncode `-6`），并给出相同的
`std::__1::system_error`、`mutex not locked`、`Operation not permitted` 诊断。
区别在终止位置：保留规格的回溯在 condition variable 的 non-throwing 边界经
`__clang_call_terminate` 进入终止；移除规格的回溯保留了抛出函数、无规格的
`condition_variable::wait` 和无人捕获异常处理帧，最终在调用链顶端终止。

所以，对于本次“无人捕获”的普通错误用例，解除规格没有改变最终进程终止、没有
使该局部清理执行，也没有改变默认诊断文字；观测到的变化是终止边界和到达终止
的异常传播路径。本文不据此作处置判断。

## 被测实现与隔离方式

保留规格组使用已有 R69 x86_64 patched runtime，头文件取 R68 baseline 安装树；
其头文件和 runtime 中的目标函数都仍带 non-throwing 规格。移除规格组采用同一
基础：

1. 自动生成 overlay，只移除 condition variable 目标声明/定义上的规格；生成器
   要求头文件恰好替换 6 处、源码恰好替换 2 处，否则失败。
2. 用 overlay 源码构建 `libcv_removed_r111.so`，为两个 out-of-line 实现提供移除
   规格后的定义。
3. 用 overlay 头文件重编移除组用例，使 steady clock 和自定义 Clock 两个头文件
   内联实现实际采用新规格；保留组则用原始头文件重编。
4. 移除组 ELF 的 `DT_NEEDED` 首项是 `libcv_removed_r111.so`，随后才是
   `libc++.so.1`；`ldd` 确认两者分别解析到本次 helper 和 R69 runtime。保留组只
   加载 R69 runtime。具体输出见 `raw/035_*`、`raw/036_*`。

overlay 与输入的完整 diff 在 `raw/042_*`：除 6 个头文件规格和 2 个源码规格外
无其他差异。当前 `codes/llvm` 两个目标源码文件 tracked diff 为 0，没有修改平台
源码或四个补丁。

## 用例形态与断言

取消用例 `code/condition_cancel_probe.cpp`：

- worker 是普通可抛函数，没有 `noexcept`；直接传给裸 `pthread_create`；
- 显式设置 deferred cancellation；调用链没有由用例添加的 `noexcept` 帧；
- worker 持 mutex 后发布 ready，主线程随后实际取得同一 mutex，证明 worker 已
  进入会释放 mutex 的等待段，再调用 `pthread_cancel`；
- RAII `CleanupProbe` 位于等待调用外层；测试同时断言等待已确认、是否进入
  terminate handler、清理标记、join 结果和值 `PTHREAD_CANCELED`，而不是只看
  退出码。

四种 mode 对应：

| mode | 调用形态 | 实现位置 |
|---|---|---|
| `wait` | `cv.wait(lock)` | out-of-line |
| `system_clock` | `cv.wait_until(lock, system_clock::now()+1h)` | system-clock out-of-line timed wait |
| `steady_clock` | `cv.wait_until(lock, steady_clock::now()+1h)` | 头文件内联 steady-clock timed wait |
| `custom_clock` | `cv.wait_until(lock, CustomClock::now()+3600s)` | 头文件内联 generic Clock timed wait |

普通错误用例 `code/uncaught_error_probe.cpp` 构造 `defer_lock` 的
`unique_lock<mutex>` 后直接调用 `condition_variable::wait`，没有 catch。每个构建
先用自定义 terminate handler 记录局部清理计数和回溯，再用默认 handler 记录真实
诊断和信号结果。取消组中同一个清理标记能稳定出现，是“普通无人捕获路径清理为
零”的正向仪器对照。

## 取消矩阵

完整逐次数据见 `tables/item3_cancel_matrix.tsv`。汇总如下：

| 构建 | 等待形态 | 次数 | 已确认进入等待 | terminate | 清理执行 | join 为 canceled | 最终退出 |
|---|---|---:|---:|---:|---:|---:|---|
| 保留规格 | wait | 5 | 5 | 5 | 0 | 0 | 5 次 86 |
| 保留规格 | system clock | 5 | 5 | 5 | 0 | 0 | 5 次 86 |
| 保留规格 | steady clock | 5 | 5 | 5 | 0 | 0 | 5 次 86 |
| 保留规格 | custom Clock | 5 | 5 | 5 | 0 | 0 | 5 次 86 |
| 移除规格 | wait | 5 | 5 | 0 | 5 | 5 | 5 次 0 |
| 移除规格 | system clock | 5 | 5 | 0 | 5 | 5 | 5 次 0 |
| 移除规格 | steady clock | 5 | 5 | 0 | 5 | 5 | 5 次 0 |
| 移除规格 | custom Clock | 5 | 5 | 0 | 5 | 5 | 5 次 0 |

没有一格出现等待函数意外普通返回。矩阵程序的行为断言全部通过。

## 普通错误无人捕获矩阵

完整逐次数据见 `tables/item3_uncaught_error_matrix.tsv`。

| 构建 | handler | 次数 | 结果 | 局部清理 | 诊断/回溯 |
|---|---|---:|---|---:|---|
| 保留规格 | instrumented | 5 | 5 次退出 86 | 0/5 | 5/5 出现已解析为 `__clang_call_terminate` 的固定 runtime 帧 |
| 移除规格 | instrumented | 5 | 5 次退出 86 | 0/5 | 5/5 出现无规格 helper 的 `condition_variable::wait` 帧及 uncaught-exception 帧 |
| 保留规格 | default | 5 | 5 次 SIGABRT (`-6`) | 0/5 | 5/5 system_error + mutex-not-locked 诊断 |
| 移除规格 | default | 5 | 5 次 SIGABRT (`-6`) | 0/5 | 5/5 同文字诊断 |

代表性原始回溯和诊断见 `raw/041_*`。地址离线解析见 `raw/038_*`：

- 保留规格的 `libc++.so.1(+0x507fb)` 解析为 `__clang_call_terminate`，下一帧是
  `condition_variable::wait`；
- 移除规格的 helper 帧解析到
  `condition_variable_removed.cpp:36`；它之前的两个 R69 libc++ 帧解析为
  `__throw_system_error`，libc++abi 中还多出无人捕获异常处理帧。

C++ 异常机制在搜索处理器的第一阶段确定没有 catch 后可直接终止，不保证先进行
第二阶段栈展开；本次两组的局部清理均未执行是实测事实。不能把取消路径中清理
会执行外推为“所有无人捕获普通异常也会清理”。

## 架构、输入与脚本快照

- 架构：x86_64 原生，kernel `6.17.0-1032-oem`；不是 QEMU。
- 编译器：Clang `22.1.8`，target `x86_64-tizen-linux-gnu`。
- baseline header SHA256：
  `51687768e13c70a26ecd13cd064c413e8eed8c39e8c43d0de1cecf6a7b519858`。
- R69 libc++ SHA256：
  `b2d8d0ee8d8e06feba4829507d66b8c75aa6d9c6c9bc042278ec7da1e528de0f`。
- R69 libc++abi SHA256：
  `aaea4e9d9320d2295176bbc054ded398c485a17a4a4a8144298d06ec76fb95da`。
- 本次 helper SHA256：
  `ace205eed4edde74f1c2d88d562f543634cf8c34d6972878f2e043c8d6bebc68`。

脚本 SHA256 均记录在 `raw/042_*`：

- `make_removed_overlay.py`：`fd1f3bf4...b1f09`
- `condition_cancel_probe.cpp`：`b3378de5...169e`
- `uncaught_error_probe.cpp`：`685200c6...3933`
- `build_item3.sh`：`bde802d2...b30f`
- `run_item3_matrix.py`：`b1a92d4e...ee31`
- `run_logged.sh`：`df21b606...b455`

## 限制和异常记录

- armv7l 与 aarch64 未执行，记 `NOT_OBSERVED`。本项强制范围是 x86_64 原生；
  可选架构没有物理板，本次未把 QEMU 用户态结果加入结论。
- 本次验证实际 libc++/glibc/pthread 组合及四种调用形态；不外推到不同 runtime
  版本、异步取消或调用链另含 non-throwing 帧的情况。
- `raw/035_*` 的最后一条 symbol 检查曾使用不存在的短路径
  `codes/R69/patched/lib/libc++.so.1.0`；同一记录前面的 `ldd` 已给出实际路径，随后
  `raw/036_*` 对实际 R69 路径成功复核。没有用该失败子命令支持结论。
- `raw/040_*` 的代表文件循环因 shell quoting 把 `$f` 保留为字面量而退出 1；其
  前半部分汇总正确，但代表性证据改由 `raw/041_*` 成功采集。失败原文保留。

本项没有修改平台源码或补丁，没有实施产品方案，也不作处置建议。
