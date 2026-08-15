#!/bin/bash
set -u

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
printf 'SDB_COMMAND='
printf '%q ' "$sdb" -s "$serial" shell \
  "uname -a; uname -m; cat /etc/tizen-release; ps -ef; df -h /var/tmp"
printf '\n'
"$sdb" -s "$serial" shell \
  "uname -a; uname -m; cat /etc/tizen-release; ps -ef; df -h /var/tmp"
rc=$?
printf 'SDB_EXIT_CODE=%d\n' "$rc"
exit "$rc"
