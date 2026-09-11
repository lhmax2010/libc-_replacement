# 平台对外 API 跨标准库兼容性盘点：第二阶段首批

状态：**已完成本批静态分类，交人工审阅；SDK 开放边界仍有缺口。**

按人工缩小后的范围处理 Dali/NUI 8 个包族、Web 3 个包族；Native 12 个包族做快速扫描及例外辨别。
其余 4 个包族仅说明用途，没有按原 27 族方案展开。第一阶段材料保持原样，本目录是后续结果。

## 一、回答业务部门时最重要的区分

1. **NUI 的所追踪 C# 应用路径经过 C 导出函数，不直接传递 Dali C++ 对象布局。**
   `View.FindChildByName(string)` → C# `DllImport` → `CSharp_Dali_Actor_FindChildByName(void*, char*)` → Dali C++。
   binder 内部仍调用 Dali C++；冻结 x86_64 ELF 同时证明其导出 C 名称、导入 `Control::New`、依赖 core/adaptor/toolkit。
   因此不能把原生 Dali C++ API 的风险原封不动转给普通 C# 调用者，也不能把整套平台栈说成不再依赖 C++ ABI。
2. **EWK 不是可按包一刀切的纯 C 接口。** 常见 `ewk_view_url_set(Evas_Object*, const char*)` 属 C 形态；
   但 `ewk_parse_cookie(const std::string&, EWKCookieContents&)` 位于 `extern "C"` 内，输出结构含四个 `std::string`。
   后者属于“传标准库对象，不承诺”，不能因导出名没有 C++ 修饰而放行。
3. **Web 应用与插件内部边界必须分开。** 所追踪脚本路径发送 JSON 文本，也支持二进制数据；
   应用脚本不直接构造 `std::vector`。然而引擎—插件同步二进制回复函数指针
   `SetSyncBinaryReply(XW_Instance, const std::vector<uint8_t>&)` 确实跨原生边界传标准库对象。
4. **Native 12 族不能全记为纯 C。** public 配方路径相交的 287 个头文件记录中，
   排除名字明确为 internal/private/implementation 等的部分后，11 族仍有 C++ 候选。
   其中四族的代表例子是调用 C 入口的内联便利包装，不等于把 `std::string` 传进平台库。
   `capi-ui-inputmethod` 剩余 6 个候选头均为 C 声明形态，可在该扫描范围内归入纯 C 类别。

以上均为**静态核查或 ELF 静态观察**，本批没有新做跨标准库运行测试。
详细依据分别在 [Dali/NUI](DALI_NUI.md)、[Web](WEB.md)、[Native 快速确认](NATIVE_QUICK.md)。

## 二、判定口径

沿用已交付 API 报告：纯 C 只有既有固定样本通过，非无条件；仅内置类型/POD 的 C++ 接口条件性可行；
传标准库对象、跨边界抛接异常、跨标准库所有权转交不承诺。
POD 指可按约定布局传递的简单数据；不能因为成员函数只接收 `int`，就忽略隐含的对象指针 `this`、类布局及生命周期。

本批“符合 C 接口类别”是**静态归类，不是该 API 已经实测通过**。
“不承诺”也不是“本轮实测必然失败”。不透明句柄由创建方提供释放入口，与接收方自行 `delete` 一个跨库 C++ 对象不同；
但句柄有效期、释放次数、回调、异常隔离及目标 ABI 条件仍须成立。

## 三、决策用汇总

表中“SDK 未定”表示已尝试冻结 public/internal rootstrap 配方、安装头和 pkg-config、源码公开/内部标记；
没有目标产品最终 SDK 清单，不能补推“已开放”或“没有开放”。命名为 public-api 也不替代该清单。
改造难度栏为基于所见边界的技术推断，不是实施建议、排期或处置推荐。

| 包族 / API 组 | 接口形态 | 本轮归类 | 若需 C 化的难点 | 应用是否直接接触 |
|---|---|---|---|---|
| dali2 / 原生 Actor、BaseHandle、信号 | 自有 C++ 句柄、String、模板回调、所有权 | C++ ABI 耦合；不能承诺整组；不能把 Dali::String 冒充 std::string | 句柄、信号注册/注销和销毁协议，非只改名称 | 存在头/链接材料；目标 SDK 未定 |
| dali2-adaptor / Application | C++ Application 返回值、CallbackBase* 转移所有权 | 回调/生命周期路径不承诺跨库任意混用 | 事件循环、回调上下文与释放归属 | 原生 SDK 未定；binder 静态依赖已见 |
| dali2-toolkit / Control 等 | C++ Control 值与继承/生命周期 | 不承诺整组；非本轮失败实测 | 简单属性可包装，派生控件/信号需契约 | 可直接链接的产物与 .pc 已见；SDK 未定 |
| dali2-csharp-binder / C# 入口 | C 导出、指针/标量/字符串编组；内部 C++ | 所追踪应用边界符合 C/句柄类别，有条件；内部 C++ 边界不随之消失 | 已存在封装；异常与生命周期覆盖仍不完整 | C# 通过内部 P/Invoke 使用，不需应用包含 Dali C++ 头 |
| dali2-extension / 原生扩展、Rive 绑定 | 直接安装的实质头为 devel/integration；另有 C# Rive → C binder | 不把原生内部头列为已开放 SDK；C# 代表路径单列 | 若开放原生 Rive，内部 std::string 和对象生命周期仍要隔离 | 原生 SDK 未定；托管扩展绑定存在，产品开放未定 |
| dali2-ui-foundation / ChartAxis 等 | C++，明确传 std::vector<Dali::String> | 对该标准库对象接口不承诺 | 数组/字符串表示、返回集合释放及回调；中到较大 | public-api 路径存在；SDK 未定，不推断 NUI 必经此库 |
| libscl-ui-nui / scl_nui_* 与内部类 | 同包有 C 回调/函数以及 C++ 类 | C 入口条件性；不能把内部类当作已开放应用 API | C 层已存在；内部类若另行开放须分离上下文/结构布局 | 实际 NUI 调用此层本轮未观测；SDK 未定 |
| csapi-tizenfx / NUI 应用层 | 托管 C# API → P/Invoke C 函数 | 所追踪路径不直接接触 Dali C++ 布局；非全部 NUI ABI 验证 | 不需要为已追踪路径再造同类 C 包装；仍有编组/回调/生命周期条件 | 所选方法有公共声明及 since_tizen 文档；最终 SDK/profile 清单缺失 |
| chromium-efl / 常见 EWK C 入口 | C 函数、字符指针、Evas/EWK 句柄 | 符合 C 类别，非本轮运行通过 | 已是 C 形态，仍须配对生命周期、禁止异常逸出 | public 配方相交；产品实际开放范围待确认 |
| chromium-efl / cookie parser | extern "C" + std::string 引用及含 string 结构 | 不承诺 | 字符串/时间字段的 C 表示及所有权；局部包装可描述，未实施 | public 配方包含，最终开放待确认 |
| chromium-efl / Node、V8 嵌入 | C++ 对象、模板、容器、引擎句柄 | 不承诺直接跨标准库 C++ 混用 | 广泛对象/回调/引擎生命周期，不能以一层改名解决 | 同包头进入 public 配方；不能推成普通 Web 应用直接调用 |
| lightweight-web-engine / WebContainer、WebView、Worker | 原生 C++ string/function，部分 std 智能指针成员 | 所列对象/回调接口不承诺 | 文本/二进制编码、异步回调及销毁，非一处包装 | 原生 SDK 未定；网页脚本不是这些 C++ 签名的调用方 |
| webapi-plugins / Web 应用 API | JavaScript 参数、JSON/二进制消息 | 应用侧不直接传 C++ 标准库对象；非 Web 功能全量验证 | 应用侧不是 C++ ABI；无需据此改写成 C API | 脚本 API 与导出代码已见，产品白名单待确认 |
| webapi-plugins / 引擎及插件辅助层 | XW C 形态入口 + 含 vector 的内部函数表 + common C++ 类 | 文本/句柄部分可按 C 类；vector、C++ 类部分不承诺 | 内部二进制回复需更改两侧协议；辅助类还涉及 C++ 生命周期 | 内部支撑层，不当成第三方 Web 应用直接 ABI |

逐项签名、异常、所有权和证据编号见 [API 分类明细](tables/API_CLASSIFICATION.tsv)。
内部图形后端、integration/devel、Bullet 等同包开发材料不因被安装就加入应用公开 API 清单。

## 四、其余四个包族是什么

| 包族 | 用途 | 为什么第一阶段遇到它 | 本轮状态 |
|---|---|---|---|
| glib2 | 通用基础工具库（容器、事件等基础设施） | public SDK 配方含 glib2-devel；条件/便利头有 C++ 信号 | 仅说明，未逐接口分析 |
| harfbuzz | 文本字形塑形库 | public 配方所选头含 C++ 条件/便利声明 | 仅说明，未逐接口分析 |
| icu | Unicode、国际化设施 | 有 C API 及 C++ API；冻结清单保留两个源码 RPM 版本记录 | 仅说明，未逐接口分析 |
| vulkan-headers | Vulkan 图形 API 的开发头，含 C 和 C++ 便利层 | public 配方同时涉及 C/C++ 头 | 仅说明，未逐接口分析 |

依据为冻结 RPM 的用途描述及第一阶段 public 配方映射，不表示这四族已获跨库支持认证。
见 `raw/054_ewk_scope.stdout` 末尾及第一阶段 `tables/STAGE2_RECOMMENDED.tsv`。是否进入下一批由人工决定。

## 五、覆盖、缺口与自行判断

- 静态输入沿用第一阶段冻结的 Base/Unified RPM。11 族中可见原生头共 1,690 条拥有记录；托管源码另列。
  该数包括后来按内部/开发用途跳过的材料，不是 1,690 个对外 API，也不是独立文件/函数的去重数。
- 本轮代表原生头与 ELF 为 x86_64；C# 源码不以 noarch 取代运行架构验证。没有运行新的混库探针；
  armv7l/aarch64 运行结果均为 `NOT_OBSERVED`，x86_64 本批运行结果同样为 `NOT_OBSERVED`。
- 源码取得方式：使用已有只读源码缓存；缺少已解包的 TizenFX 时，从已存在且 SHA256 匹配的源码 RPM
  读取 NUI 文件到 `tmp/P7_0909/csapi/`，不构建、不修改既有源码或配置。所有选读源码再与归档成员核对。
- Chromium 完整源码没有从本轮现成解包目录取得；本轮以已验证的发布头文件作接口依据，
  不推断其内部全部异常拦截。完整实现审计与实际 SDK 开放表为缺口。
- 自动表 `boundary_signal_index.tsv` 是词法命中索引，不是完整导出函数列表；模板别名、条件宏、传递 include、
  私有成员与真正跨库签名仍有区别。具体分类只使用已人工核查的签名/实现。
- 不从 `extern "C"` 推断不传标准库对象；不从 C++ 声明推断必然跨库；不从未命中配方推断不进 SDK。
- 本轮自行判断仅限：候选按 SDK 配方关联及路径标记分层；挑选能说明边界的签名；按既有框架分类；
  对 C 化难点作明确标记的推断。没有裁定产品开放策略、迁移顺序、实施方案或处置优先级。

仍需人工/业务确认的事项集中在 [QUESTIONS.md](QUESTIONS.md)。
本批材料完成后停，不自动扩展其余四族，也不把 SDK 缺口自行补成“内部专用”。
