#!/bin/bash
set -u

if [ "$#" -ne 4 ]; then
  echo "usage: $0 ARCH ROOT FAMILY LANGUAGE" >&2
  exit 2
fi

arch=$1
root=$2
family=$3
language=$4

case "$arch" in
  armv7l)
    triple=armv7l-tizen-linux-gnueabi
    loader=("$root/usr/bin/qemu-arm" -L "$root")
    ;;
  aarch64)
    triple=aarch64-tizen-linux-gnu
    loader=("$root/usr/bin/qemu-aarch64" -L "$root"
      "$root/usr/lib64/ld-linux-aarch64.so.1"
      --library-path "$root/usr/lib64:$root/usr/lib")
    ;;
  x86_64)
    triple=x86_64-tizen-linux-gnu
    loader=()
    ;;
  *) echo "unsupported arch: $arch" >&2; exit 2 ;;
esac

case "$family:$language" in
  gcc:c) driver="$root/usr/bin/$triple-gcc"; xlanguage=c ;;
  gcc:cxx) driver="$root/usr/bin/$triple-g++"; xlanguage=c++ ;;
  clang:c) driver="$root/usr/bin/$triple-clang"; xlanguage=c ;;
  clang:cxx) driver="$root/usr/bin/$triple-clang++"; xlanguage=c++ ;;
  *) echo "unsupported family/language: $family/$language" >&2; exit 2 ;;
esac

test -x "$driver" || { echo "driver not executable: $driver" >&2; exit 3; }
runner=("${loader[@]}" "$driver")
case "$language" in
  c) source='int main(void) { return 0; }' ;;
  cxx) source='int main() { return 0; }' ;;
esac

run_probe() {
  local label=$1
  shift
  local output rc status
  printf 'COMMAND[%s]=' "$label"
  printf '%q ' "${runner[@]}" "$@"
  printf '\n'
  output=$(printf '%s\n' "$source" | "${runner[@]}" "$@" 2>&1)
  rc=$?
  if [ "$rc" -ne 0 ]; then
    status=REJECTED
  elif [ -n "$output" ]; then
    status=ACCEPTED_WITH_DIAGNOSTIC
  else
    status=ACCEPTED_SILENT
  fi
  printf 'RESULT[%s]\trc=%s\tstatus=%s\n' "$label" "$rc" "$status"
  if [ -n "$output" ]; then
    printf 'OUTPUT[%s]_BEGIN\n%s\nOUTPUT[%s]_END\n' "$label" "$output" "$label"
  fi
}

tmp_output="/tmp/r100-${arch}-${family}-${language}-$$"
trap 'rm -f "$tmp_output"' EXIT

printf 'ARCH=%s\nROOT=%s\nFAMILY=%s\nLANGUAGE=%s\nDRIVER=%s\n' \
  "$arch" "$root" "$family" "$language" "$driver"
run_probe version --version
run_probe compile_stdlib -stdlib=libc++ -x "$xlanguage" -c -o /dev/null -
run_probe link_stdlib_only -stdlib=libc++ -x "$xlanguage" -o "$tmp_output" -
run_probe link_no_as_needed -Wl,--no-as-needed -x "$xlanguage" -o "$tmp_output" -
run_probe link_cxxabi -lc++abi -x "$xlanguage" -o "$tmp_output" -
run_probe link_as_needed -Wl,--as-needed -x "$xlanguage" -o "$tmp_output" -
run_probe link_full -stdlib=libc++ -Wl,--no-as-needed -lc++abi -Wl,--as-needed \
  -x "$xlanguage" -o "$tmp_output" -
run_probe trace_full -### -stdlib=libc++ -Wl,--no-as-needed -lc++abi -Wl,--as-needed \
  -x "$xlanguage" -o "$tmp_output" -
