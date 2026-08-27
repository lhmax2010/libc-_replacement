#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
cd "$workspace"

index=docs/progress/R68/INDEX.tsv
verified=0
while IFS=$'\t' read -r source upload size digest; do
  if [[ "$source" == "source" ]]; then
    continue
  fi
  test -f "$source"
  test -f "$upload"
  source_digest=$(sha256sum "$source")
  source_digest=${source_digest%% *}
  upload_digest=$(sha256sum "$upload")
  upload_digest=${upload_digest%% *}
  test "$source_digest" = "$digest"
  test "$upload_digest" = "$digest"
  actual_size=$(stat -c '%s' "$upload")
  test "$actual_size" = "$size"
  verified=$((verified + 1))
done <"$index"

source_count=$(find progress/R68 -type f | wc -l)
upload_count=$(find docs/progress/R68 -type f | wc -l)
index_count=$(( $(wc -l <"$index") - 1 ))
test "$source_count" -eq "$index_count"
test "$upload_count" -eq $((index_count + 1))

cmp -s \
  docs/progress/R68/code/sdb_executor.py \
  docs/progress/R68/code/sdb_executor.after_utf8_fix.py

comparison=tmp/R68/material_executor_comparison.diff
set +e
diff -u \
  docs/progress/R68/code/sdb_executor.before_utf8_fix.py \
  docs/progress/R68/code/sdb_executor.after_utf8_fix.py \
  >"$comparison"
diff_rc=$?
set -e
test "$diff_rc" -eq 1
diff_lines=$(wc -l <"$comparison")
test "$diff_lines" -eq 10

failure_json=docs/progress/R68/raw/capability/patched/armv7l/sample_actual_after_decode_fix/libcxx.json
failure_dir=docs/progress/R68/raw/capability/patched/armv7l/sample_actual_after_decode_fix/failures
failure_count=$(jq '[.tests[] | select(.code == "FAIL")] | length' "$failure_json")
test "$failure_count" -eq 4
failure_files=$(find "$failure_dir" -name '*.txt' -type f | wc -l)
test "$failure_files" -eq 4

printf 'verified_index_entries=%d\n' "$verified"
printf 'source_files=%d\n' "$source_count"
printf 'upload_files_including_index=%d\n' "$upload_count"
printf 'executor_diff_exit_code=%d\n' "$diff_rc"
printf 'executor_diff_lines=%d\n' "$diff_lines"
printf 'failure_count=%d\n' "$failure_count"
printf 'failure_files=%d\n' "$failure_files"
