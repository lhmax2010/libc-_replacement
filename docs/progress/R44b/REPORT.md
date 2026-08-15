# R44b 标准库层汇聚点方案可行性 spike

## 结论

本次结论为 **PARTIAL**。

- 静态口径 `50 / 7 / 16 / 8` 经逐处复核成立：libc++ 有 50 处相关 catch-all，分布于 7 个头文件；其中 16 处调用两个 `ios.cpp` out-of-line 汇聚函数；其余 34 处为头文件本地处理；另有 8 处 libstdc++ 防护在 libc++ 中无直接对应物。
- 16 处中，15 处调用 `__set_badbit_and_consider_rethrow()`，1 处调用 `__set_failbit_and_consider_rethrow()`。本 spike 严格只修改前一个函数。
- x86_64 原生 `cout` 格得到正向证据：未补丁及仅 R42 层 1 均以 134 退出并打印 `FATAL: exception not rethrown`；加入汇聚点修改后，实测线程取消完成且 `join=PTHREAD_CANCELED`，并有一次完整退出 0。
- 同一 x86_64 严格同构用例也出现过“已成功 join 后进程未在 20 秒内退出”的 143；libstdc++ 对照同样出现该现象，而原 R43b 二进制单独复核也存在运行间差异。因此将它登记为测试件进程收尾不稳定，不将单次无问题外推为稳定通过。
- armv7l 真机上，汇聚点组合仍以 134 退出；输出从未补丁/仅层 1 的 glibc `FATAL: exception not rethrown` 变为 `libc++abi: terminating`，未观察到成功 join。同板 libstdc++ 对照退出 0。
- x86_64 `getline` 补充格仍为 134；其实现位于 `istream` 的头文件本地 catch-all，不经过本次修改的 badbit 汇聚函数。

因此，“在汇聚函数补上防护能修好 `cout` 场景”只在 x86_64 获得正向观测；armv7l 未成立，`getline` 也不在该单函数覆盖内。不能申报 `AGGREGATION_VIABLE`，也不能申报全面不成立，故为 `PARTIAL`。

## 资源门禁与构建边界

固定工具 `tools/resource_gate.sh --level light` 退出 0：`MemAvailable=22262548 KiB`、`load1=0.72`、`nproc=20`。门禁通过后才执行任何后续步骤。

未重建 libc++abi。两架构均复用 R42 的 libc++abi，仅从既有 R9-C libc++ 构建树中重编一个 `ios.cpp.o` 并重链 `libc++.so.1.0`。实际并发为单个编译/链接进程，不超过 2；命令由 `nice -n 15 ionice -c 3` 启动。x86_64 首次调用因 Ninja 记录的 buildroot 内 `/bin/...clang++` 在宿主不存在而退出 127；保留原文后，将该路径机械映射到项目既有编译器包装器，重试成功。armv7l 测试件首次把编译与链接合并时失败，按 R43b 已验证的编译器调用形态拆为“先 `-c`、后链接”后成功；两次失败均为实验调用装置问题，未修改平台源码。

## 汇聚点核实

两个函数定义于 `codes/llvm/libcxx/src/ios.cpp`：

- `ios_base::__set_badbit_and_consider_rethrow()`：设置 badbit，仅在 `exceptions() & badbit` 时执行原来的 `throw;`。
- `ios_base::__set_failbit_and_consider_rethrow()`：设置 failbit，仅在 `exceptions() & failbit` 时执行原来的 `throw;`。

它们在 `codes/llvm/libcxx/include/ios` 中仅有非 inline 声明。16 个调用点逐处读代码确认，均在头文件 catch-all 中直接调用其中之一；清单见 `tables/aggregated_16.tsv`。未汇聚 34 处见 `tables/unaggregated_34.tsv`，构成为 `istream` 23、`future` 9、`string` 1、`__ostream/basic_ostream.h` 1。50 处全集见 `tables/libcxx_catch_points_50.tsv`。

8 处无对应物并非上述 50 的子集，而是 libstdc++ 防护映射的另一口径：experimental executor 4、GNU vstring 2、TR2 dynamic_bitset 1，以及 condition_variable 1（libc++ 对应物为 noexcept relock guard、无 catch）。见 `tables/no_counterpart_8.tsv`。

libstdc++ 的 `__catch` / `__throw_exception_again` 来自 `libsupc++/exception_defines.h` 的异常开关兼容宏；它们不汇聚处理逻辑。65 处防护仍是逐处写出的 handler。本次汇聚机会来自 libc++ 头文件调用 out-of-line `ios.cpp` 函数，与这两个宏无关。

## 消费者重编边界

修改两个 `ios.cpp` 函数的实现只要求重编/重链 libc++：函数不 inline，定义不在安装头文件中，消费者中的调用点跨 TU 调用共享库符号，因此消费者无需因实现变更而重编。

其余 34 处位于安装头文件中的 inline/template 实现。若修改这些位置，新的处理代码只有在消费者重新编译后才会进入消费者对象，故消费者须重编才能获得修改。该事实差异见 `rebuild_boundary.tsv`；未换算时间或人日。

## Spike 改动

实验副本仅改 `libcxx/src/ios.cpp`：加入 `<cxxabi.h>`，并在 `__set_badbit_and_consider_rethrow()` 的活动异常上下文中识别 `__cxxabiv1::__forced_unwind` 后立即重抛；其他异常继续保持原先“默认吞掉、仅 exception mask 命中才重抛”的行为。未修改 failbit 函数及任何头文件。完整 diff、上游原文及修改后全文位于 `code/libcxx/src/`。

## 实测矩阵

完整逐格结果见 `spike_matrix.tsv`，未经整理的 stdout、stderr 与退出码在 `raw/`。执行方式为 x86_64 宿主原生目标用户态（无 QEMU）及 armv7l 开发板原生（无 QEMU）。应用源码逐字复用 R43b：裸 `pthread_create`、线程函数非 noexcept、应用层无 catch。

armv7l 的加载身份由板上 `ldd` 与 `LD_DEBUG=libs` 证明：三个组合分别加载部署目录中的 libc++/libc++abi，汇聚组合加载本次重链的 libc++ 与 R42 libc++abi。同样的宿主证据也已保存。

## 未汇聚与无对应物的事实性质

- 34 处未汇聚点是消费者头文件内联/模板实现；修改会进入消费者对象，需消费者重编。
- 8 处无对应物没有可直接修改的 libc++ 对应 catch 点；其中 7 处属于 libstdc++ 扩展设施，1 处 libc++ 采用无 catch 的不同实现形态。

本报告不提出修法、不估算成本、不修改平台源码。

## 板端清理核验

正式部署目录 `/var/tmp/r44b_20260815`、共享传输文件及所有测试进程均已清除并核验。传输机制排障期间曾向 `/home/owner/r44b_push_probe` 推送 444 字节探针；该文件带 `User::Home` Smack 标签，当前 `User::Shell` 会话能读但无权删除，`chsmack` 与 `smackexec` 尝试均未成功。该残留已按原路径、权限、标签落盘，未隐瞒为“清理成功”。它不是测试二进制或库，不参与任何实验结果。
