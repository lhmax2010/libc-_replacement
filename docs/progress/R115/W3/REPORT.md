# armv7l 物理板取消矩阵与 EHABI 核查

## 结果

本轮 **armv7l 物理板**完成 21 格 ×5 次取消矩阵，x86_64 原生用相同来源探针完成
对应 21 格 ×5 次；逐格行为一致。另以 GCC/Clang 各 5 次最小调用链在物理板核查
CANTUNWIND。不是 QEMU 用户态运行验证；QEMU 只用于主机执行 ARM 编译工具。

`shared_mutex` 与 `shared_timed_mutex` 的写者取消后损坏在物理板各 5/5 复现。
`-fno-exceptions` 旧调用方并未如评审推测那样必然中止：回收为 PTHREAD_CANCELED，
但 C++ 锁清理没有执行，底层 mutex 残留，其他线程获取超时。

## 板卡、构建与对照

- 板卡实测型号 Raspberry Pi 4 Model B Rev 1.5，uname 返回 armv7l、6.12.80-arm-rpi4-v7l，glibc 2.40。
- SDB 地址 `192.168.108.26:26101`。devices 初始为空；connect 成功，随后实际 shell 身份确认，不能仅凭 connect 的退出码判为可用。
- 使用原有 LLVM/Clang 22 工具与 GCC 14.2.0，C++20、O0、禁内联；R111/R112/R113 探针来源快照在 `snapshots/`。
- 保留组用原头和原库入口；解除组在 tmp 覆盖头上精确删除 6 个规格、实现上删除 2 个规格。
  两个头内 timed 变体通过重编用例生效；不改平台源码或四补丁。
- 解除组额外 DSO 重编 condition_variable、mutex、shared_mutex、future、system_error，
  先链接覆盖 DSO，再使用现有修补 ABI 库；不是将“原来的 noexcept 调用方”错误地重编成新调用方。
- R112 旧调用方的 .o 用原头编译，只换链接对象；新调用方控制组用解除头重编。
- 物理板没有 timeout 命令。初次 smoke 远端实际退出 127，测试未运行；随后仅在独立构造函数设置 alarm(8)，
  不增加工作线程的调用帧或清理，不更改其异常表。该失败保留在 raw/011。
- 远端 SDB 通常返回 host exit=0；每次远端命令额外打印真实 `REMOTE_EXIT`，表使用后者。
  每次运行设置 core 大小为零；连接未中断，未使用失败重试额度。
- 动态库实际加载检查及板上 SHA 在 raw/024；最终运行二进制/DSO 的板上摘要在 raw/056，
  主机摘要在 raw/061。未将 sysroot 内 libgcc_s 冒充板载文件：两者 SHA 不同，已明确记录。

## 三个矩阵

完整逐格见 [ARCH_COMPARISON.md](ARCH_COMPARISON.md)，逐轮表为
`matrix_armv7l.tsv`、`matrix_x86_64.tsv`、两个 `supplement_*.tsv`。

| 形态 | x86_64 原生（每格 5 次） | armv7l 物理板（每格 5 次） |
|---|---|---|
| 保留规格：wait / system / steady / custom | 四格全部 terminate 标记；清理计数 0 | 同左 |
| 解除规格：wait / system / steady / custom | 四格全部 canceled、清理计数 1 | 同左 |
| 原头、有邻近调用、解除库 | canceled；C++ 清理与真实解锁均 1；trylock 成功 | 同左 |
| 原头 quiet、解除库 | canceled；清理/解锁均 0；native trylock=EBUSY；另一线程 timedlock=ETIMEDOUT | 同左 |
| 原头 -fno-exceptions（有调用/quiet 两格） | 均为上述锁残留 | 同左，不是 abort |
| 原头、保留库（补充控制） | terminate 标记，exit86 | 同左 |
| 新头、新库（有调用/quiet 两格补充控制） | canceled、清理/解锁均 1，另线程可获取 | 同左 |
| timed_mutex | canceled、清理执行，业务锁可重用 | 同左 |
| shared_mutex 基类写者 | canceled、内部 mutex 已解锁、写者位仍 1；新读/写线程都超时，随后取消回收 | 同左 |
| shared_timed_mutex 定时写者 | canceled，原读者释放后新读/写定时获取都失败 | 同左 |
| future.wait | canceled，随后 promise 可置值，future 取到精确值 42 | 同左 |
| condition_variable_any 普通锁 | canceled、用户 mutex 可获取，再次定时等待可用 | 同左 |
| condition_variable_any 重锁故意抛异常 | terminate 标记 | 同左 |

terminate 探针安装了标记处理器并 `_Exit(86)`，因此这里没有把 86 描述成原生 SIGABRT。
shared_timed_mutex 的写者位沿用原探针以合法读者持锁时新增读者失败作间接观察，
未直接访问私有位；它的内部 native mutex 状态本格 `NOT_OBSERVED`。
shared_mutex 格直接访问的是库的 `__shared_mutex_base`，不是篡改公开类的私有可见性。
这些测法区别保留，不能把一个格的直接内省外推到另一个格。

R113 原探针对 timed_mutex/future/普通 cv_any 使用短暂等待辅助确认，非逐一跟踪底层等待入口；
本轮忠实复跑该方法，未把它写成内核栈精确定位。共享锁两格有第二阶段状态验证。
表内 reusable 等数值辅助列是 marker 的出现统计；原始值断言才是依据，marker 不出现不能代替负向观测。

## CANTUNWIND：对象、最终 ELF 与实测分开

EHABI 是 ARM 的异常处理二进制约定；`.ARM.exidx` 是函数地址区间与展开描述的索引。
`CANTUNWIND` 表示对应区间没有可用的展开描述，不等于“运行中必然 abort”。

| 样本 | 对象文件静态核查 | 最终 ELF 静态核查 | armv7l 物理板实测 |
|---|---|---|---|
| Clang 旧调用方 -fno-exceptions | worker 为 CANTUNWIND | 索引合并为覆盖 worker 的 CANTUNWIND 区间；worker 地址另以符号表对应 | 有邻近/quiet 各 5/5 canceled，锁残留 |
| GCC 14.2.0 最小无异常工作函数 | 未生成展开节，不称作“编译器已经发出 CANTUNWIND 对象记录” | 链接后 worker 起点 0x964 为 CANTUNWIND | 5/5 canceled；SIGABRT marker 未出现 |
| Clang 22 同源最小工作函数 | 明确 CANTUNWIND 项 | 链接后 worker 起点 0x964 为 CANTUNWIND | 5/5 canceled；SIGABRT marker 未出现 |
| GCC -fno-exceptions 加 -funwind-tables 控制 | 有实际展开指令 | 本控制只检查对象 | NOT_OBSERVED（未作为额外运行格） |

最小工作函数只发布已进入状态并循环 pthread_testcancel；main 用裸 pthread_create、
cancel、join 断言回收值，另设 SIGABRT 标记处理器。
GCC 样本用实际 cc1plus 生成汇编，再用 ARM 汇编器组装；其默认生成兼容 ARMv5T 的代码，
在 armv7l 物理板执行。它不是把 Clang 产物称作 GCC 产物；汇编快照和命令均保存。
该最小函数无 C++ RAII，故不能凭“正常回收”宣称 C++ 清理完成；RAII 的对照来自上面的旧调用方矩阵。

本地 GCC 源码 `libgcc/unwind-arm-common.inc:342` 将 CANTUNWIND 映射为 END_OF_STACK；
强制展开循环在取不到条目时给 stop 函数添加结束标记。该源码提供与实测相符的机制，
但板载 libgcc_s 和构建 sysroot 的摘要不同，因此没有声称已完成板载二进制与该源码逐指令对应。
“libgcc 可能停止继续展开”与“glibc 必然 abort”不是同一论断；后一论断不符合这些实测格。
静态输出见 raw/017、023、034、055；最小运行见 raw/cant_runs。

## angle 的原快照产物

从 R98 原始元数据定位并取得 `angle-2.1-1.armv7l.rpm`；不是换用最新仓库。
快照为 `tizen-unified-toolchain_20260829.015247`，源码版本记录
`f2bdc8655536088e3003ac1537b3bed3fcdbd23f`。
RPM SHA256 与原元数据完全相同：
`90347018842fcf93fd649798499118254850be133b8941d34ff4e267d99b21f7`。

构建日志同时观察到 g++、`-fno-exceptions`、`-funwind-tables`。
实际 ELF 的结果如下（**静态核查，不是 angle 工作负载取消实测**）：

| ELF | exidx 项数 | CANTUNWIND 项数 | 其余项数 |
|---|---:|---:|---:|
| libEGL_angle.so | 86 | 2 | 84 |
| libGLESv1_CM_angle.so | 4 | 1 | 3 |
| libGLESv2_angle.so | 9348 | 2 | 9346 |
| libfeature_support_angle.so | 36 | 2 | 34 |

索引项是地址区间，不是源码函数数量；CANTUNWIND 可能含链接生成的边界/尾部区间。
未据此认定 angle 的某个业务线程会经过哪一项。每项地址与解码原文保存在 raw/angle，
摘要见 [angle_exidx.tsv](angle_exidx.tsv)。四个库是该 RPM 的实体 ELF；其他三个名字为链接。

## armv7l libstdc++ 后端

平台 GCC 14.2.0 头的实际预处理输出：

```text
_GLIBCXX_USE_PTHREAD_RWLOCK_T = 1
_GTHREAD_USE_MUTEX_TIMEDLOCK = 1
_GLIBCXX_USE_PTHREAD_RWLOCK_CLOCKLOCK = 1
__GLIBCXX__ = 20240801
```

包含 shared_mutex、断言上述选择和 sizeof 与 pthread_rwlock_t 相同、访问 native_handle
的探针编译成功。证据 raw/040、041，源码 code/backend.cpp。
因此对这份 armv7l 开发头，shared_mutex 和 shared_timed_mutex 都选择 pthread 后端；
不是条件变量 fallback。该结论不能推广到任意外部旧编译产物。

## 验证纪律、未完成项与清理

资源门 medium 通过。每格使用进程隔离，没有只检查退出码。
自动核验共 210 次矩阵运行及 10 次最小 CANTUNWIND 运行。
第一次核验脚本误以为 shared_timed 格会打印 reusable=0，实际原探针打印 reader_acquired/writer_acquired；
已按原始字段修正核验脚本，不改探针、不覆盖运行结果，失败保留 raw/060，重验 raw/062。

本任务状态：`MEASURED_WITH_LIMITATIONS`。要求的矩阵、GCC/Clang/angle 异常表及开发头后端均已取得证据；
不包含 angle 真实业务取消路径，不声称覆盖所有优化等级、编译器组合或板载 unwinder 的源码映射。
本轮未做 aarch64，也未做 W4 回滚实验。

开工前 ps 与本任务候选目录检查有记录；收尾 ps 正向对照可见 ps 自身，未见本轮探针进程。
板上 `/var/tmp/r115_20260907` 和 4 个同名传输包已清除，路径不存在的检查通过。
删除的是本轮上传的实验副本，可从主机 tmp/R115-W3 与已保存构建脚本重建；未删除其他任务材料。
脚本/来源快照 SHA 见 raw/059，最终验证脚本 SHA 在后续交接摘要。

完整 `git diff --cached --check` 报退出 2，原因包括原始远端 CRLF、readelf 原文的行末空白及
CSV 默认 CRLF；这些记录未为通过格式检查而改写。手写代码与 Markdown 的范围检查通过
（raw/067）。不把完整检查报告写成通过。
