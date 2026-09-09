# 项目仓提交与推送记录

提交命令原文：

```bash
git add -- docs/progress/BUILD_W1_0909 docs/progress/BUILD_STATUS_0909.md docs/progress/BUILD_QUESTIONS_0909.md
git diff --cached --name-only
git status --short
git commit -m 'Document incomplete LLVM validation gate' -m '记录三架构门禁矩阵、八小时时限中止与未推送结论。'
printf 'commit_exit=%s\n' "$?"
```

结果：提交 `c8d6d336352ddbf33ba60a906818576937fa9037`，退出码 `0`；仅暂存
并提交列明的 W1、共同状态与问题文件，工作区已有的 R111、R114、R115
等无关改动未暂存。

推送与核对命令原文：

```bash
git push origin codex/r5-ehabi-diagnosis
push_rc=$?
local_sha=$(git rev-parse HEAD)
remote_sha=$(git ls-remote origin refs/heads/codex/r5-ehabi-diagnosis | awk '{print $1}')
printf 'push_exit=%s\nlocal_sha=%s\nremote_sha=%s\n' "$push_rc" "$local_sha" "$remote_sha"
test "$push_rc" -eq 0 && test "$local_sha" = "$remote_sha"
printf 'verify_exit=%s\n' "$?"
```

输出：

```text
To github.com:lhmax2010/libc-_replacement.git
   2b8c3fc..c8d6d33  codex/r5-ehabi-diagnosis -> codex/r5-ehabi-diagnosis
push_exit=0
local_sha=c8d6d336352ddbf33ba60a906818576937fa9037
remote_sha=c8d6d336352ddbf33ba60a906818576937fa9037
verify_exit=0
```

未使用 force，未推送 package 仓或其他分支。
