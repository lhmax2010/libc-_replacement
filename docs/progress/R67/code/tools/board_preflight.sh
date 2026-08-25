#!/usr/bin/env bash
set -euo pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101

printf 'COMMAND='; printf '%q ' "$sdb" devices; printf '\n'
"$sdb" devices
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell 'uname -a; uname -m; cat /etc/tizen-release; ps -eo pid,ppid,args'; printf '\n'
"$sdb" -s "$serial" shell 'uname -a; uname -m; cat /etc/tizen-release; ps -eo pid,ppid,args'
printf 'COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "ps -eo pid,args | grep -E '/var/tmp/(r5[0-9]|r6[0-9])|R5[0-9]|R6[0-9]' | grep -v grep || true"; printf '\n'
"$sdb" -s "$serial" shell "ps -eo pid,args | grep -E '/var/tmp/(r5[0-9]|r6[0-9])|R5[0-9]|R6[0-9]' | grep -v grep || true"
