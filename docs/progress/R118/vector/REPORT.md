## std::vector

### 概要

**PARTIAL。** 基础为 C++17，另有表 C 的两项 C++23 补测；x86_64 原生与 armv7l 物理板。两库身份、构建配置与 string 阶段相同。基本实例为 `vector<int>`，另独立覆盖 `vector<bool>` 与 `vector<string>`。不能由一种实例外推任意元素/分配器。

| 差异类别 | 本表项目数（不是差异总数） | 影响开发者 | 需改代码 |
| --- | ---: | --- | --- |
| 布局 | 3 | bool 特化不同；普通实例相同不代表混用受支持 | 跨库传对象时需处理 |
| 行为 | 6 | 本轮多数语义相同，异常文字不同 | 不依赖诊断文字、增长策略 |
| 接口 | 5 | 扩展单侧可用；部分 C++23 接口实现进度不同 | 用这些接口时可能要改 |
| 跨 DSO | 2 组（1 组真实实测，1 组未观测） | 实际回调仍缺口 | 不能宣布支持 |

完整数值见 [VALUES](VALUES.md)；[原生矩阵](x86_64_matrix.json)、[物理板矩阵](armv7l_matrix.json)。所有可运行格 5 次，具体内容断言；源码/命令/输出与 SHA 随附。平台源只读，未重编真实 provider。

### A. 布局

| 项 | libstdc++ | libc++ | 测试程序 | 两架构结果 | 影响 |
| --- | --- | --- | --- | --- | --- |
| vector<int> sizeof/alignof | 24/8；12/4 | 24/8；12/4 | [layout](../code/vector/layout.cpp) | 顺序 x86_64；armv7l，每格 5 次 | 同尺寸不是兼容证明 |
| begin/end/capacity 端点 | 0/8/16；0/4/8 | 0/8/16；0/4/8 | [endpoints](../code/vector/endpoints.cpp)，以及 layout 编译器输出 | 运行时 size=3、capacity=7 使三个端点不同；从对象表示读到唯一偏移，静态字段再交叉核对 | 第一版 size=capacity 时两个端点相同，原始记录保留，不据此识别两字段 |
| vector<bool> sizeof/alignof | 40/8；20/4 | 24/8；12/4 | [layout](../code/vector/layout.cpp) | 两架构均不同；代理引用大小为 16；8 | bool 特化不是普通 bool 数组 |

两侧源码：[GNU stl_vector.h](source/stl_vector.h) 93–95 行的三个指针；[LLVM vector.h](source/vector.h) 的 `__begin_`、`__end_`、`__cap_`。bool 对照 [GNU stl_bvector.h](source/stl_bvector.h) 的 `_Bvector_impl_data`（两个位迭代器与尾存储指针），[LLVM vector_bool.h](source/vector_bool.h) 的 `__begin_`、`__size_`、容量及分配器。完整私有成员、位迭代器及填充偏移在 `vector_*_layout_*_record_layout.stdout`，不是伪造 `offsetof` 的结果。普通 vector 没有测到 string 式内联元素缓冲；这是该配置源码和样本观察，不是未来实现限制。

### B. 行为

| 项 | libstdc++ | libc++ | 测试程序 | 两架构结果 | 影响 |
| --- | --- | --- | --- | --- | --- |
| 默认、范围、拷贝、移动、赋值与嵌套 | 内容正确 | 内容正确 | [construction](../code/vector/construction.cpp) | 两架构各 5 次：3,5,8；嵌套字符串长度 1,3，内部字符 c 保留 | 不是任意元素构造/移动性质证明 |
| push_back / reserve / shrink | 0→1→2→4→8→16→32→64→128→256；reserve(23)=23；100 缩至 3 | 固定样本完全相同 | [growth](../code/vector/growth.cpp) | 两架构各 5 次 | “本样本无差异”不等于容量策略有标准固定值 |
| at 越界 | out_of_range；消息含 index=2,size=2 | out_of_range；消息 `vector` | [at](../code/vector/at.cpp) | 两架构内容仍 5,9 | 不匹配固定消息文本 |
| 分配失败 | bad_alloc，size/capacity/2,4,6 不变 | 同左 | [allocation](../code/vector/allocation.cpp) | 两架构各 5 次 | 仅 int、reserve 与确定性 new 注入；可抛移动构造的元素未覆盖 |
| 迭代、失效与 C 数组连续性 | 无扩容 push 后旧 first 可读为 1；erase 返回 3；扩容地址变化，元素跨度 1 | 同左 | [iteration](../code/vector/iteration.cpp) | 顺序为 1,3,4；不读已经失效的迭代器 | 无扩容也不保证被删/插入点之后的旧迭代器有效 |
| bool 代理与位容量 | 代理不是 bool&；翻转后 000；容量按 64 位 /32 位块增长 | 本样本相同 | [bool](../code/vector/bool.cpp) | x86：64→128→256，缩至64；ARM：32→64→128→256，缩至32 | 不能把代理地址当 bool* 交给 C；位操作并发另有约束，未实测 |

增长/异常/缩容源码：[GNU vector.tcc](source/vector.tcc)、`stl_vector.h::_M_check_len/_M_range_check`；LLVM `vector.h::__recommend/at/reserve/shrink_to_fit`。本轮没有用“仍可读”的未定义操作验证失效；规则参照 [容量条款](https://eel.is/c++draft/vector.capacity)、[插入和删除](https://eel.is/c++draft/vector.modifiers)。[bool 特化的代理定义](https://eel.is/c++draft/vector.bool.pspc) 与源码/实测分开列出。公开草案只作约束说明，C++23 后新增接口不属于本轮 C++17 运行矩阵。

### C. 接口

| 项 | libstdc++ | libc++ | 测试程序 | 两架构结果 | 影响 |
| --- | --- | --- | --- | --- | --- |
| debug/vector / __gnu_debug::vector | 编译，5 次 size=2、values=2,5 | 无此头，编译失败 | [gnu_extension](../code/vector/gnu_extension.cpp) | 两架构一致 | GNU 调试扩展，不是标准 vector 接口承诺 |
| vector::__invariants | 无成员，编译失败 | 编译，5 次 invariants=1、size=2 | [libcxx_extension](../code/vector/libcxx_extension.cpp) | 两架构一致 | 不依赖实现内部式名称 |
| vector<bool>::data | 编译失败 | 编译失败 | [bool_data_extension](../code/vector/bool_data_extension.cpp) | 两侧都失败；普通 vector<int>.data 是正向控制 | **不是单侧缺接口的差异** |
| C++23 from_range 构造 | 无匹配构造，编译失败 | 编译运行 5 次 size=3、values=2,5,9 | [from_range](../code/vector/cxx23_from_range_extension.cpp) | 两架构，-std=c++23 | 标准接口实现覆盖差异，不是 LLVM 扩展 |
| C++23 append_range | 无成员，编译失败 | 编译运行 5 次 size=3、values=2,5,9 | [append_range](../code/vector/cxx23_append_range_extension.cpp) | 两架构 | 本例可考虑以 insert(end,begin,end) 表达；复杂 range 需另核 |

范围骨架相关行见 `R117_entities.tsv` / `R117_members.tsv`。未把“单侧解析命中”直接升级为缺 API；未逐重载穷尽接口差集。

补测源码：LLVM vector.h:230、487；GNU stl_vector.h 对应类型未提供这两个接口。源码检索及同一程序的双侧编译分别保存，GNU 其他普通构造与 LLVM 新接口成功作为控制，不仅凭关键词零命中。

### D. 跨 DSO

| 包边界 | 传递方式 | 实测结果 | 影响 |
| --- | --- | --- | --- |
| 消费探针 → 真实平台 boost-program-options | split_unix 返回 vector<string>；含 string 输入 | 两架构：同侧 5 次 size=3，逐字节 alpha / two words / omega，消费侧销毁完成；两个混合方向均自然链接失败 | 该失败涉及签名中的 string，实现命名空间挡住了调用；**没有隔离证明 vector 本身的跨库运行效果** |
| 引擎 → webapi-plugins 的 SetSyncBinaryReply | const vector<uint8_t>&，函数指针回调 | `NOT_OBSERVED`：本轮未取得真实实例注册、回调和生命周期的可运行夹具；既有数据层演示不是替代 | 三指针相同不足以宣布实际回调安全 |

实测见 [x86_64 边界](x86_64_boundaries.json)、[armv7l 边界](armv7l_boundaries.json)、[独立程序](../code/boundary/boost_vector.cpp)。公开回调头原文位于 `docs/progress/P7_0909/stage2/snapshots/evidence/webapi-plugins/353da9cbd3a2_XW_Extension_SyncMessage.h:36`，消费位置 `e4597e6f32ca_extension.cc:348`。该组没有新增真实跨库成功或失败的运行结论。

### 开发者须知

本次普通整型数组的构造、增长和内容测试两边相同，换库不意味着要改写每个 vector。仍需避开对容量倍数、错误消息的依赖。增删元素后按规则更新迭代器，别因为内存地址没变就继续使用旧位置。

`vector<bool>` 会把多个布尔值压在机器字里，它不是能直接交给 C 函数的 bool 数组。需要字节数组的接口应显式准备字节缓冲和长度。

把 vector 对象直接交给另一套库编译的组件时，即使对象一样大，也没有自动获得兼容保证；里面的元素可能又是不同布局的字符串，释放过程还会调用各自的代码。跨组件数据可采用元素指针与数量的明确约定，但谁拥有和释放缓冲仍必须说明。

### 本类型未覆盖的

真实 SetSyncBinaryReply 注册/回调链；没有凑足 3–5 个真实独立包边界。未测自定义分配器、抛出移动/拷贝的元素、任意嵌套类型、所有语言模式与重载、debug/hardening 组合、并发、取消。普通 vector 没有标准内建 hash 专用化，不套用 string 的 hash 测项；未把不可用操作写成库差异。5 次证明固定格可重复，不构成总体安全证明。
