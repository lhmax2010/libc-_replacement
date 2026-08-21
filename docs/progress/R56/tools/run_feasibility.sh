#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
out="$workspace/tmp/R56/feasibility"
raw="$workspace/progress/R56/raw/feasibility"
commands="$workspace/progress/R56/commands"
tests="$workspace/progress/R56/code/tests"
cxx="$workspace/progress/R33/tools/tizen-clang++"
fixed_headers="$workspace/tmp/R55/headers/x86_64/include/c++/v1"

mkdir -p "$out" "$raw" "$commands"

run_case() {
  local sequence=$1
  local name=$2
  shift 2
  local command_file="$commands/${sequence}_${name}.command.txt"
  local exit_file="$commands/${sequence}_${name}.exit.txt"
  local output_file="$raw/${name}.log"

  printf '%q ' "$@" > "$command_file"
  printf '\n' >> "$command_file"
  set +e
  "$@" > "$output_file" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE=%d\n' "$rc" > "$exit_file"
  printf '%s\t%d\n' "$name" "$rc"
}

run_case 003 compiler_version "$cxx" --version

run_case 004 forward_libcxx \
  nice -n 15 ionice -c 3 "$cxx" -std=c++17 -O0 -g -fexceptions \
  -stdlib=libc++ -nostdinc++ -I"$fixed_headers" -c \
  "$tests/forward_decl_catch.cpp" -o "$out/forward_libcxx.o"

run_case 005 forward_libstdcxx \
  nice -n 15 ionice -c 3 "$cxx" -std=c++17 -O0 -g -fexceptions \
  -stdlib=libstdc++ -c "$tests/forward_decl_catch.cpp" \
  -o "$out/forward_libstdcxx.o"

run_case 006 include_libcxx \
  nice -n 15 ionice -c 3 "$cxx" -std=c++17 -O0 -g -fexceptions \
  -stdlib=libc++ -nostdinc++ -I"$fixed_headers" -c \
  "$tests/include_cxxabi_catch.cpp" -o "$out/include_libcxx.o"

run_case 007 include_libstdcxx \
  nice -n 15 ionice -c 3 "$cxx" -std=c++17 -O0 -g -fexceptions \
  -stdlib=libstdc++ -c "$tests/include_cxxabi_catch.cpp" \
  -o "$out/include_libstdcxx.o"

exit 0
