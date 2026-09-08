# LLVM 中 MLGO AOT 消费代码的来源核对

## 1. 明确结论

人工提出的说法需要拆成两层后才能成立：

1. **“LLVM 上游没有 MLGO AOT 消费机制”不成立。** `llvmorg-22.1.8`
   上游已经具备 `TENSORFLOW_AOT_PATH`、`saved_model_cli aot_compile_cpu`、
   预生成模型 `.h/.o` 覆盖入口、`ReleaseModeModelRunner`，以及 inliner 和
   regalloc eviction 两个直接调用点。
2. **“当前三架构预编译资产的 RPM 接入方式是 Tizen 自己加的”成立。**
   三个 `mlgo_*_model.tar.gz`、`Source1002`–`Source1004`、按架构解包、
   显式 5 个 runtime 对象清单和构建期校验均只存在于 Tizen packaging。
3. `0005-Merge-MLGO-AOT-compilation-support-and-optimization-.patch`
   对上游只扩展了 **两个 CMake 文件**：允许不安装/不链接独立的
   `tf_xla_runtime`，并把指定的预编译 runtime 对象恰好一次嵌入
   MLGO 组件。它没有新增 inliner/regalloc 的 C++ 推理调用代码。
4. 因而，“升级 2.18 要改 LLVM 高层 C++ 消费逻辑”目前没有证据。
   已确认要处理的是 Tizen 的资产生成、合并 tar 结构、spec 路径、
   显式 runtime 对象清单及校验。Tizen 的通用嵌入 CMake 扩展是否需要改，
   取决于最终对象清单；其现有接口本身可接收任意分号分隔对象列表。
5. 但不能据此说升级完全没有上游约束。`llvmorg-22.1.8` 的官方
   `MLGO.rst` 明确写着“当前支持 TF 2.15”；CMake 没有强制版本检查，
   但 2.18 位于该版本文档声明的支持范围之外。

本报告只界定来源和工作范围，不选择 TensorFlow 版本。

## 2. 输入身份与核对方法

| 输入 | 身份 | 用途 |
| --- | --- | --- |
| Tizen LLVM 工作树 | `codes/llvm`，`tizen_base`，HEAD `5ed6c77278dfa7a470667cf1a137723d3c96fe60` | 当前消费路径与 git 历史 |
| LLVM 上游 | `llvmorg-22.1.8`，tag object `e0130735…`，peeled commit `ca7933e4…` | 同版本逐字节对照 |
| Tizen MLGO squash | `fd7a4929c0e424d564cdd1f0c55af9153dcea321` | `0005` 导出补丁对应的提交 |
| Tizen packaging import | `3d723549f6037bdab5be47aaa6e6354fd3707d89` | spec 和三个资产的来源 |
| W1 旧/新资产对照 | `tmp/BUILD_W1_0905/diagnosis-x86` 与既有报告 | 核对 2.15/2.18 生成类契约及资产结构差异 |

上游 tag 没有出现在本地 Tizen 仓的 tag 集合中，因此没有写入
`codes/llvm` 或 fetch 到该仓。采用 `git ls-remote` 确认官方 tag 身份，
再从 GitHub 官方 raw tag 下载所需文件到本材料目录。12 个上游文件
（11 个代码文件与 1 份文档）
逐字节与当前 Tizen 文件比较；`codes/llvm` 始终为干净工作树。

## 3. 从 Source1002 到 C++ 调用的完整链条

### 3.1 Tizen packaging：选择并解包资产

`packaging/llvm.spec:41-46` 声明：

```spec
Source1002: mlgo_arm_model.tar.gz
Source1003: mlgo_aarch_model.tar.gz
Source1004: mlgo_x86_model.tar.gz
```

`packaging/llvm.spec:155-168` 在 `%prep` 中按 armv7l、aarch64、x86_64
分别解到 `mlgo_verify_assets`。三个 tar 当前各有 12,782 个条目，顶层均含：

- `InlinerSizeModel.h/.o`；
- `RegAllocEvictModel.h/.o`；
- `xla_runtime_objects/` 下 5 个显式对象；
- `mlgo_sysroot/include/`；
- `mlgo_sysroot/xla_aot_runtime_src/CMakeLists.txt` 和
  `libtf_xla_runtime_prebuilt.a`。

具体文件名和三包 SHA256 见 `raw/022_asset_structure_concise.*`。

### 3.2 Tizen spec 把资产交给 CMake

`packaging/llvm.spec:188-193` 定义资产根和以下 5 个 runtime 对象：

```text
xla_compiled_cpu_function.cc.o
cpu_function_runtime.cc.o
custom_call_status.cc.o
executable_run_options.cc.o
runtime_single_threaded_matmul_f32.cc.o
```

`packaging/llvm.spec:257-265` 设置：

- `TENSORFLOW_AOT_PATH` 指向包内 `mlgo_sysroot`；
- `LLVM_MLGO_EXPORT_TF_XLA_RUNTIME=OFF`；
- `LLVM_MLGO_EMBED_TF_XLA_RUNTIME_OBJECTS` 指向上述 5 个对象；
- 两组 `LLVM_OVERRIDE_MODEL_HEADER_*` / `OBJECT_*` 指向模型头与对象。

`packaging/llvm.spec:276-340` 又核验覆盖文件存在、目标架构一致、没有退回
自动 mock、runtime 对象确实进入 CMake cache，且独立的
`libtf_xla_runtime.a` 不被安装。

这些块均由 Tizen commit `3d723549…` 引入。官方 tag 中没有
`packaging/llvm.spec` 或三个 tar；对应官方 raw URL 返回 HTTP 404。

### 3.3 上游已有的 AOT 入口与模型覆盖机制

`llvm/CMakeLists.txt:1174-1220` 的主体来自上游：

- `TENSORFLOW_AOT_PATH` 非空时定义 `LLVM_HAVE_TF_AOT`；
- 从该路径定位 `saved_model_cli` 和 TensorFlow 头文件；
- 加入 `xla_aot_runtime_src`；
- 识别 inliner/regalloc 的覆盖头和覆盖对象。

`llvm/cmake/modules/TensorFlowCompile.cmake:41-113` 也是上游代码：

- 没有覆盖文件时调用 `saved_model_cli aot_compile_cpu`；
- 有配对覆盖时用 `configure_file(... COPYONLY)` 复制 `.h/.o`；
- 把模型对象和头加入 `GeneratedMLSources`；
- 定义 `LLVM_HAVE_TF_AOT_<MODEL>`。

因此，“把预生成 `InlinerSizeModel.h/.o` 和
`RegAllocEvictModel.h/.o` 交给 LLVM”本身不是 Tizen 新机制；Tizen spec
是在调用上游已有的 override 接口。

### 3.4 模型对象进入 LLVM 组件

- `llvm/lib/Analysis/CMakeLists.txt:1-18` 为 `InlinerSizeModel` 调用
  `tf_find_and_compile`；`:163,171,174` 把生成物、依赖和链接依赖加入
  `LLVMAnalysis`。
- `llvm/lib/CodeGen/CMakeLists.txt:1-18` 对 `RegAllocEvictModel` 做同样处理；
  `:260,270,275` 把它们加入 `LLVMCodeGen`。

这两个文件与上游 `llvmorg-22.1.8` **逐字节一致**。

### 3.5 C++ 运行时调用

Inliner 路径：

- `llvm/lib/Analysis/InlineAdvisor.cpp:35-37` 激活 AOT 路径；
- `llvm/lib/Analysis/MLInlineAdvisor.cpp:67-73` 包含生成的
  `InlinerSizeModel.h` 并选择生成类；
- 同文件 `:75-97` 构造
  `ReleaseModeModelRunner<llvm::InlinerSizeModel>`。

Regalloc eviction 路径：

- `llvm/lib/CodeGen/RegAllocEvictionAdvisor.cpp:60-62` 激活 AOT 路径；
- `llvm/lib/CodeGen/MLRegAllocEvictAdvisor.cpp:51-57` 包含生成的
  `RegAllocEvictModel.h`；
- 同文件 `:354-392` 构造
  `ReleaseModeModelRunner<RegAllocEvictModel>`。

两条路径最终都进入
`llvm/include/llvm/Analysis/ReleaseModeModelRunner.h:54-137`，通过生成类/
其 TensorFlow 基类调用：

```cpp
CompiledModel->LookupResultIndex(...);
CompiledModel->LookupArgIndex(...);
CompiledModel->arg_data(Index);
CompiledModel->Run();
CompiledModel->result_data(ResultIndex);
```

上述 5 个生产 C++ 文件均与官方 tag **逐字节一致**。另外，
`MLRegAllocPriorityAdvisor.cpp` 当前把 `CompiledModelType` 固定为
`NoopSavedModelImpl`，不消费这三个 tar 中的 AOT 模型，故不计入生产消费链。

## 4. `0005` 补丁实际加了什么

归档记录将
`0005-Merge-MLGO-AOT-compilation-support-and-optimization-.patch`
映射到提交 `fd7a4929…`，导出补丁 SHA256 为
`da2cacae22ef8ececf808596b14ff53b6741db23e3f34d7fc5e07a75fa3543c2`。
当前 checkout 中不保留 GBS 临时导出的 patch 文件，但提交对象完整可读；
`raw/007_platform_mlgo_patch_full.*` 保存了完整 diff。

实际 diff 只有：

| 文件 | 规模 | 作用 |
| --- | ---: | --- |
| `llvm/CMakeLists.txt` | 12 增、5 删 | 新增 runtime 导出开关和预编译对象列表入口；仅在开关开启时安装/导出 `tf_xla_runtime` |
| `llvm/cmake/modules/TensorFlowCompile.cmake` | 28 增、2 删 | 把传入的 runtime 对象恰好一次加入模型所在组件；关闭导出时不再把 `tf_xla_runtime` 加入链接库 |
| 合计 | 40 增、7 删 | 不改任何 MLGO C++ 推理调用 |

补丁标题/commit message 提到预编译模型和 spec 更新，但该 squash 的实际
文件 diff 没有 spec 或 tar。当前 packaging 是另一个 Tizen commit
`3d723549…` 一次性加入的，包含 `llvm.spec`、manifest 和三个二进制资产。
来源判定以实际 diff 为准，而不以提交说明外推。

## 5. 上游机制与 Tizen 扩展的关系

关系是“**在上游 AOT 机制上增加平台打包和 runtime 嵌入策略**”，不是另起
一套推理框架：

```text
Tizen spec/三架构 tar
  -> 上游 TENSORFLOW_AOT_PATH 与模型 .h/.o override
  -> 上游 Analysis/CodeGen 的 tf_find_and_compile
  -> 上游 ReleaseModeModelRunner 与两个 advisor

Tizen 额外分支
  -> 不导出独立 tf_xla_runtime
  -> 把 spec 指定的预编译 runtime .o 恰好一次嵌入组件
```

按清单行口径，生产消费链共核对 15 个代码块或位置，归入 10 个文件：

- 1 个 Tizen-only 文件：`packaging/llvm.spec`；
- 2 个“上游主体 + Tizen 扩展”文件：`llvm/CMakeLists.txt`、
  `TensorFlowCompile.cmake`；
- 7 个逐字节上游文件：两个组件 CMake、两个激活点、两个 advisor 和
  `ReleaseModeModelRunner.h`。

若只问 `0005`：Tizen 自己增加/改写的是 2 个文件、40 增 7 删。
若问整个 RPM 资产接入：还必须计入 Tizen-only spec 和三个 tar。
若问实际执行模型的 C++ 代码：本次找到的生产调用点全部来自上游。

## 6. 上游对 TensorFlow 版本的约束

这里要区分文档约束与代码门禁：

- 官方 `llvmorg-22.1.8/llvm/docs/MLGO.rst:691-694` 明确写明 release
  AOT 需要 TensorFlow pip 包，并称“当前支持 TF 2.15”。
- `llvm/CMakeLists.txt` 和 `TensorFlowCompile.cmake` 没有解析 TensorFlow
  版本，也没有拒绝 2.18 的条件。
- 同一文档 `:720-722` 还说明上游正从 TensorFlow 包自带 AOT 编译器
  迁向树内 EmitC 方案，相关细节将变化。

因此事实表述应是：**LLVM 22.1.8 上游已有完整 AOT 消费机制，文档支持
版本为 TF 2.15；2.18 没有被 CMake 技术性禁止，但不在该 tag 的文档支持
声明内。**

这也修正了上一轮报告中一处不完整表述：上一轮说 LLVM 文档/CMake
“没有 2.15/2.18 版本判断或兼容矩阵”。“CMake 没有门禁”仍成立，
但遗漏了 `MLGO.rst:693` 的明确 TF 2.15 支持声明。本任务不修改上一轮材料，
在此单独申报。

## 7. 适配 TensorFlow 2.18 的准确范围

### 7.1 已确认要处理，且全部位于 Tizen 控制范围

1. 生成与导入匹配的 2.18 模型头、模型对象、TensorFlow 头和 XLA runtime；
2. 把开发人员当前的 runtime/inliner 分包重组为现有三架构合并 tar，
   或修改 Tizen spec 的 `Source1002`–`Source1004` 消费结构；
3. 复核 `MLGO_RUNTIME_OBJECTS` / `MLGO_REQUIRED_RUNTIME_OBJECTS`；
4. 同步调整 Tizen spec 的路径、存在性检查、架构检查和 runtime 嵌入检查；
5. 对三架构做链接、功能和性能验证，并确认只存在目标标准库 ABI。

### 7.2 Tizen CMake 扩展是否必须改

`LLVM_MLGO_EMBED_TF_XLA_RUNTIME_OBJECTS` 接收的是普通 CMake list，补丁本身
没有写死对象名。对象名写死在 spec。因此，如果 2.18 仍可用“由 spec
传入所需对象列表并嵌入一次”的策略，两个 CMake 文件无需修改；如果最终
需要改变“嵌入一次/不导出 runtime”的策略，修改仍落在 Tizen `0005` 扩展。

2.18 runtime 归档有 34 个成员，而旧资产为 30 个；这不能直接回答最终应
显式嵌入几个对象。需要以实际未定义符号闭包和链接结果确定，当前为
`NOT_OBSERVED`。

### 7.3 上游代码是否必须改

当前没有已证实的必要修改：

- W1 的 2.18 `InlinerSizeModel` 仍是
  `tensorflow::XlaCompiledCpuFunction` 子类；
- 上游 `ReleaseModeModelRunner` 使用的高层方法仍由该基类提供；
- 观察到的 inliner 头变化是 `_xla_*` 入口名称/output prefix 和旧 runtime
  setter 消失，成对替换 `.h/.o` 与匹配 runtime 后，不要求 advisor 直接
  调用该低层入口；
- regalloc 候选头在 W1 对照中甚至逐字节一致。

但这不是对 2.18 的上游支持保证。若后续构建证明生成类不再满足
`ReleaseModeModelRunner` 契约，可能要改的上游来源文件包括：

- `llvm/include/llvm/Analysis/ReleaseModeModelRunner.h`；
- 两个 advisor 的生成类接入点；
- 上游 `TensorFlowCompile.cmake` 的 tfcompile 调用约定。

修改这些文件会形成下游维护差异，并且所用 TensorFlow 版本超出 LLVM
22.1.8 文档声明的 TF 2.15 支持范围。当前没有观察到必须走到这一步。

## 8. 自行判断与尚存疑问

### 8.1 本任务采用的判断口径

1. “消费代码”限定为从 Source 资产到组件链接，再到生成类运行时调用的
   直接链条；训练、日志、TFLite development mode 和普通模型生成脚本不算
   当前三个 tar 的直接消费者。
2. “上游有/无”以官方 `llvmorg-22.1.8` 文件逐字节比较为准；git blame
   只作来源辅助，不以作者邮箱单独判定。
3. 对“多少”同时给文件和 diff 行数口径，避免把文件数、代码块数和行数
   混成一个数字。
4. 对 2.18 只列已由现有资产和代码证明的最小范围；没有把 34 个归档成员
   推断成 34 个必须嵌入对象。

### 8.2 尚存疑问/未观测

1. 2.18 最终链接闭包需要哪些 XLA runtime 对象：`NOT_OBSERVED`；需要把
   候选接入 LLVM 链接，记录逐项未定义符号解析结果。
2. 采用 2.18 后是否仍要保留“不导出独立 tf_xla_runtime、只嵌入一次”的
   平台策略：本任务未作产品/打包决策。
3. 2.18 生成类在三架构完整 LLVM 构建和运行中是否百分之百满足上游
   `ReleaseModeModelRunner` 契约：尚未完成导入构建，记 `NOT_OBSERVED`。
4. `0005` 的 GBS 导出文件当前不在工作区；其 SHA/commit 映射来自既有归档，
   内容用对应 Git commit 的完整 diff 复核。未重新执行一次 GBS export。
5. 上游文档声明 TF 2.15，而 CMake 不作版本检查；LLVM 项目对 2.18 的
   非文档化兼容程度没有由本任务实测。

## 9. 纪律执行

- 资源门禁 `--level light`：通过；
- `codes/llvm` 全程只读，任务开始时状态干净；
- 未修改源码、spec 或配置；
- 未向 Gerrit 或任何外部源码仓推送；
- 所有正式产物位于 `docs/progress/BUILD_MLGO_ORIGIN/`。

技术性记录：`raw/003_*` 的初次检索多带了无关的 Hexagon `XLA` 文本，且
指定了不存在的仓根 `CMakeLists.txt`，stderr 已保留；后续改为限定目录和
精确标识。`raw/004_*` 是全工作区的宽泛定位检索，虽正常返回退出码 0，
但输出只受 `sed` 上限约束，不能用于穷尽性或数量判定；后续所有结论均由
`raw/005_*` 以后的限定检索重做。
