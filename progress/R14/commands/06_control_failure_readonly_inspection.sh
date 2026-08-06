#!/usr/bin/env bash
set -euo pipefail
set -x

workspace=/home/toolchain/development/libc++_replacement
tmp_root="$workspace/tmp/R14"

for elf in \
  "$tmp_root/runtime/A/libnew_A.so" \
  "$tmp_root/runtime/A/liblegacy.so" \
  "$tmp_root/runtime/B/libnew_B.so" \
  "$tmp_root/runtime/B/liblegacy.so" \
  "$tmp_root/runtime/A/libc++abi.so.1.0" \
  "$tmp_root/runtime/B/libc++abi.so.1.0"; do
  printf '===== READONLY UNWIND CONSTRUCTION %s =====\n' "$elf"
  readelf --sections --wide "$elf" | awk 'NR <= 5 || /\.eh_frame|\.gcc_except_table/'
  readelf --segments --wide "$elf"
  readelf --unwind --wide "$elf"
done

python3 - <<'PY'
import csv
from collections import Counter

path = '/home/toolchain/development/libc++_replacement/progress/R14/tables/matrix_execution.tsv'
rows = list(csv.DictReader(open(path), delimiter='\t'))
same = [row for row in rows if row['direction'] in ('new_new', 'legacy_legacy')]
cross = [row for row in rows if row['direction'] in ('new_legacy', 'legacy_new')]
print('ALL', len(rows), Counter(row['outcome'] for row in rows))
print('SAME_SIDE', len(same), Counter(row['outcome'] for row in same))
print('CROSS_SIDE', len(cross), Counter(row['outcome'] for row in cross))
print('SAME_SIDE_CHECK_OBSERVED', Counter(row['type_check'] for row in same))
print('ALL_EVENT_COUNTS', Counter(row['event_count'] for row in rows))
PY
