#!/usr/bin/env bash
set -u

SCRATCH=/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b/local/BUILD-ROOTS/scratch.armv7l.0
EXTRACT=/home/toolchain/development/libc++_replacement/tmp/R6/elf-extract-armv7l-r3
export LD_LIBRARY_PATH="$EXTRACT/usr/lib:$SCRATCH/usr/lib:$SCRATCH/lib"
exec /usr/bin/qemu-arm-static -L "$SCRATCH" "$SCRATCH/lib/ld-linux.so.3" "$@"
