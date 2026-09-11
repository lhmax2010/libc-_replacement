# 平台对外 API 跨标准库兼容性盘点：第一阶段范围报告

状态：**`STAGE1_COMPLETE_AWAITING_CONFIRMATION`**。仅完成范围界定、头文件形态初分及第二阶段建议。
**第二阶段尚未开始。** 本报告不新增“支持/不支持”结论，不修改已交付 API 报告的判定框架。

## 一、范围已经收敛到什么程度

建议第二阶段先核查 **27 个源码包族、28 个源码 RPM 版本记录**：
其中 26 族来自 40 个开发候选包版本组，另加 `csapi-tizenfx` 托管侧。
不是把全部开发包当作应用 API，也不是把一个包视为一种不可分割的接口。
完整建议清单在 [STAGE2_RECOMMENDED.tsv](tables/STAGE2_RECOMMENDED.tsv)，等待人工确认后才开展逐接口分析。

本阶段沿用此前冻结的 Base/Unified 仓。统计是该复合快照的可见范围，不是在线最新仓、全部产品或第三方应用 BOM。
完整方法见 [METHOD.md](METHOD.md)，待确认事项见 [QUESTIONS.md](QUESTIONS.md)。

| 计数对象 | 数量 | 含义 |
|---|---:|---|
| 分架构二进制 RPM 记录 | 11,332 | 两个冻结二进制仓的原始记录 |
| 包版本组 | 4,472 | 按仓、包名、epoch/version/release 归并架构 |
| 名称含 devel/headers 的包版本组 | 980 | 原分架构记录为 2,571；包名只是筛选条件 |
| 含识别到的头文件或开发包名的候选 | 1,070 | 逐一查询 RPM 并复核载荷 SHA256 |
| 非聚合 include 头文件提供方包版本组 | 960 | 下文五类统计的分母，不等于 960 个已承诺公开 API |
| 其余候选 | 110 | 36 个仅在非 include 路径有识别到的头文件、73 个开发包无识别到的头文件、1 个 SDK 聚合根 |
| public / internal / C# SDK 配方文件 | 167 / 55 / 2 | 同一冻结仓实际交付的 rootstrap XML |
| public 配方的去重 name 属性 | 162 | `eo` 出现两次，`capi-appfw-application` 出现五次；不能把文件数称为唯一 API 名数 |

所有候选共 348,449 条包内头文件路径记录；其中 include 树 234,148 条，扣除 SDK 聚合根的 4,967 条副本后，
非聚合提供方计 229,181 条。它们含内核、工具和通用开发材料，是发现上界，**不是平台对应用暴露的函数数量**。
路径被不同 RPM 所有时分别计数，不冒充去重 API 或独立类型数。

[冻结分母](tables/all_binary_records.tsv)、[包版本归并](tables/logical_packages.tsv)、
[完整候选清单](tables/provider_scope.tsv)均可逐条核查。

## 二、怎样判断“对外 API”

本阶段使用分层证据，而不是单个二元标签：

1. **实际交付开发材料**：RPM 文件清单与安装头文件内容相符；无 devel 名称但实际带头文件者也纳入。
2. **公开或内部 SDK 配方关联**：读取冻结的 `app-rootstrap-native-data-public/internal`，保留所选 RPM、
   架构条件和必要路径。public 配方关联到 199 个非聚合头文件提供方包版本组；
   195 个组实际有必要头文件路径交集。二者不是同一个集合。
3. **应用能否直接使用**：公有 API 路径、CAPI 文档标记及配方交集是候选依据；
   最终仍需要目标产品/profile/SDK 的实际开放清单。当前完整应用开放集合为 `NOT_AVAILABLE`。

167 份 public 配方中，163 份有本轮必要头文件路径交集，共 6,062 条去重的“代表 RPM + 路径”拥有关系。
其中 **21 份配方文件、19 个去重 name 属性**含 C++ 声明信号。
这不是“21 个不兼容 API”：头文件可能只有 C++ 便利封装、条件代码，或由应用自身实例化，未必构成跨库对象传递。
配方条件未执行，以上是条件并集的路径匹配，不是当前所有架构的 SDK 构建结果。

没有头文件交集的四份 public 配方是 `ector`、`gcc`、`libgcc`、`libstdc`；
只说明本轮必要路径匹配没有得到头文件，**不说明它们没有 API**。
internal 配方中两条 `gstreamer-plugins-base-devel` RPM 接受项没有映射到冻结包名，保留原文，未猜测替代包。

证据：[逐配方结果](tables/sdk_api_groups.tsv)、[具体路径交集](tables/sdk_header_path_matches.tsv)、
[未匹配包名](tables/sdk_recipe_unmatched_rpm_names.tsv)、[配方摘要核验](tables/sdk_recipe_identity.tsv)。

### 需要人工明确的边界

- public Chromium 配方本身列入 `_internal` 头、Node 和 V8 头/库。
  “在 public 配方中”与“命名为 internal”两项事实均保留，不自行裁决所有这些入口对业务开放。
- Dali/LWE 在本次配方 RPM accept 中没有匹配项，但其 devel 包和头文件实际存在。
  同一匹配方法能检出 Chromium，故有正向对照；零匹配不等于“不对外”。
- 代表性内容分析采用 x86_64 优先；1,070 个代表包中 x86_64 914、noarch 13、aarch64 96、armv7l 47。
  不同发布版本不强行合并。其余架构仅比较文件名集合，未验证内容、宏展开或 ABI 等价。

## 三、五类头文件形态初分

以下为**非聚合 include 头文件提供方**；五类是工作清单分组，不是产品 API 分类标准。
C 栏指有 C 声明形态、未发现本规则 C++ 强特征；**精确的“纯 C”总数为 `NOT_OBSERVED`**，
因为本阶段没有展开全部条件宏和传递 include。C++ 栏是声明信号，不等于真实跨动态库接口。

| 主分组 | 提供方包版本组 | C 声明形态头 | C++ 信号头 | 未定/宏或 include 为主 | 不可读等 |
|---|---:|---:|---:|---:|---:|
| Native/CAPI 候选 | 109 | 402 | 25 | 20 | 0 |
| UI 框架（Dali/NUI/EFL 等） | 51 | 1,177 | 1,176 | 194 | 8 |
| Web 引擎及相关 | 3 | 84 | 239 | 24 | 1 |
| 服务与中间件候选 | 527 | 7,581 | 10,084 | 2,161 | 6 |
| 其他（含通用库、编译工具、内核开发材料） | 270 | 98,528 | 58,723 | 48,694 | 54 |
| 合计 | 960 | 107,772 | 70,247 | 51,093 | 69 |

其中有 C++ 信号的包版本组分别为 10 / 18 / 3 / 154 / 94，共 279；
有 C 声明形态且无 C++ 信号、无不可读 include 头的组分别为 98 / 32 / 0 / 368 / 170，共 668。
其余 13 个组不能落入这两类，不补推为纯 C。

[最终统计表](tables/provider_category_summary.tsv)与[完整逐头记录（无损压缩）](tables/headers.tsv.gz)可回溯。
`tables/category_summary.tsv` 是初版统计，已被最终表取代，不能与本表混用。

### 代表性例子（只说明形态）

| 包/层 | 本阶段直接观察 | 不能外推的结论 |
|---|---|---|
| `capi-appfw-application-devel` | 4 个头中 3 个 C 形态、1 个 C++；`app.h` 有 `extern "C"` 与 `ui_app_main` 声明，`app.hpp` 有 `tizen_appfw::UiAppBase` 类；两者都列入 public 配方必要路径 | 不能按 capi 包名前缀称全部是 C，也未证明便利包装跨库传递 C++ 对象 |
| `dali2-devel` | 121 个头中 119 个 C++ 信号、2 个未定；`public-api/actors/actor.h` 有 namespace/class | 没有据此裁决当前产品是否允许第三方直接用原生 DALi |
| `dali2-toolkit-devel` | 70 个头中 67 个 C++ 信号、3 个未定；`Control` 所在公有路径已抽查 | 不代表已经核过全部标准库对象、所有权或异常边界 |
| NUI 托管层 | 发布清单有 `Tizen.NUI.dll` 等程序集，也有 `libdali2-csharp-binder.so` 等绑定产物 | 只证明存在分层材料，尚未核实际绑定签名或所有调用路径 |
| `lightweight-web-engine-devel` | 3 个头均有 C++ 信号；包括 `LWEWebView.h`、`LWEWorker.h`、`PlatformIntegrationData.h` | 不能把 JavaScript 应用、宿主集成和直接原生调用混为一种边界 |
| `chromium-efl-devel` | 305 个头：79 个 C 形态、207 个 C++ 信号、19 个未定；EWK 与 Node/V8 同包，`ewk_view.h`、`node.h` 已分别抽查 | 不能仅看 EWK 就把整个包判为纯 C，也不能把全部 Node/V8 安装头判为已获产品公开承诺 |
| `tizen-core-devel` | 5 个头均为 C 声明形态；`tizen_core.h` 是代表 | 不是跨库运行通过证明 |
| ICU / 公共便利头 | `uscript.h` 等存在条件 C++ 声明；更广安装集还有 `UnicodeString` 等 C++ 头 | 只按 public 配方与整个安装集分别列范围，不推断所有 C++ 头都是应用必用入口 |

[21 份代表头文件及摘要](tables/representative_headers.tsv)、[对应形态复核](tables/representative_classifications.tsv)。
官方文档也将 NUI 描述为 DALi 之上的 C# 工具包，这仅用于解释为何分层盘点，
不能替代当前绑定源码核查。[官方文档与本地取回记录](WEB_SOURCES.md)。

## 四、建议的第二阶段范围与工作量

建议只先进入下列 **27 个包族**，保留版本差异和公有/内部/绑定层切分：

| 范围块 | 包族数 | 明确的分析对象 |
|---|---:|---|
| Dali/NUI 与托管绑定 | 8 | dali2、dali2-adaptor、dali2-csharp-binder、dali2-extension、dali2-toolkit、dali2-ui-foundation、libscl-ui-nui、csapi-tizenfx |
| Web 层 | 3 | chromium-efl、lightweight-web-engine、webapi-plugins；EWK、Node/V8、插件/脚本、原生集成分别列项 |
| Native 应用框架与服务 | 12 | appcore-agent、bundle、capi-appfw-app-common、capi-appfw-app-control、capi-appfw-application、capi-media-vision、capi-network-smartcard、capi-ui-inputmethod、cion、component-based、libscreen_connector、notification |
| 公开 SDK 中的通用 C++ 条件/便利接口 | 4 | glib2、harfbuzz、icu、vulkan-headers；先区分纯头封装与真正跨库边界 |

取样理由及对应每个二进制包见 [完整建议表](tables/STAGE2_RECOMMENDED.tsv)。
两份 ICU 发布版本分别对应不同架构集合，源码 RPM 记录维持两条；包族数不冒充架构验证次数。
原生候选中非 internal/private 路径的 C++ 信号头计 1,350 条，只用于估计规模，不是 1,350 个不兼容接口。

其余候选的建议分流见 provider_scope.tsv：

- **231 个包版本组**：有非内部候选 C++ 信号，但没有 public 必要路径匹配依据；先取得业务实际应用依赖再确认是否扩大范围。
- **682 个组**：可暂缓对外 C++ 深查；没有相应非内部 C++ 信号或只有未定声明。这不抹去内部 C++ 命中，不表示无条件支持纯 C。
- **108 个组**：本轮没有 include API 深查对象，暂缓；另外一个 SDK 聚合副本单列，不重复统计。
- **6 个组**：先闭合不可读/符号链接等头文件缺口；不是自动跳过。
- **2 个 glibc-devel 版本组**：有 C++ 条件声明；是否另行展开通用 C 库便利头，请人工确认，不默认把它们纳入新的标准库课题。

这些分流与 40 个建议原生候选加总为 1,070；加入托管层是补充 API 层，不改变原生包候选分母。

**工作量粗估：单执行者 32–56 人时，约 4–7 个八小时工作日。**
依据是 27 个包族、上述头文件规模、NUI 绑定链及三个 Web 层的分拆需求；这是规划估计，不是已经测得的工时。
估算按本任务的 API 组归类、签名实例和暴露清单编制，包含交叉核对与汇总；
不包含逐重载运行实测、补建 SDK、全产品/全架构验证或新增包装实现。
若源码/SDK 材料缺失，先列缺口，不能靠估算时间推断结论。

## 五、自检、覆盖限制与自行判断

- 8 个正/负向控制通过，21 份真实代表头文件的摘要与形态重算一致。
  已逐条汇总头文件拥有关系，包级和五类小计与明细一致，见 [VALIDATION.json](VALIDATION.json)。
- 初版把 namespace_id/namespaces 的部分标识符以及 Objective-C @class 误计为 C++；
  已加词界和语言区分，对 65,244 个原 namespace/class 信号头重查并核对字节不变。
  同时把 14 个 arm/jetson 等前缀的内核开发包从粗服务桶归入“其他”。
  前版脚本/汇总、[逐项修正](tables/classification_changes.tsv)与 [重分类摘要](RECLASSIFICATION.json)均保留；最终数字只来自修正后结果。
- 全部常规已读内容与 RPM 文件摘要相符。所有安装路径中的 102 个符号链接未跨 RPM/宿主路径追踪，
  1 个文件含 NUL、未作为文本分类，详见 [缺口表](tables/header_gaps.tsv)。非聚合 include 主表中的对应数量为 69，不能与全安装路径的 103 混用。
- 宏展开、传递 include、生成头文件、其它后缀、C++ 便利包装与真实导出之间的关系尚未全面查明。
  “C 形态”不是纯 C 语义证明；“C++ 信号”不是跨标准库不兼容证明。
- 原生/Dali/Web 的源码取消使用情况是既有背景，本阶段没有重验三架构运行结果，也没有执行任何运行兼容性探针。
- 当前材料不覆盖仓外私有 SDK、第三方组件、产品定制接口和未提供的应用依赖集合。
  没有拿它们的 `NOT_AVAILABLE` 分母去补全“全平台 API 总数”。
- 自行判断限于明示的候选筛选、代表架构选择、工作分组、历史配方路径并集及阶段二范围/工时估计。
  产品公开承诺、接口支持判定和实施方案没有自行裁决。
- 探查缓存位置时 raw/004、007 退出 2，按下载状态中的真实位置重新定位成功；
  首次代表样本猜为 ui_app.h 未命中（raw/023 退出 1），实际文件是 app.h，随后按真实文件清单复查通过。
  原输出未覆盖或改写，没有把不存在的样本写成已核查。

## 六、交接

完整清单、最终五类统计、公开/内部配方映射、建议范围和所有原始命令/退出码均在本目录。
大表 headers.tsv 以 gzip 无损封存，解压后的 SHA256 见 RECLASSIFICATION.json；其它材料摘要见 SHA256SUMS。
脚本快照与 SCRIPT_SHA256SUMS 一并交付。材料提交仅推送 GitHub `codex/runtime-validation`；不推 Gerrit，不改源码或配置。
第一阶段起止 UTC 与耗时以 raw/001_resource.time.json 和封存记录为准，未超过两小时。

**请先审阅范围及 QUESTIONS.md；确认后再进入第二阶段。**
