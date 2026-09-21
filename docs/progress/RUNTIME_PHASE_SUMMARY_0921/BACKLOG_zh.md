# 暂停项逐项清单

主因 C：别名、返回类型、派生/外层类型等使原名字面不能完成证明。主因 D：真实产物、调用链或配置缺失。两者可重叠；保留原判定，不重新定界。PARTLY 不是保证可闭合；YES_LOCAL_RESEARCH 只表示可继续本地取证。

| 候选 | 主因 | 缺口；重启条件 | 本线可解性 | 依据 |
| --- | --- | --- | --- | --- |
| std::exception | C | 需把公开派生异常的 provider 抛出路径与实际消费方对应；RTTI 交集不代替对象传递证明。 从 SeedGenException / Json::Exception 对应业务入口追到真实 throw，再对应消费方 catch 与运行时绑定；RTTI 名字不是异常对象传播证据。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::ifstream | D | CUpti 工具结构对应 provider/消费方的真实产物与生成配置尚未对应。 CUpti 公开结构 ifstream* fileHandler 所在外层结构的构造/调用入口及实际消费者。 | NOT_OBSERVED | [证据: pending](EVIDENCE.md#pending) |
| std::mutex | D | 宏配置、所在外层类的实际对象传递和内联实例化位置尚未闭合。 cereal LockGuard 与 CEREAL_THREAD_SAFE 的实际宏值、外层对象成员及生成实例的所属包。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::ostringstream | D | 外层对象或返回流的实际跨包调用，以及生成配置与发布 ELF 的对应。 node_v8.h 的 out_stream 返回 ostringstream*：先确认实际 Node/V8 构建版本，再找调用入口及消费者。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::lock_guard | D | 需追踪 LockedBinaryQueue 返回对象及别名的跨包消费；保存的全部去重上下文不代替该对象流证明。 MutexedBinaryQueue::LockedBinaryQueue 的返回对象是否跨包；追 m_guard 存储与调用点。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::runtime_error | C | 需区分公开派生异常的真实抛出/捕获路径与纯头文件实例化；普通符号交集不够。 cereal::Exception、nlohmann 所用 runtime_error 的实际抛出入口与消费方调用/捕获；区分头内实例。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::unique_lock | D | 外层类对象边界、宏实例化及非内联实际引用尚未对应。 cereal LockGuard 的 unique_lock 成员、Eigen 宏别名及外层对象的实际实例归属。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::invalid_argument | C | 需核对 throw_exception 的模板/内联形态与最终消费方，不能把运行库异常构造调用列作 CROSS_PACKAGE。 Eigen/nntrainer/libtbb 的 invalid_argument 抛出辅助函数：确定非内联提供端及真实调用端。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::thread | D | 需追踪含 thread 的外层实例是否跨包；nested id 命中不能自动算 thread 传递。 Eigen EnvThread::thr_ 与 klay AuditTrail::dispatcher 的外层对象公开方法和外包消费者。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::ofstream | D | 所有公开声明/成员与基类流形态的边界核查尚未闭合；不以当前样本宣布平台零使用。 cuDNN getStream 返回 ostream 引用时，实际派生对象是否为 ofstream；其他公开流路径逐项核对。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::istringstream | D | 需要区分派生流对象与公开签名里的基类流，并核对剩余声明/成员；当前不能证明候选本身没有消费方。 XMLInputArchive / StringToFst 使用 istringstream 后的 istream 接口调用链与模板实例归属。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::bad_alloc | C | 需由真实 provider 分配失败/抛出路径与消费方对应，不以运行库 bad_alloc 构造或 typeinfo 引用代替。 Thrust/libtbb 分配失败入口与调用端：定位哪个实际 provider 抛 bad_alloc，再确认是否越过包边。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::condition_variable | D | 该构造入口在不同平台包之间的实际引用尚未闭合；其到标准库 wait 的引用单列 RUNTIME_CALL。 AurumXML.h condition_variable* 构造入口的真实调用者、对象存活范围和宏/构建版本。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::chrono::milliseconds | C | 需处理别名展开以及返回类型不在普通修饰名中编码；不能据字面名字交集为零认定无消费方。 以 FromChrono / ToChronoMilliseconds 的函数身份查引用；展开 duration&lt;Rep,ratio&lt;1,1000&gt;&gt;，核对 Rep 和返回类型。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::queue | D | 公开内联模板实例与其调用位置尚未对应；无实体包间对象证据，不计 CROSS_PACKAGE。 cereal queue 序列化模板与 OpenFST 含 queue 类的实际实例提供端、消费端。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::optional | D | 需确定条件宏、实际模板实例与消费方；不能把可见的纯头声明当成已发生的 DSO 边。 absl optional 分支宏与 AbslParseFlag / cereal 实例归属，以及真正对象类型是 std 还是替代实现。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::out_of_range | C | 需对非内联 provider 抛出路径和实际调用边做对应；不把标准库异常支持函数算业务边。 nlohmann/Eigen 的 out_of_range 抛出位置与非内联入口、消费方、异常开关对应。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::chrono::seconds | C | 实际非内联实例及消费方尚未闭合；duration 整数与 ratio 的符号展开必须按具体周期对应，不能将所有 duration 边都算 seconds。 按 FromChrono / ToChronoSeconds 身份查 consumer；只接受 ratio&lt;1&gt; 的精确 duration 特化与返回类型。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::regex | C | 返回类型通常不进入普通函数修饰名；需核对 getRegex 的实际消费者、是否内联及生成配置，不能按名字交集零命中判无消费方。 nntrainer getRegex 的确切签名、是否内联、导出/引用身份、返回 std::regex 的构建版本。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::istreambuf_iterator | D | 需要对应实例化发生的包与实际动态引用；当前没有真实 iterator 对象跨包的闭合证据。 cereal/rapidxml/picojson/Boost 实际 istreambuf_iterator 临时对象所进入的模板实例及其包归属。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::enable_shared_from_this | D | 需把具体派生对象的跨包入口、真实 consumer 与当前安装头版本关联；基类声明本身不是包边。 gRPC Channel 或 Node SiblingGroup 的真实对象边，及 enable_shared_from_this 基类/weak_this 存储关系。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::chrono::microseconds | C | 缺具体周期匹配的实际 consumer；内联 FromChrono 声明不能单独证明两个平台包交接对象。 以 FromChrono / ToChronoMicroseconds 身份核对 ratio&lt;1,1000000&gt;、Rep、实际 consumer。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::future | D | 不能使用失活声明作公开边；含成员的外层类及真实跨包消费尚未闭合。 BehaviorTree AsyncActionNode 的 future&lt;NodeStatus&gt; 成员对象是否由外包消费；排除 OpenCV #if 0 声明。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::recursive_mutex | D | 对应构造函数是否被其他平台包引用未闭合；弱指针指向对象与弱指针自身必须分别记，不能从模板字样直接判对象已交接。 iotivity InProcClientWrapper / InProcServerWrapper 的 weak_ptr&lt;recursive_mutex&gt; 构造引用与底层对象身份。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::variant | D | 需确定模板实例的提供包、消费包以及 ICU 外层 Operand 的实际对象交接。 ICU Operand 所含 optional&lt;variant&lt;...&gt;&gt; 的实际对象边，或 cereal variant 的模板实例跨包关系。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::weak_ptr | D | 模板消费方实例/该构造的两个平台包真实引用尚未闭合；运行库控制块引用不足以判业务包边。 iotivity weak_ptr&lt;recursive_mutex&gt; 构造入口或 cereal weak_ptr 实例的两个平台包引用。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::logic_error | C | 派生异常实际跨包的抛出/捕获路径未与发布 ELF 对应，异常支持符号不能替代对象边。 Thrust/Open3D/Vulkan 派生 logic_error 的实际业务抛出入口、调用链和捕获端。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::mt19937 | D | 测试安装头的引用形参不能据名称判断在包内还是跨包；缺实例/consumer 对应。 TensorFlow slice_tester 的 mt19937& 形参对应实例和消费者，区分测试包内部调用。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::reverse_iterator | C | 具体 rbegin/rend 返回迭代器的调用可能内联，普通名字交集不够；未闭合真实返回对象的两个包。 按 rbegin/rend 或实际公开函数身份查返回 reverse_iterator 的实例；确认迭代器基底和包归属。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::uniform_int_distribution | D | 需要判断含成员的外层对象与模板函数是否由另一包消费；不把所有命中当内部临时变量。 TensorFlow local_device_state 的 uniform_int_distribution 成员外层对象与真实消费关系。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::random_device | D | 默认参数产生的整数与 random_device 对象不可混算；外层成员对象真实跨包路径未闭合。 TensorFlow local_device_state 的 random_device 成员外层对象；区分默认参数产生的整数与设备对象。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::bitset | D | 已安装头确有公开形态；实际两个源码包的消费关系仍未闭合。grpc 的 WriteEventSet 是别名，不能只凭类型名认定传递。 TensorDim(bitset&lt;MAXDIM&gt;...) 的跨源码包消费者，而非 nntrainer 同包内部引用；以及 cereal 实例。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::once_flag | D | static 字段不随实例一起跨界；oneDNN 含成员外层对象是否跨另一平台包尚未闭合。 oneDNN cache_blob_id 实例中的 once_flag 外层对象流；其他 static 字段不随实例传递。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::fstream | C | 函数身份检索尚未闭合真实外包消费；不以直接动态名字检索未命中覆盖宏、静态实例和其他架构。 按 UDT/SRT sendfile/recvfile 身份核对 fstream 引用参数、实际导出及引用者。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::reference_wrapper | D | 需对应模板实例落点与实际外包消费；不是无公开签名，也不是一个引用参数自动等同 wrapper 对象。 AnyInvocable::operator=(reference_wrapper&lt;F&gt;) 及 sigc bound_argument 的实际模板实例与外包 consumer。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::atomic_bool | D | 第一批 atomic 阳性实例是 atomic&lt;unsigned int&gt;*，不能拿来证明 atomic_bool。含 bool 成员的外层对象真实包边仍待核对。 BehaviorTree / grpc 含 atomic&lt;bool&gt; 成员的具体外层对象边；不能复用 atomic&lt;unsigned int&gt; 的阳性。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::unordered_multimap | D | 不能只看局部样本判内部；Matcher 外层对象和 Xbyak 宏展开后的实际包边未闭合。 OpenFST Matcher/StateTable 存储的 unordered_multimap 实例，或 Xbyak 宏展开后的对象消费链。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::chrono::system_clock::time_point | D | 同时存在该应用自带 grpc 1.27 与平台 grpc 1.76 的同名实现，尚未证实 consumer 跨源码包绑定平台库；不能从同名交集选择 provider。 stt-engine 中 Timepoint2Timespec 引用到底绑定其自带 grpc 1.27 还是平台 grpc 1.76。 | NOT_OBSERVED | [证据: pending](EVIDENCE.md#pending) |
| std::error_code | D | Rust 自带 LLVM 20 与平台 LLVM 22 可能构成同名误配，NEEDED/SONAME 和源码包含链尚未闭合；不能按 llvm 源包粗略归运行库。 LLVM StringError(error_code) 的实际 provider 是否平台 LLVM 22，而非 Rust 自带 LLVM 20；核 NEEDED/SONAME/包含链。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::system_error | C | 抛出对象真实跨包与捕获端未闭合；运行库构造/析构引用不是跨平台包异常传播证据。 Vulkan/BehaviorTree/Boost 的 system_error 派生/抛出入口、消费方与异常运行时配置。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::thread::id | D | 需追踪具体含 id 的外层对象跨包入口；不能因 nested 名字把它计成 thread 对象边。 iotivity AtomicWrapper&lt;thread::id&gt; 或 Eigen created_by_thread_id_ 的外层对象实际包边。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::priority_queue | D | 模板实例和含成员外层 TimerQueue 对象与实际外包消费未闭合。 TFRT TimerQueue priority_queue 成员及 cereal helper 实例的提供/消费关系。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::promise | D | 不能以局部样本判全部内部；Open3D 为 aarch64 安装头样本，x86_64 索引不足以排除其外部消费。 Open3D PeerConnectionManager::Create 的 promise 引用消费方及对应 aarch64 产物。 | NOT_OBSERVED | [证据: pending](EVIDENCE.md#pending) |
| std::uniform_real_distribution | D | 含成员的真实外层实例未与消费方对应；不因局部用法占多数就判不跨包。 Open3D Random 的 uniform_real_distribution 成员外层实例及真实外包对象入口。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::multiset | D | 公开模板形态存在，但尚缺跨源码包的实际实例消费证明；宏生成不得忽略。 cereal/zypp/glog multiset 模板/宏生成接口的实际实例、提供包、消费包。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::ostream_iterator | D | 需核对模板实例是否真正经外包动态符号传递；不能把给内联函数的临时参数当成跨包。 RapidXML/picojson 的 ostream_iterator 临时值进入的具体模板实例是否产生外包引用。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::forward_list | D | 外层状态/模板实例/共享 internals 的实际包间对象关系未闭合；不能以模板名未进普通动态符号判内部。 OpenFST 返回 forward_list 引用/指针的状态对象及 pybind11 internals 注册链的真实跨模块共享关系。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::default_random_engine | C | 具体引擎别名展开及含成员 Scheduler 的实际外包消费者未闭合。 展开 default_random_engine 对应具体引擎，再按 Scheduler 公开函数/对象身份追 rand_engine_ 成员。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::cv_status | C | 实际返回 cv_status 的函数已见；真实消费者和模板实例尚未闭合。 按 tsl::internal::wait_until_system_clock 与 condition_variable::wait_for 身份查实际 consumer 和返回 cv_status。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::istream_iterator | D | 模板实例所在包与是否跨 DSO 未闭合；不能把内联源码调用等同平台包边。 Boost greg_facet 内联 operator&gt;&gt; 构造的 istream_iterator 是否进入不同包提供的实例。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::length_error | C | 两侧出现 throw/catch 不代表这两个实际包间发生传播；需实际调用路径与配置对应。 gmpxx / OpenUSD 抛 length_error 的实际业务入口与 pybind11 或其他真实调用端的对应。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::exception_ptr | C | 真实转换器回调或外层对象的两个包关系未闭合；函数指针调用不一定有可匹配的导入符号。 pybind11 ExceptionTranslator 函数指针的注册者/调用者、exception_ptr 实际参数与共享 internals 的模块关系。 | NOT_OBSERVED | [证据: pending](EVIDENCE.md#pending) |
| std::atomic_int | D | 第一批 atomic 的真实实例是 unsigned int，不能替代 int；OpenUSD 具体外包调用仍未闭合。 OpenUSD refBase 返回 atomic_int& 的实际外包调用者与对象实例；不能复用 unsigned int 原证据。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::u16string | C | 已观察 Chromium→ICU 的 readOnlyAliasFromU16StringView 边传的是 string_view&lt;char16_t&gt;，不是 u16string；不能拿这条近似名字边替代所需证据。 以 ICU 迭代器解引用或 Node ToU16String 函数身份查返回 u16string 的调用端。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::any | D | TfDiagnosticInfo 与 CustomCallBinding 的实际外包对象关系未闭合；仅头文件类型出现不能算业务包边。 OpenUSD TfDiagnosticInfo(any) 或 TensorFlow CustomCallBinding(vector&lt;any&gt;) 的实际外层对象边。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::atomic_flag | D | 需把含成员 Interpreter/Dispatcher/Spinlock 的完整公开对象与实际 consumer 对应；不是无公开形态。 TensorFlow Interpreter / OpenUSD Dispatcher / dmlc Spinlock 所含 atomic_flag 的实际外层对象流。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::basic_ostringstream | C | 实际 LogMessage 派生对象是否跨另一平台包尚未闭合；不能只凭局部样本判内部。 按 TensorFlow LogMessage 构造/公开函数身份核对跨包对象及 basic_ostringstream&lt;char&gt; 基类存储。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::domain_error | C | 接口中的 throw 已见，但虚调用实际对象、调用端和异常是否穿过包边尚未对应；不能凭 throw/catch 两端分别出现就宣称传播。 Dali ProxyAccessible::GetChildAtIndex 的具体虚调用对象、调用包与 domain_error 传播路径。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::streampos | C | 公开返回流位置或含位置状态的外层对象实际消费关系未闭合；不能因局部样本判全内部。 按 tellg/tellp 或返回流位置的实际公开函数身份查 fpos&lt;state&gt; 对象，而非搜 streampos 原别名。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::back_insert_iterator | D | 公开模板参数确有对象，但实例提供包/消费包尚未对应，不能把 header-only 调用当成已确认跨包符号边。 fmt get_container(back_insert_iterator) 等具体模板实例的 provider/consumer 和底层容器对象。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
| std::byte | C | MLIR 公开别名对应哪些导出函数和实际消费方未闭合；不能因 C 入口名字不带类型就判无对象。 以 MLIR AsyncRuntime 中使用 ValueStorage 的 C 导出函数身份取证，再对应 byte* 的实际 consumer。 | YES_LOCAL_RESEARCH | [证据: pending](EVIDENCE.md#pending) |
| std::overflow_error | C | 这些是独立源码路径，尚未证明一个实际平台调用链跨包传播该异常；运行库构造引用不补为包边。 OpenUSD 抛 overflow_error 的实际入口与 cereal/pybind11 或其他真实消费方对应。 | PARTLY | [证据: pending](EVIDENCE.md#pending) |
