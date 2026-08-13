#!/usr/bin/env bash
set -euo pipefail

base=${1:?workspace root}
out=${2:?result directory}
mkdir -p "$out"

cases=(
  baseline_cancel
  forced_guard_cancel
  no_catch_cancel
  a_observe_cancel
  a_detect_cancel
  a_real_std
  a_real_int
  b_cancel
  b_real_std
  b_real_int
  c_restore_cancel
  c_no_restore_cancel
  d_cancel
  d_real_std
  d_real_int
)

for stdlib in libstdcxx libcxx; do
  if [[ "$stdlib" == libstdcxx ]]; then
    root="$base/tmp/GBS-ROOT/R32-candidates-libstdcxx-x86_64-20260813-01/local/BUILD-ROOTS/scratch.x86_64.0"
  else
    root="$base/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
  fi
  loader="$root/lib64/ld-linux-x86-64.so.2"
  library_path="$root/usr/lib64:$root/lib64"
  binary="$base/tmp/R32/run/$stdlib/usr/bin/r32-forced-unwind-candidates"
  for case_name in "${cases[@]}"; do
    log="$out/${stdlib}_${case_name}.log"
    {
      printf 'EXECUTION=HOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU\n'
      printf 'UNAME_MACHINE=%s\n' "$(uname -m)"
      printf 'COMMAND='
      printf '%q ' timeout --signal=KILL 10s "$loader" --library-path "$library_path" "$binary" "$case_name"
      printf '\n'
    } >"$log"
    set +e
    timeout --signal=KILL 10s "$loader" --library-path "$library_path" "$binary" "$case_name" >>"$log" 2>&1
    rc=$?
    set -e
    printf 'EXIT_CODE=%d\n' "$rc" >>"$log"
    if (( rc > 128 )); then
      signal=$((rc - 128))
      printf 'SIGNAL_NUMBER=%d\n' "$signal" >>"$log"
      printf 'SIGNAL_NAME=%s\n' "$(kill -l "$signal" 2>/dev/null || printf UNKNOWN)" >>"$log"
    else
      printf 'SIGNAL_NUMBER=NONE\nSIGNAL_NAME=NONE\n' >>"$log"
    fi
  done
done
