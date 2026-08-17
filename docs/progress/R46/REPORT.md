# R46：头文件站点防护的正向验证

## 结论

本次结论为 `HEADER_FIX_EFFECTIVE`，严格限定于实测的 2/34 个头文件站点及两个架构：

- `std::getline` 对应的 `codes/llvm/libcxx/include/istream:1328`；
- `std::packaged_task<void>::operator()` 对应的 `codes/llvm/libcxx/include/future:1764`。

在 x86_64 宿主原生和 armv7l 开发板原生执行中，两处均呈现相同结果：未打补丁为 134；只换入 R45 libc++abi、头文件不改仍为 134；只重链 libc++ 而不重编消费者仍为 134；消费者用补过防护的头文件重新编译后退出 0，`pthread_join` 返回 `PTHREAD_CANCELED`，与 libstdc++ 对照一致。

failbit 汇聚函数也取得正向证据，但该调用点外层还有 badbit catch，必须保留已由 R44b/R45 验证过的 badbit 防护才能隔离观察 failbit：只修 badbit 时两架构均为 134；badbit 与 failbit 两函数都带防护时两架构均为 0，并与 libstdc++ 对照一致。状态记为 `EFFECTIVE_WITH_NESTED_BADBIT_GUARD`。

## 口径与输入冻结

资源门禁 `tools/resource_gate.sh --level medium` 首次通过：`MemAvailable=20033440 KiB`、`load1=1.00`、`nproc=20`，退出码 0。构建只重编 `ios.cpp` 并重链 libc++，没有重建 libc++abi。R45 libc++abi 复用对象 SHA256：

- x86_64：`c280e67272ae1aa7e60ec7994392740606418828f228ae8f09871b8c41d08436`；
- armv7l：`d29552d2916a638ae4f82fa77185a3f791843c7373ebde7b4381b637776250ec`。

站点总量复核沿用 R44b 已落盘清单并重新计行：50 个 catch-all、7 个头文件；其中 16 个汇聚站点、34 个头文件本地站点。8 个“libstdc++ 有防护而 libc++ 无对应物”的站点是另一口径，不属于 50 的子集。34 个本地站点分布为：istream 23、future 9、ostream 1、string 1。

输入与 SHA 原文见 `commands/002_input_inventory.log`，完整清单副本见 `tables/unaggregated_34.tsv`、`tables/aggregated_16.tsv`、`tables/no_counterpart_8.tsv` 和 `tables/libcxx_catch_points_50.tsv`。

## 选样与参照

选样见 `tables/site_selection.tsv`。H1 具备已知的两架构 134 基线，阻塞 `read(2)` 是明确取消点。H2 属不同设施，调用体内显式执行 `pthread_testcancel()`，且 libstdc++ 在 `_Task_setter<void>` 中有直接的 forced-unwind 重抛保护。

实验修改严格限定为：

1. 在 H1 catch-all 前增加 `catch (const __cxxabiv1::__forced_unwind&)`，设置与原 catch-all 相同的 badbit 状态后重抛；
2. 在 H2 catch-all 前增加同类型 catch 并重抛；
3. 在 failbit out-of-line helper 中增加识别并重抛；因调用路径存在外层 badbit catch，最终组合同时包含既有 badbit 防护。

完整 diff 与被改文件全文位于 `code/libcxx/`。`codes/llvm` 工作树未被修改。

## 消费者重编实测

H1/H2 都位于头文件模板或内联函数，捕获表与处理代码进入消费者对象。实测而非机制推断的结果见 `tables/consumer_rebuild.tsv`：

- 只重链 libc++、保留旧消费者：两架构、两场景均为 134；
- 使用补丁头文件重编消费者、运行库保持同一 R45 ABI：两架构、两场景均为 0。

因此，在本次 2 个样本站点上，不重编消费者时行为没有变化；重编消费者后才获得新行为。与 `ios.cpp` out-of-line 汇聚函数不同，后者替换共享库即可被既有消费者调用。

## 运行矩阵

逐格结果见 `tables/runtime_matrix.tsv`；未经整理的 stdout、stderr、退出码和加载身份分别见：

- `raw/x86_64_matrix_final.raw.log`；
- `raw/armv7l_board_matrix_final.raw.log`。

x86_64 明确为宿主原生执行，armv7l 明确为板上原生执行，均未使用模拟器运行测试。armv7l 测试消费者使用 DSO 载体，是因为该架构 R45 实验 ABI 未携带平台 `LLVM_22` 版本脚本，静态链接阶段无法同时满足平台 libc++.so 的版本化 ABI 引用和新 typeinfo；DSO 保留 `GLOBAL DEFAULT UND` 的 forced-unwind typeinfo，由板上实际加载的 R45 libc++abi 解析。该载体与测试逻辑未增加 catch。

运行时身份由 host loader `--list`/`LD_DEBUG=libs` 与板上 `ldd`/`LD_DEBUG=libs` 原文支撑。补丁消费者的 forced-unwind typeinfo 形态为：x86_64 `GLOBAL DEFAULT UND @LLVM_22`，armv7l DSO `GLOBAL DEFAULT UND`。

## failbit 补验

用例使 `basic_ostream::operator<<(streambuf*)` 在 source streambuf 的 `underflow()` 中命中显式 `pthread_testcancel()`。该异常先进入内层 failbit catch；若 failbit helper 重抛，还会进入外层 badbit catch。因此：

- 无防护：134；
- 只有 R45 ABI：134；
- 只有 badbit helper 防护：134；
- badbit + failbit helper 防护：0，join 为 canceled；
- libstdc++：0，join 为 canceled。

以上结果在 x86_64 与 armv7l 完全一致，直接区分了 failbit 新增防护的效果。

## 代表性边界

本次只覆盖 2/34 个头文件本地站点。覆盖了两个不同设施、两种明确取消点，但没有实测其余 32 个站点：22 个其他 istream 站点、8 个其他 future 站点、1 个 ostream 站点、1 个 string 站点。它们在控制流、状态更新和异常处理语义上可能不同，本报告不把两个样本外推为其余站点的运行时结论。

## 板端纪律与清理

板端 preflight 确认目标目录不存在、项目相关进程数为 0。测试完成后 `/var/tmp/r46_20260817` 与传输归档均删除成功，相关进程数为 0。已登记例外 `/home/owner/r44b_push_probe` 在前后均存在，本任务未尝试删除或使用。

## 已知工具过程记录

初次消费者构建脚本错误地把完整源码头目录作为 overlay，且缺少 fail-fast，造成目标缺件；错误在任何运行实验之前被发现。原始失败日志保留在 `commands/017_*`、`018_*`、`020_*`、`021_*`，修正为只包含 `istream`/`future` 的稀疏 overlay 后成功。该过程不参与最终矩阵结果。

本任务没有做全量实现、没有修改平台源码、没有给出方案推荐或成本估算。
