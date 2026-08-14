# R34-A：S1 最小实现代码与用例审阅材料

日期：2026-08-14

本目录只整理 R33 已存在的 S1 实验实现、验证用例、构建配置、运行原文与对照源码。未重新构建，未修改实现、用例、平台源码或既有制品，未进行根因诊断。

## 目录

- `identity/`：实验源码基线、版本和原始/实验文件 SHA256；
- `implementation/diff/`：相对未修改基线的完整 diff；
- `implementation/current/`：两个改动文件的完整实验版内容，同时提供带行号副本；
- `usecase/src/`：完整验证用例；
- `usecase/commands/`：两个目标的编译/链接脚本原文、构建命令输出及运行矩阵原文；
- `usecase/results/`：六个用例格各自的 stdout、stderr 与退出码；
- `build/configure/`：完整 CMake 配置、build、version-script 重链接材料及关键 Cache 值；
- `build/runtime/`：loader `--list`、`LD_DEBUG=libs` 与运行时文件身份；
- `references/`：libstdc++ 和未修改 libc++abi 的带行号对照片段；
- `expectation_actual.tsv`：每个变体的预期与原始实际结果；
- `INDEX.tsv`：逐文件来源与 SHA256；
- `MANIFEST.sha256`：除自身外全部文件的校验清单。

## 运行时身份核验

`build/runtime/loader_list.log` 和 `ld_debug_libs_real.log` 均显示 `libc++abi.so.1` 实际解析至：

```text
/home/toolchain/development/libc++_replacement/tmp/R33/s1-runtime-r3/libc++abi.so.1
```

该符号链接的实际文件为 `libc++abi.so.1.0`，SHA256 为：

```text
b68f36d44940632ce6604730fef176afbfcb78f1f4a521b1ee19d61755f6bd59
```

以上是 R33 实验版运行时文件；完整命令、搜索路径、init/fini 路径与退出码均保留在 `build/runtime/`。

## 体积边界

策展目录内所有单文件均不超过 5 MB。
