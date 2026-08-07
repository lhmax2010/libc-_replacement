# R17 方法与口径

## 1. 只读边界与输入

本任务只读取 `progress/R11/`、`progress/R16/` 与 R11 已留存的 JSON 分片元数据；没有下载、安装、构建、解包 RPM，也没有重新对平台 ELF 执行 `readelf`/`nm`。输入文件与 SHA256 见 `tables/input_files.tsv`，命令原文与退出码见 `commands/`。

## 2. compiler-rt 比对对象

R16 比对对象不是共享库导出，而是三个静态 builtins 库：

- armv7l：`libclang_rt.builtins-arm.a`
- aarch64：`libclang_rt.builtins-aarch64.a`
- x86_64：`libclang_rt.builtins-x86_64.a`

提取命令均为 `nm -g --defined-only <archive>`。三份完整原文从 R16 原样复制到 `raw/41_*_compiler_rt_builtins_symbols.txt`，复制前后的 SHA256 相同。因此不存在“平台 builtins 为静态库、却拿共享库导出比对”造成的系统性低估。此集合比较仍只表示精确符号名命中，不证明 ABI、语义、可见性或版本节点兼容。

## 3. libgcc_s 去重口径

`libgcc_symbol_versions.tsv` 的单位是“定义符号基础名 × 版本节点”对；`libgcc_export_classification.tsv` 的单位是唯一基础符号名。R17 的 helper 分母与缺口分母均沿用后者，不按别名关系合并不同名称。

- armv7l：1163 对、1163 个唯一基础名，去除版本重复 0。
- aarch64：159 对、159 个唯一基础名，去除版本重复 0。
- x86_64：183 对、177 个唯一基础名，去除版本重复 6；六个基础名各同时位于两个节点，详见 `tables/libgcc_multiple_version_base_names.tsv`。

## 4. 家族分类

分类单位是 R16 已判定为 `COMPILER_HELPER` 且不在对应 compiler-rt 静态 builtins 精确名称集合中的唯一基础符号名。分类顺序如下：

1. `__aeabi_*` → `AEABI`；
2. `__bid_*`、`__dpd_*`、`__dec*` → `DECIMAL_FLOAT`；
3. `__gnu_*` 以及任务书列举的定点前缀 → `FIXED_POINT`；
4. 常规整数、浮点、转换、位运算前缀 → `CONVENTIONAL_INT_FLOAT`；
5. 其余 → `OTHER`，并在成员表逐一保留名称。

完整成员与分类见 `tables/missing_helper_family_members.tsv`；脚本见 `tools/analyze_gap.py`。

## 5. 实际引用面为何不可得

R11 扫描器确实对每个 ELF 执行了 `readelf --dyn-syms` 并在内存中解析全部 UND，但持久化前执行了 `if "Unwind" not in base: continue`；只有名称含 `Unwind` 的 UND 被写入 `unwind` 记录与 `unwind_undefined_symbols.tsv`。源码原文见 `commands/02_r11_filter_scope_source.txt`。R11 的 11,332 个 JSON 分片只有 `commands/elfs/extract/failures/needed/package/unwind` 等字段，抽查证据见 `commands/05_r11_retained_schema.txt`；命令日志只保留命令与退出码，不含未过滤的 dynsym 标准输出。

因此，现有 R11/R16 输入不能回答任意 helper 是否被平台 ELF 实际引用。重新读取已解包 ELF 也属于本任务禁止的“重扫”。R17 对每个缺口符号将 ELF 数、包数写为 `NOT_AVAILABLE`，状态统一为 `R11_UNFILTERED_UNDEFINED_SYMBOLS_NOT_RETAINED`；不得把它解释成零引用。由此，“有实际引用且 compiler-rt 未覆盖”的真实缺口规模及其版本节点也为 `NOT_AVAILABLE`。

## 6. 计数单位

- 符号数：唯一基础符号名；家族比例分母是各架构缺口符号数。
- 符号版本对：基础名 × 版本节点；只用于版本节点分布，不能与唯一符号数相加。
- ELF 数：R11 `runtime_elf=YES` 的文件实例。
- 二进制包数：带架构的 RPM 包实例；“含运行时 ELF 包”是至少有一个 `runtime_elf=YES` 的包实例。

各分母见 `tables/counting_scope.tsv`。本任务不把源码包、二进制包和 ELF 混用。
