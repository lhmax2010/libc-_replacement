#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r54_20260821
archive="$workspace/tmp/R54/r54_noncancel_supplement.tar"
shared=/opt/usr/home/owner/share/tmp/r54_noncancel_supplement.tar
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" push "$archive" "$shared"; printf '\n'
"$sdb" -s "$serial" push "$archive" "$shared"
printf 'TRANSFER_EXIT_CODE=0\n'
command="set -e; mkdir '$remote/noncancel'; tar -C '$remote/noncancel' -xf '$shared'; chmod 0755 '$remote/noncancel/noncancel_launcher' '$remote/noncancel/'*.so; cd '$remote/noncancel'; sha256sum -c MANIFEST.sha256"
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
"$sdb" -s "$serial" shell "$command"
printf 'DEPLOY_EXIT_CODE=0\n'
for mode in getline_success getline_failure packaged_exception string_normal; do
  for variant in before after; do
    command="set +e; LD_LIBRARY_PATH='$remote/runtime/abc' '$remote/noncancel/noncancel_launcher' '$remote/noncancel/noncancel_$variant.so' '$mode'; rc=\$?; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; exit 0"
    printf 'CELL=%s:%s COMMAND=' "$variant" "$mode"; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
    "$sdb" -s "$serial" shell "$command"
    printf 'SDB_CARRIER_EXIT_CODE=0\n'
  done
done
