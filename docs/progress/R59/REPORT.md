# R59 四项代码缺陷的定位与修复设计

## 结论摘要

四项均成立。三方 x86_64 原生矩阵为：F1 `86/0/0`、F2 `5/134/0`、M1 `6/134/0`、M2 `86/86/0`（顺序为当前全补丁版/未打补丁 libc++/libstdc++）。完整定义和原始输出见 `tables/defect_verdicts.tsv` 与 `raw/matrix/`。

- F1：补丁引入的明确回归。
- F2：typed 分支新增后产生的状态提交缺陷；未打补丁版另有吞取消并 abort 的既有问题。
- M1：typed 分支新增后产生的永久非就绪形态；未打补丁版另有吞取消并 abort 的既有问题。
- M2：缺陷成立，但三方对照显示相同失败在未打补丁 libc++ 已存在；当前补丁保留了该问题，不是相对基线的新回归。

本任务未修改四个补丁、平台源码或任何运行库。

## 输入与身份

当前代码取自 `docs/progress/R58/code/current/`，未打补丁基线取自 `docs/progress/R58/code/baseline/`。三份关键当前文件 SHA256：

- `ios.cpp`: `6734c4d2fbe5b9708c318a083e41f74633010a17a850c411dacd126e0b6964a4`
- `istream`: `4111af1754590455948aaa3e14a90a742445e852485322771bb923c80281bd7c`
- `future`: `80a837cdda19397af933d28f1ed611e9bf27504df2b17fd645f685bef9ac6a9d`

基线 SHA、运行库 SHA、`ldd` 与加载记录分别见 `raw/001_source_sha256.txt`、`raw/identity/sha256.txt`、`raw/identity/ldd_current.txt`、`raw/identity/ld_debug_current.log`。构建命令逐字保存在 `raw/build/*.log`。

## 独立复核

### F1

当前 `ios.cpp:363-388` 的两个 helper 都先在嵌套 try 中重抛并捕获，再在 helper 尾部按掩码裸重抛。自定义 `exception_class` 的非强制外来异常探针设置 `exceptions(badbit)` 后：当前版先打印外来异常清理，再进入 terminate handler（86）；基线和 libstdc++ 均进入顶层 catch、`badbit=1`、退出 0。评审推演与实测一致。

### F2

当前 `istream:797-800` 只执行 `__state |= badbit; throw;`，`istream:807` 的 `setstate` 不可达。实测当前版线程取消和 join 均成功，但 `rdstate=0/bad=0/fail=0`，探针以 5 报错；libstdc++ 为 `rdstate=1/bad=1/fail=1`。基线在 catch-all 吞取消后以 134 结束，无法形成正确状态对照。修复形态由相邻 handler 与 libstdc++ `istream.tcc:414-417` 共同支持。

### M1

`future.cpp:113-121` 在执行 deferred callable 前清除 deferred 位并解锁；当前 `future:793-794,826-827` 捕获后只重抛，没有恢复标记或完成状态。实测第一等待线程被取消后，第二等待线程超时，callable 调用次数仍为 1（退出 6）。libstdc++ 的 `_M_set_result(..., true)` 依赖 `call_once`，异常退出不会完成 once；参考实测第二次执行 callable 并返回 42。基线因吞取消退出 134。

### M2

当前 `future:858-864,902-908` 在 typed handler 中先构造 broken-promise 异常并调用 `set_exception`，最后才重抛；`future.cpp:80-82` 表明状态已有值时会抛 `promise_already_satisfied`。确定性窗口探针在 `set_value` 已完成、外层 try 尚未退出时发起异步取消：当前版和基线均进入 terminate handler（86），libstdc++ 传播阶段存活，等待方收到 broken promise，退出 0。

调试器原始记录 `raw/m2_gdb_trace.log` 直接显示当前版依次抛出 `broken_promise`，随后 `set_exception` 从 `future.cpp:81` 抛出 `promise_already_satisfied`，末尾原重抛未执行。

## 复现夹具校正记录

为避免把测试件缺陷冒充产品结论，保留了三轮无效/不完整 M2 记录：

1. `raw/matrix_initial_invalid_m2/`：链接器 wrapper 无法截获共享库内部建线程，线程句柄无效。
2. `raw/matrix_m2_noexcept_invalid/`：窗口对象析构默认 `noexcept`，三方均因该帧终止。
3. `raw/matrix_m2_uncaught_result/`：libstdc++ 已完成传播，但主线程未捕获 `future.get()` 的普通 broken-promise 异常，导致结果读取阶段 terminate。

最终夹具把析构显式设为 `noexcept(false)`，并只在主线程结果读取处记录普通异常；被取消线程中没有新增 catch。最终结果只取 `raw/matrix/`。

## 同类问题检查

检查范围为补丁实际改动的 `istream`、`future`、`ios.cpp`，共 23/9/2 个 typed handler。结果见 `tables/same_class_review.tsv`：没有发现评审四项之外的新同类站点。这里的“未发现”只覆盖上述三个文件，不外推到整个 libc++ 源码树。

## 修复设计与验证盲区

F1/F2 的具体修复文本及 M1/M2 待裁决选项见 `DESIGN.md`。三条既有盲区及新增的 M2 竞态格见 `tables/blind_spot_regression_design.tsv`。这些格均设计为可重复脚本，后续升版门禁应检查语义结果与状态，不再只检查进程退出码。

## 自行判断与尚存疑问

- 自行判断：M2 探针必须把窗口对象析构声明为 `noexcept(false)`；否则测到的是语言层 terminate，而不是共享状态问题。该判断由“三方均 86”反证并已保留原始证据。
- 自行判断：在主线程捕获 `future.get()` 的普通异常只用于区分“worker 传播失败”和“等待方收到错误”，不改变 worker 的取消路径。
- 尚存疑问（待人工裁决）：M1 采用重试、永久错误还是取消屏蔽策略。
- 尚存疑问（待人工裁决）：M2 采用原子无抛完成、隔离完成异常还是施加可验证的取消限制。
- 尚存疑问：M1/M2 的最终策略应定义到何种跨实现一致性；ISO C++ 未规定 pthread 取消进入 future 状态机后的结果。

## 未覆盖范围

- armv7l、aarch64；本任务按要求只做 x86_64 原生。
- 多等待者同时竞争 deferred 重试的状态机压力测试。
- M2 在不同优化级别、LTO 和其他 ABI 运行库下的竞态窗口。
- 内存分配失败时“完成共享状态”接口的行为。

## 纪律符合性

资源门禁 light 通过；只编译独立测试件，未重建 libc++/libc++abi；未连接开发板；未修改正式补丁；未推送外部源码仓。命令与退出码见 `commands/`。`003_source_context.command.txt` 如实登记了一次未在调用时保存命令原文的证据缺口，未重建后冒充原件。
