# W3 Boost 边界其余各条实测

## 结论

`PARTIAL`。90 条真实 Boost C++ 语义边已逐条分类并标记测量状态；按“该边包含被测代表符号，且该架构确实执行”计，R82 已测 12 条，本轮新增 x86_64 15 条，合计 27/90。其余 63 条没有被标成已测：39 条是同一代表符号组但该架构未执行，24 条属于未选符号组。每条边可能包含多个 C++ 符号，因此 27 也只表示边上的代表符号组，不表示边内所有符号逐个动态执行。

## 1. 90 条边分类

| 用户要求的分类 | 边数 | R82 对应口径 |
|---|---:|---|
| 布局敏感 | 63 | 33 `HIDDEN_STD_LAYOUT` + 6 `HIDDEN_STD_LAYOUT+DIRECT_STD_SIGNATURE` + 24 `DIRECT_STD_SIGNATURE` |
| C++ ABI 耦合但布局未证明 | 27 | 27 `NO_STD_LAYOUT_EVIDENCE` |
| 仅 C 接口 | 0 | 输入集合本身限定为 `TRUE_CPP_ABI_COUPLING` |
| 无交集 | 0 | 同上 |

分类是对全部 90 行机械生成，逐行见 `tables/w3_edges_90_results.tsv`。布局未证明不是安全证明。

## 2. 已有实测及精确映射

R82 的三个代表用例在实际执行过的架构上对应 12 条边：

- Boost.Filesystem 8 条：`E020,E021,E073,E074,E076,E077,E089,E090`；x86_64/armv7l 均观察到静默错值，消费方销毁路径到达；
- Boost.Iostreams 2 条：`E005,E057`；x86_64/armv7l 均在取得可断言值前终止，销毁完成标记未到达；
- Boost.Program_options 2 条：`E034,E053`；x86_64 在访问 provider 构造对象时终止，销毁完成标记未到达。

同符号组的 aarch64 边以及 Program_options 的 armv7l 边仍记 `NOT_OBSERVED_FOR_ARCHITECTURE`，没有把其他架构结果外推。

## 3. 本轮选取与理由

### 3.1 Boost.Test

Boost.Test 占 42 条边，是此前未动态覆盖的最大 provider。`ut_detail::normalize_test_case_name` 出现在 36 条跨架构边中，由平台 provider 构造并按值返回 libstdc++ `std::string`；consumer 可以检查规范化后的确切文本，并自行调用 libc++ 的 `std::string` 析构。因此它同时覆盖公开布局传递、具体值断言和异侧销毁。

本轮实际执行 x86_64，对应 12 条边：`E018,E031,E037,E044,E045,E046,E048,E054,E055,E058,E059,E080`。另外 24 条同符号组的 armv7l/aarch64 边没有执行。

### 3.2 Boost.Thread

Boost.Thread 的 9 条边全部属于“C++ ABI 耦合但布局未证明”，并共同包含 `enter_once_region`、`rollback_once_region`、`commit_once_region`。它是检验“未见 std 布局不等于未经测试也可判安全”的直接样本；用例断言 once 状态的完整转换，而不是只看退出码。

本轮实际执行 x86_64，对应 3 条边：`E014,E065,E067`；另 6 条 armv7l/aarch64 边未执行。该接口接收调用方构造的 `once_flag&`，provider 不构造返回对象；所以“消费方销毁 provider 构造对象”在此结构上 `NOT_APPLICABLE`。用例仍记录 flag 作用域退出，且该类型在当前头文件下为 trivial destructor。

## 4. 构建与运行时身份

consumer 使用平台 Clang、平台 libc++/libc++abi 头与 DSO构建；provider 是 R11 冻结的真实 x86_64 Boost.Test/Boost.Thread DSO。provider 的 `DT_NEEDED` 含 `libstdc++.so.6`，consumer 的动态依赖含 `libc++.so.1`/`libc++abi.so.1`。每次新测试的 `/proc/self/maps` 均明确观察到 libc++、libstdc++ 和目标 Boost provider 同时加载，两个用例均为 5/5。

Boost.Test 通过原始导出符号和 x86_64 Itanium sret 调用形态，让 provider 在对齐的额外存储中构造返回对象，再由 consumer 解释并析构。为排除该 raw 调用方式本身错误，同一源码另以 libstdc++ 构建正向对照；两组使用同一个平台 provider。

## 5. 新实测结果

| 格 | 重复 | 具体值断言 | 消费方销毁/作用域退出 | 结果 |
|---|---:|---|---|---|
| libstdc++ consumer + Boost.Test provider 对照 | 5 | 5/5 精确为 `alpha_beta_gamma`，长度 16 | 5/5 析构完成 | 5/5 通过 |
| libc++ consumer + Boost.Test provider | 5 | 5/5 观察长度 24，预期 16，值不相等 | 5/5 显式 libc++ 析构和 `dlclose` 完成 | 5/5 静默错值，退出 41 |
| libc++ consumer + Boost.Thread provider | 5 | 5/5 状态序列精确为 `1,1,0` | 5/5 作用域退出完成 | 5/5 该样本通过，退出 0 |

Boost.Test 首轮二进制也做了 5 次：当时 consumer 解释出的长度为 16，但具体 16 字节仍 5/5 不等于预期，且析构完成。增加固定值观测后重编，权威重跑得到表中稳定的长度 24。两个构建共同说明对象布局解释依赖 consumer 生成的具体栈/对象形态，不能因某次“长度碰巧正确”或析构未崩溃而判兼容。首轮材料保留在 `w3_runtime_matrix.tsv`，权威重跑为 `w3_runtime_matrix_retry.tsv`。

Boost.Thread 的成功仅证明当前单线程 once 状态转换样本；没有覆盖并发竞争、异常回滚压力、其他符号或其他架构，不能升级为该 provider 安全。

## 6. 未测部分与风险

未按本任务实际架构口径测量的 63 条边完整列于 `tables/w3_unmeasured_edges.tsv`：

- 44 条布局敏感：23 条是已选符号组的未执行架构，21 条是未选符号组。相同 provider 家族的已测样本已经出现静默错值或终止，风险评为高；不能据家族结果替代逐条值和销毁观察。
- 19 条 C++ ABI 耦合但布局未证明：16 条是本轮符号组的 armv7l/aarch64 未执行边，3 条为 Boost.Test 未选符号组。风险保持未知；Boost.Thread x86_64 成功不能外推。

若按“不区分架构，只看代表符号是否被某处测试”计，R82 三组触及 21 条、本轮两组触及 45 条、未选符号组 24 条；该口径只用于说明选择覆盖面，不用于 `ALL_MEASURED` 判定。权威结论采用 27 已测、63 未测。

## 7. 证据索引

- 全 90 条分类、代表符号及结果：`tables/w3_edges_90_results.tsv`；
- 未测 63 条：`tables/w3_unmeasured_edges.tsv`；
- 分类与最终统计：`tables/w3_edge_classification_summary.tsv`、`w3_final_summary.tsv`；
- 新混合矩阵：`tables/w3_runtime_matrix_retry.tsv`、`w3_runtime_summary_retry.tsv`；
- libstdc++ 正向对照：`tables/w3_libstdcxx_reference.tsv`；
- R82 既有矩阵：`docs/progress/R82/tables/mixed_runtime_matrix.tsv`；
- 构建、符号、动态依赖、SHA256、命令原文、stdout、stderr 与退出码：`raw/`。

本轮没有修改平台源码或四个补丁，没有使用开发板，没有把“未崩溃”当作“没有问题”。
