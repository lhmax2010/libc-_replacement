# R54 三层整体内部验证报告

## 结论

本任务结论为 **`PARTIAL`**。

四个补丁按“层 A → 层 B → 层 C 头文件批 → 层 C 库内批”顺序可无冲突应用，两架构整体产物均构建成功，改动源码无新增警告，产物中未发现本项目诊断字符串。层间依赖的两条既有判断均由实测支持：层 C 缺层 B 时两架构均因 `__forced_unwind` 类型不存在而编译失败；层 B+C 缺层 A 时两架构的 typed 防护和标准库路径均失败。层 A、A+B、A+B+C 的贡献边界与预期一致。

不能定为 `INTEGRATION_VERIFIED` 的原因有两项：

1. `std::async` 内部工作线程被取消后，全补丁版在 x86_64 与 armv7l 上均未从 `future.wait()` 返回；参考 libstdc++ 格均退出 0。该格属于本任务新建的层 C 设施矩阵，此前无覆盖。
2. 多线程同时在标准库输出路径中被取消时，两架构均出现非确定性超时/终止。该现象在全补丁版与 libstdc++ 参考侧均出现，不是全补丁版独有，但按任务纪律必须申报为不稳定结果。

两处既知不可达站点 `basic_string::shrink_to_fit` 与 `basic_ostream<char>::sentry::~sentry()` 按任务要求记 `NOT_OBSERVED`，未重复论证。

## 1. 输入、补丁与整体构建

上游基线：`codes/llvm` commit `5ed6c77278dfa7a470667cf1a137723d3c96fe60`。

资源门禁：`tools/resource_gate.sh --level medium` 退出 0 后才开始后续步骤；门禁记录为 `raw/resources/001_resource_gate_medium.log`。构建统一使用并行度 2、`LLVM_PARALLEL_LINK_JOBS=1`、`nice -n 15` 与 `ionice -c 3`。资源采样未触及 1 GiB 红线。

| 顺序 | 补丁 | SHA256 |
|---:|---|---|
| 1 | 层 A `0001-Layer-A-*` | `9017093cca625662299c18e129e41d5fd43ecbae986a464aa286efb391549f20` |
| 2 | 层 B `0002-Layer-B-*` | `10cc7e989f40cb176b8cc9110cc1244db2788eb3028be7bd0606e30ba61138f1` |
| 3 | 层 C 头文件批 | `8beb1bb0e2b1a8d41908dcb70592fe5182782d3102f558f50fda5b2710a6dfec` |
| 4 | 层 C 库内批 | `fdcb004ca4cb6935efdc24c366757b50cb7916f1d5ad7cc8111dfbf5fd32d7d1` |

补丁应用冲突：**0/4**。最初一次 `git -C tmp/... apply` 因外层证据仓与被忽略的 `tmp/` 组合而返回 0 但未修改 C 层源副本；该批失真输出已单独保留并明确标为无效。随后改用显式 `git apply --directory=tmp/...`，以源码命中与文件 SHA 双重核验后重新构建、重新执行全部计入结论的矩阵。

| 架构 | libc++abi SHA256 | libc++ SHA256 |
|---|---|---|
| x86_64 | `c33908a63c72844a3fec4d260be325cd55e5845cb7359aa8831b3f2d177e3079` | `a840f017e557ce31338bd593c888f18c148ed6676db8e8c147ccf5eee9f6a739` |
| armv7l | `062d05b4335d970f893ab95e9abb9fca372ba09dce0f521264b4c0c11c8a9b6c` | `11819815aaa8c331a4ff00a1e91ae62e9a460fc109d6929c7c587844156ee62d` |

构建警告口径：统计 CMake/Ninja 完整日志中的 `warning:`；x86_64 首次/续跑分别 23/54 条，armv7l 144 条，均为工具链参数未在当前编译阶段使用等通用警告。按被改文件名检索的新增警告为 **0**。`strings` 未命中 `R40/R41/R42` 诊断标记；libc++ 中标准系统常量的 `TRACE` 名称不计作本项目诊断字符串。

## 2. 层间交互

完整逐格结果见 `tables/layer_interaction.tsv`，分母为每架构实际执行/编译的 11 格（9 个运行格 + 2 个 C-only 编译申报格中的本架构一格）。

- 无补丁：catch-all 重抛与标准库 cout 路径均复现失败。
- 仅 A：catch-all 重抛恢复；标准库路径仍失败。
- A+B：typed 防护恢复；标准库路径仍失败，证明 B 的作用边界。
- A+B+C：typed 防护恢复；armv7l cout 退出 0；x86_64 cout 的工作线程 join 为 `PTHREAD_CANCELED`，但进程退出阶段超时，libstdc++ 参考格同样如此。
- B+C（缺 A）：两架构 typed 防护与 cout 均失败，直接支持层 C 对 A 的依赖。
- 仅 C（缺 A+B）：两架构均编译失败，直接错误为 libc++ 站点引用的 `__cxxabiv1::__forced_unwind` 类型不存在，支持层 C 对 B 的编译期依赖。

因此文档中的两条依赖关系均成立；本任务未发现相反证据。

## 3. 并发取消

口径：4 个工作线程；每个“架构 × 运行时 × 模式”组合独立运行 20 次。共 `2 × 2 × 4 × 20 = 320` 格，分母 320。完整输出逐格位于 `raw/{x86_64,armv7l}/concurrency/`。

| 模式 | x86_64 全补丁/参考 | armv7l 全补丁/参考 |
|---|---|---|
| 多线程同时取消 | 20/20，20/20 | 20/20，20/20 |
| 取消与真实异常并发 | 20/20，20/20 | 20/20，20/20 |
| 重复取消 | 20/20，20/20 | 20/20，20/20 |
| 标准库输出并发取消 | 退出 0：9/20，9/20；四线程均 join：15/20，17/20 | 退出 0：6/20，3/20；四线程均 join：6/20，3/20 |

前三类共 `240/240` 退出 0。标准库输出并发取消出现时通时不通：x86_64 全补丁与参考均 11/20 非零/超时；armv7l 全补丁 14/20、参考 17/20 被板端守护终止。该现象不是全补丁版独有，但结果不稳定，故不得表述为通过。

## 4. 层 C 自建“设施 × 架构”矩阵

实际设施：cout、getline 自由函数、getline 成员、read、算术输入、packaged_task、async；另将两个已知不可达站点显式列为 `NOT_OBSERVED`。每个可执行设施均测未打补丁、全补丁、libstdc++ 参考，分母为 `7 × 2 × 3 = 42` 个运行格；另有 4 个不可达申报格。

- getline 自由函数、getline 成员、read、packaged_task：两架构均由基线 134 变为全补丁 0，且与参考格一致。
- 算术输入：三组合均退出 0；该用例没有复现基线缺陷，因此只证明非回归，不证明对应防护被触发。
- cout：armv7l 全补丁与参考均退出 0；x86_64 两侧均完成取消 join，但进程退出阶段超时，记录为 `JOIN_CANCELED_THEN_EXIT_TIMEOUT`。
- async：基线与参考退出 0；全补丁版在两架构均等待不返回（x86_64 124，armv7l 143）。该格为新发现的未通过项。
- string `shrink_to_fit`、ostream sentry 析构：`NOT_OBSERVED / UNREACHABLE_NOEXCEPT`。

可重复执行材料为 `src/facility_probe.cpp`、`tools/build_probes.sh`、`tools/run_x86_core.sh` 与 `tools/run_board_core.sh`；逐选项编译/链接命令在 `commands/`，逐格输出在 `raw/`。

## 5. 既有验证整体复跑

| 测试 | x86_64 | armv7l | 口径 |
|---|---:|---:|---|
| R51 回归矩阵 | 18/18 | 18/18 | 每架构 9 个场景 × 2 套运行时 |
| libc++abi `forced_unwind{1..4}` + `unwind_0{1..6}` | 20/20 | 20/20 | 每架构 10 测试 × 2 套运行时 |
| libc++ 选定 iostream/future/string 测试 | 7/7 | NOT_AVAILABLE | armv7l 板上无编译器/lit，且无同构预编译集合 |
| 非取消行为 | 8/8 | 8/8 | 改前/改后 × getline 成功/失败、packaged_exception、string_normal |

计入既有复跑的格未发现回归。armv7l 非取消附加件首次以单文件方式传输时因执行标签不适用得到 126，8 格全部判无效；随后按归档部署方式重跑为 8/8 退出 0，并完成第二次清理核验。

## 6. 运行时身份与板端边界

宿主与板端均保存了库 SHA256、`ldd` 和 `LD_DEBUG=libs` 原文，证明关键格加载 R54 全补丁版 libc++/libc++abi。板端测试路径为 `/var/tmp/r54_20260821`；任务结束后路径、两个传输归档均不存在，相关进程数为 0。未安装 RPM、未改系统库或配置、未重启或启停服务。

## 7. 已验证与未覆盖范围

已验证：x86_64 原生、armv7l 真机；DWARF 与 ARM EHABI；四补丁整体构建；层间剥离；单线程和四线程取消；指定标准库设施；ABI 上游测试；非取消行为。

未覆盖：

- aarch64；
- 混合 libc++/libstdc++ 栈帧跨越；
- LTO；
- `dlopen` 插件边界；
- 超过 4 个线程或长时间压力；
- armv7l 上游 libc++ iostream/future/string 源码测试的 lit 同构执行；
- 两个 `noexcept` 不可达站点的正向触发；
- async 等待不返回与并发 cout 不稳定的根因定位（本任务只验证，不修改补丁）。

## 8. 证据入口

- 命令与退出码：`progress/R54/commands/`
- 原始输出：`progress/R54/raw/`
- 汇总表：`progress/R54/tables/`
- 代码与配置策展副本：`docs/progress/R54/code/`
- 多方评审材料：`docs/progress/R54/review_package/`
