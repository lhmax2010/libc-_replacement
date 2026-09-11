# W1 续测：三组目标接口及专用 shim 边界

2026-09-11；分支 `codex/runtime-validation`。本报告是本次续测的现行结果，原轮 W1–W4 文件保留原轮证据范围。**总状态：PARTIAL，不是三组接口全部验收通过。**

## 结论总览

| 目标 | 本轮事实 | 判定及限定 |
| --- | --- | --- |
| `ewk_parse_cookie` | x86_64 原生、armv7l 物理板：GNU 同侧各 5/5 四字段正确；libc++ 消费方→GNU provider 各 5/5 在调用中触发 SIGSEGV | 已测混用方向 `CONFIRMED_INCOMPATIBLE`；完整双向矩阵 `PARTIAL`，没有 libc++ Chromium provider 的反方向实测 |
| `SetSyncBinaryReply` | 定位到浏览器侧真实接收/赋值路径；vector 表示有独立实测；未建立真实、有效实例上的回调运行格 | `PARTIAL`；实际回调行为 `NOT_OBSERVED`，不能判安全，也不能报告错值 |
| binder→Dali | 两架构上，真实属性包装源码子集→真实完整 core 的 4 格×5 次均正确；完整 GNU binder→GNU 依赖链各 5/5 正确；只将 core 换为 libc++ 时各 5/5 加载失败 | `PARTIAL`；属性子集通过不等于完整 binder/图形/虚调用兼容；已测的是具体依赖链的链接故障，不是所有 Dali 边都失败 |

[主矩阵](MATRIX.md)逐格链接全部五次原始输出。计数为 **16 个已执行目标格、80 次尝试**：60 次具体值符合，10 次 EWK 故障信号，10 次完整 binder 依赖链加载失败。**这不是完整需求分母**，未观测格不能从分母中消失。另有 shim、表示诊断、初次 Dali 矩阵和环境试跑，均未混进上述数字。

## 环境、组件身份与方法

- 按人工确认选择 `elementary-tizen`。x86_64 使用冻结 RPM 的实际动态库和 Tizen 加载器，在 `tmp/` 组装依赖目录；未安装或替换系统库。板端另实查安装了 `elementary-tizen-1.25.1-0.armv7l`。这两个环境不是同一产品镜像，架构结果分别记录。
- 修复的是**探针的依赖解析方法**：识别 RPM 的 SONAME 符号链接后找到 sessiond、jsoncpp 等；随后避免混用缺少平台扩展符号的宿主 PulseAudio/libpng。原轮“未取得”不等于平台没有这些库。详见 [闭包](closure.json)、`raw/025–026` 及各次加载诊断。
- GNU Chromium provider 是实际 `libchromium-impl.so`，不是重写的 cookie parser。直接定位其导出函数，没有把薄封装库的加载成功当作实现已执行。其原始成员、实现摘录和二进制摘要见 [源码事实](SOURCE_FACTS.md)、[成员索引](source_members.json)、[二进制摘要](supplement_binary_identity.json)。
- Dali 的 libc++ provider 是**原始源码完整构建的 `dali2-core`**，两架构都有；没有构造模拟 core。另一方面，重编的 binder 测试件只包括原样的 `common.cpp`、`property-value-wrap.cpp`，**不是完整 binder 包**。另外单独运行冻结/板端完整 GNU binder 作对照。
- 两架构完整 core 的配置/构建见 `build_dali_core.py`、`build_dali_core_arm.py` 和对应原始记录。CMake 输出仅放临时目录，关闭会在源码侧生成 pkg-config 文件的选项。原始 core 1,100 个文件、binder 477 个文件逐字节摘要对照归档一致；内存中翻转一个字节的正向对照有效。见 [源码完整性](source_integrity.json)。
- x86_64 GNU 探针/初版 GNU bridge 使用宿主 GCC 13.3.0 头文件和编译器；实际加载的 GNU 运行库来自 Tizen 闭包。libc++ 侧使用 Clang 22.1.8 与既有 libc++ 产物。**不把宿主头文件当成平台头文件**：vector 又用冻结 GNU 14.2 头文件独立重复五次。ARM 则使用 ARM devel 包和 GNU 14.2 头文件。
- 原 ARM/QEMU 编译包装器本轮报 frontend exit 255；未改其配置，改用 x86_64 Clang 22.1.8 交叉生成 ARM ELF、既有 ARM sysroot/链接器。**编译工具是否经 QEMU 与测试执行环境分开**：ARM 行为测试是在 rpi4 **物理板**，不是 QEMU 用户态。
- 每个有效运行格保存 `/proc/self/maps`，断言两套标准库与实际目标 provider/包装件同时加载，随后比较具体值。完整 binder 加载失败格另保存加载前实际 core 映射；失败后 binder 被加载器回滚，**不能声称该格完整映射断言通过**。

## 一、`ewk_parse_cookie`

### 直接调用：实测

固定输入：`sid=ABC123; Domain=example.test; Path=/probe`。初始输出对象为空；预期 `domain=example.test`、`name=sid`、`path=/probe`、`value=ABC123`、`expiry_date_utc=0`。

| 消费方→provider | x86_64 原生 | armv7l 物理板 |
| --- | --- | --- |
| GNU→GNU，同侧对照 | 5/5 四字符串、expiry 正确；消费方销毁路径完成 | 5/5 同上 |
| libc++→GNU | 5/5 在真实调用中 SIGSEGV；返回值/四字段/消费方正常销毁 `NOT_OBSERVED` | 5/5 观测 SIGSEGV；字段未返回 |
| libc++→libc++，同侧对照 | `NOT_OBSERVED` | `NOT_OBSERVED` |
| GNU→libc++ | `NOT_OBSERVED` | `NOT_OBSERVED` |

实测 `sizeof(std::string)` / `sizeof(EWKCookieContents)`：x86_64 GNU **32 / 136**、libc++ **24 / 104**；armv7l GNU **24 / 100**、libc++ **12 / 52**。这些是当前探针编译配置的数字，不外推到其他 ABI 开关。

x86_64 补充回溯位于实际 Chromium impl 中，调用栈包含 `ewk_parse_cookie`（[原始回溯](raw/054_ewk_backtrace_selective.stdout)）。更深的两帧没有符号名，**未推断具体源码行或究竟先读错哪个字段**。ARM 为避免板端 crash-manager 产物，在探针中装置信号记录器，观测 SIGSEGV 后退出 139；没有修改 provider 或系统信号配置。这个 ARM 结果应称“观测故障信号”，不是默认信号处理下的 core dump 实测。

缺口：本次取得并运行的是 GNU Chromium provider；没有取得/构建配套的 libc++ Chromium impl。实际源码已流式读取，但**源码可得不等于迁移后的完整 provider/运行配置已可得**。反方向及其同侧对照均未运行，不以本方向替代。需要对应的真实 libc++ 产物、构建身份与依赖配置。

### 专用 shim：既有成功，也有明确的原型问题

本轮不是仅引用 Boost 机制：确实构建了两个 TU（独立编译单元），前端使用 libc++、后端使用 GNU，中间仅传字符指针、长度、标量和不透明回调上下文；后端调用**同一个真实 Chromium provider**。原对外签名保留，`dlsym`/`dladdr` 检查实际函数来自指定 provider，内部桥函数隐藏可见性。见 [构建与运行脚本](run_ewk_shim.py)、[结果](ewk_shim_results.json)。

- 对上述完整 cookie、空输出对象：x86_64 libc++→shim→GNU **5/5** 四字段正确，前端对象作用域销毁完成，三方映射满足。GNU→GNU 直接调用与 libc++→GNU 直接故障构成对应对照。
- **进一步发现原型语义不完整**：输出预置 `domain=keep.example`、`path=/keep`，输入仅 `sid=ABC123`。真实 GNU 直接调用 **5/5 保留**这两个未被输入指定的字段；实验 shim **5/5 将其清空**，虽然返回成功且销毁完成。值断言退出 28。见 [复用输出实测](ewk_reused_output_results.json)、[直接调用样本](raw/ewk_reused_output_gnu_1.stdout)、[shim 样本](raw/ewk_reused_output_cxx_1.stdout)。
- **静态原因**：真实实现有条件更新 domain/path；原型后端总是建立空输出再把四字段传回，未传入调用前的输出状态。没有把这一差异掩盖成“全部通过”，也没有对平台实施修正。

因此：**指针/长度可承载数据，单向固定样本的转换机制已实测；这个原型不能直接作为保持全部原语义的部署实现。目标部署仍 `SHIM_UNCERTAIN`。** 上述状态差异不证明双 TU 思路不可行；它说明还要处理输入/输出状态、失败后的更新规则等，不能只按四个新字符串复制估价。反方向、异常/内存不足、并发、其他输入、ARM shim、旧 ELF 自动路由均未验证。

原型规模：2 个 TU；数据路径为原签名入口、GNU 后端调用/转换、统一四字段接收函数，另有 2 个配置/定位辅助函数。不是每个字符串都必须单独写一对函数。为保持复用输出语义，还需传递/保留调用前的状态；完整部署代码量 `NOT_AVAILABLE`，不能把此原型的规模当总改动量。

## 二、`SetSyncBinaryReply`

### 真实路径的静态核查

实际浏览器实现：`Interface::SyncMessagingSetSyncBinaryReply(instance, const std::vector<uint8_t>&)` 查找有效实例，再调用 `Instance::SyncBinaryReplyToJS(reply)`，后者执行 `sync_binary_reply_ = reply`。从插件回到引擎的这一次回复是 **插件提供 vector、引擎读取/复制 vector**；不能因最初请求由引擎发起，就把回调的方向反过来。[原始源码摘录](SOURCE_FACTS.md)

同一归档还有 service/node 实现，它只记录“不支持”，不读取 vector。**没有使用这个 no-op 作为浏览器数据传递的正向对照。** 源码归档共扫描 921,773 个成员，按相关文件名读取 6,356 个候选，找到三处目标名所在文件；这只是本次检索范围，不能称全平台只有这些调用。[检索索引](extension_source_members.json)

### 为什么实际回调矩阵仍是 `NOT_OBSERVED`

- 真实回调需要已注册的有效 `XW_Instance`，通过实际 `XW_Initialize` 的接口表取得入口。当前 impl 的动态导出表检索没有得到可直接调用的这些 C++ 私有入口；同一 ELF 的 `ewk_parse_cookie`/EWK 初始化导出作为检索正向对照。**不等于内部没有实现**，实现源码已找到。
- x86_64 真实 `ewk_init()` 前置试跑等待 Wayland server，30 秒到期，未取得有效浏览器实例（[记录](raw/053_browser_initialization.stdout)、[诊断](raw/053_browser_initialization.stderr)）。仅仅 `dlopen` 成功不解决实例初始化。
- 未构造假 manager、假实例、手写接收回调或无效 ID 的 no-op 来替代真实引擎。ARM 本轮未建立有效 Web 应用/插件会话；不是以板子不可连接为理由跳过。
- 两方向及同侧对照，实际回调长度/内容均 `NOT_OBSERVED`。还缺可复用的真实浏览器/插件初始化流程和对应两套 provider 产物。没有把这些格记为 `CONFIRMED_SAFE` 或 `CONFIRMED_INCOMPATIBLE`。

### vector 内部表示：独立实测，非接口替代实测

x86_64 普通配置，冻结 **GNU 14.2 头文件**与 libc++ 头文件各 5 次。每次包含长度 **0、1、4、257**、预留容量 **512** 的四个对象，逐字节比较 `(i*37+19)%256`，读取对象的原始三个机器字并对照 `data()`、`size()`、`capacity()`；随后在本库内复制、清空原对象、销毁，检查复制内容。

两侧均为 24 字节、对齐 8；四个所测状态中的三个字分别与 **数据起点、已用末尾、容量末尾**一致，全部比较通过。不是只测 `sizeof`。此前宿主 GNU 13.3 头文件的额外 5 次也保留，但不拿它代替平台 GNU 14.2 头文件结果。[GNU 14.2 结果](vector_gnu14_results.json)、[libc++ 与附加 GNU 13 结果](vector_representation_results.json)

**能说到的强度**：所测普通配置、四种状态的表示位置和含义一致。**不能说**：真实 `SetSyncBinaryReply` 已兼容。该辅助探针不调用该回调，没有验证跨库赋值/销毁、调试模式、自定义 allocator、空但未分配对象、异常路径或完整实例生命周期。每次虽也加载真实 provider 并留 maps，但加载它不等于调用了目标接口。

### shim 适用性：静态可表达，部署未证

`uint8_t` 序列可以表达为 `const uint8_t* + size_t`：发送侧取本库的 data/size，接收侧创建本库 vector，并在真实实例上调用/赋值。这里不必传 allocator 或 vector 对象，也不应让另一库释放发送侧缓冲。

针对**一个选定回复入口**可有一对两侧转换入口；还需要接口表/回调注册适配、`XW_Instance` 生命周期与必要的上下文映射。不能只写两个普通函数就声称已接入实际回调链。完整相关入口数与实现工作量尚未量化；零长度、借用缓冲的有效期、重入、失败处理仍需验证。结论 **`SHIM_UNCERTAIN`**，不是数据形态不可转换。

## 三、binder 与 Dali

### 两架构实测范围

| binder 消费方→core provider | x86_64 原生 | armv7l 物理板 |
| --- | --- | --- |
| 原样属性包装子集 GNU→GNU，对照 | 5/5，7 个值及销毁正确 | 5/5，同左 |
| 原样属性包装子集 libc++→libc++，对照 | 5/5 | 5/5 |
| 原样属性包装子集 libc++→GNU | 5/5 | 5/5 |
| 原样属性包装子集 GNU→libc++ | 5/5 | 5/5 |
| 完整 GNU binder + GNU core/adaptor/toolkit | 5/5 相同属性用例正确 | 5/5 |
| 完整 GNU binder + **仅 core 改为 libc++**，adaptor/toolkit 仍 GNU | 5/5 加载失败；属性调用未发生 | 5/5 同一缺失符号 |

七个值为整数 `0`、`-123456789`、`2147483647`，浮点 `1.25`，空/短/长三个字符串。每个对象都经实际包装入口创建、读回，再通过真实 binder 删除入口销毁；返回给探针的 C 字符串依照该包装的 `strdup` 路径释放。探针注册真实 SWIG 的异常回调，断言异常计数为 0。**这是被测销毁路径执行，不是泄漏检测或任意 allocator 互换证明。**

这里的字符串参数实际是 **`Dali::String`**，不能因它名为 String 就写成传 `std::string`；本轮代码未模拟该类。源文件、编译命令、provider 摘要见 `binder_property_source_identity.json`、`arm_binding_identity.json`、`dali_binary_identity_final.json`、`arm_board_uploads.json`。

### 完整依赖链失败的具体位置

两架构都报告 GNU adaptor 导入的下列方法无法解析：

```text
Dali::Integration::Scene::GetFrameRenderedCallback(
  std::vector<std::pair<std::unique_ptr<Dali::CallbackBase>, int>>&)
```

上面为去掉默认模板实参后的易读写法；完整 mangled name 与诊断原文保存在各次原始输出，不以简写作符号匹配。GNU core 导出 GNU 命名的版本；新 core 导出含 `std::__1` 的 libc++ 版本。两份导出表还共同命中 `Property::Value::Get(int&)`，作为检索正向对照。[x86_64 符号对照](raw/049_dali_symbols_source.stdout)、[ARM 新 core](raw/086_arm_provider_linkage.stdout)、[ARM 加载失败](raw/arm_physical_dali_full_gnu_to_cxx_1.stdout)

**事实能约束的迁移状态**：本轮这个“旧 adaptor/toolkit/binder + 新 core”组合不能完整加载。这不是“完整 libc++ Dali provider + 旧 binder”实测，也没有证明 binder、core、adaptor、toolkit 任意两个都必须同批迁移。不能从属性子集通过反推出整套安全；也不能从这一条缺符号反推出每条属性调用都不兼容。

### 未覆盖的对象/虚调用路径

x86_64 完整 GNU 同侧 `CSharp_Dali_View_New`→`Control::New` 的初始化前提试跑也触发 SIGSEGV，回溯经过 `Dali::CustomActor`、`ControlImpl::New`。当前未建立所需图形/核心初始化上下文；**没有通过的该路径正向对照，因此不归因为跨库故障**。[试跑](raw/051_control_initialization.stdout)、[回溯](raw/055_control_backtrace_selective.stdout)

完整 libc++ binder、完整迁移后的 adaptor/toolkit 集合、Control 的状态/虚调用、回调重入、异常跨边界、全部对象所有权，以及任意模板参数均未验证。属性子集结果只覆盖列明的入口，不能写成“Dali 全部已验证”。

### shim 适用性

- 属性中的字节/字符串等**值**可以做指针/长度转换；本轮实际属性子集无需这种转换也通过，但不因此支持整个接口族。
- 对象地址能作为**不透明句柄**传递，前提是另一侧不把它解释为本库 C++ 对象、不自行作虚调用或按本库布局析构。句柄本身不是对象 ABI 转换。
- 保持原来的 C++ 对象/虚调用语义时，仅传指针与长度不够：要另外定义方法调用、引用计数/销毁、回调与状态协议。已看到的 `vector<pair<unique_ptr<CallbackBase>, int>>` 也不是简单字节 vector；其中有回调对象及所有权。
- 因此不是“一对转换函数覆盖 Dali”。工作量以确认后的方法集合、对象生命周期和回调集合为单位，目前完整数量 `NOT_AVAILABLE`；没有编造总函数数或人时。**`SHIM_UNCERTAIN`**，未实测完整对象代理方案，也未证明所有 C 句柄协议都不可能。

## 代价、缺口与自行判断

| 项目 | 本轮能确认的代价/条件 | 尚缺材料或验证 |
| --- | --- | --- |
| 字符串/字节 shim | 两侧各按自己的类型构造/销毁；复制数据；上游字段/签名变化会影响对应转换 | 端到端计时、分配计数、所有输入/失败语义、真实反方向 provider |
| EWK 专用原型 | 除输出四字段外，复用输出还涉及调用前状态；已出现成功返回但字段静默改变 | 保持该语义的完整协议与验证；不能直接部署当前原型 |
| vector 回调 | 字节表示可复制，但需接入真实接口表与有效实例，不能跳过注册链 | 两种引擎/插件配置、有效 Web 会话、真实回调运行矩阵 |
| Dali 对象 | 需要逐方法/生命周期边界，而不是把对象内存当字节数组复制 | 完整迁移后依赖集合、图形初始化方式、方法与所有权清单 |

所有复制开销只有结构分析，**未给未测的耗时数字**。异常不能作为 C++ 异常对象直接穿过这里的 POD 通道；迭代器不能脱离其容器和库实现随意转换；所有权必须留在可正确销毁的一侧，或有另行定义的配对释放协议。这些是协议约束分析，不是已有全量运行验证。

本轮自行采用的执行解释：以实际属性包装子集隔离无图形前提的 binder→core 边，同时把完整 binder 的不同结果单列；用配套 Tizen 加载器诊断而非替换系统；ARM 编译故障后换用同版本原生交叉编译方式；只在探针中记录故障信号；shim 只在临时目录作实验，并明确其状态语义缺陷。均未修改平台源码/配置、未实现生产方案、未选择迁移批次。

仍须人工/组件方补齐：libc++ Chromium provider 及配置；真实浏览器/插件实例初始化流程；完整 libc++ Dali 依赖集合及图形初始化方法；正式 SDK 暴露范围和产品清单。**devel 头文件可见不等于正式 SDK 承诺**。本轮未新增 Native 包装运行结果，原轮八个 ELF 的编译链接证据不被改写为运行安全。

## 板端、审计与交接

板端上传目录 `/opt/usr/home/owner/share/tmp/api_w1_0911_1050` 创建前确认不存在；所有上传件 SHA256 与本地一致。执行后按确切上传清单逐文件删除并 `rmdir`，确认目录不存在。[上传清单](arm_board_uploads.json)、[清理核验](arm_board_cleanup.json)。删除的是可由本地临时构建物恢复的测试副本；没有删除其他任务资料或替换系统库。**本轮占用已结束，板子已释放。**

构建物仅在 `tmp/API_0911/`，不进 git。脚本最终快照及 SHA256、各核心命令原文/退出码/原始输出随材料保存；板端的 `API_TARGET_EXIT` 与主机 SDB 退出码分开记录。初次失败、重跑和诊断均保留，不只保存成功输出。若干只用于展示既有日志的读取命令未单独重复落盘，不能声称整段交互的所有读取命令已完整审计。旧版与最终版探针的差异在 [脚本版本说明](SCRIPT_VERSIONS.md) 说明。自检见 [SELF_CHECK.md](SELF_CHECK.md)。

推送仅针对 GitHub `codex/runtime-validation`，不推 Gerrit、不 force。既有两份 R115 脏文件不在本次改动范围。完成后交人工审阅，不代业务部门选择方案。
