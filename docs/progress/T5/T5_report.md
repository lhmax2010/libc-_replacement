# T5 门禁工具版本断言修订与解冻重跑报告

状态：`DONE`。

## 输入与范围

版本断言的冻结原件为
`/home/toolchain/plan_evaluation/p1/libcxx_packaging/multiarch/run_static_gates.sh`，
SHA256 为
`e37add65648c9afdb11eb320926b2285ae6e6da324c70b9510909b799df42d3e`。
旧目录保持只读；原件副本保存在
`progress/T5/raw/run_static_gates.before.sh`。当前工作树此前没有该入口，故按
原字节将它纳入 `gates/tools/run_static_gates.sh` 后，仅修订符号版本字面量。

原工具第 16、18、33--36 行的 `21.1.1-2` 是旧制品文件名选择条件，不是
本任务所指的 `@@LLVM_21` 符号版本断言；T3/T4 无新制品，本任务没有猜测新
RPM release，也没有改变这些输入选择逻辑。

## 全部修订点

共 5 个源码行、8 个字面量实例；file:line 与逐处前后文本如下。完整原始 diff
见 `progress/T5/raw/version_literal.diff:1`。

| file:line | 修订前 | 修订后 |
|---|---|---|
| `run_static_gates.sh:54` | `default_LLVM_21`、`single_at_LLVM_21` | `default_LLVM_22`、`single_at_LLVM_22` |
| `run_static_gates.sh:74` | `$8 ~ /@@LLVM_21$/` | `$8 ~ /@@LLVM_22$/` |
| `run_static_gates.sh:76` | `$8 ~ /@LLVM_21$/ && $8 !~ /@@LLVM_21$/` | `$8 ~ /@LLVM_22$/ && $8 !~ /@@LLVM_22$/` |
| `run_static_gates.sh:91` | `$8 !~ /@LLVM_21$/` | `$8 !~ /@LLVM_22$/` |
| `run_static_gates.sh:98` | `$8 ~ /@LLVM_21$/ && $8 !~ /@@LLVM_21$/` | `$8 ~ /@LLVM_22$/ && $8 !~ /@@LLVM_22$/` |

新工具 SHA256 为
`d24f7c234d266bf970dbfd9c24939052681eba2105c186d0002009891c119885`。
将新文件中的 `LLVM_22` 反向规范化为 `LLVM_21` 后，与旧原件逐字比较退出
0；`bash -n` 退出 0。自检汇总见 `progress/T5/self_check.tsv:1`。因此没有
非版本字面量逻辑差异。

## 42 项解冻重跑

先验证旧 v1.4 冻结清单 142/142，再用冻结 catalog、fixtures 和四个治理
门禁入口重跑全部 42 项。结果为 42/42 与预期一致，其中 5 GREEN、37 RED
（`progress/T5/matrix/summary.tsv:1`）。分工具结果：G6 7/7、G7 13/13、
EDGE_DIFF 7/7、PROMOTION_LEDGER 15/15，均 PASS
（`progress/T5/matrix/tool_status.tsv:1`）。

逐项矩阵、命令、stdout、stderr、退出码及工具生成 findings 全部保存在
`progress/T5/matrix/`；没有只重跑受影响项，也没有把预期红 fixture 误记为
失败。

## 新冻结登记

新登记为 `FROZEN_WAVE_EXECUTION_V1_5_LLVM_22`，记录五个工具版本、SHA 与
fixture 数；治理矩阵 fixture 总数为 42。libc++ 静态打包门本次 fixture 数
为 0，因为 T3 红停导致 T4 没有 LLVM 22 新制品，不能伪造制品级运行
（`gates/FROZEN_EXECUTION_VERSION.md:3`）。

旧、新符号断言工具登记并存于 `gates/SYMBOL_ASSERTION_FREEZE.tsv:1`，旧项
明确标为 `SUPERSEDED_BY_LLVM_22`。v1.4 冻结登记全文也保留在
`gates/FROZEN_EXECUTION_VERSION_V1_4_SUPERSEDED_BY_LLVM_22.md:1`。
新 `FREEZE_MANIFEST.sha256` 共 145 项，校验 145/145 通过。
