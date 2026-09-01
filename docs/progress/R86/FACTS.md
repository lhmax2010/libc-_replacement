# R86 事实查明

## 1. 五处是否真的“不会抛”

结论是否定的。这里要区分“成功的普通路径”与“实现中客观存在的异常/强制展开路径”。源码逐项结果见 `tables/site_facts.tsv`。

### D01 `condition_variable::wait`

`libcxx/src/condition_variable.cpp:33-39` 在锁不属于调用者时调用 `__throw_system_error(EPERM, ...)`，底层 `__libcpp_condvar_wait` 返回错误时也调用 `__throw_system_error`。阻塞于 `pthread_cond_wait` 时还可能收到 NPTL 线程取消的 forced unwind。

因此它本来就有普通 `system_error` 生成路径；当前 `noexcept` 把这些错误和 forced unwind 都转成终止。若只去掉异常规格，成功等待的锁与返回行为不变，但错误路径从“终止”变为“异常向上传播”，不是全路径无行为变化。

### D02 `__do_timed_wait(system_clock)`

`libcxx/src/condition_variable.cpp:41-63` 同样在锁无效时生成 `system_error(EPERM)`；`__libcpp_condvar_timedwait` 返回非 0 且非 `ETIMEDOUT` 时生成 `system_error`。底层定时等待也是取消点。

所以结论与 D01 相同：成功/正常超时路径本身可以保持，错误与取消路径会改变。

### D03 `__do_timed_wait(steady_clock)`

`libcxx/include/__condition_variable/condition_variable.h:197-217` 在锁无效时生成 `system_error(EPERM)`，`pthread_cond_clockwait` 返回非 0 且非 `ETIMEDOUT` 时生成 `system_error`；该 pthread 调用也可承载 forced unwind。

### D04 `__do_timed_wait<Clock>`

`condition_variable.h:220-224` 调用 `Clock::now()`、计算 `tp - Clock::now()`，再进入 `wait_for`，后者最终进入 D02 或 D03。自定义 Clock 的 `now()` 不是由 libc++ 保证不抛的封闭实现；其操作可抛，后续 D02/D03 又有上述普通错误和 forced unwind 路径。

### D05 `wbuffer_convert::~wbuffer_convert`

D05 没有可直接“删除”的显式 `_NOEXCEPT`：它是派生自 `basic_streambuf` 的虚析构，当前异常规格是隐式 non-throwing。其函数体调用 `__close()`；输出模式下 `__close -> sync` 进一步调用：

- 用户可覆盖的 `codecvt::do_unshift`；
- wrapped streambuf 的 `sputn`/`pubsync`/`pubseekoff` 虚路径；
- 这些开放虚调用可能抛任意普通异常，也可能进入输出取消点。

所以 D05 也不是“函数体不会抛”。而且代表性语言规则探针确认：非抛虚基类析构的派生析构不能单独声明 `noexcept(false)`，编译器报“looser exception specification”。要放宽它，必须扩大到基类层次，或改变析构结构；这不是简单删一处标注。

## 2. ABI 与已编译程序

### 2.1 符号名

本机 GCC 13 的最小探针显示，在当前使用的 Itanium C++ ABI 下，函数自身的顶层 `noexcept` 不进入 D01/D02/D04 这类函数符号名：移除前后 demangled/raw name 相同。当前 R85 三架构包中的 D01 与 D02 分别以同一 C++ 名称导出为 `@@LLVM_22`。

但 C++17 起 `noexcept` 是函数类型的一部分。探针中函数指针作为模板实参时，non-throwing 指针使用 `PDoFvvE`，potentially-throwing 指针使用 `PFvvE`。因此函数指针类型、依赖它的模板实例名、重载/类型 traits 等仍可能变化。

### 2.2 “能链接”不等于“兼容展开”

调用点探针是本项最关键的限定：

- 调用声明为 `noexcept` 的目标时，调用者没有为该调用生成清理 landing pad/LSDA；
- 调用可能抛的目标时，调用者生成 personality、LSDA、局部对象清理和 `_Unwind_Resume`。

因此 D01/D02 若沿用原符号、仅换成可展开实现，旧程序通常不会“找不到符号”，但旧调用点可能没有为异常穿越准备正确的清理元数据。不能把符号名不变写成二进制完全兼容。

### 2.3 out-of-line 与 header 的区别

- **D01/D02**：共享库中存在真实动态符号，当前为 `@@LLVM_22`。可以在库中承载旧、新实现，但新头文件必须使新对象绑定到新默认版本；旧对象仍绑定旧版本，才能保留原契约。
- **D03/D04/D05**：当前头文件带 `_LIBCPP_HIDE_FROM_ABI`，在 R85 三架构 libc++ 动态符号表中没有观察到对应符号。实现随使用者编译进入消费对象。库端双版本不能替换旧对象内已有代码。
- 全量重编能让 D03/D04 的新消费者取得新异常规格和调用点元数据；不重编的预置/第三方对象仍保留旧边界。D05 即使全量重编，也还必须先选择一个语言上可行的结构方案。

C++03 兼容头也有 D01-D04 的镜像声明/实现；D05 的 C++03 镜像在 `include/__cxx03/locale`。任何实际修改的范围都不能只看主头文件。本任务没有修改这些文件。

## 3. 保留新旧两个版本

### 3.1 现有能力与本地打包形态

Tizen libc++ spec 当前通过 GNU linker version script `packaging/libcxx-llvm22.map`，该脚本只有一个 `LLVM_22 { global: *; };` 节点。它证明当前产物已经使用符号版本，但源码树中没有像 libstdc++ `compatibility-condvar.cc` 那样现成的 libc++ condition-variable 兼容 wrapper 框架。

本任务用独立的、没有修改 libc++ 的 `.symver` 小探针验证了机制本身：

- 同一个 API 名可以同时导出旧 `api@LLVM_22` 与新默认 `api@@LLVM_23`；
- 在当前 Tizen map 的通配节点形态上追加继承节点也能产生这两个版本；
- 通配 `global: *` 会顺带暴露实现 helper，真实实现必须把 helper 隐藏或在脚本中精确控制。

`LLVM_23` 只是探针名称，不是本报告选定的实际版本节点。实际节点名和 release 规则是待人工裁决的 ABI 政策。

### 3.2 对各站点的可行性

- **D01**：可借鉴 GCC。保留旧 non-throwing wrapper 在旧版本，让新头文件/新对象绑定可展开的新默认版本。GCC PR103382 已真实使用这一形态。
- **D02**：它也是当前导出的 `@@LLVM_22` 符号，GNU 符号版本机制在技术上可承载双版本；但它是私有成员，libc++ 没有现成 wrapper，仍需设计能访问内部状态且不额外泄露 helper 的实现。本任务只确认“机制可行”，没有把具体实现视为已解决。
- **D03/D04/D05**：没有可统一重定向的共享库动态符号，双版本手法不能覆盖旧实例。D03/D04 的新头文件加全量重编可解决新产物；旧对象仍需要淘汰或接受旧行为。D05 还需单独的结构决定。

### 3.3 libstdc++ 的真实先例

GCC commit `9e18a25331fa25c3907249fede65a02c6817b06e`（PR103382）同时移除了：

- 内部普通 wait 的 `noexcept`；
- 内部 system/monotonic timed wait 的 `noexcept`；
- 公开 `condition_variable::wait` 的 `noexcept`。

只有公开 out-of-line `condition_variable::wait` 使用旧 `@GLIBCXX_3.4.11` / 新默认 `@@GLIBCXX_3.4.30` 双版本。其回归测试覆盖 ordinary wait、predicate wait、`wait_for`、system-clock `wait_until` 的 pthread cancellation。本机系统 libstdc++ 的符号表也观察到了这两个版本。

## 4. libstdc++ 五处对应形态及 future

| libc++ 站点 | libstdc++ 对应形态 |
|---|---|
| D01 | 公开 wait 原来带 `noexcept`，PR103382 后移除；只此公开符号保留旧/新双版本。 |
| D02 | 内部 system-clock wait 原来带 `noexcept`，PR103382 后移除；没有单独导出双符号。 |
| D03 | 内部 monotonic-clock wait 原来带 `noexcept`，PR103382 后移除；没有单独导出双符号。 |
| D04 | generic public `wait_until<Clock>` 在该修复前后都没有 `noexcept`；它调用的内部 wait 在 PR103382 中解除。 |
| D05 | 析构仍是隐式 non-throwing，但 libstdc++ 将其 `= default`，析构不调用 `sync()`，故没有 libc++ 的析构输出取消路径。 |

`future::wait` 结构也不同：

- libc++ 的导出类 `__assoc_sub_state` 内含 mutex + condition_variable；普通 `wait` 循环执行 `__cv_.wait`，定时等待执行 `__cv_.wait_until`。async/deferred 等共享状态共用这一路径。
- libstdc++ `_State_baseV2::wait` 先调用 `_M_complete_async()`，async 派生实现通过 `_M_join()` 直接 `thread::join()`，随后用原子状态等待；普通原子等待在 Linux 可走 futex，另有 condition-variable fallback。

所以“让 future 不经条件变量”在 libstdc++ 有结构先例，但把它移植成 libc++ 方案会牵涉 shared-state 布局、deferred 执行、定时等待、多等待者与各平台 fallback；它并不自动修复直接使用 condition_variable 的 D01-D04，也与 D05 无关。

## 5. 事实边界

- D01/D03/D04 的动态取消差异已由 R72 样本观察：libc++ 退出 134，libstdc++ 干净取消。D02/D05 动态样本仍为 `NOT_OBSERVED`；其可达性来自源码路径，不把 D01 表现外推成“已观测”。
- `.symver` 探针证明本机 GNU 链接器和当前 map 形态的技术表达能力，不等于已经在 QuickBuild/三目标架构完成真实 libc++ wrapper 验证。
- 没有实施完整方案，也没有修改平台源码或补丁。
