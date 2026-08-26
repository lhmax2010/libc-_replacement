# R70 报告：等待方取消定性、armv7l 补跑与八处待议站点评估

## 结论

总判定为 **`VERIFIED`**：等待方取消已经完成两架构、三组合各 20 次的定性；R69 缺失的 armv7l 运行矩阵已补齐；八处待议站点均已逐项评估。未修改四个补丁或平台源码。

等待方取消属于 **既有缺口**，不是本方案引入：

| 架构 | 未打补丁 libc++ | 最终补丁 libc++ | libstdc++ 参考 |
|---|---:|---:|---:|
| x86_64 | 0/20，均 134 | 0/20，均 134 | 20/20 |
| armv7l | 0/20，均 134 | 0/20，均 134 | 20/20 |

八处清单确实包含 `condition_variable` 的一处，但本用例**没有命中该站点**。该站点是 `condition_variable_any` 的 scoped-unlock guard；本用例的 `future` 共享状态使用普通 `condition_variable`。libc++ 普通 `condition_variable::wait(unique_lock<mutex>&)` 本身被声明和定义为 `noexcept`，而 libstdc++ 对应函数不是 `noexcept`。两侧 134/0 的运行结果与该源码差异一致；本任务因已定性为既有缺口，没有按“本方案引入”分支做调用栈深挖，也不据此实现修法。

## 1. 资源、执行边界与身份

- 固定门禁 `tools/resource_gate.sh --level medium` 返回 0；开工时 20 CPU、可用内存约 19 GiB、load1 0.62。
- 运行期采样可用内存约 19 GiB，没有触发 1 GiB 中止线。
- x86_64 在宿主原生运行；armv7l 在 `192.168.108.26:26101` 真机原生运行，无模拟器。
- 板卡身份为 armv7l / Tizen 11.0 / `tizen-unified-toolchain_20260728.012216_tizen-headed-armv7l`。
- 复用 R69 交叉编译产物，没有重建。板上实际加载：
  - `libc++.so.1`：`c44942bf3eccce1f77d67ec1fca8bdb16c8a2a9e18280c0c47e9bfcdcc95573c`；
  - `libc++abi.so.1`：`f8a4456a111eb86e9552a9411d08636e5e34300db16a0e01b8d8a637a14b418f`。
- 板上 `ldd` 与 `LD_DEBUG=libs` 均显示从 `/var/tmp/r70_20260826/runtime/fixed/` 加载上述实验库。
- 部署前专用路径不存在且项目进程数为 0。收尾核验专用路径、共享归档均不存在，目标进程数为 0。

## 2. 八处原始清单核对

原始清单为 `docs/progress/R44b/tables/no_counterpart_8.tsv`，SHA256：
`ce2ddb0f9f353c7c79c4be869cd861bf6001d3385a06e254079b573d2657ef1f`。

八处组成：

- `include/experimental/executor`：4 处，行 1734、1794、1812、1830；
- `include/ext/vstring.tcc`：2 处，行 605、681；
- `include/std/condition_variable`：1 处，行 268；
- `include/tr2/dynamic_bitset.tcc`：1 处，行 255。

因此条件变量相关站点**在八处之中**。其形态是 `condition_variable_any::_Unlock::~_Unlock()` 在已有异常传播期间重新取得用户锁：对 forced unwind 重抛，对其他 relock 异常吞掉。

## 3. 等待方取消的定性

测试件 `r69_new_cases.cpp` 使用裸 `pthread_create` 创建等待方；等待方阻塞在 `shared_future::wait()`，应用层没有 catch。x86_64 新补 20 次基线全部输出 `libc++abi: terminating due to uncaught foreign exception` 并退出 134。R69 最终版的 20 次同样全部 134，参考 20 次全部退出 0并断言：

`CANCEL_RC=0 JOIN_RC=0 JOIN_CANCELED=1 VALUE=42`。

armv7l 三组结果逐格相同。因此按任务三分法定性为 **既有缺口**：补丁没有引入它，但当前层 C 没有覆盖它。

调用链的源码核对结果：

1. `future.cpp:108-121` 的 `__assoc_sub_state::wait/__sub_wait` 调用成员 `condition_variable __cv_.wait(__lk)`；
2. 该成员是普通 `condition_variable`，不是八处清单中的 `condition_variable_any`；
3. libc++ 在 `__condition_variable/condition_variable.h:107` 与 `src/condition_variable.cpp:33` 将普通 wait 声明、定义为 `noexcept`；
4. libstdc++ 在 `include/std/condition_variable:97-98` 的对应 wait 没有 `noexcept`。

这是源码与现象的对应记录，不是运行时调用栈结论；按任务要求，既有缺口不深入抓栈。

## 4. armv7l 全量补跑

完整汇总见 `tables/armv7l_summary.tsv`，每格原始输出见 `raw/armv7l/matrix/`。

- 定向：F1、F2、M1 最终版与参考均通过；M2 锁外析构两侧各 20/20；引用返回格两侧通过。
- N1：未补丁状态断言格 134；最终版与参考均 0，并断言 `RDSTATE=1 BAD=1 FAIL=1`。
- deferred 重试期再取消：最终版与参考各 20/20；两次等待者取消成功，第三次执行返回 42，调用次数为 3。
- 等待方取消：基线与最终版各 0/20（134），参考 20/20。
- 设施：cout、自由/成员 getline、read、packaged_task、async 的基线失败格均由最终版修复为 0，参考为 0。算术设施的一次普通时序格基线也为 0，但专用 N1 状态格稳定复现 134/0/0。
- R51 九场景：最终版与参考 18/18。
- 上游：libc++abi 最终版 10/10、libstdc++ 参考 10/10、选定 libc++ future/iostream/string 测试 11/11。
- 非取消路径：async 正常值与真实异常、packaged_task 正常值与真实异常、iostream 格式错误三组合均通过。
- 并发：multi-cancel、取消与异常并发、重复取消两侧均 20/20；M1 多等待者两侧均 20/20。
- 标准库并发输出仍是已登记的两侧共同不稳定：最终版 6/20，参考 3/20。

## 5. 与 x86_64 的逐格对照

完整表见 `tables/cross_architecture.tsv`。除下列四个已有时序/退出形态差异外，双方实际共同覆盖的退出分布一致：

| 格 | x86_64 | armv7l | 说明 |
|---|---|---|---|
| async 未补丁取消 20 次 | 8/20 | 4/20 | 未补丁竞态窗口；两架构均混合 0/134 |
| 标准库并发，最终版 | 15/20；失败 124 | 6/20；失败 143 | 已登记不稳定；两侧均存在 |
| 标准库并发，参考 | 0/20；失败 124 | 3/20；失败 143 | 同上 |
| cout 最终版单格 | 124 | 0 | x86_64 已登记为进程退出阶段超时；取消与 join 已完成 |

等待方取消、N1 状态断言、deferred 再取消、F1/F2/M1/M2、R51 回归及非取消路径的结论均跨架构一致。

## 6. 八处待议站点评估

逐处机器可读表见 `tables/no_counterpart_8_evaluation.tsv`。

### 6.1 experimental executor（4 处）

四处分别保护 use_future completion handler 以及 executor 的 dispatch/post/defer：forced unwind 重抛，普通异常写入 promise。libc++ 22.1.8 不提供该 experimental executor/use_future 扩展，故不是“位置不同但等价”，而是设施不存在。参照侧在用户 completion function 或 executor 操作到达取消点时可达；libc++ 当前无调用路径。若存在实现，只能是新增兼容扩展的头文件代码，需要消费者重编。

### 6.2 GNU vstring（2 处）

两处是 `__versa_string` 的流提取与 getline；forced unwind 前设置 `badbit`。libc++ 不提供 GNU vstring 扩展。参照侧 streambuf 输入可到取消点；libc++ 当前设施不存在。可行位置只能是新增 GNU 兼容扩展头，需要消费者重编。

### 6.3 condition_variable_any（1 处）

libc++ 有结构等价位置：`include/condition_variable:150-159` 的 `__unlock_guard`，但其析构是 `noexcept` 并直接调用用户锁的 `lock()`。`condition_variable_any::wait` 会实例化这个 guard；若 forced unwind 在 relock 用户代码中产生，离开 noexcept 析构会 terminate，因此是条件可达。该位置属于头文件模板，若实现需重编消费者。

它与本轮 future 等待失败不是同一位置：future 使用普通 condition_variable；后者的 out-of-line wait 自身是 noexcept。

### 6.4 TR2 dynamic_bitset（1 处）

该处保护 TR2 dynamic_bitset 的流提取并在重抛前设置 `badbit`。libc++ 没有该 TR2 扩展。参照侧 streambuf 输入可到取消点；libc++ 当前设施不存在。可行位置只能是新增兼容扩展头，需要消费者重编。

八处中，只有 condition_variable_any 在现有 libc++ 中有结构等价实现位置；其余七处属于 libc++ 不提供的扩展设施。没有一处可作为既有 out-of-line 库符号单独交付。

## 7. 自行判断、疑问与未覆盖范围

自行判断：

1. 将 R69 的最终版与参考 20 次原始结果作为 x86_64 已完成组，仅新增缺失的基线 20 次；没有无意义地重跑已完整的两组。
2. 不把八处中的 condition_variable_any guard 误认成本用例命中点；以 future 成员类型和实际调用链作区分。
3. 对第三部分的“可达”分别陈述参照设施与 libc++ 当前设施，避免把“设施不存在”写成“防护不可达”。

尚存疑问：

- 普通 `condition_variable::wait` 的 `noexcept` 差异是否应纳入未来层 C 范围，属于新的设计裁决，本任务不做建议。
- condition_variable_any guard 的最小稳定运行时触发夹具本轮未构造；其可达性依据源码调用关系与用户锁可执行代码，不是新增实测。

未覆盖：aarch64、宽字符、LTO、混合运行时帧、动态加载边界，以及七个缺失扩展设施在“新增兼容实现”后的运行行为。未向 Gerrit 或外部源码仓推送。
