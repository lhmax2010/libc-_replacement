# R27：双运行时 provider 闭包与永久边归属

## 结论摘要

本轮只复用 R11/R22/R24/R25/R26 已落盘数据与冻结符号交集，没有重扫平台仓、构建、安装或修改任何既有输入。

- R26 的 **75 个跨界 provider 种子**不是不动点。沿 `consumer P → provider Q` 方向迭代时，第一轮新增 `libcynara-commons-0.26.0-1.src.rpm`，随后不再新增；**保守闭包与语义闭包均为 76/483 个源码 RPM 身份**，相对种子增加 1。
- 两种闭包都覆盖 407 个二进制包映射记录。闭包覆盖 76/483（15.735%）全量源码身份，或 76/479（15.866%）待迁平台源码身份，不覆盖整个平台。
- 完整图为 1,234 条 `TRUE_CPP_ABI_COUPLING` 边。R26 的同一规则将其分为 1,221 条 `CXX_SEMANTICS`、0 条 `C_ONLY`、13 条 `UNDETERMINED`。保守闭包允许全部 1,234 条传播，语义闭包只允许 1,221 条确认语义边传播；成员结果仍同为 76。
- 切面 5 的 12/12 条边均由 4 个厂商源码身份指向平台 provider，且 12/12 均为 `CXX_SEMANTICS`。直接 provider 去重后为 6 个；递归闭包没有新增成员，因此永久保留旧构建的平台自有源码身份为 **6/479**。
- 6 个永久平台 provider 对应的二进制包数合计为 79。连同 5 个厂商二进制 Name 所映射的 4 个厂商源码身份，永久旧运行时集合按源码身份计为 **10/483**。该数字只做账目陈述，不改变现行端点定义。

## 一、闭包定义与方向

R22 边定义是 `consumer P → provider Q`：P 的动态 UND 与 Q 的动态 DEF 去版本后有 `_Z` 风格符号交集。闭包问题问的是 `P_old` 自身构建需要谁的 old ABI，因此传播方向与边方向一致：当前集合含 P 且 P→Q 满足传播判据时，将 Q 加入集合。

这与 R25 的“尚未迁消费者指向已迁 provider”切面统计用途不同，但底层有向边方向没有反转。方法原文在 `tables/method_and_scope.tsv`。

## 二、75 个种子与逐轮闭包

75 个种子的逐包身份、阶段、二进制包数和 R26 边分类见 `tables/seed_providers_75.tsv`。逐轮结果为：

| 闭包 | 第 0 轮 | 第 1 轮新增 | 最终规模/483 | 相对种子差值 | 二进制包数合计 |
|---|---:|---|---:|---:|---:|
| 保守 | 75 | `libcynara-commons-0.26.0-1.src.rpm` | 76 | +1 | 407 |
| 语义 | 75 | `libcynara-commons-0.26.0-1.src.rpm` | 76 | +1 | 407 |

唯一新增传播边是：

`security-manager-2.4.0-0.src.rpm → libcynara-commons-0.26.0-1.src.rpm`

该边引用 `_ZN6Cynara12PolicyResultC1ERKt` 与 `_ZNK6Cynara12PolicyResult10policyTypeEv`，R26 同一规则判为 `CXX_SEMANTICS`，故两种闭包都传播。逐轮清单见 `closure_iterations.tsv`，76 个最终成员及其进入轮次见 `closure_members.tsv`，被访问的 239 条出边见 `closure_propagation_edges.tsv`。

## 三、闭包属性分布

| 集合 | 阶段一 | 阶段二层 1 | 阶段二层 2 | 阶段二层 3 | 阶段三 | 厂商单列 | 合计 |
|---|---:|---:|---:|---:|---:|---:|---:|
| 种子 | 9 | 43 | 17 | 6 | 0 | 0 | 75 |
| 保守闭包 | 10 | 43 | 17 | 6 | 0 | 0 | 76 |
| 语义闭包 | 10 | 43 | 17 | 6 | 0 | 0 | 76 |

新增包位于阶段一，二进制包数为 16，因此种子二进制包数合计从 391 增至 407。完整阶段与二进制包统计见 `closure_stage_distribution.tsv`。

## 四、保守闭包与语义闭包

R26 已分类 1,191 条曾跨切面的边；R27 对从未跨切面的另外 43 条边复用同一肯定规则，43/43 均为 `CXX_SEMANTICS`。完整 1,234 条边的分布为：

| 分类 | 边数/1,234 |
|---|---:|
| CXX_SEMANTICS | 1,221 |
| C_ONLY | 0 |
| UNDETERMINED | 13 |

语义闭包相对保守闭包排除 13 条不可判定边。其中 12 条 Rust 碰撞边的消费者没有进入保守闭包；另 1 条 `capi-system-media-key → bundle` 的消费者在闭包内，但 provider `bundle` 本来就是种子，故即使保守传播也不增加成员。因此两种闭包成员差值为 0，不能把这一结果解释为 13 条边已得到语义确认。逐边见 `closure_semantic_excluded_edges.tsv`，两种模式的边数与成员数对照见 `closure_mode_comparison.tsv`。

## 五、切面 5 的 12 条永久边

切面 5 的消费者去重后恰为 4 个厂商源码身份：

- `drm-service-core-tizen-0.3.9-0.src.rpm`：2 条；
- `hal-api-drm-1.0.5-1.src.rpm`：6 条；
- `hal-backend-drm-generic-1.0.3-0.src.rpm`：2 条；
- `pen-wave-1.4.0-2.src.rpm`：2 条。

未出现非厂商消费者。12 条边均由 R26 符号实证判为 `CXX_SEMANTICS`。完整符号列在 `permanent_edges_12.tsv`。

去重 provider 为 6 个：

| provider 源码身份 | 阶段 | 厂商边数 | 二进制包数 |
|---|---|---:|---:|
| `app-core-1.27.13-0.src.rpm` | 阶段二层 1 | 1 | 13 |
| `bundle-0.18.15-0.src.rpm` | 阶段一 | 1 | 17 |
| `gcc-14.2.0-1.13.src.rpm` | 阶段一 | 4 | 21 |
| `gcc-14.2.0-1.15.src.rpm` | 阶段一 | 4 | 20 |
| `gtest-1.15.2-0.src.rpm` | 阶段一 | 1 | 2 |
| `rpc-port-1.21.15-0.src.rpm` | 阶段二层 1 | 1 | 6 |

六者全部位于第一部分的 76 包闭包内。

## 六、永久双运行时闭包

以第 5 节的 6 个 provider 为永久种子，按同一方向递归：保守闭包与语义闭包均在第 0 轮即达到不动点，最终均为 **6 个**，没有新增包。

这不表示六者之间没有依赖：已观测 13 条内部依赖，例如 `app-core → bundle/gtest/gcc`、`rpc-port → bundle/gtest/gcc`、`gtest → gcc`。这些 provider 已全部在直接永久集合中，因此递归没有扩大成员。相关边见 `permanent_closure_propagation_edges.tsv`。

## 七、E-B 端点账目差异

现行 E-B 表述假设平台自有包需要永久保留 GCC C++ 运行时的数量为 0，并把 5 个厂商二进制包作为显式例外。冻结数据实测为：

- 平台自有永久 provider：6/479 个源码 RPM 身份；
- 厂商例外：5 个二进制 Name，映射为 4 个源码 RPM 身份；
- 两者按源码身份合计：10/483。

因此现行“平台自有永久例外为 0”的账目与实测相差 6 个平台源码身份。本报告不提出新端点定义，也不评价处置方案。

## 八、覆盖边界

本计算只覆盖 R22 冻结的动态 ELF `DT_NEEDED + UND/DEF` 图。静态库、header-only/inline/template、dlopen 未唯一解析边、C 回调抛异常、`void*` 所有权传递，以及未进入 R11 发布形态快照的对象不在图内。13 条符号级不可判定边没有推断补全。

输入路径、SHA256 和大小见 `inputs/input_identity.tsv`；四个仓的 revision/repomd SHA256 见 `tables/data_sources.tsv`。本报告不做排期、人日估算或方案推荐。
