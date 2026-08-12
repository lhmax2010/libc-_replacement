#!/usr/bin/env bash
set -euo pipefail

base=${1:?workspace root}
out=${2:?result directory}
mkdir -p "$out"

for stdlib in libstdcxx libcxx; do
  if [[ "$stdlib" == libstdcxx ]]; then
    root="$base/tmp/GBS-ROOT/R31-matrix-libstdcxx-x86_64-20260812-01/local/BUILD-ROOTS/scratch.x86_64.0"
  else
    root="$base/tmp/GBS-ROOT/R31-matrix-libcxx-x86_64-20260812-01/local/BUILD-ROOTS/scratch.x86_64.0"
  fi
  loader="$root/lib64/ld-linux-x86-64.so.2"
  library_path="$root/usr/lib64:$root/lib64"
  for case_name in bare-catchall forced-guard noexcept-catchall std-thread-catchall bare-no-catch; do
    binary="$base/tmp/R31/run/$stdlib/usr/bin/r31-$case_name"
    log="$out/${stdlib}_${case_name}.log"
    {
      printf 'EXECUTION=HOST_NATIVE_X86_64_TARGET_USERSPACE_NO_QEMU\n'
      printf 'UNAME_MACHINE=%s\n' "$(uname -m)"
      printf 'COMMAND='
      printf '%q ' timeout --signal=KILL 10s "$loader" --library-path "$library_path" "$binary"
      printf '\n'
    } >"$log"
    set +e
    timeout --signal=KILL 10s "$loader" --library-path "$library_path" "$binary" >>"$log" 2>&1
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
