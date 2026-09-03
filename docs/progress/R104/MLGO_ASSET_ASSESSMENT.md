# LLVM/MLGO 预编译资产事实核查（不作方案选择）

状态：`FACTS_ESTABLISHED_WITH_REGENERATION_INPUTS_NOT_AVAILABLE`

本节只回答资产性质、可再生成性、接口形态及 LLVM 暂留 libstdc++ 的
边界后果。按人工要求，禁用 MLGO 不列为备选，也未实施任何方向。

## 1. 资产是什么、如何进入构建

`packaging/llvm.spec` 把三份架构资产作为源码包内容声明：

| 架构 | 文件 | 字节数 | SHA256 |
| --- | --- | ---: | --- |
| armv7l | `packaging/mlgo_arm_model.tar.gz` | 28,477,748 | `dd468c06f2d0bdd8176b390cefd577f8a0a2d41d566f95682fd4114626ab5138` |
| aarch64 | `packaging/mlgo_aarch_model.tar.gz` | 30,461,764 | `76f896051817600c4133bf90c34b8b32aa50469297956d572fe20ebd2663463e` |
| x86_64 | `packaging/mlgo_x86_model.tar.gz` | 28,652,731 | `6c221a95f06b5b506d3f47bf49252bea92404774829c81d555097fb26f790462` |

每份归档有 12,782 个条目，关键载荷是：

- `InlinerSizeModel.h/.o` 与 `RegAllocEvictModel.h/.o`；
- `xla_runtime_objects/` 下 5 个已经编译好的 TensorFlow/XLA `.cc.o`；
- `mlgo_sysroot/`、预编译 `libtf_xla_runtime_prebuilt.a` 和许可证。

spec 在 `%prep` 中按目标架构直接解开相应归档，在 CMake 配置时通过
`LLVM_OVERRIDE_MODEL_*` 指向模型头/对象，并通过
`LLVM_MLGO_EMBED_TF_XLA_RUNTIME_OBJECTS` 嵌入 5 个运行时对象。因此这些
对象是**随源码包交付的预编译输入**，不是当前 LLVM 构建生成的输出。

证据：原始记录 `135_inventory_mlgo_assets_and_history`、
`136_inspect_mlgo_archive_contents`，以及 spec 本身。

## 2. 来源与生成者

能机械确认的事实：

- 模型对象采用 LLVM 的 `tf_compile`/TensorFlow AOT 路径；上游
  `TensorFlowCompile.cmake` 实际调用 `saved_model_cli aot_compile_cpu`，输入
  包括 SavedModel 目录、tag、signature、模型类名和目标 triple；
- 归档内 5 个 XLA 运行时对象的 `.comment` 均标记 Clang 21.1.1；归档内
  TensorFlow 头声明版本 2.15.1；
- 当前 `tensorflow2` 仓为 2.18.0，相应 5 个源文件的 SHA256 与资产中的
  对象来源版本不能视为同一份；
- Git 历史只显示三份大归档随基线导入，归档和提交中没有生成者、生成命令、
  输入 revision 或 CI 任务标识。

`NOT_AVAILABLE`：现有材料不能确定实际生成者是内部训练流程、总部流程还是
外部交付方；也不能恢复当时的完整命令和构建环境。相关查找已覆盖本仓历史、
归档内容和项目现有脚本，见原始记录 `141`、`142`、`143`、`146`、`147`、
`148`。

## 3. 能否重新生成

当前材料**不足以按位或按 ABI 可靠重生**：

- 三份归档均不含 `saved_model.pb`、SavedModel 目录、训练数据或训练/导出
  脚本；模型 `.h/.o` 的直接输入缺失；
- `mlgo_sysroot/xla_aot_runtime_src` 仅有 CMake 描述和预编译 archive，没有
  5 个 `.cc` 源文件；
- 当前 TensorFlow 2.18.0 树能找到同名的 5 个源文件，但资产标记为 2.15.1，
  两者不能替代；平台仓当前也未找到 2.15.1 的可定位 revision/tag；
- 缺少准确的 TensorFlow revision、补丁集、编译选项和 Clang 21.1.1 构建根。

若人工补齐 SavedModel/训练导出物、准确 TensorFlow 2.15.1 源码与补丁、生成
脚本和工具链，技术路径是存在的。模型 AOT 对象和运行时对象含目标机器码，
必须按 armv7l、aarch64、x86_64 分别生成。现有证据不能量化训练时间和算力，
因此成本记 `NOT_AVAILABLE`；AOT 编译本身与重新训练应分开估算。

## 4. 当前接口是否只能走 C++

当前连接路径确实是 C++ ABI：生成头声明模型类，LLVM 的
`ReleaseModeModelRunner`/ML advisor 直接构造并调用这些 C++ 类型；XLA
运行时对象还定义/引用含 `std::string` 的 C++ 方法。它不是已存在的纯 C
opaque-handle API。

可以另行设计纯 C 边界，但这不是替换一个链接参数：需要新增一个用资产原有
libstdc++ ABI 编译的桥，使用 opaque handle 与标量/显式缓冲区暴露 create、
evaluate、lookup、destroy，并保证所有对象所有权和异常都在桥内终止；LLVM
一侧改为只调用该 C API。这会引入双运行时、桥层生命周期、错误传递和性能
验证，且要覆盖三架构。现有代码中没有该桥，因此这里只能判为“技术上可设计，
实现与验证代价显著”，未实施。

## 5. 实际阻断机制：内联命名空间符号不匹配

Itanium C++ ABI 的 mangled name 会编码命名空间。libc++ 的标准库实体位于
`std::__1`，libstdc++ 新 ABI 的相关实体位于 `std::__cxx11`。当
`std::string` 等类型进入函数参数、返回值或类方法签名时，两边产生不同符号；
链接器不会把它们视为同一函数。

armv7l 实际失败中，预编译 XLA 对象提供的是含 `std::__cxx11` 的定义，新构建
LLVM 请求的是含 `std::__1` 的定义。原始证据 `119`/`120` 统计到：

- `custom_call_status.cc.o`：2 个 `std::__cxx11` 相关符号；
- `executable_run_options.cc.o`：3 个；
- `xla_compiled_cpu_function.cc.o`：3 个；
- 新 LLVM MLInlineAdvisor 对象：116 个 `std::__1` 符号；
- 新 LLVM MLRegAlloc 对象：69 个 `std::__1` 符号。

这正是总部报告所述“内联命名空间符号不匹配”的实际实例；它与返回类型未
进入符号名时可能发生的静默布局错读不同，本例在链接阶段明确失败。

## 6. LLVM、BCC、bpftrace 暂留 libstdc++ 的边界

若三者一起保留 libstdc++：

- `bcc-tools -> llvm` 的 4 条 ELF 边、584 个布局敏感 C++ 符号仍为同一 ABI；
- `bpftrace -> llvm` 的 2 条 ELF 边、201 个布局敏感 C++ 符号仍为同一 ABI；
- LLVM 包内部 113 条 ELF 边、17,589 个布局敏感符号仍为同一 ABI。

因此暂停三者而不是只暂停 LLVM，避免了在这三组已证明的边上制造新混合。
但对外仍有已观测边：Unified 的 `rpi4-linux-kernel` 产物 `/usr/bin/perf` 与
`/usr/bin/trace` 共 4 条 ELF 边、9 个去重 C++ 符号指向 `libLLVM.so.22.1`。
该组已证明为 C++ ABI 耦合，但现有证据未证明或排除标准库布局传递，不能标为
安全。其余 14 个 Unified `BuildRequires` consumer 只证明构建期使用，不能
仅据依赖声明推断同进程 C++ 边。

事实结论是：三者作为阶段性同 ABI 岛可以避免**内部**已知风险，但当前证据
不足以证明它能长期无风险存在；至少上述 rpi4 运行期 C++ 边仍需让两端同 ABI，
或另行证明接口不传递受影响状态。这里不选择处置方向。

## 7. 其他可能的同类资产与系统化查找

当前已有明确候选，而非凭包名推测：

1. `bpftrace` spec 的 `Source1002: prebuilt-static-bpftrace.tar.gz`，安装为
   `bpftrace-static`；它与 LLVM 链一并暂停，仍需核对归档的编译 ABI；
2. 既有 vendor-prebuilt 盘点中的 `drm-service-core-tizen`、
   `hal-api-drm-haltests`、`hal-backend-drm-generic`、`pen-wave`。R13 对它们
   证明的是三架构预编译 ELF 的 `libgcc_s`/展开器风险，其中 `pen-wave` 另有
   C++ 内容；这不是它们必然存在标准库布局边的证明，是否与迁移后代码交换
   C++ 对象仍需逐边核实；
3. `tensorflow2` 的 AOT/静态归档交付路径；本轮会按静态成员与真实 consumer
   链接检查，不能用 `DT_NEEDED` 替代；
4. 名称不含 `prebuilt`、但源码归档内部实际含 ET_REL、ET_DYN 或 ar archive
   的载荷。这部分不能靠文件名扫描排除。

可系统化执行的机械流程：对全部 SRPM 的每个 Source 归档按 magic 解包；用
`file`/ELF header/ar member 而非扩展名枚举预编译对象；记录 `.comment`、DWARF
producer、目标架构；用 `nm/readelf` 分类 `std::__1`、`std::__cxx11`、
`GLIBCXX_*`；从 spec 和实际 link command 确认资产是否进入产物；最后把资产
的 DEF 与迁移后 consumer 的 UND 做精确交集并按 C API/C++ ABI/布局敏感分类。
仅发现资产不等于发现风险，必须完成最后两步。

本轮低成本 spec 指示扫描覆盖已有 R87 源码语料和 11 个实施仓，命中 LLVM
MLGO 与 bpftrace 预编译载荷（原始记录 `174`）。它不是上述 magic 全量扫描，
因此不宣称候选全集已闭合。

## 8. 尚存问题与自行判断

- `NOT_AVAILABLE`：三份资产的真实生成者、SavedModel/训练数据、准确生成
  revision/命令、重生成算力成本；需由资产提供方或 MLGO 训练流水线补齐。
- `NOT_OBSERVED`：尚未对全部平台 SRPM 做归档内二进制 magic 全扫；当前只列
  已有证据的候选和可执行方法。
- 自行判断：把“重生模型”和“重编 XLA runtime objects”拆开；前者缺模型
  输入，后者缺准确旧源码/配置，两者的可行性不能混写。
- 自行判断：LLVM/BCC/bpftrace 的 17,589/584/201 是三者内部同 ABI 边；用它们
  衡量暂留方案的内部一致性，但没有把构建期依赖误写成运行期边。
