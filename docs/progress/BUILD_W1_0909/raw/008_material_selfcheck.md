# 材料自检

命令原文：

```bash
set -o pipefail
sha256sum -c docs/progress/BUILD_W1_0909/build_logs/SHA256SUMS
printf 'sha_check_rc=%s\n' "$?"
awk -F '\t' 'NR>1 {n++} END {print "matrix_rows=" n; exit(n==3?0:1)}' docs/progress/BUILD_W1_0909/MATRIX.tsv
printf 'matrix_check_rc=%s\n' "$?"
rg -n 'IN_PROGRESS|本次同一完整构建同时关闭|armv7l / 不启用 libc\+\+：`PASS`' docs/progress/BUILD_W1_0909 docs/progress/BUILD_STATUS_0909.md docs/progress/BUILD_QUESTIONS_0909.md || true
printf 'stale_marker_check_rc=%s\n' "$?"
git branch --show-current
git diff --check -- docs/progress/BUILD_W1_0909 docs/progress/BUILD_STATUS_0909.md docs/progress/BUILD_QUESTIONS_0909.md
printf 'diff_check_rc=%s\n' "$?"
```

输出：

```text
docs/progress/BUILD_W1_0909/build_logs/aarch64_gcc_nolibcxx_partial.log.gz: OK
docs/progress/BUILD_W1_0909/build_logs/armv7l_gcc_nolibcxx.log.gz: OK
docs/progress/BUILD_W1_0909/build_logs/x86_64_gcc_nolibcxx_lock_failure.log.gz: OK
docs/progress/BUILD_W1_0909/build_logs/x86_64_gcc_nolibcxx_partial.log.gz: OK
sha_check_rc=0
matrix_rows=3
matrix_check_rc=0
stale_marker_check_rc=0
codex/r5-ehabi-diagnosis
diff_check_rc=0
```

退出码：`0`。旧状态/旧结论检索无输出。
