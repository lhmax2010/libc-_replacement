#!/bin/bash
set -euo pipefail
compiler_root=/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0
export LD_LIBRARY_PATH="$compiler_root/usr/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
exec "$compiler_root/usr/bin/clang" --no-default-config --sysroot="$compiler_root" -resource-dir "$compiler_root/usr/lib64/clang/22" "$@"
