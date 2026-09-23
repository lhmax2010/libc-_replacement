# 状态：R4 CLOSED / 推送已核实、当前配方审计关闭

| R4部分 | 结果 | 证据 |
| --- | --- | --- |
| LLVM人工推送 | 5c169afc→da14498e→f203923a；两笔作者/提交人/签字均Hao Lin；与db5b49af树一致 | raw/R4_004–006、010 |
| 历史配方 | 26项按裁决关闭、27项排除范围、6个获准副本已推；旧记录不改 | LOCAL_SPECS_R4.md |
| runtime构建单元 | 历史固定快照证实独立SRPM，同llvm仓两份spec；实时OBS接入形式NOT_OBSERVED | HISTORICAL_METADATA_R4.json、FINAL_RESULT_R4.md |
| 当前reference差异 | 20260914.073422未列runtime及四类二进制，原因NOT_OBSERVED | METADATA_IDENTITIES_R4.json |
| 分支清单 | 12行/11仓，逐行远端MATCH | QUICKBUILD_BRANCHES_R4.tsv |
| 下一步 | 人工起QuickBuild，再与既有预期失败清单对账 | docs/LINE_STATUS.md |

## R3历史状态（已由R4裁决关闭，不作为当前阻断）

| R3部分 | 结果 | 证据 |
| --- | --- | --- |
| runtime四组 | 新口径全PASS，R2原始FAIL保留 | prep-check/runtime_R3/ |
| 第二笔 | db5b49afa…，与指定候选逐字节一致 | LOCAL_COMMITS_R3.json |
| 两笔/包仓 | 干净，ahead2/behind0，不推、无signoff | SANDBOX_COMMITS_R3.diff |
| ARM prep | 退出0；31.98秒；未build | prep-check/runtime_R3/prep-command_R3.json |
| 本地spec扫描 | 112路径；38 sandbox一致、15原仓一致、32差异（6获准）、27未判定 | spec-audit_R3/LOCAL_SPECS_R3.tsv |
| 门禁 | 两项之外26历史差异，按指令STOP_FOR_REVIEW | spec-audit_R3/EXTRA_DIFFERENCES_R3.diff |
| R2全根表 | 原样保留，不继续 | raw/R3_030 |

## R2历史状态（以下不替代R3）

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
