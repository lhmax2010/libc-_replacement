#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
payload="$workspace/tmp/R52/regression-board-payload"
archive="$workspace/tmp/R52/r52_regression_board_payload.tar"
mkdir -p "$payload/bin" "$payload/runtime"
cp "$workspace/tmp/R52/regression/armv7l/libcxx" "$payload/bin/libcxx"
cp "$workspace/tmp/R52/regression/armv7l/libstdcxx" "$payload/bin/libstdcxx"
cp "$workspace/tmp/R51/build-product-armv7l/lib/libc++abi.so.1.0" "$payload/runtime/libc++abi.so.1"
chmod 0755 "$payload/bin/libcxx" "$payload/bin/libstdcxx"
chmod 0644 "$payload/runtime/libc++abi.so.1"
(cd "$payload" && find bin runtime -type f -print0 | sort -z | xargs -0 sha256sum > MANIFEST.sha256)
tar -C "$payload" -cf "$archive" .
sha256sum "$archive"
cat "$payload/MANIFEST.sha256"
