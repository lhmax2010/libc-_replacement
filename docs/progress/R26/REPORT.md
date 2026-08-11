# R26：跨界 ABI 边的语义性质判定

## 结论摘要

本轮只复用 R11/R22/R24/R25 已落盘数据、下载 RPM和冻结符号交集，没有重扫平台仓或 ELF，没有构建、安装或修改既有输入。

- 1,191 条去重跨切面边中：**CXX_SEMANTICS 1,178（98.908%）/ C_ONLY 0 / UNDETERMINED 13（1.092%）**。
- 75 个 provider 中：**74 个至少有一条 CXX_SEMANTICS 边**；全部边为 C_ONLY 的 provider 为 0；含 UNDETERMINED 但无 CXX_SEMANTICS 的 provider 为 1。
- 因此只计实证 CXX_SEMANTICS 时，对应 provider 数为 **74/75**；把 UNDETERMINED 也计入时为 **75/75**。
- 13 条不可判定边中，12 条共同指向 `rust-proc-macro2-1.0.51-1.src.rpm`，是 Rust 旧式 `_ZN` mangling 与 R22 `_Z` 前缀过滤器碰撞；另 1 条是 `tizen_base::logger` 数据符号，公开头未找到声明、类型未编码。
- C_ONLY 总体为 0，因此 C_ONLY 抽样总体/样本量均为 **0/0**，没有静默跳过。

这些数字只陈述冻结动态符号图的语义证据，不评价处置方式。

## 一、判定方法

R22 的原始命令为 `readelf --dyn-syms -W <ELF>`；消费者 UND 与 provider DEF 去版本后求交。R26 对冻结交集运行 `c++filt`，按肯定证据分类：

- `std::`、`[abi:cxx11]`；
- RTTI/typeinfo、vtable/VTT、thunk；
- 异常/throw/catch/terminate；
- 构造、析构；
- 明确的自定义类参数；
- cv/ref 限定成员函数的隐含类对象；
- 公开头实证的类方法或类静态对象。

没有肯定证据就留 UNDETERMINED。R22 真边定义要求每边至少一个 `_Z` 风格交集，因此“全部引用均为无修饰 C 符号”的 C_ONLY 在这 1,191 条总体中结构上为零。命令、规则和限制分别见 `classification_method.tsv` 与 `METHODOLOGY.md`。

## 二、逐边分类

| 分类 | 边数/1,191 | provider 数/75 | 消费者数/372 |
|---|---:|---:|---:|
| CXX_SEMANTICS | 1,178 | 74 | 360 |
| C_ONLY | 0 | 0 | 0 |
| UNDETERMINED | 13 | 2 | 13 |

消费者/provider 列按每类去重，类别之间可能重叠：bundle 同时有 CXX 和 1 条不可判定边。完整逐边结果在 `edge_semantic_classification.tsv`；75,163 条逐符号肯定/不可判定证据在本地完整表 `semantic_evidence_symbols.tsv`，策展副本按 5 MB 限制拆分且有映射表。

边级原因：1,176 条由符号本身确认；1 条由 LLVM 公开头确认类方法；1 条由 FlatBuffers 公开头确认类静态对象；12 条 Rust 碰撞；1 条公开头仍无法确定。gtest 边的 `[abi:cxx11]` 已由符号直接确认，公开头作为补充复核。

## 三、逐切面分布

| 切面 | 边分母 | CXX_SEMANTICS | C_ONLY | UNDETERMINED | CXX provider | UNDET provider |
|---|---:|---:|---:|---:|---:|---:|
| 1 阶段一完成 | 953 | 952 | 0 | 1 | 9 | 1 |
| 2 阶段二层 1 完成 | 574 | 562 | 0 | 12 | 51 | 1 |
| 3 阶段二层 2 完成 | 259 | 259 | 0 | 0 | 50 | 0 |
| 4 阶段二层 3 完成 | 118 | 118 | 0 | 0 | 32 | 0 |
| 5 阶段三完成 | 12 | 12 | 0 | 0 | 6 | 0 |

同一去重边可跨多个切面，因此本表各行不能相加为 1,191。

## 四、provider 维度

| provider 类别 | provider 数/75 |
|---|---:|
| 至少一条 CXX_SEMANTICS | 74 |
| 全部边为 C_ONLY | 0 |
| 含 UNDETERMINED、无 CXX_SEMANTICS | 1 |

唯一第三类 provider 是 `rust-proc-macro2-1.0.51-1.src.rpm`，对应 12 条 Rust mangling 碰撞边。bundle 的 69 条边为 68 条 CXX + 1 条不可判定，因此仍属于第一类。

跨界边最多的前五 provider 为：gcc 两个精确源码身份 345/331 条（均全 CXX）、gtest 121/121 条 CXX、bundle 68 CXX + 1 UNDET、aul 48/48 条 CXX。完整 75 行和前 20 分别在 `provider_semantic_summary.tsv`、`provider_top20.tsv`。

任务所问的两个机械数是：

- 只对“至少一条 CXX_SEMANTICS”provider 计数：**74**；
- 把“含 UNDETERMINED”也计入并去重：**75**。

## 五、C_ONLY 复核

C_ONLY 边为 0/1,191，故抽样总体为空、样本量为 0，误判修正为 0。不能从 CXX 或 UNDETERMINED 类借样本来伪造 C_ONLY 复核。

另对四个符号歧义做公开头普查式复核：LLVM `raw_ostream::write`、FlatBuffers `ClassicLocale::instance_`、gtest `UnitTest`/ABI-tagged flag 得到 CXX 实证；bundle `tizen_base::logger` 声明 NOT_FOUND，保持不可判定。命令和退出码在 `commands/002_public_header_reviews.log`，逐项结果在 `public_header_reviews.tsv`。

## 六、盲区

- 回调：在 1,191 条边随附的无修饰 C 符号中，名称 token 近似命中 **57 条边/16 个 provider/38 个消费者**。这只表示名称含 callback/register/handler/listener/hook/notify 等，不证明存在回调，更不证明回调会抛异常。
- `void*` 传递 C++ 对象与所有权：符号名无法恢复参数语义，规模 NOT_AVAILABLE。
- header-only inline/template：不产生跨包动态符号，不在图中，规模 NOT_AVAILABLE。
- 普通非模板函数返回类型：通常不进入 Itanium 名称；无法从名字判断返回的对象类型，规模 NOT_AVAILABLE。
- 静态库、dlopen、未唯一解析依赖和无动态 UND 的布局耦合继续位于输入图之外。
- Rust `_ZN` 碰撞：实测 12 条边/1 provider/12 consumers，已作为 UNDETERMINED 单列，没有冒充 C++ 或 C。

回调名称候选完整表见 `callback_name_candidates.tsv`；其余盲区见 `blind_spots.tsv`。不可量化项没有推断补全。

## 数据与覆盖边界

输入路径、SHA256 和大小见 `inputs/input_identity.tsv`；仓 revision、repomd SHA256 和发布形态说明见 `tables/data_sources.tsv`。本报告不做处置建议、可行性判断、风险评级或人日估算。
