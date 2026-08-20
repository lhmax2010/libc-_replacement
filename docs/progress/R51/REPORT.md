# R51：层 A + 层 B 产品化实测报告

## 结论

结论为 `PRODUCTION_READY`，口径限定为本任务已执行的 x86_64 原生与 armv7l 真机、`libgcc_s` unwinder、下列回归用例和上游测试。

- 两个 `git format-patch` 已按层拆分；层 A 为 2 个文件、15 行新增/1 行删除，层 B 为 3 个文件、27 行新增。
- 两个补丁分别单独应用于上游基线后，x86_64 `cxxabi_shared` 均独立构建成功。层 B 的“可独立构建”不改变其在 armv7l 上的功能依赖：实际使用须位于层 A 之后。
- 剥离后的合并版本在 x86_64 与 armv7l 均构建成功；最终 ELF 中 `R40_*`、`R41_*`、`R42_*` 诊断字符串实测零命中。
- 产品版回归矩阵 18/18 格退出 0；每格退出码、join 值、析构与 handler 输出顺序均与同架构 libstdc++ 对照一致，也与剥离前 R45 实验版一致。
- 上游测试 `forced_unwind{1,2,3,4}` 与 `unwind_0{1..6}`：两架构产品版 20/20 通过；其中 armv7l `forced_unwind1` 退出 0。
- 未打补丁的 cancel 对照在两架构均复现退出 134 / SIGABRT 与 `FATAL: exception not rethrown`。
- 板端部署目录、传输归档与相关进程均完成清理核验；最终计数为 0。

## 基线与只读边界

- 上游源码树：`codes/llvm`。
- commit：`5ed6c77278dfa7a470667cf1a137723d3c96fe60`。
- describe：`upstream/22.1.8-9-g5ed6c77278df`。
- 基线文件 SHA256：
  - `libcxxabi/include/cxxabi.h`：`3c2ca8786f9419e1151a5fe92b8dbdcdadf0bde3011e7154d60b172b67ff4ebc`
  - `libcxxabi/src/cxa_exception.cpp`：`1feb758c547d8ddc2f4f3a08fe9fde586bba55649dd30e13191659d807093e19`
  - `libcxxabi/src/cxa_personality.cpp`：`55e136184adbaeee078a339afde2d6da103f4af7cb7c5c31926f2911127eff03`
- `codes/` 未修改；任务结束前再次计算的三份 SHA256 与上述值一致。

## 资源门禁

`tools/resource_gate.sh --level medium` 首次调用退出 0，随后才执行源码盘点、构建与板端 preflight。构建显式使用 `CMAKE_BUILD_PARALLEL_LEVEL=2`、`LLVM_PARALLEL_LINK_JOBS=1`、`nice -n 15`、`ionice -c 3` 和 `-j2`。每次构建均有资源采样；未观测到低于 1 GiB 的情况。

原文：`raw/resources/001_resource_gate_medium.log`、`raw/resources/*/resource_samples.log`。

## 诊断代码剥离

R45 实验副本中的诊断点完整登记见 `tables/diagnostic_inventory.tsv`。剥离内容包括：

- `LIBCXXABI_R42_TRACE` 宏、`stdio.h` 观测依赖及全部 `R42_TRACE` 调用；
- `R40_RETHROW_STATE`、`R40_RESUME_OR_RETHROW_RETURN`、personality 入口/返回/扫描/cleanup 登记日志；
- 仅为观测引入的 `r40_raw_state`、`r40_rethrow_result`；
- 所有以 `R40_`、`R41_`、`R42_` 命名的诊断字符串。

剥离后只保留四处功能改动。有效重建日志中有 62 条编译器警告，全部为既有 wrapper/configuration 参数 `--rtlib=libgcc` 或 `--ld-path=...` 在编译阶段未使用；没有警告指向三份被修改源码，也没有编译 error。原文见 `commands/019_rebuild_after_correct_apply.log` 与 `commands/031_warning_abi_and_patch_summary.log`。

## 补丁拆分

### 层 A：缺陷修复

文件：`patches/0001-Layer-A-libc-abi-preserve-foreign-forced-unwind-stat.patch`  
SHA256：`9017093cca625662299c18e129e41d5fd43ecbae986a464aa286efb391549f20`

- `cxa_exception.cpp`：`__cxa_rethrow` 对 native exception 保持 `_Unwind_RaiseException`，对 foreign exception 改用 `_Unwind_Resume_or_Rethrow`；SJLJ 分支不变。
- `cxa_personality.cpp`：ARM EHABI 强制展开 cleanup phase 中，`ttypeIndex != 0` 的 catching handler 直接安装上下文，不进入 `__cxa_begin_cleanup`。
- 两处作为同一系列：仅有重抛改动会在 ARM 暴露 handler 被误登记为 cleanup 的回归。

### 层 B：GNU 扩展语义

文件：`patches/0002-Layer-B-libc-abi-expose-GNU-forced-unwind-marker-typ.patch`  
SHA256：`10cc7e989f40cb176b8cc9110cc1244db2788eb3028be7bd0606e30ba61138f1`

- `cxxabi.h`：增加与 libstdc++ 同形态的抽象 `__forced_unwind`，成员为私有 out-of-line 虚析构与纯虚 `__pure_dummy()`。
- `cxa_exception.cpp`：定义 out-of-line 虚析构，形成 key function。
- `cxa_personality.cpp`：foreign forced unwind 的 typed handler 匹配映射至该类型。
- 层 B 在 armv7l 上依赖层 A，因为匹配后的防护 handler 通过重抛继续强制展开。

两个补丁的正文均包含英文问题描述、根因、改法、依赖、验证范围与未覆盖范围，并附中文说明；没有写入本项目内部任务编号。

## 拆分与构建验证

| 对象 | 架构 | 结果 | 说明 |
|---|---|---:|---|
| 仅层 A | x86_64 | PASS | 独立应用并完成 `cxxabi_shared` 构建 |
| 仅层 B | x86_64 | PASS | 独立应用并完成 `cxxabi_shared` 构建；不代表功能上不依赖层 A |
| 合并产品版 | x86_64 | PASS | runtimes-only；另以平台版本脚本重链 |
| 合并产品版 | armv7l | PASS | runtimes-only，ARM EHABI |

最终产物：

- x86_64、带 `LLVM_22` 版本节点：`f4f1c05aa7c7d73c2f5290e903255a809139c38d3121ee9f8d91ba766f81b591`
- armv7l：`44672c2c43506954efc28ea23a150a7eb7dcca5e5dd5634f2628f349cb78c04d`

构建时曾发现一次本任务搭建问题：最初对无独立 `.git` 的临时目录使用 `git apply -C`，Git 将补丁报告为 `Skipped patch` 却返回 0，导致首轮构建实为上游基线。该轮产物全部作废；随后从项目根使用显式 `--directory=tmp/R51/...`，先核验功能标记实际存在，再重新完成四次构建。原始证据见 `commands/011_prepare_source_variants.log`、`commands/018_correct_patch_application_directory.log` 与 `commands/019_rebuild_after_correct_apply.log`。

## 回归矩阵

产品版每架构 9 格：cancel、exit、真实 `std::runtime_error`、非强制展开 foreign exception、cancel/exit 无 catch、cancel 嵌套 handler+后续 cleanup、cancel/exit catch-all 重抛。两架构共 18 格。

- 产品版：18/18 退出 0。
- 产品版与 libstdc++：18/18 的退出码、join 值和应用可见输出顺序均 `MATCH`。
- 产品版与剥离前 R45：18/18 的退出码、join 值和应用可见输出顺序均 `MATCH`。
- `cancel`/`cancel-catchall-rethrow` 的 join 均为 `PTHREAD_CANCELED`；`exit`/`exit-catchall-rethrow` 的 join 均为 `0x2a`。
- 嵌套格两架构均依次观测到原始 cleanup、forced handler、rethrow、handler 内两层 cleanup，随后 `PTHREAD_CANCELED`。
- 消费者编译与链接命令中 `--export-dynamic` 实测零命中。

逐格表：`tables/regression_matrix.tsv`；完整顺序对照：`tables/sequence_comparison.tsv`；未经整理的 stdout/stderr/退出码：`raw/x86_matrices.log` 与 `raw/armv7l_board_matrices.log`。

## 上游测试

测试集合为 `forced_unwind{1,2,3,4}.pass.cpp` 与 `unwind_0{1..6}.pass.cpp`。产品版两架构共 20/20 通过；armv7l `forced_unwind1` 退出 0。ARM 只沿用既有的类型写法适配，没有改变测试逻辑。

逐项结果见 `tables/upstream_test_matrix.tsv`，完整源码和编译命令见 `code/tests/` 与 `commands/021_build_test_binaries_final.log`。

## 剥离前后行为

明确结论：在本任务覆盖的 18 个回归格和 20 个产品版上游测试格内，剥离诊断代码没有改变任何已观测行为。应用输出序列、退出码、信号、join 值均无差异。未将诊断行本身纳入“行为一致”判据。

## ABI 变化

有 ABI 表面扩展与既有函数行为修正：

- 新增导出的 `__forced_unwind` 析构、vtable、typeinfo 与 typeinfo-name；x86_64 平台重链产物带 `LLVM_22` 节点，armv7l 构建产物为当前构建口径下的无版本导出。
- 没有删除或改名任何既有导出符号，没有改变 SONAME。
- 既有 `__cxa_rethrow` 的函数签名不变，但 foreign exception 的运行时行为改变。
- personality 的既有导出签名不变，但 foreign forced unwind 与 EHABI catching-handler 路径行为改变。

符号原文见 `commands/031_warning_abi_and_patch_summary.log`。

## 运行时身份与板端清理

- x86_64 以显式 loader `--library-path` 运行，并由 loader `--list` 与 `LD_DEBUG=libs` 记录实际加载的产品版 libc++abi。
- armv7l 在 `192.168.108.26:26101` 真机原生执行，无 qemu；板上 `ldd`、`LD_DEBUG=libs` 与 SHA256 均已落盘。
- 板端部署路径为 `/var/tmp/r51_20260820`，传输归档为 `/opt/usr/home/owner/share/tmp/r51_board_payload.tar`。收尾时两个路径不存在，相关进程计数为 0。

原文分别位于两份 raw matrix 和 `commands/025_prepare_and_deploy_board.log`、`commands/027_board_cleanup.log`。

## 未覆盖范围

以下均未在本任务观测，不能由本报告补全：

- aarch64；
- LLVM libunwind（本任务使用 libgcc_s）；
- SJLJ 运行时行为；
- 多线程并发取消、重复取消竞态；
- libstdc++/libc++ 混合运行时帧穿越；
- 优化级别、LTO、sanitizer 等额外构建组合；
- 完整 libc++abi 测试套件（本任务只跑指定的 10 个测试）。

## 证据导航

- 补丁与完整被改文件：`code/patches/`、`code/libcxxabi/`
- 测试源码：`code/tests/`
- 完整编译/链接/运行命令与原始输出：`code/raw/`
- CMake 配置：`code/config/`
- 全部任务命令原文和退出码：`commands/`
- 材料索引：`code/INDEX.tsv`
- 全量校验和：`MANIFEST.sha256`
