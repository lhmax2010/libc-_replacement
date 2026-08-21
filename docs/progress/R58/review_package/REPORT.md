# R58 四补丁最终形态的整体复验报告

## 结论

本任务结论为 **`INTEGRATION_VERIFIED`**（限本报告明确覆盖的架构、配置与可达站点）。

R55 修订后的层 C 头文件补丁与层 A、层 B、层 C 库内补丁可按顺序无冲突应用；源码命中与文件 SHA 双重核验均确认四补丁实际进入实验副本。x86_64 与 armv7l 的 libc++abi/libc++ 均构建成功，改动源码未出现新增警告，产物中未检出 R40/R41/R42 诊断字符串。

本轮最重要的新判据 `std::async` 已通过：全补丁版在 x86_64 与 armv7l 各重复 20 次，均为 **20/20 退出 0、20/20 等待方返回**；真实异常仍以 `std::runtime_error("async-real-error")` 经 `future.get()` 原样取得。四补丁当前组合未复现 R54 的等待挂起。

两处既知 `noexcept` 不可达站点仍按任务约定记 `NOT_OBSERVED`。标准库并发输出仍不稳定，但全补丁与 libstdc++ 参考侧均出现，且已由任务背景登记为非本方案独有边界；本轮未将其解释为补丁回归。

## 1. 输入、应用与构建

基线为 `codes/llvm` commit `5ed6c77278dfa7a470667cf1a137723d3c96fe60`。资源工具 `tools/resource_gate.sh --level medium` 退出 0 后才执行其余步骤；实测 `MemAvailable=15275684 KiB`、`load1=9.66`、负载上限 20。构建统一使用 `-j2`、`LLVM_PARALLEL_LINK_JOBS=1`、`nice -n 15`、`ionice -c 3`。

| 顺序 | 补丁 | SHA256 |
|---:|---|---|
| 1 | Layer A | `9017093cca625662299c18e129e41d5fd43ecbae986a464aa286efb391549f20` |
| 2 | Layer B | `10cc7e989f40cb176b8cc9110cc1244db2788eb3028be7bd0606e30ba61138f1` |
| 3 | Layer C 头文件批（R55） | `5562639277ff71921d18be41a1ed80755e11c0b70378e45567eadc99a57335c5` |
| 4 | Layer C 库内批 | `fdcb004ca4cb6935efdc24c366757b50cb7916f1d5ad7cc8111dfbf5fd32d7d1` |

应用冲突为 **0/4**。每个补丁均先 `git apply --check`，再实际应用；随后以 12 个功能标记的源码命中及 8 个最终文件 SHA 核验，避免“apply 返回 0 但副本未变”的旧问题。

| 架构 | libc++abi SHA256 | libc++ SHA256 |
|---|---|---|
| x86_64 | `26d1c600a7c0eaec53ab45918cb03715cb0fbe5b67c6208df1e0618184fb7b52` | `4681090f94ca1bd1982af5d0acf8936cd2a57d04efc28bdd8c5db72cd8b3d9b1` |
| armv7l | `1c22a77f964a4519981cf8ae76353c8897fdf0c09a1df70b9eea5efda20fdebd` | `72737d6d2dc3c0ddef7d1eb074e6235bbfedb3c2c1fd02aff0da2ebf4a3d9b2e` |

x86_64 初次 libc++ 编译暴露实验副本缺少 LLVM `libc/shared/fp_bits.h`；补入同一 `codes/llvm/libc` 基线并重新配置后成功。失败与两次重试完整保留，不计为补丁源码失败。成功日志中按改动文件名检索的新警告为 0。

## 2. 层间依赖复验

两架构结果一致：

- 无补丁：catch-all 重抛及标准库路径复现失败。
- 仅 A：catch-all 重抛恢复；标准库设施仍失败。
- 仅 A+B：typed 防护恢复；标准库设施仍失败。
- A+B+C：typed 防护和全部可达设施恢复。
- B+C 缺 A：typed 防护和标准库路径均以 134 失败。
- 仅 C 缺 A+B：两架构均在编译期因 `__cxxabiv1::__forced_unwind` 不存在而失败。

与 R54 的层间依赖结论一致。x86_64 cout 在个别独立进程退出阶段出现 124，而同轮 libstdc++ 参考亦出现；线程取消与 join 已完成，属于已登记的退出阶段不稳定，不改变依赖判断。逐格证据见 `tables/layer_matrix.tsv`。

## 3. 设施矩阵

执行口径为 2 架构 × 7 个可执行设施 × 3 组合，共 **42 格**，另有 4 个 `NOT_OBSERVED` 申报格。getline 自由函数、getline 成员、read、packaged_task 的基线均复现 134，全补丁与参考均退出 0。算术输入三侧退出 0，只能证明非回归。cout 的工作线程在全补丁侧正确取消，仍保留上述参考侧共有的退出阶段边界。

`async` 专项另按每架构、每组合重复 20 次：

| 架构 | 全补丁 | libstdc++ 参考 | 判据 |
|---|---:|---:|---|
| x86_64 | 20/20 | 20/20 | 退出 0 且 `WAIT_RETURNED=1` |
| armv7l | 20/20 | 20/20 | 退出 0 且 `WAIT_RETURNED=1` |

取消后的 `get()` 返回 `future_error`，信息为共享状态在 ready 前 promise 被析构；这是 R55 既定修后语义。`std::async` 内部线程由 libc++ detach，因此 join 记 `NOT_AVAILABLE_STD_ASYNC_OWNS_DETACHED_THREAD`，未伪造 join 结果。

## 4. 非取消路径

两架构上未打补丁、全补丁、libstdc++ 参考均通过：async 正常取值、async 真实异常传递、packaged_task 正常取值、packaged_task 真实异常传递、getline 正常/EOF、string 正常路径、算术格式错误状态。关键观察：

- async 真实异常类型为 `std::runtime_error`，内容严格为 `async-real-error`；
- packaged_task 正常值为 24，异常内容严格为 `packaged-real-error`；
- 算术格式错误设置 fail 状态且不改写哨兵值。

未观察到由最终四补丁引入的非取消行为变化。

## 5. 既有回归与上游测试

| 测试集 | x86_64 | armv7l | 分母口径 |
|---|---:|---:|---|
| R51 回归矩阵 | 18/18 | 18/18 | 9 场景 × 2 运行时 |
| libc++abi forced_unwind/unwind | 20/20 | 20/20 | 10 测试 × 2 运行时 |
| 选定 libc++ iostream/future/string | 7/7 | NOT_AVAILABLE | 板上无编译器/lit，且本轮未生成同构预编译集合 |

宿主选定 libc++ 测试是从当前最终头文件和当前最终库重新编译所得，不是沿用旧二进制。

## 6. 并发取消

口径为 2 架构 × 2 运行时 × 4 模式 × 20 次，共 **320 格**。

- 多线程同时取消：80/80 退出 0。
- 取消与真实异常并发：80/80 退出 0。
- 重复取消：80/80 退出 0。
- 标准库并发输出：全补丁 x86_64 16/20、armv7l 10/20；参考 x86_64 7/20、armv7l 3/20。两侧均非确定性超时，失败样本全部保留。

未发现只发生于全补丁侧的并发失败；也未将参考侧共有的不稳定表述为通过。

## 7. 身份、开发板与边界

宿主和开发板均保存库 SHA256、`ldd` 与 `LD_DEBUG=libs` 原文。板端使用 `/var/tmp/r58_20260821`，收尾核验：部署目录不存在、共享归档不存在、相关进程数为 0。未安装 RPM、未修改系统库或配置、未重启。

已验证：x86_64 原生、armv7l 真机；DWARF 与 ARM EHABI；动态 libc++；无 LTO；指定设施、异常与四类并发模式。

未覆盖：aarch64；混合运行时帧穿越；LTO；`dlopen`/插件边界；超过 4 个工作线程和长时间压力；armv7l 的 libc++ 源码测试同构集合；两个 `noexcept` 不可达站点。层 C 条件编译改造仍为已登记待办；R56 已实测“仅前置声明”在语言层不可编译，R57 材料在仓内为 `NOT_AVAILABLE`。

自行判断：算术输入基线未复现缺陷，因此仅按非回归计；参考侧共有的 cout/并发输出超时不归因于本补丁。尚存疑问：条件编译改造采用何种完整类型承载方式，仍待人工另行裁决。

## 8. 证据入口

- 原始命令与退出码：`progress/R58/commands/`
- 逐格原始输出：`progress/R58/raw/`
- 汇总表：`progress/R58/tables/`
- 代码策展副本：`docs/progress/R58/code/`
- 评审材料包：`docs/progress/R58/review_package/`
