资源采样中的 `cat /proc/loadavg`、`MemAvailable` 与 `df` 成功；随后直接对
GBS buildroot 执行 `du -sh`，误进入 buildroot 绑定的 `/proc`，产生大量
`Permission denied` / 进程消失提示并令复合命令退出 1。该非零是检查命令
写法的技术性问题，不是资源判据失败，也未改变构建状态。后续不再递归遍历
buildroot；磁盘只用 `df` 采样。观测值：load 4.70/4.41/4.30，
MemAvailable 22630344 KiB，磁盘可用 123799604 KiB；均未触发停止条件。
