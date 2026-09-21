#!/bin/bash
set -e
target_root=/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/R105-boost-aarch64/local/BUILD-ROOTS/scratch.aarch64.0
exec /usr/bin/qemu-arm64-static -L "$target_root" "$target_root/lib64/ld-linux-aarch64.so.1" --library-path "$target_root/lib64:$target_root/usr/lib64" "$target_root/usr/bin/ld" "$@"
