# Web 层三个包族：应用边界与原生集成边界

本轮只作静态分类。没有新运行网页、引擎或跨库探针，故“已实测可用”为 `NOT_OBSERVED`。
原生头进入 devel 包、public rootstrap 配方关联、最终应用 SDK 开放，是三个不同事实层级。

## 1. chromium-efl

### 常见 EWK 函数是 C 形态，但不能概括整个 EWK

实际发布头 `ewk_view.h` 在 `extern "C"` 块内声明：

```cpp
EXPORT_API Eina_Bool ewk_view_url_set(Evas_Object* o, const char* url) TIZEN_DEPRECATED_API;
```

参数为 Evas 句柄与字符数据，不是 std::string 对象。[头文件及注释](raw/065_ewk_view.stdout)。
分类为**符合 C 接口类别，有条件**；只读头文件不是新做了一次混库调用。
这里也有版本范围限制：许多所见 EWK 入口带 deprecated 标记，不能凭头文件存在判定当前产品推荐或允许使用。

生命周期并非自动安全。例如 `ewk_context_form_autofill_profile_get_all` 的注释要求返回的 profile
通过 `ewk_autofill_profile_delete` 删除。[配对约定](raw/113_ewk_context_owner.stdout)。
Eina_List 容器本身与元素释放是不同责任；该段不足以补全所有分配/释放契约。
不允许把此类句柄当调用方自己 `delete` 的 C++ 实例，也不能忽略回调异常逸出和间接依赖。

### 明确的反例：C 链接名中仍传递 std::string

`ewk_cookie_parser.h` 同样包含 `extern "C"`，但原文是：

```cpp
struct EWKCookieContents {
  time_t expiry_date_utc;
  std::string domain;
  std::string name;
  std::string path;
  std::string value;
};
EXPORT_API Eina_Bool ewk_parse_cookie(const std::string& cookie_str,
                                    EWKCookieContents& cookie);
```

[完整原文及行号](raw/057_ewk_exception.stdout)。这里 `extern "C"` **没有把参数和结构体变成 C/POD**。
输入 string、输出结构内四个 string 均是实质边界数据；调用方构造/析构输出对象的责任也不能忽略。
按既有框架，此接口混用 libstdc++/libc++ **不承诺**。本轮没有测其具体错误形态，不能写成已崩溃或已静默错值。

包内 79 条 `ewk*` 头文件拥有记录的初分为 75 条 C 形态、3 条宏/include 等未定、1 条 C++ 信号（即该 parser）。
这 79 条**包含 internal/product 头**，不能当作 79 个公开函数或公开 API 组。
见 [逐头清单](raw/054_ewk_scope.stdout)；其中命名为 internal 的条目本轮不深入归入应用面。
parser 与常见 EWK 头都在 public 配方必要路径交集中，但仍须最终 SDK 白名单确认。

**C 化难点（推断，不实施）**：该 parser 的接口规模有限，可以描述为输入字符数据与输出时间/字符字段，
但必须明确编码、长度、输出缓冲归属与失败处理；只保留 extern "C" 不会解决 string 布局。

### Node / V8 不等同于普通 Web 应用 API

同一个 devel 包也交付 Node/V8 C++ 头，public 配方中有相交路径。
例如 node.h 中 `InitializationResult::args()`、`exec_args()`、`errors()` 返回 `const std::vector<std::string>&`；
`ProcessGlobalArgs` 接收多个 `std::vector<std::string>*`。[声明](raw/111_node_interface_snapshot.stdout)。
这些属于原生嵌入/扩展层，标准库对象、引擎对象和生命周期耦合，**不承诺直接跨标准库混用**。
不能把它们算成普通 JavaScript 应用会直接构造的 C++ 对象。

**C 化难点（推断）**：对象模型、回调、上下文及引擎生命周期范围广，不能靠逐符号改名完成。
该结论不等于声称 Node 包内不存在其他形式的 C 接口；本轮没有全量盘点其所有替代接口。
完整 Chromium 源码没有在本轮现成解包目录取得；以发布头判断接口形态，内部异常隔离为 `NOT_AVAILABLE`，
不把“头上没有 throw”当“不抛异常”。

## 2. lightweight-web-engine

所见 3 条发布头为 LWEWebView.h、LWEWorker.h、PlatformIntegrationData.h，均有 C++ 信号。
核心原生 API 中，WebContainer 与 WebView 不是 C 函数表：

```cpp
void LoadURL(const std::string& url);
std::string EvaluateJavaScript(const std::string& script);
void EvaluateJavaScript(const std::string& script,
                        std::function<void(const std::string&)> cb);
void AddJavaScriptInterface(const std::string& exposedObjectName,
                           const std::string& jsFunctionName,
                           std::function<std::string(const std::string&)> cb);
```

[WebContainer 精确声明](raw/114_lwe_api_exact.stdout)。
Worker 层还有 `ServiceWorker::Initialize(const std::string&)`、
`RegisterOnStatusChangedHandler(const std::function<void(WorkerProcessState)>&)`；SharedWorker 有对应设施。
[Worker 头](raw/096_lwe_worker_header.stdout)。这些不是仅标量参数，按框架**不承诺**混用。

**生命周期事实**：`WebContainer::Destroy()` 调用内部 delegate 的 Destroy，清空成员，最后 `delete this`。
[实现](raw/080_lwe_destructor.stdout)。头内还有
`using LWEDelegateRef = std::unique_ptr<void, std::function<void(void*)>>`，类成员不是无条件可由另一个库布局解释的 POD。
这些提供方释放安排应保留为事实；不能因存在 Destroy 就忽略 string/function 边界，也不能把调用者任意 delete 视为等价。

**异常事实与限制**：所选转发方法没有在函数体中建立完整异常转换层，回调来自调用方；
实际错误抛出/捕获和全部间接路径本轮未运行、未证明，不据此断言每个入口都抛或都不抛。
已知异步取消是先前核查的背景，这次接口盘点没有重验其可达性或修复它。

**应用是否接触**：冻结 public 配方中未获得该 devel 头的相交依据，已有安装头与 .pc 并不证明进入应用 SDK。
因此原生集成入口为 SDK 未定；网页里的 JavaScript 不是上述 C++ 方法签名的直接调用者。
**C 化难点（推断）**：需要文本/二进制表示、异步回调上下文与注销、容器/视图释放、图形上下文约定；不是简单 extern "C"。

## 3. webapi-plugins

### 第三方 Web 应用实际接触的层

源码中的 `ApplicationManager.prototype.getCurrentApplication()` 调用
`native.callSync('ApplicationManagerGetCurrentApplication', {})`，失败时抛脚本错误对象，成功时构造脚本 Application。
[脚本入口](raw/112_webapi_js_dispatch.stdout)。
`NativeManager.callSync` 对 `{cmd, args}` 作 JSON 序列化，调用引擎扩展消息通道，再解析响应。
另外有二进制回复和 Uint8Array 数据路径。[公共消息帮助层](raw/038_webapi_transport_body.stdout)。

这里应用传的是**JavaScript 值、JSON 文本或二进制数据**，不是直接 new/delete `common::Instance`、std::string 或 std::vector。
脚本错误对象也不是 C++ 类型化异常跨标准库按 RTTI 捕获。
这是静态可追踪的分层，不是对所有插件、所有产品权限和 Web API 功能作完备验证。

### 引擎与原生插件之间：C 形态并不覆盖全部消息通道

基础扩展头声明 C 形态入口及整数句柄、void* 接口查询：

```cpp
int32_t XW_Initialize(XW_Extension extension, XW_GetInterface get_interface);
typedef void (*XW_HandleMessageCallback)(XW_Instance instance, const char* message);
// XW_MessagingInterface 中：
void (*PostMessage)(XW_Instance instance, const char* message);
```

[基础头](raw/024_webapi_header.stdout)。冻结 x86_64 `libtizen_application.so` 定义未修饰的 `XW_Initialize`，
也导入 common C++ 辅助层。[符号表](raw/elf_webapi-plugins-profile_common_libtizen_application.so_symbols.stdout)。

但同步头有一处不能忽略的实质差异：

```cpp
struct XW_Internal_SyncMessagingInterface_1 {
  void (*Register)(XW_Extension, XW_HandleSyncMessageCallback);
  void (*SetSyncReply)(XW_Instance, const char* reply);
  void (*SetSyncBinaryReply)(XW_Instance, const std::vector<uint8_t>& reply);
};
```

[精确头文件](raw/094_webapi_sync_abi.stdout)明确标为 internal、跨 Crosswalk 版本不承诺稳定；
整个块虽有 `extern "C"`，第三项仍传 vector。
不是只有声明：`Instance::SendSyncBinaryReply(const std::vector<uint8_t>&)` 调用该函数指针。
[实际调用](raw/104_webapi_native_binary_call.stdout)。

因此：文本/句柄通道可以按 C 形态分类；**引擎—插件二进制回复的 vector 边界不承诺跨标准库混用**。
该风险在平台内部原生支撑层，不应错误写成“第三方 JavaScript 应用直接传 vector”。
按本任务界限，只追到解释应用路径所需的内部一跳；不把这个 internal 表当成已开放应用 SDK，也不继续扩查所有内部插件类。

### 所有权、异常与 C 化难点

Extension 将 Instance 指针放入引擎实例数据，`OnInstanceDestroyed` 回调取回后在插件辅助层 `delete`。
[实现 170–212 行](raw/039_webapi_extension_impl.stdout)。这是有归属的回调生命周期，并非 Web 应用接收 C++ 对象后自行释放。
`common::Extension` / `Instance` 头还有 C++ 类、std::function/picojson 等辅助接口，不能因 XW 初始化函数是 C 名称就放行整个 devel 包。
[辅助类声明](raw/093_webapi_extension_header.stdout)。实际全部 C++ 异常逸出路径本轮没有证明，记 `NOT_OBSERVED`。

**C 化难点（推断）**：应用脚本入口不是需要改造成 C++ ABI 的对象；若仅改变上述原生二进制回复边界，
需要两端一致的字节指针/长度、有效期与错误协议。若把全部 common C++ 辅助类再公开给第三方原生插件，范围更大，
还涉及继承、回调和生命周期，不能用一个 C 初始化入口代表全部。
最终 SDK 是否开放原生插件开发头为 `NOT_AVAILABLE`；有 devel 包不等于第三方 Web 应用直接使用它。
