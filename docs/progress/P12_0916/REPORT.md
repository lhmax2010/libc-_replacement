# adaptor 实施候选：实测结果与交付边界

## 当前结论更新：P13

原后端分配失败的 5/5 SIGABRT **已修复**，现在 5/5 返回 ENOMEM、进程存活；两种放法的 12 场景和 17 边界回归通过。当前仍为 **RELEASE_BLOCKED，但原因已改变**：在真实 provider 内部 malloc 点失败时出现稳定累计的未配对分配，不能承诺所有资源释放；析构双异常逸出的要求也不能由外层 catch 实现。生产方向、armv7l 等仍未验证。

当前修复代码、错误契约和部署条件请看 [P13 交接](../P13_0917/FINAL.md)、[错误契约](../P13_0917/W3/ERROR_CONTRACT.md) 和 [部署更新](../P13_0917/W3/DEPLOYMENT_UPDATE.md)。以下内容及 code/ 是 **P12 历史证据**，保留用于复核，不应当作修复版的最新结论或源码。

## P12 历史记录

结论：**PARTIAL / RELEASE_BLOCKED，不能作为已验收的生产实现直接上线。**

真实 GNU EWK 上，转发层的两种确定性放法均验证成立；但错误路径发现后端分配失败时进程终止（5/5），未达到人工要求的完整错误处理。生产方向还缺真实 libc++ EWK。以下事实不能被“普通场景全部通过”替代。

## 1. 已完成的代码与机制

源代码和可复现脚本见 [code](code/)，两种源码/GN/spec 候选补丁见 [patches](patches/)。实验工作目录是 `tmp/P12/`；没有改平台源码、配置或系统库，没有连接离线开发板。

两个编译单元各自使用明确的一套标准库头文件，内部仅传指针、长度、整数和回调。四个输出字段在 provider 侧先按调用前状态构造，完成解析、转换、分配后，应用侧才交换有变化的字段；对象由构造它的一侧销毁。没有把 GNU 字符串地址直接当 libc++ 字符串读取。

独立 DSO 导出原名 `ewk_parse_cookie`，另外导出供转发层直接引用的唯一入口 `p12_ewk_parse_cookie`。正式候选不含测试故障注入入口。其内部 EWK 结构只由后端实例化，避免该结构的同名隐式析构函数在同一 DSO 内合并两套字段布局。

### 转发层两种放法

| 放法 | 确定路由 | 实测范围 | 未验证部分 |
| --- | --- | --- | --- |
| 独立 DSO | 真实生成器的 cookie 分支直接引用唯一 adaptor 入口；转发 DSO 的 DT_NEEDED 指定 `libewk-cookie-adaptor.so.1` | x86_64，反向组合，两套库和真实实现同时加载 | 产品 GN/RPM、安装升级、生产 ABI 配置 |
| 内嵌 | 同一生成器分支直接引用链接进转发 DSO 的两个 adaptor 对象；原 wrapper 继续导出旧名字 | 同上 | 同上，以及整个产品符号/分配器/异常环境 |

两者都不使用 LD_PRELOAD，也不让全局 dlsym 的搜索顺序来选择 adaptor。后端用编译时指定的绝对路径取得实现句柄，对该句柄查找真实函数，并检查符号确实属于该根 DSO；自指向拒绝为 ELOOP。句柄按进程生命周期保留，失败初始化不会在同一进程重试。

这与“给未修改的原转发二进制加 LD_PRELOAD”不同：原二进制按绝对实现路径打开库、按专用句柄 dlsym。我们修改的是**原生成器源码的副本**。源自真实 SRPM 的 generator/main、SDK 声明和真实 GNU 实现，不是模拟 EWK；但转发目标使用独立 non-TV 测试配置重编，并非完整产品 GN/RPM 重建。

事实链：原始反汇编显示专用句柄查找 → 修改真实生成器只为 cookie 插入明确路由 → 真实源目标构建成功 → 每次确认入口 dladdr 落在转发 DSO，maps 包含双库、入口与 provider → 断言四字段、expiry、返回值、复用语义通过。已排除依赖加载顺序的部署方法。若产品转发源/ABI/实现查找规则不同，不能直接沿用本轮部署结论。

## 2. 实测矩阵（全为 x86_64 原生）

“每场景 5 次”通过每个新进程运行全部场景、重复启动 5 个进程实现。没有把进程数、场景数、字段断言数混用；下表不同批次不累加为一个覆盖分母。

| 项目 | 结果 | 原始索引 |
| --- | --- | --- |
| GNU 应用 → 无 adaptor 转发层 → 真实 GNU EWK | 12 场景 × 5 全部精确通过，销毁完成 | [加固矩阵](v2_EWK_RESULTS.json) |
| libc++ 应用 → 独立 adaptor 转发层 → 真实 GNU EWK | 12 × 5 全部精确通过，销毁完成 | 同上 |
| libc++ 应用 → 内嵌 adaptor 转发层 → 真实 GNU EWK | 12 × 5 全部精确通过，销毁完成 | 同上 |
| libc++ 应用 → 无 adaptor 转发层 → 真实 GNU EWK | 5/5 SIGSEGV；这是检错对照，不是通过格 | 同上 |
| 去掉测试导出后重新验证以上四格 | 三个正向格仍为各 12 × 5；混库对照仍 5/5 SIGSEGV | [发布候选矩阵](RELEASE_RESULTS.json) |
| 17 个附加边界；同侧、独立、内嵌 | 每格 5 轮，字段与同侧逐项一致；expiry 按时间窗/差值核对 | [边界](v2_BOUNDARY_RESULTS.json) |
| 前端四个字段逐个注入分配失败；旧 expiry 未初始化；未变字段存储保持 | 两放法各 5 轮通过；注入失败返回 false/ENOMEM，调用方输出不变 | `raw/v2_fault_{separate,embedded}_{1..5}.*` |
| POD 协议、C 头文件、两侧 vector 协议、三种加载失败 | 6 组 × 5 轮通过；C11 编译通过 | [单元结果](UNIT_RESULTS.json) |
| 后端实际超大分配错误契约 | **5/5 SIGABRT，未返回预期 -ENOMEM** | [独立失败记录](ALLOCATION_CONTRACT_RESULTS.json) |
| 同一后端/用例仅链接 GNU 的正向对照 | 5/5 返回 -12（ENOMEM）、进程存活 | [GNU 对照](ALLOCATION_CONTROL_RESULTS.json) |
| vector 构造数据层 | 同侧 GNU、同侧 libc++、两个跨库方向：各 8 场景 × 5，通过长度、逐字节、实例值和销毁检查 | [数据层](v2_VECTOR_RESULTS.json) |
| vector 故意破坏首字节的检错对照 | 每轮 6 个非空场景被判失败；两个空场景保持通过，5 轮一致 | 同上 |

12 场景覆盖初始空输出、预置状态、可选 Domain/Path、显式空属性、拒绝、长字符串、Expires、Max-Age、重复调用。17 个补充场景包括 NUL、非 UTF-8、UTF-8、边界长度、重复属性、超大 Max-Age、拒绝前后复用和输入引用输出字段。原始 stdout 包含实际字段十六进制，不只退出码。

“销毁完成”指测试作用域正常退出及数据层明确清空释放验证，不等于完成泄漏检测、并发竞态检查或任意业务对象生命周期证明。maps 对普通 EWK/vector 矩阵是逐运行断言；故障辅助测试有的只打印 maps，有的仅检查指定错误及状态，不把它们宣称为完整路由矩阵。

## 3. 新发现：生产错误契约尚未成立

第一次协议单元测试原本拟验证总长度溢出，但选值没有发生加法溢出，进入实际巨量分配并终止。修正测试的算术输入后溢出检查通过；**原失败不是可以删除的无效证据**。

独立复测先成功加载真实 EWK，再限制进程虚拟地址空间为 1 GiB，使后端巨量字符串分配可控失败。5/5 输出 `terminate called after throwing an instance of 'std::bad_alloc'`，SIGABRT，未返回预期错误。后端源码虽有 typed catch，实测仍未达到该契约。测试描述的超长借用区不会被成功分配并读取；该试验检验失败路径，不是合法超长业务缓冲区的完整语义样本。

**已确认：所测配置下错误处理不完整。未确认：该次失败的 throw/personality/typeinfo 精确归因。** 双异常运行时的候选解释不能在此写成新实测事实。未擅自增加吞掉任意异常/强制展开的 catch-all，也未改变运行时配置来掩盖失败。是否扩展隔离实验已提问，等待裁决。

补充对照使用同一个后端对象和同一份用例，只移除应用对 libc++/libc++abi 的显式链接，5/5 返回预期 -ENOMEM。这个有限对照支持“错误捕获对运行时链接配置敏感”，不单独证明 foreign exception 或类型身份分裂中的哪一种。

另一个既有 provider 行为：128 位十进制 Max-Age 输入，在同侧与两种 adaptor 中都产生溢出相关的负 expiry。源实现 `strtol` 后直接加当前时间，没有相应 ERANGE/加法溢出防护。这里只记录一致性，**不把一致的错误行为说成正确**；未修改真实 parser。代码保留 provider errno，但本轮边界表没有单独断言该场景 errno 的精确数值。

## 4. 生产方向、真实回调、架构

| 范围 | 状态 | 缺什么/已准备什么 |
| --- | --- | --- |
| GNU 应用 → libc++ EWK | NOT_AVAILABLE | 真实 libc++ `chromium-efl` 实现及匹配的头文件、ABI 命名空间、工具链/依赖证据；生产侧两 TU 编译候选与 [补测入口](code/future_ewk_matrix.py) 已备 |
| SetSyncBinaryReply 实际注册回调 | NOT_AVAILABLE | 真实宿主、XW 实例初始化、驱动入口；[待补编排脚本](code/future_callback_matrix.py) 已备，但仍须实现/提供真实宿主 driver，不能说路径已可运行 |
| armv7l | NOT_OBSERVED_BOARD_OFFLINE | 无物理板实测、无 QEMU 替代；未连接板子 |
| 产品 GN/RPM 构建、签名、安装升级/回退 | NOT_OBSERVED | 两份补丁干净应用、独立构建入口成功，不等于实际产品打包测试 |

vector 后端目前调用的是明确标注的构造接收器 `int p12_send_reply(int32_t, const vector<uint8_t>&)`。真实 XW 函数指针返回 void 且依赖已注册实例。没有用这个数据层返回值/生命周期契约冒充真实 SetSyncBinaryReply；即使 vector 大小同为 24 字节，也不因此承诺直接跨库可用。

## 5. 质量边界与自行判断

已实现：POD 空指针/长度组合、单项 max_size、聚合长度溢出检查；字节不擅自转码或按 strlen 截断；输出先准备后提交；根 DSO 归属、自循环、缺库/缺符号检查；GNU ABI=1 和两侧头文件类别检查；候选移除测试导出。

上述检查不能验证任意悬空指针或已损坏 C++ 引用；`sizeof` 哨兵仅检漂移，不证明 ABI 安全。调用前状态字段按真实源码保留，旧 expiry 不读取。候选对加载/已捕获错误采用 false+errno、保持输出；这是新增错误合同，已单独向人工确认，不能视为原 API 无条件保证。

没有确认任意 foreign exception、取消、并发首次加载/重入、new/delete 抢占、所有同名弱符号、不同 Chromium ABI 命名空间、旧 GNU ABI=0、全部发行 ELF、图形/TV 配置、升级中途部分组件组合。未选择最终部署放法，未假定业务部门同意就补齐技术验证。

“应用零改动”在这里仅指相同探针 ELF、同侧 ABI 不变时由部署层完成转换；不是宣称已审计所有发布应用，也不是同一个旧名字自动识别 GNU/libc++ 两类调用方。完整问题与人工待决见 [QUESTIONS.md](QUESTIONS.md)。

## 6. 交付与复核

操作、配置前提、部署候选和待测入口见 [DEPLOYMENT.md](DEPLOYMENT.md)。所有报告、源码/脚本快照、patch、原始记录纳入本线提交；构建 ELF 留在 tmp，不进入 Git。脚本/源码 SHA256 见 `SNAPSHOT_SHA256.json`，输入身份见 [INPUT_ARTIFACTS.json](INPUT_ARTIFACTS.json)，原始 SRPM 成员见 [SOURCE_INDEX.json](SOURCE_INDEX.json)。

资源 gate medium 通过；全程单并行小目标构建，nice 19 / ionice 3，每个构建子进程虚拟内存上限 4 GiB，小于本机物理内存的一半。没有全量构建 Chromium，单次构建未达到 500 目标的 I/O 探测节点；没有以虚拟地址上限冒充 RSS 实测。完成后停，交人工审阅；本次不签发上线通过结论。
