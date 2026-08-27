#!/usr/bin/env bash
set -euo pipefail

source_dir=${1:?source directory required}
build_dir=${2:?build directory required}
output_dir=${3:?output directory required}

mkdir -p "$output_dir"
lit="$source_dir/llvm/utils/lit/lit.py"

for suite in libcxx libcxxabi; do
  test_dir="$build_dir/$suite/test"
  show_stdout="$output_dir/$suite.show-tests.stdout"
  show_stderr="$output_dir/$suite.show-tests.stderr"
  noexec_stdout="$output_dir/$suite.no-execute.stdout"
  noexec_stderr="$output_dir/$suite.no-execute.stderr"
  noexec_json="$output_dir/$suite.no-execute.json"

  show_cmd=(python3 "$lit" --show-tests "$test_dir")
  printf 'SHOW_TESTS_COMMAND='
  printf '%q ' "${show_cmd[@]}"
  printf '\n'
  "${show_cmd[@]}" >"$show_stdout" 2>"$show_stderr"
  rc=$?
  printf 'SHOW_TESTS_EXIT_CODE=%d SUITE=%s\n' "$rc" "$suite"

  noexec_cmd=(nice -n 15 ionice -c 3 python3 "$lit" -j 2 --no-execute
    --show-unsupported --show-skipped -o "$noexec_json" "$test_dir")
  printf 'NO_EXECUTE_COMMAND='
  printf '%q ' "${noexec_cmd[@]}"
  printf '\n'
  start=$(date +%s)
  "${noexec_cmd[@]}" >"$noexec_stdout" 2>"$noexec_stderr"
  rc=$?
  elapsed=$(( $(date +%s) - start ))
  printf 'NO_EXECUTE_EXIT_CODE=%d SUITE=%s ELAPSED_SECONDS=%d\n' "$rc" "$suite" "$elapsed"
done
