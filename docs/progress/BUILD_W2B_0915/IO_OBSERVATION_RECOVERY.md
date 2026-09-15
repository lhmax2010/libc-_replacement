# I/O 等待状态的恢复记录

以下为本轮工具返回内容的摘录，由人工可读记录补入；不是日志包装器自动
写入的 stdout。原命令通过终端工具执行，整体退出码 0。

相关命令原文：

```bash
ps -p 136240 -o pid,ppid,stat,etime,wchan:30
```

实际输出：

```text
    PID    PPID STAT     ELAPSED WCHAN
 136240  136239 DN         00:31 wait_on_buffer
```

PID 136240 是 `raw/013_io_recheck.command.txt` 中运行的 Python I/O
探针。其最终超时退出码 124 由日志包装器自动写入
`raw/013_io_recheck.exitcode`。编译器磁盘等待另有自动记录
`raw/014_io_anomaly_stop.stdout.txt`，不依赖本恢复摘录。
