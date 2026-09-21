# 按架构的跨包面与标准库新发现

待人工审阅。本文只汇编既有记录，不增加源码分析、探针、板上运行或兼容性承诺。表中的“确认”必须连同证据种类和架构一起阅读。

## 一、收账结论与计数单位

原先按候选登记的 **44 项**，对应 **16 个有向源码包对、21 条符号边**；随后追加不同消费方的 WaitUntil 与 Reader，现有 x86_64 登记为 **18 包对、23 符号边**。候选、包对、符号边不能相加：同一符号边可包含多个候选，一个包对可包含多个符号。完整原候选映射见 [候选到边](CANDIDATE_MAP.md) / [TSV](CANDIDATE_MAP.tsv)。{{base-map}} {{edges}}

追加记录使用了 `steady_clock::rep`、`streamoff` 等表达式标签，不是重新审定了一批候选；“44 → 18/23”是两个阶段的登记衔接，不能读成“44 个互不重复类型直接产生 23 个独立缺陷”。WaitUntil 的不同消费方分别成边，Reader 保留为对象边而不计作直接内建类型符号冲突。{{extra-proofs}} {{arm-final}}

| 架构 | 已确认的包间引用 | 证据覆盖 | 不能声称什么 |
| --- | --- | --- | --- |
| x86_64 | 18 包对 / 23 符号边 | 冻结的 3,085 个 ELF，选自该架构 10,946 个运行 ELF，约 28% | 不是平台全集，不是 23 项混合运行失败 |
| armv7l | 定点核查的 2 包对 / 2 符号边：ui-foundation → dali2、zypper → libzypp | 真实 ARM 消费方 UND、提供方定义及 NEEDED/SONAME 对应 | 其余边未复核；完整 ARM 分母为 NOT_OBSERVED，不能套用 16/21 或 18/23 |
| aarch64 | 包间引用全集及本表各边：NOT_OBSERVED | 只完成下文指定类型测量 | 不把类型测量当作包间 ELF 引用或运行实证 |

以上分母及架构事实分别回链：{{scope}} {{arm-elf}} {{arm-count}} {{arm-final}}。ARM 两条证据没有来自 x86_64 的继承；原始符号条目、ELF 路径和完整 SHA 在 [逐边附表](EDGES_zh.md)。

## 二、完整跨包面

本表是公开声明、对象存储链与真实已发布 GNU ELF 引用的**静态核查结果**，不是混合标准库业务运行矩阵。仅调用标准库自身的运行库引用不计入本表。测试产物边仍登记，但不据此声称第三方应用实际直接调用。形态与所有权来源为逐候选原证据；未核实的留存、复制及释放责任保持未定。{{base-map}} {{edges}}

{{edge_table}}

机器可读完整表：[EDGES.tsv](EDGES.tsv)。每行保留 x86_64 消费方/提供方路径、SHA、符号原文；有 ARM 证据的行另列 ARM 路径、SHA 和 UND/DEF 原文。`NOT_OBSERVED` 不是“不存在”。

### Reader 的归类修正

**事实链（静态核查）**：真实调用为 `Reader(InputStream const&, Validate const&)`；`InputStream` 的 `_size` 是 `DefaultIntegral<std::streamoff,-1>`，后者保存实际数值成员；该类还含 `_name` 字符串、`_stream` 等成员。构造函数名编码的是 `InputStream` 与 `Validate` 类名，不包含 `_size` 的内部类型。原头文件摘录 `InputStream.h` 的成员区、`DefaultIntegral.h` 及 ELF 记录均已保存。{{structure}} {{extra-proofs}}

**已排除的解释**：不能把该 Reader 符号当作 `streamoff` 直接参数的 `l/x` 冲突。真实 ARM 两侧头文件交叉编译得到相同的 Reader 引用名，GNU 包间真实引用也存在；这既不删除对象边，也不证明其内部对象可跨库解释或释放。{{arm-signatures}} {{arm-elf}}

**反驳需要什么**：若能够推翻这组冻结头/ELF 的对应或证明实际跨越的不是所列 `InputStream` 对象，需要修正该边；若要证明跨库兼容，则仍需匹配 libc++ 提供方上的内容、状态和销毁测试，仅有符号相同不够。现归类为“跨包 C++ 对象（引用及成员状态）”，计数不变。

## 三、可纳入对外报告的新发现

### 同名高分辨率时钟并不代表同一种时间

**事实链**：已测 x86_64 配置下，`high_resolution_clock` 在 libstdc++ 展开为 `system_clock`，在 libc++ 展开为 `steady_clock`；两侧均实测为空类型，大小均为 1 字节。保存的定义分别指向 GNU `bits/chrono.h` 的别名及 libc++ `__chrono/high_resolution_clock.h` 的条件分支。后者在启用单调时钟时选 steady_clock，关闭该配置时可选 system_clock；本文不是无条件描述所有 libc++ 配置。{{clock}} {{clock-gnu}} {{clock-cxx}}

**静态语义证据**：GNU 保存的 `system_clock` 定义说明它代表系统墙上时间，`is_steady=false`；`steady_clock` 说明时间以均匀速率单调增长，`is_steady=true`。空的时钟类型本身没有非静态数据成员，这排除了“时钟标签里存储布局不同”作为本项解释，**没有排除时间含义不同，也没有覆盖 time_point 的状态**。平台安装头中已有 Boost 的别名导入、LLDB 的 StatsClock/StatsTimepoint、LLD 的 startTime 成员样本；这些样本本身不新增真实消费边。{{clock-gnu}} {{clock-cxx}} {{clock}}

**与已确认跨包面的关系**：WaitUntil 传递 time_point 的事实已经有公开签名、内部 duration/rep 存储链及 ELF 引用；但它的头明确写的是 **steady_clock，不是 high_resolution_clock**。不能把“time_point 确实跨包”和“高分辨率时钟两侧别名不同”拼成“WaitUntil 已实测静默错值”。{{structure}} {{edges}}

**具体错值场景（推断，未实测）**：若一个 GNU 组件把高分辨率时钟的“自起点以来时间”统一为纳秒整数交出，另一 libc++ 组件未经时钟域/起点换算，将它重建为自己 high_resolution_clock 的截止时间，那么接收侧会把墙上时间数值解释成单调时钟时间。整数传递和读取可以成功，值却不是同一截止时刻，可能导致过早超时或错误的长等待。这是明确了“按整数传递、单位已统一、未换算起点”的条件性反例，不是某条已发布调用链的运行发现；本轮没有测得该场景的错值或时间差。

**已排除的解释**：不是用大小不同解释这项语义风险；不是把所有 chrono 类型视为同一时钟；也没有声称混合 C++ 符号总能链接通过——直接传递不同类型时，链接可能先拒绝。

**反驳需要什么**：若实际两侧构建选中相同的时钟，或边界保留并校验时钟域、起点且完成换算，或并不存在这种时间重解释，该场景就不成立。要把它提升为产品缺陷，需要真实消费路径和可核对的截止时间实测；当前保持“别名与空类型已测，错值场景为推断”。

### 同宽整数仍可能形成不同的链接名称

**事实链（实测）**：指定的 **13 项表达式**在 x86_64 和 aarch64 均为 GNU `long` / libc++ `long long`，实际类型编码为 `l` / `x`，两侧均 8 字节；armv7l 则两侧均为 `long long`、编码 `x`、8 字节。表中给的是类型表达式数量，不是独立包边或独立故障数量。两库均测，既有记录逐格五轮一致并有类型编码正向对照。{{types-x86}} {{types-arm}} {{types-aarch64}} {{arm-final}}

{{type_table}}

完整来源与 SHA 列见 [BUILTIN_TYPES.tsv](BUILTIN_TYPES.tsv)。x86_64 为原生探针；armv7l 与 aarch64 为交叉编译后 QEMU 用户态打印。类型由目标头、宏及编译配置决定，QEMU 不重新选择类型；这些结果不证明板上业务运行正确。两侧采用 GNU 14 / libc++ 22.1.8 的已记录配置。{{arm-config}} {{aarch64-config}} {{arm-final}}

**已查到的公开暴露（静态，不把声明当成消费边）**：

| 提供包及头 | 公开声明/结构 | 与本项的关系及消费证据 |
| --- | --- | --- |
| dali2，conditional-wait.h | `using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;`；`void WaitUntil(const ScopedLock& scope, TimePoint timePoint);` | time_point → duration → rep；toolkit 与 ui-foundation 消费方各有 x86_64 边，ARM 仅定点核查 ui-foundation |
| podofo，PdfInputDevice.h | `virtual std::streamoff Tell() const;`；`virtual void Seek(std::streamoff off, std::ios_base::seekdir dir = std::ios_base::beg);`；`virtual std::streamoff Read(char* pBuffer, std::streamsize lLen);` | Seek 的参数直接涉及底层整数；Tell/Read 的返回类型通常不进普通函数修饰名，不能一概称链接必失败。Tell/Seek 既有身份查询有定义但未取得异包 UND；Read 未完成该查询；虚调用未排除 |
| abseil-cpp，absl/time/time.h | `FromChrono(const std::chrono::nanoseconds& d)`，及 microseconds / milliseconds / seconds 重载；另有 system_clock::time_point 重载 | 对应 duration/clock 的 rep；已有身份查询未取得异包 UND，内联/模板途径未排除，不新增边 |
| libzypp，InputStream.h / Reader.h | `std::streamoff size() const`；`void setSize(std::streamoff val_r)`；Reader 借用 InputStream | size/setSize 是已保存的内联成员；Reader 的符号只含外层类名，内部成员类型分歧不构成 Reader 直接符号冲突；对象边仍存在 |

公开声明、准确源码位置和源码 SHA：{{declarations}} {{structure}}；实际引用：{{extra-proofs}} {{arm-elf}}。上述限定阴性沿用有 `bundle::Add` 正向对照的身份查询，不作平台级无消费者断言。{{boundary-control}} 对其余 char_traits/off_type、istreambuf_iterator::difference_type 等表达式，本文没有可追加的已闭合独立消费边。

**WaitUntil 的两重机制**：x86_64 的 duration 数值类型选择不同，是本次新增的内建 `l/x` 因素；此外两套库的 chrono 内联命名空间及 clock 类型身份也不同，是既有因素。armv7l 的前者消失，但真实头文件生成的两侧函数符号仍不同，后者仍在。aarch64 已测的是上述类型，未测 WaitUntil 业务引用或函数符号，不能借 LP64 补出该证据。{{extra-proofs}} {{arm-signatures}} {{arm-final}}

**已排除的解释**：同为 8 字节并不使 long 与 long long 成为同一 C++ 类型；这不是本项的“4 对 8 字节宽度变化”，也不是漂亮打印拼写不同造成的计数。比较依据是实际 TypeToken 修饰名，直接类型编码随记录保留。函数返回类型及外层对象成员未必进入函数名，因此不把所有出现位置说成链接失败。

**反驳需要什么**：若指定目标配置下两侧实际类型编码相同，则该配置的内建分歧不成立——armv7l 的测量正是这个反例。对某接口的链接后果，还需确认类型确实进入其符号名以及实际消费者；若无该参数/模板实参、存在明确兼容入口或根本不跨库，不能用本项推定失败。此处没有完成真实 libc++ 提供方上的逐边运行验证。

**可直接引用的一段话**：一些标准库类型名在两套实现中对应不同的整数类型。在已测 x86_64 与 aarch64 配置下，这些整数占用的字节数相同，但编译器在函数名称中区分 long 和 long long，因此使用它们的参数或模板类型可能使调用方找不到提供方的函数。这与“内存布局不同、运行时把对象读错”不是同一种问题；返回类型或对象内部成员也不一定改变函数名。已测 armv7l 配置中，这类整数选择差异不存在，不能将另一架构的问题直接移植过去。

## 四、挂账：状态和重启条件

| 工作项 | 状态 | 重启条件 |
| --- | --- | --- |
| 62 项函数身份/材料缺口 | 暂停，C 主因 23 项、D 主因 39 项；不是平台级阴性 | 逐项见 [缺口与重启条件](BACKLOG_zh.md) / [完整 TSV](BACKLOG.tsv)，保留本线可解性与依赖 |
| 65 项编译期设施的结果投影 | 0/65 完整关闭；15,793 处 RESULT_UNRESOLVED；已停 | 人工重新授权投入；先补实际模板实参、实例化上下文、名字查找或结果结构证据，不能用手选 int 样本关项 |
| 第 201–495 个候选 | 未定界，295 项 | 人工启动下一批定界并确认范围/方法；本次不继续 |
| 其余 572 个表达式的 armv7l 复测 | 不测，状态仍未测 | 只有人工更改范围并给出产品相关问题时重启，不因本次 13 项一致就判它们安全 |

计数来源：{{pending}} {{projection}} {{projection-count}} {{base-count}} {{measurement}} {{mechanisms}}。不同表的候选、命中位置及表达式不是同一单位，不相加。

**投影计数的最新口径**：结果位置分别为无对象 15,770、类型分歧 10、标准库对象 13、未定 15,793；“13 个对象结果位置”不等于上节“13 个内建类型表达式”。发现对象结果也不等于已确认新消费边；完整候选关闭仍为 0/65。此前少掉“两侧同一类型”的断言已在新产物纠正，旧产物保留。{{projection-count}} {{projection}}

**不测决定的理由需纠正**：剩余 572 项中，按既有 x86_64 结果分解为相同 283、仅命名空间/标签差异 114、实现类/组合类型差异 111、枚举相关差异 4、不可得 60。已知命名机制不作为新发现重复投入，是保留不测决定的背景；但不能将其余所有项改写成“全为已知实现差异”。相同项不等于 ARM 同样相同，不可得项仍缺结果。本文保留“不测”的人工范围裁决，不替剩余项补结论。{{measurement}} {{mechanisms}}

## 五、下一阶段逐边输入与前置

以下是**规划估计，不是新增实测或优先级排序**。每行都需要该行指定的 **libc++ 构建提供包**及匹配的公开头、依赖库、构建选项和来源 SHA；并保留 GNU 提供方用于同侧控制与反方向。当前哪些新版包已生成，本次没有重新盘点，状态为“未复核”，不能把所需清单当现货清单。

沿用既有三类型验证模板：先通过双方同侧控制，再测混合方向；真实提供方，记录实际加载库；断言具体值、失败状态与适用的销毁责任，每格五轮。链接不通过也是结果，不用强制改名或强转函数指针伪造自然可链接。转移所有权的检查不能退化为单看退出码。{{template}}

**架构安排的边界**：本表所有边均可在 x86_64 提供方及初始化夹具就绪后先开始验证；这不承诺当前所有服务均能在无图形环境运行。对于 armv7l 产品结论，每条边都需要其自身 ARM 版本的验证，计划沿用物理板验收；先补未核查边的 ARM 实际引用。仅凭当前材料，不能分出“技术上非物理板绝不能测”的确定子集，标为 NOT_OBSERVED。渲染、WebView、推理及服务类边的实际初始化/设备依赖在各行列为前置；若只能在板上满足，届时需板，不在本轮替其决定。已查到 ARM 引用的两条也没有运行通过豁免。

**量级的适用范围**：表内 h / working days 为单边、单架构、提供包和环境齐备后的夹具与验证/记录量级；不含提供包重建、依赖修复、环境搭建等待及排板时间。分为简单数据调用、状态/资源调用、服务/回调初始化较重的调用，是编辑规划判断，不是历史工时测量。旧三类型时间记录均标 `complete_scope=false`，不足以校准完整逐边成本，因此不给总工时。{{timings}} {{template}}

{{next_table}}

详细输入：[NEXT_STAGE.tsv](NEXT_STAGE.tsv)。每行的提供方 ELF 目标、来源证据与 SHA 均可核对。规划不是处置选择，也不声称相同提供包下不同符号可互相代替验证。

## 六、未覆盖部分与自行判断

未覆盖：冻结 ELF 之外的产物、静态链接、宏生成与条件编译、生成头、模板实例化、别名后的无限定名、dlopen/间接调用、未扫描架构、真实混合库业务调用的逐边内容/销毁/异常/并发验证。公开头和测试 ELF 的存在不等同 SDK 面向第三方应用的可用性。范围沿用既有具名冻结安装头/ELF 集合和 x86_64 结构测量集合；本次没有扩展任何技术测量范围，只把已存 ARM 定点证据并列列出。{{scope}} {{arm-final}}

自行判断仅限：将计数分成候选/包对/符号边和架构；按已审阅依据重述 Reader 的类别；把时间错值场景明确列为条件性推断；给逐边规划量级；纠正“不测 572 项”的构成说明。未重判 62 项或推进 65 项关闭，未重跑内建别名/空类型测量。待人工确认的只有这些编辑表述及下一阶段启动条件，不在此替业务作兼容策略决定。

自检见 [SELF_CHECK.md](SELF_CHECK.md)，证据路径和完整 SHA 见 [EVIDENCE.md](EVIDENCE.md)，逐输入完整摘要见 [INPUT_SHA256.json](INPUT_SHA256.json)。事实链、候选解释和反驳条件已分开；没有把“没有崩溃”写成“没有问题”。
