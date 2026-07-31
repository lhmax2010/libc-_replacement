# T8：正式 RPM 宏层三层证据重跑

## 结论

状态：`DONE`。

在新建、单用途并已退役的 GBS target 根中取得 RPM 4.14.1；基于 T7 冻结的当前配置重新生成 baseline/formal RPM 上下文后，三层证据全部通过。该结论只证明正式宏机制满足本任务定义的等价性与隔离性，不把 T2 中构建系统可能忽略注入参数的疑点误判为已解决。

## 输入与环境

- 500 项样本的完整 spec manifest 固定于 `progress/T8/inputs/manifests/spec_manifest.v2.tsv:1`，SHA-256 为 `a48dce41bbeb44b6a604d4daa776385a9aa36da537e70ad974a557ac19e3e002`。
- 抽样 manifest 固定于 `progress/T8/inputs/manifests/sample_manifest.v2.tsv:1`，SHA-256 为 `c2824cf552353c01eccfc76cf5d4383876c4c1e5bf89973ce9ddfc774cb769aa`；三项输入摘要见 `progress/T8/input_manifest_sha256.log:1`。
- 新 target 环境的完整 GBS 命令见 `progress/T8/environment_command.txt:1`，执行退出码为 0；构建全程日志 `progress/T8/environment_gbs.log:1` 为 `LOCAL_ONLY`，SHA256 `9d269691ec4d6e926a35e6db3b9831ce74c86e3df042d2258873a4a44412d7c9`。
- baseline/formal 的四份 RPM 上下文摘要见 `progress/T8/input_context_sha256.log:1`。T7 新下载的 effective buildconf 与旧证据字节相同不等于复用旧上下文；本任务仍从当前冻结配置重新生成并重新执行全部证据。
- 只对历史已验证 runner 的只读路径和冻结身份 SHA 做了环境适配；差异及自检见 `progress/T8/runners/environment_only.diff:1` 与 `progress/T8/self_check.tsv:1`。

## 第一层：非成员 rpmspec 等价性

- 500 个样本全部完成，其中 498 个为非成员、2 个按历史清单归为成员；另加入 1 个复杂负样本。
- baseline/formal 解析失败均为 0，新增解析失败为 0。
- 499 个非成员范围（498 个样本非成员加复杂负样本）的字节差异失败为 0。
- 汇总见 `progress/T8/layer1_summary.tsv:1`；逐 spec 批量原始结果 `progress/T8/layer1_rpmspec_results/comparison.tsv:1` 为 `LOCAL_ONLY`，SHA256 `3ecc0e42311b510731752eaa0564a367731fc7ca7336ba9609b3454739d87436`。

## 第二层：五个正式成员的条件门

五个成员的 baseline/formal 展开均为空差异；对每个 spec 的 `%optflags`/`%build_ldflags` 文本引用检查退出码均为 1，因此按规格判定为 `PASS_MEMBER_NO_TEXTUAL_FLAG_REFERENCE`，5/5 通过。逐项台账见 `progress/T8/layer2_conditional_members/formal_member_conditional_gate.tsv:1`。

历史 runner 在处理首个成员后触发了“成员必须非空差异”的旧哨兵。该断言已被本次规格的条件判据取代，不构成 T8 红项；首个成员的当次新鲜输出由第二层接续消费并重新检查，裁决记录见 `progress/T8/adjudication.tsv:1`。

## 第三层：宏求值移除等价性

- 五个正式成员乘两个宏共 10 行：正式后缀恰好出现一次，移除后与 baseline 字节相同，baseline 中后缀 grep 退出码为 1，10/10 通过。
- 三个非成员乘两个宏共 6 行：formal 与 baseline 字节相同，baseline 中后缀 grep 退出码为 1，6/6 通过。
- `%optflags` 的五个成员结果均记录“后缀不位于字符串结尾”的信息性事实，但未把“必须位于结尾”当成判据，符合任务纪律。
- 完整 16 行矩阵见 `progress/T8/layer3_macro_eval_results/macro_eval_removal_equivalence_matrix.tsv:1`。

## 闭环

三层范围、观测和证据链见 `progress/T8/evidence_chain.tsv:1`；最终自检见 `progress/T8/self_check.tsv:1`。新建 target 根已标记 `RETIRED_T8_EVIDENCE_ONLY`，不得复用。
