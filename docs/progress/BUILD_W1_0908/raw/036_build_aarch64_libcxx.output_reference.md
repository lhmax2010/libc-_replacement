# aarch64 完整构建输出索引

GBS 顶层进程因 W1 时间上限被定向发送 `SIGTERM`，退出码为 `143`。
构建工具写入的逐行完整日志归档为：
`build_logs/full_build_aarch64_partial.log`。

归档 SHA256：
`02a142c545b529276c7e36c90ac9de070c897d8f4e706670aeef392ce61a653b`。

最终已完成至 Ninja `7376/7545`，随后日志明确记录：

```text
[ 6240s] ninja: build stopped: interrupted by user.
[ 6240s] error: Bad exit status from /var/tmp/rpm-tmp.2F7J1q (%build)
```

这不是候选代码的编译失败；它是达到时间上限后的主动中止结果。
