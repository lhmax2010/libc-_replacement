# 人工确认后的低资源续跑

## 输入、授权与时间

人工已允许在恢复后的 armv7l 构建树继续必要重编。上一轮 PATH 引发
缓存重置的失误及证据完整保留在 REPORT.md，不删除、不改写成环境故障。

本轮重新计时：2026-09-15 15:51:49 +08；截止 2026-09-16 03:51:49 +08。
medium 门禁退出 0，load1=1.94，MemAvailable=9526832 KiB，磁盘可用
320896336 KiB（raw/042–043）。

## 启动前实测门禁

在原构建树实际重跑导出 spec 的完整 CMake 配置参数，而非只阅读旧缓存。
raw/045 和 build-w2c-confirm-config.build.log 保存原始输出与退出码。

```text
FIXED_PATH=/bin:/usr/bin:/sbin:/usr/sbin
CMAKE_CXX_COMPILER:UNINITIALIZED=/bin/armv7l-tizen-linux-gnueabi-clang++
clang version 22.1.8
set(CMAKE_CXX_COMPILER_ID "Clang")
set(CMAKE_CXX_COMPILER_VERSION "22.1.8")
LLVM_ENABLE_LIBCXX:BOOL=ON
COMPILER_AND_LIBCXX_CONFIG_CHECK=PASS
```

实际执行编译器 --version，并从 CMake 生成的 Ninja 命令中核对
IVUsers.cpp 的编译行包含 `-stdlib=libc++`。完整行保留于配置日志。
配置过程与内层退出码均为 0。`llvm_confirm_config.sh` 和 `llvm_resume.sh`
都显式设置 `PATH=/bin:/usr/bin:/sbin:/usr/sbin`，不继承 shell 的 PATH。

## 资源与监控

使用 govern_resume.py，构建 jobs=1、nice=19、ionice class=3。
MemoryMax 请求16536451072字节，实际 memory.max=16536449024字节，
为总物理内存50%按页取整。raw/050 实际检查 scope 内各进程，均为
nice19和I/O idle，内存限额生效；不只是命令行上写了选项。

每500目标探测、30秒超时、冻结后等10分钟、最多3次暂停的逻辑保持不变。
监控器增加的 CMake 缓存重置立即停止门禁保持启用。
构建开始前脚本 SHA256 见 raw/044、047；初始 Ninja 日志快照为
ninja_log_before_confirmed_resume.tsv。

## 最终状态（本次续跑）

LLVM armv7l 已于2026-09-15 19:10:02 +08完成，内外层退出码均0。
写出22个armv7l RPM，其中12个非调试RPM用于本地构建输入；清单与SHA256见
llvm_armv7l_rpms.tsv。raw/090 从最终libllvm RPM提取后实测：
libLLVM.so.22.1依赖libc++/libc++abi/libgcc_s、不依赖libstdc++；
动态符号中std::__1为5646行、std::__cxx11为0行。
这完成的是LLVM输入准备，不是bcc-tools的验证。
bcc-tools armv7l两格已完成，各五次RPM核查通过；结合沿用的其他四格，
六格闭合并推送5bd0654e24bb231035ea684d278f674aa95a2cd2。
bpftrace四格完整构建及源码程序/提供方核查通过，但预编译static子包
内嵌标准库未观测，推送门禁范围待人工确认；该包未提交、未推送。
最终矩阵及限制见FINAL_REPORT.md、MATRIX.md。

中间产物检查：本次第 2351/7146 目标已链接 libLLVM.so.22.1；
raw/069–070 的实际 readelf 输出含 libc++.so.1、libc++abi.so.1、
libgcc_s.so.1，不含 libstdc++.so.6。这只是构建树中间产物检查，
不等于整包构建、RPM 内容核查或 bcc-tools 门禁已经通过。

早期第 270 目标附近有 I/O 等待。追加的三次低优先级探测耗时约
20.45、20.58、18.90 秒，均未超过 30 秒，因此未执行冻结或放宽资源。
第三次另读了当时待编译的实际源文件，以补充“只测新建文件”不能反映
所有源文件读取延迟的局限。原定每 500 目标的探测仍独立执行；
完整构建中第500至7000目标的十四次定期探测均退出0。
未触发连续三次10分钟暂停路径；不将其记录为本轮实测通过。全部原始记录保留。

## 重编量与时间的统计口径

将原801个实际完成目标的日志标签与本次完成标签逐项比对，而非采用
dry-run 的7146推断实际工作量。对匹配的编译对象读取 Ninja 记录的
任务开始/结束时间，给出重编任务耗时合计。
该耗时不是“如果没有发生失误”的精确反事实墙钟差额；生成/链接目标、
配置恢复和I/O暂停分别说明，不能混入一个假精确值。

Ninja 完成 7146/7146 后的实测统计（raw/085）：原先完成的801个目标标签中，
800个在本次再次执行；其中745个编译对象的 Ninja 记录耗时合计1262.828秒
（21.047分钟）。唯一未同标签再次执行的是 `Generating VCSRevision.h`
（raw/087）；不把该项计为重编。与中途统计的799个匹配不同，最终还有
一个匹配目标在后段完成；此处采用完整构建日志，而非中途估计。

统计脚本：measure_rework.py、rework_unmatched.py。
原始 Ninja 日志另存 ninja_log_after_confirmed_build.tsv（raw/086）。
写包退出码与RPM核查在上述“状态”节单独记录，未以重编统计替代产物核查。

## 新增待确认事项（不阻断 LLVM/bcc-tools）

bpftrace 的 Source1002 为预编译二进制 tar，spec 安装为 bpftrace-static，
该子包 %post 还会复制覆盖 /usr/bin/bpftrace。编译参数不会重编这份资产。
已询问本轮四格是否只覆盖源码构建的两个程序、将预编译子包单列未验证，
或要求该子包也完成标准库核查才允许推送。已进一步核查两架构原资产及
四格RPM中的同一资产，逐字节相同；两架构均已strip，动态依赖只显示C库。
未据此推断其内嵌标准库。证据见raw/119–122及checks/bpftrace_*。
