# 外部出处

查证日期：2026-09-08。以下均为可公开访问的项目原始资料；本任务没有以搜索结果摘要替代原始页面。

1. [MLGO 当前 `Pipfile`](https://github.com/google/ml-compiler-opt/blob/main/Pipfile)：当前稳定依赖精确固定 `tf-agents==0.19.0`、`tensorflow==2.15.0`、`dm-reverb==0.14.0`。
2. [MLGO 提交 `8379a896`（Bump python deps）](https://github.com/google/ml-compiler-opt/commit/8379a896aa0a2c5bf7b643929ec5958d670a9665)：从 TensorFlow 2.12 升至 2.15；提交说明明确写出未升至最新版本是因为最后一个 TF-Agents 正式版只与 2.15.0 兼容。
3. [TF-Agents 官方兼容表](https://github.com/tensorflow/agents#releases)：0.19.0 对应 TensorFlow 2.15.0 与 dm-reverb 0.14.0；更高 TensorFlow 版本只列 nightly 配对方式，没有 TF-Agents 2.18 正式配对。
4. [MLGO README](https://github.com/google/ml-compiler-opt/blob/main/README.md)：依赖通过 lockfile 安装，并明确 LLVM 的 release mode 也需要同一个 TensorFlow 包。
5. [LLVM MLGO 文档](https://github.com/llvm/llvm-project/blob/main/llvm/docs/MLGO.rst)：说明 MLGO 的训练/推理模型接口；没有固定 TensorFlow 版本。
6. [LLVM 顶层 CMake](https://github.com/llvm/llvm-project/blob/main/llvm/CMakeLists.txt) 与 [`TensorFlowCompile.cmake`](https://github.com/llvm/llvm-project/blob/main/llvm/cmake/modules/TensorFlowCompile.cmake)：通过 `TENSORFLOW_AOT_PATH` 使用 TensorFlow 安装树，没有版本门禁；生成头、对象和 runtime 是配套输入。
7. TensorFlow 2.15.1 与 2.18.0 的官方源码对照：[`saved_model_cli.py` 2.15.1](https://github.com/tensorflow/tensorflow/blob/v2.15.1/tensorflow/python/tools/saved_model_cli.py)、[`saved_model_cli.py` 2.18.0](https://github.com/tensorflow/tensorflow/blob/v2.18.0/tensorflow/python/tools/saved_model_cli.py)、[`codegen.cc` 2.15.1](https://github.com/tensorflow/tensorflow/blob/v2.15.1/tensorflow/compiler/aot/codegen.cc)、[`codegen.cc` 2.18.0](https://github.com/tensorflow/tensorflow/blob/v2.18.0/tensorflow/compiler/aot/codegen.cc)。本任务保存了逐文件下载副本及 SHA256/差异记录。
8. [TensorFlow 官方源码构建组合表](https://github.com/tensorflow/docs/blob/master/site/en/install/source.md#tested-build-configurations)：2.15 使用 Python 3.9–3.11、Clang 16.0.0、Bazel 6.1.0；2.18 使用 Python 3.9–3.12、Clang 17.0.6、Bazel 6.5.0。
9. [TensorFlow 2.18.0 release note](https://github.com/tensorflow/tensorflow/releases/tag/v2.18.0)：列出的主要改进没有 `tfcompile`、AOT、MLGO 或 libc++ 专项说明。
10. [LLVM issue #60751](https://github.com/llvm/llvm-project/issues/60751)：给出 LLVM MLGO 与 TensorFlow C++ runtime 的真实链接符号，包括带 `std::__cxx11` 的接口，说明 TensorFlow runtime/标准库 ABI 是实际集成边界。

## 未找到的公开资料

在 MLGO 仓的提交历史、issue/讨论检索，以及 LLVM/TensorFlow 的相关 issue 检索中，未找到以下内容：

- MLGO 稳定依赖组后续升到 TensorFlow 2.16、2.17 或 2.18 的提交；
- 明确说明 TensorFlow 2.16–2.18 的 `saved_model_cli aot_compile_cpu` 与 MLGO 不兼容的 issue；
- 明确说明 TensorFlow 2.18 为 MLGO 带来所需性能收益、bug 修复或原生 libc++ 资产生成支持的材料。

以上是“在本次检索范围内未找到”，不是这些资料绝对不存在。检索查询及本地 Git/API 历史记录见 `raw/006_*`、`raw/007_*`、`raw/008_*`；官方页面可能随上游更新，固定提交链接优先于 `main` 链接。
