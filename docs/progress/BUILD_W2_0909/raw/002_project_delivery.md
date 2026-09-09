# 项目仓提交与推送记录

提交与推送命令原文：

```bash
git diff --check -- docs/progress/BUILD_W2_0909 docs/progress/BUILD_STATUS_0909.md
git add -- docs/progress/BUILD_W2_0909 docs/progress/BUILD_STATUS_0909.md
git diff --cached --name-only
git commit -m 'Record unavailable downstream package adaptation' -m 'LLVM 门禁未闭合，按前置规则跳过 bcc-tools 与 bpftrace。'
commit_rc=$?
git push origin codex/r5-ehabi-diagnosis
push_rc=$?
local_sha=$(git rev-parse HEAD)
remote_sha=$(git ls-remote origin refs/heads/codex/r5-ehabi-diagnosis | awk '{print $1}')
printf 'commit_exit=%s\npush_exit=%s\nlocal_sha=%s\nremote_sha=%s\n' "$commit_rc" "$push_rc" "$local_sha" "$remote_sha"
test "$commit_rc" -eq 0 && test "$push_rc" -eq 0 && test "$local_sha" = "$remote_sha"
printf 'verify_exit=%s\n' "$?"
```

结果：

```text
commit_exit=0
push_exit=0
local_sha=818e28df7e2ead7ef1b99bfc1e231276c3b6ba0d
remote_sha=818e28df7e2ead7ef1b99bfc1e231276c3b6ba0d
verify_exit=0
```

提交仅含 W2 报告、门禁记录和共同状态行。未使用 force，未推送 package
仓或其他分支。
