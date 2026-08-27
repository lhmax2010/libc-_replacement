# SDB 执行器版本说明

- `sdb_executor.py`：第一步结束时使用的当前版本。
- `sdb_executor.after_utf8_fix.py`：当前版本的逐字节副本，便于成对审阅。
- `sdb_executor.before_utf8_fix.py`：依据修正补丁确定性恢复的修改前版本。它不是当时另存的原始文件，不冒充原件；与修改后版本的唯一源码差异是未向 `subprocess.run()` 传递 `errors="replace"`。
- `sdb_executor.utf8_fix.diff`：上述两个版本的完整 unified diff。

修改前行为的原始证据位于：

- `../raw/capability/patched/armv7l/sample_actual/libcxx.stdout`
- `../raw/capability/patched/armv7l/sample_actual/libcxx.stderr`
- `../raw/capability/patched/armv7l/sample_actual/libcxx.json`
- `../raw/capability/patched/armv7l/sample_actual/run_metadata.tsv`

修改后同一分片的原始证据位于：

- `../raw/capability/patched/armv7l/sample_actual_after_decode_fix/libcxx.stdout`
- `../raw/capability/patched/armv7l/sample_actual_after_decode_fix/libcxx.stderr`
- `../raw/capability/patched/armv7l/sample_actual_after_decode_fix/libcxx.json`
- `../raw/capability/patched/armv7l/sample_actual_after_decode_fix/run_metadata.tsv`
