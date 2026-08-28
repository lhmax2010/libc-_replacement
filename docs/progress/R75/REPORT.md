# R75：问题五 `shrink_to_fit` 复现用例修正与实测

日期：2026-08-28  
承接：`progress/R74/REPORT.md` 中“不一致清单”第 1 项

## 1. 结论摘要

R74 指出的复现缺陷已经被消除：最终用例先完成字符串构造和 `reserve(65536)`，再开启分配门禁；主线程只在 `shrink_to_fit()` 触发的下一次 `allocate()` 已进入后请求取消。调试栈直接证明目标分配位于 `basic_string::shrink_to_fit` 内部，而不再位于字符串构造阶段。

最终用例在 x86_64 与 armv7l、libstdc++ 与项目 libc++ 四个格上均稳定导致进程异常终止，没有一次线程干净取消或正常返回。因此，“取消在 `shrink_to_fit` 的不抛异常边界内送达时，两套实现都不能把它变成线程干净退出”得到实测支持。

但终止形态和最终故障机制并不完全相同：x86_64 两套库均为 20/20 退出 134；armv7l 项目 libc++ 为 20/20 退出 134，系统 libstdc++ 为 20/20 退出 139。设备调试栈证明 armv7l/libstdc++ 先由 `shrink_to_fit` 的不抛异常边界调用 `std::terminate()`，随后在 `__gnu_cxx::__verbose_terminate_handler()` 内发生二次 SIGSEGV。因此，“两套标准库表现一致”在 armv7l 上若指相同退出信号或相同最终机制则不成立；只能说二者都未能线程干净取消、最终都结束进程。

## 2. 用例修正

原报告用例的自定义分配器从第一次分配起就永久停在 `pthread_testcancel()`，使取消在 `CancelString(1024, 'x')` 构造期间送达，控制流从未到达 `reserve()` 或 `shrink_to_fit()`。

最终用例增加两个独立门禁：

1. `target_allocation_enabled` 只在构造和 `reserve()` 完成后开启；
2. `target_allocation_entered` 只在开启后的目标分配中置位，主线程据此发出取消请求。

同时记录分配序号、容量和目标分配大小。x86_64 的目标分配为 libstdc++ 第 3 次、1025 字节，libc++ 第 3 次、1032 字节；armv7l 对应序号同为第 3 次。原用例与最终用例的逐行差异见 `raw/022_problem5_final_correct.diff`。

两次探索尝试也完整保留：无取消送达点的版本两套库均超时；未加门禁的版本两套库均线程干净取消；最终门禁版本两套库均退出 134。结果见 `raw/018_attempts_digest.txt`。

## 3. x86_64 结果与调用栈

最终矩阵每套库运行 20 次：

- libstdc++：20/20 命中 `EVENT prepared` 和 `EVENT target_allocate`，20/20 退出 134；
- 项目 libc++：20/20 命中上述两个事件，20/20 退出 134。

调试器在目标分配处和最终 SIGABRT 处各取一次全线程栈：

- libstdc++：`CancelAllocator<char>::allocate` → `basic_string::shrink_to_fit` → `worker`；最终栈含 `std::terminate()`；
- libc++：`CancelAllocator<char>::allocate` → `basic_string::shrink_to_fit` → `worker`；最终栈含 `__clang_call_terminate`、`std::terminate()` 和 libc++abi 的 foreign-exception 终止处理。

自动证据门禁要求两份调试记录都同时包含目标函数、分配器帧和预期终止信号，结果为 PASS。关键行见 `raw/017_debug_key_lines.txt`，完整记录见 `raw/x86_debug/`。

运行时身份通过 SHA256、`ldd` 和 `LD_DEBUG=libs` 留证。项目 libc++ 实际加载 `tmp/R69/build-x86_64/lib/libc++.so.1` 与 `libc++abi.so.1`，不是宿主同名库；身份摘要见 `raw/016_identity_digest_inputs.txt`。

## 4. armv7l 结果

两个二进制均为 ARM EABI5、带调试信息的动态 PIE。部署后再次计算二进制和项目运行库 SHA256，与宿主构建产物一致；`ldd` 证明 libc++ 格从独立目录加载项目 `libc++.so.1` 与 `libc++abi.so.1`。

每套库运行 20 次：

- 系统 libstdc++：20/20 传输成功、远端退出 139、目标事件 20/20；
- 项目 libc++：20/20 传输成功、远端退出 134、目标事件 20/20。

40 次运行均取得可解析的远端退出码，没有超时或传输失败。结构化矩阵和聚合见 `raw/arm_final/matrix.tsv`、`raw/arm_final/summary.tsv`。

系统 libstdc++ 的设备端 GDB 现场补取了两次。第一份在实际 SIGSEGV 处保存全线程栈：

`__gnu_cxx::__verbose_terminate_handler` → libstdc++ 内部帧 → `std::terminate` → `__clang_call_terminate` → `basic_string::shrink_to_fit` → `worker`。

第二份用分阶段断点确认先后顺序：阶段 1 先命中 `std::terminate()`，调用者是 `__clang_call_terminate` 和 `shrink_to_fit`；阶段 2 随后进入 `__verbose_terminate_handler()`；阶段 3 才收到 SIGSEGV，PC 为 `__verbose_terminate_handler+38`。故 139 不是取消绕开 `shrink_to_fit` 后在别处偶然崩溃，而是同一不抛异常终止路径中的二次故障。完整记录见 `raw/arm_libstdcxx_gdb/007_gdb.stdout` 与 `raw/arm_libstdcxx_terminate_sequence/007_gdb.stdout`，提要见 `raw/044_arm_gdb_digest.txt`、`raw/046_arm_terminate_sequence_digest.txt`。

SIGSEGV 指令为 `ldrb r3, [r4, #0]`，当时 `r4=0xe1a0b00d`，即 verbose handler 在读取当前异常类型名称时解引用了无效地址。寄存器和反汇编见 `raw/048_arm_terminate_fault_detail.txt`。这解释了为何该格没有走到 `abort()`，而是以 139 结束。

板端只使用独立目录 `/opt/usr/home/owner/share/tmp/r75_20260828`。完成后该目录已移除并核验不存在，进程清单也已复核。

## 5. 机制边界

两套头文件都把 `basic_string::shrink_to_fit()` 声明为 `noexcept`/`_NOEXCEPT`。当前项目 libc++ 的函数体确有 `catch (const __forced_unwind&) { throw; }`，但该重抛仍需离开不抛异常函数，因此到达终止路径；源码摘录见 `raw/023_shrink_to_fit_declarations.txt` 与 `raw/024_libcxx_shrink_to_fit_body.txt`。

本轮证明的是一个刻意构造的确定性边界条件：取消被送达于 `shrink_to_fit()` 内部的分配器取消点。它不能证明普通分配器会自然提供取消点。四格共有的上游机制是强制展开试图离开 `noexcept` 的 `shrink_to_fit`，从而进入 `std::terminate`；armv7l/libstdc++ 独有的下游机制是 verbose terminate handler 内二次段错。因此对外报告必须显式披露该架构差异，不能笼统写成“两套标准库表现一致”。

## 6. 边界与异常申报

- 没有修改平台源码、补丁或对外报告；只新增 R75 复现代码、驱动、证据和本报告。
- ARM 构建驱动首次退出 1，原始驱动输出为空；未改代码的重试退出 0。构建子日志使用固定文件名，重试覆盖了首次子日志，因此无法从现存材料可靠断言首次失败原因。
- 复核阶段曾让 `rg` 把正在生成的输出文件纳入搜索，形成自扩张扫描；发现后停止，并改用明确排除 `progress/R75/**` 的检索。误生成文件保留为 `resume_rg_context.txt`，不计入结论。
- 复核时曾误把 R74 的第 5 个代码块当作“问题五复现”；错误 diff 保留为 `raw/020_problem5_final.diff`。随后依据 `blocks.json` 定位到第 10 个代码块并生成正确 diff。
- 自行判断与尚存疑问见 `SELF_DECISIONS_AND_QUESTIONS.md`。
