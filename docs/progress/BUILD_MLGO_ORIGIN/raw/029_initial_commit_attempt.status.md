# 首次提交命令的技术性非零

`git diff --cached --check` 对逐字保存的命令输出和官方上游文件报告原有的
行尾空白，返回退出码 `2`，因此 `&&` 链在真正执行 `git commit` 之前停止。
这不属于内容判据失败，也没有产生提交。

修正方式：保留 raw 输出和官方文件的原始字节不动；重新执行时只对本任务
自行编写的 `REPORT.md`、`CONSUMER_ORIGIN.tsv`、`EXTERNAL_SOURCES.md` 与
`README.md` 做 whitespace 检查，然后提交整个任务目录。未放宽内容核对。

