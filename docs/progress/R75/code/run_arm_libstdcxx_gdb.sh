#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
remote=/opt/usr/home/owner/share/tmp/r75_gdb_20260828
binary="${workspace}/progress/R75/bin/armv7l/shrink_to_fit_reproducer.libstdcxx"
commands="${R75_GDB_COMMANDS:-${workspace}/progress/R75/code/arm_libstdcxx.gdb}"
out="${R75_GDB_OUT:-${workspace}/progress/R75/raw/arm_libstdcxx_gdb}"
mkdir -p "${out}"

run_checked() {
  local log=$1
  shift
  {
    printf 'COMMAND:'
    printf ' %q' "$@"
    printf '\n'
  } > "${log}"
  set +e
  "$@" >> "${log}" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE: %s\n' "${rc}" >> "${log}"
  return "${rc}"
}

run_checked "${out}/001_devices.log" "${sdb}" devices
run_checked "${out}/002_preflight.log" "${sdb}" shell "if test -e '${remote}'; then find '${remote}' -maxdepth 2 -print; else echo R75_GDB_PATH_ABSENT; fi"
run_checked "${out}/003_mkdir.log" "${sdb}" shell "mkdir -p '${remote}'"
run_checked "${out}/004_push_binary.log" "${sdb}" push "${binary}" "${remote}/shrink_to_fit_reproducer.libstdcxx"
run_checked "${out}/005_push_commands.log" "${sdb}" push "${commands}" "${remote}/arm_libstdcxx.gdb"
run_checked "${out}/006_identity.log" "${sdb}" shell "sha256sum '${remote}/shrink_to_fit_reproducer.libstdcxx'; file '${remote}/shrink_to_fit_reproducer.libstdcxx'; ldd '${remote}/shrink_to_fit_reproducer.libstdcxx'"

set +e
timeout --signal=TERM --kill-after=3s 45s "${sdb}" shell "cd '${remote}' && gdb -q -batch -x arm_libstdcxx.gdb --args ./shrink_to_fit_reproducer.libstdcxx" > "${out}/007_gdb.stdout" 2> "${out}/007_gdb.stderr"
gdb_transport_rc=$?
set -e
printf 'COMMAND: device gdb batch run of armv7l libstdc++ reproducer\nTRANSPORT_EXIT_CODE: %s\n' "${gdb_transport_rc}" > "${out}/007_gdb.command"

run_checked "${out}/008_cleanup.log" "${sdb}" shell "rm -rf '${remote}'"
run_checked "${out}/009_cleanup_verify.log" "${sdb}" shell "if test -e '${remote}'; then echo R75_GDB_PATH_REMAINS; exit 1; else echo R75_GDB_PATH_REMOVED; fi"

test "${gdb_transport_rc}" -eq 0
