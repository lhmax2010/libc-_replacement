# R72：libc++ 中“取消即终止”边界的枚举

## 1. 范围与结论

本任务未修改平台源码、产品代码或任何补丁；新增内容仅为枚举脚本、消费面扫描脚本、验证用例和报告材料。

以项目当前的 Linux/glibc 2.40、pthread 后端、`_LIBCPP_HAS_COND_CLOCKWAIT=1` 配置为口径，源码可闭合验证的结果为：

- **两套库实际行为差异：5 个边界定义，5 个均可达**。其中 4 个位于 libc++ 的 `condition_variable`：一个普通等待边界、两个时钟特化的定时等待边界、一个任意时钟模板边界；libstdc++ 的对应函数均已移除 `noexcept`。另 1 个是 `wbuffer_convert` 析构：两侧析构都隐式不抛异常，但只有 libc++ 析构会主动 `sync()`，因此只有 libc++ 把 wrapped streambuf 的取消点带入该边界。
- **两侧共有：8 个边界定义**。包括 `jthread` 的析构/移动赋值、`random_device` 析构、filesystem 内部文件描述符析构、`basic_filebuf` 析构、进程退出时的标准流刷新，以及两个依赖虚拟 streambuf 输出路径的析构边界。这些不是迁移新增。
- **另有 1 个仅语法形态不同但未形成独立行为差异的内部 helper**：libc++ `FileDescriptor::close()` 显式 `noexcept`，libstdc++ 对应 helper 未显式标注；但当前唯一调用者是两侧都隐式 `noexcept` 的 RAII 析构函数，外层共同边界已经遮蔽该差异。

逐项证据见 `tables/verified_boundaries.tsv`。上述“5/5”是当前目标配置下经源码闭合或具体可构造虚路径复核的**可证数字**，不是对任意用户回调、任意自定义 streambuf、任意 libc 或任意模板实例化的数学完备证明；盲区见第 8 节。

## 2. 方法与校准

方法分三环：

1. 用 Universal Ctags 定位 libc++ `include/` 与 `src/` 中的函数定义；显式识别 `noexcept`、`_NOEXCEPT`、`_LIBCPP_NOEXCEPT`，并把析构函数列为“隐式异常规格待复核”。
2. 以 POSIX required/may 清单及 glibc Linux 取消包装器为种子，建立词法调用图，从每个不抛异常候选做有界 BFS。
3. 对候选逐项人工复核重载、同名函数、条件编译、实际 libc 包装器和 libstdc++ 对应实现。自动输出只作为候选生成器，不直接作为结论。

计数单位是当前配置中源码可见的函数定义；同一模板按定义计 1，不按实例化数膨胀。对仅由已计边界包裹、且源码中没有独立定义的隐式外层析构不重复计数。两库对照同时检查异常规格和调用结构；`wbuffer_convert` 说明“对应函数也隐式不抛异常”并不自动等价于“两侧同样受影响”，因为 libstdc++ 对应析构没有 libc++ 的 `sync()` 调用。

校准成功：脚本重新发现了已知项 `condition_variable::wait -> __libcpp_condvar_wait -> pthread_cond_wait`（`commands/028_calibration_verified_rows.log`）。同时校准也暴露了词法图的预期假阳性，例如 `wait`、`remove`、`max` 等同名函数会串错；因此原始 882 行候选没有被当作 882 个边界。完整原始候选保留在 `raw/noexcept_candidates.tsv`。

## 3. 取消点清单与平台口径

`raw/cancellation_points.tsv` 完整列出：POSIX required 58 个名字、POSIX may 205 个名字，以及初始的 13 个保守 glibc 扩展种子。来源是 Linux `pthreads(7)` 与 POSIX 取消规则。名字级清单仍需结合参数复核，例如 `fcntl` 仅特定阻塞命令、`lockf` 仅 `F_LOCK` 属 required cancellation point；枚举器故意先宽收集、后人工排除。

平台实际口径另用与目标 libc 同版本的 GNU glibc 2.40 官方源码核对 `SYSCALL_CANCEL`/取消包装器；官方 tarball SHA256 为 `19a890175e9263d748f627993de6f4b1af9cd21e03f080e4bfb3a1fac10205a2`，GitHub mirror 的同 tag commit 为 `3d1aed874918c466a4477af1da35983ab036690e`。所抽查的取消/非取消 wrapper 文件在二者间逐文件 SHA256 相同。Linux 顶层源码确认 67 个包装器文件名；其中 32 个名字不在上述 POSIX-required 名字集中，见 `tables/platform_extension_cancel_points.tsv`。这 32 个含 64 位别名和 Linux 扩展（如 `accept4`、`epoll_wait`、`getrandom`、`preadv2`、`splice` 等），不等价于“32 个彼此独立的规范 API”。目标 GBS sysroot 的 `libc.so.6` 为 glibc 2.40，SHA256 `215bfd3dc2b21691e626990cc0990a38d6f7e9f83b5a2f64c9e1e033d07ba656`；源码材料是同版本上游旁证，不冒充平台补丁源码。

POSIX “may” 不能直接冒充本平台的实际取消点。例如当前 glibc 的 `stat/lstat/mkdir/ioctl` 走非取消 syscall，`closedir` 走 `__close_nocancel`；相关 libc++ filesystem/noexcept 路径只列入 `tables/portable_only_candidates.tsv`，未计入平台实数。stdio 与自定义 streambuf 路径依赖 FILE 标志及虚调用，按条件可达处理。

## 4. 实际差异与可达设施

5 个实际差异为：

| ID | libc++ 边界 | 取消点 | 深度 | libstdc++ |
|---|---|---|---:|---|
| D01 | `condition_variable::wait` | `pthread_cond_wait` | 2 | 对应声明和定义均非 `noexcept` |
| D02 | `__do_timed_wait(system_clock)` | `pthread_cond_timedwait` | 2 | 对应 helper 非 `noexcept` |
| D03 | `__do_timed_wait(steady_clock)` | `pthread_cond_clockwait` | 1 | 对应 helper 非 `noexcept` |
| D04 | `__do_timed_wait<Clock>` | 经 `wait_for` 到 D03 | 3 | 任意时钟路径非 `noexcept` |
| D05 | `wbuffer_convert::~wbuffer_convert` | `sync()` 经 wrapped streambuf 到输出取消点 | 开放虚路径 | 对应析构同为不抛异常，但不调用 `sync()` |

前 4 项不只影响直接使用 `condition_variable` 的代码。源码可达的设施包括 `condition_variable_any`、future/shared_future、`timed_mutex`/`recursive_timed_mutex`、`shared_mutex`/`shared_timed_mutex` 和任意时钟版 `this_thread::sleep_until`；D05 影响绑定输出 streambuf 的已弃用 `wbuffer_convert`。路径见 `tables/facility_paths.tsv`。

可达性判据是：公开入口能在正常线程调用中阻塞于对应 pthread 取消点，且调用链上的该 libc++ helper 带不抛异常规格。对 D05，具体可构造条件是 wrapped streambuf 的同步输出进入取消点。普通 wait、steady 定时等待与自定义时钟等待均取得动态证据；system-clock 特化与 D05 取得静态/具体构造路径证据，本轮按“抽样 2–3 项”的任务要求未另做运行样本。

## 5. 抽样验证

用例 `code/noexcept_cancel_samples.cpp` 满足：工作线程非 `noexcept`、裸 `pthread_create`、应用层无 catch、取消点明确，并断言 `pthread_join` 的结果必须为 `PTHREAD_CANCELED`。

三组结果完全一致：

- libc++ 的 `wait`、`wait_for`、自定义时钟 `wait_until` 均收到 `pthread_cancel`（`CANCEL_RC=0`），随后打印 `libc++abi: terminating due to uncaught foreign exception`，退出 134，未能返回 join 结果。
- libstdc++ 三组均退出 0，`JOIN_RC=0` 且 `JOIN_CANCELED=1`。

逐格原始输出在 `raw/tests/`，汇总见 `tables/sample_results.tsv`。最终二进制对应的 `commands/039_final_runtime_identity.log` 中，`ldd` 与 `LD_DEBUG=libs` 证明 libc++ 格加载指定实验构建的 `libc++.so.1`/`libc++abi.so.1`，参照格加载系统 `libstdc++.so.6`。

## 6. 平台消费面

消费面以 R32 已成功扫描的 371 个 source RPM 为分母，对会到达 D01–D05 的设施做源码词法命中；结果见 `raw/facility_usage_summary.tsv`，逐文件命中见 `raw/facility_usage_hits.tsv`。

扫描 371/371 source RPM 成功、失败 0；它们对应 347 个去重包名。至少命中一种相关设施的有 **35 个 source RPM / 26 个去重包名**。按 source RPM / 去重包名分设施为：`condition_variable` 13/10、future 族 35/26、timed-mutex 族 3/2、shared-mutex 族 4/3、`sleep_until` 3/2、`wbuffer_convert` 2/1。同一包可同时计入多类，故分项不可相加；`wbuffer_convert` 的唯一包名是 GCC 自身源码/测试，不是独立产品消费者。

该计数只是近似上界：它会计入测试、示例、内嵌第三方代码和未构建分支；它不证明包最终以 libc++ 编译，也不证明线程会在该设施中被取消。反过来，宏封装、生成代码和无源码预编译对象可能造成漏计。

## 7. 处置方向素材（不作推荐）

1. **参照实现做法**：GCC PR103382 从普通 wait、内部定时 wait 和实现定义处移除了 `noexcept`，理由是 NPTL 的 `__forced_unwind` 必须能够穿过。GCC 的说明同时把旧头文件已记录的异常规格称为潜在 ABI 问题。
2. **libc++ 当前政策**：libc++ 的 `noexcept` policy 允许从版本 13 起把标准写为 “Throws: Nothing” 的函数额外标成 `noexcept`，并明确承认用户提供的抛出可能不再传播。公开检索未找到 libc++ 对 pthread cancellation/condition_variable 这一精确场景的 issue 或维护者处置表态，记 `NOT_FOUND`。
3. **若以移除异常规格处理**：D01/D02 有 out-of-line 定义，但异常规格也出现在头文件声明，旧消费者仍可能按 `noexcept` 生成终止路径；D03/D04 是头文件内联 helper。D05 位于头文件模板中，但其差异还包含析构是否执行 `sync()`，不是只删异常规格即可等价。其交付性质包含库重建和消费者重编两个维度。
4. **若以结构替换处理**：可以改变上层设施采用的等待原语或控制流，使强制展开不穿过该边界；不同设施位于库内或头文件，交付性质不同。
5. **在 `noexcept` 内 catch 后再普通重抛**不能消除边界：异常离开函数时仍违反异常规格。若采用运行时专用续跑机制，则属于不同于普通 C++ 重抛的实现形态。
6. **上游处理**：可形成 libc++ policy 例外或跨平台条件实现；这会涉及上游规则与非-pthread ABI 配置，而不是单纯的平台包修改。

以上仅陈述形态与约束，不排序、不推荐。来源 URL、检索词与 `NOT_FOUND` 记录见 `research/upstream_sources.tsv`。

## 8. 盲区、判断与疑问

### 枚举盲区

- 虚函数/用户回调是开放世界：自定义 streambuf、allocator、deleter、predicate 或用户析构可以在任意隐式 `noexcept` 析构内调用取消点，无法仅靠 libc++ 源码闭合枚举。
- 词法图不解析重载、模板实例化、函数指针、虚调用、宏展开后的精确目标；本轮靠人工复核消除已见假阳性，但不能证明零漏项。
- POSIX 允许实现把 205 个 “may” 函数设为取消点；本轮平台实数只适用于 glibc 2.40/Linux 目标配置，不外推到 musl、其他 glibc 版本或非 Linux。
- 只做 x86_64 动态抽样；静态源码路径同属 pthread 后端，但未在 armv7l 动态复跑。
- `-fno-exceptions`、LTO、非 pthread backend、动态插件与自定义 libc 未覆盖。

### 自行判断

- 将“函数定义”作为计数单位，模板 D04 计 1 个定义，不按潜在实例化数膨胀。
- 将 S01 记为“语法差异但行为被共同外层边界遮蔽”，不并入 5 个迁移行为差异。
- 对 POSIX-may 项采用平台源码实证口径；未确认由 glibc 实现为取消点的，不计入 Tizen 当前实数。
- 没有为本轮连接开发板；用户明确允许抽样在 x86_64 原生执行，本轮目标是源码枚举与两库对照。

### 尚存疑问

- libc++ 上游是否把 POSIX forced unwind 视为其 `noexcept` policy 的例外：公开检索未找到直接表态，`NOT_FOUND`。
- 开放世界边界不存在有限、绝对完备的源码清单；因此“5 个实际差异”应理解为本轮明确限定口径内的可证数，而不是所有可能程序的全称命题。

## 9. 过程异常申报

- 三次早期只读搜索范围写得过宽并被终止，退出 130，记录在 `commands/002_inventory.log`、`005_platform_source_inventory.log`、`009_stop_overbroad_find.log`。
- 官方 sourceware glibc clone 返回 403，随后使用 glibc 官方 GitHub mirror 的同一 `glibc-2.40` tag，记录在 `commands/010_*`、`011_*`。
- 第一版消费面扫描按设施重复遍历 371 包，因延迟过高被中止并改为单次组合扫描；记录在 `commands/018_stop_slow_facility_scan.log`。
- 一次生成扩展取消点表的 here-doc 带入多余 `+`，Python 语法错误退出 1；原文保留在 `commands/024_platform_extension_set.log`，修正版在 `025_*`。
- 一次在完整解包 GCC 源中做无界文本检索被终止；已有精确上游来源，未影响结论，记录在 `commands/026_stop_unbounded_gcc_search.log`。
