# TensorFlow 2.15 与 2.18 AOT 接口差异（接入前核查）

本表比较 x86_64 上 LLVM 实际消费的 `InlinerSizeModel.o` 与五个
`xla_runtime_objects/*.o` 的外部定义符号。符号集合先经 `llvm-nm -C -j
--defined-only --extern-only` 提取并去重；模型头文件另作逐行 diff。该比较用于
揭示版本差异，不能把 TensorFlow 内部实现符号逐字相同当作 2.18 接入门禁。

| 集合 | 数量 |
| --- | ---: |
| 2.15 外部定义符号 | 76 |
| 2.18 外部定义符号 | 175 |
| 两边共有 | 44 |
| 仅 2.15 | 32 |
| 仅 2.18 | 131 |

关键差异如下：

- 模型入口由
  `_xla_home_linhao_mlgo_assets_x86_64_publish_InlinerSizeModel_llvm_InlinerSizeModel`
  变为 `_xla_InlinerSizeModel_llvm_InlinerSizeModel`；新生成头文件中的声明和
  调用点同步采用新入口。
- `XlaCompiledCpuFunction::LookupArgIndex` / `LookupResultIndex` 的字符串参数
  从 `std::__cxx11::basic_string` 变为 `std::__1::basic_string`，这是本轮要
  解除的 ABI 阻断。
- 2.18 runtime 还增加/移除了 TensorFlow/XLA 内部定义；完整列表分别在
  `old_only_interface_symbols.txt` 与 `new_only_interface_symbols.txt`。
- 新头文件不再调用 `set_static_data_use_xla_runtime(data, false)`；这属于
  TensorFlow 2.18 生成接口变化，未通过修改 LLVM 上游 C++ 代码补回。

接入可用性的判据因此是：新头文件与新对象成对进入 CMake、
`libLLVM.so.22.1` 完整链接、`ReleaseModeModelRunner` 实际构造并执行模型，
而不是要求两代 TensorFlow 的内部符号集合相同。
