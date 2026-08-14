# R34-B 代码审阅材料

本目录完整收录 R34-B 实验实现、验证用例、构建配置、命令与原始输出。
所有 libc++abi 改动仅存在于 `tmp/R34B/llvm-exp/` 实验副本；平台源码
`codes/llvm` 未修改。

## 基线身份

- 平台源码：`codes/llvm` 的 `tizen_base` 分支，HEAD
  `5ed6c77278dfa7a470667cf1a137723d3c96fe60`。
- 上游版本：`upstream/22.1.8` peeled commit
  `ca7933e47d3a3451d81e72ac174dcb5aa28b59d1`。
- 数值版本：22.1.8。
- 原始与实验文件 SHA256 见
  `../tables/input_identity.tsv`；原始命令与输出见
  `../commands/006_input_identity_and_baseline.log`。

## 内容索引

- `implementation/baseline/`：三个被改文件的平台原始完整内容。
- `implementation/current/`：三个被改文件的实验版完整当前内容。
- `implementation/s1_complete_vs_platform.diff`：S1 完整改动相对平台
  基线的 diff。
- `implementation/r34b_cxa_rethrow_only.diff`：本任务新增 rethrow 改动。
- `implementation/rejected_attempt1_*.diff`：构建前由作用域断言拦截并
  已恢复的错误机械补丁，保留用于审计。
- `usecase/src/s1_rethrow_probe.cpp`：六种验证形态的完整用例源码。
- `usecase/matrix/`：两套标准库、六格用例的完整 stdout/stderr、
  命令与退出码。
- `build/tools/`：编译器包装器、受限构建监控脚本及矩阵运行脚本。
- `build/config/libcxx-llvm22.map`：实验共享库链接所用版本脚本。
- `build/cmake/CMakeCache.txt`：实验构建的完整 CMake 配置缓存。
- `reference/`：宏查证、基线身份、配置、构建、链接、运行、运行时
  库身份、EHABI 边界及矩阵比较的原始命令与输出。
- `INDEX.tsv`：逐文件来源、上传路径与 SHA256。

## 运行时身份

`reference/017_runtime_identity.log` 中目标 loader 的 `--list` 与
`LD_DEBUG=libs` 原文均显示 libc++ 用例实际加载实验版
`tmp/R34B/runtime/libc++abi.so.1`，而非系统版 libc++abi。

本目录仅供人工审阅；在审阅完成前，不据此向 armv7l、LLVM libunwind
或其他配置扩展结论。
