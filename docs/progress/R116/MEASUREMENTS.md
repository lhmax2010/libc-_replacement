# 隔离展开器验证记录

## 环境和构建边界

实测架构：**x86_64 原生宿主**，Ubuntu glibc 2.39-0ubuntu8.8；不是 Tizen 系统镜像、物理开发板或 QEMU。Tizen 编译包装器、既有 libc++／libc++abi 构建物用于探针编译及运行，但实际 glibc 身份以上述宿主为准。[身份](raw/051_host_environment.stdout)。

LLVM 源树 `5ed6c77278dfa7a470667cf1a137723d3c96fe60`，只读。llvm-libgcc 使用 Release、显式 opt-in、Tizen x86_64 目标、Ninja 并行度 2；关闭项目测试等无关构建。资源闸门 light 返回 0，构建与运行使用 `nice -n 15`、`ionice -c 3`。[初始配置命令](raw/005_configure.command.txt)、[目标配置](raw/007_configure_target.command.txt)、[最终 Cache](snapshots/CMakeCache.txt)。

构建过程没有隐去失败：初次配置缺默认目标信息；补齐后 GNU 链接器拒绝重复版本节点；切换为 lld 后构建、安装成功。曾在构建失败后尝试安装，安装也失败。所有退出码与 stderr 原样保留于 `raw/005*`、`007*`、`013*`、`016*`、`019*`、`020*`、`022*`。修改仅为隔离目录 CMake 构建参数，不改平台配置。

产物安装至 `tmp/R116/install-native/`，通过每个进程的 `LD_LIBRARY_PATH` 加载。实际物理文件为 `libunwind.so.1.0`，SONAME `libunwind.so.1`，`libgcc_s.so.1` 经别名链指向它；实验库 SHA256：`3b2f537ff57753da9d386468c04cbe3b68885615d3a37611529f368c99396f4f`。[文件／动态节](raw/023_experimental_identity.stdout)。没有覆盖系统 `libgcc_s.so.1`，没有修改 shell 永久环境或平台配置。

GNU 对照提供方是实际宿主 `/usr/lib/x86_64-linux-gnu/libgcc_s.so.1`，SHA256 `d93224d2b0dab4247598be683adca02f5cf00586f99c187579cd7e92058fb7cb`。它不是符号差异表使用的 Tizen sysroot 文件；两者不可混称。

## 正式矩阵与断言

完整 [150 行测量](measurements.tsv)、[30 格汇总](measurement_summary.tsv)、[逐次命令、stdout、stderr、退出码](raw/runs/)。每格恰为 5 次；成功／失败均要求实际提供方身份确认。

`identity.cpp` 在构造期读取 `/proc/self/maps`，打印候选展开器映射，并用 `dlvsym` 与 `dladdr` 确认 `_Unwind_ForcedUnwind@GCC_3.0` 的提供方。LLVM 格同时断言没有检测到其他命名为 libgcc_s／libunwind 的共享展开器；GNU 格是正向对照。该检查不宣称排除任意隐藏静态展开实现。本次自建探针并未故意注入其他静态展开器。

| 每个提供方的格数 | 用例与具体断言 |
|---|---|
| 8 | 保留／移除异常规格 × 普通、system clock、steady clock、自定义 Clock 等待。保留组要求 `event=terminate cleanup_count=0` 且退出 86；移除组要求 join 的取消标记且清理计数 1。 |
| 4 | 无回滚／有回滚 × 普通、定时写者取消。无回滚组断言读者释放后写者位仍为 2147483648、后续 try 获取失败；有回滚组断言回滚 1、状态保留读者而清除写者位、后续真实读写成功、最终状态 0。 |
| 1 | backtrace、dlopen、插件抛出整数 42、主程序 catch、插件局部析构计数 1、dlclose 成功；主程序及插件栈帧各至少 3。 |
| 2 | 修补／未修补 libc++ 与 libc++abi **成对**加载，工作线程在 read 上取消并 catch 后重抛。修补组返回取消标记且捕获／清理均为 1；未修补组 SIGABRT，输出 foreign exception 诊断。 |

合计 15 × 2 × 5 = 150。GNU 和 LLVM 在本矩阵各相同格中结果一致。[分析输出](raw/041_analysis.stdout)。

取消使用裸 `pthread_create`，线程函数及业务调用链没有自行添加 `noexcept`，延迟取消模式；保留规格是被测边界。定时内联路径使用对应头文件重新编译探针，并非仅替换 DSO。写者探针复用已验证的控制／回滚隔离版本，把相关运行库复制到本次隔离目录；未改写平台库。[构建脚本](code/build_probes.py)、[执行脚本](code/run_probes.py)、[等待源码快照](snapshots/condition_cancel_probe.cpp)、[写者源码快照](snapshots/shared_probe.cpp)。

保留规格探针安装自定义 terminate handler，退出 86 是人为观测标记，不是操作系统信号编号。未修补重抛组的 `.exitcode` 为 Python 记录的 `-6`，表示 SIGABRT。命令设置 core 文件大小为零；timeout 的 stderr 仍可能出现 “dumped core” 文案，不能把文案当成已产生 core 文件。

`run_probes.py` 对重抛格先标记 OBSERVED，不预设结局；后续人工核对与汇总记录其结果，不将这些格的脚本状态等同于已预设的预期匹配。其余格直接检查值／状态。**损坏对照的退出 0 是断言损坏符合观测预期，不是库行为正确。**

## smoke、配对失败和覆盖限制

正式矩阵外的 smoke 单列，不计入 150 次。曾只复制未修补 libc++abi，与修补 libc++ 混配导致加载失败（缺 forced-unwind 类型信息）；随后复制同构建的 libc++ 配成完整基线，才形成正式未修补矩阵。[失败](raw/027_rethrow_smoke.stderr)、[补齐配对命令](raw/028_complete_baseline_pair.command.txt)、[重新 smoke](raw/029_pair_rethrow_smoke.stderr)。加载失败进程无法进入探针身份检查，不能作为展开行为证据。

本次没有测试 ARM／aarch64 实验展开器、实际 Tizen glibc 镜像、全部取消站点、异步取消、生产插件、旧外部二进制、JIT 动态帧登记、跨命名空间加载、信号栈回溯、所有普通错误路径及性能。写者实例不能证明任意用户 Clock、模板参数和回调。

## 符号与 ARM 静态核查

[符号比较](symbol_comparison.tsv)统计动态符号表中已定义 FUNC／IFUNC／OBJECT 的唯一 `(名称, 版本)` 对，未带版本者记 `UNVERSIONED`。Tizen x86 sysroot 198 对、实验库 231 对，前者独有 108 对。它不是库文件总符号数量，也不是已证明会失败的消费者数量。例：有的同名函数仍存在但不带原版本；`__register_frame_info` 等接口也需单独区分。glibc 的六项 x86 入口存在，并不能冲销这些差异。

ARM 只做目标宏、版本脚本预处理、已有 sysroot ELF 静态核查。检测到 `__ARM_PCS` 而非 `__ARM_PCS_VFP`；版本脚本的 ARM 条件只检验架构／EABI／浮点指令能力等，不检查浮点参数调用约定。已有 ARM DSO 的本次 `readelf -A` 未输出属性，属性值记 `NOT_OBSERVED`，不能补全。ARM 实际实验库 ABI 和强制展开结果未测。[宏](raw/034_arm_macros.stdout)、[预处理](raw/035_arm_version_script.stdout)、[ELF](raw/053_local_arch_metadata.stdout)。

## 可复核与清理

自编脚本在 `code/`，复用源码、配置和上游关键实现文本在 `snapshots/`；[来源表](snapshot_origins.tsv)与最终 `SHA256SUMS` 记录身份。构建命令、实际退出码和失败原文保留，不把本地绝对依赖路径伪装成可独立运行的发布包：重现需取得对应只读源码、工具链及既有运行库；清理后原命令路径不再存在。

完整 Chromium 归档 SHA256 为 `0f8ee6bdc0ace963f33e9bd7864d13e9c0bbaca83fb4faead0ebb4b61c97711b`，用于本次固定版本扫描，不将 1.4 GB 归档提交。原始下载失败与完整下载分开保存；不足 512 MB 的第一次下载不用于完整性结论。

清理事实与最后核验以 [交接记录](HANDOFF.md) 为准。系统库、源码和配置不在清理目标内。
