# R45：修复 B1 与类型形态，并重跑汇聚点验证

## 结论

本轮 B1 与 M1 均按既定判据完成实验修正，验证结论如下：

- **B1 已修复。** `armv7l + R45 + forced_unwind1.pass.cpp` 退出 0；同格 R42 退出 134，未补丁与 libstdc++ 对照均退出 0。R45 的两架构 20 个上游测试格全部退出 0，未观察到新增回归。
- **M1 已修复。** `__cxxabiv1::__forced_unwind` 现为私有虚析构加纯虚 `__pure_dummy()` 的抽象类。按引用捕获在 libc++/libstdc++ 两侧均编译成功；按值捕获在两侧均因抽象类型编译失败。
- **强 typeinfo 形态保留。** out-of-line 虚析构继续作为 key function；x86_64 与 armv7l 的 libc++abi 均导出 `GLOBAL DEFAULT` 的 typeinfo、typeinfo-name、vtable 与析构符号。消费者在不使用 `--export-dynamic` 时形成 `GLOBAL DEFAULT UND`，由库内强定义提供。
- **R42 回归矩阵恢复。** 每架构执行 28 格（原七类场景与新增两个 catch-all 重抛场景，各含 R42/R45/libstdc++，另含未补丁 cancel 对照）。R45 的 18 个两架构场景格全部退出 0，退出状态和 join 值逐格匹配 libstdc++。
- **汇聚点重新判定为 `AGGREGATION_VIABLE`。** x86_64 与 armv7l 上，未补丁及“仅 R45 libc++abi”均在 `cout` 内部 catch-all 吞掉强制展开后退出 134；“R45 libc++abi + ios.cpp 汇聚点修改”均已观察到 `JOIN_IS_CANCELED=1`，与 libstdc++ 对照的功能结果一致。armv7l 修复组合直接退出 0，证明 R44b 的 ARM 失败由 B1 挡住，不是汇聚点间接重抛在 EHABI 上天然失效。

这些改动只存在于 `tmp/R45/source-patched` 实验副本；未修改 `codes/llvm` 或其他平台包源码，也未向外部源码仓推送。

## 资源门禁与执行边界

任何只读检查或板端 preflight 之前，先执行固定工具：

```text
tools/resource_gate.sh --level medium
```

门禁退出 0：`nproc=20`、`MemAvailable=22369164 KiB`、`load1=0.88`、构建分区可用 491 GB。构建均由 `nice -n 15 ionice -c 3` 启动，并发固定为 2，`LLVM_PARALLEL_LINK_JOBS=1`；采样未触及 1 GiB 中止线。完整原文见 `raw/resources/` 与 `raw/build/`。

仅构建 libc++abi runtimes 目标及验证所需测试件；不构建整个 LLVM。汇聚点 libc++ 复用 R44b 已重编的单个 `ios.cpp` 产物，不重建 libc++abi 以外的工程。

测试件构建阶段保留了两次装置错误原文。x86_64 初版脚本未启用 `set -e`，一次版本节点链接失败后错误地返回 0；后续的产物存在性检查立即发现缺件，没有用该结果运行测试。脚本改为 `set -euo pipefail`、完成 LLVM_22 版本化重链后重新构建成功。armv7l 初次把编译与链接合成一条命令时失败，随后按已验证的交叉工具链形态拆成“先生成对象、再链接”并成功。失败尝试与最终命令均完整保留在 `raw/build/`，没有把失败尝试计为通过。

## B1：handler 与 cleanup 门控

libstdc++ 的参照实现位于 `libstdc++-v3/libsupc++/eh_personality.cc:734-735`：仅当 `found_type == found_cleanup` 时调用 `__cxa_begin_cleanup`。R42 的实验实现另设 `isForcedUnwindHandler`，只让新加的 typed forced-unwind handler 绕过 cleanup 入栈；catch-all handler 因未置该字段，仍被错误送入 `__cxa_begin_cleanup`。

R45 将门控改为 EHABI 扫描结果自身的 handler 判据：

- `ttypeIndex != 0`：catching handler，保存 barrier cache、安装寄存器并返回 `_URC_INSTALL_CONTEXT`，不调用 `__cxa_begin_cleanup`；
- `ttypeIndex == 0`：真 cleanup，保留既有 `__cxa_begin_cleanup` 路径。

条件仍限定为 forced unwind 且 foreign exception；原生异常、非强制展开及 Itanium 控制流未被此门控改变。`__cxa_begin_cleanup` 的非空检测未修改。

`isForcedUnwindHandler` 修正后不再承载额外信息：其四个调用点仅为字段声明、初始化、匹配时赋值和门控读取，均已删除；门控直接使用已有 `ttypeIndex`。R42 到 R45 的完整 diff 见 `code/diff/R42_to_R45_cxa_personality.cpp.diff`。

## M1：抽象类型与 key function

类体改为与 libstdc++ 形态相同的：

```cpp
class _LIBCXXABI_TYPE_VIS __forced_unwind {
  virtual ~__forced_unwind();
  virtual void __pure_dummy() = 0;
};
```

纯虚函数不是 key function；首个非纯且非 inline 的虚函数仍为 out-of-line 析构。符号表实测证明两架构库内 RTTI/vtable 均为强全局默认可见定义。测试件无需 `--export-dynamic`；按值捕获不能编译，因而不会从 foreign unwind 对象之后的内存做值拷贝。原文见 `raw/tests/m1_type_shape_and_visibility.log` 与 `raw/build/library_identity_and_symbols.log`。

## 上游测试矩阵

运行：

- `libcxxabi/test/forced_unwind{1,2,3,4}.pass.cpp`
- `libcxxabi/test/unwind_0{1..6}.pass.cpp`

组合为 x86_64 宿主原生 / armv7l 板上原生，以及未补丁 / R42 / R45 / libstdc++ 对照。共 80 格；R45 为 20/20 通过。唯一直接体现 B1 的差异是 armv7l `forced_unwind1`：R42=134，R45=0。其余 R45 格均为 0。ARM 测试只沿用 R43b 已核定的类型写法适配，测试逻辑未改；diff 原文在 `raw/commands/011_upstream_test_source_diff.log`。

逐格表见 `tables/upstream_test_matrix.tsv`，未经整理的 stdout、stderr、退出码见 `raw/tests/x86_64_matrices.raw.log` 与 `raw/tests/armv7l_matrices.raw.log`。

## R42 回归矩阵与新增格

九类场景为：cancel、exit、真实 C++ 异常、非强制 foreign 异常、cancel/exit 无 catch、nested cancel、cancel catch-all 后重抛、exit catch-all 后重抛。R45 两架构 18 格全部退出 0；每格的 join 值和退出码均与 libstdc++ 对照匹配。

新增两格在 armv7l 上直接区分 B1：

| 场景 | R42 | R45 | libstdc++ |
|---|---:|---:|---:|
| `pthread_cancel` + `catch (...) { throw; }` | 134 | 0，`PTHREAD_CANCELED` | 0，`PTHREAD_CANCELED` |
| `pthread_exit` + `catch (...) { throw; }` | 134 | 0，`join=0x2a` | 0，`join=0x2a` |

x86_64 的 R42、R45 与 libstdc++ 在这两格均退出 0，说明 B1 修改未给 Itanium 路径引入可观察回归。逐格输出和顺序见 `tables/regression_matrix.tsv` 及两份完整 raw log。

## 汇聚点 spike 重跑

使用 R44b 原测试源码和原 `ios.cpp` 单函数实验改动，应用层无 catch、裸 `pthread_create`、线程函数非 `noexcept`。功能结果如下：

| 架构 | 未补丁 | 仅 R45 ABI | R45 ABI + ios.cpp | libstdc++ |
|---|---|---|---|---|
| armv7l | 134，FATAL | 134，FATAL | 0，join canceled | 0，join canceled |
| x86_64 | 134，FATAL | 134，FATAL | join canceled | join canceled |

因此，补上标准库汇聚点防护能修复 `cout` 场景的正向证据已在两架构取得；R44b 的 armv7l `libc++abi: terminating` 是 B1 的 catch-all cleanup 错误登记所致，B1 修复后不再出现。

### `0_and_143` 的原文边界

x86_64 对修复组合和 libstdc++ 各重复三次。修复组合三次均先打印 `JOIN_IS_CANCELED=1`，随后由外层 `/usr/bin/timeout` 在 20 秒处发送 SIGTERM，退出 143；libstdc++ 对照两次同样为 143、一次自行退出 0。`timeout --verbose` 明确打印发送 TERM 的动作，故 143 的直接来源是实验 watchdog，而不是补丁自行发出信号。额外的 `strace` 对照中两套实现又均自行退出 0，显示该收尾现象受执行时序影响。

“成功 join 后某些运行为何未在 20 秒内自行退出”的更深层原因未被稳定观测到，登记为 `NOT_OBSERVED`。本任务只据逐格功能输出判定：修复组合与 libstdc++ 均完成强制展开、析构和线程 join；不把 143 解释为补丁回归。完整证据见 `raw/tests/x86_64_aggregation.raw.log` 和 `raw/tests/x86_64_0_and_143_diagnostic.log`。

## 产物身份与板端清理

- x86_64 R45 原始 libc++abi：`961a15be…`；用于目标用户态的 LLVM_22 版本化重链：`c280e672…`。
- armv7l R45 libc++abi：`d29552d2…`。
- armv7l R42 对照 libc++abi：`086fd2…`。
- x86_64 汇聚点 libc++：`89caaa91…`；armv7l 汇聚点 libc++：`0335dec…`。

完整 SHA256 和实际加载路径在 `raw/build/library_identity_and_symbols.log`、两份矩阵 raw log 的 loader/LD_DEBUG 区段。

板端 `/var/tmp/r45_20260815`、共享传输归档及测试进程均已清除：目录不存在、传输文件不存在、目标进程计数 0。既有登记例外 `/home/owner/r44b_push_probe` 未使用、未尝试删除。

## 覆盖与边界

本结论覆盖 x86_64 原生和 armv7l 真机的指定上游测试、R42 回归矩阵及 `cout` 汇聚点场景。未实现替代方案，未修改未汇聚的 34 个头文件 catch 点，也未修改 8 个无对应物设施；这些对象的性质仍沿用 R44b 的静态清单。本报告不做方案推荐或成本估算。
