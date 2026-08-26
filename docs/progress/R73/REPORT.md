# R73：libc++ 的 `noexcept` 边界——影响面分析与业界处置调研

## 1. 范围与总结果

本任务只做分析与调研，未修改平台源码、任何产品补丁或落章文档；没有连接开发板，也没有新增构建或运行实验。运行时事实复用 R72 已归档的逐格原始输出，本轮新增的是消费面精化、包级上下文复核、上游调研和与层 C 的关系分析。

结论性事实如下：

1. 接受现状时，D01–D04 的共同触发条件是：Linux/glibc 线程取消可用、目标线程启用取消、线程正阻塞于 libc++ `condition_variable` 的普通或定时等待路径、另一线程在该窗口调用 `pthread_cancel`。D05 还要求 `wbuffer_convert` 析构的 `sync()` 通过 wrapped streambuf 进入输出取消点，条件明显更窄。
2. R72 的 **35 source RPM / 26 去重包名**是刻意宽收的词法上界，裸词 `future` 带来大量非 C++ 标准库命中。本轮按显式 `std::`/`using std::` 语法精化后，剩 **11 source RPM / 9 去重包名**；对这 9 个包名全部做了上下文复核。
3. 在这 9 个显式使用者中，**确认的生产代码“目标线程在相关设施中等待且被 `pthread_cancel`”为 0**；GCC 源包中有 1 个与 D01 完全同形的上游自测。其余包未观察到设施等待点与取消请求的源码内关联。这个“0”不是安全证明：取消可来自其他翻译单元、依赖库或进程宿主，因此这 9 个包仍是潜在暴露面。
4. libc++ 官方政策使用“may”，允许但不强制把 “Throws: Nothing” 强化为 `noexcept`；没有找到该政策内的 pthread-cancellation 专用例外，也没有找到 LLVM 对这一精确冲突的 issue、维护者拒绝或处置表态，均按限定检索范围记 `NOT_FOUND`。
5. GCC PR103382 把该行为认定为错误并移除相关 `noexcept`。最终变更同时处理 ABI：新代码绑定 `GLIBCXX_3.4.30` 的可取消实现，旧 `GLIBCXX_3.4.11` 保留终止行为的 wrapper。
6. 定向检索未发现 Chromium 或常见 libc++ 发行版对此边界的本地补丁。Chromium 的公开线程封装采用投递退出任务后 join；Android/Bionic 则不提供 `pthread_cancel`。这些是不同层次的规避事实，不是 libc++ 边界的等价修复。
7. 5 处边界与层 C 最终补丁**没有文件级重叠**。D01–D04 与层 C 的 future 改动有运行路径上的语义相邻：层 C 处理执行方 catch，`noexcept` 边界处理等待方展开；两者并不互相替代。按当前补丁形态继续层 C 工作不会因文件冲突返工；若未来选择重构 future 的等待实现，则可能重新触及 `future`，这是特定处置方向带来的交叉，而非当前必然返工。

## 2. 五处边界的触发条件与现场表现

逐项表见 `tables/boundary_trigger_conditions.tsv`。

### D01：普通 `condition_variable::wait`

- 条件：目标线程在 `pthread_cond_wait` 中阻塞，取消处于启用状态，其他线程对它发起取消。
- 设施：不仅是直接 `condition_variable`，还包括 libc++ 用它实现的 `condition_variable_any`、future/shared_future、共享互斥设施等。
- 已观察表现：libc++ `CANCEL_RC=0` 后打印 `libc++abi: terminating due to uncaught foreign exception`，退出 134，join 结果未取得；libstdc++ 退出 0，join 返回 `PTHREAD_CANCELED`。
- 归因难度：stderr 看起来像“未捕获外来异常”，不会直接指出 `condition_variable::wait noexcept`；需要调用栈中的 `__clang_call_terminate` 才能把现场归到边界。

### D02：system-clock 定时等待 helper

- 条件：线程阻塞于 `pthread_cond_timedwait` 且在超时前收到取消。
- 可达性：公开 `wait_until(system_clock)` 有具体静态路径。
- 动态表现：本轮及 R72 均未取得动态样本，记 `NOT_OBSERVED`；不得把 D01 的退出文本推断复制为本项观测。

### D03：steady-clock 定时等待 helper

- 条件：当前配置 `_LIBCPP_HAS_COND_CLOCKWAIT=1`，线程阻塞于 `pthread_cond_clockwait`，在超时前收到取消。
- 已观察表现：与 D01 相同，libc++ 退出 134，libstdc++ 干净取消。

### D04：任意 Clock 模板 helper

- 条件：使用非 system/steady 的 Clock 重载，路径经 `wait_for` 到 steady helper，并在底层阻塞窗口收到取消。
- 常见性：相较普通 wait 和 wait_for，多了自定义 Clock 这一条件，因而是 4 个 CV 边界中较窄的一项。
- 已观察表现：libc++ 退出 134，libstdc++ 干净取消。

### D05：`wbuffer_convert` 析构

- 条件：已弃用的 `wbuffer_convert` 处于输出模式并有需要同步的状态；析构在持有待处理取消的线程中执行；其 `__close -> sync` 最终通过 wrapped streambuf 虚调用进入实际输出取消点。
- 与 libstdc++ 的差别不只是异常规格：两侧析构都隐式不抛异常，但 libstdc++ 对应析构不执行 libc++ 这条 `sync()` 路径。
- 动态表现：`NOT_OBSERVED`。因此其可达性来自具体源码路径，现场文本和实际发生频率均未观测。

### 常见性口径

“常见/罕见”没有运行遥测支撑，本报告只按附加前置条件陈述：D01 是最直接的常规阻塞形态；D02/D03 需在定时窗口取消；D04 再要求自定义 Clock；D05 同时要求已弃用设施、析构同步和虚拟输出取消点。是否真的频繁发生，仍取决于平台代码是否使用 `pthread_cancel` 终止这些等待线程。

## 3. 平台包的真实使用形态

### 3.1 方法与代表性

R72 的宽口径扫描覆盖 371/371 source RPM，得到 35/26。为避免把英文单词、注释和非标准库类型当作设施，本轮对原始命中做第二层过滤：只接受显式 `std::name`、`::std::name` 或 `using std::name`。脚本是 `code/refine_facility_hits.py`，完整逐行命中在 `raw/explicit_std_facility_hits.tsv`。

精化结果为 11 source RPM / 9 去重包名：`angle`、`binutils`、`binutils-aarch64`、`binutils-armv7hl`、`binutils-armv7l`、`boost`、`gcc`、`libzypp`、`nnstreamer`。本轮不是再抽少量样本，而是复核了全部 9 个显式命中包名；多个 binutils/GCC source RPM 因版本或架构复用同一上游源码，仍按任务既有 source RPM 与去重包名两种口径分别保留。

随后在这些源码树中检索 `pthread_cancel`、取消状态/类型配置和 `pthread_exit`，再人工核对取消请求文件与标准设施文件是否属于同一实现路径。结果见 `tables/package_risk_review.tsv`。

### 3.2 结果

- `angle`、`libzypp`、`nnstreamer` 有生产路径上的 condition variable/future 使用，但源码树中未命中取消请求。
- binutils 的显式设施位于 GDB support thread pool；唯一 `pthread_cancel` 请求在 CRIS simulator 测试，未观察到与生产 thread pool 的关联。4 个架构/包名变体同源。
- Boost 的标准设施命中分布于公共头、示例和测试；取消请求位于 `libs/core/test/yield_prim_pthread_cancel_test.cpp`，未观察到与这些等待路径的生产关联。
- GCC 的 libstdc++ testsuite 中包含 PR103382 的精确场景，这是测试证据，不是迁移到 libc++ 的产品消费者。

因此：

- **确证处于风险形态的生产包：0 个**；
- **确证同形但仅测试用途：1 个去重包名（GCC）**；
- **显式使用相关设施、但源码内部未观察到取消关联：9 个去重包名**。这里 GCC 也可能有其他设施使用，所以类别不是互斥的安全分区。

无法把后一个数字解释为“不会触发”。`pthread_cancel` 可以由包外宿主、插件管理器、运行框架或未扫描二进制发起；源码共现只能证明正关联，不能证明无关联。35/26 仍作为宽上界保留，11/9 是显式 C++ 设施使用的较可信近似，0 是本轮能在源码内部闭合的生产风险形态数。

## 4. 与此前线程取消问题的比较

| 维度 | 此前层 A/B/C 问题 | 本轮 `noexcept` 边界 |
|---|---|---|
| 机制 | foreign forced unwind 的识别、重抛及显式 catch-all 吞并 | forced unwind 离开不抛异常函数时由编译器终止桩终止 |
| 明确差异点 | 层 C 最终为 50 个显式 catch 站点，另有运行时层改动 | 当前限定配置下 5 个实际差异边界 |
| 触发 | 取消穿过相关 handler/catch 路径 | 取消恰在相关 wait 或析构输出路径中发生 |
| 影响面旁证 | 既有任务已有应用和标准库设施矩阵 | 宽扫 35/26，显式使用 11/9，生产取消关联 0 个被证实 |
| 现场 | abort/foreign exception、可能表现为未重抛 | 同样可只见 uncaught foreign exception；边界需调用栈识别 |

触发难度方面，D01 的技术窗口并不苛刻，但平台包是否采用 pthread cancellation 是决定因素；D04/D05 的附加条件更窄。影响面方面，现有证据只支持“显式设施使用者少于宽扫上界”，不支持把潜在外部取消排除。归因方面，两类问题都可能以 134 和 foreign-exception 文本出现；本轮问题还需识别 `__clang_call_terminate` 所在边界，因此不能声称定位代价更低。是否“相当”没有工时数据，记为定性相近而非量化结论。

## 5. 业界与上游处置事实

完整检索表见 `research/upstream_research.tsv`，检索范围见 `research/search_scope.tsv`。

### 5.1 libc++ 的立场

[libc++ 官方政策](https://libcxx.llvm.org/DesignDocs/NoexceptPolicy.html)写的是自版本 13 起“may”强化 “Throws: Nothing”，并明确这种做法可能阻止用户提供函数的异常传播。D95821 的[评审记录](https://reviews.llvm.org/D95821)特意把 “will” 改为 “may”，理由是允许实现这么做但不要求逐项遵守。因此存在的是**实现裁量空间**，不是一个已经定义好的 pthread-cancellation 例外机制。

对 LLVM GitHub issue、本地 llvm-project 历史进行精确检索，没有找到 `pthread_cancel + condition_variable/noexcept` 或 `forced_unwind + condition_variable` 的 issue/讨论，也没有找到“提出后被拒”的维护者表态，结论为 `NOT_FOUND`。这只覆盖记录在 `search_scope.tsv` 的查询，不代表不存在换词、未索引或线下讨论。

WG21 的 [P3085R0](https://www9.open-std.org/JTC1/SC22/WG21/docs/papers/2024/p3085r0.html)明确指出 C++ 视角不能抛出的 `fclose` 在 POSIX 取消中可以参与异常式展开，并认为这使 `noexcept` 准确性更难界定。它是 2024 年 R0 提案，不作为已采纳标准结论。

### 5.2 GCC PR103382

GCC 的[初始补丁说明](https://gcc.gnu.org/pipermail/libstdc%2B%2B/2021-December/053608.html)直接把 `condition_variable::wait noexcept` 称为错误：NPTL 的 `__forced_unwind` 会因此终止进程，正确行为是让它穿过。变更同时覆盖内部普通/定时/clock wait。

讨论的代价主要是 ABI：已有调用方依据旧头文件把函数视为不抛异常。最终[提交记录](https://gcc.gnu.org/pipermail/libstdc%2B%2B/2021-December/053631.html)采用双版本：新 `@@GLIBCXX_3.4.30` 支持取消，旧 `@GLIBCXX_3.4.11` 通过 `noexcept` wrapper 保留旧行为。社区因此没有把“标准写不抛出”解释为必须终止；同时也没有无视旧二进制兼容性。

检索到的讨论中，兼容 wrapper/符号版本是最终采用方案。没有找到主提交采用“禁用取消”或运行时绕过异常规格；不能把讨论中未采用的可能性写成 GCC 已实施行为。

### 5.3 Chromium、Android 与其他项目

- Chromium 的公开 [`Thread::StopSoon/Stop`](https://chromium.googlesource.com/chromium/src.git/+/refs/heads/main/base/threading/thread.cc)使用投递退出任务和 join；其[自有条件变量封装](https://chromium.googlesource.com/chromium/src/+/HEAD/base/synchronization/condition_variable.h)直接包 pthread condition variable。定向检索未找到 `pthread_cancel` 用法或该 libc++ 边界的本地补丁，记 `NOT_FOUND`。这说明公开主干呈现协作式退出形态，不证明所有 Chromium 衍生代码都禁用取消。
- Android/Bionic 的[官方状态页](https://android.googlesource.com/platform/bionic/+/refs/heads/android11-dev/docs/status.md)明确不实现 `pthread_cancel`，理由是实现成本和正确使用难度。这从 libc 层消除了触发能力，但改变了平台提供的 API，不是标准库局部修复。
- 对 Debian、Fedora、FreeBSD、Alpine 的 libc++ packaging 做了目标查询，未发现精确本地补丁，记 `NOT_FOUND`。FreeBSD 等平台的取消/展开机制也不应直接等同于 glibc forced unwind。

## 6. 与层 C 的文件和机制关系

层 C 最终补丁身份：

- 头文件批 20 站点：`0a32dc44e0e57ed524283bc416bdc5c4f6a00b131de5e421485b6e7bcb4b82e9`；
- 库内批 30 站点：`88228328db96b38ca139ec1fa3b04ddd82c25f17b97e8a90c674feb2a654d90e`。

### 6.1 文件重叠

5 个边界涉及：

- `libcxx/include/__condition_variable/condition_variable.h`；
- `libcxx/src/condition_variable.cpp`；
- `libcxx/include/__locale_dir/wbuffer_convert.h`。

层 C 涉及：

- `libcxx/include/__ostream/basic_ostream.h`；
- `libcxx/include/future`；
- `libcxx/include/istream`；
- `libcxx/include/string`；
- `libcxx/src/future.cpp`；
- `libcxx/src/ios.cpp`。

交集为空。没有某个现有 hunk 需要同时承载两种改动。

### 6.2 机制相容性

层 C 在 catch 站点识别 forced unwind、提交必要状态后重抛；它要求异常继续离开该 handler。`noexcept` 边界则发生在异常继续向外传播之后或另一线程的等待路径：即使 catch 处理完全正确，只要展开跨过 D01–D05，仍会终止。因此：

- 两者兼容，不互相替代；
- future 场景可能同时需要执行方的层 C 防护和等待方边界允许取消穿过，但它们不是同一帧、同一站点；
- 单纯去掉 D01–D04 的异常规格不会使层 C 多余；单纯层 C 也不会修复等待方被取消；
- D05 没有层 C 对应 catch 路径。

### 6.3 是否会造成后续返工

按**当前已经确定的层 C 补丁内容**继续后续工作，不会因为这 5 处出现文件冲突或需要回改现有层 C hunk；因此“暂不处置是否必然导致后续层 C 返工”的答案是 **否**。

边界条件是：如果未来处置方向选择重构 `future` 的等待架构，而不是仅调整 condition-variable 异常规格，那么 `include/future`/`src/future.cpp` 可能重新被修改并需要重验。这是某个尚未裁决方向的后果，不能据此把返工写成必然。

## 7. 处置方向素材（不排序、不推荐）

1. **按 GCC 方向移除 D01–D04 的异常规格**：涉及公开头文件、头文件 helper 和共享库定义；新消费者要重编才能取得一致的调用方异常元数据。对既有二进制还需处理 ABI/符号版本问题。若形成上游接受的 libc++ 变更，可避免永久本地偏离；若仅平台补丁，则形成平台差异。
2. **按平台能力条件化异常规格**：仅在 glibc/pthread-cancellation 配置下允许展开，其他 ABI runtime 保持原状。仍涉及头文件和消费者重编，也引入条件配置矩阵；是否可被上游接受未有直接材料。
3. **双符号/兼容 wrapper**：D01/D02 有 out-of-line 部分，可借鉴 GCC 为旧、新消费者保留不同行为；D03/D04 是 header helper，不能只靠共享库双符号覆盖全部 4 处。验证需含旧对象+新库、新对象+新库以及定时/自定义 Clock。
4. **改变等待实现结构**：让 future 等设施不经过这些边界，或采用不同等待原语。可能同时改库内和头文件，future 方向会与层 C 文件重叠，验证面包括所有上层设施与竞态。
5. **在旧边界内屏蔽/延迟取消**：可避免 forced unwind 离开 `noexcept`，但取消可能推迟到边界之后；若 wait 永不返回，取消也可能长期不完成。它改变取消时序，需要阻塞、超时、通知和多等待者验证。没有发现 GCC 最终提交采用此形态。
6. **应用层协作退出而不使用 `pthread_cancel`**：需要修改应用/框架；Chromium 的公开线程封装呈现这一形态。它避开触发而非改变 libc++，与本项目此前“迁移不改消费者语义”的目标是不同性质的工作。
7. **平台不提供线程取消**：Android/Bionic 是实例；性质是 libc/API 平台决定，影响所有消费者，不是标准库包内交付。
8. **编译器/运行时对 forced unwind 特判绕过 `noexcept`**：工具链全局改动，可能改变语言异常规格的既有语义；本轮未找到已部署的同类项目实例，验证代价至少覆盖普通异常、析构、ABI 和两种展开模型。
9. **D05 单独处置**：可以讨论析构是否同步、异常规格或 wrapped streambuf 结构；这些方向会改变刷新/资源语义，且位于头文件模板，需消费者重编。它不能由 D01–D04 的处理自动解决。
10. **接受并记录差异**：无代码或构建成本；D01–D05 的触发和 134 风险保持存在，外部发起取消的风险仍无法由包内源码扫描排除。

本节只列形态、交付性质和验证面，不作选择或优先级排序。

## 8. 盲区、自行判断与疑问

### 盲区

- 包级分析只有源码，没有平台运行遥测，也未覆盖包外发起的取消、闭源二进制、插件宿主和生成代码。
- 显式 `std::` 过滤会漏掉类型别名、宏、命名空间整体导入后无限定名和封装库；因此 11/9 不是下界证明。
- D02/D05 没有动态样本；D05 的虚调用是开放世界，无法用 libc++ 源码穷尽所有 wrapped streambuf。
- 联网检索受查询词和索引范围限制；LLVM/发行版的 `NOT_FOUND` 不等价于不存在私人补丁或未索引讨论。
- 未验证 aarch64、非 glibc、非 pthread backend、LTO、旧消费者/新库混合 ABI。

### 自行判断

- 把 R72 的 35/26 保留为宽词法上界，同时新增显式 `std::` 的 11/9 口径；没有用后者覆盖或改写 R72 原结论。
- 将全部 9 个显式命中包名作为本轮抽样，因数量可控且比再抽 2–3 个更少引入抽样偏差。
- “确证风险形态 0 个”严格限定为生产源码内可闭合关联；没有据此把包判安全。
- 把 D01–D04 的使用常见性和 D05 的罕见性写成“前置条件多少”的判断，不冒充运行频率统计。
- 没有新增运行实验：任务目标是影响面和业界调研，R72 已提供 D01/D03/D04 的两库动态证据，本轮不重复构建。

### 尚存疑问

- 平台运行时是否存在由框架/服务管理器对上述 9 个包的工作线程发起取消：源码包内无法回答。
- libc++ 维护者是否在未被精确关键词命中的讨论中表达过 POSIX cancellation 例外立场：`NOT_FOUND`。
- D05 在真实 wrapped streambuf 上的退出表现和频率：`NOT_OBSERVED`。
- 若未来裁决处置 D01/D02，Tizen 的 libc++ 符号版本/ABI 策略应如何承载旧消费者：本任务不选择。

## 9. 结论回答

- **若接受差异，何时出问题？** 当取消请求命中 D01–D04 的阻塞窗口，或 D05 的析构同步输出窗口，forced unwind 穿过 libc++ 不抛异常边界并触发终止。D01/D03/D04 已动态证实；D02/D05 动态表现未观测。
- **风险形态包数？** R72 宽命中 35/26；显式标准设施使用为 11 source RPM / 9 包名；源码内确证生产取消关联 0，精确同形自测 1 个包名。外部取消使“0”不能解释为零风险。
- **业界如何处理？** GCC 移除异常规格并以新符号版本处理 ABI；Android 不提供取消；Chromium 公开线程封装采用协作退出。没有找到 LLVM 的精确 issue/拒绝记录或其他发行版的本地补丁。
- **与层 C 是否冲突？** 当前文件无重叠、机制互补；future 路径可同时经过两类逻辑，但不是同一站点。
- **暂不处置是否会让后续工作返工？** 对当前层 C 补丁，**不会必然返工**。未来若选“重构 future 等待实现”这一特定方向，会重新触及层 C 文件并要求重验；其他方向未必。

本报告不做修复、不做处置建议、不扩大既有方案范围。

## 10. 过程异常申报

- 首次提交命令在 `git diff --cached --check` 处退出 2；原因是逐字保存的原始命令/源码证据日志中含尾随空白。为避免篡改原始输出，没有清洗这些日志。
- 为补录该失败原文而执行的命令块没有在 `git diff --cached --check` 非零后显式中止，后续 `git commit` 因而继续并成功。这是本轮命令包装的控制流缺陷。完整原文见 `commands/022_commit_with_global_check_warning.log`。
- 该事件发生于本项目材料提交阶段：产品源码与补丁没有修改；人工撰写文件已通过限定路径的空白/完整性检查，材料 SHA/大小校验为 0 错误。首次 commit 尚未 push，本报告和该日志随后以 amend 纳入同一最终提交。
