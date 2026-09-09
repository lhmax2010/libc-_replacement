#!/bin/sh
set -eu

: "${QEMU_ROOT:?set QEMU_ROOT to the aarch64 chroot root}"
: "${QEMU_OPT:?set QEMU_OPT to the aarch64 opt binary}"
: "${QEMU_HOST_LD_LIBRARY_PATH:?set QEMU_HOST_LD_LIBRARY_PATH to host-side target library paths}"

exec /usr/bin/qemu-arm64-static \
  "$QEMU_ROOT/usr/lib64/ld-linux-aarch64.so.1" \
  --library-path "$QEMU_HOST_LD_LIBRARY_PATH" \
  "$QEMU_OPT" "$@"
