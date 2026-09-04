# 运行时验证线交接说明

## 1. 交接目的与基线

本分支 `codex/runtime-validation` 是运行时问题验证的独立工作线，创建于
`codex/r5-ehabi-diagnosis` 的提交
`e7790fc87f1cfcbd3620e51867263a16ce46fad5`。它用于让一个不了解既有调查过程的
新会话直接接手运行时问题，避免与仍在推进的包编译适配互相覆盖。

本文中的“已证实”只指已有原始命令、输出或源码证据支持的结论；“推断”不会
写成实测；未执行的格使用 `NOT_OBSERVED`，所需材料无法取得时使用
`NOT_AVAILABLE`。

## 2. 本工作线的边界

### 2.1 负责

- `noexcept` 五处边界的方案裁决、实现和运行时验证；
- libc++ 与 libstdc++ 混合运行时边界验证，包括跨界对象布局、异常、所有权和
  析构；
- 两套运行时同进程共存场景；
- `pthread_cancel`，尤其异步取消，对产品和运行库的影响；
- aarch64 尚缺的运行时覆盖；
- 已登记的其他运行时、ABI 与行为差异的复核。

### 2.2 不负责

- 平台包的编译适配、spec 修改和包级 `-stdlib=libc++` 接入；
- MLGO/XLA AOT 预编译资产的重建或接口改造；
- Unified 全量编译失败的定位与修复；
- `sandbox/lhmax2025/libcxx-migration` 上各包的提交；
- LLVM 打包、project_config、QuickBuild 发布流程本身。

如运行时验证发现编译线问题，只记录证据并交给编译适配线，不在本分支顺手
修改 spec、配置或 AOT 资产。

## 3. 已完成的运行时工作

### 3.1 线程取消兼容补丁与全量回归

定稿方案由四个独立提交构成，必须作为一套理解：

1. 层 A：外来异常重抛改用 `_Unwind_Resume_or_Rethrow`；ARM EHABI personality
   以 `results.ttypeIndex != 0` 识别 catching handler。这两处必须同时存在。
2. 层 B：定义 `__cxxabiv1::__forced_unwind`，personality 在强制展开时映射为
   该类型。
3. 层 C 头文件批：20 处强制展开防护。
4. 层 C 库内批：30 处库实现防护（14 处直接，另 16 处经 ios helper）。

依赖关系是：层 C 依赖层 B；armv7l 上层 C 还依赖层 A。不能把单个提交当成
独立修复。干净基线再生、补丁 SHA256、三树逐文件一致性和后续修复的材料在
`docs/progress/R69/`；sandbox 提交材料在 `docs/progress/R76/`。

验证过的源码锚点是 sandbox 代码提交
`08a64b6eb95ed74339f1d862a6c22a2f93fa78f9`。R88 独立确认总部 squash
`e79f1c3dde12c356142b77231e304c0da67adaf1` 与该锚点的完整 Git tree 相同，
tree ID 都是 `e35b4a41...`，13 个涉及文件逐字节一致，11 项关键修复均在；见
`docs/progress/R88/`。后续 sandbox 的 packaging 元数据/spec 提交不改变上述
运行时代码结论，不能把新的分支 HEAD 误写成当时运行时测试的代码身份。

官方测试套件已完成所选 x86_64 与 armv7l 配置的基线/补丁全量对照。armv7l
最终结果是 libc++ 11,321/11,321、libc++abi 81/81；两侧逐项状态一致，213 项
共同失败，`SCHEME_INTRODUCED=0`、`INDETERMINATE=0`。结合此前 x86_64 全量
对照，两架构均没有发现补丁引入的失败。armv7l 曾因 root/owner 身份漂移造成
28 项伪差异，R81 统一为更接近产品的 owner 身份并隔离临时目录后全部收口。
完整结果、逐项分类和夹具隔离证据在 `docs/progress/R77/` 与
`docs/progress/R81/`。

重要限制：官方套件没有精确触发下面 D01--D05 的 `pthread_cancel` 穿越；普通
condition_variable/future/stream 测试通过不能替代取消边界验证。

### 3.2 `noexcept` 五处边界

已确认五处构成 libc++ 与 libstdc++ 实际差异且均可达：

| 编号 | 站点 | 形态 | 已知动态状态 |
|---|---|---|---|
| D01 | `condition_variable::wait` | 库外联定义 | libc++ 取消终止；libstdc++ 干净取消 |
| D02 | `condition_variable::__do_timed_wait(system_clock)` | 库外联定义 | 精确动态触发 `NOT_OBSERVED` |
| D03 | `condition_variable::__do_timed_wait(steady_clock)` | 头文件内联 | libc++ 取消终止；libstdc++ 干净取消 |
| D04 | `condition_variable::__do_timed_wait<Clock>` | 头文件模板 | libc++ 取消终止；libstdc++ 干净取消 |
| D05 | `wbuffer_convert::~wbuffer_convert` | 头文件模板类析构 | 精确动态触发 `NOT_OBSERVED` |

枚举、可达设施、抽样调用栈在 `docs/progress/R72/`；产品影响与业界处理调查在
`docs/progress/R73/`；事实表、路线矩阵和验证探针在 `docs/progress/R86/`。

接手决策时不能沿用“这些函数本来就不会抛”的假设：

- D01--D03 的实现包含生成 `system_error` 的错误路径，pthread wait 也是取消点；
- D04 会调用可能抛出的自定义 `Clock::now()`/算术，并进入 D02/D03；
- D05 的 `__close -> sync` 会调用用户可覆盖的 codecvt/streambuf 虚函数，可能抛
  普通异常或进入取消点；
- D05 没有一处可直接删除的显式 `noexcept`。它因非抛虚基类析构而隐式
  non-throwing；探针已经证明派生析构不能单独声明 `noexcept(false)`，编译器
  会报更宽松异常规格。

ABI 事实也已查清：函数自身顶层 `noexcept` 在当前 Itanium ABI 下不进入
D01/D02 等函数的符号名，但 C++17 起它属于函数类型。旧调用点可能因为原声明
为 non-throwing 而没有生成清理 landing pad/LSDA，所以“符号仍能解析”不等于
“旧二进制可安全展开”。D01/D02 是版本为 `@@LLVM_22` 的动态符号，可技术性
保留新旧版本；D03/D04/D05 是头文件隐藏实现，库端符号版本不能替换已经编入
消费对象的旧代码。

R86 已评估 A--J 十类路线（A、B 各含子型），没有作推荐或排序：

- A1：直接解除 D01--D04 异常规格，D05 另案；
- A2：D01/D02 双版本，D03/D04 头文件解除并重编，D05 另案；
- B1：让 future 不再经 condition_variable；
- B2：condition_variable 改用不产生取消展开的内部等待原语；
- C：A2、头文件重编和 D05 专门方案的组合；
- D：产品侧改为 cooperative stop；
- E：在边界附近屏蔽取消；
- F：在 `noexcept` 内识别 forced unwind；
- G/H/I：分别为析构不同步、显式 close/sync 协议、放宽 streambuf 析构层次；
- J：接受并记录差异。

其中以下方向已有明确排除或严格限制，勿从零重复评估：

- 产品侧 cooperative stop 可以降低已知调用者暴露，但不修复标准库站点，也
  覆盖不了未知、第三方或闭源调用者；因此已排除为“五站点完整修复”，而不是
  否认它可作为单个产品的缓解措施。
- 在 `noexcept` 内捕获 forced unwind 后重抛仍会穿越该边界并终止；吞掉它会
  破坏 NPTL 强制展开协议。路线 F 在语义上不可行。
- 边界附近关闭取消不能形成局部完整解：恢复时可能立即投递 pending cancel，
  不恢复又改变调用者线程状态；异步取消还可能命中别处。
- 只改 future 的结构不能解决直接 condition_variable 使用或 D05；还会触及
  shared-state ABI、时序、性能和多平台 fallback。
- D05 析构不再 sync 会改变无取消路径，可能丢失尾部编码状态或缓冲数据；放宽
  `basic_streambuf` 基类析构则影响整个派生生态和 ABI。

当前卡点不是缺材料，而是需要人工决定 ABI/兼容策略及 D05 的正常语义取舍。
接手者应从 `docs/progress/R86/FACTS.md`、`ROUTES.md`、
`tables/site_facts.tsv` 和 `tables/route_matrix.tsv` 开始，而不是重新枚举方案。

### 3.3 函数指针、类型参数与 traits 兼容风险扫描

R87 针对 D01--D05 检索了取函数地址、成员函数指针、函数类型作为模板参数或
函数参数、`decltype`/traits/`noexcept(...)` 等会把异常规格带入类型或实例名的
形态。已覆盖语料中没有相关命中：371 个已解包源码包、11,332 个已发布包、
39,166 条 ELF 记录、8,911 个静态归档、约 494 万个 mangled 符号。准确边界是
39,166 条 ELF 中有 1 条扩展 section-count 文件未解析，来自
`u-boot-rubikpi3`，与目标 140 包补扫集合无关。

零命中已由 R90 的阳性校准支撑：构造的 ELF/共享库、静态归档和源码三类载体
共 42 个预期阳性，先用独立 mangled/demangle 手段确认，再原样运行 R87 三个
扫描器，结果 42/42 命中、0 漏检。校准只证明扫描器能找出已覆盖语料中的目标
形态，不扩大语料范围。见 `docs/progress/R87/` 与 `docs/progress/R90/`。

R92/R93 对源码盲区作了补扫与材料收口：最终 136 包完整、2 包因 9 个悬空
符号链接目标而部分覆盖、2 包源码不可得，状态仍是 `PARTIAL`。因此对外表述应
是“已覆盖语料零命中”，不能写成全平台绝对不存在。见
`docs/progress/R92/`、`docs/progress/R93/`。

### 3.4 产品异步取消使用

R79 的静态检索确认 3 个生产源码包直接设置异步取消：

- `lightweight-web-engine`：确认存在 async cancel + C++ frame/RAII + 取消点的
  完整风险形态；
- LLVM OpenMP runtime (`libomp`)：确认存在完整风险形态；
- Mesa Vulkan WSI：命中发生在 C 线程路径，未确认 C++ EH/RAII 风险形态。

该结论是静态证据，不是现场运行观察；间接包装、运行时动态选择和未取得源码
仍是盲区。完整调用链、文件行号和检索分母在 `docs/progress/R79/`。

### 3.5 跨运行时对象布局与 Boost 边界

R80 已证明同名 C++ 边界可在链接和装载阶段完全匹配，却因 libc++/libstdc++
对象布局不同静默错读。代表性 `std::deque<int>` 用例中，两侧对象大小分别为
48 与 80；provider 报告 4 个元素，consumer 可读出巨大错误长度，而链接器不
报错。33 个代表类型中有 10 个在 x86_64 的大小或对齐不同。见
`docs/progress/R80/`。

R82 对平台真实 Boost 边界分类：90 条 C++ 符号边中，63 条已由公开签名或
Boost 类型定义证明传递标准库布局敏感状态，27 条仅能标为
`NO_STD_LAYOUT_EVIDENCE`，不能据此宣称安全；涉及 13 个 consumer 源码包、75
个架构化 ELF，其中 6/13 包已确认风险形态。

已完成的代表性动态结果：

- filesystem：x86_64 与 armv7l/QEMU 各 5/5 静默数据错误，consumer 异侧销毁
  路径完成但未证明无潜在堆损坏；
- iostreams：x86_64 5/5 退出 134，armv7l/QEMU 5/5 退出 139；
- program_options：x86_64 5/5 退出 139；armv7l 为 `NOT_OBSERVED`。

这支持“同进程共存可以成立，但跨界传递 C++ 语义不安全”。尚未逐条动态执行
90 条边，27 条模糊边没有运行期安全证明，armv7l 物理板、program_options 的
armv7l 格、aarch64 和销毁路径的内存检查都未完成。R93 还修正了 R82 的脚本
归档问题：armv7l 当次实际只执行两个用例，当前三用例脚本并非当次版本；只能
提供执行等价重建，不能声称逐字节恢复。见 `docs/progress/R82/` 与
`docs/progress/R93/`。

### 3.6 两套运行时共存场景

R78 完成了三种受控形态，并用行为断言而非仅退出码判定：

1. 纯 C ABI：x86_64/armv7l 均通过；armv7l 真实 ICU DSO 直接依赖
   libstdc++，libc++ 主程序经 `u_getVersion` 成功，进程中四个相关 DSO 同时
   加载。
2. 跨运行时线程取消：取消点位于 libstdc++ 插件，forced unwind 穿过旧插件
   与 libc++ 主程序；两架构的两侧析构各执行一次，join 返回
   `PTHREAD_CANCELED`。
3. 普通类型化 C++ 异常跨界：两架构都不能按共享源码中的
   `BoundaryError` 类型捕获，只进入 `catch(...)` 并退出 12。

结论是：C API 共存有实证；本方案专门支持的 POSIX forced unwind 可在该受控
栈正确完成；普通 C++ 异常、对象、allocator、RTTI 或所有权不能由此推定安全。

`pen-wave` 仍未完成：板上真实 DSO 与源包 hash 一致，但镜像缺少其
`DT_NEEDED libGLESv2.so.2`，`dlopen` 退出 3，真实 API 没有进入；x86_64 资产
不可得。需要带完整 GLES 依赖闭包的目标镜像再测，不能用 ICU 结果替代。见
`docs/progress/R78/`。

### 3.7 其他已经厘清的运行时差异

- `shrink_to_fit` 最终用例见 `docs/progress/R75/`。x86_64 两库及 armv7l
  libc++ 都在 `shrink_to_fit` 内部进入主动终止（退出 134）；armv7l
  libstdc++ 的 139 是 verbose terminate handler 已开始执行后的二次段错误，
  上游触发仍是同一 `shrink_to_fit` noexcept/terminate 机制。报告不能把 139
  直接称为与 134 相同的进程终止方式。
- 已知 25 类差异的官方套件触及情况在
  `docs/progress/R81/tables/known_25_coverage.tsv`；大量符号、ABI、部署和共存
  差异不由单运行时官方套件直接覆盖。

## 4. 当前未完成项与继续条件

| 项目 | 当前状态 | 卡点/所需材料 |
|---|---|---|
| `noexcept` 五站点方案 | 材料齐备，未裁决 | 人工选择 ABI 兼容策略、D05 正常语义取舍和支持的旧对象范围；选择后再设计补丁与验证矩阵 |
| Boost 90 条边 | `PARTIAL` | 只动态测代表性三类；需补其余边或明确抽样接受标准，补 armv7l program_options、物理板及内存安全检查 |
| `pen-wave` | `PARTIAL` | 需要含 `libGLESv2.so.2` 及完整依赖闭包的产品镜像；x86_64 资产当前 `NOT_AVAILABLE` |
| aarch64 运行时覆盖 | `NOT_OBSERVED` | 需要 aarch64 构建产物、sysroot/目标板或可信执行环境；线程取消、五边界、共存、跨布局均未跑 |
| D02/D05 精确取消触发 | `NOT_OBSERVED` | 需要最小可达用例与两库/多架构动态矩阵 |
| R92 源码补扫尾部 | `PARTIAL` | bcc-tools/chromium-efl 的 9 个悬空链接目标，另有 2 包源码不可得 |
| 混合对象销毁的隐性损坏 | 未证明安全 | filesystem 虽返回，尚缺 ASan/Valgrind 或等价内存检查；不要把“未立即崩溃”写成安全 |

## 5. 关键决策原则及由来

### 5.1 先解决库问题，不把产品改法当完整答案

已知产品改成 cooperative cancellation 可以降低暴露，但第三方、闭源和未知
调用者仍能触发标准库边界。故产品侧改法只能作为产品级缓解，不能取代五站点
的库级裁决。

### 5.2 不能在 `noexcept` 内“识别后继续”

forced unwind 被识别后若继续重抛，仍要穿过 non-throwing 边界，语言运行时会
终止；若吞掉则破坏 pthread/NPTL 的强制展开协议。现有四补丁中的
`__forced_unwind` 识别用于可传播的 catch 防护，不能把同一技巧机械搬进
`noexcept` 函数并期待绕过异常规格。

### 5.3 当前裁决不受未来上游提交形态约束

R86 的明确任务定位是“先把问题解决好”。是否拆分、怎样满足 LLVM 上游政策、
上游是否接受属于后续交付问题，不应倒过来排除当前技术上正确的方案。仍需遵守
C++/ABI/平台兼容事实，不能把“不考虑提交形态”理解为“不考虑兼容性”。

### 5.4 换实现不换行为

目标是消除线程取消强制展开导致的异常终止，同时尽量保持无取消路径的可观察
语义。任何改动都必须分别验证：成功等待、通知、超时、错误传播、缓冲刷新、
析构、资源回收、时序与 ABI。仅为了让取消通过而删除 sync、改变错误从终止到
传播、改变 shared-state 调度或丢失输出，都属于需要人工明确接受的行为变化，
不能默认归为“内部实现调整”。

## 6. 材料路径索引

| 主题 | 主要材料 |
|---|---|
| 四补丁身份、干净再生、三树一致性 | `docs/progress/R69/` |
| sandbox 提交、依赖顺序、QuickBuild | `docs/progress/R76/` |
| armv7l 官方套件初跑与断点 | `docs/progress/R77/` |
| armv7l 夹具隔离、补跑、完整分类 | `docs/progress/R81/` |
| sandbox 与总部 squash 逐文件/树一致性 | `docs/progress/R88/` |
| `noexcept` 站点枚举与动态样本 | `docs/progress/R72/` |
| 五站点影响面与业界先例 | `docs/progress/R73/` |
| 五站点事实、ABI 探针、A--J 路线 | `docs/progress/R86/` |
| 函数指针/类型/traits 扫描 | `docs/progress/R87/` |
| 扫描器 42/42 阳性校准 | `docs/progress/R90/` |
| 未扫源码包补扫及收口 | `docs/progress/R92/`、`docs/progress/R93/` |
| 异步取消产品使用 | `docs/progress/R79/` |
| 跨库符号与对象布局最小实证 | `docs/progress/R80/` |
| Boost 90 边分类与代表性实测 | `docs/progress/R82/` |
| 三类共存场景与 `pen-wave` 缺口 | `docs/progress/R78/` |
| `shrink_to_fit` 调用栈与 armv7l 139 区分 | `docs/progress/R75/` |
| 已知差异系统材料 | `docs/progress/R66/`、`docs/progress/R81/tables/known_25_coverage.tsv` |

各目录中的 `REPORT.md` 是入口；最终事实应继续回到对应 `raw/`、`commands/`、
`tables/` 和代码快照核实，不能只引用摘要。

## 7. 不得触碰的并行工作

- 不要在本分支修改 11 个 Base 包的
  `sandbox/lhmax2025/libcxx-migration` 分支；已通过、阻塞或待验证状态属于
  R104/R105 编译适配线。
- 不要修改 `packaging/libcxx-runtimes.spec`、project_config 或 QuickBuild
  配置；R91/R94/R96/R97 是编译器选择与打包线。
- 不要重建或替换 MLGO/XLA AOT 资产；LLVM、bcc-tools、bpftrace 的协调迁移
  仍由编译线处理。
- 不要借运行时任务处理 Unified 全量构建失败。
- 若确需这些线的输入，以只读方式引用并记录具体 revision；不要 merge、
  cherry-pick 或向其远端分支推送。

## 8. 接手后的证据纪律

1. 结论必须能独立复核。引用源码时给仓库 revision、文件、行号并贴实际代码；
   引用二进制时记录包/架构/SHA256、符号或反汇编原文。
2. 严格区分实测、源码可达性分析和推断；一个架构/运行时的结果不能外推到
   另一格。
3. 未运行写 `NOT_OBSERVED`；材料或能力不可取得写 `NOT_AVAILABLE` 并记录
   已尝试方法。
4. 任何“零命中”检索必须先用已知阳性样本校准，而且阳性须由独立方法确认。
   坏掉的扫描器与真正没有命中的扫描器会产生相同表面结果。
5. 每次执行前保存编号化、不可变的脚本快照及 SHA256，命令直接引用该快照；
   同时保存依赖脚本清单，确保交付物与实际执行一一对应。
6. 原始命令、stdout、stderr、退出码逐字落盘；非零要区分被测现象、技术性
   命令错误与判据失败。
7. 资源和开发板操作继续使用既有门禁、低并发、明确清理范围；连接中断和环境
   保护提示不通过扩大重试来绕过。

## 9. `noexcept` 决策的最短接手路径

一个全新会话应按以下顺序接手，而无需重做既有调查：

1. 阅读 `docs/progress/R86/FACTS.md` 与 `tables/site_facts.tsv`，确认五站点的
   抛出能力、ABI 和 out-of-line/header 区分。
2. 阅读 `docs/progress/R86/ROUTES.md` 与 `tables/route_matrix.tsv`，保留所有
   路线的正常路径、旧二进制和验证成本条件，不先做技术实现。
3. 阅读 `docs/progress/R72/` 的动态样本和 `docs/progress/R73/` 的 GCC
   PR103382/libstdc++ 对照，确认 D01/D03/D04 已观测与 D02/D05 未观测的边界。
4. 向人工提交的裁决问题至少包括：D01/D02 是否采用符号双版本；旧
   D03/D04/D05 对象是否要求淘汰/重编；D05 是保留析构 sync 语义、引入显式
   close 协议，还是接受其他有行为变化的路线；普通错误传播变化是否接受。
5. 人工选定后再形成逐站点补丁设计和新旧对象/新旧库矩阵；在裁决前不要实施
   某一路线，也不要以未来上游提交便利性替人工选择。

只读本文及上述四组材料，已经可以进入方案裁决；剩余障碍是政策与兼容取舍，
不是缺少对站点、ABI 或既有路线的调查。
