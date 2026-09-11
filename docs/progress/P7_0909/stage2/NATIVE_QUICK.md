# Native 应用框架与服务：十二个包族快速确认

## 口径与结果

不能把“传统 capi C 入口存在”改写成“这十二族交付的公开候选头全部是纯 C”。
本轮复用冻结 public SDK 配方的必要路径交集，逐文件读内容、复核 SHA256、屏蔽注释后查 C/C++ 形态；
有 C++ 例外再阅读代表完整声明/内联方法。不编译、不重新构建平台库。

共 **287 条头文件拥有记录、12 个包族**。public 配方中本来就列入部分 internal/private/implementation 名字的头，
所以“配方出现”不能直接视为产品最终公开承诺。下面先剔除这些明确名字，剩余项称“非内部名候选”。
这仍不是业务确认后的完整 SDK 白名单；尤其不能因未带 internal 后缀就认定所有接口对应用开放。

| 包族 | public 配方头记录 | 排除内部名 | 剩余候选 | C 形态 | C++ 信号 | 宏/include 等未定 |
|---|---:|---:|---:|---:|---:|---:|
| appcore-agent | 4 | 1 | 3 | 2 | 1 | 0 |
| bundle | 4 | 1 | 3 | 1 | 1 | 1 |
| capi-appfw-app-common | 7 | 2 | 5 | 4 | 1 | 0 |
| capi-appfw-app-control | 4 | 1 | 3 | 2 | 1 | 0 |
| capi-appfw-application | 4 | 1 | 3 | 2 | 1 | 0 |
| capi-media-vision | 69 | 18 | 51 | 41 | 9 | 1 |
| capi-network-smartcard | 9 | 0 | 9 | 2 | 6 | 1 |
| capi-ui-inputmethod | 8 | 2 | 6 | 6 | 0 | 0 |
| cion | 42 | 4 | 38 | 9 | 28 | 1 |
| component-based | 7 | 1 | 6 | 4 | 2 | 0 |
| libscreen_connector | 23 | 6 | 17 | 3 | 13 | 1 |
| notification | 106 | 30 | 76 | 24 | 42 | 10 |

来源：[最终逐族统计](tables/native_summary.tsv)、[逐头与 extern C 特征](tables/native_refined_scan.tsv)、
[全部头快照索引](tables/native_public_scan.tsv)。C++ 信号不是不兼容函数数；未定项也不补推为纯 C。
除 4 个自动正/负控制外，真实 `inputmethod.h` 是 C guard 正向样本，`app.hpp` 和 `bundle_cpp.h` 是 C++ 正向样本；
EWK cookie parser 的混合声明进一步证明只查 extern "C" 会误判。

## 唯一可在本扫描范围内整组归入 C 类别的项

**capi-ui-inputmethod**：8 条 public 配方头中，2 条内部头有 C++ 信号，按本轮范围跳过；
剩余 6 条均为 C 声明形态，`inputmethod.h` 有 `extern "C"`、CAPI 模块与版本标记。
[代表原文](raw/107_native_c_header_control.stdout)。
在这些候选头的覆盖范围内记为“符合纯 C 接口类别”，不再逐接口深查。
这不是预处理全部传递依赖的形式化证明，也不是新做了跨库运行验证；回调、句柄生命周期及目标 C ABI 条件仍然保留。

## 十一个例外逐项辨别

以下每项的完整头均已有本目录快照。没有把头文件内联构造的 string 自动认定成传进平台库的 string。

| 包族 | 具体例外 | 边界事实与本轮归类 | 后续仍需确认的事 |
|---|---|---|---|
| appcore-agent | `service_app.hpp` 的 `tizen_appfw::ServiceAppBase::Run(int, char**)` | 头内构造 C 生命周期回调表后调用 C service_app_main；是 C++ 便利封装，不是纯 C 头，也不是已证明 std 对象跨库 | 用户派生方法/回调异常与最终 SDK 开放 |
| capi-appfw-app-common | `app_common.hpp` 的 `std::string AppBase::GetId() const` | 方法体调用 `app_get_id(char**)`，在调用方包装成 string，用 free 配对；所选库边界仍是 C 数据 | 其他模板/回调及完整释放契约，不能凭一个例子背书整类 |
| capi-appfw-app-control | `app_control.hpp` 的 `SetOperation(const std::string&)`、`GetOperation()` | 头内用 `c_str()` 调 C API，返回 C 字符数据再包装；不是把该 string 引用直接传给平台 C 库 | 回调/数组方法等剩余路径；条件性 C 包装，不判必然不兼容 |
| capi-appfw-application | `app.hpp` 的 `UiAppBase` | 头内 C 生命周期回调适配类，继承上述 AppBase；不是只含 C 声明 | 派生回调、平台 SDK 实际开放 |
| bundle | `bundle_cpp.h` 的 `std::string GetString(const std::string&) const`、`std::vector<KeyInfo> GetKeys()` | EXPORT_API 类中外置方法声明，实质标准库类型签名；如果调用者跨库使用这层，不承诺 | 不能用 bundle.h 的 C 性质覆盖 bundle_cpp.h；产品是否公开 C++ 层 |
| capi-media-vision | `IObjectDetection::setEngineInfo(std::string, std::string)`、`getEngineType(...) -> const std::string&` | C++ 虚接口及标准库参数/返回引用，不能归纯 C；若跨库调用不承诺 | 这些机器学习后端/类型头是否实际作为应用 SDK，而非配方带入的集成材料 |
| capi-network-smartcard | `SEService.h` 同时有 `smartcard_service_api::SEService` 类与 `se_service_create_instance` C 函数 | 明确 C/C++ 混合，C 分支可按 C 类；类构造/析构、继承与回调不是无条件 POD 接口 | C++ helper 的依赖与开放范围、所有权/异常契约；没有从 using namespace std 推断容器跨库 |
| cion | `ClientChannel(std::string)`、`SendData(std::vector<char>, int) -> std::vector<char>`、`TryConnect(std::shared_ptr<PeerInfo>)` | 标准库对象、共享所有权、虚回调均可见；跨库 C++ 层不承诺 | 产品是否公开这些 .hh；应用与平台两端生命周期 |
| component-based | `Application::OnCreateFactoryMap() -> std::map<std::string, std::unique_ptr<Component::FactoryBase>>` | 虚接口返回容器及唯一所有权，明确不是仅 C/POD；不承诺 | 应用继承与 factory 销毁两端，真实 SDK 范围 |
| libscreen_connector | `RemoteSurface(const std::string&, Type, std::shared_ptr<WlSurface>, bool)`、`Bind(std::shared_ptr<WlSurface>)` | 导出类传 string/shared_ptr，成员有 unique_ptr；跨库这层不承诺 | 原生应用是否允许直用远端 surface 类；别与下层 C 句柄混为一层 |
| notification | `Manager(std::unique_ptr<IEventSender>, std::unique_ptr<IEventListener>, std::string)`，`Get(...) -> std::list<std::unique_ptr<AbstractItem>>` | notification-ex 扩展层含容器及所有权，不是传统 notification C 入口的同义词；不承诺 | SDK 是否开放扩展 C++ 层；不能按包族名称直接放行 |

代表证据：

- 四族内联包装：[service](raw/089_native_service_wrapper.stdout)、[common](raw/106_native_event_wrapper.stdout)、
  [control](raw/088_native_inline_control.stdout)、[application](raw/090_native_app_wrapper.stdout)。
- 标准库/对象例外：[bundle](raw/085_native_bundle.stdout)、[vision](raw/063_native_vision.stdout)、
  [smartcard](raw/062_native_smartcard.stdout)、[cion](raw/084_native_cion.stdout)、
  [component](raw/064_native_component.stdout)、[surface](raw/087_native_remote_surface.stdout)、
  [notification](raw/086_native_notification.stdout)。

**异常与所有权的证据强度**：unique_ptr/shared_ptr/容器返回值和虚析构是声明事实；
具体分配点、每条异常传播路径和跨运行库按类型捕获没有在这次快速确认中全部追完。
因此不写“全部不会抛”“全部可安全释放”或“全部必失败”。对四族头内包装，更不能把调用者本地创建的 std 对象
与真正传给不同标准库提供方的对象混算。

**范围决定**：本轮完成的是快速筛选及上述代表例外辨别，没有把这 11 个 Native 例外继续展开为新的全量逐函数项目。
它们是否实际进入产品 SDK、以及是否另批深查，列入人工问题；本批已确认的全量包族范围仍为 Dali/NUI 8 + Web 3。
