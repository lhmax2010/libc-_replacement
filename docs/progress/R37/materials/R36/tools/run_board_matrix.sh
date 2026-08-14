#!/bin/sh
set -u

remote=/var/tmp/r36_s1_20260814

run_case() {
  standard_library=$1
  mode=$2
  if [ "$standard_library" = libcxxabi ]; then
    command="LD_LIBRARY_PATH=$remote $remote/s1_libcxxabi $mode"
  else
    command="$remote/s1_libstdcxx $mode"
  fi
  printf '=== CASE standard_library=%s mode=%s ===\n' "$standard_library" "$mode"
  printf 'REMOTE_COMMAND=%s\n' "$command"
  if [ "$standard_library" = libcxxabi ]; then
    LD_LIBRARY_PATH="$remote" "$remote/s1_libcxxabi" "$mode"
  else
    "$remote/s1_libstdcxx" "$mode"
  fi
  rc=$?
  printf 'RUN_EXIT_CODE=%d\n' "$rc"
  if [ "$rc" -ne 0 ]; then
    printf 'MATRIX_FAIL_CLOSED=YES\n'
    return "$rc"
  fi
  printf 'CASE_ASSERTION=PASS\n'
}

for mode in cancel exit real foreign cancel-no-catch exit-no-catch; do
  run_case libcxxabi "$mode" || exit $?
  run_case libstdcxx "$mode" || exit $?
done

printf 'MATRIX_EXECUTED_CELLS=12\n'
printf 'MATRIX_PASS_CELLS=12\n'
printf 'MATRIX_RESULT=PASS\n'
