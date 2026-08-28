# R75 审阅材料导航

## a. 最终用例完整源码

- `code/shrink_to_fit_reproducer.cpp`

## b. 失败尝试源码与原因

- `code/attempt1_no_delivery_point.cpp`：没有实际取消送达点；两套库均超时。
- `code/attempt2_ungated_allocator.cpp`：分配器从字符串构造期即开放取消点；两套库均在线程构造阶段干净取消，未进入 `shrink_to_fit`。
- `raw/x86_attempts/matrix.tsv`：三版用例的编译/运行结果。
- `raw/018_attempts_digest.txt`：失败原因摘要。
- `raw/022_problem5_final_correct.diff`：原报告用例与最终用例逐行差异。

## c. 编译命令原文

- x86_64/libstdc++：`raw/x86_attempts/shrink_to_fit_reproducer.libstdcxx.compile.log`
- x86_64/libc++：`raw/x86_attempts/shrink_to_fit_reproducer.libcxx.compile.log`
- armv7l/libstdc++：`raw/arm_build/libstdcxx.compile.log`、`raw/arm_build/libstdcxx.link.log`
- armv7l/libc++：`raw/arm_build/libcxx.compile.log`、`raw/arm_build/libcxx.link.log`
- 构建产物与运行库身份：`raw/x86_final/identity.sha256`、`raw/arm_build/identity.log`、`raw/arm_final/006_identity.log`

## d. 20 次运行完整原始输出

- x86_64：`raw/x86_final/libstdcxx.run01.*` 至 `run20.*`，以及 `libcxx.run01.*` 至 `run20.*`。
- armv7l：`raw/arm_final/libstdcxx.run01.*` 至 `run20.*`，以及 `libcxx.run01.*` 至 `run20.*`。
- 每次均保留命令、stdout、stderr；结构化总表仅作导航，不替代原文。

## e. 调用栈证据

- x86_64 两套库完整栈：`raw/x86_debug/libstdcxx.stdout`、`raw/x86_debug/libcxx.stdout`。
- armv7l/libstdc++ SIGSEGV 全线程栈：`raw/arm_libstdcxx_gdb/007_gdb.stdout`。
- armv7l/libstdc++ 分阶段终止路径：`raw/arm_libstdcxx_terminate_sequence/007_gdb.stdout`。
- armv7l 关键提要：`raw/044_arm_gdb_digest.txt`、`raw/046_arm_terminate_sequence_digest.txt`、`raw/048_arm_terminate_fault_detail.txt`。

## armv7l/libstdc++ 结论

该格先从 `basic_string::shrink_to_fit` 的不抛异常边界进入 `std::terminate`，随后才在系统 libstdc++ 的 `__gnu_cxx::__verbose_terminate_handler+38` 内收到 SIGSEGV。它与其余三格共享“强制展开穿过不抛异常边界而进入 terminate”的上游机制，但最终信号和 terminate handler 内的二次故障机制不同。因此，armv7l 上不能无条件写“两套标准库表现一致”；只能写二者都未能干净取消且都导致进程结束，并必须列明 libstdc++ 为 139、libc++ 为 134。
