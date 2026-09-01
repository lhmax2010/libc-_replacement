# R86 路线评估

本文件按字母列出可行或被检验过的方向，不表示推荐或优先级。总览见 `tables/route_matrix.tsv`。

## 路线 A：解除异常规格

### A1：D01-D04 直接解除，D05 另案

可能涉及的代码面（仅说明，不实施）：

1. D01：主/C++03 头声明和 `src/condition_variable.cpp` 定义去掉 `_NOEXCEPT`/`noexcept`。
2. D02：主/C++03 头声明和同一 `.cpp` 定义去掉异常规格。
3. D03：主/C++03 头内联声明、定义去掉异常规格。
4. D04：主/C++03 头模板声明、定义去掉异常规格。
5. D05：不能按相同办法单独加 `noexcept(false)`；必须从 G/H/I 等 D05 路线选择结构或扩大基类契约。

覆盖：直接解决 D01-D04 的 forced unwind 边界，不解决 D05。

正常路径：成功等待、被通知和正常超时的算法可以保持；但普通错误会从终止变为传播 `system_error`，自定义 Clock/虚调用异常也可能开始传播。编译器为调用点生成的异常清理数据会改变。因此只能说“不涉及取消且没有异常的成功路径意图不变”，不能说全部正常行为无变化。

已编译程序：D01/D02 符号名不变，通常仍可解析；旧 caller 的异常元数据却可能不完整，不能把它作为兼容策略。D03/D04 需要重编使用者。D05 未解决。

实现/验证量：源码行数有限，但 ABI 与平台交付验证不小。至少需要两架构 D01-D04 取消用例、无效锁与底层错误路径、system/steady/custom Clock、所有 predicate overload、libc++/libc++abi 官方套件、旧对象/新对象与旧库/新库矩阵、平台全量重编和运行抽样。

### A2：D01/D02 双版本，D03/D04 头文件解除

D01/D02 的旧版本保留 non-throwing 行为，新默认版本允许 forced unwind；新头文件使新对象绑定新版本。D03/D04 与 A1 一样靠新头文件和重编。

覆盖：D01-D04；D05 未解决。

正常路径：与 A1 相同。旧对象调用 D01/D02 仍保持旧终止契约；新对象取得新传播行为。

已编译程序：它显式解决 D01/D02 的旧 caller 问题。D03/D04 没有库符号，旧对象仍含旧边界，必须淘汰、重编或接受混合行为。

实现/验证量：除 A1 外，需要添加实际版本节点、旧 wrapper、新默认实现、隐藏 helper；逐架构验证 `readelf --version-info`、old/new symbol binding，并覆盖包升级与回滚。D02 私有成员 wrapper 的具体写法和版本节点命名没有在本任务中选定。

## 路线 B：改变实现结构

### B1：future 不经 condition_variable

可设想借鉴 libstdc++：async 任务完成通过 join；状态就绪使用原子等待/futex 或等价抽象；定时等待另行实现。对 libc++，需要处理：

- `__assoc_sub_state` 当前公开 ABI 布局含 mutex、condition_variable 和 state；
- async、deferred、promise、packaged_task、future/shared_future 共用状态；
- 多等待者、析构等待、at-thread-exit、异常状态发布；
- `wait_for`/`wait_until` 的时钟、精度和 spurious wakeup；
- 非 Linux 目标及 futex 不可用的 fallback。

覆盖：只减少 future 这一条通向 D01-D04 的路径。直接 condition_variable、condition_variable_any、timed/shared mutex、sleep_until 等仍到达 D01-D04；D05 不受影响。它不是五站点的完整解法。

正常路径：等待调度、公平性、性能、唤醒和析构时序都可能变化，即便没有取消。

已编译程序：直接改变 `__assoc_sub_state` 字段有 ABI 风险；若保持布局则需要旁表、复用存储或 pimpl 式工程，复杂度转移而不消失。

验证量：完整 future 官方套件、async/deferred 组合、多等待者/竞态/异常/析构、时钟和超时精度、取消、性能、ABI、各平台 fallback、平台全量测试。

### B2：condition_variable 内部改成不产生取消展开的等待原语

这一路可以减少 deferred cancellation 在 pthread wait 中送达，但会改变 condition_variable 的 native-handle、唤醒、时钟、内存序、公平性及平台抽象。异步取消仍可在任意指令点送达，所以它不能可靠保证 forced unwind 永远不跨 D01-D04；D05 完全不在覆盖内。

若用“关闭取消后等待”实现，则属于路线 E 的状态/时机问题。若用 raw futex，则需要证明 mutex 释放与重新取得、通知不丢失、超时和虚假唤醒等完整语义。

## 路线 C：组合处理

可组合 A2（D01/D02）、新头文件重编（D03/D04）和一个 D05 结构方案（G/H/I）。这是唯一可以在不把 D01-D04 混为 D05 的前提下覆盖五处的方案族，而不是一个已经确定的具体补丁。

覆盖、正常路径和验证量取决于 D05 子选择；总体是 A2 与该子选择验证集合的并集。旧 D03-D05 实例仍会保留旧行为，混合新旧产物必须单独定义支持边界。

## 路线 D：产品侧取消迁移

把已知程序从 `pthread_cancel`（特别是 async cancel）迁移到 cooperative stop + notify + join，可降低已知生产使用者暴露。

覆盖：不修复任何标准库站点，未知调用者、第三方和闭源对象仍可触发。它只能是产品风险处置，不是五处库差异的技术消除。

正常路径：停止时延、资源回收和 shutdown 协议会变化。验证必须逐产品覆盖正常退出、超时、死锁和异常清理。

## 路线 E：边界附近屏蔽取消

在进入等待/析构前 disable cancellation、离开后恢复，可以把部分 deferred cancellation 延后。但恢复时可能立即投递 pending cancel；若恢复动作仍在 non-throwing 函数内，问题重现；若不恢复，则悄然改变调用者线程状态。async cancel 又可能命中屏蔽/恢复之外的边界。

覆盖：无法给五处构成完整且局部正确的解法。正常取消时机和状态发生变化，需要嵌套状态、所有 return/error/pending 情形验证。

## 路线 F：在 `noexcept` 内识别 forced unwind

捕获后重抛仍会穿过 `noexcept` 并终止；吞掉 forced unwind 会破坏 NPTL 强制展开协议，属于不安全行为。该方向在语义上不成立，不能解决站点。

## D05 专门路线

### G：析构不再同步

对齐 libstdc++ 的结构，让 D05 析构不执行 `__close -> sync`，可移除 D05 的析构输出取消路径。

后果：当前析构中 codecvt `unshift`、buffer `sputn`、wrapped `pubsync` 的效果会消失，可能丢失尾部编码状态或缓冲数据；这是无取消普通路径的可见变化。头文件模板要求消费方重编，旧实例不变。验证要覆盖 stateful codecvt、部分转换、短写、失败 streambuf、显式/隐式刷新和取消。

### H：显式 close/sync 协议，析构不同步

新增或要求调用者在析构前显式完成可能失败/取消的收尾，再使析构不进入这些操作。它把错误和取消放到可传播边界，但要求 API/调用点迁移；漏调者的行为和 G 相同。旧源/旧二进制不自动取得协议。

### I：扩大 `basic_streambuf` 析构的异常规格

若把虚基类析构乃至派生层次改为 potentially-throwing，D05 才能合法声明 `noexcept(false)`。这会影响整个 streambuf 派生生态、析构函数类型 traits、第三方 override、旧 caller 展开与标准契约，远超单站点。需要全 iostream/第三方派生类/ABI/升级验证；其标准符合性也需正式条文与目标语言模式审查。本任务没有查到足以把这一路视为局部可交付改动的证据。

## 路线 J：接受并记录差异

不改代码，五处的 forced-unwind 终止窗口保留。成本在诊断、文档和运维；它不解决任何站点，也不能消除外部取消源。

## 共同验证检查点

无论选择含代码变更的哪条路线，都至少要明确：

- 新构建对象与旧构建对象能否混用；
- D01/D02 的动态版本绑定和旧行为是否准确保留；
- D03/D04/D05 的预编译对象淘汰范围；
- deferred 与 asynchronous cancellation 各自行为；
- 普通异常、错误码、超时、spurious wakeup 和资源清理；
- armv7l、aarch64、x86_64 的编译/符号/运行结果；
- libc++/libc++abi 官方套件与平台全量编译、关键运行时回归。
