#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
payload="$workspace/tmp/R53/board-supplement"
archive="$workspace/tmp/R53/r53_board_supplement.tar"
rm -rf "$payload"
mkdir -p "$payload/regression" "$payload/r52" "$payload/noncancel"
cp "$workspace/tmp/R52/regression/armv7l/libcxx" "$payload/regression/libcxx"
cp "$workspace/tmp/R52/regression/armv7l/libstdcxx" "$payload/regression/libstdcxx"
cp "$workspace/tmp/R52/tests/armv7l/dso_launcher" "$payload/r52/dso_launcher"
cp "$workspace/tmp/R52/tests/armv7l/libunpatched_headers.so" "$payload/r52/unpatched_headers.so"
cp "$workspace/tmp/R52/tests/armv7l/libpatched_headers.so" "$payload/r52/patched_headers.so"
cp "$workspace/tmp/R52/tests/armv7l/liblibstdcxx.so" "$payload/r52/libstdcxx.so"
cp "$workspace/tmp/R52/noncancel/armv7l/launcher" "$payload/noncancel/launcher"
cp "$workspace/tmp/R52/noncancel/armv7l/before.so" "$payload/noncancel/before.so"
cp "$workspace/tmp/R52/noncancel/armv7l/after.so" "$payload/noncancel/after.so"
chmod 0755 "$payload/regression/"* "$payload/r52/"* "$payload/noncancel/"*
(cd "$payload" && find regression r52 noncancel -type f -print0 | sort -z | xargs -0 sha256sum > MANIFEST.sha256)
tar -C "$payload" -cf "$archive" .
sha256sum "$archive"
cat "$payload/MANIFEST.sha256"
