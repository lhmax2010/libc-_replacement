#!/usr/bin/env bash
set -euo pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
root=/opt/usr/home/owner/share/tmp/r78_coexist_20260829

"$sdb" -s "$serial" shell \
  "ps -ef | awk '/r78_coexist_20260829|new_main|pen_wave_probe/ && !/awk/ {print; n++} END {if (n) exit 1}'"
"$sdb" -s "$serial" shell "rm -rf -- '$root'"
"$sdb" -s "$serial" shell \
  "if test -e '$root'; then echo ROOT_STATE=PRESENT; exit 1; else echo ROOT_STATE=ABSENT; fi"
"$sdb" -s "$serial" shell \
  "ps -ef | awk '/r78_coexist_20260829|new_main|pen_wave_probe/ && !/awk/ {print; n++} END {if (n) exit 1; print \"PROCESS_STATE=ABSENT\"}'"
echo RESULT=PASS
