# R36：S1 完整实现的 armv7l / ARM EHABI 验证

## 结论

结论为 `S1_PARTIAL_ARMV7L`。

实验版 libc++abi 成功构建为 ARM EABI5 共享库并在板上由指定隔离路径实际加载；但矩阵首格 `pthread_cancel + catch (__forced_unwind&) { throw; }` 在真机 armv7l 上命中 forced handler、执行析构后，重抛仍打印 `libc++abi: terminating` 并退出 134。矩阵按 fail-closed 在首格停止，余格均未执行，不能把 x86_64 的 `S1_COMPLETE` 扩展到 ARM EHABI。

另有一项流程偏差：开工前资源数据已落盘（20 CPU、18 GiB available、1 分钟负载 1.38，客观上满足阈值），但门禁脚本因 awk 引号错误退出 1；执行者未在该工具退出后停止，仍进行了构建。`040_resource_gate_parser_correction_post_build.log` 只复核解析方法和事后状态，不冒充开工前门禁。本报告将其登记为 `PROCESS_DEVIATION_RESOURCE_GATE_SCRIPT_EXIT_1`。

## 板卡身份与执行载体

- SDB：`192.168.108.26:26101`，设备名 `rpi4`。
- 实测：armv7l，Tizen 11.0 Unified，build id `tizen-unified-toolchain_20260728.012216_tizen-headed-armv7l`。
- 新板与旧 `.25` 的 kernel/rpi4 信息一致，但旧证据没有 machine-id/build-id，故物理板身份为 `UNDETERMINED`，未宣称同一块板。
- 因而重新核验 R22 的候选路径。当前非 root SDB 会话下 `/var/tmp` 与 `/opt/var/tmp` 可写、可原生执行且可清理；其余五个列出的候选受权限/Smack 限制。实际部署使用 `/var/tmp/r36_s1_20260814`。
- 测试程序在开发板原生执行，`QEMU_USED_FOR_TEST_EXECUTION=NO`。QEMU 仅在宿主上承载 armv7l 版 clang 22.1.8 交叉编译器。

## EHABI 路径分析

R34-B 的三处改动都编译进 EHABI：

1. `cxxabi.h:41` 的 `__forced_unwind` 类型位于公共头。
2. `cxa_personality.cpp:822-838` 的 foreign forced type 映射属于公共 `scan_eh_tab`。EHABI 专用 personality `cxa_personality.cpp:1191-1297` 在 forced phase 2 的 1265-1268 行以 `_UA_CLEANUP_PHASE | _UA_FORCE_UNWIND` 调用该函数，因此会执行映射。
3. `cxa_exception.cpp:627-659` 的 `__cxa_rethrow` 是公共实现；保留 SJLJ 分支后，EHABI foreign 路径同样调用 `_Unwind_Resume_or_Rethrow`。

libgcc_s 的 ARM 实现以 `unwinder_cache.reserved1` 保存 forced stop function；`_Unwind_Resume_or_Rethrow` 据其是否为零选择普通 raise 或继续 forced phase 2。LLVM libunwind 也把 ARM 的 `PRIVATE_1` 映射到 `reserved1`，但本次构建使用 libgcc_s，未启用 LLVM libunwind。

静态分析没有发现三处改动被 EHABI 条件编译排除，因此允许进入构建。真机首格随后证明：仅有这三处改动仍不足以使 ARM forced rethrow 成功。待人工裁决的专用工作性质是核查/处理 `__cxa_rethrow` foreign 分支到 ARM `_Unwind_Resume_or_Rethrow` 之间的 EHABI forced-state 保留与续展，涉及 `cxa_exception.cpp:627-659`、libgcc `unwind-arm-common.inc:77,649,714-723` 及 EHABI personality 的 1191-1297 行；本任务未实现任何第四处修改，也未推断具体修法。

## 构建

- 源码：R34-B 实验副本，沿用类型定义、personality 映射、foreign rethrow 三处改动。
- 配置：standalone runtimes，`LLVM_ENABLE_RUNTIMES=libcxx;libcxxabi`，只构建 target `cxxabi`；`LIBCXXABI_USE_LLVM_UNWINDER=OFF`，静态 ABI 关闭，共享 ABI 开启。
- 资源约束：`-j2`、`LLVM_PARALLEL_LINK_JOBS=1`、`nice -n 15`、`ionice -c 3`。构建中样本 MemAvailable 19,754,360 KiB，未触发 2 GiB 停止线。
- 结果：1696/1696，退出 0；输出 SHA256 `21a7bae558296c19ac3e9762c26ac6bd3dc0e218a7f7be245ec3fb4b8b1cb598`。
- 输出确认为 ARM EABI5，SONAME `libc++abi.so.1`，DT_NEEDED 含 `libgcc_s.so.1`；UND 含 `_Unwind_Resume_or_Rethrow@GCC_3.3`、`_Unwind_VRS_Get/Set@GCC_3.5`。

## 用例与运行时身份

板上没有 libc++/libc++abi 包或文件，且纪律只允许部署测试二进制与实验版 libc++abi。因此运行探针不依赖 libc++.so：forced unwind、cancel、exit、foreign、no-catch 判据与 R34-B 同构；“真实异常”采用自定义有类型 C++ 异常而非 `std::runtime_error`。这一差异不影响实际执行并红停的 cancel 格。原 R34-B `std::runtime_error` 形态源码也保留在交付材料，但未在板上执行。

`ldd` 和 `LD_DEBUG=libs` 均确认 `s1_libcxxabi` 加载 `/var/tmp/r36_s1_20260814/libc++abi.so.1`；`s1_libstdcxx` 的 ldd 指向系统 `/lib/libstdc++.so.6`。板端 SHA 与宿主逐项 MATCH。

## 矩阵结果与 x86_64 对照

armv7l 首格原文顺序：

```text
cancel:ready
cleanup:cancel
cancel:forced-handler
libc++abi: terminating
RUN_EXIT_CODE=134
MATRIX_FAIL_CLOSED=YES
```

R34-B x86_64 同格为：forced handler 后 `join=PTHREAD_CANCELED`，退出 0。两架构结果明确不同。其余矩阵格和系统未补丁对照格未执行；其中板上系统对照还受“无系统 libc++abi”这一独立缺口限制。完整状态见 `tables/matrix_results.tsv`。

## ABI 与改动范围

本任务没有新增功能改动，实验源码仍只含 R34-B 三处。类型定义会新增 forced-unwind typeinfo/name 导出；personality 映射改变 forced foreign exception 的 handler 匹配；foreign `__cxa_rethrow` 改用 Resume-or-Rethrow。armv7l 结果表明该完整三处实现尚不能宣称成立。

## 清理

板上部署的两个测试二进制、实验版 libc++abi 实体及两个符号链接均逐项 `unlink` 退出 0，目录 `rmdir` 退出 0，`test ! -e` 退出 0。core limit 为 0，core pattern 为 crash-manager 管道，部署目录中未产生 core 文件。未安装 RPM、未修改系统库、未改配置、未重启或启停服务。

## 未观测与不可得

- fail-closed 后的 11 个标准矩阵格：`NOT_OBSERVED`（真实异常的 libc++abi 变体仅在运行时身份检查中额外观察到退出 0，不计为矩阵格）。
- 板上系统未打补丁 libc++abi cancel 对照：`NOT_AVAILABLE`，板上无该运行时，且未获准部署非实验版运行时。
- EHABI foreign forced-state 在 handler/rethrow 边界的实际数值：`NOT_OBSERVED`；本任务没有新增调试功能或修改判据。

本报告仅陈述实测结果，不作方案推荐，不提交上游，不修改平台包。
