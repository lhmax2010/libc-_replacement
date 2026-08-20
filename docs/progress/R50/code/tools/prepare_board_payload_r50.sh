#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
payload="$workspace/tmp/R50/board-payload"
archive="$workspace/tmp/R50/r50_board_payload.tar"
base="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
r45="$workspace/tmp/R45/build-armv7l/lib/libc++abi.so.1.0"
patched="$workspace/tmp/R50/build/armv7l/libc++.so.1.0"
tests="$workspace/tmp/R50/tests/armv7l"
rm -rf "$payload"
mkdir -p "$payload"/{bin,dso,unpatched,layer_ab,key}
cp "$tests/dso_launcher" "$payload/bin/dso_launcher"
cp "$tests/libunpatched.so" "$payload/dso/unpatched.so"
cp "$tests/libpatched.so" "$payload/dso/patched.so"
cp "$tests/liblibstdcxx.so" "$payload/dso/libstdcxx.so"
cp "$base/libc++.so.1.0" "$payload/unpatched/libc++.so.1"
cp "$base/libc++abi.so.1.0" "$payload/unpatched/libc++abi.so.1"
cp "$base/libc++.so.1.0" "$payload/layer_ab/libc++.so.1"
cp "$r45" "$payload/layer_ab/libc++abi.so.1"
cp "$patched" "$payload/key/libc++.so.1"
cp "$r45" "$payload/key/libc++abi.so.1"
chmod 0755 "$payload/bin/dso_launcher" "$payload/dso"/*.so
find "$payload/unpatched" "$payload/layer_ab" "$payload/key" -type f -exec chmod 0644 {} +
(cd "$payload" && find bin dso unpatched layer_ab key -type f -print0 | sort -z | xargs -0 sha256sum > MANIFEST.sha256)
tar -C "$payload" -cf "$archive" .
sha256sum "$archive"
cat "$payload/MANIFEST.sha256"
