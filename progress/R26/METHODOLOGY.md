# R26 判定方法与局限

## 输入与符号提取

本轮没有重扫 repodata 或 ELF。R22 已对消费者和实际 DT_NEEDED provider 执行 `readelf --dyn-syms -W <ELF>`，将版本化名称在首个 `@` 处分割，再以消费者 UND 与 provider DEF 求交。R22 只要交集中至少一个名称以 `_Z` 开头，就把源码级边标为 `TRUE_CPP_ABI_COUPLING`。R26 从这张冻结表取得每边的 `cpp_symbols` 和 `c_symbols`，再用 `c++filt` 批量解名。

因此存在一个口径约束：R25 的 1,191 条边全部来自 R22 真边，每边至少有一个 `_Z` 风格符号。按任务定义，“全部引用均为无 Itanium 修饰 C 符号”才是 C_ONLY，所以本总体不能机械产生 C_ONLY；不能把同时存在的 C 符号覆盖掉该边已经存在的 C++ 符号。

## 肯定式分类规则

一条边只要至少一个相交符号满足以下肯定证据，就归 `CXX_SEMANTICS`，完整命中符号进入 `semantic_evidence_symbols.tsv`：

1. 解名含 `std::` 或 `[abi:cxx11]`；
2. `_ZTI/_ZTS` typeinfo、`_ZTV/_ZTT` 虚表/VTT、`_ZTh/_ZTv/_ZTc` thunk；
3. 异常、throw/catch/terminate 等符号；
4. 构造或析构函数；
5. 参数中明确出现命名空间限定的自定义类类型；
6. `_ZNK/_ZNV/_ZNr` 限定的非静态成员函数，隐含跨边界 `this` 类对象；
7. 符号本身不编码足够类型时，已下载 devel RPM 的公开头明确确认类方法或类静态对象。

若每个引用都无 `_Z` 风格符号才可归 `C_ONLY`。无法满足肯定规则的 `_Z` 风格符号保持 `UNDETERMINED`，不根据名称印象猜测。

## 歧义复核

只读流式读取四个既有 x86_64 devel RPM 的公开头，没有安装或落地解包：

- LLVM `raw_ostream.h` 确认 `raw_ostream` 是类且 `write` 是成员；
- FlatBuffers `util.h` 确认 `ClassicLocale::instance_` 是 `ClassicLocale` 类静态对象；
- gtest/gmock 头确认 `UnitTest` 类和 `InitGoogleMock`，同时动态符号带 `[abi:cxx11]`；
- bundle 的三个公开头未找到 `tizen_base::logger` 声明，其类型不能从数据符号名恢复，保持 UNDETERMINED。

## 局限

- 自定义类：只有上述肯定证据才归 CXX；仅命名空间作用域或数据名称且类型不编码者不可判定。
- 不透明 C 指针：符号名不包含 C 参数类型，无法区分普通句柄与 C++ 对象指针。
- 回调：`void reg(void(*cb)(void*), void*)` 表现为 C 符号，回调是否抛异常穿越 provider 不能由动态符号判定。表内仅对 C 符号名作 callback/register/handler 等 token 近似，不当作事实确认。
- `void*` 所有权：动态名称无法恢复 pointee 与所有权，规模 NOT_AVAILABLE。
- 返回类型：Itanium 普通非模板函数通常不编码返回类型，缺失时不推断。
- header-only inline/template：没有跨包动态符号，不进入 R22/R25 图，规模 NOT_AVAILABLE。
- 静态库、dlopen、未唯一解析 DT_NEEDED、弱符号绑定时序仍沿用 R22/R24 已申报盲区。
- Rust 旧式 mangling 也使用 `_ZN…E` 外形；12 条边被 R22 的前缀规则纳入，但 `c++filt` 显示 Rust hash 后缀/转义。本轮不把它们改判成 C 或 C++，统一 UNDETERMINED。

## C_ONLY 抽样

C_ONLY 总体实测为 0/1,191，所以样本量为 0，公开头复核不适用。该结果不是“未抽样”；若强行从非空类别抽取会改变任务定义。四项公开头复核是为闭合 `_Z` 歧义，不冒充 C_ONLY 抽样。
