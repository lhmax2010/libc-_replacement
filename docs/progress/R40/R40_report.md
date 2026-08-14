# R40：S1 实现的两项遗留问题

## 结论摘要

本轮未修改平台包源码；全部 libc++abi 改动均位于 `tmp/R40/` 的实验副本。

第一部分结论为 **LIBCXXABI_LIBGCC_EHABI_INTERACTION**：armv7l 在
`__cxa_rethrow` 入口处实测 `unwinder_cache.reserved1` 为非零，故
`_Unwind_Resume_or_Rethrow` 没有退化为 `_Unwind_RaiseException`。
该 API 进入 libgcc 的 forced phase 后，libc++abi EHABI personality
先返回一次 continue，随后在原 handler 帧再次得到 handler found，最终
`libc++abi: terminating`、退出 134。失败不是 reserved1 被清零，也不是
第三处改动未被调用；直接可观测形态是「保留强制展开状态后，EHABI
personality 再次识别同一 typed handler」。本轮未实现修法。

第二部分两种实验方向在消费者**均不加 `--export-dynamic`**时，四个
「类型映射」格都实际命中 `cancel:forced-handler`：

- 方向甲（key function / libc++abi 强定义）在两架构均生效。消费者
  对 RTTI 是 UND，动态绑定到 libc++abi 的 GLOBAL 定义，库侧具有唯一
  强拥有者。
- 方向乙（显式 default visibility）在两架构也生效，但 RTTI 仍是弱
  定义；动态链接器把 libc++abi 的引用绑定到主程序中的弱定义。该方向
  实测解决了匹配，但没有形成「库内唯一强定义」。

两方向的 armv7l cancel 格仍在独立的 EHABI 重抛问题处退出 134；真实
C++ 异常格均退出 0。这里不把「类型映射生效」误报成「S1 整体完成」。

## 1. 输入身份与边界

上游基线为 `upstream/22.1.8` peeled commit
`ca7933e47d3a3451d81e72ac174dcb5aa28b59d1`。R40 从 R34-B 三处 S1
实验改动复制出 diagnostic、strong、visibility 三个独立副本。基线和
输入文件 SHA256 见 `tables/source_identity.tsv`。

平台 `codes/llvm` 未改；`git status --short` 仅保留任务开始前已有的
四组无关未跟踪项。板端未安装 RPM，运行时仅用 `LD_LIBRARY_PATH`
指向 `/var/tmp/r40_s1_20260814`。

## 2. 资源门禁

独立脚本 `tools/resource_gate.sh` 经 `bash -n` 和干跑验证。调整后判据为
可用内存低于 8 GiB 返回 20、load1 大于 `0.8*nproc` 返回 21；仅负载
失败可按十分钟间隔最多三试。开工时 `nproc=20`、可用内存约 20 GiB、
load1=14.21，低于阈值 16；首试通过。所有 configure/build/probe link
前均显式检查门禁退出码，构建用 `CMAKE_BUILD_PARALLEL_LEVEL=2`、
`LLVM_PARALLEL_LINK_JOBS=1`、`nice -n 15 ionice -c 3`。六个 runtimes-only
配置均未启用 `LLVM_ENABLE_PROJECTS`，构建退出 0。构建时间短于五分钟，
监控脚本仍记录了首末资源采样，未触发 2 GiB 终止线。

旧 0.5 阈值红项、一次封装参数遗漏及部署载体试错均完整保留，见
`tables/command_anomalies.tsv`；这些记录均未越过门禁启动构建，或未形成
有效矩阵观测。

## 3. 第一部分：armv7l 重抛定位

### 3.1 静态路径

`codes/gcc/libgcc/unwind-arm-common.inc:77-80` 将 forced stop function
映射到 `unwinder_cache.reserved1`、参数映射到 `reserved4`；
`__gnu_Unwind_ForcedUnwind` 在 645-655 行设置两字段并进入 forced phase。
`__gnu_Unwind_Resume_or_Rethrow` 在 714-723 行仅当 reserved1 为零才调用
RaiseException，非零则直接调用 `unwind_phase2_forced`。对该文件所有
字段使用点的全文检索未发现清零赋值。

libc++abi EHABI personality 读写的是 `barrier_cache`，未写
`unwinder_cache.reserved1`。其 forced phase 在
`cxa_personality.cpp:1283-1307` 调用 `scan_eh_tab`，并把
`_URC_HANDLER_FOUND` 安装为 landing context。完整原文与 file:line 在
`commands/010_static_analysis_arm_rethrow.log`。

因此从 `_Unwind_ForcedUnwind` 到 catch 中 `__cxa_rethrow`，已检索代码
范围内没有 reserved1 清零路径；运行时观测进一步证实其非零。

### 3.2 观测方法及不改变控制流说明

板上没有 gdb/gdbserver/lldb/strace/ltrace，故按任务许可在实验副本加入
只读打印：

- `cxa_exception.cpp` 在调用 `_Unwind_Resume_or_Rethrow` 前打印 UCB
  地址及 reserved1/reserved4；
- `cxa_personality.cpp` 打印 EHABI state、force 标志、SP/barrier SP 和
  `scan_eh_tab` 返回码；
- 仅把既有 noreturn 调用的返回值存入局部变量，并在它异常返回时打印，
  未增加分支、未写异常对象、未改变调用参数。

加入 personality 打印前后的 arm 基准格都命中 forced-handler 后退出
134；x86_64 基准格前后均退出 0，说明观测打印未改变已观察行为。
diff 见 `patches/diagnostic.diff`，完整文件见
`code/implementation/diagnostic/current/`。

### 3.3 运行时事实

armv7l 真机原生输出的关键顺序是：首次 forced scan reason=6、进入
`forced-handler`、`reserved1=0xb6e2fdf8`（非零）、重抛后的 scan
reason=8、再次在原 SP 得到 reason=6，随后 terminating、SIGABRT。
`R40_RESUME_OR_RETHROW_RETURN` 从未打印，因此 API 没有返回错误码。

x86_64 宿主原生目标用户态对照在 rethrow 时 `private_1` 同样非零，
随后 join=PTHREAD_CANCELED、退出 0。逐项数据见
`tables/runtime_state.tsv`，完整输出分别为
`commands/056_run_diag_arm_cancel_personality.log` 与
`commands/049_run_diag_x86_cancel_return.log`。

### 3.4 责任边界

libgcc ARM 侧实测接收到非零 stop function 并进入其源码规定的 forced
phase；失败发生在该 forced phase 再次调用 libc++abi EHABI personality
后。libc++abi 的新增 typed mapping 又把该帧识别为 handler。故现有证据
把直接原因定位为 **libc++abi typed mapping 与 libgcc ARM forced-resume
遍历语义的交互**，不是单方字段清零。停止函数在更深层的精确调用轨迹
因板上无调试器而 `NOT_OBSERVED`；本轮没有据此推断或实现第四处改动。

## 4. 第二部分：typeinfo 可见性

### 4.1 既有默认差异

旧 x86_64 R34-B 测试件在未显式添加 `--export-dynamic` 时，RTTI 已以
WEAK DEFAULT 进入 `.dynsym`；旧 armv7l R38 strict 测试件的相同 RTTI
只在 `.symtab`，`.dynsym` 零命中。两者链接命令均未出现
`--export-dynamic`。原文见 `commands/106_prior_default_typeinfo_visibility.log`。

所以 x86_64 的既有 `S1_COMPLETE` 的确带有此前未显式申报的前提：该
测试件/工具链组合默认把 RTTI 放入动态符号表。此处只陈述实测差异，
未把其归因到某个未观测的链接器内部规则。

### 4.2 方向甲：库侧强 RTTI

实验改动把空类改为带非内联虚析构函数的类，并在
`cxa_exception.cpp` 定义该 key function。结果：两架构 libc++abi 的
typeinfo/typeinfo-name 均为 GLOBAL DEFAULT；两架构消费者均为 GLOBAL
DEFAULT UND，且不使用 `--export-dynamic`。`LD_DEBUG=bindings,symbols`
实测消费者和库内部均绑定到 libc++abi 定义。

x86_64 cancel/real 均退出 0；armv7l real 退出 0，cancel 先命中
forced-handler，后在第一部分的重抛交互处退出 134。故方向甲的
「无需消费者链接选项即可匹配」为 `EFFECTIVE`。

该实验方向改变了新类型的类布局（增加虚表指针）并新增析构、vtable、
RTTI 等导出表面；这些是实验事实，不构成方案评价。

### 4.3 方向乙：显式 default visibility

实验仅把类标记为 `__attribute__((visibility("default")))`。两架构库和
消费者仍各自生成 WEAK DEFAULT RTTI，但消费者无需
`--export-dynamic` 即进入 `.dynsym`。动态绑定原文显示 libc++abi 的
RTTI 引用被主程序定义抢占，双方因而使用同一运行时地址。

行为矩阵与方向甲相同：两架构均命中 forced-handler；x86_64 cancel
退出 0，armv7l cancel 在独立重抛问题处退出 134；real 四格均退出 0。
故方向乙对「无需消费者链接选项即可正确匹配」也是 `EFFECTIVE`，但它
没有把静态定义收敛为库侧唯一强定义。

完整符号形态在 `commands/079_typeinfo_symbol_inventory.log`，动态绑定
在 `commands/104_*_typeinfo_bindings.log`、
`commands/105_*_typeinfo_bindings.log`，矩阵在
`tables/typeinfo_matrix.tsv`。

## 5. 构建、运行与板端清理

所有测试件完整源码沿用 R34-B 的 `s1_rethrow_probe.cpp`；probe 构建脚本
逐项打印编译/链接选项。x86_64 动态链接 libc++.so；armv7l 因板上无
libc++.so，使用既有静态 libc++.a 与候选动态 libc++abi。第二部分所有
probe 的 `EXPORT_MODE=none`。每格均有 `ldd`/loader list、SHA256、
`LD_DEBUG=libs` 完整输出。

SDB 原生 push 生成的条目因 Smack 标签不可访问，四个退出 126 的无效格
已单列并排除；有效传输通过板端 shell 建立 `User::Shell` 标签文件。收尾
前板端项目进程为 0；随后删除精确临时目录
`/var/tmp/r40_s1_20260814`，核验路径不存在且项目进程仍为 0。证据见
`commands/107_board_cleanup_files_and_processes.log` 和
`tables/board_cleanup.tsv`。

## 6. 未观测项与结论边界

- libgcc/glibc stop function 的逐指令调用轨迹：板端无调试器，
  `NOT_OBSERVED`；已有打印足以排除 reserved1 为零。
- 两个 typeinfo 方向只覆盖 x86_64/Itanium 与 armv7l/EHABI、libgcc_s
  unwinder；未测试 LLVM libunwind。
- 第二部分验证类型匹配与一个真实异常对照，不扩展为完整 S1 回归矩阵。
- armv7l 的 134 是第一部分已独立存在的重抛问题，不能用于否定两种
  typeinfo 方向的匹配有效性。

本报告仅陈述实测事实，不做方案推荐，不修改平台包，不提交上游。
