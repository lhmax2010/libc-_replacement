# 状态：R2 PARTIAL / 展开空行口径待确认

| R2部分 | 结果 | 证据 |
| --- | --- | --- |
| LLVM四组 | 全PASS | prep-check/equivalence_R2/ |
| 第一笔 | 已改正文，tree不变，617a2100… | LOCAL_COMMITS_R2.json |
| runtime | 默认组严格比较因12个开头空行未通过；第二笔未创建 | prep-check/runtime_R2/default.diff |
| runtime prep | NOT_OBSERVED | FINAL_RESULT_R2.md |
| 追加输入 | 每架构19份重算SHA：6 A / 1 B / 12获准C | inputs_R2/introduced_R2.json |
| 全根输入 | 未闭合 | inputs_R2/ALL_INPUTS_R2.tsv |
| 包仓 | ahead1/behind0，干净，未推 | export_commands_R2.json |

## R1 历史状态（保留）

| 部分 | 结果 | 证据 |
| --- | --- | --- |
| 两批LLVM配方身份 | PASS；同SHA、主spec只差授权一行 | SPEC_IDENTITY.json、BASELINE_TO_VERIFIED_LLVM.diff |
| libclang.a子包对应 | PASS；两架构llvm-static-devel，bpftrace有直接BR | LIBCLANG_PACKAGE.json |
| 本地候选 | 已提交、未推，ahead1/behind0、干净 | LOCAL_COMMIT.json、SANDBOX_COMMIT.diff |
| ARM prep | PASS、退出0、未进入build | prep-check/ |
| 三路径宏门禁 | Clang/未定义符合；指定GCC override不符合 | macro-result.json、gcc-baseline-result.json |
| 11包主配方对账 | 9仅VCS、1一致、LLVM1行待推 | RECIPE_AUDIT.tsv |
| 静态runtime供给 | 同仓runtime候选功能差异未推，其他服务端来源未观测 | UNPUSHED_LIBCXX_RUNTIME.diff |
| 项目材料 | 内容提交已推、远端核对一致；回执追加归档；包仓始终不推 | DELIVERY.md、raw/044–046 |

当前不进入修复、不推进QuickBuild。待人工裁决GCC门禁与runtime输入范围。
