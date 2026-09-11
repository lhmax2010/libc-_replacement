# 全部问题与替换文本

定位为原文件行号；API＝`HQ_files/api_compatibility_report.md`，
ABI＝`HQ_files/abi_comparison_preliminary.md`。中英 Chromium 分别简称 C-ZH／C-EN。
以下仅给出供人工采用的修改，不改原报告。相同问题多处出现合并列出，不能只修其中一处。

## 必须改

### M01　接口总表把条件性分析写成通用实测安全结论

位置：API 11–19、48–73；ABI 15–24。
证据：R78 只测 C ABI 插件与 ICU 单函数，R80 只测 33 个标准库类型指标；
`Engine`／`Rect` 对应跨库成员调用没有原始实测。非静态方法还有隐式 `this`，不能只看显式参数。
已有 Boost.Thread 的特定 once_flag 边界取得精确状态通过，这是有界的 C++ ABI 交互样本，
但并不是 Engine／Rect 类接口的实测，也不能据此推广全部内置类型／POD 接口。
性质：**静态核查＋有限实测回查**，不是断言这些接口不能实现。

替换概要与表中“已实测可以”为：

> 接口传递的数据、对象生命周期、异常／取消路径及实际 ABI 配置共同决定兼容条件。
> 已实测的 C 接口样本在其固定环境与输入下通过；不代表任意 C 接口天然安全。
> 仅传内置类型或 POD 的 C++ 接口，在两侧目标 ABI、完整类型定义、调用约定、编译选项、
> 对象生命周期及异常边界一致时，原则上可设计兼容边界；这是条件性分析。
> 本文的 Engine／Rect 示例尚无相应跨库实测，不能标为“已实测支持”。

标准库对象、异常与所有权三行如保留“不支持”，改为：

> 当前支持范围不承诺这类未统一 ABI 的直接跨界用法；已有反例证明链接成功不足以保证正确，
> 不是断言每个类型、所有组合均必然发生同一种错误。

### M02　“安全”异常适配示例缺取消与指针前提

位置：API 81–98、75–79。代码加头后可编译，但 `*out` 要求有效指针；catch-all 会截获
可到达的强制展开，不能把吞取消后返回错误码作为通用安全模板。
证据：原块语法检查 `raw/syntax/api_03*`；R116 的 `code/api_probe.cpp` 明确 catch 后重抛；
R69 补丁逐处在 catch-all 前辨识／重抛强制展开；R78 跨库取消样本确认展开可穿越边界。
性质：**源码核查／条件性风险，不是新增运行实验**。

替换“安全——挡住异常”及紧邻说明为：

> 下面只示意普通 C++ 错误转错误码：前提是 out 指向有效可写 int，且本调用区间不会
> 发生线程取消。实际接口还应明确空指针／错误输出语义。允许线程取消时不得用
> catch (...) 吞掉强制展开；需在匹配的运行时中辨识并继续传播，且另外核验完整调用链
> 的清理与不抛边界。这个简化示例不构成该取消场景的安全实现。

若保留代码，在 try 前加 `if (out == nullptr) return -3;`，并仍保留上述取消前提。
这不是建议平台禁用取消，也不是已经核准一个通用 C 异常桥接实现。

### M03　异常反例被替换成未测类型，机理过度简化

位置：API 18、101–103、173–189；ABI 20、131–132。
原始依据：[共享异常类型](../R78/code/boundary_error.hpp)、
[旧插件](../R78/code/old_plugin.cpp)、[接收方](../R78/code/new_main.cpp)、
[逐场景记录](../R78/tables/scenario_matrix.tsv)、[R78 原材料入口](../R78/REPORT.md)。
原始代码路径在仓根为 `docs/progress/R78/code/`。
两架构测的是简单聚合 `BoundaryError { int code; }`，不是 runtime_error 派生类。
libc++abi `cxa_personality.cpp` 的 typed catch 分支先检查 native_exception；foreign 不进入
该类型匹配。不能把这一机制等同于“所有 RTTI 类型信息都丢失”。

替换机理与“已实测”段为：

> 在所测 libstdc++ 插件→libc++ 接收方组合中，插件抛 BoundaryError{77}；
> x86_64 原生和 armv7l 实板均未进入同类型 catch，而进入 catch-all 并退出 12。
> 相关 libc++abi personality 将非本运行时异常视为 foreign，普通 typed catch 不匹配
> 这一路径。结果依赖实际异常运行时、personality 和符号绑定，不能外推为任意 DSO、
> 方向或类型均不可按类型捕获。EngineError 派生类本身未测。

代码改用原 `BoundaryError` 形态，或明确把 EngineError 标为“说明性伪代码、此类型未测”。
对于 non-throwing 边界改写为“异常实际越过其边界时会触发终止”，不是只要调用链任一处
存在 noexcept 就一定终止。

### M04　分配释放示例和“两套独立堆”解释错误

位置：API 105–121；ABI 21。
`free(name)` 不调用 operator delete；对 new[] 结果调用 free 是分配释放 API 不配对，
同一套库也不能这样用。源码显示 GNU 普通 operator new/delete 与 LLVM 默认实现均可
落到 malloc/free；仅因加载两套 C++ 库不能推成两套独立堆或必然破坏堆。
证据：`codes/gcc/libstdc++-v3/libsupc++/{new_op,del_op}.cc`、
`codes/llvm/libcxxabi/src/stdlib_new_delete.cpp:54,123`，见 raw/012、015。

替换注释与解释为：

> `new[]` 必须与相应 `delete[]` 配对，不能用 `free`；这一错误与是否跨标准库无关。
> 跨组件释放还需符合实际分配器／替换 new-delete、对齐、析构、对象布局和模块生命周期
> 契约，不能仅由库名判断。提供方给出配对释放函数，或调用方提供缓冲区，可以避免让
> 使用方猜测这些内部契约；这不等于无条件完成异常、取消及有效指针验证。

### M05　智能指针跨界风险的因果解释错误

位置：API 193–200。
`shared_ptr` 不会仅因跨边界就“双方各自计数”；正常副本指向控制块，混合实现可能错误
解释布局、计数编码、虚函数或 deleter。`unique_ptr` 也可带自定义 deleter，不是天然固定
调用提供方那个 delete。证据：`codes/llvm/libcxx/include/__memory/shared_ptr.h`、
`__memory/shared_count.h` 与 `libcxx/src/memory.cpp`；R80 对两类仅确认外层大小／对齐。

替换为：

> 直接跨标准库传递智能指针不在当前支持承诺内。风险包括智能指针及控制块布局、引用
> 计数协议、析构／deleter 与模块生命周期不兼容；不能由外层大小相同排除。
> shared_ptr 不是因跨边界自动产生两份独立计数，释放路径也不能只由“哪一侧分配”决定。

### M06　把大小／对齐叫完整布局，测量工具版本混淆

位置：API 136–150、278；ABI 5、30、32、50–56、127。
33 个具体类型中 10 个 size 或 align 不同、23 个均相同成立；成员级尚未检查。
`function<void()>` 对齐 **8／16** 也不同。GCC 14.2 是所查平台源码，布局探针用 GCC
13.3.0；LLVM 侧编译器为缓存 Clang 21.1.1，链接 T4 平台 libc++／libc++abi，不应把
Clang 与运行库版本合写成一套测量工具版本。见 R80 raw/017、032、041 和 code/layout_probe.cpp。

替换为：

> 在 x86_64 固定探针配置下，33 个所列具体类型实例中，10 个 sizeof 或 alignof 不同，
> 23 个这两个指标相同；没有完成成员偏移、节点、控制块或操作语义等价性验证。
> GNU 侧使用宿主 GCC 13.3.0；LLVM 侧使用 Clang 21.1.1 与 T4 平台 libc++／libc++abi。
> 对应 T4 libc++ 头中的版本宏为 220108，即 22.1.8；它不是 Clang 编译器版本。
> 平台 GCC 14.2 源码对照及导出清单是另一个证据输入，不能冒充这次探针编译器。

小节标题分别改为“大小或对齐不同的类型”“大小与对齐相同的类型”，数值表原数保留。

### M07　deque 实测经过改写、退出语义和符号规则过宽

位置：API 152–169；ABI 60–74。
依据：R80 code/layout_{boundary.h,provider.cpp,consumer.cpp}，raw/036。

替换实验说明为：

> 原实验的 provider 返回 `make_payload()` 构造的 `std::deque<int>{11,22,33,44}`。
> provider／consumer 的 sizeof 为 80／48，长度视图为 4／106884723786536。
> 链接和加载成功，没有信号崩溃；consumer 主动返回 42 标记错值，并调用 provider 的
> destroy_payload 释放对象。该错误长度仅是当次输出，不是任意运行都固定得到的值。

若保留改名示例，必须标“简化改写，非原始实测代码”，不能把原错值当成改写版已测输出。
原函数名编码为 `_Z12make_payloadv`；`_Z9make_datav` 是 make_data 的正确编码，但不是
这份原始实验的符号。两者已用 c++filt 对照，后者不是拼写算错。

替换普遍链接说法为：

> 此处普通非模板函数的返回类型未直接编码进 Itanium 修饰名，因而本例可链接。
> 不是所有 C++ 返回类型或签名都不影响符号：模板函数和函数类型等规则不同，ABI tag
> 也可能使名字不同。原实验先尝试的 std::string 版本就因 abi:cxx11 tag 链接失败。

[Itanium 规则](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling-function)。

### M08　转换层的“不可能”与强制复制说法无依据

位置：API 215–239、248–255。
“一个编译单元只能链接一套标准库”混淆编译时头文件配置与最终链接；R78／R80 的进程
就是双库共存。异常可在提供方捕获后编码；迭代可用句柄／批次协议；所有权可用配对释放
函数（本报告自己已给出）。是否可转换及代价取决于接口，不必每次复制。

替换为：

> 通常分别以各自头文件和 ABI 编译桥接两侧，以明确的数据表示或句柄交换。
> 不能把两套 std 对象当成相同布局直接解释。普通异常、迭代和所有权可以设计显式协议，
> 但这不是任意 C++ 对象的透明转换；线程取消尤其不能当作普通错误吞掉。
> 转换的复制、生命周期和维护成本依具体接口决定，本轮未实现或测量通用转换层。
> 当前不采用通用转换层是处理选择，不是已证明它在技术上没有设计空间。

判断表的“不能”列改为“不可直接跨 ABI 解释，需另设计协议”，避免与 opaque handle
和配对释放示例矛盾。“用纯 C 能表达”可作筛查启发式，不是充分安全判据。

### M09　导出符号数的来源及计数对象写错

位置：ABI 80–95、162。
raw/026、028 已重放原始 parser；`6057=4546 FUNC+1511 OBJECT` 是 GCC baseline
manifest 的去版本键，不是纯函数 API 数。1,969 来自 T4 已定义导出文本。
交集忽略版本，不能叫“版本化符号完全相同”；构造／析构 ABI 变体也各计一个符号名。

替换表头与来源为：

> 去除版本后按符号名去重的输入条目数：GCC x86_64 基线清单 6,057；
> T4 平台 libc++ 已定义导出记录 1,969；精确同名交集 17。
> 这不是全部公共 C++ API 个数，也不是两份现场库用同一命令测得的直接性能／规模比较。
> 17 项在 GNU 为 GLIBCXX_*／CXXABI_*，在 LLVM 为 LLVM_22，版本契约不相同。

### M10　“17 个尚未核实语义”等过时缺口

位置：ABI 85、129。
R80 `tables/d09_common17.tsv` 已逐项列标准层含义；`raw/d09_*_run.txt` 分组输出相同。

替换为：

> 17 个同名符号已逐项静态核对标准层用途；在各自正确配对的运行时中，分组行为探针
> 输出一致。尚未证明的是错误配对、符号抢占／dlsym、双运行时 TLS、异常对象跨 provider
> 复制析构等组合的可互换性，不能由同名同用途推出任意混用安全。

### M11　取消表混用基线、补丁和计数单位

位置：ABI 103–109、145。
本轮固定 GCC 14.2 源树 `49c50605…` 的 include/src/libsupc++ 源文本，带对照扫描出
65 个 typed-catch 句法点／16 文件（含扩展、兼容源码及未求值条件分支）。未补丁 LLVM
`5ed6c772…` 的 libcxx/include、src 中为 0；不包括 libcxxabi 的识别逻辑。
第一批补丁 50 保护站点＝20 头侧＋30 库侧；20＋14＋2＝36 个新增 typed handler。
两处汇聚 handler 覆盖库侧 16 个站点，而不是 16 个新增子句。

替换表前说明及相关两格为：

> 本表对照固定未打本地补丁的源码，不代表已修补运行库的当前状态。
> GNU 侧按生产源码 typed-catch 句法点计为 65；LLVM 原始 libcxx 对应目录中该显式
> 识别子句为 0。此数字不表示编译后实例、可达取消点或待修复问题数。
> 第一批补丁按原审计口径覆盖 50 个站点，实际新增 36 个 typed handler；两种单位分列。

出处：HANDLER_SITES.tsv、raw/013；R69 `code/evidence/commands/013_three_tree_gate_semantic_audit.log`。
若总部不需要具体 GNU 计数，可用“固定 GNU 源码存在显式识别并重抛子句”而不保留无口径约数。

### M12　pthread_rwlock 被说成内核管理、无取消问题

位置：ABI 109–112、148。
glibc 2.40 pthread_rwlock 本身有用户态状态／原子操作／futex，并明确“不支持获取时取消”。
libstdc++ 后端受 `_GLIBCXX_USE_PTHREAD_RWLOCK_T` 等宏控制，也有 CV fallback。
未修补 libc++ 写者状态残留要限定在能展开越过等待边界的区间；保留规格时可能先终止。

替换为：

> 在所核对的 pthread 后端配置中，libstdc++ 使用 glibc 的读写锁状态机；该实现的获取
> 路径不提供延迟取消点，不能称为“取消后由内核自动恢复”。libstdc++ 另有条件变量
> fallback，不能从架构名直接推广到全部构建。libc++ 原写者在预占写者位后等待，
> 若取消展开可以穿过该等待而无回滚，会留下状态；本批已经加入相应回滚。
> 两种后端均不能据此宣称任意异步取消安全。

出处：R114/W2 原 glibc 快照 raw/012:208–209、platform_shared_mutex.hpp；
R114/W1、R115/W4、IMPL_0908/W3 的状态矩阵。

### M13　“一律对齐”被写成实际全等，遗漏已知行为差异

位置：ABI 139–154；API 276–281 的总范围。
第六节开头确实写了“处理原则”，这点**已经区分主观原则，不能指责完全未标注**。
问题在后续绝对化及“做不到”不是证据结论：已知两库错误检查、取消响应、缓冲字节不同。

替换节首、节尾及“对象布局”处理说明为：

> 本节陈述本项目已采用的处理原则，不是标准强制要求，也不是两套库全部行为等价的
> 实测结论。以下只描述已裁决项目的具体对齐目标及实现边界；对象布局和符号集合不在
> 本轮追求互换的范围内，不能由“标准不要求”推成“技术上绝无可能”。
> 等待取消的展开目标、普通错误处理、读写锁取消响应及析构输出需分别说明：
> libc++ 仍保留其错误检查；未捕获普通错误仍可终止。wbuffer_convert 对齐的是析构
> 不再自动刷新，而不是字节数：新／旧／GNU 探针为 3／9／0，缓冲算法未改。

依据：R109/W1 锁所有权实测；R111/ITEM3 未捕获矩阵；IMPL_0909/W1 三方记录。

### M14　C 接口示例及 ICU 结论范围

位置：API 29–52。
头文件独立编译缺 `<stddef.h>`；加该头的 C11 正向控制通过。ICU “双方行为完全正常”
不符合单入口覆盖强度。

替换头文件首行加 `#include <stddef.h>`；替换实测段为：

> armv7l 物理板上，libc++ 主程序通过 dlopen/dlsym 调用既有
> `/usr/lib/libicuuc.so.78.1` 的 `u_getVersion`，输出 78.1.0.0，断言主版本为 78，
> 并记录 libc++、libc++abi、libstdc++ 与 ICU 同时加载，退出 0。
> 这证明该固定 C 入口样本可用，不是 ICU 全接口、分配释放、回调或产品功能验收；
> x86_64 的真实 ICU 同形运行在该材料中为 NOT_OBSERVED。

### M15　架构范围与已知限制缺漏

位置：API 278–281；ABI 116–135 及取消表。
真实 ICU 只有 armv7l 实板；R82 Boost 的 armv7l 是 QEMU。
R110 已有 **aarch64 QEMU** 的 Boost 边界新增 22 条，累计代表组 49/90，不能说一切
aarch64 运行时验证均未做。33 类型布局未作 ARM 全表是另一件事。

替换范围段为：

> 33 个类型的大小／对齐及 deque 最小实例限 x86_64；ICU 单 C 入口限 armv7l 实板；
> 受控 C ABI 与 BoundaryError 异常有 x86_64 原生和 armv7l 实板记录。
> Boost 另有 x86_64、armv7l QEMU 与 aarch64 QEMU 的代表边记录；并非所有边、所有
> 符号或真实业务调用都覆盖。aarch64 的完整取消／本批官方套件等验证仍未完成。
> 未覆盖还包括任意旧调用方、额外 noexcept／无异常帧、任意模板／回调、异步取消、
> 自定义分配器及实际插件加载组合、全量成员布局与所有第三方接口；详见配套支持范围。

ABI 未覆盖列表另将“17 未核查”改 M10；不要把已有普通异常机制核查全部写成空白。
附上当前支持声明的旧／新四绑定表与头文件重编限制，不能暗示换库可修所有旧对象。

### M16　Chromium 搬运后缺失全部相对证据目标

位置：C-ZH 28 处、C-EN 29 处；详见 RELATIVE_LINK_REPLACEMENTS.tsv 的逐行目标。
原目录可用，HQ 目录不可用；这不是内容被转述改写。

替换方式：用表中每个 `suggested_target` 替换相应 Markdown 链接目标。
例如两稿的 `(MEASUREMENTS.md)` 改为：

`(https://github.com/lhmax2010/libc-_replacement/blob/859126994bd7e3b8bb03ee86154e23e1b04b2153/docs/progress/R116/MEASUREMENTS.md)`

表为**具体建议文本**，没有实际批改报告。也可由人工决定连同保留目录关系的完整证据包
一起交付；仅发送当前 HQ 四文件不满足可点开核查。新目标对应已推送提交，不是工作区临时路径。

### M17　Chromium 五个固定源码 URL 指向错误仓结构

位置：C-ZH／C-EN 的条件变量、等待头、写者、定时写者、转换缓冲来源链接。
全文 36 个唯一 HTTP URL 中这五个 404；不是新增节的 13 个 URL 失败。
原始抓取使用 **libcxx 分仓**，报告却写成 llvm-project 总仓加 libcxx 子目录。

逐条用 CORRECTED_URLS.tsv 的 replacement_url 替换；例如：

> 条件变量实现：
> https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx/+/97b436da4c33663581d394f4ee0a5977fc38c2f4/src/condition_variable.cpp

其余路径为 `include/__condition_variable/condition_variable.h`、`src/shared_mutex.cpp`、
`include/shared_mutex`、`include/__locale_dir/wbuffer_convert.h`，共用上述分仓和固定 revision。
五条正确地址的 TEXT 内容均重新抓取、解码后与原始快照逐字节及 SHA256 一致（raw/020）。

## 建议改

### S01　其余示例明确是片段，不是完整可编译程序

API 81、107、129、154、173、193、204 起始各块。
替换说明：“以下为接口片段，省略头文件和实现；提供方与消费方属于不同翻译单元。
省略号是伪代码，调用语句须置于函数体。各 C 接口方案是替代选项，不能作为同名 C 重载一起导出。”
详见 CODE_REVIEW.md；不要把所有省略写法一律当成算法错误。

### S02　Chromium 导出组合数再明确 ABS 版本节点

198／231／108 原计数可重算，不须擅自更改。补一句：
“此口径包含平台表的 15 个 ABS 版本节点名；不是 108 个缺失函数或 108 个失败消费者。”
原 parser 对已定义 FUNC/IFUNC/OBJECT 计数，未排 ABS；见 raw/020。

### S03　八处、两处、七类等给外部读者展开名称

没有残留内部任务代号，但只写数量仍不易理解。可替换为：
“GNU 独有／无同形保护的八个源站点：实验 executor 四处、GNU vstring 两处、
condition_variable_any 重锁守卫一处、TR2 dynamic_bitset 一处；不是当前 LLVM 下实测的八个故障。
两处既有不可达保护指 basic_string::shrink_to_fit 和 basic_ostream<char>::sentry 析构的
不抛边界，不能混同于后续已修复的两项代码缺陷。”
依据 R46/tables/no_counterpart_8.tsv、R58/KNOWN_GAPS.md；本轮未为其新增运行结果。

### S04　CEF／ICU 行增加直接原始出处

API 241–242 可改为：
“CEF 通过 C API 隔离运行时，并提供 C++ wrapper；ICU 对跨版本二进制兼容提出稳定 C API
等条件。这是有条件的接口设计参照，不是任意 C 边界均安全的证明。”
[CEF 项目维护者说明](https://github.com/chromiumembedded/cef/issues/3836)、
[ICU 官方条件](https://unicode-org.github.io/icu/userguide/icu/design.html#icu-binary-compatibility)。

### S05　工期和未来工作承诺区分事实

ABI 118 的“预计数日”没有测量或工作分解依据；API 268–272 的排序与“我们会做／只有我们
能查”是计划／组织分工，不是已完成核查。
替换：“以下项目尚未完成；工期需另行确认。平台可得源码由平台侧核查，外部实现需其
提供方提交接口、构建、生命周期与异常／取消契约。工作安排由人工确认。”

### S06　对齐目标、证据日期与英文派生稿同步

API／ABI 有英文派生文件但未列入本次逐段范围。建议补统一版本号／证据截止时间，
并注明“本报告不是全部产品验收结论，支持范围以候审声明为准”。
Chromium 已有证据截止日期与严格限制，保留即可，不把 HTTP 今天可访问当成今天重新验证了产品。
