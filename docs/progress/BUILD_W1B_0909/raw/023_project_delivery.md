# 本项目材料提交与推送

提交命令：

```bash
git add -- docs/progress/BUILD_W1B_0909
git diff --cached --check
git commit -m "docs: close the LLVM MLGO validation gate" ...
```

首次格式检查因 8 个新文件末尾多一个空行而退出 `2`，未创建提交；只删除
这些空行后重新执行，退出码 `0`：

```text
bb4decbc8593f887af974f2cb411e0f8909e4b50
17 files changed, 471 insertions(+), 111 deletions(-)
```

推送命令：

```bash
git push origin codex/r5-ehabi-diagnosis
```

输出与退出码：

```text
To github.com:lhmax2010/libc-_replacement.git
   94d1c74..bb4decb  codex/r5-ehabi-diagnosis -> codex/r5-ehabi-diagnosis
EXIT_CODE=0
```

本文件作为交付证据另行提交；只涉及本项目文档分支。
