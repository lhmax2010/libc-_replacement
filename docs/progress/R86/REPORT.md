# R86：`noexcept` 边界五处的处置方案评估

## 结论摘要

本任务只形成裁决素材，**不推荐、不排序、不选择方案**，也没有实施任何改动。

1. “五处原本不会抛，删除标注只解除取消终止”这一前提不成立。D01-D03 明确生成 `system_error`，D04 可遇到自定义 Clock 异常并委托到 D02/D03；D05 的开放虚调用可抛普通异常或进入取消点。
2. D01-D04 的成功等待/通知/正常超时路径可以在简单解除异常规格时保持算法意图；错误/取消路径必然从终止改为传播，调用点异常清理代码也会变化。
3. D05 没有显式标注可删。它是覆盖 non-throwing 虚基类析构的隐式 non-throwing 析构，不能单独改为 `noexcept(false)`；必须改变析构同步结构、引入显式收尾协议，或扩大整个 streambuf 基类契约。
4. 顶层 `noexcept` 不进入这五个函数自身的 Itanium 符号名，但进入函数类型，并影响 caller 是否生成展开/清理元数据。因此 D01/D02 “同符号可链接”不等于旧程序 unwind-compatible。
5. D01/D02 是真实 `@@LLVM_22` 动态符号，GNU old/new symbol version 技术上可用；D03/D04/D05 是 hidden header 实现，不能靠共享库双符号替换旧对象。
6. libstdc++ 在 PR103382 中移除了公开普通 wait 和内部普通/两种定时 wait 的 `noexcept`，只对公开 `condition_variable::wait` 保留新旧双版本。generic Clock wrapper 本来就不带标注。其 wbuffer 析构仍隐式 non-throwing，但不做 libc++ 的析构 `sync()`。
7. 改 future 结构只能移除一个间接使用路径，不能修复 D01-D05 本身；D01-D04 还有多种直接/间接消费者，D05 独立。

完整事实见 `FACTS.md`，逐路线的覆盖、后果和验证量见 `ROUTES.md`，探针与原始证据索引见 `VALIDATION.md`。

## 第一步四组事实

### 1. 实际抛出能力

| 站点 | 查明结果 |
|---|---|
| D01 | 非持锁调用和 pthread wait 错误生成 `system_error`；wait 是取消点。 |
| D02 | 非持锁调用和非 timeout 的 timedwait 错误生成 `system_error`；timed wait 是取消点。 |
| D03 | 非持锁调用和非 timeout 的 clockwait 错误生成 `system_error`；clockwait 是取消点。 |
| D04 | 自定义 `Clock::now()`/时钟运算可抛，随后进入 D02/D03。 |
| D05 | 析构同步调用用户 codecvt 与 wrapped streambuf 虚函数，可抛普通异常或进入输出取消点。 |

结论：只能确认“不发生错误/取消的成功路径意图可保持”，不能确认“去掉标注对所有正常路径无影响”。特别是 D05 根本不能单独作这种修改。

### 2. 对已编译程序的影响

- 五处函数自身的顶层异常规格不改变 Itanium 函数名；最小探针和三架构真实 D01/D02 符号支持这一点。
- `noexcept` 是函数类型的一部分，函数指针/相关模板类型会变化。
- 旧 caller 可能按 non-throwing 调用生成，无对应清理 landing pad；新实现若从同符号展开，不能宣称完全二进制兼容。
- D01/D02 可由库端版本化承载旧、新行为。D03/D04/D05 已嵌入消费者，只能通过新头文件重编影响新产物；旧产物保留原行为。

### 3. 保留新旧版本

- 本地独立 `.symver` 探针成功同时导出 `@LLVM_22` 和 `@@LLVM_23`，说明 GNU 机制和当前 Tizen map 形态具有表达能力；探针节点名不代表实际方案。
- libc++ 当前只有 `LLVM_22 { global: *; };`，没有现成 condition-variable compatibility wrapper。真实实现还要避免通配导出 helper。
- D01 可直接类比 GCC；D02 也有动态符号，机制可行但私有成员 wrapper 仍需工程设计；D03-D05 没有库符号可作双版本。
- 头文件三处若随平台全量重编，D03/D04 的新产物不需要库端兼容 wrapper；未重编对象仍有缺口。D05 还需先解决异常规格/结构约束。

### 4. libstdc++ 对照

详见 `FACTS.md` 的逐项表。关键差异是：GCC PR103382 同时解除内部普通和两种定时等待，只给公开 out-of-line wait 做符号双版本；future async 先 join 任务线程，而 libc++ shared state 普遍等待 condition_variable；libstdc++ wbuffer 析构不调用 sync。

## 第二步路线清单

以下顺序仅为编号：

- **A1**：解除 D01-D04，D05 另案。覆盖 4/5；源码改动面集中，但 ABI、旧 caller、全量重编验证面大。
- **A2**：D01/D02 双版本 + D03/D04 新头文件重编。覆盖 4/5；旧 D01/D02 可保留原契约，旧 D03/D04 仍嵌有边界。
- **B1**：future 改用 join/atomic-futex 等结构。只减少 future 暴露，不修复五站点定义；可能触及 `__assoc_sub_state` ABI、调度和多平台 fallback。
- **B2**：condition_variable 改底层非取消等待。不能可靠覆盖 async cancellation，且改变 native_handle/唤醒/时钟等；D05 不覆盖。
- **C**：A2 + D03/D04 重编 + 一个 D05 子方案。理论覆盖 5/5，但不是单一已定设计，验证量是各子项并集。
- **D**：产品迁移 cooperative stop。减少已知暴露，不修库，也不能覆盖未知/闭源调用者。
- **E**：边界附近屏蔽取消。恢复 pending cancellation 的时点仍可能在边界内，或遗留错误线程状态；不能构成完整解法。
- **F**：在 noexcept 内 catch/rethrow 或吞 forced unwind。前者仍 terminate，后者违反强制展开语义，不可行。
- **G**：D05 析构不 sync。只解决 D05，但正常输出收尾改变且可能丢数据。
- **H**：D05 显式 close/sync 协议。只解决遵守新协议的 D05 使用者，需要调用点迁移。
- **I**：放宽 basic_streambuf 基类析构。可让 D05 合法放宽，但影响整个 streambuf 派生层次、ABI/源码/标准契约，验证面远超单站点。
- **J**：接受并记录。零代码变化，五处风险全部保留。

每条路线对成功路径、错误路径、旧程序、验证量和未知风险的逐项回答在 `ROUTES.md` 与 `tables/route_matrix.tsv`。

## 小规模验证

完成三个不触碰 libc++ 源码的最小验证：

1. 符号/函数类型/caller EH 元数据探针：成功，退出 0。
2. D05 派生虚析构约束探针：预期编译拒绝得到确认；外层验证退出 0。
3. GNU 双符号版本探针：标准 map 和 Tizen 通配 map 形态均成功，退出 0。

没有临时删除真实 libc++ 的异常规格，也没有构建完整候选方案。R72 已有 D01/D03/D04 动态结果，本轮不重复。D02/D05 动态行为仍为 `NOT_OBSERVED`。

## 未查清的事实及所需条件

1. **真实 D01/D02 libc++ compatibility wrapper 的最终代码形态**：未实施。需先由人工裁决版本策略，再在目标源码构造候选并用三架构链接器验证。
2. **新版本节点名称与发布/回滚策略**：属于产品 ABI 政策，本任务没有选择；需要发布负责人给出规则。
3. **QuickBuild 在 armv7l/aarch64/x86_64 对 `.symver` 具体实现的接受情况**：独立本机探针不能替代目标构建，需候选实现后的 QuickBuild。
4. **D02/D05 的目标板动态取消结果**：`NOT_OBSERVED`。查清需两架构最小运行用例；D05 还需定义具体 wrapped streambuf/codecvt 场景。
5. **D05 哪种普通输出语义允许改变**：无产品要求，不能自行决定。需要人工明确是否允许取消析构 sync、要求显式 close，或考虑基类层次变更。
6. **预编译/闭源对象的实际存量与可重编比例**：`NOT_AVAILABLE`。需要平台发布清单和二进制来源信息，才能量化 D03-D05 混合版本风险。
7. **B1/B2 在非 Linux/futex fallback 目标的完整约束**：本任务只查了当前源码结构，没有目标平台清单与性能/调度要求。

## 自行判断与尚存疑问

### 本任务自行判断

1. 把“正常路径”拆成成功路径和普通错误路径，因为源码明确存在 `system_error`；若合并描述会错误地声称行为完全不变。
2. 把 D05 从路线 A 的“一律删标注”中拆出，因为语言探针证明它没有可独立放宽的显式规格。
3. 将 `LLVM_23` 仅用作 `.symver` 探针名称，不把它建议为发布节点。
4. 复用 R72 动态结果，不重复消耗资源；D02/D05 严格保留 `NOT_OBSERVED`。
5. 把“future 重构”和“整个 condition_variable 底层重构”拆成 B1/B2，因为覆盖面和风险不同。
6. 任务书一方面禁止向 Gerrit/外部源码仓推送，另一方面明确要求材料 `commit 并 push`；结合 R83-R85 的既有交付方式，本任务把后者理解为只允许推送本项目的文档分支，禁令仍覆盖 Gerrit、LLVM 上游和平台源码仓。除此之外没有扩大推送范围。

### 尚存疑问

- 人工若选择解除 D01/D02，要求是允许新程序观察并捕获原有 `system_error`，还是只要求 forced unwind 穿越、普通错误仍终止？两者会产生不同 wrapper/错误策略。
- D05 的产品目标是仅允许线程取消，还是同时允许 codecvt/streambuf 的普通异常传播？当前没有足以自行选择的要求。
- 平台是否承诺所有包含 D03-D05 的包全量重编并淘汰旧对象？若不承诺，需要定义混合版本支持边界。

## 纪律确认

- 平台源码、四个补丁：**无修改**。
- 完整方案：**未实施**。
- Gerrit、LLVM 上游及平台源码仓：**未推送**。
- 本项目材料仓：按任务要求仅提交并推送 `docs/progress/R86/` 材料；这是上述自行判断所说明的唯一例外。
- 结论状态：**EVALUATION_COMPLETE / DECISION_NOT_MADE**。
