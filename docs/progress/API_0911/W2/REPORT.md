# 双编译单元 shim：固定样本 SHIM_VIABLE，目标接口仍待验证

## 已完成的实测，不外推的范围

**SHIM_VIABLE 仅指本报告的普通输入转换样本**。真实 provider 是平台构建的 Boost.Program_options 1.83.0（GNU 构建 release 105.10.8、libc++ 构建 release 105.10.7），不是模拟实现。调用的原接口为 `std::string boost::program_options::to_internal(const std::wstring&)`。这是验证双编译单元机制的独立实例，**不是 EWK、SetSyncBinaryReply 或 Dali 的替代实测**；这三处 shim 的部署可行性仍为 `SHIM_UNCERTAIN`。

| 应用 → 实际 provider | 路径 | 运行次数 | 每次具体内容断言 | maps/作用域检查 |
| --- | --- | ---: | --- | --- |
| libstdc++ → libc++ | 两 TU shim | 5/5 | 5/5 | 两库与精确 provider 路径均通过；两侧返回/局部对象离开作用域 |
| libc++ → libstdc++ | 两 TU shim | 5/5 | 5/5 | 同上 |
| libstdc++ → libstdc++ | 直接调用正向控制 | 5/5 | 5/5 | 同时显式加载 libc++；不是仅单库环境的对照 |
| libc++ → libc++ | 直接调用正向控制 | 5/5 | 5/5 | 同时显式加载 libstdc++ |

总计 **4 格、20 次运行、100 项内容比较**；额外 1 次 `LD_DEBUG=bindings` 是绑定诊断，不增加矩阵格数。全部为 **x86_64 原生**。五项输入：空串、ASCII、中文加补充平面字符、内嵌 NUL、257 字符长串；输出长度分别 0、9、11、3、257，逐字节/长度比较而非只看退出码。逐项 hex 在原始输出中。[矩阵](matrix.json)、[产物与库 SHA256](identity.json)。

provider TU 自行构造并销毁其 wstring/string，通过隐藏 C 函数把 wchar 指针/长度与 char 指针/长度交给调用方 TU；调用方 TU 创建自己的 string。输入输出 std 对象不穿过两 TU 中间边界。不让消费方拿另一库构造的 std 对象自行析构；那会重新制造本来要避开的边界。
作用域完成标记证明代码到达析构后的检查点，**不是内存泄漏检测、全部分配器证明或取消清理证明**。

## 三种表述需要限定，不能当作普遍不可能性证明

1. **自定义兼容布局类型直接代替 std::string：原签名下不成立。** 自定义类型不是该声明的参数类型；强制改指针解释不会建立合法对象生命周期，也不保持标准库不变量。若改为显式 C 数据协议则变成另一种设计，不是类型直接替换。仅 `sizeof` 相同也不够；本探针 string 为 GNU 32、libc++ 24 字节，而 vector<uint8_t> 两边均为 24 字节，**后者不证明 vector 跨库安全**。
2. **单 TU 用同一套 std 定义同时解释两种布局：不成立。** 但“一个 TU/程序只能链接一套标准库”不准确；编译时类型选择与最终动态链接是不同阶段。一个 TU 也可经 C/不透明接口调用另一侧，后者仍需合适实现。Clang 官方说明允许不同标准库的代码共存，但不应跨边界传标准库对象，同时警告 C++ ABI 运行库的共存问题。[官方工具链说明](https://clang.llvm.org/docs/Toolchain.html#c-standard-library)。
3. **把一个 provider 原样编两次直接无控制地塞一起：不能据此获得兼容。** 同名导出、ODR、公共异常/分配符号需要解决。但“来自两个项目所以双版本一定无法导出”**尚未证明，不能写死**。GCC 双 ABI 是它自己两套实现与命名空间的机制，不是 libc++ 兼容开关；符号版本能选择入口，不能转换对象布局。[GCC Dual ABI](https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_dual_abi.html)、[GNU ld 版本脚本](https://sourceware.org/binutils/docs/ld/VERSION.html)。

同理，“两个 TU 是唯一可能形态”也不能当排他定理：两个 DSO 以 C 协议通信等属于同一隔离原理的其他封装方式，本轮未测试。

## 本次如何控制解析，哪些仍未控制

`caller.cpp` 只看到应用侧 std 头，保持原函数签名；`provider_side.cpp` 只看到 provider 侧 std 头。两个 PIC `.o` 链进一个 `.so`，用 `-fvisibility=hidden` 隐藏 C 桥，只显式暴露调用入口与探针计数。真实 provider 未重编、未修改。

GNU 参数含 `std::__cxx11`，libc++ 参数含 `std::__1`，本例的**不同修饰名**区分 shim 导出和真实 provider 导入。每次 `dladdr` 打印真实 callee 的精确路径，maps 核实实际加载，另以动态符号表、版本表和 loader 绑定日志交叉核验：`raw/*_symbols.*`、`raw/009_versions.*`、`raw/008_binding_trace.*`。

**这不是每个公共 C++ ABI 符号都按 TU 隔离。** 绑定日志显示本次 shim 的 `__gxx_personality_v0` 绑定到 libc++abi 的 LLVM_22，而 GNU 运行库内部仍有 CXXABI_1.3 绑定；不能据普通返回的成功证明跨边界异常正确。`new/delete`、RTTI、异常、取消与链接次序变化仍需另测。既有平台版本化运行库是本次环境条件，不能换成任意系统库后沿用结果。

EWK 的 `extern "C"` 同名导出没有本例的参数修饰名区分条件。若要保留同名入口，还要证明包装入口与真实函数的寻址不递归、不被抢先解析（例如经过明确句柄取得真实入口，或定义版本化入口）；本轮**未实现/验证这种路由**。vector 回调还需要在真实注册点接入双方匹配的接口表；Dali 需要处理对象、虚方法与生命周期，不是 string 复制的同义词。

## 构建体系与先例

**静态核查**：真实 binder spec 用 CMake/make，并区分 MOBILE/TV/COMMON；见 `raw/007_platform_build_fact.*`。CMake 支持对象库及逐 target 编译选项，所以表达“两个对象集合分别编译再链接”有现成机制。[对象库](https://cmake.org/cmake/help/latest/command/add_library.html#object-libraries)、[编译选项](https://cmake.org/cmake/help/latest/command/target_compile_options.html)。
**实测**：当前工具链手工命令能够完成上述两个 TU 的编译和链接。
**未观测**：未修改真实包配方，未在 GBS 中重建这三组目标；全局工具链 wrapper、LTO、打包规则、RPM 自动依赖能否保留这种细粒度设置仍待包内验证。不能写“平台构建体系已全面支持”。

**已有相关先例**：LLVM 开发者记录了 FreeBSD 让两套标准库共用 libcxxrt，以及部分 Linux 配置让 libc++ 使用 GNU ABI 支持的做法。[开发者原始记录](https://lists.llvm.org/pipermail/cfe-dev/2016-August/050278.html)、[libcxxrt 项目](https://github.com/libcxxrt/libcxxrt)。这是**共存前提/ABI 支持库的先例，不是本任务三个接口已上线同款双 TU shim 的证据**。以 libc++/libstdc++/shim/wrapper/two translation units 搜索后，本轮没有核实一项精确同形态生产案例，记 `NOT_OBSERVED`，不等于不存在。

## 代价、边界及反驳条件

| 事项 | 可核的代价或限制 | 谁承担 |
| --- | --- | --- |
| 转换维护 | string/vector 元素协议、长度单位、编码、空值、错误、allocator/句柄分别定义；“每类型一对”不能覆盖所有函数语义 | 平台接口与 shim 维护方 |
| 复制 | 本例 wchar 输入在 provider 侧复制，UTF-8 输出在 caller 侧复制；另有真实 provider 编码转换。时间/峰值内存未基准测量，不编造微秒或百分比 | 平台承担运行开销；业务决定可接受阈值 |
| 上游演进 | 参数、字段、回调表顺序/版本变化要同步；旧应用链接身份与入口选择需回归 | provider、shim 与测试维护方 |
| 错误表现 | 可以静默错值，也可链接失败、异常、终止、悬挂或内存破坏；不限定为只有静默错值 | 测试需字节/长度/长短串/边界值/双方向/加载身份/生命周期检查 |
| 异常 | 样本把失败转 C 状态，调用侧再建本地异常；**不保持原异常类型/消息/捕获契约**，异常场景未实测。catch-all 样本不适用于线程取消语义，不可原样部署 | 需要接口契约与取消路径单独验证 |
| 迭代器/引用 | 复制不能透明维持原对象位置、引用有效期或零拷贝迭代器；需句柄/操作协议 | 双方接口维护者，可能触及应用 |
| 所有权 | 可在生产侧销毁后复制出数据；不能自动支持消费侧 delete 另一标准库对象、shared_ptr 控制块等 | 创建/释放必须配对并验证 |

若普通样本在相同 SHA/加载身份/输入条件下可复现错值或销毁失败，则本报告限定的 `SHIM_VIABLE` 可被反驳。反过来，若能为三个目标完成真实双方向、同名符号/回调路由、异常与生命周期矩阵，就可以提升目前针对它们的 `SHIM_UNCERTAIN`。不需要先认同本报告，原始脚本和结果可独立复核。

## 引用与自行判断

选择现有双库 Boost 构建，是为了验证机制而不制造 provider；没有用它补齐 W1 的空格。使用既有平台 libc++ 与主机 GNU 运行库的精确 SHA 记录在 identity.json，不宣称同一套产品 rootfs。
网页访问与快照见 [sources.json](sources.json)；GCC、GNU ld 页面可经检索工具读取，本地 curl 返回 HTTP 错误，记录未隐藏；libcxxrt 原猜测 README.md 返回错误，改查项目首页。检索可见不等于所有网络环境均可直接下载。
