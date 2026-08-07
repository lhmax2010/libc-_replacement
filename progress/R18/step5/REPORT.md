# R18 第 5 步：既有 sandbox 分支推送

## 结论

`PASS_SANDBOX_PUSH_AND_VERIFICATION`

目标提交 `5ed6c77278dfa7a470667cf1a137723d3c96fe60` 已在首次尝试中，以非强制直推方式推送到既有分支：

`sandbox/lhmax2025/libcxx-ehabi-backport`

推送命令退出 0。没有新建 sandbox 分支，没有使用 `refs/for/`，没有强推、rebase 或改写历史，也没有推送 `tizen_base`。

## 推送前冻结

| 项目 | 值 | 结果 |
|---|---|---|
| LLVM 本地分支 | `tizen_base` | MATCH |
| LLVM 本地 HEAD | `5ed6c77278dfa7a470667cf1a137723d3c96fe60` | MATCH |
| LLVM 工作树 | `git status --porcelain` 零输出 | PASS |
| origin URL | `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/llvm` | MATCH |
| sandbox 推送前 SHA | `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b` | RECORDED |
| `origin/tizen_base` 推送前 SHA | `fd7a4929c0e424d564cdd1f0c55af9153dcea321` | RECORDED |
| sandbox → 目标提交 | 快进关系 | PASS |
| EHABI 提交在目标历史中 | 是 | PASS |

命令原文与退出码见 `commands/00_preflight_identity_and_remote_freeze.txt`；待推提交的完整 `git show` 原文见 `commands/01_target_commit_git_show.txt`。

## 推送

实际命令：

```text
git -C codes/llvm push origin 5ed6c77278dfa7a470667cf1a137723d3c96fe60:refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
```

- 尝试次数：`1/3`
- 退出码：`0`
- 远端报告：`3d3e3da49e00..5ed6c77278df`

完整原文见 `commands/02_sandbox_push.txt`。

## 推送后核对

| 核对项 | 推送前 | 推送后 | 结果 |
|---|---|---|---|
| sandbox SHA | `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b` | `5ed6c77278dfa7a470667cf1a137723d3c96fe60` | MATCH_TARGET |
| EHABI backport 历史 | `3d3e3da…` 为旧 HEAD | `merge-base --is-ancestor 3d3e3da… 5ed6c772…` 退出 0 | PASS |
| `origin/tizen_base` | `fd7a4929c0e424d564cdd1f0c55af9153dcea321` | 同值 | UNCHANGED |
| LLVM 本地工作树 | 干净 | 干净 | PASS |

完整核对原文见 `commands/03_postpush_verification.txt`。

## 覆盖边界

本任务只验证分支推送与 Git 历史/远端引用身份。没有启动、等待或轮询 QuickBuild；QuickBuild 结果由人工观察后回传。
