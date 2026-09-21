# 按架构的跨包面与标准库新发现

待人工审阅。本文只汇编既有记录，不增加源码分析、探针、板上运行或兼容性承诺。表中的“确认”必须连同证据种类和架构一起阅读。

## 一、收账结论与计数单位

原先按候选登记的 **44 项**，对应 **16 个有向源码包对、21 条符号边**；随后追加不同消费方的 WaitUntil 与 Reader，现有 x86_64 登记为 **18 包对、23 符号边**。候选、包对、符号边不能相加：同一符号边可包含多个候选，一个包对可包含多个符号。完整原候选映射见 [候选到边](CANDIDATE_MAP.md) / [TSV](CANDIDATE_MAP.tsv)。[证据: base-map](EVIDENCE.md#base-map) [证据: edges](EVIDENCE.md#edges)

追加记录使用了 `steady_clock::rep`、`streamoff` 等表达式标签，不是重新审定了一批候选；“44 → 18/23”是两个阶段的登记衔接，不能读成“44 个互不重复类型直接产生 23 个独立缺陷”。WaitUntil 的不同消费方分别成边，Reader 保留为对象边而不计作直接内建类型符号冲突。[证据: extra-proofs](EVIDENCE.md#extra-proofs) [证据: arm-final](EVIDENCE.md#arm-final)

| 架构 | 已确认的包间引用 | 证据覆盖 | 不能声称什么 |
| --- | --- | --- | --- |
| x86_64 | 18 包对 / 23 符号边 | 冻结的 3,085 个 ELF，选自该架构 10,946 个运行 ELF，约 28% | 不是平台全集，不是 23 项混合运行失败 |
| armv7l | 定点核查的 2 包对 / 2 符号边：ui-foundation → dali2、zypper → libzypp | 真实 ARM 消费方 UND、提供方定义及 NEEDED/SONAME 对应 | 其余边未复核；完整 ARM 分母为 NOT_OBSERVED，不能套用 16/21 或 18/23 |
| aarch64 | 包间引用全集及本表各边：NOT_OBSERVED | 只完成下文指定类型测量 | 不把类型测量当作包间 ELF 引用或运行实证 |

以上分母及架构事实分别回链：[证据: scope](EVIDENCE.md#scope) [证据: arm-elf](EVIDENCE.md#arm-elf) [证据: arm-count](EVIDENCE.md#arm-count) [证据: arm-final](EVIDENCE.md#arm-final)。ARM 两条证据没有来自 x86_64 的继承；原始符号条目、ELF 路径和完整 SHA 在 [逐边附表](EDGES_zh.md)。

## 二、完整跨包面

本表是公开声明、对象存储链与真实已发布 GNU ELF 引用的**静态核查结果**，不是混合标准库业务运行矩阵。仅调用标准库自身的运行库引用不计入本表。测试产物边仍登记，但不据此声称第三方应用实际直接调用。形态与所有权来源为逐候选原证据；未核实的留存、复制及释放责任保持未定。[证据: base-map](EVIDENCE.md#base-map) [证据: edges](EVIDENCE.md#edges)

| 序号 | 消费方 → 提供方 | 接口 | 跨界形态；所有权 | ELF 架构 | 原始证据 |
| --- | --- | --- | --- | --- | --- |
| 1 | appcore-agent → app-core | AppCoreBase::AddEvent | 值参数 shared_ptr&lt;EventBase&gt;; 共享所有权；是否留存未验证 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-1) |
| 2 | amd → bundle | Bundle::Add | const 引用：string 与 vector&lt;string&gt;; 借用；不假定异侧释放 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-2) |
| 3 | amd → bundle | Bundle::Bundle(initializer_list) | 值视图；pair&lt;string,string&gt; 元素; 传视图不等于移交底层元素 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-3) |
| 4 | ise-engine-anthy → isf | scim::utf8_wcstombs | wstring const 引用; 借用输入；返回对象按真实声明处理 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-4) |
| 5 | xwalk-extensions-common → jsoncpp | Json::Reader::parse | 隐式 this 指针；Reader 的 deque/stack 成员；输出引用; 消费方构造 Reader；借用调用 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-5) |
| 6 | grpc → abseil-cpp | SpinLockWait | atomic&lt;unsigned int&gt;* 与转换表指针; 借用控制字；无移交声明 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-6) |
| 7 | com.samsung.dali-demo → dali2-adaptor | ActorAccessible::GetMatches | tuple 值参数；array/unordered_map/hash/equal_to 子对象; 值传递；内部资源约定未验证 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-7) |
| 8 | dali2-adaptor → dali2 | Scene::AddFrameRenderedCallback | unique_ptr&lt;CallbackBase&gt; 值参数; 明确转移独占所有权及删除器 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-8) |
| 9 | enlightenment → gtest | StringStreamToString | stringstream 指针；streambuf/openmode 成员; 借用；原消费方为测试产物 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-9) |
| 10 | app-installers → manifest-parser | DeltaParser::ParseManifest | filesystem::path const 引用; 借用路径 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-10) |
| 11 | app-installers → cert-svc | SignatureValidator::checkList | list const 引用；SignatureData 输出引用; 借用输入；输出释放约定待核 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-11) |
| 12 | com.samsung.dali-demo → dali2-toolkit | WebView::RegisterPageLoadStartedCallback | function 值参数；回调 string const 引用; 捕获对象所有权取决于 callable | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-12) |
| 13 | dali2-toolkit → dali2 | CalculateHash | string_view const 引用; 借用视图及其存储 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-13) |
| 14 | dali2-toolkit → dali2 | ConditionalWait::WaitUntil | time_point 值参数；duration/rep 成员；ScopedLock 引用; 时间值不移交堆资源 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-14) |
| 15 | dali2-toolkit → dali2 | SingletonService::GetSingleton | type_info const 引用; 借用编译器产生的类型信息 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-15) |
| 16 | capi-media-vision → inference-engine-interface | GetInputTensorBuffers | map 非 const 输出引用；less 成员; 后端填入；tensor buffer 最终释放责任未闭合 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-16) |
| 17 | app-core → gtest | PrintStringTo | string const 引用；ostream 指针及其状态成员; 借用；原消费方为单元测试产物 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-17) |
| 18 | data-provider-master → notification | SharedFile::SetPrivateSharing | list&lt;shared_ptr&lt;...&gt;&gt; 与 multimap 值参数; 值参数；复制/移动及留存待核 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-18) |
| 19 | libzypp-bindings → libzypp | ResPool::setRequestedLocales | unordered_set&lt;Locale&gt; const 引用; 借用容器 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-19) |
| 20 | libzypp-bindings → libzypp | ZConfig::multiversionSpec | set&lt;string&gt; 值参数; 声明不能决定复制或移动 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-20) |
| 21 | libzypp-bindings → libzypp | CheckSum::CheckSum | string const 引用与 istream 引用; 借用输入及流 | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_zh.md#edge-21) |
| 22 | dali2-ui-foundation → dali2 | ConditionalWait::WaitUntil | time_point 值参数；duration/rep 成员；ScopedLock 引用; 时间值不移交堆资源 | x86_64; armv7l | [ELF / SHA / symbol](EDGES_zh.md#edge-22) |
| 23 | zypper → libzypp | xml::Reader::Reader | InputStream const 引用；内部 string/shared_ptr/streamoff 状态; 借用参数；构造后持有关系未在本轮验证 | x86_64; armv7l | [ELF / SHA / symbol](EDGES_zh.md#edge-23) |


机器可读完整表：[EDGES.tsv](EDGES.tsv)。每行保留 x86_64 消费方/提供方路径、SHA、符号原文；有 ARM 证据的行另列 ARM 路径、SHA 和 UND/DEF 原文。`NOT_OBSERVED` 不是“不存在”。

### Reader 的归类修正

**事实链（静态核查）**：真实调用为 `Reader(InputStream const&, Validate const&)`；`InputStream` 的 `_size` 是 `DefaultIntegral<std::streamoff,-1>`，后者保存实际数值成员；该类还含 `_name` 字符串、`_stream` 等成员。构造函数名编码的是 `InputStream` 与 `Validate` 类名，不包含 `_size` 的内部类型。原头文件摘录 `InputStream.h` 的成员区、`DefaultIntegral.h` 及 ELF 记录均已保存。[证据: structure](EVIDENCE.md#structure) [证据: extra-proofs](EVIDENCE.md#extra-proofs)

**已排除的解释**：不能把该 Reader 符号当作 `streamoff` 直接参数的 `l/x` 冲突。真实 ARM 两侧头文件交叉编译得到相同的 Reader 引用名，GNU 包间真实引用也存在；这既不删除对象边，也不证明其内部对象可跨库解释或释放。[证据: arm-signatures](EVIDENCE.md#arm-signatures) [证据: arm-elf](EVIDENCE.md#arm-elf)

**反驳需要什么**：若能够推翻这组冻结头/ELF 的对应或证明实际跨越的不是所列 `InputStream` 对象，需要修正该边；若要证明跨库兼容，则仍需匹配 libc++ 提供方上的内容、状态和销毁测试，仅有符号相同不够。现归类为“跨包 C++ 对象（引用及成员状态）”，计数不变。

## 三、可纳入对外报告的新发现

### 同名高分辨率时钟并不代表同一种时间

**事实链**：已测 x86_64 配置下，`high_resolution_clock` 在 libstdc++ 展开为 `system_clock`，在 libc++ 展开为 `steady_clock`；两侧均实测为空类型，大小均为 1 字节。保存的定义分别指向 GNU `bits/chrono.h` 的别名及 libc++ `__chrono/high_resolution_clock.h` 的条件分支。后者在启用单调时钟时选 steady_clock，关闭该配置时可选 system_clock；本文不是无条件描述所有 libc++ 配置。[证据: clock](EVIDENCE.md#clock) [证据: clock-gnu](EVIDENCE.md#clock-gnu) [证据: clock-cxx](EVIDENCE.md#clock-cxx)

**静态语义证据**：GNU 保存的 `system_clock` 定义说明它代表系统墙上时间，`is_steady=false`；`steady_clock` 说明时间以均匀速率单调增长，`is_steady=true`。空的时钟类型本身没有非静态数据成员，这排除了“时钟标签里存储布局不同”作为本项解释，**没有排除时间含义不同，也没有覆盖 time_point 的状态**。平台安装头中已有 Boost 的别名导入、LLDB 的 StatsClock/StatsTimepoint、LLD 的 startTime 成员样本；这些样本本身不新增真实消费边。[证据: clock-gnu](EVIDENCE.md#clock-gnu) [证据: clock-cxx](EVIDENCE.md#clock-cxx) [证据: clock](EVIDENCE.md#clock)

**与已确认跨包面的关系**：WaitUntil 传递 time_point 的事实已经有公开签名、内部 duration/rep 存储链及 ELF 引用；但它的头明确写的是 **steady_clock，不是 high_resolution_clock**。不能把“time_point 确实跨包”和“高分辨率时钟两侧别名不同”拼成“WaitUntil 已实测静默错值”。[证据: structure](EVIDENCE.md#structure) [证据: edges](EVIDENCE.md#edges)

**具体错值场景（推断，未实测）**：若一个 GNU 组件把高分辨率时钟的“自起点以来时间”统一为纳秒整数交出，另一 libc++ 组件未经时钟域/起点换算，将它重建为自己 high_resolution_clock 的截止时间，那么接收侧会把墙上时间数值解释成单调时钟时间。整数传递和读取可以成功，值却不是同一截止时刻，可能导致过早超时或错误的长等待。这是明确了“按整数传递、单位已统一、未换算起点”的条件性反例，不是某条已发布调用链的运行发现；本轮没有测得该场景的错值或时间差。

**已排除的解释**：不是用大小不同解释这项语义风险；不是把所有 chrono 类型视为同一时钟；也没有声称混合 C++ 符号总能链接通过——直接传递不同类型时，链接可能先拒绝。

**反驳需要什么**：若实际两侧构建选中相同的时钟，或边界保留并校验时钟域、起点且完成换算，或并不存在这种时间重解释，该场景就不成立。要把它提升为产品缺陷，需要真实消费路径和可核对的截止时间实测；当前保持“别名与空类型已测，错值场景为推断”。

### 同宽整数仍可能形成不同的链接名称

**事实链（实测）**：指定的 **13 项表达式**在 x86_64 和 aarch64 均为 GNU `long` / libc++ `long long`，实际类型编码为 `l` / `x`，两侧均 8 字节；armv7l 则两侧均为 `long long`、编码 `x`、8 字节。表中给的是类型表达式数量，不是独立包边或独立故障数量。两库均测，既有记录逐格五轮一致并有类型编码正向对照。[证据: types-x86](EVIDENCE.md#types-x86) [证据: types-arm](EVIDENCE.md#types-arm) [证据: types-aarch64](EVIDENCE.md#types-aarch64) [证据: arm-final](EVIDENCE.md#arm-final)

| 类型表达式 | x86_64 GNU / libc++ | armv7l GNU / libc++ | aarch64 GNU / libc++ |
| --- | --- | --- | --- |
| std::streamoff | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::char_traits&lt;char&gt;::off_type | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::char_traits&lt;wchar_t&gt;::off_type | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::char_traits&lt;char16_t&gt;::off_type | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::char_traits&lt;char32_t&gt;::off_type | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::istreambuf_iterator&lt;char&gt;::difference_type | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::chrono::system_clock::rep | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::chrono::steady_clock::rep | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::chrono::high_resolution_clock::rep | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::chrono::nanoseconds::rep | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::chrono::microseconds::rep | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::chrono::milliseconds::rep | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::chrono::seconds::rep | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |


完整来源与 SHA 列见 [BUILTIN_TYPES.tsv](BUILTIN_TYPES.tsv)。x86_64 为原生探针；armv7l 与 aarch64 为交叉编译后 QEMU 用户态打印。类型由目标头、宏及编译配置决定，QEMU 不重新选择类型；这些结果不证明板上业务运行正确。两侧采用 GNU 14 / libc++ 22.1.8 的已记录配置。[证据: arm-config](EVIDENCE.md#arm-config) [证据: aarch64-config](EVIDENCE.md#aarch64-config) [证据: arm-final](EVIDENCE.md#arm-final)

**已查到的公开暴露（静态，不把声明当成消费边）**：

| 提供包及头 | 公开声明/结构 | 与本项的关系及消费证据 |
| --- | --- | --- |
| dali2，conditional-wait.h | `using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;`；`void WaitUntil(const ScopedLock& scope, TimePoint timePoint);` | time_point → duration → rep；toolkit 与 ui-foundation 消费方各有 x86_64 边，ARM 仅定点核查 ui-foundation |
| podofo，PdfInputDevice.h | `virtual std::streamoff Tell() const;`；`virtual void Seek(std::streamoff off, std::ios_base::seekdir dir = std::ios_base::beg);`；`virtual std::streamoff Read(char* pBuffer, std::streamsize lLen);` | Seek 的参数直接涉及底层整数；Tell/Read 的返回类型通常不进普通函数修饰名，不能一概称链接必失败。Tell/Seek 既有身份查询有定义但未取得异包 UND；Read 未完成该查询；虚调用未排除 |
| abseil-cpp，absl/time/time.h | `FromChrono(const std::chrono::nanoseconds& d)`，及 microseconds / milliseconds / seconds 重载；另有 system_clock::time_point 重载 | 对应 duration/clock 的 rep；已有身份查询未取得异包 UND，内联/模板途径未排除，不新增边 |
| libzypp，InputStream.h / Reader.h | `std::streamoff size() const`；`void setSize(std::streamoff val_r)`；Reader 借用 InputStream | size/setSize 是已保存的内联成员；Reader 的符号只含外层类名，内部成员类型分歧不构成 Reader 直接符号冲突；对象边仍存在 |

公开声明、准确源码位置和源码 SHA：[证据: declarations](EVIDENCE.md#declarations) [证据: structure](EVIDENCE.md#structure)；实际引用：[证据: extra-proofs](EVIDENCE.md#extra-proofs) [证据: arm-elf](EVIDENCE.md#arm-elf)。上述限定阴性沿用有 `bundle::Add` 正向对照的身份查询，不作平台级无消费者断言。[证据: boundary-control](EVIDENCE.md#boundary-control) 对其余 char_traits/off_type、istreambuf_iterator::difference_type 等表达式，本文没有可追加的已闭合独立消费边。

**WaitUntil 的两重机制**：x86_64 的 duration 数值类型选择不同，是本次新增的内建 `l/x` 因素；此外两套库的 chrono 内联命名空间及 clock 类型身份也不同，是既有因素。armv7l 的前者消失，但真实头文件生成的两侧函数符号仍不同，后者仍在。aarch64 已测的是上述类型，未测 WaitUntil 业务引用或函数符号，不能借 LP64 补出该证据。[证据: extra-proofs](EVIDENCE.md#extra-proofs) [证据: arm-signatures](EVIDENCE.md#arm-signatures) [证据: arm-final](EVIDENCE.md#arm-final)

**已排除的解释**：同为 8 字节并不使 long 与 long long 成为同一 C++ 类型；这不是本项的“4 对 8 字节宽度变化”，也不是漂亮打印拼写不同造成的计数。比较依据是实际 TypeToken 修饰名，直接类型编码随记录保留。函数返回类型及外层对象成员未必进入函数名，因此不把所有出现位置说成链接失败。

**反驳需要什么**：若指定目标配置下两侧实际类型编码相同，则该配置的内建分歧不成立——armv7l 的测量正是这个反例。对某接口的链接后果，还需确认类型确实进入其符号名以及实际消费者；若无该参数/模板实参、存在明确兼容入口或根本不跨库，不能用本项推定失败。此处没有完成真实 libc++ 提供方上的逐边运行验证。

**可直接引用的一段话**：一些标准库类型名在两套实现中对应不同的整数类型。在已测 x86_64 与 aarch64 配置下，这些整数占用的字节数相同，但编译器在函数名称中区分 long 和 long long，因此使用它们的参数或模板类型可能使调用方找不到提供方的函数。这与“内存布局不同、运行时把对象读错”不是同一种问题；返回类型或对象内部成员也不一定改变函数名。已测 armv7l 配置中，这类整数选择差异不存在，不能将另一架构的问题直接移植过去。

## 四、挂账：状态和重启条件

| 工作项 | 状态 | 重启条件 |
| --- | --- | --- |
| 62 项函数身份/材料缺口 | 暂停，C 主因 23 项、D 主因 39 项；不是平台级阴性 | 逐项见 [缺口与重启条件](BACKLOG_zh.md) / [完整 TSV](BACKLOG.tsv)，保留本线可解性与依赖 |
| 65 项编译期设施的结果投影 | 0/65 完整关闭；15,793 处 RESULT_UNRESOLVED；已停 | 人工重新授权投入；先补实际模板实参、实例化上下文、名字查找或结果结构证据，不能用手选 int 样本关项 |
| 第 201–495 个候选 | 未定界，295 项 | 人工启动下一批定界并确认范围/方法；本次不继续 |
| 其余 572 个表达式的 armv7l 复测 | 不测，状态仍未测 | 只有人工更改范围并给出产品相关问题时重启，不因本次 13 项一致就判它们安全 |

计数来源：[证据: pending](EVIDENCE.md#pending) [证据: projection](EVIDENCE.md#projection) [证据: projection-count](EVIDENCE.md#projection-count) [证据: base-count](EVIDENCE.md#base-count) [证据: measurement](EVIDENCE.md#measurement) [证据: mechanisms](EVIDENCE.md#mechanisms)。不同表的候选、命中位置及表达式不是同一单位，不相加。

**投影计数的最新口径**：结果位置分别为无对象 15,770、类型分歧 10、标准库对象 13、未定 15,793；“13 个对象结果位置”不等于上节“13 个内建类型表达式”。发现对象结果也不等于已确认新消费边；完整候选关闭仍为 0/65。此前少掉“两侧同一类型”的断言已在新产物纠正，旧产物保留。[证据: projection-count](EVIDENCE.md#projection-count) [证据: projection](EVIDENCE.md#projection)

**不测决定的理由需纠正**：剩余 572 项中，按既有 x86_64 结果分解为相同 283、仅命名空间/标签差异 114、实现类/组合类型差异 111、枚举相关差异 4、不可得 60。已知命名机制不作为新发现重复投入，是保留不测决定的背景；但不能将其余所有项改写成“全为已知实现差异”。相同项不等于 ARM 同样相同，不可得项仍缺结果。本文保留“不测”的人工范围裁决，不替剩余项补结论。[证据: measurement](EVIDENCE.md#measurement) [证据: mechanisms](EVIDENCE.md#mechanisms)

## 五、下一阶段逐边输入与前置

以下是**规划估计，不是新增实测或优先级排序**。每行都需要该行指定的 **libc++ 构建提供包**及匹配的公开头、依赖库、构建选项和来源 SHA；并保留 GNU 提供方用于同侧控制与反方向。当前哪些新版包已生成，本次没有重新盘点，状态为“未复核”，不能把所需清单当现货清单。

沿用既有三类型验证模板：先通过双方同侧控制，再测混合方向；真实提供方，记录实际加载库；断言具体值、失败状态与适用的销毁责任，每格五轮。链接不通过也是结果，不用强制改名或强转函数指针伪造自然可链接。转移所有权的检查不能退化为单看退出码。[证据: template](EVIDENCE.md#template)

**架构安排的边界**：本表所有边均可在 x86_64 提供方及初始化夹具就绪后先开始验证；这不承诺当前所有服务均能在无图形环境运行。对于 armv7l 产品结论，每条边都需要其自身 ARM 版本的验证，计划沿用物理板验收；先补未核查边的 ARM 实际引用。仅凭当前材料，不能分出“技术上非物理板绝不能测”的确定子集，标为 NOT_OBSERVED。渲染、WebView、推理及服务类边的实际初始化/设备依赖在各行列为前置；若只能在板上满足，届时需板，不在本轮替其决定。已查到 ARM 引用的两条也没有运行通过豁免。

**量级的适用范围**：表内 h / working days 为单边、单架构、提供包和环境齐备后的夹具与验证/记录量级；不含提供包重建、依赖修复、环境搭建等待及排板时间。分为简单数据调用、状态/资源调用、服务/回调初始化较重的调用，是编辑规划判断，不是历史工时测量。旧三类型时间记录均标 `complete_scope=false`，不足以校准完整逐边成本，因此不给总工时。[证据: timings](EVIDENCE.md#timings) [证据: template](EVIDENCE.md#template)

| 序号 | 需 libc++ 构建的提供包 | 先决条件 | 具体值/销毁检查 | 单边单架构量级 |
| --- | --- | --- | --- | --- |
| 1 | app-core | 事件对象、可用 AppCore 实例 | 事件字段、引用计数、销毁次数 | 4–8 h |
| 2 | bundle | 可构造 Bundle 与多值输入 | 键、元素数量和逐字节内容；各侧销毁 | 2–4 h |
| 3 | bundle | 真实构造入口、元素生命周期 | 字段与空/重复输入；构造后销毁 | 2–4 h |
| 4 | isf | 宽字符输入、真实返回类型与环境 | 非 ASCII 字符及输出字节；返回对象销毁 | 2–4 h |
| 5 | jsoncpp | 真实 Reader/Value 构造与错误输入 | 解析值、错误队列、复用及销毁 | 4–8 h |
| 6 | abseil-cpp | 受控转换表、线程退出/超时夹具 | 状态转换值及线程结束；不以不挂起代替断言 | 4–8 h |
| 7 | dali2-adaptor | 真实可访问性对象、注册及初始化 | 匹配集合的具体内容；聚合对象销毁 | 1–2 working days |
| 8 | dali2 | Scene、帧事件、真实 CallbackBase 生命周期 | 回调值/次数；接管方销毁且仅一次 | 1–2 working days |
| 9 | gtest | 原测试入口与真实 stringstream | 流内容、模式、结果及两侧对象销毁 | 2–4 h |
| 10 | manifest-parser | 可控 manifest 文件、解析初始化 | 路径与解析字段；失败/销毁 | 4–8 h |
| 11 | cert-svc | 合法/非法 URI 样本、证书服务环境 | 列表内容、校验结果、输出状态与销毁 | 4–8 h |
| 12 | dali2-toolkit | WebView/页面加载事件及回调注销路径 | 回调字符串、次数、捕获对象销毁 | 1–2 working days |
| 13 | dali2 | 真实导出与稳定输入生命周期 | 固定输入 hash 与同侧对照；空/嵌零输入 | 2–4 h |
| 14 | dali2 | 时钟域明确的等待/唤醒夹具、锁前提 | 期限/唤醒结果、锁状态；不强接不匹配符号 | 4–8 h |
| 15 | dali2 | 服务注册、对象类型身份与同侧对照 | 返回对象身份、字段与生命周期 | 4–8 h |
| 16 | inference-engine-interface | 真实后端、模型/张量配置和释放契约 | 键、buffer 维度/内容、失败状态与配对释放 | 1–2 working days |
| 17 | gtest | 原单测入口、真实输出流 | 转义后字节、流标志；销毁 | 2–4 h |
| 18 | notification | 真实 SharedFile/AbstractItem、权限与服务初始化 | 共享条目/字段、失败状态、对象销毁 | 1–2 working days |
| 19 | libzypp | 可控 ResPool 状态与 Locale 构造 | locale 集合、重复/空集合、销毁 | 4–8 h |
| 20 | libzypp | 隔离配置实例/状态，不改系统配置 | 精确规则集合、复用及销毁 | 4–8 h |
| 21 | libzypp | 确定算法及输入流、真实返回状态 | 校验值、流位置/错误状态、销毁 | 2–4 h |
| 22 | dali2 | 此消费方的等待/唤醒夹具；不可继承另一消费方运行结果 | 期限/唤醒和锁状态；两架构分别匹配符号 | 4–8 h |
| 23 | libzypp | 真实 InputStream/Validate、XML 输入和错误路径 | XML 字段、流位置、构造失败及销毁 | 4–8 h |


详细输入：[NEXT_STAGE.tsv](NEXT_STAGE.tsv)。每行的提供方 ELF 目标、来源证据与 SHA 均可核对。规划不是处置选择，也不声称相同提供包下不同符号可互相代替验证。

## 六、未覆盖部分与自行判断

未覆盖：冻结 ELF 之外的产物、静态链接、宏生成与条件编译、生成头、模板实例化、别名后的无限定名、dlopen/间接调用、未扫描架构、真实混合库业务调用的逐边内容/销毁/异常/并发验证。公开头和测试 ELF 的存在不等同 SDK 面向第三方应用的可用性。范围沿用既有具名冻结安装头/ELF 集合和 x86_64 结构测量集合；本次没有扩展任何技术测量范围，只把已存 ARM 定点证据并列列出。[证据: scope](EVIDENCE.md#scope) [证据: arm-final](EVIDENCE.md#arm-final)

自行判断仅限：将计数分成候选/包对/符号边和架构；按已审阅依据重述 Reader 的类别；把时间错值场景明确列为条件性推断；给逐边规划量级；纠正“不测 572 项”的构成说明。未重判 62 项或推进 65 项关闭，未重跑内建别名/空类型测量。待人工确认的只有这些编辑表述及下一阶段启动条件，不在此替业务作兼容策略决定。

自检见 [SELF_CHECK.md](SELF_CHECK.md)，证据路径和完整 SHA 见 [EVIDENCE.md](EVIDENCE.md)，逐输入完整摘要见 [INPUT_SHA256.json](INPUT_SHA256.json)。事实链、候选解释和反驳条件已分开；没有把“没有崩溃”写成“没有问题”。
