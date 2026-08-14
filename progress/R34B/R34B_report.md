# R34-B：补齐 `__cxa_rethrow` 的外来异常处理并重验 S1

日期：2026-08-14  
执行边界：只修改 `tmp/R34B/llvm-exp/` 实验副本；`codes/llvm`、平台包
源码与冻结制品均未修改。全部运行均为 x86_64 宿主 CPU 原生执行、
目标 Tizen 用户态 loader 隔离，未使用 QEMU。

## 结论

结论为 `S1_COMPLETE`，严格限定于本次 x86_64、Itanium/DWARF、
libgcc_s 组合及 12 格矩阵。

补齐 foreign `__cxa_rethrow` 后，libc++ 与 libstdc++ 各执行 6 格：
`cancel`、`exit`、真实 `runtime_error`、非强制 foreign 重抛、取消无
catch 对照、退出无 catch 对照。12/12 退出 0、信号 0；按每格命令头
之后的完整输出逐字比较，6/6 用例两侧无差异。

代码已经完整放入 `docs/progress/R34B/code/` 等待人工审阅。在人工审阅
完成前，不据此结果向 armv7l、LLVM libunwind 或其他配置扩展结论。

## 1. `_LIBUNWIND_STD_ABI` 查证

本地 GCC 源码唯一实际定义位于
`libstdc++-v3/config/os/hpux/os_defines.h:89-94`：IA64 HP-UX 使用系统
unwind 库，而该库不提供非标准 IA64 Unwind ABI 扩展
`_Unwind_Resume_or_Rethrow`，所以定义 `_LIBUNWIND_STD_ABI` 并回退到
`_Unwind_RaiseException`。`ChangeLog-2003:4904-4909` 记录了同一理由。

本平台为 x86_64，编译器 `-dM` 未定义该宏；R33/R34B 配置为
`LIBCXXABI_USE_LLVM_UNWINDER=OFF`。平台 `libgcc_s.so.1` 实测导出
`_Unwind_Resume_or_Rethrow@@GCC_3.3`，故未触发停报条件。

两套实现的源码行为一致：

- libgcc `unwind.inc:255-274`：`private_1 == 0` 调
  `_Unwind_RaiseException`，非零继续 forced phase 2；
- LLVM libunwind `UnwindLevel1-gcc-ext.c:39-60`：`PRIVATE_1 == 0` 调
  `_Unwind_RaiseException`，非零调 `_Unwind_Resume` 继续既有展开。

定义、使用处、平台宏、配置、动态符号与两实现原文见
`commands/003`–`005`，结构化表见
`tables/libunwind_std_abi_findings.tsv`。

## 2. 实验改动与作用域

实验副本先由 R33 S1 副本逐字复制。R34B 最终只在
`libcxxabi/src/cxa_exception.cpp::__cxa_rethrow` 的非 SJLJ 分支增加
native/foreign 分流：

- native exception 仍调用 `_Unwind_RaiseException`；
- foreign exception 调用 `_Unwind_Resume_or_Rethrow`；
- SJLJ 分支仍调用 `_Unwind_SjLj_RaiseException`，未改动。

首次机械补丁因同形代码块误匹配到 `__cxa_throw`，作用域自检在构建前
发现；该改动已恢复，失败 diff 与命令原文保留。最终断言
`__cxa_throw` 与 R33 副本逐字一致，且新调用只在
`cxa_exception.cpp:654` 出现一次。最终差异见
`patches/r34b_cxa_rethrow_only.diff`。

完整 S1 相对平台基线共改 3 个文件、24 行新增、1 行删除：类型定义、
personality 映射、foreign rethrow。逐项见 `tables/change_scope.tsv`。

ABI 表面没有删除或改签既有符号；新增
`__forced_unwind` 的 typeinfo/name 两个弱导出符号。与此同时，既有
`__gxx_personality_v0` 的 foreign+force+typed-handler 路径和既有
`__cxa_rethrow` 的 foreign 路径发生行为变化。

## 3. 构建与资源约束

开工门禁为 20 CPU、可用内存约 20 GiB、load1 低于阈值 10、构建
分区可用约 470 GiB。配置为 runtimes-only：

- `LLVM_ENABLE_RUNTIMES=libcxx;libcxxabi`；未设置
  `LLVM_ENABLE_PROJECTS`；
- `LIBCXXABI_USE_LLVM_UNWINDER=OFF`；shared ON、static OFF；
- `CMAKE_BUILD_PARALLEL_LEVEL=2`、`LLVM_PARALLEL_LINK_JOBS=1`；
- 构建和链接均由 `nice -n 15 ionice -c 3` 启动；`cmake --build`
  显式 `-j2`，直接链接与两个 probe 的编译逐个串行执行。

构建 5 秒内完成，因此没有达到 5 分钟周期采样点；开始与结束样本均
约 19 GiB 可用内存，未触发 2 GiB 红线。完整配置、构建、链接和资源
原文见 `commands/010`–`016` 与
`results/build_resource_monitor.log`。

## 4. 验证矩阵

完整源为 `src/s1_rethrow_probe.cpp`。libc++ 格使用实验头和实验
`libc++abi.so.1`；libstdc++ 格使用平台 GNU 运行时。逐格结果见
`tables/matrix_results.tsv`，原始 stdout/stderr、退出码和信号见
`results/matrix/`。

- cancel：两侧均析构一次、命中 forced handler、join 为
  `PTHREAD_CANCELED`；
- pthread_exit：两侧均析构一次、命中 forced handler、join 为
  `0x2a`；
- runtime_error：两侧均析构后进入 `runtime_error` handler，未进入
  forced handler；
- 非强制 foreign：两侧均实测 `private_1=0`，inner catch 重抛后进入
  outer catch，cleanup 回调一次，退出 0；
- 两个无 catch 对照：两侧分别得到 `PTHREAD_CANCELED` 与 `0x2a`。

这也直接验证了本次改动对 `private_1 == 0` 的非强制外来异常仍退化为
普通 `_Unwind_RaiseException` 行为；本次两侧输出逐字一致。

## 5. 运行时身份

目标 loader 的 `--list` 与 `LD_DEBUG=libs` 均显示 libc++ 用例实际加载：

`tmp/R34B/runtime/libc++abi.so.1`

其实际文件 SHA256 为
`ed6416234c6c6c02a067b982199b0906e2a74fed7cd1efd89eb56fca50071b8d`。
原文见 `commands/017_runtime_identity.log`。

## 6. armv7l/EHABI 待验边界

本任务未执行 armv7l。`__cxa_rethrow` C++ 函数主体共用，但 ARM 使用
不同的 UCB 布局、ARM EHABI personality 分支和 libgcc ARM unwind
实现。`unwind-arm-common.inc:714-723` 以 `UCB_FORCED_STOP_FN` 选择普通
raise 或 forced phase 2，而非 x86_64 的字段访问形式。

需在 armv7l 原生载体复核的具体项列于 `tables/ehabi_pending.tsv`：
typed handler 映射、cancel/exit 的 forced rethrow、返回值、非强制
foreign 重抛及 stop 函数继续调用。本任务只列出，未实施。

## 7. 证据与审阅边界

命令原文与退出码位于 `commands/`；完整实验代码、diff、改后文件、
用例、构建/运行命令和输出、CMake 配置及 runtime 身份证据复制到
`docs/progress/R34B/code/`。本任务不提交上游、不修改平台包，也不做
方案推荐。
