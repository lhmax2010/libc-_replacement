## std::function

### 概要

**PARTIAL：两架构布局、普通行为及小对象矩阵已实测；GNU 真实 LWE 同侧夹具已由后续任务修复，混合运行矩阵仍未完成。** 配置同前两份报告，C++17，x86_64 原生、armv7l 物理板。不是任意 lambda、任意函数签名及构建选项的总体证明。

| 差异类别 | 本表项目数（不是差异总数） | 影响开发者 | 需改代码 |
| --- | ---: | --- | --- |
| 布局 | 2 | 大小、对齐、存储和调用指针均不同 | 跨组件直接传递需处理 |
| 行为 | 5 | 移动后源状态、是否分配、错误文字不同 | 依赖这些细节时需改 |
| 接口 | 2 | GNU 扩展不通用；旧形式构造两侧均失败 | 扩展使用方需改 |
| 跨 DSO | 1 个真实提供方的公开回调路径；补充 GNU 同侧两架构各 5 次通过 | 混合格自然链接失败，不能宣布跨侧运行结果 | libc++ provider 仍不可得 |

完整数值与五次逐格结果：[VALUES](VALUES.md)、[x86_64](x86_64_matrix.json)、[armv7l](armv7l_matrix.json)，逐字节补测与扩展另有同目录 `*_threshold_matrix.json`、`*_gnu_extension_matrix.json`。源码、编译/运行命令及 SHA 随附。

### A. 布局

| 项 | libstdc++ | libc++ | 测试程序 | 两架构结果 | 影响 |
| --- | --- | --- | --- | --- | --- |
| sizeof / alignof | 32/8；16/4 | 48/16；24/8 | [layout](../code/function/layout.cpp) | 顺序 x86_64；armv7l，int(int) 与 void() 两个实例均如此，每格 5 次 | 不能把一侧对象当另一侧对象读取 |
| 成员偏移，编译器静态输出 | 存储区/manager/invoker：0/16/24；0/8/12 | 缓冲区/目标指针：0/32；0/16；外层包装成员位于 0 | 同程序的 `function_*_record_layout.stdout` | GNU 空基类、联合内指针、LLVM 对齐填充完整保留；不是在运行程序里直接访问 private 成员 | 函数包装不是一根可随意传递的函数指针 |

两侧源码：[GNU std_function.h](source/std_function.h) 62–130、252、668–669 附近的 `_Any_data`、`_M_manager`、`_M_invoker`；[LLVM function.h](source/function.h) 190–213 的 `__value_func`。LLVM 的 `aligned_storage<3*sizeof(void*)>` 实际按对齐扩成 x86 32 字节 /ARM 16 字节，再加目标指针与尾填充；不能把模板参数 24/12 直接当缓冲区实际 sizeof。`raw/060_function_header_diff` 确认本次使用的 function 头与只读源码一致。

### B. 行为

| 项 | libstdc++ | libc++ | 测试程序 | 两架构结果 | 影响 |
| --- | --- | --- | --- | --- | --- |
| 默认/拷贝/移动/赋值/嵌套/reference_wrapper | 移动小 lambda 后源为空；目标三次均返回 12；嵌套返回 13，引用包装计数 1,2 | 移动后源仍非空；其余具体值相同 | [construction](../code/function/construction.cpp) | 两架构各 5 次 | 不依赖移动后源一定为空；需要清空时显式赋 nullptr |
| 平凡、align=1 的小对象阈值 | 1–40 字节扫描中最大不分配为 16 /8，下一字节开始分配 | 最大不分配 24 /12，下一字节开始分配 | [threshold](../code/function/threshold.cpp) | 两架构每格 5 次，每个尺寸返回自身整数值；分配/释放计数配对 | 这只是所定义可调用类型族的阈值，不是所有 function 的通用阈值 |
| 对齐/非平凡拷贝改变分配 | align8 对象：x86 不分配、ARM 分配；align16 两架构均分配；4 字节非平凡 noexcept-copy 对象两架构均分配 | align8 两架构不分配；align16 x86 不分配、ARM 分配；上述 4 字节对象不分配；可能抛的 copy 则分配 | [sbo](../code/function/sbo.cpp) | 每格 5 次；值为 71/73，计数配对。align32 两侧均分配 | 类型性质和对齐同样重要；不只比较捕获变量总字节数 |
| 空调用异常 | bad_function_call，what=`bad_function_call` | bad_function_call，what=`std::bad_function_call` | [empty](../code/function/empty.cpp) | 两架构各 5 次，捕获后仍为空 | 捕获异常类型而非精确文字 |
| 分配失败/目标抛异常 | 替换为大目标失败时 bad_alloc，原目标仍返回 23；目标 invalid_argument 原消息 `target-input` 传播 | 同左 | [allocation](../code/function/allocation.cpp)、[target_exception](../code/function/target_exception.cpp) | 两架构每程序每格 5 次，目标调用计数为 1 | 仅固定赋值与目标调用，不是跨库异常契约证明 |

小对象条件的源码：GNU `__stored_locally` 同时检查 location-invariant、sizeof、alignof；LLVM 检查包装目标的 sizeof 与 noexcept-copy，另外存在不同实现配置分支，本轮不把未启用分支的条件套进来。移动源码 LLVM `__value_func(__value_func&&)` 对内联目标 clone 后保留源；GNU 移动设置原 manager/invoker 为空。原始定位见 `../raw/057_function_source_evidence.stdout` 与 `../raw/072_lwe_final_error.stdout`。

规范区分：[移动后的源值未指定、小目标避免分配属于实现空间](https://eel.is/c++draft/func.wrap.func.con)；[空调用和目标异常](https://eel.is/c++draft/func.wrap.func.inv)。本轮发现不是违反这些保证的证明。`reserve/push_back/capacity/at/迭代器/C 字符串构造` 不适用于 function，不能硬凑一行差异；存为容器元素已测一个 vector 样本，按 function 自身比较/hash 当 map key 未测且不预设存在这样的接口。

异常文字的实现源码另附：[GNU functional.cc](source/functional.cc) 的 `bad_function_call::what()`、[LLVM functional.cpp](source/functional.cpp) 的同名方法；不是按报错文字猜测异常类型。

### C. 接口

| 项 | libstdc++ | libc++ | 测试程序 | 两架构结果 | 影响 |
| --- | --- | --- | --- | --- | --- |
| __is_location_invariant 的用户特化 | 编译；给本例纯整数、无自引用的非平凡目标声明可重定位，5 次 value=37、allocations=0 | 编译失败：无该主模板 | [gnu_extension](../code/function/gnu_extension.cpp) | 两架构均实际编译与同侧运行；GNU 头 62–71 行明示可特化 | 不把这个 GNU 扩展或其“不分配”结果带到另一库；任意类型不能随意声明可重定位 |
| allocator_arg 形式构造 | C++17 编译失败 | C++17 编译失败 | [allocator_extension](../code/function/allocator_extension.cpp) | 两侧失败，不计单侧差异；普通构造是正向控制 | 只给当前模式实测，不推断其他标准模式 |

抽取的范围骨架/成员见 `R117_entities.tsv` / `R117_members.tsv`。没有逐个验证所有重载、CTAD、语言模式或新型函数包装设施；不得将单侧扫描缺失视为确定的 API 缺失。

### D. 跨 DSO

**后续补充：GNU_CONTROL_FIXED，跨侧矩阵仍 PARTIAL。** [夹具修复与完整矩阵](../../R119_batch1/W0/REPORT.md)记录：驱动消息循环并加载页面，在 headless 夹具中使用公开接口禁用绘制后，GNU/GNU 的 x86_64 与 ARM 物理板各 5/5，回调次数均 1、值均 `3`、销毁完成。libc++ 消费方对 GNU provider 两架构均自然链接失败；真实 libc++ LWE 不可得。因此不把整个类型报告改成完整通过。以下保留本轮原始尝试，**不代表修复后的当前控制状态**。

| 包边界 | 传递方式 | 实测结果 | 影响 |
| --- | --- | --- | --- |
| LWE 消费探针 → 真实 lightweight-web-engine.mobile.so → 同 RPM 的实现库 | EvaluateJavaScript 的 string 输入和 function<void(const string&)> 回调 | x86 GNU 头的自然链接成功；libc++ 头的自然链接失败（实际缺失的修饰名在原始 stderr）。**GNU 同侧运行控制尚未通过** | 链接阶段结果确定；运行时混合传递、正确值与销毁均不能据此判定 |

逐步尝试均保留：首次加载缺 libtizen-core-wl；按冻结 RPM 清单补接 124 个唯一候选后，单独加载实现仍缺 CookieManager::Destroy；先加载同包前端后，两库可加载。按公开头的初始化示例，主线程模式可初始化且创建 headless 容器。无事件循环时回调未到；使用已加载 GLib 的主上下文后收到空内容，未通过“1+2 得 3”的断言；增加页面加载步骤后，同侧路径抛 bad_function_call。调试回溯确认在 GNU 运行库的 `std::__throw_bad_function_call`，调用来自真实实现库、经 GLib 事件派发，**并未查清是哪一项宿主回调前置缺失，不能归因成跨标准库错误**。

记录：[加载身份](lwe_availability.json)、[依赖补接与歧义候选](lwe_closure.json)、[前端与实现加载](lwe_front_result.json)、[回调结果](lwe_callback_results.json)、`../raw/function_lwe_control_trace.*`。测试程序 [lwe_callback.cpp](../code/boundary/lwe_callback.cpp)；初始化依据为真实公开头，未改平台配置，storage 仅在 tmp/R118。未为消除日志而创建 `/etc/tizen-platform.conf`，也未猜选所有多候选产品库。

公开签名来源：LWEWebView.h:344–349 的 AddJavaScriptInterface/EvaluateJavaScript，LWEWorker.h 的状态回调，均在既有公开头快照。后三项没有各自建立真实实例控制，记 `NOT_OBSERVED`。本轮未取得 libc++ 构建的 LWE；未运行 ARM 的这套 LWE 宿主。不能把其他 Boost 或数据层探针借来填充 function 的真实回调格，也未达到 3–5 个真实独立包边界。

### 开发者须知

不要假定 function 移动后原变量一定为空。要表达“以后不能再调用”，可以明确把原变量置空；普通目标的调用结果在本次样本中保持一致。

同样大小的 lambda 也可能一边分配内存、一边不分配。捕获对象的对齐和拷贝会不会抛异常同样影响结果；性能敏感代码需要在目标库上测，不能用一条固定字节阈值猜测。

function 不只保存回调地址，还保存对象和管理这些对象的代码。不要把它直接交给另一套库编译的组件并假定可用；可以把接口设计为明确的 C 回调、上下文指针及释放函数，但上下文寿命、调用线程、异常和何时注销都要约定。本轮尚未把真实 Web 引擎回调验证到能对外承诺的程度。

### 本类型未覆盖的

真实 LWE 的 GNU 同侧宿主控制已在后续补充；仍未覆盖跨侧回调运行、真实 libc++ provider、3–5 个真实包边界、所有 callable/allocator/签名、函数包装的其他实现配置、并发、重入、取消、库卸载后的代码寿命。本轮分配计数只覆盖探针替换的 new/delete（包含对齐分配），不证明整个系统没有泄漏。五次重复仅说明固定格可复现。
