# R43b 上游测试基线与标准库层 forced-unwind 防护实测报告

## 结论

- A 部：上游 `forced_unwind1` 在 `armv7l + R42 libc++abi` 下退出 134，并输出 `libc++abi: terminating`；同一测试在 armv7l 未打补丁 libc++abi、armv7l libstdc++、以及 x86_64 三个运行时组合下均退出 0。该上游测试复现了任务定义的 B1 缺陷。除这一格外，适用范围内其余上游测试没有出现补丁后回归。
- B 部结论为 `CONFIRMED`：x86_64 宿主原生与 armv7l 真机上，`cout`、阻塞 `getline`、`packaged_task` 三个场景在未补丁和 R42 libc++abi 下均输出 `FATAL: exception not rethrown` 并退出 134；libstdc++ 对照均正常 join 为 `PTHREAD_CANCELED`。R42 四处改动没有改变这三类标准库内部 catch-all 场景。
- `shrink_to_fit` 的内部 catch 路径没有被本轮稳定触发，结论为 `NOT_OBSERVED`；纯计算加 `pthread_testcancel()` 对照在两架构、三运行时组合下均退出 0。
- 静态复核得到 libstdc++ 代码中的防护子句恰为 **65 处 / 16 文件**；另外有 2 处文档示例，未计入代码分子。libc++ `include/` 与 `src/` 对 `forced_unwind` 的检索为零命中。

## 1. 资源、环境与身份

本轮采用轻档门禁。独立脚本先通过 `bash -n` 与干跑；开工时 20 CPU、可用内存约 24 GiB、1 分钟负载 4.32（阈值 30），结论 PASS。构建并行度固定为 2，使用 `nice -n 15` 与 `ionice -c 3`。执行期采样未低于 1 GiB。

| 项目 | 实测值 |
|---|---|
| llvm 源码基线 | `codes/llvm@5ed6c77278dfa7a470667cf1a137723d3c96fe60` |
| gcc 源码基线 | 见 `code/commands/039_input_identities.log` |
| x86_64 执行方式 | 宿主原生；目标 userspace loader/sysroot；无 QEMU |
| armv7l 执行方式 | `192.168.108.26:26101` 真机；测试运行无 QEMU |
| 板卡身份 | armv7l / Tizen 11.0 / `tizen-unified-toolchain_20260728.012216_tizen-headed-armv7l` |
| R42 与未补丁库 SHA256 | 见 `code/commands/039_input_identities.log` 与每格运行日志 |

开工前板上未见本项目残留进程。部署后以逐文件 SHA256 核验；收尾确认专用目录删除、11 个测试进程名均无 PID。完整记录为 `code/commands/008_board_preflight_retry.log`、`code/commands/026_board_deploy.log`、`code/commands/030_board_cleanup.log`。

armv7l 测试件在宿主交叉编译；包装器通过 `qemu-arm-static` 执行目标 userspace 中的 clang，但产出的测试件全部在 armv7l 真机原生运行。复用的两个编译器包装器与 R42 CMakeCache 已归档到 `code/config/`。

armv7l 测试件静态链接 R38 的 `libc++.a`，并动态加载 `libc++abi.so.1`；未补丁/R42 两格仅通过 `LD_LIBRARY_PATH` 切换 ABI 库。板端 `LD_DEBUG=libs` 明确记录 R42 格从专用部署目录加载 `libc++abi.so.1`，其余依赖来自板上 `/lib`。x86_64 使用动态 libc++，相同日志确认 R42 ABI 来自 `tmp/R42/runtime/patched/x86_64/`。

## 2. A 部：上游测试套件

### 2.1 测试与适配

执行清单为：

- `forced_unwind1.pass.cpp` 至 `forced_unwind4.pass.cpp`
- `unwind_01.pass.cpp` 至 `unwind_06.pass.cpp`

原始源码、SHA256 与完整内容见 `code/commands/009_upstream_test_inventory.log`、`code/commands/040_test_source_sha.log` 和 `code/src/upstream/`。

ARM 仅作三处语法适配，测试逻辑与判据未变：

- `forced_unwind1.pass.cpp` 两处 `struct _Unwind_Exception*` 改为 `_Unwind_Exception*`；
- `forced_unwind2.pass.cpp` 一处同样改动。

完整 diff 见 `code/commands/015_adaptation_diff.log` 与 `code/commands/016_adaptation_diff_2.log`。仓内 `codes/llvm` 原件未改。

`forced_unwind4` 的上游元数据为 `REQUIRES: linux && target=aarch64-{{.+}}-gnu`。本轮仍记录了它在 x86_64/armv7l 的执行结果，但表中标为 `OUTSIDE_UPSTREAM_REQUIRES_OBSERVED`，不把该格当作其上游支持域内的 PASS。

### 2.2 结果

完整 60 格 A 部结果在 `tables/runtime_matrix.tsv`；未经整理输出分别在 `code/commands/023_x86_matrix.log` 和 `code/commands/029_board_matrix_retry.log`。

| 架构 | 运行时 | 适用测试结果 | 特记 |
|---|---|---:|---|
| x86_64 | 未补丁 libc++abi | 9/9 退出 0 | forced_unwind4 域外运行也退出 0 |
| x86_64 | R42 libc++abi | 9/9 退出 0 | forced_unwind4 域外运行也退出 0 |
| x86_64 | libstdc++ 对照 | 9/9 退出 0 | forced_unwind4 域外运行也退出 0 |
| armv7l | 未补丁 libc++abi | 9/9 退出 0 | forced_unwind4 域外运行也退出 0 |
| armv7l | R42 libc++abi | 8/9 退出 0 | `forced_unwind1` 退出 134 |
| armv7l | libstdc++ 对照 | 9/9 退出 0 | forced_unwind4 域外运行也退出 0 |

`forced_unwind1` 的上游测试体确含 catch-all 后重抛。armv7l + R42 格打印 `libc++abi: terminating`，退出 134；其未补丁与 libstdc++ 对照退出 0。因此上游测试实际复现 B1。其他适用测试中没有新增非零退出。

## 3. B 部静态复核

### 3.1 libstdc++ 计数

完整全文命中在 `code/commands/010_libstdcxx_forced_unwind_all.log`；机器可读的 65 行代码防护表在 `tables/libstdcxx_guards_65.tsv`，文件分布在 `tables/libstdcxx_guard_distribution.tsv`。

实测口径：匹配代码中的 `catch`/`__catch`，捕获 `__cxxabiv1::__forced_unwind&`，并排除文档。结果为 65 个防护子句、16 个代码文件，与评审值一致。整个 `libstdc++-v3` 树的 116 个文本命中分类为：防护 65、其他代码用途 41、注释 5、文档 4、类型定义/声明 1；因此全文命中数不能直接作为防护分子。逐命中分类见 `tables/libstdcxx_all_forced_unwind_hits.tsv`。

### 3.2 libc++ 对应实现

`codes/llvm/libcxx/include/` 与 `src/` 的大小写无关检索对 `forced_unwind` 为零命中，独立零命中断言见 `code/commands/053_libcxx_forced_unwind_zero_check.log`；catch-all 上下文全集保留在 `code/commands/011_libcxx_forced_unwind_all.log`。

65 行表逐行给出对应设施与 libc++ 位置。这里的“对应”是功能路径的对应，不声称两个实现逐函数一一同构；同一 libc++ catch-all 会覆盖多处 libstdc++ 模板实例。主要实测形态为：

- ostream/istream/iomanip：catch-all 后调用 badbit helper；
- future：catch-all 后保存 `current_exception()`，部分路径按异常掩码重抛；
- string `shrink_to_fit`：catch-all 后返回；
- condition_variable_any：libc++ 采用 `noexcept` unlock guard 直接 relock，没有同形 catch；
- TR2/experimental 扩展若无直接 libc++ 对应物，表中明确标 `NO_DIRECT_LIBCXX_EQUIVALENT`。

`ios_base::__set_badbit_and_consider_rethrow()` 位于 `codes/llvm/libcxx/src/ios.cpp:362`：先置 badbit，仅当 `exceptions() & badbit` 非零才执行 `throw;`。默认构造状态的 exception mask 为 `goodbit`，故默认配置不会由该 helper 重抛。原文集中在 `code/commands/031_libcxx_correspondence_reference.log`。

## 4. B 部运行时实测

测试件使用裸 `pthread_create`，线程函数非 `noexcept`，应用源码无 catch，也未使用 `std::thread`。红线静态核验见 `code/commands/013_test_source_redline_check.log`。完整源码为 `code/src/stdlib_cancel_probe.cpp`。

| 场景 | x86 libc++ 未补丁/R42 | x86 libstdc++ | arm libc++ 未补丁/R42 | arm libstdc++ | 观测结论 |
|---|---|---|---|---|---|
| cout | 134 / 134 | 0，已取消 | 134 / 134 | 0，已取消 | 两架构确认 |
| 阻塞 getline | 134 / 134 | 0，已取消 | 134 / 134 | 0，已取消 | 两架构确认 |
| packaged_task | 134 / 134 | 0，已取消 | 134 / 134 | 0，已取消 | 两架构确认 |
| shrink_to_fit | 0 / 0 | 0 | 0 / 0 | 0 | 内部 catch 路径 `NOT_OBSERVED` |
| 纯计算 + testcancel | 0 / 0 | 0 | 0 / 0 | 0 | 取消机制对照通过 |

前三类 libc++ 失败格均打印 `FATAL: exception not rethrown`。对应 libstdc++ 格记录 `JOIN_IS_CANCELED=1`。R42 与未补丁 libc++abi 的三类结果逐格相同，因此 R42 四处改动未覆盖这些 libc++ 标准库内部 catch-all。

第一次板端矩阵因板上没有 `timeout` 命令而全部返回 127，完整失败留痕保存在 `code/commands/027_board_matrix.log`，未纳入结论。修正仅改变执行载体：由宿主 `timeout` 包住 SDB 调用，测试件、参数和被测库未改；有效矩阵为 `code/commands/029_board_matrix_retry.log`。

## 5. 消费面近似

输入为 T1 已解包成功语料与 R32 已有线程/取消分类；没有重新下载平台仓。输入 SHA256、最终流式扫描命令和退出码见 `code/commands/049_estimate_consumers_streaming.log` 与 `code/commands/039_input_identities.log`。两次被替换的低效实现保留在失败留痕中，不计入结论。

- 分母：371 个 `EXTRACTED_OK` 源码包。
- 设施使用近似：在源码扩展名文件中检索 iostream/istream/ostream、future、string、iomanip、condition_variable、bitset 及相关扩展的 include 或 `std::` token。
- 线程/取消迹象：与 R32 已落盘的 `pthread_cancel`、`pthread_create`、`pthread_setcancelstate`、`pthread_testcancel`、`std::thread` 等位置计数取交集。

实测结果为：设施使用包 `329/371`；其中同时有线程/取消迹象 `147/329`。逐包结果在 `tables/facility_consumers.tsv`，分设施汇总在 `tables/facility_consumer_summary.tsv`。

该数字是源码 token 近似，不是精确运行时影响面。头文件模板是否实例化、是否走到内部 catch、取消是否恰在该帧上发生，均无法由该扫描确定；由其他包发起取消的线程也可能无法从本包源码识别。

## 6. 覆盖边界与失败留痕

- 覆盖 x86_64/Itanium-DWARF 宿主原生和 armv7l/ARM EHABI 真机；未使用模拟器。
- 未覆盖 aarch64；`forced_unwind4` 恰要求 aarch64，因此本轮对它的两个架构结果只作信息记录。
- 没有修改平台源码、libc++ 或 libc++abi；唯一源码改动是测试副本的 ARM typedef 语法适配和新建的独立测试件。
- `shrink_to_fit` 的内部吞异常路径未被观测，不以退出 0 推断其安全性。
- 板端无 `timeout` 的第一次无效矩阵、两次早期 arm 链接尝试和首次消费扫描忽略 `tmp/` 的零结果均保留，不覆盖失败证据。

本报告不实现修法、不评价方案、不提出推荐。
