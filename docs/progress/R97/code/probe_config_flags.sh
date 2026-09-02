#!/bin/bash
set -u

if [ "$#" -ne 3 ]; then
  echo "usage: $0 ARCH CONFIG_SET BUILDROOT" >&2
  exit 2
fi

arch=$1
config_set=$2
root=$3

case "$arch" in
  armv7l)
    triple=armv7l-tizen-linux-gnueabi
    runner=("$root/usr/bin/qemu-arm" -L "$root" "$root/usr/bin/$triple-clang")
    ;;
  aarch64)
    triple=aarch64-tizen-linux-gnu
    runner=("$root/usr/bin/qemu-aarch64" -L "$root"
            "$root/usr/lib64/ld-linux-aarch64.so.1"
            --library-path "$root/usr/lib64:$root/usr/lib"
            "$root/usr/bin/$triple-clang")
    ;;
  x86_64)
    triple=x86_64-tizen-linux-gnu
    runner=("$root/usr/bin/$triple-clang")
    ;;
  *)
    echo "unsupported arch: $arch" >&2
    exit 2
    ;;
esac

gcc_common=(
  -O2 -fno-inline-functions -g2 -gdwarf-4 -pipe -Wall
  -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector-strong
  -Wformat-security -fmessage-length=0 -frecord-gcc-switches
  -Wl,-z,relro,--as-needed
)
llvm_common=(
  -Os -fstack-protector -Wno-unused-command-line-argument
  -Wno-error=unused-but-set-variable -Wno-error=unused-command-line-argument
  -g2 -gdwarf-4 -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions
  -Wformat -Wformat-security -fmessage-length=0 -frecord-gcc-switches
)

case "$arch:$config_set" in
  armv7l:gcc)
    flags=("${gcc_common[@]}" --param=ssp-buffer-size=4 -march=armv7-a
      -mtune=cortex-a8 -mlittle-endian -mfpu=neon -mfloat-abi=softfp
      -mthumb -Wp,-D__SOFTFP__ -Wl,-O1 -Wl,--hash-style=gnu
      -Wa,-mimplicit-it=thumb -D_FILE_OFFSET_BITS=64)
    ;;
  armv7l:llvm)
    flags=("${llvm_common[@]}" -march=armv7-a -mtune=cortex-a8
      -mlittle-endian -mfpu=neon -mfloat-abi=softfp -mthumb
      -Wp,-D__SOFTFP__ -D_FILE_OFFSET_BITS=64)
    ;;
  aarch64:gcc)
    flags=("${gcc_common[@]}" -feliminate-unused-debug-types -Wformat
      -march=armv8-a+fp+simd+crc+crypto -mtune=cortex-a57.cortex-a53)
    ;;
  aarch64:llvm)
    flags=("${llvm_common[@]}" -march=armv8-a+fp+simd+crc+crypto
      -mtune=cortex-a53)
    ;;
  x86_64:gcc)
    flags=("${gcc_common[@]}" -feliminate-unused-debug-types
      --param=ssp-buffer-size=4 -fdiagnostics-color=never -m64
      -march=nehalem -msse4.2 -mfpmath=sse -fasynchronous-unwind-tables
      -fno-omit-frame-pointer)
    ;;
  x86_64:llvm)
    flags=("${llvm_common[@]}" -momit-leaf-frame-pointer
      -mllvm=-regalloc-enable-advisor=release
      -mllvm=-enable-ml-inliner=release -fdiagnostics-color=never -m64
      -march=nehalem -msse4.2 -mfpmath=sse -fasynchronous-unwind-tables
      -fno-omit-frame-pointer)
    ;;
  *)
    echo "unsupported combination: $arch:$config_set" >&2
    exit 2
    ;;
esac

invoke() {
  local label=$1
  shift
  local output rc status
  printf 'COMMAND[%s]=' "$label"
  printf '%q ' "${runner[@]}" "$@" -x c -c -o /dev/null -
  printf '\n'
  output=$(printf 'int r97_probe(void) { return 0; }\n' |
    "${runner[@]}" "$@" -x c -c -o /dev/null - 2>&1)
  rc=$?
  if [ "$rc" -ne 0 ]; then
    status=REJECTED
  elif [ -n "$output" ]; then
    status=ACCEPTED_WITH_DIAGNOSTIC
  else
    status=ACCEPTED
  fi
  printf 'RESULT[%s]\trc=%s\tstatus=%s\n' "$label" "$rc" "$status"
  if [ -n "$output" ]; then
    printf 'OUTPUT[%s]_BEGIN\n%s\nOUTPUT[%s]_END\n' "$label" "$output" "$label"
  fi
}

printf 'ARCH=%s\nCONFIG_SET=%s\nTRIPLE=%s\nBUILDROOT=%s\n' \
  "$arch" "$config_set" "$triple" "$root"
invoke compiler_version --version

index=0
for flag in "${flags[@]}"; do
  index=$((index + 1))
  invoke "flag_$(printf '%02d' "$index")=$flag" "$flag"
done

invoke full_flag_set "${flags[@]}"
