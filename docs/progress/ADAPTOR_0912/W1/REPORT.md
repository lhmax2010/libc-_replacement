# 三项缺失材料的自行探查

结论：三项均 **BLOCKED**，指尚未得到满足后续真实组件实测的完整前置条件，不等于技术上不可实现。已自行定位构建入口、插件注册链、离屏入口，并执行组件编译尝试及物理板初始化探针。没有要求人工提供 SDK 清单。

## Chromium/EWK 的 libc++ 构建

静态核查的原件来自 `chromium-efl-1.1.144-1.src.rpm`。`spec_index.json`、`config_index.json` 保存提取文件路径与 SHA256；原件只提取至隔离 tmp。提取遍历归档，不能把选择提取的文件数当完整源码数。

- `build/config/c++/c++.gni:17,28`：目标及宿主 `use_custom_libcxx` 默认均为 `!is_tizen`。**配置入口存在**，但把参数改为 true 不等于已完成本平台的依赖重建；本轮没有修改平台配置。
- spec 第 146–300 行等列出 EFL、ICU、系统服务、图形等构建依赖；第 690–697 行涉及组件与编译器选择；745 行开始 Ninja 构建，包含 EWK 测试、WRT 等目标。完整摘录在 `raw/027_spec_facts.stdout`、`031_spec_config_excerpt.stdout`。
- `tizen_src/ewk/efl_integration/BUILD.gn:104` 的 `chromium-ewk` 共享库在启用 EWK 时输出 `chromium-impl`；571 行包含真实 `ewk_cookie_parser.cc`。该目标的依赖包括 base、content 及其他 Chromium 部件，**现成目标不是只含 cookie parser 的小库**。
- 已尝试用现有 Clang、libc++ 头和选择提取的真实源码，单独编译真实 parser 翻译单元，未替换 parser 实现。实际失败为缺 `base/functional/callback_forward.h`，退出 1，详见 `raw/component_ewk_cookie_parser.*`。`ParsedCookie` 及日期解析还依赖 base/net 实现；只把 GNU 产物链接过来不能当作 libc++ provider。

构建耗时：**NOT_AVAILABLE**。没有此源码快照的可用 Ninja 构建图、该机器双并行度的编译吞吐或增量断点，不能编造“必定超过六小时”的数字。当前空闲磁盘约 36.4 GiB（命令实测），也不是全量构建空间需求的实测。没有据此宣称磁盘一定不足。当前 PATH 中 `gn` 查询未找到（该查询不代表所有目录不存在 GN）；`ninja` 有可执行文件。实际选择是保存组件编译断点，不启动未具备依赖闭包的全量构建。

所缺：该产品构建参数对应的完整源码/生成头与 base/net 依赖闭包，以及 libc++ 构建这些依赖的可复现目标。全部已取得与缺失边界在上述日志可核。**libc++ EWK provider 尚未产出，反向运行留作 NOT_OBSERVED**。证明真实组件能以一致 libc++ 构建并加载，即可解除此阻塞；不需要改写或模拟 parser。

## 浏览器/插件实例

静态事实链（`raw/011_chain.stdout`、`030_registration.stdout`、`027_spec_facts.stdout`）：

1. spec 的路径为 `%{_libdir}/tizen-extensions-crosswalk`，通过 GN `xwalk_extension_path` 传入；实际浏览器 manager 用 `lib*.so` 和元数据发现插件。
2. `XWalkExtension::Initialize` 加载真实 `.so`，取得 `XW_Initialize`，将真实 `GetInterface` 提供给插件；不是仅 dlopen 即完成注册。
3. `CreateInstance` 分配并登记有效实例标识；插件的 `src/common/extension.cc:346–348` 通过接口表调用 `SetSyncBinaryReply`。
4. 浏览器 `SyncMessagingSetSyncBinaryReply` 查实际 instance，执行 `SyncBinaryReplyToJS`；后者把 reply **赋值复制**到 instance 的 `sync_binary_reply_`，由同步消息返回路径取出。调用方向是插件回调进入引擎，不是宿主向插件传一个输出 vector。

已找到插件开发测试样例（`raw/004_source_paths.stdout`），包括测试扩展、实例、JS 和 GYP，以及工具程序。**这些文件证明存在样例，不证明它们是可直接调用真实浏览器接收器的独立宿主。** 对已提取的浏览器子树进行 test 文件搜索未得目标匹配，限定为该选择提取范围，不能推广成整个平台没有测试。正向定位使用同一真实源码中的回调实现及注册函数（`011`、`030`）。

最小宿主的技术入口已查明，但真实 `XWalkExtensionManager` 带有 content、Mojo bindings 与生成的 mojom 依赖，GN 文件明确列出。编译真实 `xwalk_extension.cc` 的尝试缺 `base/logging.h`（`raw/component_xwalk_extension.*`，退出 1）。此前已记录完整 GNU provider 动态导出不能直接取得这些内部入口；不以伪造 manager/回调替代真实路径。Node/service 实现的不支持分支仍不是正向对照。

所缺：能链接真实 manager/instance 的组件构建闭包和生成接口，或能启动真实浏览器测试实例的产品运行入口。现有证据**不能断言必须启动整个浏览器**，也不能断言已经可用。真实回调运行仍为 NOT_OBSERVED。

## Dali 依赖与离屏初始化

已有缺符号是 GNU adaptor 对 core 的 `Scene::GetFrameRenderedCallback(std::vector<std::pair<std::unique_ptr<CallbackBase>,int>>& )` 导入，换成 libc++ core 后修饰名不同。core 单独重建不能解决仍用 GNU 的 adaptor；完整 binder 还加载 toolkit。此前源/符号/矩阵在 `../../API_0911/W1_CONT/`，本轮没有把它扩大成“所有接口必须同批迁移”。

`dali-adaptor.spec` 还列出 EGL/GLES、EFL、字体、图像、平台服务等依赖（`raw/010_dali_headers.stdout`）。**core/adaptor/toolkit 是 C++ 依赖链，不是全部动态依赖清单**；真实探针 maps 给出实际加载的完整集合。

静态核查：`OffscreenApplication` 的真实公开头明确说明离屏 FBO、不自带事件循环。实现 `offscreen-application-impl.cpp:48–66,83–100,121–132` 创建窗口与 adaptor，Start/Terminate 管理其运行；不是完全绕过图形系统（`raw/024_offscreen_impl.stdout`）。

实测：armv7l **物理板**，完整 GNU binder/core/adaptor/toolkit；同时加载 libc++、libstdc++，每次归档 maps。探针使用真实头，不模拟 Dali 对象。

- 初次 5/5 在 New 中报告缺 `libGLESv2.so.2`，最终 SIGABRT，目标退出 134，尚未 Start/Control::New。不是跨库失败实验。
- 继续查询已安装 `mesa` 的文件清单，发现真实驱动位于 `/hal/lib/driver`。因此第一次的“缺库”仅是搜索路径不完整，**不是板子没有 GLES**。
- 第二轮仅给探针进程的 `LD_LIBRARY_PATH` 加 `/hal/lib/driver:/hal/lib`，不改系统配置。能够进入 GLES graphics factory，但 30 秒探针期限内未完成 New。重复及退出在 `hal_offscreen_results.json`；未观察到窗口有效/Control 构造/销毁，记 NOT_OBSERVED。不能从该超时认定硬件无图形能力。

所缺：在该板上可完成离屏 adaptor 初始化的运行上下文/进一步定位该等待的证据，以及完整 libc++ adaptor/toolkit。默认与 HAL 路径两轮分开保留，后者不抹掉前者失败。探针只断言初始化标记及非空句柄，**不是属性内容、渲染输出或虚调用兼容性的验证**。

## 自行判断、范围与清理

本轮自行选定 MANUAL/ECORE 离屏模式，30 秒单进程诊断期限，两种库路径各重复五次；未根据超时自行改系统图形配置。信号处理器仅在探针中记录信号并失败退出，避免 crash-manager 遗留，不把清理缺失记为通过。借助现有 libc++ core 作临时链接输入，运行 maps 明确实际解析的是板上 GNU core；不能据链接输入把本实验标为 mixed core。

W1 的独占上传目录 `adaptor_0912_w1` 每轮按确切上传清单移除后 rmdir 验证；不清理别的会话。W2 仍需使用板子，本阶段不声称包的板子已释放。两处用户 R115 修改保留。无平台源/配置改动、无 Gerrit 推送。
