# 条件变量等待被展开打断后的状态核查

## 结论与边界

静态核查确认两处直接的有害状态预约：`src/shared_mutex.cpp:28` 和
`include/shared_mutex:249`。前者由 shared_mutex、shared_timed_mutex 的阻塞写锁共用；
后者由 shared_timed_mutex 的定时写锁及不同 Clock 实例共用。
两处在等待前设置写者位，等待被取消打断后没有清除该位的守卫。

本轮 x86_64 原生实测 6 格，每格 5 次，共 30 次：内部基类、公开 shared_mutex、
shared_timed_mutex::lock，以及 system/steady/自定义 Clock 定时写锁。
30/30 均回收到 PTHREAD_CANCELED、清理计数为 1、底层 mutex 可以重新获取，
但原读者释放后新读者及新写者均获取失败。基类格还直接断言残留写者位为 1。
这不是“线程没崩溃就安全”：业务锁已经不可用。

状态：`PARTIAL`。指定等待链的直接有害点已核查并实测；底层直调 call_once
是否扩围待人工确认，任意模板参数/用户回调不能静态证明，不能把这份表称为
所有用户实例的完整安全证明。W4 的“完整名单”前置条件因此尚未无条件满足。

## 检索与逐点证据

源码快照 HEAD：`5ed6c77278dfa7a470667cf1a137723d3c96fe60`。
范围为 libc++ `src/`、`include/`，包括冻结 C++03 头，共 1792 个 rg 可见文件。
首先检索 wait/wait_for/wait_until/__do_timed_wait/底层 helper；其次检索
__sub_wait、future copy/move、shared base 与定时锁包装；再用可跨行的成员调用
模式交叉核查。完整原始输出见 `raw/002`、`005`、`010`。

人工归属后的 124 行调用及包装在 [calls.tsv](calls.tsv)；每行实际代码与上下文在
[CALL_SITES.md](CALL_SITES.md)。原文件完整快照和 SHA256 分别在 `snapshots/`、
`source_SHA256SUMS`。生成器保留人工分类，不把未知行自动判为安全。
候选中的声明、注释 synopsis 不是执行点；atomic/barrier/latch/stop_token 的
atomic::wait 不属于这四种条件变量入口；底层 pthread/C11/Windows helper
属于实现后端，不将不同后端假定都能执行 POSIX 取消。

正向对照：检索及分类必须同时命中上述 gate2 两行，生成器有断言；运行探针必须
同时确认清理执行、native mutex 可用和业务锁不可用。两项均通过。
没有提出“平台中零个其他问题”的零命中结论。

## 状态分类摘要（静态核查）

| 调用族 | 等待前状态 | 展开时处置 | 分类与限定 |
|---|---|---|---|
| shared mutex 写者 gate2 | 设置 write_entered | unique_lock 仅解内部 mutex；写者位无回滚 | 有残留且有害，两处直接位置 |
| shared mutex 写者 gate1、读者 gate1 | 此调用未设置写者位/增加读者数 | unique_lock 解内部 mutex | 安全；不替别的线程撤销其合法状态 |
| timed_mutex、recursive_timed_mutex（含定时） | 获取业务锁所需 bool/count/id 在等候成功后修改 | unique_lock 解内部 mutex | 对等待分支安全；递归快速路径不经过 wait |
| future 的 ready 等待、copy/move | ready 尚未修改，内部锁已持有 | unique_lock 解锁 | 对内部等候分支安全 |
| future::get | 先置空调用方 future 指针 | unique_ptr/release_shared_count 释放旧引用 | 普通状态的消费残留无害；不是恢复到可再次 get |
| async 最后引用释放 | 引用计数到零，待生产完成再释放 | __on_zero_shared 自身仍 noexcept | 不能声称展开能穿过；不是本轮可回滚的退出后锁残留 |
| condition_variable_any | 复制 shared_ptr、释放用户锁；可注册停止回调 | RAII 解内部锁、重锁用户锁、注销回调 | 重锁成功时安全；任意 Lock 重锁抛出会在 noexcept 析构终止 |
| condition_variable 包装 | 局部时间换算/谓词 | 不管理用户业务状态 | 内部无预约状态；用户谓词与 Clock 副作用无法判定 |
| sleep_until 的自定义时钟分支 | 栈上 mutex/cv | 栈上 unique_lock 解锁 | 内部状态安全；用户 Clock 另计 |
| unique/shared_lock 及通用锁包装 | 完成获取后设置 owns | 由实际 Mutex 方法决定 | 随模板实参传播，不能凭包装存在判定底层安全 |

逐行表中的“安全”都以有效锁、延迟取消、可正常展开的调用帧为前提，不包括异步取消。
冻结 C++03 头仅静态核查，不宣称本轮 C++20 的解除规格覆盖了它们。
deferred future 执行用户函数是另一分支；不能把 ready 等待分支的结论外推给用户函数。
现有四补丁对 future 强制展开的处理也不能等价于消除最后引用释放的 noexcept 边界。

## 实测方法与结果

[matrix.tsv](matrix.tsv) 保留每轮对应原始记录。探针用裸 pthread_create；
工作线程无 noexcept；在 pthread_cond_wait/timedwait/clockwait 入口记录其真实 mutex，
主线程获取该 mutex 后才取消，验证已到达原生等待释放点，而非仅 sleep 猜测。
原始源代码未更改；仅在 tmp 独立副本解除 6 处头文件规格、2 处实现规格，
重新编译 condition_variable、mutex、shared_mutex、future、system_error 并先链接该 DSO。
使用现有修补 ABI 运行库。构建原文、输入/输出摘要在 `build_provenance/`，
构建脚本和解除规格脚本快照在 `snapshots/`；探针 SHA256 在 `raw/013_hashes.stdout`。

| 模式 | 次数 | 干净回收/清理执行 | 内部 mutex 可用 | 新读/写获取 | 直接读写者位 |
|---|---:|---:|---:|---|---|
| 内部共享基类 lock | 5 | 5/5 | 5/5 | 全失败 | 5/5 为 1 |
| 公开 shared_mutex::lock | 5 | 5/5 | 5/5 | 全失败 | NOT_OBSERVED（不修改可见性） |
| shared_timed_mutex::lock | 5 | 5/5 | 5/5 | 全失败 | NOT_OBSERVED |
| 定时 system clock | 5 | 5/5 | 5/5 | 全失败 | NOT_OBSERVED |
| 定时 steady clock/try_lock_for | 5 | 5/5 | 5/5 | 全失败 | NOT_OBSERVED |
| 定时自定义 Clock | 5 | 5/5 | 5/5 | 全失败 | NOT_OBSERVED |

非定时公开锁格使用 try_lock/try_lock_shared；定时格使用 30ms 带超时获取。
它们证明取消后立即重用失败；“永久”的因果依据是源码中没有后续合法持有者清除该位，
而不是有限等待在数学意义上观测了无限时间。损坏对象不执行析构，进程隔离退出。

## 范围疑问

`src/call_once.cpp:47` 直接调用 __libcpp_condvar_wait，不经过指定入口。
它在等待侧持有原生全局 mutex，但未看到包住该等待的解锁守卫；主动执行 func 分支的
exception_guard 建立在等待之后，不能据此证明等待侧安全。该发现已问人工是否扩围；
本轮其取消结果为 `NOT_OBSERVED`，未擅自设计回滚。

引导性的任务读取、仓库定位和若干只读展示命令仅保留在工具会话记录；正式检索、
构建与每轮运行均已逐字落盘。此记录纪律缺口在此申报，不回填伪造的命令退出码。
