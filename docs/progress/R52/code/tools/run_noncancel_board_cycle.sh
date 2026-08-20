#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
payload="$workspace/tmp/R52/noncancel-board-payload"
archive="$workspace/tmp/R52/r52_noncancel_board_payload.tar"
shared=/opt/usr/home/owner/share/tmp/r52_noncancel_board_payload.tar
remote=/var/tmp/r52_noncancel_20260820
libcxx="$workspace/tmp/R52/rebuilt-libcxx/armv7l"
abi="$workspace/tmp/R51/build-product-armv7l/lib"

mkdir -p "$payload/bin" "$payload/dso" "$payload/runtime"
cp "$workspace/tmp/R52/noncancel/armv7l/launcher" "$payload/bin/launcher"
cp "$workspace/tmp/R52/noncancel/armv7l/before.so" "$payload/dso/before.so"
cp "$workspace/tmp/R52/noncancel/armv7l/after.so" "$payload/dso/after.so"
cp "$libcxx/libc++.so.1.0" "$payload/runtime/libc++.so.1"
cp "$abi/libc++abi.so.1.0" "$payload/runtime/libc++abi.so.1"
chmod 0755 "$payload/bin/launcher" "$payload/dso/"*.so
(cd "$payload" && find bin dso runtime -type f -print0 | sort -z | xargs -0 sha256sum > MANIFEST.sha256)
tar -C "$payload" -cf "$archive" .
sha256sum "$archive"
cat "$payload/MANIFEST.sha256"

preflight="set +e; test ! -e '$remote'; path_rc=\$?; count=\$(ps -ef | awk '\$8 ~ /(r52_noncancel_20260820|noncancel_probe)/ {n++} END {print n+0}'); printf 'REMOTE_PATH_ABSENT_RC=%d\\nTARGET_PROCESS_COUNT=%s\\n' \"\$path_rc\" \"\$count\"; test \"\$path_rc\" -eq 0 -a \"\$count\" -eq 0"
printf 'PREFLIGHT_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$preflight"; printf '\n'
"$sdb" -s "$serial" shell "$preflight"
printf 'PUSH_COMMAND='; printf '%q ' "$sdb" -s "$serial" push "$archive" "$shared"; printf '\n'
"$sdb" -s "$serial" push "$archive" "$shared"
deploy="set -e; mkdir '$remote'; tar -C '$remote' -xf '$shared'; chmod 0755 '$remote/bin/launcher' '$remote/dso/'*.so; cd '$remote'; sha256sum -c MANIFEST.sha256"
printf 'DEPLOY_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$deploy"; printf '\n'
"$sdb" -s "$serial" shell "$deploy"

identity="set +e; sha256sum '$remote/runtime/libc++.so.1' '$remote/runtime/libc++abi.so.1' '$remote/dso/before.so' '$remote/dso/after.so'; LD_LIBRARY_PATH='$remote/runtime' ldd '$remote/dso/after.so'; echo LDD_EXIT_CODE=\$?; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/runtime' '$remote/bin/launcher' '$remote/dso/after.so' getline_success; echo LD_DEBUG_EXIT_CODE=\$?; exit 0"
printf 'IDENTITY_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$identity"; printf '\n'
"$sdb" -s "$serial" shell "$identity"

for mode in getline_success getline_failure packaged_exception string_normal; do
  for variant in before after; do
    printf '\nCELL_BEGIN matrix=noncancel arch=armv7l variant=%s test=%s\n' "$variant" "$mode"
    command="set +e; LD_LIBRARY_PATH='$remote/runtime' '$remote/bin/launcher' '$remote/dso/$variant.so' '$mode'; rc=\$?; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; exit 0"
    printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$command"; printf '\n'
    "$sdb" -s "$serial" shell "$command"
    printf 'CELL_END matrix=noncancel arch=armv7l variant=%s test=%s\n' "$variant" "$mode"
  done
done

cleanup="set +e; pids=\$(ps -ef | awk '\$8 ~ /(r52_noncancel_20260820|noncancel_probe)/ {print \$2}'); if test -n \"\$pids\"; then kill \$pids 2>/dev/null; sleep 1; kill -9 \$pids 2>/dev/null; fi; rm -rf '$remote'; rm -f '$shared'; test ! -e '$remote'; a=\$?; test ! -e '$shared'; b=\$?; count=\$(ps -ef | awk '\$8 ~ /(r52_noncancel_20260820|noncancel_probe)/ {n++} END {print n+0}'); printf 'REMOTE_PATH_REMOVED_RC=%d\\nSHARED_ARCHIVE_REMOVED_RC=%d\\nTARGET_PROCESS_COUNT=%s\\n' \"\$a\" \"\$b\" \"\$count\"; test \"\$a\" -eq 0 -a \"\$b\" -eq 0 -a \"\$count\" -eq 0"
printf 'CLEANUP_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$cleanup"; printf '\n'
"$sdb" -s "$serial" shell "$cleanup"
