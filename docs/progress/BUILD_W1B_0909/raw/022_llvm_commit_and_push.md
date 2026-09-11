# LLVM 提交与推送核对

推送前：

```text
REMOTE_IMMEDIATELY_BEFORE_PUSH=c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13
LOCAL_TO_PUSH=f203923a1508c9344f5fc6b17bd8822f011655c4
FINAL_SPEC_MATCH_EXIT=0
PRE_PUSH_STATUS=
```

提交：

```text
f203923a1508c9344f5fc6b17bd8822f011655c4
parent c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13
author hao.lin <hao.lin@samsung.com>
subject packaging: add libc++ MLGO AOT assets
4 files changed, 33 insertions(+)
```

推送命令：

```bash
git push origin HEAD:refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
```

输出终态与退出码：

```text
remote: commit f203923: warning: too many message lines longer than 72 characters; manually wrap lines
To ssh://review.tizen.org:29418/platform/upstream/llvm
   c68f376fb..f203923a1  HEAD -> sandbox/lhmax2025/libcxx-ehabi-backport
EXIT_CODE=0
```

推送后核对：

```text
LOCAL_SHA=f203923a1508c9344f5fc6b17bd8822f011655c4
REMOTE_SHA=f203923a1508c9344f5fc6b17bd8822f011655c4
REMOTE_EQUALS_LOCAL=PASS
OLD_HEAD_IS_PARENT=0
```
