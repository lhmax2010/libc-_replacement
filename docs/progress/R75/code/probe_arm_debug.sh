#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
out="${workspace}/progress/R75/raw/arm_debug_probe"
mkdir -p "${out}"

run_probe() {
  local name=$1
  shift
  {
    printf 'COMMAND:'
    printf ' %q' "$@"
    printf '\n'
  } > "${out}/${name}.log"
  set +e
  "$@" >> "${out}/${name}.log" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE: %s\n' "${rc}" >> "${out}/${name}.log"
}

run_probe 001_devices "${sdb}" devices
run_probe 002_tools "${sdb}" shell "for x in gdb gdbserver coredumpctl eu-stack addr2line readelf; do command -v \"\$x\" || true; done"
run_probe 003_core_config "${sdb}" shell "ulimit -c; cat /proc/sys/kernel/core_pattern 2>/dev/null || true; cat /proc/sys/kernel/core_uses_pid 2>/dev/null || true"
run_probe 004_storage "${sdb}" shell "df -h /opt/usr/home/owner/share/tmp; ls -ld /opt/usr/home/owner/share/tmp"
