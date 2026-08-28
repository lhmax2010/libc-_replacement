#!/usr/bin/env bash
set -e
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
source="${workspace}/progress/R75/code/shrink_to_fit_reproducer.cpp"
out="${workspace}/progress/R75/raw/arm_build"
bin="${workspace}/progress/R75/bin/armv7l"
cxx="${workspace}/progress/R36/tools/armv7l-clang++"
include="${workspace}/tmp/R69/build-armv7l/include/c++/v1"
runtime="${workspace}/tmp/R69/build-armv7l/lib"
mkdir -p "${out}" "${bin}"

gnu_compile=("${cxx}" -std=c++17 -O0 -g -fexceptions -pthread -c "${source}" -o "${bin}/shrink_to_fit_reproducer.libstdcxx.o")
gnu_link=("${cxx}" "${bin}/shrink_to_fit_reproducer.libstdcxx.o" -pthread -Wl,-z,now -o "${bin}/shrink_to_fit_reproducer.libstdcxx")
libcxx_compile=("${cxx}" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ -I "${include}" -c "${source}" -o "${bin}/shrink_to_fit_reproducer.libcxx.o")
libcxx_link=("${cxx}" "${bin}/shrink_to_fit_reproducer.libcxx.o" -nostdlib++ -L "${runtime}" -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "${bin}/shrink_to_fit_reproducer.libcxx")

run_checked() {
  local log=$1
  shift
  {
    printf 'COMMAND:'
    printf ' %q' "$@"
    printf '\n'
  } > "${log}"
  set +e
  nice -n 15 ionice -c 3 "$@" >> "${log}" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE: %s\n' "${rc}" >> "${log}"
  return "${rc}"
}

run_checked "${out}/libstdcxx.compile.log" "${gnu_compile[@]}"
run_checked "${out}/libstdcxx.link.log" "${gnu_link[@]}"
run_checked "${out}/libcxx.compile.log" "${libcxx_compile[@]}"
run_checked "${out}/libcxx.link.log" "${libcxx_link[@]}"

{
  printf 'COMMAND: file and sha256sum armv7l products\n'
  file "${bin}/shrink_to_fit_reproducer.libstdcxx" "${bin}/shrink_to_fit_reproducer.libcxx"
  sha256sum \
    "${bin}/shrink_to_fit_reproducer.libstdcxx" \
    "${bin}/shrink_to_fit_reproducer.libcxx" \
    "${runtime}/libc++.so.1.0" \
    "${runtime}/libc++abi.so.1.0"
  printf 'EXIT_CODE: 0\n'
} > "${out}/identity.log"
