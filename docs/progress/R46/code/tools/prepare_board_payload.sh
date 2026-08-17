#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
payload="$workspace/tmp/R46/board-payload"
archive="$workspace/tmp/R46/r46_board_payload.tar"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"

mkdir -p "$payload/bin" "$payload/dso" "$payload/unpatched" \
  "$payload/layer_ab" "$payload/badbit_only" "$payload/failbit"
cp "$workspace/tmp/R46/tests/armv7l/dso_launcher" "$payload/bin/dso_launcher"
cp "$workspace/tmp/R46/tests/armv7l/libunpatched_headers.so" "$payload/dso/unpatched_headers.so"
cp "$workspace/tmp/R46/tests/armv7l/libpatched_headers.so" "$payload/dso/patched_headers.so"
cp "$workspace/tmp/R46/tests/armv7l/liblibstdcxx.so" "$payload/dso/libstdcxx.so"
cp "$baseline/libc++.so.1.0" "$payload/unpatched/libc++.so.1"
cp "$baseline/libc++abi.so.1.0" "$payload/unpatched/libc++abi.so.1"
cp "$baseline/libc++.so.1.0" "$payload/layer_ab/libc++.so.1"
cp "$workspace/tmp/R45/build-armv7l/lib/libc++abi.so.1.0" "$payload/layer_ab/libc++abi.so.1"
cp "$workspace/tmp/R44b/build/armv7l/libc++.so.1.0" "$payload/badbit_only/libc++.so.1"
cp "$workspace/tmp/R45/build-armv7l/lib/libc++abi.so.1.0" "$payload/badbit_only/libc++abi.so.1"
cp "$workspace/tmp/R46/libcxx-failbit/armv7l/libc++.so.1.0" "$payload/failbit/libc++.so.1"
cp "$workspace/tmp/R45/build-armv7l/lib/libc++abi.so.1.0" "$payload/failbit/libc++abi.so.1"
chmod 0755 "$payload/bin/dso_launcher" "$payload/dso/"*.so
find "$payload/unpatched" "$payload/layer_ab" "$payload/badbit_only" "$payload/failbit" -type f -exec chmod 0644 {} +
(cd "$payload" && find bin dso unpatched layer_ab badbit_only failbit -type f -print0 | \
  sort -z | xargs -0 sha256sum > MANIFEST.sha256)
tar -C "$payload" -cf "$archive" .
sha256sum "$archive"
cat "$payload/MANIFEST.sha256"
