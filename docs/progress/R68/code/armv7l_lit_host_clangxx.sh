#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
host_sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
arm_sysroot="$workspace/tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0"
export LD_LIBRARY_PATH="$host_sysroot/usr/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

exec "$host_sysroot/usr/bin/clang++" \
  --target=armv7l-tizen-linux-gnueabi \
  --sysroot="$arm_sysroot" \
  -resource-dir "$host_sysroot/usr/lib64/clang/22" \
  --ld-path="$workspace/progress/R36/tools/armv7l-ld" \
  -Wno-unused-command-line-argument \
  "$@"
