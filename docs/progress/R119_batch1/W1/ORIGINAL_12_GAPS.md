# 原十二项未定：已查事实与缺少材料

本表说明当前缺口，不以未知替代“不跨包”。

| 候选 | 已查事实 | 缺什么 | 依据 |
| --- | --- | --- | --- |
| std::exception | 已观察公开派生类（absl SeedGenException、JSON_API Exception）和运行库/类型信息引用，但没有 std::exception 对象经平台公开参数/返回值传递的闭合链。异常传播不出现在普通函数修饰名里；不能因此判为无跨包异常。 | 需把公开派生异常的 provider 抛出路径与实际消费方对应；RTTI 交集不代替对象传递证明。 | `W1/reviewed/008.json` |
| std::ifstream | 已查到 openfst 内联 Read 中的局部 ifstream，也查到 tensorflow2 打包的 cupti_pcsampling_util.h:244 公开结构字段 ifstream* fileHandler。不能将整个候选判 INTERNAL_ONLY。运行库调用另列，不补成业务跨包。 | CUpti 工具结构对应 provider/消费方的真实产物与生成配置尚未对应。 | `W1/reviewed/011.json` |
| std::size_t | absl/strings/str_format.h:469 的 SNPrintF 公开模板使用 std::size_t；这是一种内置整数别名，修饰名中不会保留 std::size_t。按名字交集为零不能作为阴性；不冒充标准库对象布局传递。 | 需按函数身份补对整数字段/参数的消费方，并确认内置别名的最终统计口径。 | `W1/reviewed/012.json` |
| std::mutex | cereal/details/static_object.hpp:92 的 LockGuard(std::mutex&) 受 CEREAL_THREAD_SAFE 控制；还观察到公开安装头的类/union 成员。不能因为 R22 无显式 std::mutex 业务符号就判全是包内。 | 宏配置、所在外层类的实际对象传递和内联实例化位置尚未闭合。 | `W1/reviewed/014.json` |
| std::ostringstream | cereal XMLOutputArchive 的 itsOS 成员及 openfst 的内联局部变量均已观察；chromium-efl/node_v8.h 另有返回 ostringstream* 的 out_stream。仅按显式动态符号名未闭合，不能判不存在消费方。 | 外层对象或返回流的实际跨包调用，以及生成配置与发布 ELF 的对应。 | `W1/reviewed/017.json` |
| std::lock_guard | 已进一步展开全部限定名命中，323 次安装头命中按 SHA256+行号去重为 145 处（raw/078）。除了函数体局部 guard，还有 cynara/containers/MutexedBinaryQueue.h:54 的 LockedBinaryQueue::m_guard，以及 cv::AutoLock、Thrust lock_t 等别名，不能归为全是局部变量。 | 需追踪 LockedBinaryQueue 返回对象及别名的跨包消费；保存的全部去重上下文不代替该对象流证明。 | `W1/reviewed/018.json` |
| std::numeric_limits | 已复核 absl/cereal 用法是 max/min/max_digits10/is_iec559 等编译期或静态成员查询，不是 numeric_limits 对象参数。长模板表达式命中不等于对象跨界。 | 8,861 次头文件限定名命中未全部逐个完成语义核对；不得据抽样宣称没有公开对象签名。 | `W1/reviewed/019.json` |
| std::runtime_error | cereal::Exception 公共派生及已安装 nlohmann 头中的 runtime_error 成员已观察；这些不能用运行库 what()/typeinfo 调用证明两个业务包传递异常。 | 需区分公开派生异常的真实抛出/捕获路径与纯头文件实例化；普通符号交集不够。 | `W1/reviewed/020.json` |
| std::unique_lock | cereal::LockGuard 私有成员 unique_lock、Eigen 宏别名及 BehaviorTree getAny 的局部 lock 已观察；缺少该对象跨两个平台包的完整证明。 | 外层类对象边界、宏实例化及非内联实际引用尚未对应。 | `W1/reviewed/022.json` |
| std::invalid_argument | Eigen 宏抛出、nntrainer NNTR_THROW_IF 以及 libtbb memory_pool.h 的抛出辅助路径已观察；没有由这些片段得到真实跨包对象传递的闭合链。 | 需核对 throw_exception 的模板/内联形态与最终消费方，不能把运行库异常构造调用列作 CROSS_PACKAGE。 | `W1/reviewed/023.json` |
| std::thread | Eigen EnvThread 的 thr_ 成员和 klay::AuditTrail::dispatcher 已观察；RCSResourceObject 使用的 std::thread::id 是嵌套类型，不是 std::thread 对象。 | 需追踪含 thread 的外层实例是否跨包；nested id 命中不能自动算 thread 传递。 | `W1/reviewed/024.json` |
| std::ofstream | openfst WriteFile、测试头和 cuDNN getStream 中观察到局部/静态 ofstream；后者以 ostream 引用返回，按不同候选名字不能不加说明重复计数。 | 所有公开声明/成员与基类流形态的边界核查尚未闭合；不以当前样本宣布平台零使用。 | `W1/reviewed/025.json` |
