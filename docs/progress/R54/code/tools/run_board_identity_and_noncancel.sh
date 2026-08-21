#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r54_20260821
for item in \
  "$workspace/tmp/R52/noncancel/armv7l/launcher:$remote/bin/noncancel_launcher" \
  "$workspace/tmp/R52/noncancel/armv7l/before.so:$remote/bin/noncancel_before.so" \
  "$workspace/tmp/R52/noncancel/armv7l/after.so:$remote/bin/noncancel_after.so"; do
  source=${item%%:*}; target=${item#*:}
  printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" push "$source" "$target"; printf '\n'
  "$sdb" -s "$serial" push "$source" "$target"
  printf 'TRANSFER_EXIT_CODE=0\n'
done
identity="set +e; chmod 0755 '$remote/bin/noncancel_launcher' '$remote/bin/noncancel_before.so' '$remote/bin/noncancel_after.so'; sha256sum '$remote/runtime/abc/libc++.so.1' '$remote/runtime/abc/libc++abi.so.1' '$remote/bin/facility_full'; LD_LIBRARY_PATH='$remote/runtime/abc' ldd '$remote/bin/facility_full'; printf 'LDD_EXIT_CODE=%d\\n' \"\$?\"; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/runtime/abc' '$remote/bin/facility_full' arithmetic; printf 'LD_DEBUG_EXIT_CODE=%d\\n' \"\$?\"; exit 0"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$identity"; printf '\n'
"$sdb" -s "$serial" shell "$identity"
printf 'IDENTITY_EXIT_CODE=0\n'
for mode in getline_success getline_failure packaged_exception string_normal; do
  for variant in before after; do
    command="set +e; LD_LIBRARY_PATH='$remote/runtime/abc' '$remote/bin/noncancel_launcher' '$remote/bin/noncancel_$variant.so' '$mode'; rc=\$?; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; exit 0"
    printf 'CELL=%s:%s COMMAND=' "$variant" "$mode"; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
    "$sdb" -s "$serial" shell "$command"
    printf 'SDB_CARRIER_EXIT_CODE=0\n'
  done
done
