#!/usr/bin/env bash
set -euo pipefail
set -x

workspace=/home/toolchain/development/libc++_replacement
src="$workspace/progress/R14/experiment_sources/audit.c"
output="$workspace/tmp/R14/bin/r14_audit.so"

gcc -std=c11 -O2 -fPIC -fno-stack-protector -nostdlib -shared \
  -Wl,-soname,r14_audit.so "$src" -o "$output"
readelf -d "$output"
readelf --dyn-syms --wide "$output"
sha256sum "$src" "$output"
