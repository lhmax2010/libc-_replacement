# 四处接口验证：PARTIAL

## 边界与停止原因

本轮使用冻结 x86_64 平台 RPM、真实头文件和既有 libc++ 运行库。未修改平台源码、配置或运行库。没有使用模拟 provider。所有 **接口行为运行格均未完成**，不得把下述加载尝试、编译成功改称跨库值验证通过；armv7l 为 `NOT_OBSERVED`，未占板。

Chromium 的依赖 `libelementary.so.1` 在冻结表中有 `elementary` 与 `elementary-tizen` 两个候选。需要确认产品依赖闭包，不能自行选变体。按任务书暂停 W1 后续接口实测，记录问题后进入独立的 W2。没有耗尽八小时；用时见状态表及原始 UTC 时间。

## 逐项结果

| 接口 | 判定 | 已取得的证据 | 未覆盖 |
| --- | --- | --- | --- |
| ewk_parse_cookie | PARTIAL | 实际 libchromium-ewk.so 导出同名 C 符号，导入 GNU string 符号；从 chromium-efl-squashfs 原镜像隔离解包真实 libchromium-impl.so；9 次逐级加载前提检查 | 在 elementary 二选一前停止；四字符串值、销毁、两方向、同侧运行控制均 NOT_OBSERVED；目标 libc++ 版 provider 未取得 |
| SetSyncBinaryReply | PARTIAL | 真实 webapi-plugins 源码的函数指针签名和调用点；插件产生 vector 并调用引擎提供的接口表 | 引擎真实回调及实例上下文未建立；两方向内容/长度、销毁、同侧控制均 NOT_OBSERVED |
| binder → Dali | PARTIAL | 真实 binder/core/adaptor/toolkit 及 ELF 依赖；尝试补齐 40 轮加载依赖 | 40 是本脚本诊断批次上限，不是平台不可运行的证据，也不是任务超时；W1 待产品闭包确认时停止续试。Control::New、混库值与销毁未测，不判必须同批迁移 |
| Native 四族所选包装 | PARTIAL | 两套标准库 × 四种代表包装，共 8 个最终 ELF 编译/链接成功；平台导入均是所选 C 入口；C++ 符号正向控制 8/8 可检出 | 未在有效 Tizen 应用上下文运行；app-control 加载尝试止于 libsessiond.so.0；没有整族安全证明 |

**没有有效的 5 次/格行为矩阵**。这不是把重复次数降为一次：行为格尚未进入，加载诊断是前提检查。每次需要两库/provider maps、值断言及同侧正向对照的要求仍未满足，不报告 `CONFIRMED_SAFE` 或 `CONFIRMED_INCOMPATIBLE`。

## Native 最终 ELF 的具体事实

代表用例：[native_probe.cpp](native_probe.cpp)、[最终结果](native_final_results.json)。

| 包装 | 实例化方法 | 最终产物的平台导入（两库相同） |
| --- | --- | --- |
| app_control.hpp | SetOperation/GetOperation，非拥有句柄 | app_control_set_operation、app_control_get_operation、app_control_destroy |
| app_common.hpp | AppBase::GetId | app_get_id |
| service_app.hpp | ServiceAppBase::Run | service_app_main、add/remove_event_handler、app_control_destroy |
| app.hpp | UiAppBase::Run | ui_app_main、add/remove_event_handler、app_control_destroy |

源码里 string 转为 char 指针/char**，回调经 C 表与 void* 往返，C++ 对象解释留在应用侧。真实头文件身份：[native_header_identity.json](native_header_identity.json)。
最终 ELF 并非“不含 C++ 符号”：本地 std::string、异常运行时、类型信息等仍有依赖；额外的 substr 正向控制确保检索能看到它们。不能仅凭 C 名称排除 C++ 参数，须与上述完整头文件/实例化路径联合阅读。

链接显式使用 `--allow-shlib-undefined`：只验证所选应用符号与真实平台库的链接形态，不验证平台库的全部传递依赖可解析。没有运行 `native_entry.cpp` 的空句柄入口；它不构成有效 app-control 或 UI/service 应用上下文。任意用户派生回调、异常传播、模板实例、全部其他成员仍未覆盖。当前只能把**所选平台边**归 C 数据形态，不能把四族整体升格为安全。

## 材料与可复核方法

- [catalog.json](catalog.json)：冻结包和本地真实产物路径；不是全平台迁移产物清单。
- [provider_paths.json](provider_paths.json)、[load_results.json](load_results.json)：实际依赖候选与停止点。
- `raw/008_ewk_dynamic.*`、`009_ewk_symbols.*`、`012_image_list.*`、`014_image_extract.*`：导出/依赖/镜像解包原始记录。
- `raw/017_callback_direction.*`：真实调用方向。初次 rg 因默认忽略规则未见正文，改为 `--no-ignore` 后检出真实正向样本；初次零输出**不作为结论**。
- `raw/native_r2_*`、`raw/native_final_*`：编译、重定位、最终链接、动态依赖及未定义符号。
- `raw/load_*`：完整加载失败原文。没有把 LOAD_ONLY 或加载失败解释为 ABI 兼容/不兼容。
- 上轮完整声明、SDK 范围限制继续适用：[`P7 stage2`](../../P7_0909/stage2/REPORT.md)。

## 自行判断与缺口

只在 `/usr/include/` 中解析 Native SDK 依赖，排除 GCC 内部同名配置头；同内容多拥有者须 SHA256 相同才合并。首次失败未隐去。
宽泛缓存目录扫描主动停止，改用冻结包/ELF 清单；因此不据该中断扫描声称其他位置不存在 libc++ 版目标组件。
需要产品确认的 elementary 变体、匹配的完整运行环境、两方向真实 provider、有效引擎实例与 Dali 初始化条件，见总疑问文件。
