# 23 条跨包边：真实调用夹具与 GNU 基线

## 结论

**PARTIAL：16/23 条完成 x86_64 本机 GNU/GNU 五轮有效基线，共 80 个有效轮次；7 条 NOT_AVAILABLE。** 已编译 19 个边夹具，最终采用 95 轮尝试（80 次有效、10 次初始化终止、5 次仅错误路径）。四条未造运行夹具。没有测 libc++ 组合、没有用板子、没有构建平台包，也没有把这 16 条称为原消费方产品端到端通过。

边集合只来自冻结的 EDGES.tsv 和 NEXT_STAGE.tsv；原消费方包用于说明这条登记边的来源，本轮新建最小消费方，不冒称启动原应用。历史 18 包对/23 符号边不因此增减。

## 来源、环境与验收条件

- Unified 固定快照：`tizen-unified-toolchain_20260917.132101`；Base：`tizen-base-toolchain_20260914.073422`。先从 reference/build.xml 取得当前身份，再固定 URL。详见 [快照](SNAPSHOTS.json)、[逐 RPM URL/SHA/版本](RPM_PROVENANCE.tsv)。
- 下载/解包 262 个 RPM（目标库、开发头与依赖，包括开发包带入的 Boost 运行组件）；仅放 `tmp/EDGE_FIXTURE_0927/`。不安装 RPM，不执行安装脚本，不修改系统库/配置。
- 另取四个示例/测试数据 RPM，解至独立 test-data 目录；未混入上述运行库集合、未运行其测试二进制。URL/SHA 及文件清单见 [EXTRA_DATA_RPMS.json](EXTRA_DATA_RPMS.json)、[数据补查](DATA_FOLLOWUP.md)。
- Clang 22.1.8、`-std=c++17 -O0 -fno-inline -stdlib=libstdc++`；GNU 指标准库配置，不冒称 GCC 前端。显式采用上述快照的 GNU 14.2.0 开发头/sysroot；每轮打印 GNU release/date/CXX11_ABI，完整编译命令在 raw。
- 每轮先 nm -D 核对**指定精确 UND**，启动时 maps 核对提供方规范路径、打印预计算 SHA，进程退出后再核 SHA。LD_DEBUG=bindings 配合 lazy PLT 记录目标真实绑定；通过轮必须同时满足退出 0、值/状态/生命周期断言、目标绑定、GNU 运行库已加载且无 libc++。
- 普通目标的依赖从隔离目录加载。单例夹具在真实离屏工厂构造后还装载了本机三份 C 图形库：EGL、GLESv2、GLdispatch，逐轮 maps/SHA 见 [HOST_DEPENDENCIES.json](HOST_DEPENDENCIES.json)。隔离目录不是 chroot；不宣称整套产品镜像环境。没有启动渲染，也不据此承诺图形链或产品环境通过。
- 串行、nice 19、ionice 3，RLIMIT_AS=9921873100 字节（实际内存 30%），light 闸门；失败调度按十分钟/连续六次停止。当前闸门均通过，原始状态在 raw。硬截止为 2026-09-28 08:30 +08。

## 逐边结果

| 边 | 原消费方 → 提供方 | 夹具 | 真实触发与断言摘要 | GNU 五轮 | 缺口 | libc++ 输入 |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | appcore-agent → app-core | [edge01.cpp](fixtures/edge01.cpp) | 派生公开 AppCoreBase/事件接口；AddEvent → RaiseEvent → RemoveEvent；事件初值 42、回调 43；首次删除 true、再次 false；事件析构一次、weak_ptr 失效 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `app-core-common;app-core-common-devel` |
| 2 | amd → bundle | [edge02.cpp](fixtures/edge02.cpp) | Bundle 默认构造 → Add(key,vector<string>) → GetStringArray → Delete；返回 0；alpha/中文/257 字符逐项相等；计数 1，删除后为空；Bundle 析构完成 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `bundle;bundle-devel` |
| 3 | amd → bundle | [edge03.cpp](fixtures/edge03.cpp) | Bundle(initializer_list<pair<string,string>>) → GetString；两个键值、计数 2；Bundle 构造/析构各一次 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `bundle;bundle-devel` |
| 4 | ise-engine-anthy → isf | [edge04.cpp](fixtures/edge04.cpp) | scim.h 公开入口 utf8_wcstombs(WideString)；A/中/🙂 转换为指定 8 个 UTF-8 字节；输入对象析构完成 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `isf;isf-devel` |
| 5 | xwalk-extensions-common → jsoncpp | [edge05.cpp](fixtures/edge05.cpp) | Reader 构造 → parse(begin,end,Value,false)；坏文档；再次有效解析；n=42、name=alpha、数组长度 3/末值 3；坏文档 false 且诊断非空；再次解析 42；析构完成 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `jsoncpp;jsoncpp-devel` |
| 6 | grpc → abseil-cpp | [edge06.cpp](fixtures/edge06.cpp) | 公开 absl::call_once 的两个调用线程形成竞争，内联路径触发 SpinLockWait；执行次数 1、结果 42；join 后销毁 once_flag；延迟绑定证明目标实际触发 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `abseil-cpp;abseil-cpp-devel` |
| 7 | com.samsung.dali-demo → dali2-adaptor | NOT_AVAILABLE | 产品 Application/可访问 Actor → ActorAccessible::GetMatches(MatchRule,sort,maxCount)（待具备匹配接口）；未执行；应断言匹配对象集合、排序及对象存续 | 未运行 | 当前导出含 array<int,5>，登记符号含 array<int,4>。已核精确导出和当前同名方法正向对照，不能用新版符号替代登记边。需要匹配旧接口的包/头，或人工重新确认版本化边范围。 | `dali2-adaptor;dali2-adaptor-devel;dali2-adaptor-integration-devel` |
| 8 | dali2-adaptor → dali2 | [edge08.cpp](fixtures/edge08.cpp) | 离屏 Application 构造/Start → Scene::Get(root layer) → AddFrameRenderedCallback → RenderOnce → GLib 事件泵；候选夹具保留 frame=4242、一次回调、unique_ptr 转移与一次销毁的强断言；本环境未到达这些断言 | 0/5；-6,-6,-6,-6,-6 | 正确线程序列的候选在 Start 阶段因 EGL error 抛 DaliException 并终止，目标调用未到达。先前在主线程读取更新线程回调队列的版本是夹具错误，五次断言失败不属 provider 缺陷；历史源码/记录保留，不降断言。 | `dali2;dali2-devel;dali2-integration-devel;dali2-adaptor;dali2-adaptor-devel;dali2-adaptor-integration-devel` |
| 9 | enlightenment → gtest | [edge09.cpp](fixtures/edge09.cpp) | 公开 EXPECT_DOUBLE_EQ 经 ScopedFakeTestPartResultReporter 捕获预期测试失败 → StringStreamToString；恰有一次非致命失败，完整诊断逐字节为 Expected equality of these values: 加两行 1.25/2.5；外层返回 0、实例构造/析构各一次 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `gtest;gtest-devel` |
| 10 | app-installers → manifest-parser | [edge10.cpp](fixtures/edge10.cpp) | DeltaParser → ParseManifest(含中文的不存在路径) → GetErrorMessage；错误路径五次 false、诊断非空、析构完成；退出 77 明示无完整正向样本 | 0/5；77,77,77,77,77 | 公开头仅给结果结构；限定本地源码检索未找到实现；源 RPM URL 返回 404，公开源码快照请求返回 403。另取 manifest-parser-examples/tests，已识别样本为普通应用清单/签名，仍未识别可作为正向对照的 Delta 文档与结果契约。仅错误路径不充当完整基线。 | `manifest-parser;manifest-parser-devel` |
| 11 | app-installers → cert-svc | NOT_AVAILABLE | SignatureValidator(真实 packagePath/SignatureFileInfo) → checkList(false,非空 UriList,SignatureData)；未执行；需已知有效/无效签名结果、证书/引用集合与资源销毁断言 | 未运行 | 已读公开契约、通过依赖装载检查，并另取 cert-svc-test/test-binaries 的真实签名样本。库内有 schema、信任库、验证插件的绝对路径，本机对应路径不存在；未确认公开 API 可重定向到隔离树。样本可得不等于信任环境可用，不能修改宿主安全配置，也未用空列表凑通过。 | `cert-svc;cert-svc-devel` |
| 12 | com.samsung.dali-demo → dali2-toolkit | [edge12.cpp](fixtures/edge12.cpp) | 离屏 Application Start → WebView::New → RegisterPageLoadStartedCallback → LoadUrl(固定 data URL) → GLib 事件泵；候选夹具断言一次回调、完整 URL、捕获对象持有/释放；本环境未到目标调用 | 0/5；-6,-6,-6,-6,-6 | 五次在 Application Start 的 EGL error 终止。公开插件接口要求平台动态实现；快照有 chromium/lwe 两个插件包，产品选型/宿主未确认，不自行替换。 | `dali2-toolkit;dali2-toolkit-devel;dali2-toolkit-integration-devel;dali2;dali2-devel;dali2-integration-devel;dali2-adaptor;dali2-adaptor-devel;dali2-adaptor-integration-devel` |
| 13 | dali2-toolkit → dali2 | [edge13.cpp](fixtures/edge13.cpp) | CalculateHash(string_view 子串) 与公开 string 重载对照；指定子串 hash 与等值独立字符串相同，空视图对照相同；打印实际 hash | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `dali2;dali2-devel;dali2-integration-devel` |
| 14 | dali2-toolkit → dali2 | [edge14.cpp](fixtures/edge14.cpp) | ConditionalWait+ScopedLock → WaitUntil(steady_clock 的 20ms 截止点)；循环处理伪唤醒；实际经过时间不少于截止期、锁可继续使用、等待计数归零；析构完成 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `dali2;dali2-devel;dali2-integration-devel` |
| 15 | dali2-toolkit → dali2 | [edge15.cpp](fixtures/edge15.cpp) | 公开离屏 Application 构造但不 Start → Handle::New/RegisterProperty → SingletonService::Register/GetSingleton → 析构；类型键查回同一对象，属性 42；未知类型空；输入 Reset 后仍存活，Application 销毁后弱句柄失效 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `dali2;dali2-devel;dali2-integration-devel;dali2-adaptor;dali2-adaptor-devel;dali2-adaptor-integration-devel` |
| 16 | capi-media-vision → inference-engine-interface | NOT_AVAILABLE | 真实配置 → BindBackend → Load(真实模型) → GetInputTensorBuffers → UnbindBackend（待材料）；未执行；需已知张量个数、大小、内容/所有权及正确错误码 | 未运行 | 已核公开抽象接口、安装的具体 facade 声明及 BindBackend/Load 前置，依赖装载通过；未取得选定真实后端与模型。不会实现模拟后端，也不把无后端的空缓冲返回当作有效基线。 | `inference-engine-interface-common;inference-engine-interface-common-devel` |
| 17 | app-core → gtest | [edge17.cpp](fixtures/edge17.cpp) | 公开 testing::PrintToString(string) 内联转入 PrintStringTo；a+换行+b 变为带引号/反斜杠转义的确切字符串；自有对象析构完成 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `gtest;gtest-devel` |
| 18 | data-provider-master → notification | NOT_AVAILABLE | 真实 AbstractItem 文件项、应用身份、接收组 → SharedFile::SetPrivateSharing → 验证访问/撤销（待环境）；未执行；需返回码、实际授权对象与清理后权限状态 | 未运行 | 已核声明、RPM 依赖、装载检查及私有文件共享语义；无真实应用身份/服务/私有文件数据。不以空集合无操作或修改主机权限充当验证。 | `notification-ex;notification-ex-devel` |
| 19 | libzypp-bindings → libzypp | [edge19.cpp](fixtures/edge19.cpp) | ResPool::instance → setRequestedLocales({en,de}) → 查询 → 恢复原集合；集合大小 2、en/de 命中、集合精确相等、恢复成功；输入集合析构 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `libzypp;libzypp-devel` |
| 20 | libzypp-bindings → libzypp | [edge20.cpp](fixtures/edge20.cpp) | ZConfig::instance → multiversionSpec(set) → 查询 → 恢复原集合；两条确定字符串逐项一致，原集合恢复；输入集合析构 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `libzypp;libzypp-devel` |
| 21 | libzypp-bindings → libzypp | [edge21.cpp](fixtures/edge21.cpp) | CheckSum(sha256,istream[abc])；算法名 sha256、摘要 ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad；流/结果对象析构 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `libzypp;libzypp-devel` |
| 22 | dali2-ui-foundation → dali2 | [edge22.cpp](fixtures/edge22.cpp) | ConditionalWait::WaitUntil 与另一个线程持锁写入/Notify；保护值 42、锁归属、等待计数 0；join 后销毁；与边 14 独立夹具及五轮 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `dali2;dali2-devel;dali2-integration-devel` |
| 23 | zypper → libzypp | [edge23.cpp](fixtures/edge23.cpp) | InputStream(内存 XML) → xml::Reader → 遍历；畸形 XML 捕获异常；五个节点名字及文本 42；正常对象析构；畸形数据按公开文档捕获 zypp::Exception 并打印动态类型/诊断，输入对象销毁 | 5/5 有效 | 仅本样本 GNU 基线；其他组合未测 | `libzypp;libzypp-devel` |

完整 mangled 名、提供库版本、声明位置和证据入口在 [RESULTS.tsv](RESULTS.tsv)；头文件 SHA/原文/归属见 [DECLARATIONS.md](DECLARATIONS.md) / [JSON](DECLARATIONS.json)。逐边 libc++ 目标和辅助包、架构、条件在 [LIBCXX_INPUTS.tsv](LIBCXX_INPUTS.tsv)。

## 未完成项与口径

- **边 7**：精确旧符号缺失，不是查询零命中就停。当前同名方法的导出是正向对照；当前角色数组编码为 `Lm5E`，登记为 `Lm4E`。见 [符号新旧原文](EDGE7_SYMBOL_CHANGE.json)。本轮没有改登记边或把新接口当替代样本。
- **边 8、12**：最终候选都编译且具备指定 UND，但在 Application::Start 前置阶段发生 EGL/DaliException 终止；目标绑定未发生。初版边 8 使用更新线程专用读取方法属于夹具错误，不算 provider 失败。纠正后保持真实帧/页面回调断言，等待正确运行环境。
- **边 10**：实际调用已发生，五次验证缺失文件 false/非空诊断及析构；缺有效文档的正向对照，因此仍 NOT_AVAILABLE，退出 77，不能列入 80 次有效运行。
- **边 11、16、18**：只完成声明/依赖/装载与前置梳理，未伪造签名验证、推理后端、安全服务或无数据空操作。逐项已试方法及具体需求见 [DETAILS.json](DETAILS.json)。
- **边 15**：无 Application 的初版五次退出 77。通过公开 OffscreenApplication 工厂（GLIB、MANUAL）构造而不 Start，完成真实类型键检索、值与销毁断言后，才纳入当前 5/5；不是降低前置检查。
- 异常范围：XML Reader 畸形输入验证公开文档的 zypp::Exception 类型族并打印动态类型；JSON/Delta 用返回值报告解析失败。未统一注入分配失败/取消，也未把不崩溃当作全异常安全。生命周期是被测对象/共享引用/回调捕获的断言，不是内部分配器全泄漏证明。
- 没有 armv7l/aarch64、混合标准库、原应用完整上下文、并发压力、任意输入覆盖。后续换输入仅适用于已经有效的夹具；缺上下文的七条仍须补齐条件并验证夹具，不能承诺只换库就全部可跑。

## 原始证据与复现

[最终选用的逐轮 JSON](SELECTED_RUNS.json) 保留每轮 command/stdout/stderr/exitcode 路径、UND 原文、绑定原文、maps/SHA。`runs/` 与 `raw/` 保留所有失败和修正尝试，不只保留成功轮。
重放方式及 libc++ 输入约束见 [README.md](README.md)；判断与尚存问题见 [DECISIONS.md](DECISIONS.md)；统计见 [COUNTS.json](COUNTS.json)；自检与源码 SHA 见 [AUDIT.json](AUDIT.json) 和 [SHA256SUMS](SHA256SUMS)。

完成后停止，交人工审阅；推送回执另见 DELIVERY.md。
