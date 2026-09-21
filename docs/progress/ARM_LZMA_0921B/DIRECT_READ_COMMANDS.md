# 直接读取命令补记

以下只读命令在工具接口直接执行，未经过 run.py；现补记原文与工具返回退出码。其后各执行命令采用 raw/ 中的 command、exitcode、stdout、stderr 成组记录。apply_patch 的文件变更本身保存在对应脚本与文档中。

退出码均为 0：

```sh
pwd; git branch --show-current; ls progress/ARM_LZMA_0921B; tail -n 35 progress/ARM_LZMA_0921B/raw/045*stdout*
```

```sh
sed -n '1,180p' progress/ARM_LZMA_0921B/run.py; sed -n '1,240p' progress/ARM_LZMA_0921/guard.py; sed -n '1,140p' progress/ARM_LZMA_0921/preserve_previous.py; cat progress/ARM_REPRO_0920/original-argv.json
```

以下三条逐条退出 0，输出经 apply_patch 建立本轮脚本副本：

```sh
sed -n '1,400p' progress/ARM_LZMA_0921/capture_failure.py
sed -n '1,400p' progress/ARM_LZMA_0921/preserve_previous.py
sed -n '1,400p' progress/ARM_LZMA_0921/guard.py
```

```sh
sed -n '1,140p' progress/ARM_LZMA_0921B/STATUS.md; sed -n '1,100p' progress/ARM_LZMA_0921B/STAGE1_RESULT.md; sed -n '1,160p' progress/ARM_LZMA_0921B/publish_previous.py
```

以下读取输出经 apply_patch 保存为被中止尝试的 executed-guard.py：

```sh
sed -n '1,400p' progress/ARM_LZMA_0921B/guard.py
```

有效轮结束后，以下三条逐条退出 0；输出经 apply_patch 保存为 cells/gdb-preserved/executed-*.py，与执行时记录的哈希核对：

```sh
sed -n '1,400p' progress/ARM_LZMA_0921B/gdb_observer.py
sed -n '1,400p' progress/ARM_LZMA_0921B/guard.py
sed -n '1,400p' progress/ARM_LZMA_0921B/capture_failure.py
```
