# R113：第二轮评审四项新发现核实记录

## 1. 范围与结论总览

本轮只验证论断，不修改平台源码或四个补丁，不实施方案，也不给处置建议。
静态源码身份为 libc++ `5ed6c77278dfa7a470667cf1a137723d3c96fe60`、
libstdc++ `49c5060508ce8a6ed6beca375233688a6bdd9140`。运行实验均为
x86_64 原生；要求使用 armv7l 物理板的核查因板卡不可连接而为
`NOT_AVAILABLE`。

| 核查项 | 判定 | 证据性质 |
|---|---|---|
| `shared_mutex` 写者取消后保留写入标记 | **成立**；`shared_timed_mutex` 定时写者路径也成立 | libc++/libstdc++ 静态核查 + x86_64 原生实测 |
| armv7l 原生取消矩阵与 `.cantunwind` 推测 | **NOT_AVAILABLE**；物理板三次连接失败 | 连接实测；未以 QEMU 替代 |
| 双版本不能保护重新链接的旧 `.o`/`.a` | **成立** | 真实完整 libc++ 双版本实验 + x86_64 原生实测 |
| 四处文字准确性 | 两处需收窄、一处原口径错误、一处源码论断成立 | 静态核查 + 既有实验边界复核 |

`NOT_OBSERVED` 表示本轮未运行或未观察；`NOT_AVAILABLE` 表示完成指定核查所需的
环境或材料不可得。

## 2. `shared_mutex` 写者取消后的状态

### 2.1 libc++ 源码核查

`codes/llvm/libcxx/src/shared_mutex.cpp:22-29` 的排他锁路径是：

```cpp
void __shared_mutex_base::lock() {
  unique_lock<mutex> lk(__mut_);
  while (__state_ & __write_entered_)
    __gate1_.wait(lk);
  __state_ |= __write_entered_;
  while (__state_ & __n_readers_)
    __gate2_.wait(lk);
}
```

函数中没有清除 `__write_entered_` 的 RAII 守卫或 `catch`。正常 `unlock()` 会把
全部状态清零；但被取消的写者尚未取得排他所有权，不会调用 `unlock()`。

`shared_timed_mutex::try_lock_until` 位于
`codes/llvm/libcxx/include/shared_mutex:233-260`。它同样先设置写入标记再等待
`__gate2_`。只有收到正常 `timeout` 返回时，253 行才清标记并通知 `__gate1_`；
异常展开路径没有对应恢复代码。源码原文见 `raw/002_*`。

### 2.2 libstdc++ 对应实现

libstdc++ 有两种后端：

- `pthread_rwlock_t` 后端不使用上述写入标记状态机；本机头文件宏实际选择此后端：
  `_GLIBCXX_USE_PTHREAD_RWLOCK_T=1`、`_GTHREAD_USE_MUTEX_TIMEDLOCK=1`。
- 条件变量 fallback `__shared_mutex_cv` 使用与 libc++ 同源的算法：
  `_M_gate1.wait` 后设置 `_S_write_entered`，再在 `_M_gate2.wait` 等待；没有异常
  回滚。定时路径也只在正常超时分支清标记并通知。

因此，论断在“条件变量状态机实现”层面是两库共有；在当前 x86_64 libstdc++
实际选择的 pthread rwlock 后端上不存在该内部标记。armv7l 平台最终使用哪个
libstdc++ 后端本轮没有物理板或对应预处理结果，记 `NOT_OBSERVED`。证据见
`raw/003_*`、`raw/012_*`。

### 2.3 x86_64 原生实测

实验库重编了解除四处条件变量异常规格后的 `condition_variable.cpp`，并用同一
新头重编 `mutex.cpp`、`shared_mutex.cpp`、`future.cpp`；探针的 `DT_NEEDED`
顺序和符号定义见 `raw/010_*`。所有用例使用裸 `pthread_create`、延迟取消、
底层 mutex 状态或带时限的竞争者断言，每格 5 次。

| 设施 | 线程回收与帧清理 | 取消后对象状态 | 5 次结果 |
|---|---|---|---|
| `timed_mutex::lock` | canceled，清理执行 | 解锁持有者后可再次 `try_lock_for` | 5/5 可用 |
| `shared_mutex` 底层 `__shared_mutex_base::lock` | canceled，清理执行，内部 mutex 可再次取得 | 写入标记仍为 1；原读者退出后，新读者、新写者各等待 300 ms 均返回 `ETIMEDOUT` | 5/5 损坏 |
| `shared_timed_mutex::try_lock_for` | canceled，清理执行 | 原读者退出后，新读者和新写者的带时限尝试均失败 | 5/5 损坏 |
| `future::wait` | canceled，清理执行 | promise 置值后同一 future 就绪并取得精确值 42 | 5/5 可用 |
| `condition_variable_any::wait` | canceled，清理执行 | 用户 mutex 可取得；同一 cv-any 再次定时等待正常超时并保持锁 | 5/5 可用 |
| cv-any + 展开时会抛出的用户锁 | 未能回收到主线程 | `__unlock_guard` 的 non-throwing 析构内 `lock()` 抛出，进入终止处理器 | 5/5 终止 |

`shared_mutex` 格直接读取实现公开的内部 base 状态，同时用两个独立竞争线程分别
验证读者和写者阻塞；不是以“进程没崩溃”代替断言。`shared_timed_mutex` 不读取
私有布局，使用公开带时限接口观察状态。完整 30 行矩阵在
`tables/item1_facility_cancel.tsv`，代表输出及构建失败修正过程在 `raw/006_*`、
`raw/008_*`、`raw/009_*`、`raw/011_*`、`raw/032_*`。

### 2.4 对修改范围的事实含义

若目标事实被定义为“取消可以穿过四处等待边界，且上述内部同步对象取消后仍可
使用”，只解除四处异常规格不能满足这一结果：`shared_mutex` 和
`shared_timed_mutex` 的状态恢复是额外的实现问题；cv-any 用户锁在重新加锁时抛出
仍会因 `__unlock_guard::~__unlock_guard() noexcept` 终止。`timed_mutex`、普通
`future::wait` 和不抛用户锁的 cv-any 在本轮格中不需要额外状态恢复即可继续使用。
这是实验覆盖范围内的事实，不是处置建议。

## 3. armv7l 物理板取消矩阵

### 3.1 连接结果

资源闸门通过。既有成功记录给出的物理板入口是
`192.168.108.26:26101`。本轮：

1. `sdb devices` 启动服务后设备列表为空；
2. 对该入口连接三次，三次文本均为
   `error: failed to connect to remote target '192.168.108.26'`；
3. 值得单列的是，三次 `sdb connect` 的进程退出码均为 0，不能只看退出码判断连接
   成功；随后的远端预检明确返回 `target not found`、退出 1。

按任务书“连接中断即停报、重试不超过 3 次”，第三次失败后停止本项。没有向板上
推送文件，也没有启动 R113 进程；因目标不可见，板上收尾核验本身为
`NOT_AVAILABLE`。原始记录见 `raw/015_*` 至 `raw/019_*`。

### 3.2 未取得的结论

- R111 四种等待形态在 armv7l 保留/解除规格的每格 5 次矩阵：
  `NOT_AVAILABLE`。
- R112 旧调用方有邻近调用、quiet、`-fno-exceptions` 的 armv7l 每格 5 次矩阵：
  `NOT_AVAILABLE`。
- GCC、Clang 以及 R98 `angle` 实际产物的 `.ARM.exidx` 核查和
  `EXIDX_CANTUNWIND` 运行行为：因本项已按连接规则停止，`NOT_AVAILABLE`。
- armv7l 是否与 x86_64 一致、`.cantunwind` 推测是否成立：均不能从本轮证据判断，
  不以 QEMU 或 x86_64 结果补全。

## 4. 双版本与重新链接的旧对象

### 4.1 实验库实现

本实验从 R69 x86_64 libc++ 构建目录的全部 54 个 `cxx_shared` 对象重新链接两套
完整 `libc++.so.1`：

- 单版本旧库使用原 54 个对象，全部符号归入测试节点 `LLVM_22`；
- 双版本库使用其余 53 个对象，并以本轮重编的条件变量对象替换原对象；普通
  `condition_variable::wait` 同时导出旧
  `@LLVM_22` 和新默认 `@@LLVM_23`。

测试节点名只用于本实验。旧 wrapper 为 non-throwing，新 wrapper 可展开；两者执行
相同的底层等待体并输出实际入口标记。私有成员没有通过修改头文件暴露：代码以
`is_standard_layout` 和大小相等两个 `static_assert` 验证
`condition_variable` 只有一个 native condition-variable 状态，再从对象地址取得
首成员地址。完整实现见 `code/item3_dual_condition_variable.cpp`，版本脚本见
`code/item3_single_old.map`、`code/item3_dual.map`；动态符号表确认两个版本均存在。

### 4.2 四种绑定实测

每格 5 次，运行时均由 `ldd` 确认加载本轮双版本 `libc++.so.1`。`readelf -V/-Ws`
确认具体等待引用记录的节点，wrapper 自身的标记再次确认实际入口。

| 情形 | 等待引用 | 实际入口 | 行为 | 结果 |
|---|---|---|---|---|
| 旧对象先链接单版本旧库形成旧 ELF，再加载双版本库 | `LLVM_22` | old | 进入终止处理器 | 5/5 |
| 用旧头编译的 `old_quiet.o` 直接重新链接双版本库 | `LLVM_23` | new | canceled join；旧 quiet 帧清理为 0，mutex 保持锁定，竞争者 `ETIMEDOUT` | 5/5 |
| 用新头编译的对象链接双版本库 | `LLVM_23` | new | 帧清理及解锁各 1，竞争者取得 mutex | 5/5 |
| 用新头重编对象，但先链接单版本旧库，再加载双版本库 | `LLVM_22` | old | 进入终止处理器 | 5/5 |

矩阵在 `tables/item3_dual_version.tsv`，符号版本在 `raw/023_*`，运行结果在
`raw/024_*`，实际加载路径在 `raw/033_*`。

### 4.3 判定与保护边界

评审关于重新链接旧 `.o`/`.a` 的论断**成立**：对象文件中的普通未定义引用不带
版本；重新链接双版本库时，链接器把它记录为新默认节点。双版本实际保护的是已经
在最终 ELF 中记录旧节点的引用，不是所有曾经用旧头编译的目标文件。

部署顺序论断也**成立**：即使用新头重新编译，如果链接阶段看到的仍是单版本旧库，
最终 ELF 会记录旧节点，后来加载双版本库时仍进入旧 wrapper。本结论是本轮 GNU
ELF/x86_64 工具链实测，不外推到未测链接器或架构。

## 5. 四处文字核查

### 5.1 Boost.Test 崩溃的描述

“Boost.Test 的崩溃是旧 ELF 替换 provider 后自然发生的”这一表述**不成立**。
R112 探针在 35 行以 `RTLD_NOW|RTLD_LOCAL` 主动 `dlopen` provider，在 41-43 行把
新 provider 的实际新 mangled name 传给 `dlsym`，随后把地址强制转换成返回
libstdc++ `std::string` 的函数指针并调用。它证明的是“显式取得新名称后用旧 ABI
调用会崩溃”。它没有构造一个旧 ELF 的自然未定义引用并证明动态加载器能把旧名
自动绑到该新名。最初使用旧 `B5cxx11` 名称时实际是 `dlsym` 失败。

证据见 `raw/026_*` 及 R112 `raw/040_*`。

### 5.2 部分符号保留与首次调用

“部分保留边是首次调用时退出”只能在下列条件同时成立时使用：

- ELF 使用惰性 PLT 绑定，而非 `BIND_NOW`；
- 缺失引用没有被进程内其他 DSO 提供；
- 运行实际到达该引用的 PLT 调用点。

R112 的 x86_64 合成实验只证明机制：lazy 且不调用缺失项时 5/5 正常；第一次调用
缺失项时 5/5 先进入 `main` 和调用点，再以 127 和 `symbol lookup error` 退出；
NOW 正向对照 5/5 在 `main` 前失败。84 个真实 ELF 只核查了动态标签，没有取得
完整进程 DSO 闭包或调用覆盖。因此带上述条件的说法成立，无条件产品结论不成立。
R112 报告 241-243 行已经写明后两个条件。证据见 `raw/026_*`。

### 5.3 Boost 交付头文件对标准条件变量的直接使用

论断**成立**，但应与 Boost.Thread 自己的条件变量区分。冻结 Boost 头文件的精确
正向命中包括：

- `boost/fiber/detail/thread_barrier.hpp:33,55`：成员是
  `std::condition_variable`，直接调用 `cond_.wait(...)`；
- `boost/asio/detail/std_event.hpp:122,135,178`：成员是
  `std::condition_variable`，直接调用 `wait` 和 `wait_for`；
- `boost/compat/latch.hpp:24,79`：成员是 `std::condition_variable`，直接调用
  predicate `wait`。

另有 Boost.Fiber scheduler 头声明标准条件变量成员；仅有成员声明而未在同一头观察
到调用的不计作“直接等待”证据。正向对照同时找到 Boost.Thread 自有
`boost::condition_variable` 的 pthread 实现，说明检索没有把两类名称混用。
证据见 `raw/030_*`、`raw/031_*`。

### 5.4 “601 个消费者”的准确口径

把 601 称为“旧 libc++ 调用方”**不成立**。
`R109/tables/w1_binary_consumers.tsv` 的 601 行是按
“仓库、包、架构、ELF 路径”区分、并映射到 R24 重编计划的已发布仓消费记录；
它们引用的都是 libstdc++ mangled name
`_ZNSt18condition_variable4waitERSt11unique_lockISt5mutexE`，其中 9 行还记录
`@GLIBCXX_3.4.30`。

产生它的更宽 `w1_binary_callers.tsv` 另有 libc++ 名称：5 条普通等待 D01 和 3 条
system-clock 内部等待 D02。这 8 条没有进入 601 表。因此准确口径是“当前发布仓中
引用 libstdc++ 普通条件变量等待、并按仓/包/架构/ELF 路径区分的 601 条消费记录”，
不是旧 libc++ 对象清单，也不是 601 个唯一源包或唯一文件路径。证据见
`raw/028_*`。

### 5.5 libstdc++ `async` 析构路径

评审源码论断**成立（静态核查）**。libstdc++ 的
`__future_base::_Async_state_impl::~_Async_state_impl()` 在
`codes/gcc/libstdc++-v3/include/std/future:1767-1773` 直接判断线程可 join 后调用
`_M_thread.join()`。该析构没有显式异常规格，按成员与基类析构条件隐式为
non-throwing；`thread::join()` 本身可能生成 `system_error`，pthread 取消也以强制
展开穿过该调用。

libstdc++ 的普通 `future::wait` 结构不同：它先经可抛的 `_M_complete_async()` →
`_M_join()`，再用原子状态等待；问题在最后共享状态销毁时直接执行 join 的析构帧。
因此两套库在 `async` 最后状态释放路径都存在 non-throwing 边界，但具体实现分别是
libc++ 析构回调内的条件变量等待与 libstdc++ 析构内的直接 join。本轮只做源码
核查，没有运行时触发这条 libstdc++ 析构取消路径，动态结果为 `NOT_OBSERVED`。
证据见 `raw/029_*`。

## 6. 自行判断、限制与尚存疑问

- 用 `std::__shared_mutex_base` 直接实例化并读取状态，是为了同时取得内部状态和公开
  竞争行为；它就是 `std::shared_mutex` 唯一的底层成员，但这是 libc++ 内部接口探针，
  不是标准可移植代码。
- `shared_timed_mutex` 没有读取私有布局；两个公开 timed 尝试均在约定时间返回失败，
  同时证明内部 mutex 没有保持锁死，并观察到逻辑写入标记阻挡后续进入。
- 双版本节点名、helper 可见性及版本脚本通配形态只服务本实验，不视为发布设计。
- armv7l 板卡不可达使第二项整体缺失；所缺内容逐项列在第 3.2 节。
- 当前 x86_64 libstdc++ 选择 pthread rwlock；目标 armv7l 的配置选择未观察。
- libstdc++ async 析构论断是静态结论，没有本轮动态复现。

## 7. 证据索引

- `tables/item1_facility_cancel.tsv`：6 类设施、每格 5 次，共 30 行；
- `tables/item3_dual_version.tsv`：4 种绑定、每格 5 次，共 20 行；
- `code/`：探针、实验库实现、版本脚本、构建和运行脚本；
- `raw/`：命令原文、stdout、stderr 与退出码；
- `SCRIPT_SHA256SUMS`：本轮脚本及探针 SHA256；
- `MANIFEST.sha256`：报告、表格、脚本的完整性清单。
