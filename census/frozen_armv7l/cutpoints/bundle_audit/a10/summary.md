# A10 — bundle compat DSO 三断点数据

## 口径

- SONAME 统计限定 `v4_active_elf_ids.txt` 中的 active ARM ELF，精确匹配 `DT_NEEDED libbundle.so.0`；这是全量快照口径，未剔除测试包。
- 分类使用消费者自身实际 UND：存在 `_ZN10tizen_base*` 即 `CPP_PRESENT`；同时纳入其 Itanium const/ref-qualified 形式（如 `_ZNK10tizen_base*`），即使同时有 `bundle_*` 也仍归此类。只有存在 `bundle_*` 且没有前者才是 `C_ONLY`；两者皆无单列 `NO_MATCH_INDETERMINATE`，不推定为纯 C。
- 输入内容哈希见 `input_manifest.tsv`。

## 1. SONAME 依赖闭包

- 共 **302 ELF / 215 包** NEEDED `libbundle.so.0`。
- 包口径：`CPP_PRESENT` **81**，`C_ONLY` **125**，`NO_MATCH_INDETERMINATE` **9**；可判定包中纯 C 占 **60.7%**。
- ELF 口径：`CPP_PRESENT` **94**，`C_ONLY` **173**，`NO_MATCH_INDETERMINATE` **35**；同一 ELF 同时引用两类符号 **48** 个。
- 全量清单见 `libbundle_needed_packages.tsv` 与 `libbundle_needed_consumers.tsv`；不可判定项单列于 `no_match_indeterminate_consumers.tsv`。

**数据侧路由倾向：**可判定包中 `C_ONLY` 为 125、`CPP_PRESENT` 为 81，纯 C 消费者为数值多数、但并非压倒性多数，C++ 消费面仍属显著规模。按题设判据，数据仅**弱倾向主体保留 `libbundle.so.0`、compat 使用新 SONAME**，不构成强路由结论。`NO_MATCH_INDETERMINATE` 的 9 包不参与这项比例判断；最终路由仍由 HLD 定案。

## 2. KeyInfo / Bundle 桥接不变量

- `bundle_cpp.h` 中符合“签名含 `std::string` 或 `std::vector<KeyInfo>`”扫描域的公开声明共 **14** 个，逐项规格见 `bridge_invariants.tsv`，逐字头文件行见 `bundle_cpp_relevant_declarations.log`。
- 核心不变量：任何 `std::string`、`std::vector`、`KeyInfo::Impl` 或其引用/存储均不得跨 flavor；输入按 bytes/length 深拷贝，按值输出在接收侧重建，`GetName()` 的引用必须锚定原 `KeyInfo` 生命周期或改为 C 边界上的 caller-owned copy。
- `GetKeys()` 必须逐元素重建 `KeyInfo` 并保持顺序/所有权；数组、字节和 argv 结果必须在匹配的 C allocator 生命周期内完成深拷贝。
- 本表是设计规格输入，不是实现证明；错误码、embedded-NUL、null/empty 等精确语义仍需桥实现对照 C API 契约确认。

## 3. flavor 混流边

- A7 的 Bundle/KeyInfo 全符号复扫恢复 **13** 条 distinct 对象边。
- 全部 13 条的两端均位于 A8 的 130 包核心分量 `a8_tier1_0001`；没有跨分量边。
- 因此，本快照中“同波同头”条件可由 130 包波次原子性覆盖；仍需把波次部署原子性作为显式发布约束。

## 4. 方法边界

- `NO_MATCH_INDETERMINATE` 表示虽有 SONAME 依赖、但动态 UND 中没有直接命中两类前缀；可能由间接调用、链接选项或非目标符号导致，不能静默归为 C-only。
- UND 前缀分类证明消费者引用面，不证明调用频率，也不决定最终 SONAME 兼容策略。
- 13 条边只覆盖 A7 在动态符号图中已证实的 Bundle/KeyInfo 对象传递；`dlopen`、函数指针/回调及配置驱动边仍不在此闭包内。
- 桥接动作仅由公开签名推导；实现中的错误处理、allocator 配对与别名/生命周期行为仍需实现评审和测试验证。
