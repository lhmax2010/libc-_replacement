# 未经日志器的只读命令追记

下列为准备脚本副本时的只读工具调用，均退出 0；输出用于生成 apply_patch，不是原始日志器输出。其源码快照已保存。

```sh
sed -n '1,400p' progress/ARM_LZMA_0921B/guard.py
sed -n '1,400p' progress/ARM_LZMA_0921B/preserve_previous.py
sed -n '1,400p' progress/ARM_LZMA_0921B/capture_failure.py
sed -n '1,400p' progress/ARM_LZMA_0921B/gdb_observer.py
sed -n '1,200p' progress/ARM_LZMA_MODE_0921/publish_final.py
# 上述每条 exit 0
```

其他开场检查命令及首次日志器复制命令见 PLAN.md。复现、取证、提交与推送命令均由日志器记录；末尾归档命令的记录另落原任务 raw/138 起，避免把正在写入的归档日志自身纳入源快照。
