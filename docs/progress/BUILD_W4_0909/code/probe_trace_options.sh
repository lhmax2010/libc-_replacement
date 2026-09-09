#!/bin/bash
# Trace-only driver probe: avoids executing target cc1/linker while proving option handling.
set -u

if [ "$#" -ne 3 ]; then
  echo "usage: $0 ARCH ROOT FAMILY" >&2
  exit 2
fi

arch=$1
root=$2
family=$3
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

case "$family" in
  gcc)
    driver="$root/usr/bin/$triple-g++"
    # The reused armv7l buildroot lacks the LTO plugin at the driver's expected
    # host path.  Disable only linker-plugin use so -### can reach its linker
    # argv; this does not alter the option-under-test.
    trace_stabilizer=(-fno-use-linker-plugin)
    ;;
  clang)
    driver="$root/usr/bin/$triple-clang++"
    trace_stabilizer=()
    ;;
  *) echo "unsupported family: $family" >&2; exit 2 ;;
esac
test -x "$driver" || { echo "driver not executable: $driver" >&2; exit 3; }
runner=("${loader[@]}" "$driver")

run_trace() {
  local label=$1
  shift
  local output rc
  printf 'COMMAND[%s]=' "$label"
  printf '%q ' "${runner[@]}" "$@"
  printf '\n'
  output=$("${runner[@]}" "$@" 2>&1)
  rc=$?
  printf 'RESULT[%s]\trc=%s\n' "$label" "$rc"
  printf 'OUTPUT[%s]_BEGIN\n%s\nOUTPUT[%s]_END\n' "$label" "$output" "$label"
}

printf 'ARCH=%s\nFAMILY=%s\nDRIVER=%s\n' "$arch" "$family" "$driver"
run_trace version --version
run_trace stdlib -### "${trace_stabilizer[@]}" -stdlib=libc++ -x c++ -c -o /dev/null /dev/null
run_trace no_as_needed -### "${trace_stabilizer[@]}" -Wl,--no-as-needed -x c++ -o /tmp/w4-trace /dev/null
run_trace cxxabi -### "${trace_stabilizer[@]}" -lc++abi -x c++ -o /tmp/w4-trace /dev/null
run_trace as_needed -### "${trace_stabilizer[@]}" -Wl,--as-needed -x c++ -o /tmp/w4-trace /dev/null
run_trace full -### "${trace_stabilizer[@]}" -stdlib=libc++ -Wl,--no-as-needed -lc++abi -Wl,--as-needed \
  -x c++ -o /tmp/w4-trace /dev/null
