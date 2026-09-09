# 自检命令恢复记录

首次调用把含 Markdown 反引号的匹配式直接嵌入 shell 命令，shell 在真正
执行自检前即报：

```text
bash: -c: line 1: unexpected EOF while looking for matching ``'
```

该次 wrapper 退出码为 `2`，记录在 `012_selfcheck.*`。随后把同一组检查
写入脚本快照 `code/selfcheck.sh`，避免嵌套 shell quoting；重跑结果见
`013_selfcheck_retry.*`。这是命令包装错误，不是判据失败。
