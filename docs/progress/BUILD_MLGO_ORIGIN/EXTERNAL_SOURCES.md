# 外部出处

本目录中的 `upstream/` 保存了以下 LLVM 官方文件在 `llvmorg-22.1.8` tag
上的逐字节副本。tag 对象为 `e013073558445169e8732e25fa86e9913bfdd24e`，
解引用后的提交为 `ca7933e47d3a3451d81e72ac174dcb5aa28b59d1`。

- [LLVM 22.1.8 的 MLGO 文档](https://github.com/llvm/llvm-project/blob/llvmorg-22.1.8/llvm/docs/MLGO.rst#L685-L722)：说明 release mode、`TENSORFLOW_AOT_PATH`，并明确写明当前支持 TF 2.15。
- [LLVM 22.1.8 顶层 CMake](https://github.com/llvm/llvm-project/blob/llvmorg-22.1.8/llvm/CMakeLists.txt#L1174-L1213)：上游原有的 TensorFlow AOT 发现、runtime target、模型覆盖入口。
- [LLVM 22.1.8 TensorFlowCompile.cmake](https://github.com/llvm/llvm-project/blob/llvmorg-22.1.8/llvm/cmake/modules/TensorFlowCompile.cmake#L41-L117)：上游原有的 `saved_model_cli aot_compile_cpu` 调用及预生成模型头/对象覆盖机制。
- [LLVM 22.1.8 Analysis CMake](https://github.com/llvm/llvm-project/blob/llvmorg-22.1.8/llvm/lib/Analysis/CMakeLists.txt#L1-L24)：inliner 模型的 AOT 构建入口。
- [LLVM 22.1.8 MLInlineAdvisor.cpp](https://github.com/llvm/llvm-project/blob/llvmorg-22.1.8/llvm/lib/Analysis/MLInlineAdvisor.cpp#L67-L97)：生成的 `InlinerSizeModel` 的直接 C++ 消费点。
- [LLVM 22.1.8 ReleaseModeModelRunner.h](https://github.com/llvm/llvm-project/blob/llvmorg-22.1.8/llvm/include/llvm/Analysis/ReleaseModeModelRunner.h#L54-L137)：生成模型类必须满足的调用契约。
- [LLVM 22.1.8 CodeGen CMake](https://github.com/llvm/llvm-project/blob/llvmorg-22.1.8/llvm/lib/CodeGen/CMakeLists.txt#L1-L23)：regalloc eviction 模型的 AOT 构建入口。
- [LLVM 22.1.8 MLRegAllocEvictAdvisor.cpp](https://github.com/llvm/llvm-project/blob/llvmorg-22.1.8/llvm/lib/CodeGen/MLRegAllocEvictAdvisor.cpp#L51-L57)：生成的 `RegAllocEvictModel` 的直接 C++ 消费点。

取得命令、HTTP/Git 结果及 SHA256 见 `raw/010_*`、`raw/011_*`、
`raw/012_*`、`raw/015_*` 和 `raw/025_*`。
