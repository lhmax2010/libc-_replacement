#!/usr/bin/env bash
set -u
set -o pipefail

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
task_root=/var/tmp/r68_lit_20260827
shared_root=/opt/usr/home/owner/share/tmp

run_checked() {
  printf '$'
  printf ' %q' "$@"
  printf '\n'
  "$@"
  local rc=$?
  printf 'exit_code=%d\n' "$rc"
  return "$rc"
}

run_checked "$sdb" -s "$serial" shell "find $task_root -mindepth 1 -maxdepth 4 -print"
run_checked "$sdb" -s "$serial" shell "find $shared_root -maxdepth 1 -name 'run-*.tar' -print"
run_checked "$sdb" -s "$serial" shell "df -k /var/tmp /opt/usr"
run_checked "$sdb" -s "$serial" shell "du -sk $task_root"
run_checked "$sdb" -s "$serial" shell "ps -ef"

# This path was created by R68 and is the exact task root; no historical or
# unknown board content is included in the deletion target.
run_checked "$sdb" -s "$serial" shell "rm -rf $task_root"
run_checked "$sdb" -s "$serial" shell "test ! -e $task_root"
run_checked "$sdb" -s "$serial" shell "find $shared_root -maxdepth 1 -name 'run-*.tar' -print"
run_checked "$sdb" -s "$serial" shell "df -k /var/tmp /opt/usr"
run_checked "$sdb" -s "$serial" shell "ps -ef"
