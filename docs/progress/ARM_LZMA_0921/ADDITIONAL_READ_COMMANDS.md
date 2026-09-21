# 会话中的只读查看补记

下列 shell 查看在会话输出中保留，未经过 run.py；在此补记实际命令原文及返回码，不冒充后来重跑的结果。均为只读，未启动新实验。文件改动通过 apply_patch 完成，实际脚本快照与最终哈希另存。

```bash
date --iso-8601=seconds
pwd
tail -3 progress/ARM_LZMA_0921/cells/preload-original/events.jsonl
tail -3 progress/ARM_LZMA_0921/cells/preload-original/build.log
```

退出码 0；时刻 2026-09-21T13:33:09+08:00，guard running，最后 Wrote 为 llvm-devel-debuginfo。

```bash
sed -n '1,260p' progress/ARM_LZMA_0921/summarize.py
```

退出码 0。

```bash
sed -n '1,220p' progress/ARM_LZMA_0921/STATUS.md
sed -n '1,230p' progress/ARM_LZMA_0921/PREPARATION.md
sed -n '1,180p' progress/ARM_LZMA_0921/ARGV_DEVIATIONS.json
```

退出码 0。

```bash
sed -n '1,250p' tmp/ARM_LZMA_0921/lzma_tap.c
sed -n '1,140p' progress/ARM_LZMA_0921/run.py
ls -lh progress/ARM_LZMA_0921/host-instrumented.strace
```

退出码 0；当时 trace 的 ls 人类可读大小为 373M，最终精确大小见 raw/091。
