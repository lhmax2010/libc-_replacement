# R112：三家评审关键论断核实记录

## 1. 范围与判定口径

本轮只核实论断，不修改平台源码或四个补丁，不给出处置建议。静态源码身份为
`codes/llvm` 提交 `5ed6c77278dfa7a470667cf1a137723d3c96fe60`；运行时实验均记录了
实际库映射或构建输入。`NOT_OBSERVED` 表示本轮没有观察到，`NOT_AVAILABLE`
表示所需材料不可得。

总判定如下：

| 论断 | 判定 | 证据性质 |
|---|---|---|
| 旧调用方解除规格后必然静默死锁 | **按“必然”表述不成立；在无可用清理表的旧帧中成立** | 源码核查 + x86_64 实测 |
| 混合目标文件发生 COMDAT 折叠，语义受链接顺序影响 | **成立**；但 ABI 标签“只随版本变化”的解释不成立 | 源码核查 + x86_64 实测 |
| 去掉 `wbuffer_convert` 析构同步后，约 4095 字符以内一字不留 | **不成立**；本用例保留首字符，其余大量丢失 | 源码核查 + x86_64 实测 |
| 既有 Boost 失败实验全部是过渡期反方向，实际方向未测 | **在本轮前成立**；本轮已补两类实际方向 | 材料核查 + aarch64 QEMU 实测 |
| 部分符号缺失在无 `BIND_NOW` 时首次调用才失败 | **机制成立，举例的 33→21 成立；对该产品进程的最终结果仍有条件** | ELF 核查 + x86_64 机制实测 |
| `async` 最后一个共享状态释放路径还有不抛帧 | **成立** | 静态核查 |
| 四类 libc++ 内部消费路径存在且此前未运行时测试 | **成立** | 静态核查 + 既有探针范围核查 |
| C++03 镜像也须同步改变才会改变 C++03 调用方 | **成立** | 静态核查 + 编译包含链核查 |
| N4861 对 `wbuffer_convert` 析构只列出删除 `cvtptr` | **成立** | 标准原文核查 |
| 601 是可见外联消费记录，不含内联与外部产品 | **成立** | 既有材料原文核查 |
| 平台发布/允许静态链接 `libc++.a`，形成 601 的现实盲区 | **发布这一前提不成立；组织策略不可得；机制假设成立** | 打包规格 + 冻结清单核查 |

## 2. 旧调用方在取消路径上的实际结局

### 2.1 评审源码引用

静态核查确认引用的代码分支存在：

- `EHStreamer.cpp` 在处理调用指令时区分 `nounwind` 调用与可能抛出的调用；
  `SawPotentiallyThrowing` 只因后者置位。它在需要时生成 landing pad 为 0 的
  call-site 区间，作为不应落入相邻清理区间的空隙。
- `cxa_personality.cpp` 在没有 LSDA 时返回继续展开；命中 landing pad 为 0 的
  call-site 记录时也继续展开；扫描后找不到覆盖 IP 的记录时调用终止路径。

对应摘录及行号在 `raw/003_inspect_eh_sources.*`、
`raw/004_inspect_cxa_personality_full.*`。因此评审列出的三个 personality 分支属实。

推理中并非必然的环节是：“旧头把目标调用编成普通 `call`”不能单独推出“该次
展开必定跳过整个旧帧的清理”。最终 IP 被哪条 LSDA 区间覆盖还取决于同一函数
附近的其他调用、代码生成和布局。实测恰好观察到两种结果。

### 2.2 实测设计

探针使用裸 `pthread_create`、延迟取消和真实 `condition_variable::wait`。
解除规格组链接解除规格的实现，但旧调用方仍用带 `noexcept` 的旧头编译。
探针分别记录：

- 帧内清理对象的析构是否执行；
- `unique_lock` 析构引发的目标 mutex 解锁次数；
- `pthread_mutex_trylock` 所见底层所有权状态；
- 另一线程用 300 ms `pthread_mutex_timedlock` 获取同一 mutex 的结果；
- `pthread_join` 是否得到 `PTHREAD_CANCELED`。

“有邻近可抛调用”形态在等待前后保留 `puts`；“quiet”形态去掉这两个调用。
每格 5 次。脚本在 `code/item1_oldcaller_cancel.cpp`、`build_item1.sh`、
`run_item1_matrix.py`，汇总在 `tables/item1_cancel_matrix.tsv`。

### 2.3 结果

| 调用方/实现 | 5 次结果 | 帧清理 | mutex 最终状态 | 另一线程 |
|---|---|---:|---|---|
| 旧头 + 保留规格 | 5/5 进入终止处理器，退出 86 | 进程先终止，未到达后续检查 | `NOT_OBSERVED` | `NOT_OBSERVED` |
| 旧头 + 解除规格，有邻近调用 | 5/5 干净 join 为 canceled | 5/5 | 5/5 可重新获取 | 5/5 获取成功 |
| 旧头 + 解除规格，quiet | 5/5 join 为 canceled | 0/5 | 5/5 `EBUSY` | 5/5 `ETIMEDOUT` |
| `-fno-exceptions` 旧头 + 解除规格，有邻近调用 | 5/5 join 为 canceled | 0/5 | 5/5 `EBUSY` | 5/5 `ETIMEDOUT` |
| `-fno-exceptions` 旧头 + 解除规格，quiet | 同上 | 0/5 | 5/5 `EBUSY` | 5/5 `ETIMEDOUT` |
| 新头 + 解除规格，两种布局 | 共 10/10 干净退出 | 10/10 | 10/10 可重新获取 | 10/10 获取成功 |

quiet 旧帧及两个 `-fno-exceptions` 格完整符合评审所述：pthread 取消清理已经
重新锁定 mutex，C++ 帧清理未运行，其他线程之后持续等不到该锁。这里的“死锁”
由定时获取的 `ETIMEDOUT` 和底层 `EBUSY` 共同确认，不是以“进程没有崩溃”替代
行为断言。

有邻近可抛调用的旧帧则执行了清理，直接反例说明“所有旧调用方必然静默死锁”
不成立。`raw/014_inspect_item1_worker_ir_sections.*` 还确认 quiet worker 没有
personality/LSDA，而带邻近调用的 worker 有清理 landing pad；`-fno-exceptions`
worker 没有 personality。

最终判定：**静默锁滞留是解除规格后旧调用方的一种真实第三结果，但不是仅凭旧头
即可断言的必然结果；是否发生取决于具体旧帧的展开表与布局。**

### 2.4 平台 `-fno-exceptions` 数量

现有冻结源码材料能检出显式文本选项，但不能把源码树中出现一个选项等同于实际
交付 ELF 的编译命令。平台所有包逐编译单元的命令数据库/完整构建日志不可得，
所以“实际有多少包以此选项编译”记为 `NOT_AVAILABLE`。

可得的 R98 抽样构建日志中至少观察到 `angle` 的真实 C++ 编译命令带
`-fno-exceptions`；R100 摘要还显示 `openfst` 的 C++ 预处理/配置命令带该选项，
但摘要没有保留足以证明所有实际编译单元的完整命令。`glibc` spec 中构建辅助
wrapper 的命令也带该选项。它们只能形成“至少有实际例子”的下界，不能形成平台
包数。证据见 `raw/061_inspect_available_build_log_fno_evidence.*`；全源码和抽样
完整日志的追加扫描因底层文件 I/O 长时间阻塞而中止，原始退出记录保留在
`raw/053_*`、`raw/062_*`、`raw/063_*`，不把不完整扫描结果作为分母。

## 3. 头文件内联函数的 COMDAT 折叠

### 3.1 宏和 ABI 标签

`_LIBCPP_HIDE_FROM_ABI` 由隐藏可见性、排除显式实例化和
`abi_tag(_LIBCPP_ODR_SIGNATURE)` 组成。评审所称“标签只随 libc++ 版本变化”
不准确：当前 ODR 签名由 hardening 模式、断言语义、是否启用异常以及 libc++
版本共同构成。源码见 `raw/017_inspect_item2_macro.*`、
`raw/021_inspect_item2_symbols_and_archive_scope_retry.*`。

平台补丁只改变目标函数的异常规格，没有改变上述四个签名输入。实验中的旧、新
目标文件因此都发射 `[abi:nqe220108]` 的完全同名弱 COMDAT 定义。

### 3.2 混合链接实测

两个 TU 分别用旧头和解除规格头编译，各自实例化 steady-clock
`wait_until/__do_timed_wait`。每个最终入口各跑 5 次：

| 链接形态 | 旧入口 | 新入口 | 最终选中语义 |
|---|---|---|---|
| `old.o new.o` | 5/5 终止 | 5/5 终止 | 旧定义 |
| `new.o old.o` | 5/5 捕获 `system_error` | 5/5 捕获 | 新定义 |
| 旧成员在前的混合静态归档、whole-archive | 5/5 终止 | 5/5 终止 | 旧定义 |

总计 30 次均符合断言。对象 section group、最终符号和 link map 均记录在
`raw/021_*`，行为矩阵在 `tables/item2_comdat_matrix.tsv`。

因此，“旧对象各自保留旧行为”不成立：在本次 GNU/ELF/lld 链接中，同一链接
单元两个入口共同采用先出现的 COMDAT 定义。这里观察到的是可重复的“先出现者
胜出”，不是观察到随机选择；其他链接器策略未实测。

### 3.3 支持范围与 8,911 个静态归档

事实影响是：混合链接单元不能按单个 `.o` 的编译头版本分别陈述运行语义；必须
把最终链接所选择的 COMDAT 定义纳入范围描述。

静态归档确实可以携带旧对象并在最终链接时参加同名 COMDAT 折叠，本轮归档用例
直接证明该机制。此前 8,911 是冻结材料中“已发布静态归档”的总数，其中
1,600 个仅有宽泛 C++ 指标；它不是已证明携带目标内联函数的归档数。因此：

- “静态归档是这种混合的一种来源”成立；
- “8,911 个归档都是这种混合来源”不成立；
- 实际包含目标实例的归档数为 `NOT_AVAILABLE`。

## 4. `wbuffer_convert` 的实际丢失量

### 4.1 源码路径

当前构造函数调用 `setbuf(nullptr, 4096)`。首次输出进入 `overflow`；首个宽字符
先被转换并写到底层 byte streambuf，随后 `__write_mode` 才把内部缓冲的
`__ibs_-1`（4095）个元素设为 put 区。后续写满时再由 `overflow` 刷出。
相关源码在 `raw/023_inspect_wbuffer_sources.*` 和
`raw/028_inspect_wbuffer_first_write_path.*`。

这解释了评审推理漏掉的首字符：put 区不是在第一个字符写入前就已建立。

### 4.2 300 次实测

每个长度、实现和结束方式跑 5 次。UTF-8 ASCII 输入使宽字符数等于输出字节数。

| 输入宽字符数 | 保留同步：析构结束 | 去掉同步：析构结束 | 去掉同步：显式 `pubsync()`/`seekoff()` |
|---:|---:|---:|---:|
| 1 | 1 | 1 | 1 |
| 3 | 3 | 1 | 3 |
| 100 | 100 | 1 | 100 |
| 4094 | 4094 | 1 | 4094 |
| 4095 | 4095 | 1 | 4095 |
| 4096 | 4096 | 1 | 4096 |
| 4097 | 4097 | 4097 | 4097 |
| 5000 | 5000 | 4097 | 5000 |
| 8191 | 8191 | 4097 | 8191 |
| 8192 | 8192 | 4097 | 8192 |

每格 5 次一致，汇总见 `tables/item3_wbuffer_matrix.tsv`。因此“一字不留”不成立；
但把丢失量称作普通“小段尾部”也不符合实测，例如 4096 个字符只输出 1 字节，
丢失 4095 个字符。

`__close()` 只有析构一个调用点；`sync()` 本身还由 `seekoff`、`seekpos` 调用，
也可经公开 `pubsync()` 到达。只去掉析构中的 `__close()` 不改变这些函数体，实测
显式 `pubsync()` 和 `seekoff()` 仍输出完整值。评审指出的措辞歧义成立。

## 5. Boost 过渡期方向与部分符号缺失

### 5.1 方向核查与实际方向实测

R109/R110 的既有混合探针均为 libc++ 消费方调用 R11 的 libstdc++ Boost
provider；同库对照为 libstdc++ 消费方。因此“本轮前没有测过实际过渡方向”成立。
既有材料见 `raw/030_inspect_existing_boost_direction_tests.*`。

本轮在 aarch64 QEMU 用户态用真实 R105 libc++ Boost provider 补了两类实际方向。
每格 5 次，并由 `/proc/self/maps` 断言 provider 与两套 C++ 运行库同时加载：

| 边 | 同库 libc++ 对照 | libstdc++ 消费方 → libc++ provider |
|---|---|---|
| Boost.Filesystem `path/status/read_symlink` | 5/5 精确值 `payload.txt`，销毁完成 | 5/5 返回空值、状态和值断言失败，销毁完成，退出 41 |
| Boost.Test `normalize_test_case_name`，`std::string` 返回 | 5/5 精确值 `alpha_beta_gamma`，销毁完成 | 5/5 先读出异常巨大 size，随后在销毁路径前 `SIGSEGV` |

矩阵见 `tables/item4_actual_direction_matrix.tsv` 和
`item4_boost_test_actual_direction_matrix.tsv`。首次 Boost.Test 尝试使用旧
`B5cxx11` 名称时，真实新 provider 不定义该名称，`dlsym` 失败；该失败记录保留在
`raw/040_*`。随后用新 provider 实际导出的同语义名称调用，才进入 ABI 传值实测。

与反方向相比，宏观上都能出现 ABI 值损坏；具体形态并不相同。既有反方向
Boost.Test 是静默错值且完成销毁，本轮实际方向为 `SIGSEGV`；Filesystem 两向
都是值错误，但具体 size/值不同。因此“失败形态完全相同”不成立。

### 5.2 `RELRO`、`NOW` 与延迟绑定

对 90 边材料解析到的 84 个按“架构+RPM 哈希+ELF 路径”去重的真实消费 ELF
逐个检查：

| 架构 | ELF | `GNU_RELRO` | `BIND_NOW` | `JMPREL`/`.plt` |
|---|---:|---:|---:|---:|
| aarch64 | 28 | 25 | 0 | 28/28 |
| armv7l | 28 | 25 | 0 | 28/28 |
| x86_64 | 28 | 25 | 0 | 28/28 |

因此“没有 `-z now`”对这 84 个文件成立；“都只有 `-z relro`”若理解为每个文件
都有 RELRO 则不成立，因为每架构有 3 个没有 `GNU_RELRO`。E029 的
`security-manager-cmd` 在三架构均为 RELRO=1、NOW=0、JMPREL=1、PLT=1。
逐文件表见 `tables/item4_consumer_binding_tags.tsv`。

部分缺失机制实测使用两个同 SONAME provider：旧版定义 `present` 和 `missing`，
运行时替换为只定义 `present` 的新版。匹配上述产品 ELF 动态标签的 lazy 格和
`-z now` 正向对照各跑 5 次：

| 形态 | 结果 |
|---|---|
| lazy，未调用缺失符号 | 5/5 进入 `main` 并正常退出 |
| lazy，首次调用缺失符号 | 5/5 先进入 `main` 和调用点，再以 127 及 `symbol lookup error` 退出 |
| NOW 对照，即使不调用缺失符号 | 5/5 在 `main` 前以 127 及同类错误退出 |

所以评审关于延迟绑定机制的论断成立。该机制用 x86_64 动态加载器实测，真实产品
ELF只做静态标签核查，未把主机实验外推为对每个产品进程的直接运行观测。

### 5.3 33→21 示例

E029 是 aarch64 `security-manager-cmd` → Boost.Program_options。对记录的 33 个
旧交集符号重新逐个查真实旧 provider 和 R105 libc++ provider：旧 provider
33/33 定义，新 provider 只有 21/33 保留相同 mangled name，缺 12 个。论断成立，
逐符号表见 `tables/item4_e029_symbol_retention.tsv`。

“该生产进程一定在首次调用退出”仍是条件性陈述：需要缺失符号没有被进程中其他
DSO解析，且执行实际到达该 PLT 项。本轮没有完整产品运行闭包，二者为
`NOT_OBSERVED`；不能仅由 33→21 推断发生频率。

## 6. 其余论断

### 6.1 `std::async` 最后共享状态释放路径

**成立（静态核查）。** `future<T>::~future()` 调用 `__release_shared()`；异步共享
状态最后一次释放进入头文件模板
`__async_assoc_state<T,F>::__on_zero_shared() _NOEXCEPT`，其中先调用 `wait()`。
`wait()` → `__sub_wait()` → `__cv_.wait(lock)`，即普通等待站点。模板体位于头文件，
会编入实例化它的消费者。源码见 `raw/054_inspect_item5_source_claims.*`。

这条路径除五个目标异常规格之外，确实再压有一个显式 `_NOEXCEPT` 的
`__on_zero_shared` 帧；`future` 析构自身还具有析构函数的隐式不抛契约。因此只
改变五处并不能把该特定展开路径陈述为没有其他不抛边界。

### 6.2 内部消费设施

**路径成立；“此前未运行时测试”成立。** 当前源码显示：

- `timed_mutex::lock` 和 `recursive_timed_mutex::lock` 循环调用 `__cv_.wait`；
- `shared_mutex` 基类的 exclusive/shared lock 路径调用 `__gate1_/__gate2_.wait`；
- `future::wait` 经 `__sub_wait` 调用 `__cv_.wait`；
- `condition_variable_any::wait` 解开用户锁后调用其内部 `__cv_.wait`。

R72 已静态列出这些可达设施，所以“此前完全不知道这些路径”不成立；但 R109、
R111 的相关运行时探针源码只直接构造 `condition_variable`，对上述设施名做零命中
并以直接探针名命中作正向对照，故“这些设施未做运行时取消实测”成立。证据见
`raw/057_inspect_item5_scope_and_prior_runtime_probes.*`。

`condition_variable_any::__unlock_guard` 的析构显式 `_NOEXCEPT`，析构体调用用户
锁的 `lock()`，源码注释也明确异常会转为 `std::terminate`。所以“展开期间重新锁
用户锁，而该锁若抛出会终止”成立（静态核查，未另做抛锁运行实验）。

### 6.3 C++03 镜像

**成立。** `__cxx03/__condition_variable/condition_variable.h` 在普通 `wait`、
system-clock、steady-clock 和 generic Clock 三个 `__do_timed_wait` 声明/定义上有
对应 `_NOEXCEPT`。R111 overlay 生成器只复制并改写主
`include/__condition_variable/condition_variable.h`，未改 C++03 镜像。

以 `-std=c++03` 编译的包含链实测见 `raw/060_compile_cxx03_header_probe_platform.*`；
其结果若因环境 I/O 中止则以源码选择逻辑为静态证据，不把未完成编译写成通过。
事实含义是：要改变 C++03 消费者所见声明/内联体，C++03 镜像必须发生对应变化；
此前实验的主头 overlay 不会替它改变。

### 6.4 N4861 D.21.2 第 11 段

**成立（标准原文核查）。** WG21 N4861，D.21.2
`[depr.conversions.buffer]` 第 11 段在 `~wbuffer_convert()` 下列出的 Effects 是
析构函数删除 `cvtptr`。原文来源：
`https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/n4861.pdf`，PDF 1688 页。

对当前事实依据的含义是：析构自动调用 `sync()` 是 libc++ 当前实现的额外可观察
行为，不是该段列出的标准析构效果；删除 conversion facet 才是该段明确要求的
效果。本轮没有据此作方案选择。

### 6.5 601 的准确口径

**成立。** 准确口径是：R11 冻结发布仓 11,332 个包记录、39,166 个 ELF 的动态
符号解析中，普通等待和 system-clock 等待得到 612 条未定义符号记录；按
“仓库、二进制包、架构、ELF 路径”去重为 601 个消费记录（131 个二进制包、
80 个 source RPM）。这 601 个都映射到 R24 重编计划。

601 不包括 steady/generic Clock 的隐藏头文件内联实例，也不包括外部产品/应用。
既有材料原文与汇总表见 `raw/057_*` 和
`docs/progress/R109/tables/w1_binary_consumers_summary.tsv`。

### 6.6 静态 `libc++.a` 盲区

**机制成立，但平台发布前提不成立。** 若另有 `libc++.a` 被静态链接，库实现可
嵌入消费 ELF，601 所用的动态 UND 口径不能可靠枚举这种实例，因而是方法学盲区。

当前平台 `libcxx-runtimes.spec` 明确设置：

- `LIBCXX_ENABLE_STATIC=OFF`；
- `LIBCXX_INSTALL_STATIC_LIBRARY=OFF`；
- libc++abi 静态库同样关闭；
- `libc++-devel` 文件表只列共享链接名、头文件和 `libc++experimental.a`，不列
  `libc++.a`。

冻结清单 8,911 个已发布静态归档中，精确 basename `libc++.a` 为 0；同一方法
命中 7 个 `libstdc++.a`，作为正向对照。见 `raw/056_summarize_static_archive_scope.*`
和 `raw/057_*`。所以在当前冻结发布材料内，发布的 `libc++.a` 数量为 0。

平台是否允许外部产品自行构建/携带静态 libc++ 的组织策略为 `NOT_AVAILABLE`；
外部 BOM、构建日志和 link map 也不可得。因此外部自带静态实例的规模为
`NOT_AVAILABLE`，不能从冻结发布仓的 0 推成全平台 0。

## 7. 自行判断、限制与疑问

- 将旧调用方分成“带邻近可抛调用”和 quiet 两种，是为验证评审推理中代码布局
  是否为必要条件；没有把任何一种人为指定为全部产品调用方的代表。
- Boost.Test 的新 provider 不再导出旧 `B5cxx11` 名称；为分开验证“缺符号”和
  “同名 ABI 传递”两种机制，先保留 dlsym 失败记录，再按新 provider 的真实导出
  名称做传值实验。
- 84 个 ELF 是从 90 边表按架构、RPM 内容哈希和路径去重所得，不替换其他材料中
  不同口径的“75 个二进制”数字。
- 平台全量逐编译单元 `-fno-exceptions` 使用数、外部静态 libc++ 使用数和 E029
  完整运行闭包均缺材料，分别标为 `NOT_AVAILABLE` 或 `NOT_OBSERVED`。
- 本轮不作处置建议或优先级排序。

## 8. 证据索引

- 原始命令、stdout、stderr、退出码：`raw/`
- 探针与脚本：`code/`
- 构建物和逐轮结果：`build/`
- 汇总表：`tables/`
- SHA256 清单：`MANIFEST.sha256`
- 尚存疑问：`QUESTIONS.md`
