# 第一阶段范围界定与计数方法

本阶段只建立候选清单与头文件形态分类；不执行第二阶段的逐接口兼容性、异常、所有权或改造难度分析。
所有支持措辞沿用已交付的 [API 报告](../../../HQ_files/api_compatibility_report.md)：
纯 C 只是固定样本通过、并非无条件；只传内置类型/POD 也须核对完整 ABI 和生命周期。
头文件扫描不是跨标准库实测，不能据其结果把任何新接口标为“已支持”。

## 输入范围

使用此前冻结的 Base/Unified `reference/repos/standard` 二进制仓，不把在线最新仓混入统计。
11,332 条二进制 RPM 记录；完整包清单和 repomd/filelists 快照在 inputs/。
repomd SHA256 与原冻结记录一致；filelists 压缩文件摘要逐项与 repomd 内记录相符。
本轮还对选定候选 RPM 重算 SHA256，对所读常规头文件核对 RPM 内文件摘要。
源码及配置均只读，没有安装 RPM、修改系统头文件、重建运行库或运行开发板任务。

## 三层范围，不混为一谈

1. **可见开发材料上界**：RPM 文件列表中含头文件，或包名含 `-devel` / `-headers`。
   不以包名直接判断它是应用 API；无 `-devel` 名称但实际交付头文件者也纳入。
   `*-devel` 可能只有 pkg-config、链接名、构建工具或元数据，单列无头文件项。
2. **公开/内部 SDK 配方证据**：冻结仓的 `app-rootstrap-native-data-public` 与
   `app-rootstrap-native-data-internal` 分别携带 public/internal rootstrap XML。
   将配方中的 RPM `accept`、架构条件、必要路径及 include_path 分别保留。
   配方包归属、配方组名、二进制包名、文件路径是不同层次；不相加。
3. **应用直接可用性**：public 配方的必要头文件路径交集、公有 API 路径或 CAPI 文档标记
   是范围候选依据，不是当前所有产品、profile、权限或 SDK 版本都开放的证明。
   配方带条件且部分列出非当前代表架构；本轮不执行配方，也不推断最终 SDK 的选择。
   缺少目标产品/profile/SDK 的最终公共头文件清单时，完整应用开放集合记 `NOT_AVAILABLE`。

`app-rootstrap-devel` 是聚合开发根，其头文件可能与独立 devel 包重复。
它可以证明分发材料存在，但不能被算成数千个新的接口提供方；提供方小计应与聚合副本分开。
NUI/C#、JavaScript/Web API 不一定有 C/C++ 头文件，另列托管/绑定/脚本层材料，不因头文件零命中而排除。

## 单位和架构

- **RPM 记录**：原仓中的 repo/name/epoch/version/release/arch；计数包括架构变体。
- **包版本组**：按 repo/name/epoch/version/release 归并；不把三架构当三个 API。
- **源码 RPM 记录**：使用 sourcerpm 完整身份，只表示打包来源，不等于 API 组。
- **头文件记录**：一个代表 RPM 内的一个安装路径；同路径出现在另一个 RPM 时仍是另一条拥有关系。
  不是独立类型数、函数数或去重 API 数。
- **SDK 配方组**：一个 rootstrap XML；name 中若含空格，保留原文，不自行拆成多个组。

每包版本组优先读 x86_64，其次 noarch、aarch64、armv7l；同时记录全部实际架构。
比较各架构文件名集合是否相同，但只解析代表 RPM 的头文件内容。
路径集合相同不等于内容或宏展开相同，本阶段不做跨架构语义外推。

## 头文件的快速形态判定

根据发布文件列表发现 `.h/.hh/.hpp/.hxx/.ipp/.tcc/.inc/.inl/.h++`，以及 `/usr/include/` 中的无后缀文件。
目录本身不当头文件；其它安装路径中的同后缀文件保留为外围材料，和 include 树计数分开。
所有 RPM 文件清单保留，未覆盖的其他头文件后缀、生成声明和传递 include 可在下一阶段补查。

分析前屏蔽注释及字符串正文，保留 `extern "C"`。输出三类和不可读项：

| 标签 | 含义 | 不能据此声称 |
|---|---|---|
| `C_DECLARATION_SHAPE` | 有 typedef/struct/enum/union/函数声明形态，未发现所列 C++ 强特征 | 已证明预处理后全为纯 C、或该 API 跨库安全 |
| `CPP_DECLARATION_SIGNAL` | 有 class、namespace、template、std::、using 别名、C++ 专用关键字或访问限定符 | 这些声明必定向应用导出，或必定不兼容 |
| `UNRESOLVED_OR_INCLUDE_MACRO_ONLY` | 仅 include/宏/常量，或当前规则不能判定 | 没有 API、没有 C++、或可以跳过全部传递头文件 |
| `NOT_AVAILABLE` | 符号链接未追踪、内容不可读/非文本等 | 该文件不存在或无风险 |

`extern "C"` 单独作为特征列，而不是“纯 C”的充分条件；同一头文件可以同时含 C 与 C++ 部分。
internal/private/integration-api/devel-api 路径和 public-api/CAPI 文档标记另列，不用名字推断 ABI 安全。
精确的“预处理后纯 C 头文件总数”在第一阶段为 `NOT_OBSERVED`；报告给出的是可复核的形态数。

八个正/负向控制覆盖 C guard、只用 int 的 C++ 类、标准库声明、注释伪命中、C/C++ 混合、伞形 include、
含 namespace_id/namespaces 的 C 声明，以及 Objective-C 的 @class 条件段。
代表性真实包还要人工抽查，不只相信自动规则。

初版缺少 namespace 后的词界，并把 @class 视为 C++ class，自检中分别由 libxml2/Linux 头文件与
SDL_syswm.h 的反例暴露。对应初版规则和数字保留在 initial_classification/；修订后重查原先命中
namespace/class 的全部头文件，核验文件摘要未变，逐单元修改记录见 tables/classification_changes.tsv。
本次推荐与最终数量不使用初版 C++ 计数。精确重分类摘要见 RECLASSIFICATION.json。

## 五个主分组是工作清单，而非互斥的产品 API 分类标准

按先后次序分组：CAPI 包/来源 → Dali/NUI/EFL 等 UI 框架 → Chromium/LWE/WRT/webapi 等 Web 层 →
Unified 其余服务/中间件候选 → Base 通用开发材料、编译工具/内核等其他。
因此“服务与中间件”是待确认的宽候选桶，不表示其中每个包是公开系统服务；
Native SDK 也包含 UI/Web/通用库，不把 CAPI 前缀等同于全部 Native API。
初筛分组规则在 scan_scope.py 的 category()；reclassify.py 另按完整包名把 arm/jetson 等前缀的内核开发包
归入“其他”，逐包修正见 tables/category_corrections.tsv。最终主表为 tables/provider_category_summary.tsv，
不是初筛 tables/category_summary.tsv。SDK 配方归属作为独立维度保留，优先用于缩小第二阶段范围。

## 本阶段不做的事

不构建/运行兼容性探针，不逐函数判定异常传播或所有权，不实施 C 包装方案。
不从 C# 程序集与 binder 的存在直接推断所有应用调用都经 C ABI；绑定链的具体签名核查留待第二阶段确认后开展。
不把纯 C 形态作为豁免生命周期、回调、异常/取消和目标 ABI 核验的理由。
同样，头文件有 C++ 便利封装，也不证明它跨动态库传递 C++ 对象：纯头实现与真正导出边界仍须区分。
