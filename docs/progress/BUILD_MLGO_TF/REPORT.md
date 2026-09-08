# MLGO 的 TensorFlow 版本选择：2.15 与 2.18 事实评估

日期：2026-09-08  
项目分支：`codex/r5-ehabi-diagnosis`  
任务边界：只读查证；未修改 `codes/`、平台源码或配置；未向 Gerrit 推送；本文不推荐、不排序两个选择。

## 1. 结论摘要

1. **MLGO 使用 2.15 有明确的官方理由，不只是“当时恰好是这个版本”。** MLGO 当前 `Pipfile` 仍精确固定 `tensorflow==2.15.0`、`tf-agents==0.19.0` 和 `dm-reverb==0.14.0`。把 TensorFlow 从 2.12 升到 2.15 的提交明确说明：没有升至当时最新 TensorFlow，是因为最后一个 TF-Agents 正式版只与 2.15.0 兼容。TF-Agents 自己的兼容表也把 0.19.0、TF 2.15.0、dm-reverb 0.14.0 列为同一组。出处见 [固定提交](https://github.com/google/ml-compiler-opt/commit/8379a896aa0a2c5bf7b643929ec5958d670a9665)、[当前 Pipfile](https://github.com/google/ml-compiler-opt/blob/main/Pipfile) 和 [TF-Agents 兼容表](https://github.com/tensorflow/agents#releases)。
2. 这个理由约束的是 **MLGO 的稳定训练/依赖栈**。本次没有找到“TensorFlow 2.18 的 AOT 编译器已知不能用于 MLGO”的公开 issue。两件事不能混为一谈：没有查到 AOT 已知缺陷，并不消除官方训练栈的版本约束。
3. TensorFlow 2.15.1 与 2.18.0 的 `saved_model_cli aot_compile_cpu` **命令参数基本不变**，但生成包装类与 XLA runtime 的内部契约、源文件集合、归档成员和符号集合已有实质变化。W1 的 2.18 候选不能作为 2.15.1 三包的直接替换品。
4. W1 看到的 inliner 入口符号从带绝对路径的长名变成短名，直接原因是两次生成使用了不同 `output_prefix`；两个 TensorFlow 版本的 Python 代码都按 `__xla_ + output_prefix + __ + cpp_class` 生成入口。**该改名不能单独归因于版本升级。**
5. **开发人员原始 2.18 生成机制不能同时选择 libc++ 与 libstdc++。** 它只下载 GCC/libstdc++ sysroot，按 GCC 安装目录寻找 libstdc++ 头文件，且没有标准库选择参数。W1 中已验证的双路径是本项目后加的改动：默认/显式 libstdc++ 等价，显式 libc++ 生成 `std::__1` runtime；不能把这个能力归于开发人员原版。
6. 2.18 确有更新的受测构建环境，并在本地候选中多出四个 runtime 归档成员；但本次**未观测到**这些变化给当前 inliner 模型带来所需性能、功能或 bug 修复，也未找到 2.18 提供原生 libc++ 支持的证据。

上面同时给出了维持 2.15 与升级 2.18 的决策素材，但不据此选择方案。逐项对比另见 [`COMPARISON.tsv`](COMPARISON.tsv)。

## 2. 查证对象与证据身份

### 2.1 本地对象

| 对象 | revision / 版本 | 用途 |
|---|---|---|
| 平台 LLVM 工作树 | `5ed6c77278dfa7a470667cf1a137723d3c96fe60` | 核对 MLGO 文档、CMake 与 spec 的资产消费方式 |
| 开发人员 2.18 AOT 分支 | `c5c4230a988bec124e83a89d8ffedb8ce07e01ea` | 核对 AOT spec 与编译器选择 |
| 开发人员原始生成器 | `e611ebb4e4dd6d47023f3b06bccc528022aa098f` | 回答原版是否支持双标准库 |
| MLGO 上游只读副本 | `b0114dbc807c109fc1556e806d8d636d535cce88` | 核对当前依赖与完整 Git 历史 |
| 现有平台资产 | 头文件标识 TensorFlow 2.15.1 | 旧资产基准 |
| W1 新候选 | TensorFlow 2.18.0 | 三架构默认 libstdc++、libc++ 与省略开关对照 |

`codes/` 全程只读。命令、标准输出、标准错误和退出码见 `raw/`；下载的 TensorFlow 官方 tag 文件及各自命令/退出码见 `upstream/`。

### 2.2 版本措辞

MLGO 官方当前精确固定的是 **2.15.0**；平台旧资产自报版本是 **2.15.1**。本文“2.15 路线”指维持平台既有的 2.15.1 资产代际，同时说明其与 MLGO 官方稳定依赖在 2.15 主版本上对齐。不能把二者表述为精确版本完全相同。

## 3. MLGO 为什么停在 2.15

### 3.1 LLVM 仓本身没有固定版本

本地 `llvm/docs/MLGO.rst` 说明 release 模式需要 TensorFlow pip 包来做 AOT；`llvm/CMakeLists.txt` 通过 `TENSORFLOW_AOT_PATH` 找 `saved_model_cli`、头文件和 XLA runtime 源码。两处都没有 2.15/2.18 版本判断或兼容矩阵。上游对应文件为 [MLGO.rst](https://github.com/llvm/llvm-project/blob/main/llvm/docs/MLGO.rst)、[CMakeLists.txt](https://github.com/llvm/llvm-project/blob/main/llvm/CMakeLists.txt) 与 [TensorFlowCompile.cmake](https://github.com/llvm/llvm-project/blob/main/llvm/cmake/modules/TensorFlowCompile.cmake)。

因此，“官方固定 2.15”的直接证据不在 LLVM CMake，而在 MLGO 训练仓的 Python 依赖组。

### 3.2 MLGO 稳定依赖组及升级理由

当前上游 `Pipfile`：

```toml
tf-agents = "==0.19.0"
tensorflow = "==2.15.0"
dm-reverb = "==0.14.0"
```

提交 `8379a896aa0a2c5bf7b643929ec5958d670a9665` 把组合从 TF-Agents 0.16 / TensorFlow 2.12 / dm-reverb 0.11 升为上述版本。提交说明明确给出的停止点理由是：最后一个 TF-Agents 正式版只与 TensorFlow 2.15.0 兼容，后续方案仍待解决。TF-Agents 官方表给出相同配对；更高版本只给 `tf-nightly` / `dm-reverb-nightly` 的 nightly 组合。

MLGO README 还明确说 LLVM release 模式需要“同一个 TensorFlow 包”。这使“训练用官方稳定 2.15、release AOT 随意换 2.18”不能在未验证时视为受官方支持的等价组合。[MLGO README](https://github.com/google/ml-compiler-opt/blob/main/README.md)

### 3.3 提交历史与后续情况

- 2023-03-23，`ac84c14...`：从 2.12 nightly 组合切回 TF-Agents 0.16 / TensorFlow 2.12 / dm-reverb 0.11 的正式版组合；提交标题是“Update ... versions out of nightly”。
- 2024-12-19，`8379a896...`：从 2.12 升到 2.15，明确留下 TF-Agents 正式版兼容约束。
- 当前上游 HEAD 的 `Pipfile` 仍为 2.15.0。通过 GitHub API 得到的 `Pipfile` 历史共 8 条；在上述提交之后，没有再改变 TensorFlow 固定版本的提交。

完整本地历史见 `raw/006_mlgo_dependency_history.*`、`raw/007_github_api_pipfile_history.*` 和 `raw/008_parse_pipfile_history.*`。

### 3.4 已知兼容问题检索结论

本次在 MLGO、LLVM 与 TensorFlow 官方仓的 commit/issue 范围内，**未找到**明确指出 TensorFlow 2.16–2.18 的 `saved_model_cli aot_compile_cpu` 与 MLGO 不兼容的记录，也未找到 LLVM 拒绝更高版本的公开记录。

这只是限定检索范围内的“未找到”，不能证明不存在。已找到的官方明确约束是 TF-Agents/训练依赖组合。LLVM issue [#60751](https://github.com/llvm/llvm-project/issues/60751) 则从另一个角度证明：MLGO release 模式真实链接 TensorFlow C++ runtime，而且接口符号会携带标准库 ABI，因此匹配 runtime 和 ABI 不是可忽略的打包细节。

## 4. TensorFlow 2.15.1 与 2.18.0 的差异

### 4.1 AOT 命令入口

对两个官方 tag 的以下文件逐文件下载并做差异核对：

- `tensorflow/python/tools/saved_model_cli.py`
- `tensorflow/python/tools/saved_model_aot_compile.py`
- `tensorflow/compiler/aot/codegen.cc`
- `tensorflow/compiler/aot/codegen.h`
- `tensorflow/compiler/tf2xla/xla_compiled_cpu_function.h`

结果：

- `aot_compile_cpu` 的 `output_prefix`、`signature_def_key`、`target_triple`、`cpp_class`、`multithreading` 等外部参数及 `aot_compile_cpu_meta_graph_def` 函数签名没有变化；
- `saved_model_cli.py` 的 17 行差异不在 AOT 接口；
- `saved_model_aot_compile.py` 只移除了 2.15.1 的一个 `_pywrap_tfcompile` import-error 转发检查；
- `codegen.h` 的被核对 AOT 选项没有变化。

所以，**调用脚本不需要因为 2.18 重写命令行形态**。但这不表示产物契约不变。

证据：`raw/013_tensorflow_aot_api_diff.*` 及 `upstream/` 中的 tag 固定源码。官方链接见 [`saved_model_cli.py` 2.15.1](https://github.com/tensorflow/tensorflow/blob/v2.15.1/tensorflow/python/tools/saved_model_cli.py) 与 [2.18.0](https://github.com/tensorflow/tensorflow/blob/v2.18.0/tensorflow/python/tools/saved_model_cli.py)。

### 4.2 生成包装类与 runtime 契约

2.15.1 的 `codegen.cc` 在生成的 `StaticData` 初始化中写入：

```cpp
set_static_data_use_xla_runtime(data, {{USE_XLA_RUNTIME}});
```

2.18.0 已不再生成该行，也不再替换 `{{USE_XLA_RUNTIME}}`。对应 `XlaCompiledCpuFunction` 中，2.15.1 保存 `use_xla_runtime_` 并有旧 runtime 执行路径；2.18.0 删除该状态和执行路径，仅暂留一个无操作 setter 以兼容部分构建目标：

```cpp
static void set_static_data_use_xla_runtime(StaticData* static_data, bool) {}
```

这说明 AOT 外部命令虽稳定，**生成头与配套 XLA runtime 的内部契约已经演进**。证据见 `raw/013_tensorflow_aot_api_diff.*`、`raw/014_xla_wrapper_runtime_interface_diff.*` 和官方 [`codegen.cc` 2.15.1](https://github.com/tensorflow/tensorflow/blob/v2.15.1/tensorflow/compiler/aot/codegen.cc) / [2.18.0](https://github.com/tensorflow/tensorflow/blob/v2.18.0/tensorflow/compiler/aot/codegen.cc)。

### 4.3 W1 的实际资产差异

同一架构下，旧 2.15.1 三包合并资产与新 2.18.0 默认 libstdc++ 候选为：

| 架构 | 旧文件数 | 新文件数 | 旧→新缺路径 | 新增路径 | 共同非二进制内容不同 | 旧→新定义符号缺失 / 新增 | 旧→新未定义符号缺失 / 新增 |
|---|---:|---:|---:|---:|---:|---:|---:|
| armv7l | 10,866 | 8,393 | 3,548 | 1,075 | 3,488 | 1,516 / 3,506 | 9 / 25 |
| aarch64 | 10,866 | 8,393 | 3,548 | 1,075 | 3,489 | 2,484 / 4,552 | 14 / 25 |
| x86_64 | 10,866 | 8,393 | 3,548 | 1,075 | 3,488 | 2,110 / 7,104 | 22 / 31 |

runtime 静态归档从每架构 30 个成员变为 34 个；新集合至少新增：

```text
convolution_thunk_f16.cc.o
convolution_thunk_f32.cc.o
runtime_single_threaded_matmul_f8.cc.o
runtime_single_threaded_matmul_u8.cc.o
```

这些数字是所有打包文件及去重全局符号的集合差异，**不是 3,506 个“公开接口新增”**。其中混合了 XLA runtime 源码集变化、内部函数变化、依赖实现与 ABI 版本差异。它们足以证明不能直接替换，但不能在未逐符号分类时给每项赋予 API 语义。

证据：`raw/016_w1_asset_evidence.*`、`raw/017_w1_version_difference_details.*` 与 W1 `asset_verification/summary.tsv`。

### 4.4 inliner 入口符号改名的性质

旧入口：

```text
_xla_home_linhao_mlgo_assets_<arch>_publish_InlinerSizeModel_llvm_InlinerSizeModel
```

新入口：

```text
_xla_InlinerSizeModel_llvm_InlinerSizeModel
```

两个 TensorFlow 版本的 `saved_model_aot_compile.py` 都按相同公式生成入口：

```python
'__xla_' + output_prefix + '__' + cpp_class
```

旧资产的 `output_prefix` 含生成机绝对路径，新命令使用短前缀。因此这是**调用/打包约定造成的命名变化**，不是已经证明的 2.15→2.18 固有改名。升级时可以选择保持旧 `output_prefix` 约定，或成对替换生成头和对象；无论哪种都需要明确并验证，不能只换 `.o`。

### 4.5 LLVM 消费侧若使用 2.18 需要处理的范围

当前 `packaging/llvm.spec`：

- 从每架构一个合并 tar 中取 TensorFlow include tree、inliner/regalloc 头与对象、XLA runtime；
- 明确列出 5 个要嵌入 LLVM 的 runtime 对象；
- 把 `InlinerSizeModel.h` 与 `.o` 作为配对覆盖输入；
- 校验目标架构、对象存在性和 runtime 嵌入状态。

因此 2.18 路线至少涉及：

1. 同步导入与 2.18 生成对象匹配的 TensorFlow 头和 XLA runtime 源码/归档，不能保留 2.15.1 头只换对象；
2. 复核并更新 spec 中显式的 5 个嵌入对象及校验清单；新 runtime 有 34 个归档成员，但这不自动表示 34 个都要作为单独对象嵌入，需按最终链接的未定义符号决定；
3. 成对更新 inliner 生成头和对象；若 output prefix 改变，同步处理入口命名；
4. 重新定义三包合并结构，或修改 spec 以消费 runtime/inliner 分包；
5. 三架构完整构建，检查未定义符号、`std::__1`/`std::__cxx11` ABI、MLGO 启用状态、推理正确性和性能。

这主要是**资产消费契约与打包接口适配**。本次没有证据表明 LLVM 的高层 `MLModelRunner` 调用语义必须修改；生成头和对象配套更换后，入口符号本身通常由生成头引用。证据见 `raw/018_llvm_mlgo_asset_consumption.*`。

## 5. 2.18 是否有本项目需要的改进

可确认的变化：

- TensorFlow 官方的 Linux CPU 受测构建组合从 2.15 的 Python 3.9–3.11 / Clang 16.0.0 / Bazel 6.1.0，更新为 2.18 的 Python 3.9–3.12 / Clang 17.0.6 / Bazel 6.5.0。[官方构建表](https://github.com/tensorflow/docs/blob/master/site/en/install/source.md#tested-build-configurations)
- W1 的 2.18 runtime 出现上述 f16/f32 convolution 与 f8/u8 matmul 成员。
- [2.18 release note](https://github.com/tensorflow/tensorflow/releases/tag/v2.18.0) 的主要项目包括 NumPy 2.0、Hermetic CUDA、TFLite 等，没有列出 `tfcompile`、MLGO、AOT 或 libc++ 专项改进。

本次未能确认的项目：

- 当前 inliner SavedModel 是否会调用新增的 runtime 成员：`NOT_OBSERVED`；
- 2.18 对当前 MLGO 模型的性能提升：`NOT_OBSERVED`，没有做基准；
- 2.18 是否修复平台当前碰到的 MLGO bug：`NOT_OBSERVED`，没有对应故障和修复记录；
- TensorFlow 2.18 是否原生支持生成 libc++ XLA runtime：未找到；W1 的 libc++ 能力来自本项目对生成脚本和 sysroot 选择的改动；
- 用 MLGO 官方 2.15 训练/导出、再用 2.18 release AOT 编译的跨版本组合是否等价：`NOT_OBSERVED`。

所以可以列出的 2.18 收益是工具链代际与 runtime 能力集合更新；是否属于“我们需要的改进”目前没有实测或官方材料支持肯定结论。

## 6. 两种选择的代价（不作推荐）

| 项目 | 维持平台 TensorFlow 2.15.1 | 升到 TensorFlow 2.18.0 |
|---|---|---|
| 所需材料 | 精确 2.15.1 revision、平台补丁、当时 flags、Clang/Bazel/Python AOT 环境、原模型与打包约定 | 开发人员 2.18 AOT 源码/环境已有；还需最终确认的三架构 libc++/libstdc++ 资产及消费约定 |
| 当前材料缺口 | 旧资产只证明版本为 2.15.1；准确 source revision 在现有平台仓未定位，`NOT_AVAILABLE` | 候选已生成，但与现有资产结构和符号集合不等价，不能直接导入 |
| LLVM 侧改动 | 若复现现有资产契约，LLVM/spec 高层消费方式无需变；标准库双路径生成机制仍需移植和验证 | 需要同步替换匹配头/runtime/模型对象，更新对象清单、三包结构与校验；未证明需改高层 MLGO 调用语义 |
| 与 MLGO 官方一致性 | 与官方稳定训练依赖的 2.15 主版本一致；平台 2.15.1 与官方 2.15.0 的补丁差异仍需核对 | 偏离当前稳定依赖组；TF-Agents 0.19.0 没有官方 TF 2.18 配对 |
| 标准库双路径 | 准确 2.15.1 环境尚未实测；需实现默认 libstdc++、显式 libc++ 并做三架构对照 | W1 修改版的双路径已实测；开发人员原版不具备该能力 |
| 已知风险 | 准确旧环境缺失；重建可复现性未证明；保留旧 TensorFlow 代际 | 资产契约变化、官方稳定训练栈偏离、跨版本训练/release 组合未验证；适配量大于直接换文件 |
| 验证量 | 三架构两标准库资产；与旧资产接口/ABI/有效载荷对照；LLVM 三架构构建、MLGO 功能与性能 | 同样需要三架构两标准库；另加 2.15→2.18 符号/结构差异收口、LLVM 消费适配、MLGO 功能与性能 |
| 已知收益 | 最大限度维持既有资产与官方稳定依赖代际 | 新的受测 Python/Clang/Bazel 组合与 runtime 成员；对当前模型的必要性/性能收益未观测 |

两条路线的实际生成时间、算力和三架构完整验证耗时均没有在本任务中重新测量，记为 `NOT_AVAILABLE`；不得用 W1 的部分时长推算完整成本。

## 7. 开发人员 2.18 原版是否支持两种标准库

明确答案：**不能。**

原始生成器 `e611ebb4e4dd6d47023f3b06bccc528022aa098f` 的事实是：

1. `fetch_sysroot.py` 下载 `libstdc++-devel`、`gcc-c++`、`gcc`、`libstdc++`，没有 libc++/libc++abi sysroot 路径；
2. `mlgo_pack.py` 只在 `usr/lib*/gcc/*/*/include/c++` 下找 C++ 头文件；
3. 命令行没有 `--stdlib` 或等价开关；
4. 开发人员 v3 的 AOT spec 设置 `/usr/bin/gcc`、`/usr/bin/g++` 和 `BAZEL_COMPILER=gcc`；
5. 所以原版生成的 XLA runtime 路径固定为 libstdc++。inliner 模型 `.o` 本身在 W1 中没有标准库 ABI 符号，但完整可用资产还包括带 C++ ABI 的 runtime，不能据此称原版支持 libc++。

W1 执行版本才新增：

```text
--stdlib {libstdc++,libc++}   # 默认 libstdc++
```

实测结果：

- 完全省略开关与显式 `--stdlib libstdc++`：三架构文件集合相同，inliner 有效载荷逐字节相同，runtime 8,388 个非归档文件逐字节相同；归档容器 SHA 因元数据不同，但 34/34 个成员逐字节相同；
- libc++ 路径：三架构 runtime 检出 `std::__1`，未检出 `std::__cxx11`；
- 同一 TF 2.18 输入下，libc++ 与 libstdc++ 文件集合、非二进制载荷和 inliner 相同；runtime 34 个成员中 29 个内容变化，5 个显式嵌入对象中 4 个变化，符合标准库 ABI 切换发生在 runtime 的定位。

因此：“2.18 可以做成双路径”已经由**本项目修改版**证明；“开发人员原版已经支持双路径”不成立。证据见 `raw/009_developer_original_stdlib_capability.*`、`raw/010_original_generator_comparison.*` 和 `raw/016_w1_asset_evidence.*`。

## 8. 自行判断与尚存疑问

### 8.1 本任务采用的判断口径

1. 把 MLGO 官方依赖固定与 TensorFlow AOT 自身兼容性分开；提交说明只直接证明前者。
2. 把所有资产全局符号差异称为“符号集合差异”，不把每一个差异都描述为稳定公开 API 变化。
3. 把 inliner 入口改名归因于已验证的 `output_prefix` 差异，不归因于版本；两个 tag 的生成公式相同。
4. “支持两种标准库”按完整 runtime 资产可选择 ABI 判断，不以标准库中性的模型 `.o` 单独判断。
5. “LLVM 消费侧适配”限定为已经从 spec/CMake 证明的资产、对象清单与打包契约；未推断必须修改 LLVM 高层推理逻辑。

### 8.2 尚存疑问与缺口

1. 平台旧 2.15.1 资产对应的准确 TensorFlow revision、平台补丁、构建 flags 与 AOT 构建环境仍为 `NOT_AVAILABLE`；只有资产内版本头可核实。
2. MLGO 官方精确固定 2.15.0，平台资产为 2.15.1；补丁级差异是否影响 AOT runtime 尚未逐文件核对。
3. 2.18 新增四个 runtime 成员是否被当前 inliner 模型实际调用：`NOT_OBSERVED`。
4. 训练/导出使用 2.15，而 release AOT 使用 2.18 的组合是否受官方支持、数值与性能是否等价：`NOT_OBSERVED`。
5. 2.18 候选中所有 missing/extra 符号尚未逐项分成稳定接口、内部实现、依赖版本和标准库 ABI 四类；当前证据只足以判定“不可直接替换”。
6. 两路线的端到端构建耗时、算力与功能/性能验证结果：`NOT_AVAILABLE` / `NOT_OBSERVED`。

## 9. 操作纪律核对

- 资源门禁 `tools/resource_gate.sh --level light`：退出码 0，无需重试；
- `codes/`：未修改；
- 平台源码与配置：未修改；
- Gerrit 或其他外部源码仓：未推送；
- 本项目只新增 `docs/progress/BUILD_MLGO_TF/` 查证材料；
- 本文不作版本选择建议、不排优先级，交人工裁决。

技术性命令错误均保留原始记录：恢复后首次读取把实际文件名
`014_xla_wrapper_runtime_interface_diff.stdout.txt` 写成了不存在的
`014_tf_xla_runtime_interface_diff.stdout.txt`，退出码 2，见 `raw/015_*`；
首次汇总自检中三次 `printf` 把以 `---` 开头的格式串当作选项，产生提示但
总命令退出码为 0，见 `raw/019_*`。后一项已仅修正打印格式后重跑，退出码 0，
见 `raw/020_*`。提交前范围检查又因嵌套 shell 中的 `awk` `$1` 被提前展开，
使目录字节数一项空缺；后续命令掩盖了该子命令失败，故包装器总退出码仍为
0，见 `raw/022_*`。已改用 `du | cut` 重跑完整范围检查，退出码 0，见
`raw/023_*`。这些技术性错误均未改变查证对象或判据。
