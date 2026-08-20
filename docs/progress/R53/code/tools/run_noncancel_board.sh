#!/usr/bin/env bash
set -u
set -o pipefail
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r53_20260820
identity="set +e; sha256sum '$remote/full/libc++.so.1' '$remote/full/libc++abi.so.1' '$remote/supplement/noncancel/before.so' '$remote/supplement/noncancel/after.so'; LD_LIBRARY_PATH='$remote/full' ldd '$remote/supplement/noncancel/after.so'; echo LDD_EXIT_CODE=\$?; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/full' '$remote/supplement/noncancel/launcher' '$remote/supplement/noncancel/after.so' getline_success; echo LD_DEBUG_EXIT_CODE=\$?; exit 0"
printf 'IDENTITY_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$identity"; printf '\n'
"$sdb" -s "$serial" shell "$identity" || exit 90
for mode in getline_success getline_failure packaged_exception string_normal; do
  for variant in before after; do
    printf '\nCELL_BEGIN matrix=noncancel arch=armv7l variant=%s test=%s\n' "$variant" "$mode"
    command="set +e; LD_LIBRARY_PATH='$remote/full' '$remote/supplement/noncancel/launcher' '$remote/supplement/noncancel/$variant.so' '$mode'; rc=\$?; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; exit 0"
    printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
    "$sdb" -s "$serial" shell "$command"; carrier_rc=$?
    printf 'SDB_CARRIER_EXIT_CODE=%d\nCELL_END matrix=noncancel arch=armv7l variant=%s test=%s\n' "$carrier_rc" "$variant" "$mode"
    ((carrier_rc == 0)) || exit 90
  done
done
exit 0
