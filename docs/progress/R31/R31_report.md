# R31：pthread_cancel 在 libc++ 下 abort 的根因诊断（x86_64 原生）

## 结论

本段结论为 `ROOT_CAUSE_CONFIRMED_X86_64_ITANIUM`。

R30 的 x86_64 用例确为宿主 x86_64 原生执行，没有经过 QEMU。其最初
用例使用裸 `pthread_create`，线程入口不是 `noexcept`，使用默认的延迟
取消，并在 `pthread_testcancel()` 取消点外包了 `catch(...)`。

abort 的直接原因是：glibc 发起的 forced unwind 被 `catch(...)` 实际
捕获，catch 块正常结束而没有重抛；随后 libc++abi 的
`__cxa_end_catch` 触发 foreign exception 的 cleanup，glibc
`nptl/unwind.c:110-114` 的 `unwind_cleanup` 以
`FATAL: exception not rethrown` 调用 `__libc_fatal`，进程收到
SIGABRT。GDB 回溯中未经过 `std::terminate`，测试安装的 terminate
handler 也没有输出。

该直接行为不是 libc++ 独有：受控矩阵中，非 `noexcept` 的裸 pthread
加 `catch(...)` 在 libstdc++ 与 libc++ 下均退出 134；两侧无
try/catch 的裸线程均析构一次、join 得到 `PTHREAD_CANCELED`、退出 0；
GNU 侧显式捕获并重抛 `__forced_unwind` 也退出 0。因而本次 abort 不是
QEMU、`noexcept`、`std::thread` 包装器或取消机制自身造成，而是
catch-all 吞下强制展开造成。

libc++abi personality 的直接观测与任务背景引用的旧注释相反：
`actions=10`（`_UA_CLEANUP_PHASE | _UA_FORCE_UNWIND`）时，测试所载入的
`libc++abi.so.1` 对 catch-all 帧返回 7（`_URC_INSTALL_CONTEXT`），
随后程序打印 `EVENT catch_all`；无 catch 对照先为析构 cleanup 返回 7，
再返回 8（`_URC_CONTINUE_UNWIND`）并正常取消。当前 22.1.8 源码
`cxa_personality.cpp:806-818` 也明确把 forced unwind 的 catch-all
记为 handler。旧的 type-3 总体注释仍写“forced 时忽略 handler”，但
2021 年提交 `cfe9ccbddd98b55e49e46bb40877ece6a47a7625` 已有意把 type-3
改为和 type-2/libsupc++ 相同；当前注释与实现不一致。

因此，R30 的语义红停没有被本段推翻：仅在 libc++ 下删除 GNU
`__forced_unwind` 防护子句，会留下仍可捕获 forced unwind 的
`catch(...)`，候选补丁仍不可提交。本任务不设计修法，也未改动候选
补丁。

## 1. R30 执行方式与原用例核实

完整原用例保存为 `inputs/r30_initial_pthread_cancel_probe.cpp`，其来源为
R30 临时 fixture 提交
`9661e4639a96dc3a58022025bc33a2c9d1c6f08b`，SHA256 为
`7b9c41e4a2fa29b8656d5c6978507e7489895f0e2ec12c32740be4f29555384c`。
原编译与执行命令、fixture 全历史及完整源码见
`commands/002_r30_fixture_history_and_shape.log`。

R30 x86_64 的实际执行命令是目标根
`/lib64/ld-linux-x86-64.so.2 --library-path <root>/usr/lib64:<root>/lib64
<binary>`；宿主 `uname -m` 为 `x86_64`，命令中无 qemu。因此准确口径
是 `HOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU`，而不是宿主发行版
用户态，也不是模拟执行。

原用例形态逐项核实如下：

- 入口为 `static void *worker(void *)`，不是 `noexcept`；调用链中没有
  显式 `noexcept` 函数；
- 使用裸 `pthread_create`，没有 `std::thread`；
- 未调用 `pthread_setcanceltype`，所以采用默认 deferred cancellation；
- 取消点为循环内的 `pthread_testcancel()`，另有 `usleep(1000)`；
- `catch(...)` 仅增加原子计数并 `puts`，没有显式抛出；
- 因此其自身形态中没有“forced unwind 穿过 noexcept 帧”这一解释。

结构化申报见 `tables/r30_execution_and_shape.tsv`。

## 2. 线程包装器源码核查

当前 libc++ 的 `__thread_proxy` 只是解包 tuple、调用线程函数并返回，
未出现 `__forced_unwind`、catch 或 `noexcept`。平台对应 GCC 14.2.0
源码的 `execute_native_thread_routine` 同样只调用 `_M_run()` 后返回。

任务背景所述“libstdc++ `thread.cc` 有 forced-unwind 专门处理”是历史
状态：GCC 提交
`754d67d5ba4a1f9994210d402893a4cf49ce6a71` 在 2017 年已删除该
try/catch。完整当前源码、历史 diff、两棵源码身份见
`commands/021_thread_wrapper_source_comparison.log`。受控矩阵中两侧
`std::thread + catch(...)` 也均进入 catch-all 后以同一 fatal 文本
退出 134。

## 3. x86_64 原生受控实验

完整源码为 `src/cancel_matrix.cpp`，构建 spec 为
`inputs/r31-cancel-matrix.spec`。两套构建都使用平台
`x86_64-tizen-linux-gnu-clang++`、`-O0 -g -fno-omit-frame-pointer`，并在
各自全新 GBS root 中完成：

- libstdc++：`commands/007_build_matrix_libstdcxx_x86_64.full.log`；
- libc++：`commands/008_build_matrix_libcxx_x86_64.full.log`。

运行全部为宿主 x86_64 内核/ISA原生执行，使用各自目标 root 的动态
加载器与库路径；完整逐格命令、输出、退出码和信号见
`results/matrix/*.log`，汇总见 `tables/native_matrix.tsv`。

| 格 | libstdc++ | libc++ |
|---|---|---|
| 裸 pthread、非 noexcept、catch-all | catch 命中，fatal，134/SIGABRT | catch 命中，fatal，134/SIGABRT |
| forced-unwind 防护后重抛 | 析构 1、canceled 1、退出 0 | N/A：该类型不可用，退出 77 |
| 裸 pthread、noexcept、catch-all | catch 命中，fatal，134/SIGABRT | catch 命中，fatal，134/SIGABRT |
| std::thread、catch-all | catch 命中，fatal，134/SIGABRT | catch 命中，fatal，134/SIGABRT |
| 裸 pthread、无 catch | 析构 1、canceled 1、退出 0 | 析构 1、canceled 1、退出 0 |

基准格已是非 `noexcept`，仍复现 abort；无 catch 对照又证明取消机制
本身可正常完成。`noexcept + catch-all` 格在 catch-all 结束时已走同一
glibc fatal 路径，故本实验没有观测到 forced unwind 穿过一个
`noexcept` 且没有 catch-all 时的独立行为；该未覆盖项不影响基准格
根因判定。

## 4. abort 崩溃点

宿主原始 core limit 为 0。每次 GDB 运行只在其子 shell 内执行
`ulimit -c unlimited`，未修改系统配置；实际采用 GDB 直接启动目标
动态加载器，不依赖持久 core 文件。

libc++ 基准格、`noexcept + catch-all` 与 `std::thread + catch-all`
三次回溯均为：`raise -> abort -> __libc_fatal ->` glibc stripped local
frame `-> __cxa_end_catch`。终止前文本均为
`FATAL: exception not rethrown`，没有 `TERMINATE_HANDLER` 输出。GNU
基准格也走 `__libc_fatal`。原文分别见 `commands/013_*`、`014_*`、
`022_*`、`023_*`，汇总见 `tables/backtrace_summary.tsv`。

复用的 glibc 2.40 源码输入与目标 glibc RPM 版本均已重新核对：
`nptl/unwind.c:110-114` 明确说明“C++ catch block 没有重抛”时 abort；
`nptl/unwind.c:125-130` 把 exception class 设为 0、cleanup 设为
`unwind_cleanup`，再调用 `_Unwind_ForcedUnwind`。完整原文、SHA256、
命令与退出码见 `commands/020_glibc_cancellation_fatal_source.log`。

调试器同时报告目标 `libthread_db` 与宿主 GDB 不匹配，目标运行库也已
strip，因此部分 glibc 本地帧只能显示地址，`__cxa_end_catch` 以上的
栈在 unwind object 已进入 cleanup 后也不再完整。报告不对这些无符号
帧补名；fatal 文本、`__libc_fatal`、`__cxa_end_catch` 以及同版本
glibc 源码中的唯一对应 cleanup 是本段实际可得的定位边界。

所以直接 abort 不经 `std::terminate`，不是 personality routine 主动
调用 terminate，而是 catch 正常结束时 `__cxa_end_catch` 处置 foreign
forced-unwind object，调用 glibc 登记的 cleanup 后 fatal。

## 5. personality 直接观测

GDB Python 观测器为 `tools/personality_trace.py`。它在实际动态符号
`__gxx_personality_v0` 入口读取 x86_64 SysV ABI 的 `actions` 参数，并
在返回点读取 `rax`。GDB 同时申报符号实际来自测试 root 的
`/usr/lib64/libc++abi.so.1`。

结果见 `tables/personality_observations.tsv`：

- catch-all：`actions=10`、force=YES、返回 7，然后
  `EVENT catch_all`，最后 fatal；
- 无 catch：第一次 `actions=10` 返回 7 并执行 destructor；第二次
  `actions=10` 返回 8，随后 join 成功、进程退出 0。

这直接回答了本任务问题：本次载入的 libc++abi 并未在
`_UA_FORCE_UNWIND` 下跳过 catch-all；它为该 landing pad 返回了
`_URC_INSTALL_CONTEXT`。原始观测见
`commands/016_personality_trace_libcxx_catchall-r2.log` 与
`commands/016_personality_trace_libcxx_no-catch.log`。

源码层面，当前 `cxa_personality.cpp:642-644` 仍保留“forced 时忽略
handler”的旧 type-3 注释，但 `:806-818` 的现实现明确称 catch-all
覆盖 forced unwind 并返回 handler found，外层 `:1100-1113` 随后安装
context。`commands/019_personality_history_and_refactor.log` 证明 2021
年的 `cfe9ccb...` 是该语义变化的来源，且它是 `upstream/22.1.8` 的
祖先；该提交说明 type-3 原逻辑“conceptually wrong”，改为与
type-2/libsupc++ 一致。

## 6. 结论边界与待板上复核

本段实测只覆盖 x86_64、Itanium/DWARF、平台 glibc 2.40、libgcc_s
unwinder，以及平台 libc++abi 22.1.8 / libstdc++ 14.2.0。可以据此排除
R30 x86_64 结果中的 QEMU 与 ARM EHABI 变量。

以下仍为 `NOT_OBSERVED`，不得由本段外推：

- armv7l 开发板原生执行时，EHABI personality 是否以同一方式让
  catch-all landing pad 运行；
- armv7l 板上 glibc/libgcc_s 的最终 fatal 回溯是否与 x86_64 相同；
- forced unwind 穿过 `noexcept` 且完全没有 catch-all 的独立行为；
- 其他取消类型、其他取消点、异常嵌套与 cleanup 组合。

本报告不提出修法，不修改候选补丁，不做方案建议。

## 7. 证据完整性

所有构建、运行、调试与源码核查命令的原文和退出码均在
`commands/`；早期命令中的失败尝试也保留，没有覆盖。两只 GBS root
在完成调试后均标记 `RETIRED_SINGLE_USE`。`MANIFEST.sha256` 覆盖本地
完整证据，`docs/progress/R31/` 为策展副本。
