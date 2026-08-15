# R42 EHABI handler 帧路径修正与回归报告

## 结论

结论为 `S1_COMPLETE_BOTH_ARCH`。

- armv7l 真机、x86_64 宿主原生各完成 patched libc++ 7 格与 libstdc++ 对照 7 格，28/28 格退出 0；两架构的未打补丁对照均退出 134/SIGABRT。
- armv7l trace-on 逐帧记录确认：forced typed handler 不再调用 `__cxa_begin_cleanup`；进入 handler 时 `propagatingExceptions` 为空；重抛后遇到真正 cleanup 时完成一次 `nil -> exception -> nil` 的压栈/出栈配对，最终 `join=PTHREAD_CANCELED`。
- x86_64 没有因第四处 EHABI 专用改动回归：patched libc++ 7/7 退出 0，并与 libstdc++ 7/7 的可观测输出逐格一致。
- 完整 S1 实现涉及 3 个源文件；相对当前平台基线的完整实验 diff 为 `+237/-6`（其中包含可关闭的 R40/R41/R42 观测代码）。新增公开的 `__forced_unwind` 类及其 out-of-line key function 会新增类型、析构、typeinfo/typeinfo-name 等 ABI 符号；既有 ABI 符号未删除。第四处改动本身不新增导出符号，只改变严格限定组合的控制路径。

## 1. 输入、环境与纪律

| 项目 | 实测值 |
|---|---|
| llvm 源码基线 | `codes/llvm@5ed6c77278dfa7a470667cf1a137723d3c96fe60` |
| 源码版本 | llvmorg-22.1.8 派生的 tizen_base |
| 构建方式 | runtimes-only；只产出 shared libc++abi；libgcc_s unwinder |
| 并行与优先级 | `CMAKE_BUILD_PARALLEL_LEVEL=2`、`LLVM_PARALLEL_LINK_JOBS=1`、`nice -n 15`、`ionice -c 3` |
| trace-off 库 | x86_64 SHA256 `f54d347c...`; armv7l SHA256 `086fd2bf...`（构建树原件） |
| trace-on 库 | armv7l SHA256 `c33b8b5c...`（构建树原件） |
| 真机 | `192.168.108.26:26101`，armv7l，Tizen 11.0，build id `tizen-unified-toolchain_20260728.012216_tizen-headed-armv7l` |

资源门禁脚本先经 `bash -n` 与干跑；每次 configure/build 前都显式调用并检查。门禁记录的 20 CPU、可用内存约 19 GiB、1 分钟负载低于 16.0 阈值，均为 PASS。三个构建串行执行，无自动并行度。构建期采样未出现可用内存低于 2 GiB。

板端开工前无本项目遗留进程。测试后第一次清理脚本因进程正则匹配到自身而在最终核验前终止，未采信；修正版按可执行名精确 `pidof` 后确认三个 PID 集均为空、专用目录不存在。见 `commands/058_board_cleanup_retry.log`。

## 2. 规范与参考实现

### 2.1 ARM EHABI 原文

从 Arm 官方 `abi-aa` 仓的固定提交 `ee4b3c12d57c8424ff60c2ae56e10690d0604ab6` 取得 `ehabi32/ehabi32.rst`，SHA256 为 `a500c2bf0a419f04f229a2d14889b5977a643a1d18710935230908ea7bfe79cf`。原文及行号摘录在 `commands/006_arm_ehabi_relevant_text.log`，完整原文在 `reference/arm/ehabi32.rst`。

实际条目说明：

- §8.1.1 区分 handler 与 cleanup：handler 结束传播，cleanup 执行后传播继续。
- §8.4 规定 phase 2 的 landing-pad 安装与 cleanup 恢复。
- §9.4.2 的 `__cxa_begin_catch`/`__cxa_end_catch` 定义 catch 生命周期。
- §9.4.2 的 `__cxa_begin_cleanup`/`__cxa_end_cleanup` 定义 cleanup 生命周期。
- 附录示例明确：cleanup descriptor 调 `__cxa_begin_cleanup`；catch descriptor 直接安装 catch context，不调用该函数。

重要边界：该版本官方 EHABI 原文没有 `_Unwind_ForcedUnwind` 或 `_US_FORCE_UNWIND` 条目。故“handler 与 cleanup 必须走不同生命周期”的依据强度为官方规范；“将 POSIX forced unwind 映射为一个可捕获的 typed handler”的依据来自 GCC 的 ABI 扩展参考实现，而不是 ARM 规范直接规定。

### 2.2 GCC/libstdc++ 参考

固定 GCC 源码 HEAD `49c5060508ce8a6ed6beca375233688a6bdd9140` 的原文见 `commands/010_static_reference_extract.log`：

- `eh_arm.cc` 将 forced unwind 映射到 `abi::__forced_unwind`。
- `eh_personality.cc` 用 `found_handler` 与 `found_cleanup` 保留分类。
- 只对 `found_cleanup` 调 `__cxa_begin_cleanup`；catching handler 不调用它。

### 2.3 正确处理的明确回答

强制展开匹配到 catching handler 时，应保存 handler/barrier 结果、设置 handler landing-pad 寄存器并返回 `_URC_INSTALL_CONTEXT`；不能把 handler 预登记到 cleanup 栈。编译器生成的 landing pad 随后走 `__cxa_begin_catch`/`__cxa_end_catch`。这一回答的 handler/cleanup 分离来自 ARM EHABI；forced typed handler 的具体扩展形态与分类方式由 GCC/libstdc++ 参考实现交叉确认。

## 3. 第四处改动

在 `scan_results` 中增加仅供本次控制流使用的 `isForcedUnwindHandler`。它只在以下全部条件满足时置位：

1. `native_exception == false`；
2. `actions & _UA_FORCE_UNWIND`；
3. catch 类型与 `__cxxabiv1::__forced_unwind` 匹配；
4. `scan_eh_tab` 返回 `_URC_HANDLER_FOUND`。

EHABI phase 2 遇到该标志时，保存 barrier cache、设置寄存器并直接返回 `_URC_INSTALL_CONTEXT`，从而绕过只属于 cleanup 的 `__cxa_begin_cleanup`。其他 `_URC_HANDLER_FOUND` 路径仍逐字保留原有 cleanup 登记逻辑。

红线核验：`__cxa_begin_cleanup` 中 `globals->propagatingExceptions` 非空即 terminate 的检测未修改。完整 diff 为 `diff/*.diff`，逐项表为 `tables/implementation_changes.tsv`。

R40/R41 诊断打印统一封装为构建期开关 `LIBCXXABI_R42_TRACE`：最终矩阵用 OFF 构建；`strings` 实测两个 OFF 库均无 R40/R41/R42 字符串，ON 库含观测点。见 `commands/032_build_identity_and_trace_toggle.log`。

## 4. 构建结果

| 架构/变体 | configure | build | 产物 |
|---|---:|---:|---|
| x86_64 trace-off | 0 | 0 | `tmp/R42/build-patched-x86_64/lib/libc++abi.so.1.0` |
| armv7l trace-off | 0 | 0 | `tmp/R42/build-patched-armv7l/lib/libc++abi.so.1.0` |
| armv7l trace-on | 0 | 0 | `tmp/R42/build-trace-armv7l/lib/libc++abi.so.1.0` |

构建日志中的告警均为 clang 在编译阶段报告 `--rtlib=libgcc` 或交叉 linker 参数未使用；链接均成功。trace-off 初次 x86 构建曾因仅用于 trace 返回值的局部变量产生一条 unused-variable 告警，随后以 `(void)` 显式消费并增量重建，最终源码与产物身份已重新落盘。

## 5. 回归矩阵

完整 31 行结果见 `tables/regression_matrix.tsv`，原始输出分别见 `commands/040_x86_matrix.log`、`commands/054_board_matrix.log`、`commands/055_board_trace_cancel.log`。

| 架构 | patched libc++ | libstdc++ 对照 | 未打补丁对照 | trace-on |
|---|---:|---:|---:|---:|
| x86_64 | 7/7 退出 0 | 7/7 退出 0 | 134/SIGABRT | 不适用 |
| armv7l | 7/7 退出 0 | 7/7 退出 0 | 134/SIGABRT | cancel 退出 0 |

消费者链接命令均未包含 `--export-dynamic`。两架构 patched 测试件的 forced typeinfo 均为 `GLOBAL DEFAULT UND` 并由实验 libc++abi 的强定义解析。x86_64 通过目标 userspace loader 在宿主原生执行；armv7l 在开发板原生执行，未使用 QEMU。

嵌套格两架构、两套标准库的顺序一致：原 try 块对象析构，进入 forced handler，handler 内层对象析构，handler 外层对象析构，然后 join 为 `PTHREAD_CANCELED`。

## 6. armv7l 逐帧验证

完整顺序见 `tables/trace_sequence.tsv`。关键事实：

1. `R41_SCAN_MATCH kind=forced-type ... ttype=2 reason=6` 后立即出现 `R42_EHABI_FORCED_HANDLER_PATH`；二者之间及 landing 前没有 `R41_EHABI_BEGIN_CLEANUP_CALL`。
2. `__cxa_begin_catch` 入口与返回均显示 `propagating=(nil)`，说明 handler 未污染 cleanup 栈。
3. 重抛时 `reserved1` 非零，forced unwind 状态保留。
4. 随后的真正 cleanup（`ttype=0`）才调用 `__cxa_begin_cleanup`，栈从 nil 压入 exception；`__cxa_end_cleanup` 将其弹回 nil。
5. 最终 `join=PTHREAD_CANCELED`、退出 0。背景中的“第二次 begin_cleanup 遇到非空栈并 terminate”序列不再出现。

因此 `propagatingExceptions` 对真 cleanup 的压栈/出栈已经配对；catch handler 不再错误进入该栈。

## 7. ABI 与覆盖边界

- 完整 S1 新增 `__cxxabiv1::__forced_unwind` 公开类型与强 typeinfo，属于新增 ABI 表面；未删除或改名现有符号。
- `__cxa_rethrow` 对 foreign exception 的行为发生条件性变化；原生异常与 SJLJ 分支保持原逻辑。
- 第四处仅编译于 EHABI personality，并进一步受 foreign+force+forced typed handler 条件限制；Itanium/x86_64 控制路径不受它影响，且实际 7/7 回归通过。
- 本轮覆盖 x86_64/Itanium-DWARF 与 armv7l/ARM EHABI、libgcc_s unwinder。未覆盖 LLVM libunwind、aarch64、SJLJ、真实产品业务负载、长期压力/并发取消。

## 8. 取证过程中的失败留痕

- 首次 `sdb push` 内容被 Smack 标签阻断，板上 shell 无权读取；未运行测试。
- stdin 流式传输不被该 SDB shell 支持，远端得到空文件；未运行测试。
- 2048 字节 base64 命令超过 SDB service name 限制；最终沿用已验证的 512 字节分块、逐文件 SHA MATCH。
- 首次板端清理脚本误杀自身；最终以精确 `pidof` 重做并通过。以上均保留原命令、输出与退出码，没有覆盖或删除失败证据。

## 9. 上游归档

备用材料位于 `docs/upstream/libcxxabi-forced-unwind/`，含问题、复现器、四层根因、完整拟议 diff、两架构测试、引用与状态。状态明确为 `NOT_SUBMITTED`；本任务没有向 LLVM、Gerrit 或其他外部源码仓提交或推送。
