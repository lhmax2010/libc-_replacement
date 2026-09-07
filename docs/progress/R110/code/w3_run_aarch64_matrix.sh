#!/bin/bash
set -euo pipefail

root=$(cd "$(dirname "$0")/../../../.." && pwd)
out="$root/docs/progress/R110/build/w3"
runtime_root=/home/toolchain/development/libc++_replacement/tmp/R11/extracted
qemu="$runtime_root/41/4154ea903c230bf1f887ef759c43e5f7dcb48de108fd2445550ce1eaff3af553/usr/bin/qemu-aarch64"
glibc="$runtime_root/76/7606e2955a44b9255ca19db9bfc1ff9b96c047c5dd1aee7a85472a0a67af84a7"
libcxx="$runtime_root/7b/7b8399da9e44581207c907dd5a25d65c31a63eab074a9760d3bc3a11372e8657/usr/lib64"
libstdcxx="$runtime_root/d3/d303775042ed2f62dbc343c0c2d00fa1fc564925a67db35676c62c458dade500/usr/lib64"
libgcc="$runtime_root/c3/c3c00b54b4714c9aa4e3ab87b1eca43af33dbc62c4d49067be27270d8733023c/usr/lib64"
provider_root="$out/provider_overlay"
providers="$provider_root/usr/lib64"
loader="$glibc/lib64/ld-linux-aarch64.so.1"
library_path="$providers:$provider_root/lib64:$libcxx:$libstdcxx:$libgcc:$glibc/lib64"
commands="$out/runtime_commands.tsv"
results="$out/runtime_results.tsv"
: >"$commands"
: >"$results"
printf 'case\trun\tcommand\texit_code\n' >>"$commands"
printf 'case\trun\texit_code\tmaps_libcxx\tmaps_libstdcxx\tmaps_provider\tvalue_state\tdestruction_state\n' >>"$results"

for input in "$qemu" "$loader" \
  "$providers/libboost_unit_test_framework.so.1.83.0" \
  "$providers/libboost_thread.so.1.83.0" \
  "$providers/libboost_filesystem.so.1.83.0" \
  "$providers/libboost_iostreams.so.1.83.0" \
  "$providers/libboost_program_options.so.1.83.0" \
  "$out/boost_test_mixed_aarch64" "$out/boost_test_control_aarch64" \
  "$out/boost_thread_mixed_aarch64" "$out/boost_thread_control_aarch64" \
  "$out/boost_filesystem_mixed_aarch64" "$out/boost_filesystem_control_aarch64" \
  "$out/boost_iostreams_mixed_aarch64" "$out/boost_iostreams_control_aarch64" \
  "$out/boost_program_options_mixed_aarch64" "$out/boost_program_options_control_aarch64"; do
  test -r "$input"
done

run_one() {
  local case_name=$1
  local run=$2
  shift 2
  local stdout="$out/${case_name}.run${run}.stdout"
  local stderr="$out/${case_name}.run${run}.stderr"
  local -a command=(nice -n 15 ionice -c 3 "$qemu" "$loader" --library-path "$library_path" "$@")
  local rendered
  printf -v rendered '%q ' "${command[@]}"
  set +e
  "${command[@]}" >"$stdout" 2>"$stderr"
  local rc=$?
  set -e
  printf '%s\t%s\t%s\t%s\n' "$case_name" "$run" "$rendered" "$rc" >>"$commands"

  local maps_line
  maps_line=$(rg -m1 '^runtime_maps=' "$stdout" || true)
  local map_cxx=NOT_OBSERVED map_std=NOT_OBSERVED map_provider=NOT_OBSERVED
  if [[ "$maps_line" =~ libcxx:([01]) ]]; then map_cxx=${BASH_REMATCH[1]}; fi
  if [[ "$maps_line" =~ libstdcxx:([01]) ]]; then map_std=${BASH_REMATCH[1]}; fi
  if [[ "$maps_line" =~ provider:([01]) ]]; then map_provider=${BASH_REMATCH[1]}; fi

  local value_state=NOT_OBSERVED destruction_state=NOT_OBSERVED
  if [[ "$case_name" == boost_test_* ]]; then
    if rg -q '^observed_size=16 expected_size=16 value_ok=1$' "$stdout" &&
       rg -q '^observed_hex=616c7068615f626574615f67616d6d61$' "$stdout"; then
      value_state=EXACT_PASS
    elif rg -q '^observed_size=' "$stdout"; then
      value_state=OBSERVED_MISMATCH
    fi
    if rg -q '^consumer_destroy_completed=YES$' "$stdout" && rg -q '^dlclose_completed=YES$' "$stdout"; then
      destruction_state=COMPLETED
    elif rg -q '^consumer_destroy_begin=YES$' "$stdout"; then
      destruction_state=STARTED_NOT_COMPLETED
    fi
  elif [[ "$case_name" == boost_thread_* ]]; then
    if rg -q '^states=1,1,0 expected=1,1,0$' "$stdout"; then value_state=EXACT_PASS; fi
    if rg -q '^consumer_scope_exit_completed=YES$' "$stdout"; then destruction_state=COMPLETED_TRIVIAL; fi
  elif [[ "$case_name" == boost_filesystem_* ]]; then
    if rg -q '^sizeof_path=.* status_ok=1 value_ok=1 returned_size=11 returned=payload.txt$' "$stdout"; then
      value_state=EXACT_PASS
    elif rg -q '^sizeof_path=' "$stdout"; then value_state=OBSERVED_MISMATCH; fi
    if rg -q '^destruction_completed=1 result=' "$stdout"; then destruction_state=COMPLETED; fi
  elif [[ "$case_name" == boost_iostreams_* ]]; then
    if rg -q '^sizeof_params=.* open_ok=1 size_ok=1 value_ok=1 observed_size=19$' "$stdout"; then
      value_state=EXACT_PASS
    elif rg -q '^sizeof_params=' "$stdout"; then value_state=OBSERVED_MISMATCH; fi
    if rg -q '^destruction_completed=1 result=' "$stdout"; then destruction_state=COMPLETED; fi
  elif [[ "$case_name" == boost_program_options_* ]]; then
    if rg -q ' initial_size=0$' "$stdout" && rg -q '^after_size=1 value_ok=1$' "$stdout"; then
      value_state=EXACT_PASS
    elif rg -q '^sizeof_variables_map=' "$stdout"; then value_state=OBSERVED_MISMATCH; fi
    if rg -q '^destruction_completed=1 result=' "$stdout"; then destruction_state=COMPLETED; fi
  fi
  printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' \
    "$case_name" "$run" "$rc" "$map_cxx" "$map_std" "$map_provider" \
    "$value_state" "$destruction_state" >>"$results"
}

for run in 1 2 3 4 5; do
  run_one boost_test_mixed "$run" "$out/boost_test_mixed_aarch64" \
    "$providers/libboost_unit_test_framework.so.1.83.0"
  run_one boost_test_control "$run" "$out/boost_test_control_aarch64" \
    "$providers/libboost_unit_test_framework.so.1.83.0"
  run_one boost_thread_mixed "$run" "$out/boost_thread_mixed_aarch64"
  run_one boost_thread_control "$run" "$out/boost_thread_control_aarch64"
  run_one boost_filesystem_mixed "$run" "$out/boost_filesystem_mixed_aarch64"
  run_one boost_filesystem_control "$run" "$out/boost_filesystem_control_aarch64"
  run_one boost_iostreams_mixed "$run" "$out/boost_iostreams_mixed_aarch64"
  run_one boost_iostreams_control "$run" "$out/boost_iostreams_control_aarch64"
  run_one boost_program_options_mixed "$run" "$out/boost_program_options_mixed_aarch64"
  run_one boost_program_options_control "$run" "$out/boost_program_options_control_aarch64"
done

awk -F '\t' 'NR > 1 { count[$1]++; rc[$1 FS $3]++; value[$1 FS $7]++; destroy[$1 FS $8]++ }
  END { for (key in count) print key, count[key] }' "$results" | sort

test "$(awk -F '\t' '$1 == "boost_test_control" && $3 == 0 && $4 == 0 && $5 == 1 && $6 == 1 && $7 == "EXACT_PASS" && $8 == "COMPLETED" {n++} END {print n+0}' "$results")" -eq 5
test "$(awk -F '\t' '$1 == "boost_thread_control" && $3 == 0 && $4 == 0 && $5 == 1 && $6 == 1 && $7 == "EXACT_PASS" && $8 == "COMPLETED_TRIVIAL" {n++} END {print n+0}' "$results")" -eq 5
test "$(awk -F '\t' '$1 == "boost_test_mixed" && $4 == 1 && $5 == 1 && $6 == 1 {n++} END {print n+0}' "$results")" -eq 5
test "$(awk -F '\t' '$1 == "boost_thread_mixed" && $3 == 0 && $4 == 1 && $5 == 1 && $6 == 1 && $7 == "EXACT_PASS" && $8 == "COMPLETED_TRIVIAL" {n++} END {print n+0}' "$results")" -eq 5
test "$(awk -F '\t' '$1 ~ /_control$/ && $3 == 0 && $4 == 0 && $5 == 1 && $6 == 1 && $7 == "EXACT_PASS" && ($8 == "COMPLETED" || $8 == "COMPLETED_TRIVIAL") {n++} END {print n+0}' "$results")" -eq 25
test "$(awk -F '\t' '$1 ~ /_mixed$/ && $4 == 1 && $5 == 1 && $6 == 1 {n++} END {print n+0}' "$results")" -eq 25
printf 'ASSERTIONS=PASS architecture=aarch64 cases=10 repeats_per_case=5 controls=PASS maps=PASS\n'
