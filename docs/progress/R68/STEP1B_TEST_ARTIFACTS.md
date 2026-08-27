# R68 第二步开跑前：测试产物成因与身份核验

本阶段只补齐并核验测试运行库；没有启动任何测试组合，也没有连接开发板运行用例。断点仍为“组合 1 / x86_64 打补丁 / 尚未开始”。

## 1. 缺失成因

结论：x86_64 的两个测试安装目录缺少 `libc++.so.1.0` 与
`libc++abi.so.1.0`，是因为此前只完成了配置和 `lit --no-execute`
能力探测，没有执行 `runtimes-test-depends` 构建目标；不是配置差异、
安装目标差异，也不是构建失败。

依据：

- 四个构建目录都启用了 `LIBCXX_INCLUDE_TESTS=ON`、
  `LIBCXXABI_INCLUDE_TESTS=ON`，均为 `RelWithDebInfo`，且
  `LLVM_ENABLE_RUNTIMES=libcxx;libcxxabi`。
- armv7l 打补丁与未打补丁目录分别有明确的
  `cmake --build ... --target runtimes-test-depends -- -j2` 记录，安装
  目录中的两个库随该目标生成。
- x86_64 两目录此前没有同类构建记录，测试安装库目录也不存在；只见
  配置与无执行探测记录。
- 因此两架构的差异来自已执行步骤不同，而非 CMake 安装目标或测试
  配置不同。

原始盘点见 `commands/080_artifact_cause_inventory.log`。

## 2. x86_64 补齐

两组合均使用与 armv7l 相同的目标和资源约束：

```text
env CMAKE_BUILD_PARALLEL_LEVEL=2 LLVM_PARALLEL_LINK_JOBS=1 nice -n 15 ionice -c 3 cmake --build <build-dir> --target runtimes-test-depends -- -j2
```

- 打补丁版：本阶段新构建，初次构建退出 0；随后原命令重复执行为
  up-to-date，退出 0。初次交互输出被工具界面截断，此事实已在命令
  记录中明确登记，重复核验的完整输出已落盘。
- 未打补丁版：本阶段新构建，退出 0，完整输出已落盘。

记录分别见 `commands/081_build_patched_x86_test_dependencies.log`、
`commands/082_build_baseline_x86_test_dependencies.log` 和
`raw/full_run/build_*_x86_test_dependencies*.stdout`。

## 3. 八个运行库的 SHA256

| 架构 | 组合 | 来源 | libc++.so.1.0 | libc++abi.so.1.0 |
|---|---|---|---|---|
| x86_64 | 打补丁 | 本阶段新构建 | `e6cff6f0081747844e3326f762c2e876f16cff73a198f91eba065bd1e35299a0` | `63b5f9946ff47fab9f1ca31b8b63c298a5eda8ed7d656be571bf61190203bd89` |
| x86_64 | 未打补丁 | 本阶段新构建 | `f47953baa9e2521a65a56a76eb885ed2cf514f91263f67c3b8e791e9c0136998` | `ae10890d42ca1036deec181cfb015d4ac8b31a27db6b36421e9eb904430f8405` |
| armv7l | 打补丁 | 复用第一步 `commands/048` 构建 | `747a61aa6d075229b49771fa42b1110edc983b7273874e75fe0d3dbde25e93ed` | `f063f695f510a5fd23bc80547e07bf988401a3be6048b07b69b51a637f143b26` |
| armv7l | 未打补丁 | 复用第一步 `commands/063` 构建 | `d293b1a3bed8a4114b5a7c30d3732a4299a52221fd7cc7d5aada9a369899a737` | `58b8bcc8ffe95b78bb1a3d5951efe9507ddf44400fe3e6ad8debeed09a93b374` |

ELF 原文确认 x86_64 四件均为 64-bit x86-64，armv7l 四件均为
32-bit ARM EABI5。完整 `sha256sum`、`file` 与四个
`libc++abi.so.1.0` 的 `readelf -Ws` 输出见
`raw/full_run/test_artifact_elf_identity.retry.stdout`。

## 4. 补丁组合身份核验

身份检查退出 0，最终输出 `RESULT\tPASS`。检查同时成立的判据为：

1. 打补丁源码树为干净的
   `ae3471c26c66489439354bb1e89df6af8b56da3e`；未打补丁源码树为干净的
   `5ed6c77278dfa7a470667cf1a137723d3c96fe60`。
2. 两源码树的九个被改文件分别逐字节匹配 R69
   `baseline_and_final_sha256.tsv` 的基线列与最终列。
3. x86_64 与 armv7l 的打补丁构建均以 R69 最终树为
   `CMAKE_HOME_DIRECTORY`；两套未打补丁构建均以基线树为源。
4. 四个测试安装目录内的五个相关公开头文件逐字节匹配各自源树：
   `basic_ostream.h`、`future`、`istream`、`string`、`cxxabi.h`。
5. 两架构的打补丁 `libc++abi` 均含强制展开类型及
   `_Unwind_Resume_or_Rethrow` 标记；两架构基线均不含这些补丁标记。
6. 每个架构内，打补丁与未打补丁的 `libc++`、`libc++abi` 哈希均
   不同。

检查源码为 `tools/verify_test_artifact_identity.py`，完整结果为
`raw/full_run/test_artifact_identity.retry.stdout`。

据此，armv7l 的四个产物不只是“存在且可读”：其 CMake 来源、安装
头文件、被改源码哈希、ELF 架构和 ABI 特征均与预期补丁组合一致。

## 5. 技术性非零记录

在身份核验阶段有三次不代表判据失败的技术性非零，均完整保留：

- 身份脚本首次直接执行因缺少可执行位返回 126；补可执行位后同一
  检查退出 0。
- ELF 证据命令先后两次把测试安装根误写为构建目录直接下的
  `test-suite-install`，实际路径为 `libcxx/test-suite-install`，两次
  均返回 1；按身份脚本中的确切路径重采后退出 0。

这些失败发生在检查逻辑执行前或附加证据路径解析阶段，没有改变构建
产物，也没有掩盖身份判据。对应记录为 `commands/084`、`087`、
`087a` 及最终成功的 `086`、`088`。其中第二次错误命令未在执行当时
重定向到文件，`087a` 明确标为依据工具调用记录补登，不冒充原始
stdout/stderr。

归档提交前另有一次技术性非零：`git diff --cached --check` 对
`readelf -Ws` 逐字保存的原始输出报出行尾空格并返回 2。提交尚未
发生。该原文不能为通过格式检查而改写；后续对其使用 SHA256 与索引
核验保持逐字节完整，对其余非原始输出材料继续执行 whitespace 检查。
记录见 `commands/093_staged_diff_check_raw_output.log`。

## 6. 当前状态

- x86_64 与 armv7l、打补丁与未打补丁四组合所需运行库已齐备并完成
  身份核验。
- 尚未执行任何全量测试组合。
- 等待人工确认后，才从“组合 1 / x86_64 打补丁 / 尚未开始”继续。
