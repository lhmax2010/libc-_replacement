# LWE 回调夹具修复

结论：`GNU_CONTROL_FIXED`。x86_64 原生、armv7l 物理板的 GNU 同侧控制各 5/5，回调次数 1、内容字节 `33`（字符串 `3`）、销毁完成。跨库运行矩阵仍为 `PARTIAL`，不能把夹具修复写成跨库兼容已证。

## 失败的具体位置与原因

1. **静态核查**：`src/public/delegate/LWEWebContainerDelegate.cpp:986–1030` 将异步求值排入 message loop。没有驱动消息循环就不能期待同步得到回调。
2. **静态核查**：`src/core/page/WebView.cpp:1007–1018` 在没有 mainBrowsingContext 时仍调用回调，但传空字符串。R118 的“回调到了但值为空”符合这个分支；直接期待 `1+2` 为 `3` 缺页面前置。
3. **实测**：页面加载后原夹具抛 `bad_function_call`。本轮 GNU-only 调试重现，throw 调用点位于真实实现库基址加 `0x586175`，返回地址 `0x58617a`，见 `raw/lwe_original_trace`。没有把剥离符号的回溯虚构成源码行号。
4. **静态核查及定位推论**：`CreateHeadless`（上述 delegate:703–713）只创建容器，不安装绘制缓冲回调；`Renderer.cpp` 初始化 renderingPrepareCallback 为空，`RendererSoftware.cpp:106/131` 会调用该回调。原回溯中的空 function 检查/调用形态与之对应。缺调试符号，精确 C++ 源码行归属仍是源码与反汇编的对应推论，不是带行号的运行回溯。
5. **实测修复**：在夹具调用公开的 `RegisterCanRenderingHandler`，返回 false。`Renderer.cpp:418/464` 的 canRendering 门控停止绘制，但不阻止页面加载及脚本消息循环。两架构均得到 `3`。没有填入模拟 EvaluateJavaScript、没有重建或修改真实 provider、没有启动完整浏览器或显示窗口。

所读源码及哈希见 `source_manifest.json`；原始源码摘录在 raw/006、008、009；新程序见 [lwe_callback.cpp](../code/lwe_callback.cpp)。

## 矩阵与身份

| 消费方 / provider | x86_64 原生 | armv7l 物理板 |
| --- | --- | --- |
| GNU / GNU LWE | 5/5 回调 1 次、字节 33、销毁完成 | 5/5，同左 |
| libc++ / GNU LWE | 自然链接失败，无运行格 | 自然链接失败，无运行格 |
| GNU / libc++ LWE | NOT_AVAILABLE：没有真实 libc++ LWE 构建 | NOT_AVAILABLE：同左 |
| libc++ / libc++ LWE | NOT_AVAILABLE：缺 provider，不能借 GNU 产物充数 | NOT_AVAILABLE：同左 |

自然链接缺失涉及 `std::__1::function` 及 `std::__1::string` 的修饰名；不通过猜测另一侧符号名并强转函数指针来制造“自然调用成功”。链接失败不是运行崩溃，也未独立隔离每个类型造成的链接影响。

provider 来自冻结 source RPM `lightweight-web-engine-1.4.0-1.src.rpm` 的 mobile 产物：x86 RPM SHA256 `f2a206acdb684eae2183aff41e9b53351cebb50e0f2635b633a89e6ee155f3d7`；ARM `3e88844a0970832494dfb24067c7b8d52276413b200f44885ce43bb8b401f674`。矩阵 JSON 记实现 ELF 哈希及程序哈希。每次在实际进程内打印标准库与真实 LWE 的 `/proc/self/maps`；GNU 同侧不刻意引入 libc++。链接失败或无产物格没有可记录的运行 maps。

ARM 板上原库与冻结产物哈希不同，首次身份断言在上传前停止。之后冻结 LWE 全套同包辅助库、GNU 标准库、coregl 的 GLES/EGL/COREGL 被上传至独立 r119 目录，不替换板上文件。其他依赖来自板子现状，因此两架构不是同一个完整镜像环境；身份差异保存在 `arm_system_identity.json` / `arm_uploads.json`。缺 GLES 的尝试返回 42，补接真实 coregl 后控制通过；不将 loader 失败计为类型差异。

## 清理与偏差

上传件及 cookies.txt 已按实际清单逐项删除，storage 与 r119 目录已删除并核验不存在；见 `board_cleanup.json`。**一次旧版断言在 dlopen 失败时产生 SIGABRT，shell 报告 core dumped；系统诊断目录尚待权限确认，不宣称全部诊断已清理。** 后续夹具改为打印 dlerror、返回 42，避免把加载前置错误当崩溃试验。另一次板上无 timeout 命令，未启动程序，已改用程序内部等待期限与主机连接超时；均保留原始失败。

## 未覆盖及自行判断

- libc++ LWE 在已知可得产物中未取得；本轮不构建替代引擎。
- 自然混合链接失败，不能回填跨库回调值或销毁结果。
- 禁止绘制是本轮最小 headless 夹具选择；只覆盖脚本回调，不验证绘制、真实 UI 宿主、并发、重入或库卸载。
- 源码/反汇编对应不能替代精确调试符号；原错误不归为标准库差异。
- R118 历史原始记录不改，类型报告以本轮补充更新 D 节；原 SHA256 清单仍描述旧封存时点，报告更新另在本轮提交追踪。
