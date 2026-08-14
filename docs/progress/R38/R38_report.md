# R38：armv7l S1 验证重跑（补齐对照组）

## 结论

结论为 `S1_PARTIAL_ARMV7L`。

本轮在 192.168.108.26 真机 armv7l 上完成了 13/13 个请求格：R34-B 同构的 6 个实验版 libc++abi 格、同板 libstdc++ 的 6 个对照格，以及未打补丁 libc++abi 的 cancel 对照格。单格失败没有中断矩阵。

- 同板 libstdc++ 对照 6/6 退出 0。带 `catch (__forced_unwind&) { throw; }` 的 cancel 与 exit 均命中 `forced-handler`，分别得到 `PTHREAD_CANCELED` 与 `0x2a`。因此该板上的 libstdc++ 防护代码正常工作。
- 未打补丁 libc++abi 的 cancel 对照退出 134（SIGABRT），依次打印 `cleanup:cancel`、`cancel:catch-all`、`FATAL: exception not rethrown`。原始 134 已在同板复现。
- 沿用 R34-B 三处改动的实验版 libc++abi：cancel 与 exit 均未命中 `forced-handler`，而是进入 `catch-all` 后退出 134；real、foreign、cancel-no-catch、exit-no-catch 四格均退出 0。
- 因 forced handler 未命中，R34-B 的 EHABI 类型映射在本轮运行路径上没有产生预期匹配；foreign `__cxa_rethrow` 的 `_Unwind_Resume_or_Rethrow` 改动没有被 cancel/exit 的 forced handler 路径触发。本任务未加入调试性第四处改动，因此内部 personality 状态为 `NOT_OBSERVED`。

以上只描述观测，不推断修法，也不作方案推荐。

## 1. 环境基线

板上 `/lib/libgcc_s.so.1` 属于 `libgcc-14.2.0-1.14.armv7l`，SHA256 为 `0eba387a1e9bbed6ecc903e426a84ed3aa4b7f848f547d19b08d151072c19335`。其动态符号实测包含 `_Unwind_Resume_or_Rethrow@@GCC_3.3`、`_Unwind_VRS_Get@@GCC_3.5`、`_Unwind_VRS_Set@@GCC_3.5` 和 `__gnu_unwind_frame@@GCC_3.5`。板端无 `readelf`，故只读拉取该库后在宿主执行 readelf；命令、退出码和完整输出见 `commands/003-006`。

R34-B x86_64 构建根的 `/usr/lib64/libgcc_s.so.1` 属于 `libgcc-14.2.0-1.13.x86_64`，SHA256 为 `cbfab9aea05310c23d5c5dc897d522cada479beb814bf1d2c61d249f0252c6fd`，同样导出 `_Unwind_Resume_or_Rethrow@@GCC_3.3`。

两者 GCC 数值版本同为 14.2.0，但 Release 分别为 1.14 与 1.13，架构也不同，故版本并非完全一致。若 ARM 行为与 x86_64 不同，本任务无法把架构差异与 libgcc_s Release 差异拆开。完整对照见 `tables/environment_baseline.tsv`。

板上 libstdc++ 为 `libstdc++-14.2.0-1.14.armv7l`，路径 `/lib/libstdc++.so.6`，可用于同板对照。

## 2. 资源门禁与构建

资源门禁为独立可执行脚本 `tools/resource_gate.sh`。`bash -n` 与一次干跑均退出 0；每个配置、构建和测试件编译动作前均独立调用，并在调用点显式检查非零退出码。全部门禁通过：20 CPU、可用内存约 18 GiB、开工时 load1 低于 10、构建分区余量约 470 GiB。

构建全程设置 `CMAKE_BUILD_PARALLEL_LEVEL=2`、`LLVM_PARALLEL_LINK_JOBS=1`，命令以 `nice -n 15 ionice -c 3` 启动，构建参数显式 `-j2`。未打补丁 libc++abi 和静态 libc++ 均在 5 分钟内完成，监控留下开始/结束采样；没有触发 2 GiB 资源红线。资源及构建原文见 `commands/008-030`。

实验版 libc++abi 复用 R36 已构建且 SHA 匹配的 R34-B 三处改动产物，未重复构建；未打补丁对照由 `codes/llvm@5ed6c77278dfa7a470667cf1a137723d3c96fe60` 的干净基线新建，SHA256 分别为：

- 实验版：`21a7bae558296c19ac3e9762c26ac6bd3dc0e218a7f7be245ec3fb4b8b1cb598`；
- 未打补丁版：`366660b64d2e4d9f1f57162b561c13e883126e33c4ae092a1b17f3fd58380cd0`。

## 3. 用例同构性

用例逐字使用 `progress/R34B/src/s1_rethrow_probe.cpp`，SHA256 为 `2e04b02c7a2b307d84faa482d6bb594e54b7e55e3cfd04e1f475e13ffb703cf3`。真实异常格仍为 `std::runtime_error("r34b-real")`，没有使用 R36 的自定义 `RealException`。

为避免板上没有 `libc++.so` 导致替换测试语义，本轮把 libc++ 静态链接进测试二进制，libc++abi 仍动态隔离加载。`ldd` 和 `LD_DEBUG=libs` 直接证明实验/未打补丁格分别加载 `/var/tmp/r38_s1_20260814/patched/libc++abi.so.1` 与 `/var/tmp/r38_s1_20260814/unpatched/libc++abi.so.1`；libstdc++ 格加载 `/lib/libstdc++.so.6`，三者均加载 `/lib/libgcc_s.so.1`。见 `commands/038_board_runtime_identity.log`。

R34-B foreign 格源码读取 Itanium `_Unwind_Exception.private_1`，而 ARM EHABI `_Unwind_Control_Block` 没有该字段。首次逐字编译按预期失败并留痕；最终保持源文件逐字不变，在头文件完成后仅将该字段访问映射到 `unwinder_cache.reserved1`。因此 cancel、exit、real 和两项 no-catch 为严格同构；foreign 的控制流与判据相同，但含目标结构布局适配，不作无条件的“逐字构建上下文同构”申报。详见 `tables/source_equivalence.tsv` 与 `commands/019-030`。

## 4. 验证矩阵

板端没有 `timeout` 命令（实测查找退出 1），矩阵通过直接 SDB shell 逐格执行。每格测试命令、stdout/stderr、被测退出码与信号、SDB 载体退出码均逐字记录；13 次 SDB 载体均退出 0。完整原文见 `commands/040_board_matrix.log`，结构化结果见 `tables/matrix_results.tsv`。

实验版 libc++abi 与 R34-B x86_64 的差异只有两格：

- cancel：x86_64 为 `forced-handler -> join=PTHREAD_CANCELED`、退出 0；armv7l 为 `catch-all -> FATAL`、退出 134；
- exit：x86_64 为 `forced-handler -> join=0x2a`、退出 0；armv7l 为 `catch-all -> FATAL`、退出 134。

real、foreign、cancel-no-catch、exit-no-catch 四格与 R34-B x86_64 的关键输出和退出码一致。libstdc++ 六格与 R34-B 对应结果一致。

## 5. 三处改动的生效阶段

仅按本轮可观测输出：

1. `__forced_unwind` 类型定义存在，测试件和实验库能够构建、装载；这不等同于证明 handler 已匹配。
2. cancel/exit 没有打印 `forced-handler`，而打印 `catch-all`，故预期的 forced-foreign 类型映射/匹配在本轮 ARM 运行路径上未生效。
3. 因 forced handler 未进入，该 handler 中的 `throw;` 没有执行，所以外来异常 `__cxa_rethrow -> _Unwind_Resume_or_Rethrow` 修订没有在这两格中被触发；不能用这两格评价它在 EHABI 下的内部续展行为。

结构化申报见 `tables/change_effectiveness.tsv`。这与 R36 报告的单格 `forced-handler` 输出不同；本轮严格使用 R34-B 原始用例并有完整同板对照，事实差异予以保留，不在本任务中推断原因。

## 6. 清理

开工前 PID 9448/9450 均不存在，未发现本项目遗留进程。测试结束后只删除 `/var/tmp/r38_s1_20260814` 下本任务部署物；文件、两个子目录和根目录删除均退出 0。独立复核得到 `DEPLOY_PATH_PRESENT=NO`，随后拉取板上完整 `ps -ef` 并以精确测试路径/进程名匹配，零命中。见 `commands/041-045` 与 `tables/cleanup_verification.tsv`。

## 7. 结论边界

- `S1_PARTIAL_ARMV7L`：完整矩阵已执行，但实验版 libc++abi 的 forced handler 在 cancel/exit 两格未命中。
- 同板 libstdc++ 防护行为正常，排除了“这块板上防护代码普遍不可用”。
- 未打补丁 libc++abi 复现 134，确认板上能复现原问题。
- 板端与 R34-B 宿主的 GCC 主版本一致，Release 与架构不同；本任务不能拆分这两个变量。
- 本任务没有实现第四处改动，没有修改平台包源码，没有推送外部源码仓，也不提出修法或方案建议。
