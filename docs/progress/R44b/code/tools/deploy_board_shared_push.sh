#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
archive="$workspace/tmp/R44b/deploy/r44b-armv7l.tar.gz"
shared=/opt/usr/home/owner/share/tmp/r44b_payload.tar.gz
remote=/var/tmp/r44b_20260815

run() {
  printf 'COMMAND='; printf '%q ' "$@"; printf '\n'
  "$@"
  rc=$?
  printf 'EXIT_CODE=%d\n' "$rc"
  return "$rc"
}

run "$sdb" -s "$serial" push "$archive" "$shared" || exit 90
run "$sdb" -s "$serial" shell "sha256sum '$shared'" || exit 90
run "$sdb" -s "$serial" shell "rm -rf '$remote' && mkdir -p '$remote' && cp '$shared' '$remote/payload.tar.gz' && rm -f '$shared' && cd '$remote' && tar -xzf payload.tar.gz && rm -f payload.tar.gz && chmod 0755 bin/libcxx_probe bin/libstdcxx_probe && chmod 0644 unpatched/*.so.1 layer1/*.so.1 aggregation/*.so.1 && sha256sum -c MANIFEST.sha256" || exit 90
run "$sdb" -s "$serial" shell "test ! -e '$shared'; echo SHARED_TRANSFER_FILE_ABSENT_RC=\$?; find '$remote' -maxdepth 3 -type f -ls" || exit 90

run "$sdb" -s "$serial" shell "set +e; for suite in unpatched layer1 aggregation; do echo LOADER_LIST_SUITE=\$suite; LD_LIBRARY_PATH='$remote/'\$suite ldd '$remote/bin/libcxx_probe'; echo LDD_EXIT_CODE=\$?; done; echo LOADER_LIST_SUITE=libstdcxx_control; ldd '$remote/bin/libstdcxx_probe'; echo LDD_EXIT_CODE=\$?; exit 0" || exit 90
run "$sdb" -s "$serial" shell "set +e; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/aggregation' '$remote/bin/libcxx_probe' control; rc=\$?; echo LD_DEBUG_REMOTE_EXIT_CODE=\$rc; exit 0" || exit 90
