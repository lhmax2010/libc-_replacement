#!/bin/bash
set -u

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r39_s1_20260814

run_case() {
  local label=$1
  local binary=$2
  printf '=== CASE=%s ===\n' "$label"
  printf 'REMOTE_COMMAND=LD_LIBRARY_PATH=%s LD_DEBUG=bindings,symbols %s cancel\n' "$remote" "$binary"
  "$sdb" -s "$serial" shell "set +e; LD_LIBRARY_PATH='$remote' LD_DEBUG=bindings,symbols '$binary' cancel; case_rc=\$?; printf 'REMOTE_CASE_EXIT_CODE=%d\\n' \"\$case_rc\"; if test \"\$case_rc\" -gt 128; then printf 'REMOTE_CASE_SIGNAL=%d\\n' \"\$((case_rc - 128))\"; else printf 'REMOTE_CASE_SIGNAL=0\\n'; fi; exit 0"
  local carrier_rc=$?
  printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier_rc"
  return "$carrier_rc"
}

run_case R36 "$remote/r36_test" || exit $?
run_case R38_STATIC "$remote/r38_static_test" || exit $?
