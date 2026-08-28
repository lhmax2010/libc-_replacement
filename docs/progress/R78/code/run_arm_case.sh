#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
  echo "usage: $0 REMOTE_COMMAND" >&2
  exit 2
fi

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
root=/opt/usr/home/owner/share/tmp/r78_coexist_20260829
temporary=$(mktemp -d)
trap 'rm -rf -- "$temporary"' EXIT

set +e
"$sdb" -s "$serial" shell \
  "cd $root; LD_LIBRARY_PATH=$root/runtime:/usr/lib LD_DEBUG=libs $1; remote_result=\$?; echo __R78_REMOTE_EXIT_CODE=\$remote_result" \
  >"$temporary/stdout" 2>"$temporary/stderr"
sdb_result=$?
set -e

cat "$temporary/stdout"
cat "$temporary/stderr" >&2
if [[ $sdb_result -ne 0 ]]; then
  echo "__R78_SDB_EXIT_CODE=$sdb_result" >&2
  exit 90
fi

remote_result=$(sed -n 's/^__R78_REMOTE_EXIT_CODE=\([0-9][0-9]*\)\r*$/\1/p' \
  "$temporary/stdout" | tail -1)
if [[ -z "$remote_result" ]]; then
  echo "__R78_REMOTE_EXIT_CODE=NOT_OBSERVED" >&2
  exit 91
fi
exit "$remote_result"
