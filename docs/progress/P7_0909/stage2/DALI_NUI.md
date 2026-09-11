# Dali/NUI 与托管绑定：八个包族逐项核查

本文件的“观察”均为冻结源码/发布头/ELF 的静态核查，不是运行通过结论。
“C 化难点”是技术推断，不是推荐方案。最终 SDK 白名单未提供，不能将平台 devel 包等同于应用 SDK。

## 1. dali2-csharp-binder：C# 入口确实有 C 封装，但内部仍用 C++

应用调用链可分为三层：

```text
应用的 C# View.FindChildByName(string)
  → Tizen.NUI 内部 Interop.Actor.FindChildByName(HandleRef, string) : IntPtr
  → libdali2-csharp-binder.so 的 CSharp_Dali_Actor_FindChildByName(void*, char*) : void*
  → binder 内创建 Dali::String，调用 Dali::Actor::FindChildByName，再 new Dali::Actor
```

- 公开方法 `ViewPublicMethods.cs:530–535` 有 `since_tizen` 注释，调用 Interop，返回托管 View。
  [原始记录](raw/035_csapi_dispose.stdout)。该记录中 548 行另一个带隐藏标记的方法**不**作为已公开 API 的依据。
- `Interop.Actor.cs:58–59` 声明 `DllImport(..., EntryPoint="CSharp_Dali_Actor_FindChildByName")`。
  同文件也有 GetName/GetId/DeleteActor 等导入。[声明](raw/029_csapi_actor_declarations.stdout)。
- `NDalicPINVOKE.cs:26` 指定 `libdali2-csharp-binder.so`，不是让 CLR 直接调用 Dali 的修饰 C++ 符号。
  [库名与异常回调注册](raw/036_csapi_lib_callback.stdout)。P/Invoke 指 .NET 调用原生导出函数并对参数作编组。
- wrapper 的 `extern "C"` 块及 `void*`/`char*` 签名可在 actor-wrap.cpp 中核查。
  966–999 行将字符数据转换为 Dali 自有 String，并在原生侧分配返回句柄。[实现](raw/076_binder_lookup.stdout)。

独立 ELF 对照也成立：冻结 **x86_64** `libdali2-csharp-binder.so.2.0.0` 定义了未修饰的
`CSharp_Dali_Actor_FindChildByName`、`CSharp_Dali_View_New`、`CSharp_Dali_delete_View`，
而 `_ZN4Dali7Toolkit7Control3NewENS1_16ControlBehaviourE` 是其未定义导入，由 toolkit 提供。
动态依赖有 `libdali2-core.so.2`、`libdali2-adaptor.so.2`、`libdali2-toolkit.so.2`、`libstdc++.so.6`。
见 [binder 符号表](raw/elf_dali2-csharp-binder_libdali2-csharp-binder.so.2.0.0_symbols.stdout)、
[动态依赖](raw/elf_dali2-csharp-binder_libdali2-csharp-binder.so.2.0.0_dynamic.stdout)、
[toolkit 符号表](raw/elf_dali2-toolkit_libdali2-toolkit.so.2.0.0_symbols.stdout)。这是**部署前冻结产物**，不是迁移后库的运行加载记录。

### 创建、释放与错误通道

`CSharp_Dali_View_New()` 在原生侧调用 `Dali::Toolkit::Control::New(...)` 后分配 Control；
`CSharp_Dali_delete_View(void*)` 在 binder 内执行 `delete`。[实现](raw/077_binder_view_create_delete.stdout)。
C# `ViewInternal.cs:1363–1367` 的 `ReleaseSwigCPtr` 调 `Interop.View.DeleteView`；
后者显式导入上述删除入口。[释放分发](raw/078_csapi_dispose_path.stdout)、[删除导入](raw/079_csapi_view_pinvoke.stdout)。
这不是 C# 应用用自己的 C++ 标准库对 Dali 对象作 `delete`。不过释放时机、引用计数、回调和重复释放条件仍然存在。

普通错误有转换设施：`CALL_CATCH_EXCEPTION` 捕获 `std::out_of_range`、`std::exception`、`DaliException` 等，
设置 SWIG pending exception；C# 检查后抛出托管异常，而不是要求 C# 按 C++ RTTI 捕获异常对象。
[宏体](raw/037_binder_catch_body.stdout)、[C# 检查](raw/034_csapi_public_name.stdout)。
但是不能据此宣称“所有异常都被封住”：`control-wrap.cpp:166` 和 `actor-wrap.cpp:995` 的分配位于相应 try/catch 之外；
`CALL_CATCH_EXCEPTION` 还明确捕获 `abi::__forced_unwind&` 后 `return`（283–286 行）。
这是静态存在的限制；这次没有实测异常/取消是否到达此点及最终运行表现，不实施修复。

**分类**：所追踪应用边界符合 C 导出/不透明句柄的条件性类别，不直接传递 Dali C++ 对象布局；
不能扩写为“全部 NUI 接口均无 ABI 风险”。C# 数值结构编组、bool 宽度、调用约定、回调有效期、第三方自行 P/Invoke
或本地插件路径仍需各自契约。binder—Dali 的 C++ 边界仍需核对，C 封装不是它们内部混用标准库的豁免。

## 2. csapi-tizenfx：应用看到的是托管 API，不是 Dali 头文件

源码 RPM `csapi-tizenfx-15.0.0.19383-1.src.rpm` 已在本地，SHA256 为
`472490abfd8201b5dc01e412779a0cbebbd7499616e5f09f18a33bac67de9015`；本轮只读解出 NUI 相关成员。
[取得记录](raw/014_csapi_acquire.stdout)、[成员清单](tables/csapi_selected_members.tsv)。

所选 `View.FindChildByName(string)`、`View.ID` 等是 C# 方法/属性。`ID` 调内部 GetId，再经 Actor 的 C 导出。
`Name` 的非 XAML getter 在该版本读取托管 `internalName` 缓存，**不能把它画成每次必调原生 GetName**。
[公开属性](raw/067_csapi_name_property.stdout)、[实际缓存与 ID](raw/100_nui_name_bridge.stdout)。
本报告用 FindChildByName 的明确调用链作为主要证据，没有把存在同名 Interop 方法当成所有属性都必经它。

公开 C# 声明、`since_tizen` 标记、程序集交付与 SDK 最终可用清单不同；源码中确有 `EditorBrowsable(Never)` 方法。
本轮不把整个 public 文件夹或 C# `public` 关键字视为产品开放承诺。
**分类**：托管层及所追踪 C 绑定路径按上一节；不为整个 TizenFX、所有 profile 或所有自定义本地插件背书。

## 3. dali2：原生应用直接链接路径存在，SDK 开放仍待确认

安装头有 `dali/public-api/...`，开发包提供 pkg-config；冻结 core ELF 有真实 C++ 导出。
这些材料证明直接构建/链接的**技术入口存在**，没有证明指定产品的第三方 SDK 包含它。

例如 `BaseHandle(BaseObject*)`、`~BaseHandle()`、`BaseHandle(const BaseHandle&)` 具有引用计数/资源语义，
不是仅因其中存指针就可以当作 POD。[声明与生命周期注释](raw/042_dali_handle.stdout)。
公开 `ConnectSignal(..., T&&)` 通过模板分配 FunctorDelegate/函数对象，并向库侧传递销毁回调；
`FunctorDestroyer<T>::Delete(void*)` 在知道实际类型的模板代码中删除对象。[完整机制](raw/043_dali_delegate.stdout)。
这有配对机制，**不能写成必然错误释放**；但已经超出“只传 int/POD”的无对象接口，仍有跨边界所有权和回调条件。

该版本代表公开接口使用 **Dali::String / StringView 等自有类型**，不是 `std::string` 的别名结论。
所见 `std::move`、`std::size_t` 不能被计成跨边界传标准库对象。
graphics-api 中确有 std::vector，但属于另列的后端开发接口，本轮不把它纳入已开放应用面。
具体头见 [dali-string.h](raw/041_dali_string.stdout)，全体候选和内部路径见 `tables/boundary_headers.tsv`。

普通异常的明确例子：`Any::AssertAlways(const char*)` 在 any.cpp:128 抛出 `Dali::DaliException`。
[实现](raw/081_dali_throw_real.stdout)。跨 C++ 调用方捕获这一对象的实际混库结果为 `NOT_OBSERVED`；
不能因为它不是标准库异常就承诺不同运行库间的 RTTI/展开/捕获。

**分类**：原生句柄/模板/所有权路径不承诺整组混用；没有认定所有 Dali 原生函数必不兼容。
**C 化难点（推断）**：基本操作可以描述为不透明句柄和数值/字节参数；信号、继承扩展、对象销毁需要完整契约，不只是给符号加 extern "C"。

## 4. dali2-adaptor：Application 与事件循环

公开头中的例子：`static Application New(int*, char***, Dali::StringView)`、
`bool AddIdle(CallbackBase*)`、`Signal<void(Application)>`。
[签名](raw/044_adaptor_public.stdout)。
`AddIdle` 文档明确说回调调用后被删除、所有权交给该类，并要求主事件线程调用。[原文](raw/051_adaptor_lifetime.stdout)。

**分类**：有 C++ 对象返回和回调所有权转移，不归入“仅内置/POD”整组；不承诺任意跨库混用。
源码库内部的所有可能异常/任意回调没有全路径证明，记 `NOT_OBSERVED`，而非“不抛异常”。
**C 化难点（推断）**：应用生命周期、事件循环、回调上下文与注销/释放协议；不能忽略线程约束。
SDK 开放未定；binder 的原生依赖已静态观察到。

## 5. dali2-toolkit：直接 C++ 与 C# 包装是两条不同入口

`Control::New()` 返回 Control，类有构造、析构与继承关系。[公开头](raw/045_toolkit_public.stdout)、[工厂签名](raw/071_toolkit_factory.stdout)。
`dali2-toolkit.pc` 明确给出 `-ldali2-toolkit` 并依赖 `dali2-core`：
[实际 .pc](snapshots/pkgconfig/dali2-toolkit-devel/dali2-toolkit.pc)。真实 ELF 定义了 `Control::New`。
因此原生 native 应用直接包含头并链接的技术路径存在；**是否进入目标 SDK、是否获第三方使用支持仍未定**。

另一条已追踪入口是 C# → binder → `Control::New`，应用侧没有直接构造该 C++ Control。
这两条不能合并成“Dali 全不支持”或“经 C# 所以原生 C++ 也支持”。
同源码包交付的 Bullet/physics、scene3d/integration 材料没有被冒充普通控件 SDK。

**分类**：原生 Control 等 C++ 对象路径不承诺整组；C# 代表路径按第一节。
**C 化难点（推断）**：简单属性包装有限，但自定义派生控件、信号、引用计数需要回调/生命周期设计。

## 6. dali2-extension：不把内部开发头算成应用 API

四条头文件拥有记录中，`dali-extension.h` 只有 include guard；实质头为 `devel-api/rive-animation-view/...`
及 `integration-api/key-extension.h`，Rive 头被两个开发包拥有，不能算两种独立 API。
[实际路径](raw/046_extension_public.stdout)、[伞形头原文](raw/049_extension_umbrella.stdout)。
按本轮边界，不深入把这些原生 devel/integration 头当第三方 SDK；最终 SDK 是否另外开放为 `NOT_AVAILABLE`。

但已有托管扩展绑定不是空白：`Tizen.NUI.Extension/Interop.RiveAnimationView.cs` 导入
`libdali2-csharp-binder-rive-animation.so`，`New(string)` 返回 IntPtr。
[C# 端](raw/101_nui_rive_bridge.stdout)。C 导出 `CSharp_Dali_RiveAnimationView_New__SWIG_0(char*)` 内部构造
`std::string` 再调用 `RiveAnimationView::New`。[原生转换](raw/105_binder_rive_boundary.stdout)。
这正说明 C# 边界可以与内部 std::string 边界分离；后者不因应用只传字符串编组数据而消失。
产品是否公开该托管扩展及其完整生命周期路径，本轮没有补推。

## 7. dali2-ui-foundation：明确的标准库对象边界

`dali-ui-components/public-api/chart/chart-axis.h` 中：

```cpp
void SetLabels(const std::vector<Dali::String>& labels);
std::vector<Dali::String> GetLabels() const;
```

同类派生自 `BaseHandle` 并有构造/析构；这是实质标准库容器签名，不是 `std::move` 之类辅助调用。
[完整声明](raw/052_chart_std.stdout)。按框架，此接口若允许应用直接跨标准库调用，**不承诺**。
返回容器的对象生命周期与元素析构属于边界条件；本轮没有用没崩溃来判断它安全。

SDK 未定。不能因为包名带 Ui 或与 NUI 同属 UI 就宣称当前 `View` 一定经过此库；
本轮具体追踪到的基础 View wrapper 使用的是 toolkit Control。
**C 化难点（推断）**：字符串数组传入/取回、长度和编码、释放方，以及其他图表回调；不止修改链接名。

## 8. libscl-ui-nui：同包存在 C 入口和 C++ 实现头

17 个头文件拥有记录中，有明确的 C 接口：
`int scl_nui_init(Ecore_Wl2_Window*, const char*)`、`int scl_nui_fini()`，以及字符/数值/void* 形式的绘制回调。
[cscl-ui-nui.h](raw/056_scl_api.stdout)。冻结 ELF 另有 `libscl-ui-nui-capi.so.0.11.2`，并非只有 C++ 类库。
[产物身份](tables/elf_identity.tsv)。
同包的 sclcontext/sclstructs 等确有 std::map/list/string，但不能直接把这些实现结构算成 C 接口传递对象。
公开 SDK 配方没有相交依据，所选 TizenFX NUI 源码搜索也未找到 `scl_nui_init`/该库名；
同法能找到 NDalicPINVOKE 的实际库名与 Rive 导入，正向对照有效。
这是限定扫描的 `NOT_OBSERVED`，**不表示没有其他 C# 程序集、输入法或产品代码使用它**。

**分类**：已见的 C 参数入口符合 C 类别且有条件；内部 C++ 头不并入应用公开面，SDK/实际应用使用待确认。
**C 化难点（推断）**：这组入口已经有 C 层；若另要公开上下文 C++ 类，必须分离结构布局、状态与所有权，不能只改名。

## 证据强度小结

八族都已逐项列明入口、可见数据形态、SDK 证据与限制。没有对所有头文件中的所有重载作完整异常/所有权证明。
源码成员与冻结 RPM 的身份链见 `tables/source_identity.tsv`；头文件摘要见 `tables/headers.tsv`；
ELF 是只读静态检查，未用 `/proc/self/maps` 冒充实际运行加载。
