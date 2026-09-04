# 未经 `run_logged.sh` 包装的只读展示命令（恢复记录）

本文件是恢复记录，不是日志包装器自动生成。执行过程中有若干只读的
`sed`/`cat`/短 Python 统计用于在交互界面查看已经落盘的材料；它们的
退出码均为 0，但当时未单独重定向 stdout。其关键输入与输出已分别由
`003`、`004`、`005`、`012`、`016`、`017`、`018`、`020` 的正式落盘命令
重新覆盖；未用这些展示命令生成最终判定。

恢复的命令类别如下：

- `rg --files progress/R106 | sort | head -80` 与 `sed -n ... run_logged.sh`；
- `sed`/`cat` 查看 `003`、`006`、`008`、`012`、`014`、`015`、`016`、`018` 的已落盘 stdout/stderr；
- 短 Python 片段读取 repomd 的 `location`/`checksum`；正式下载与 checksum 由 `007`、`018` 覆盖；
- 短 Python 片段核对逐架构、唯一包名、R95 payload 与 primary Requires 的计数；最终结果由 `017` 生成到 TSV；
- `rg`/`awk` 查看 `xdgmime`、`bcc-tools`、`toybox` 的结果行；相同证据均保留在最终表与 `012`/`016` 中；
- `wc/head/rg` 查找 R95/R98 表结构和 step0 文档位置；正式输入盘点由 `003`、`005`、`011`、`012` 覆盖。

由于这些命令仅展示已有文件、没有产生或改变分析输入，未伪造其原始
stdout；缺少的那份即时 stdout 明确记为 `NOT_AVAILABLE_AS_SEPARATE_LOG`。

