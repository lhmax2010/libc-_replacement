## std::string

### 概要

**PARTIAL：两架构基础矩阵已实测；不能称“完整验证”。** 架构为 x86_64 原生与 armv7l 物理板；C++17，Clang 22 同一前端分别使用 GCC 14.2 头文件/运行库或平台 libc++ 22.1.8 实施版。GNU 使用 CXX11_ABI=1，未测旧字符串模式。平台 libc++ 不是未经修改的上游发行库，`raw/011_header_diff.stdout` 保存头文件与只读源码的差异（强制展开相关处理）；不把平台版本结果外推到所有构建。

| 差异类别 | 本表项目数（不是差异总数） | 影响开发者 | 需改代码 |
| --- | ---: | --- | --- |
| 布局 | 3 | 对象跨库直接传递受影响 | 有这种传递时需要处理 |
| 行为 | 8 | 精确容量、hash、compare 数值、诊断文本依赖受影响 | 依赖这些细节时要改 |
| 接口 | 3 | 扩展不通用 | 用扩展的代码要改 |
| 跨 DSO | 3 个接口组，2 个源码包族 | 自然链接失败与同名入口崩溃须区分 | 不能靠只换运行库修正 |

完整输入、数值与五次结果见 [VALUES](VALUES.md)、[x86_64](x86_64_matrix.json)、[armv7l](armv7l_matrix.json)。每个可运行格 5 次，断言语义与具体内容；差异值不要求在两库间相等。完整命令、输出、退出码在 `../raw/`；库身份见 [identities](identities.json)，源码快照与 SHA 见 [sources](sources.json)。

### A. 布局

| 项 | libstdc++ | libc++ | 测试程序 | 两架构结果 | 影响 |
| --- | --- | --- | --- | --- | --- |
| sizeof / alignof | 32/8；24/4 | 24/8；12/4 | [layout.cpp](../code/string/layout.cpp) | 顺序均为 x86_64；armv7l，每格 5 次 | 对象大小不同，不能按另一库的类型解释 |
| 成员偏移（编译器布局，非运行时私有成员访问） | 指针/长度/缓冲或容量：0/8/16；0/4/8 | 长模式：标记与容量位域在 0，长度/指针 8/16；4/8；短模式标记/长度位域在 0，字符缓冲 1 | 同程序的 `*_record_layout.stdout` | 私有成员及位域由实际目标编译器给出；空分配器/填充和匿名联合的完整层次保存在原始输出 | 不把位域当普通可取址成员，不修改 private |
| SSO 容量（不堆分配的字符数） | 两架构均 15 | 22；10 | 同上，逐长度 0–40，计 new 次数及缓冲位置 | 首次分配分别在 16 / 23 / 11；GNU 内联 data 偏移 16/8，LLVM 为 1/1 | 小串性能与跨边界解释受影响；不是固定标准承诺 |

两侧源码：[GNU 成员定义](source/basic_string.h) 191–213 行、[LLVM 短长表示](source/string) 822–918 行。配置可能改变布局；上述数字只属于记录的配置。

### B. 行为

| 项 | libstdc++ | libc++ | 测试程序 | 两架构结果 | 影响 |
| --- | --- | --- | --- | --- | --- |
| 默认/拷贝/移动/C 字符串/范围/赋值 | 内容断言通过 | 内容断言通过 | [construction](../code/string/construction.cpp) | 两架构各 5 次：空串长度 0，C 串长度 1，带 NUL 的计数字符串长度 4，range 内容 `range`；移动目标保留原值 | 移动后源的观测值不是通用保证 |
| push_back 增长 | 15→30→60→120→240→480 | x86：22→47→95→191→383；ARM：10→23→47→95→191→383 | [growth](../code/string/growth.cpp) | 每次扩容时的 size 与 capacity 均输出 | 不应硬编码扩容倍数或容量 |
| reserve / shrink_to_fit | reserve(100)=100；缩小后容量 15 | reserve(100)=103；缩小后 22/10 | 同上 | 两架构均有完整 request 序列；缩小前 GNU 200、LLVM 207 | shrink_to_fit 不是保证降到 size |
| at 越界 | out_of_range；包含 index/size 的消息 | out_of_range；消息 `basic_string` | [at](../code/string/at.cpp) | 两架构各 5 次；内容仍 `abc` | 捕获类型可保留；别依赖消息逐字相等 |
| 分配失败 | bad_alloc，原内容和容量不变 | 同左 | [allocation](../code/string/allocation.cpp) | 两架构各 5 次；100 个 q 保留，容量分别 100 / 103 | 仅 reserve 的确定性 new 注入，不是所有分配失败点的证明 |
| 迭代及地址稳定性 | 预留后 append 地址保持；超过容量后改变 | 同左 | [iteration](../code/string/iteration.cpp) | 内容顺序 `abcde`；保存的是整数地址，未使用失效迭代器 | 地址相同不证明任何操作下的迭代器契约；不能套用 vector 的全部规则 |
| c_str / data / npos | 尾零且 C++17 data 可写；npos 为 size_t 最大值 | 同左 | [c_api](../code/string/c_api.cpp) | 两架构：含 NUL 内容 `61007a`，size=3、strlen=1、末尾零；npos 分别 18446744073709551615 / 4294967295 | 传 C API 时明确长度；不把嵌入零后的字节当 C 字符串可见内容 |
| compare / hash / map key | 长度差样本 compare=-99；hash("abc")：3663726644998027833 / 3350977461 | compare=-1；hash("abc")：4220206313085259313 / 324500635 | [compare_hash](../code/string/compare_hash.cpp) | 两架构每格 5 次；比较符号相同，map 顺序均 a,z；其余 hash 见完整附录 | 比较只判断正负零；不能把实现 hash 当跨库/跨位宽持久标识 |

两侧源码定位：GNU `basic_string.h` 的 `_S_compare`、`compare`、`at`、`data`、`hash<string>`；增长见 `basic_string.tcc::_M_create`（135 起）。LLVM `string::__recommend`、`__grow_by`、`compare`、`__throw_out_of_range`、`__string_hash`，hash 算法在 `__functional/hash.h`。逐条原始行号检索保存在 `../raw/029_string_source_lines.stdout`，完整源码快照可直接核。

标准约束参照公开工作草案：[字符串失效及错误保证](https://eel.is/c++draft/string.require)、[容量](https://eel.is/c++draft/string.capacity)、[移动后的源状态](https://eel.is/c++draft/string.cons)。这些是约束说明，不把滚动草案的新增接口算作 C++17 实测接口。

### C. 接口

| 项 | libstdc++ | libc++ | 测试程序 | 两架构结果 | 影响 |
| --- | --- | --- | --- | --- | --- |
| ext/vstring.h / __gnu_cxx::__vstring | 编译及 5 次运行通过，size=9，text=extension | 头文件不存在，编译失败 | [gnu_extension](../code/string/gnu_extension.cpp) | 两架构均实际编译；GNU 控制排除程序本身错误 | 这是 GNU 扩展类型，不是 std::string 的另一个标准成员 |
| string::__invariants() | 无成员，编译失败 | 编译且 5 次返回 invariants=1、size=9 | [libcxx_extension](../code/string/libcxx_extension.cpp) | 两架构结果见独立 extension JSON | 实现内部式名称，不作为可移植接口依赖 |
| __resize_default_init 假设 | 编译失败 | 编译失败 | [member_extension](../code/string/member_extension.cpp) | 两侧都失败，**不计差异** | 不凭印象列非标准成员；保留失败发现过程 |

范围骨架的类型/成员抽取分别在 `R117_entities.tsv`、`R117_members.tsv`；单侧解析命中不是接口缺失证明。本轮没有逐重载、逐语言版本穷尽所有候选，C++20/23/26 条件接口仍属未覆盖。

### D. 跨 DSO

| 包边界 | 传递方式 | 实测结果 | 影响 |
| --- | --- | --- | --- |
| Chromium EWK 公开入口 → 真实 GNU chromium-efl | const string& 输入、含四个 string 的输出；同名 C 导出 | x86：GNU 同侧 5/5 四字段正确且销毁完成；LLVM 消费侧 5/5 SIGSEGV，双库/provider maps 均确认；ARM 尚未运行（诊断目录权限待确认） | 同名导出未提供对象转换；相反方向真实 libc++ EWK 不可得 |
| 消费探针 → 真实 boost-program-options，to_internal(string) | string 引用与返回值 | 两架构四组合：同侧每格 5/5 保留 `61006263` 并完成销毁；混合组合自然链接失败 | 参数中的实现命名空间不同，不能写成“链接器总会放行” |
| 消费探针 → 真实 boost-test，normalize_test_case_name | 平台真实 Boost 头的 const_string 输入，string 返回 | 两架构同侧每格 5/5 返回 `alpha_beta_gamma` 并完成销毁；混合自然链接失败 | 返回类型标签/符号身份不同；没有强制 dlsym 后冒充自然调用 |

原始矩阵：[x86 Boost](x86_64_boundaries.json)、[ARM Boost](armv7l_boundaries.json)、[x86 EWK](x86_64_ewk.json)。[Boost 程序](../code/boundary/boost_string.cpp) 与 [EWK 程序快照](../code/boundary/ewk_probe_snapshot.cpp)。实际 provider 路径及 SHA 在矩阵中；GNU/LLVM 两种 Boost 平台构建来自既有产物，不是模拟提供方。

实际使用线索：LWEWebView.h 的 LoadURL/GetURL（范围骨架来源），EWK 公共头及 Boost 符号边清单分别保存在既有 P7/R110 材料。**上述新探针不等于重跑这些业务包的原可执行文件**；三个接口组仅涉及两个提供方源码包族，没有凑成“3–5 个独立源码包边界已全测”。

### 开发者须知

普通字符串内容操作的样本保持一致，不需要因为换库就重写所有字符串代码。要改的是对内部细节的依赖：用 `compare()<0` 而不是比较它是否等于 -1；别把库的 hash 值持久保存成跨设备标识；别依赖固定容量或异常消息。

最危险的是把字符串对象直接交给由另一套库编译的组件。同样叫 string，内部位置和大小却不同；函数名字能对上也不代表对象能读对。组件双方应使用匹配的对象约定，或把边界改成字符指针与长度，并在各自一侧构造、释放对象。带零字节的内容必须保留长度，不能仅靠 strlen。

### 本类型未覆盖的

ARM EWK 崩溃矩阵等待诊断清理权限；GNU→真实 libc++ EWK 为 `NOT_AVAILABLE`。LWE 的实际初始化/调用链未在本轮完成。未覆盖所有 allocators/traits、旧 GNU 字符串模式、宽字符串、所有重载/语言模式、并发、取消、DSO 卸载后的悬挂引用。构造范围、异常点和 hash 输入都是固定样本，不是所有合法输入的证明。5 次重复说明固定条件可复现，不是随机样本置信度或普遍兼容证明。
