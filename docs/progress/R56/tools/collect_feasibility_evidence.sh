#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
raw="$workspace/progress/R56/raw"
commands="$workspace/progress/R56/commands"
out="$workspace/tmp/R56/feasibility"

mkdir -p "$raw/identity" "$raw/feasibility" "$commands"

run_logged() {
  local sequence=$1
  local name=$2
  shift 2
  local command_file="$commands/${sequence}_${name}.command.txt"
  local exit_file="$commands/${sequence}_${name}.exit.txt"
  local output_file="$raw/identity/${name}.log"

  printf '%q ' "$@" > "$command_file"
  printf '\n' >> "$command_file"
  set +e
  "$@" > "$output_file" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE=%d\n' "$rc" > "$exit_file"
  return "$rc"
}

run_logged 009 upstream_baseline git -C "$workspace/codes/llvm" rev-parse HEAD
run_logged 010 source_sha256 sha256sum \
  "$workspace/progress/R56/code/tests/forward_decl_catch.cpp" \
  "$workspace/progress/R56/code/tests/include_cxxabi_catch.cpp" \
  "$workspace/tmp/R55/source-fixed/libcxxabi/include/cxxabi.h" \
  "$workspace/tmp/R55/source-fixed/libcxx/include/future" \
  "$workspace/tmp/R55/source-fixed/libcxx/include/istream" \
  "$workspace/tmp/R55/source-fixed/libcxx/include/string"
run_logged 011 control_object_sha256 sha256sum \
  "$out/include_libcxx.o" "$out/include_libstdcxx.o"
run_logged 012 control_symbols readelf -Ws "$out/include_libcxx.o"
run_logged 013 control_symbols_libstdcxx readelf -Ws "$out/include_libstdcxx.o"
run_logged 014 existing_header_includes rg -n \
  'cxxabi.h|__forced_unwind' \
  "$workspace/tmp/R55/source-fixed/libcxx/include/future" \
  "$workspace/tmp/R55/source-fixed/libcxx/include/istream" \
  "$workspace/tmp/R55/source-fixed/libcxx/include/string"

exit 0
