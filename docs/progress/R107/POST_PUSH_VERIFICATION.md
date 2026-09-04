# R107 首次推送后核验

## 推送结果

- 推送命令未使用 `--force`、`+refspec` 或其他强制形式；完整输出见
  `raw/016_push_runtime_branch.*`。
- 新分支：`refs/heads/codex/runtime-validation`
- 首次远端提交：`f6020d79fa5da0c09673be8fb2a355f68be2685e`
- 推送前远端分支：3 条；推送后：4 条。
- 与 `remote_branches_before.tsv` 逐项比较，原有 3 条的名称和 SHA 完全相同。
- 唯一新增项为 `codex/runtime-validation`，其 SHA 与当时本地 HEAD 一致。

## 推送前后对照

| 分支 | 推送前 SHA | 首次推送后 SHA | 结论 |
|---|---|---|---|
| `codex/r3-t3-input-gap-evidence` | `c46342d4877812637a6cbc997c7e1ac6cfe25fb9` | `c46342d4877812637a6cbc997c7e1ac6cfe25fb9` | 未变 |
| `codex/r5-ehabi-diagnosis` | `e7790fc87f1cfcbd3620e51867263a16ce46fad5` | `e7790fc87f1cfcbd3620e51867263a16ce46fad5` | 未变 |
| `main` | `948bb9bf3cab95471f2908634a034b325aac5d42` | `948bb9bf3cab95471f2908634a034b325aac5d42` | 未变 |
| `codex/runtime-validation` | 不存在 | `f6020d79fa5da0c09673be8fb2a355f68be2685e` | 唯一新增 |

机器可读快照为 `remote_branches_before.tsv`、`remote_branches_after.tsv` 和
`remote_existing_after.tsv`。首次核验脚本因 `awk` 字段变量被 shell 提前展开
返回 1，完整保留为 `raw/017_*`；修正仅涉及核验脚本的引号/取字段方式，
`raw/018_*` 返回 0，并报告：

```text
EXISTING_BRANCHES_UNCHANGED=PASS
ADDED_BRANCH_COUNT=1
NEW_BRANCH=refs/heads/codex/runtime-validation
NEW_BRANCH_HEAD=f6020d79fa5da0c09673be8fb2a355f68be2685e
```

补录本核验材料会在新分支上形成后续文档提交；它只推进新分支，原三分支仍须
在最终推送后再次核验。
