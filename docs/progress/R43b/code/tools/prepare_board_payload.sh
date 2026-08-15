#!/bin/bash
set -eu

workspace=/home/toolchain/development/libc++_replacement
payload="$workspace/tmp/R43b/payload"
archive="$workspace/tmp/R43b/r43b-armv7l.tar.gz"
rm -rf "$payload"
mkdir -p "$payload/libcxx" "$payload/libstdcxx" "$payload/patched" "$payload/unpatched"
for suite in libcxx libstdcxx; do
  for source in "$workspace/progress/R43b/build/armv7l/$suite"/*; do
    [ -x "$source" ] || continue
    cp "$source" "$payload/$suite/"
  done
done
cp "$workspace/tmp/R42/deploy/armv7l/patched/libc++abi.so.1" "$payload/patched/libc++abi.so.1"
cp "$workspace/tmp/R42/deploy/armv7l/unpatched/libc++abi.so.1" "$payload/unpatched/libc++abi.so.1"
find "$payload/libcxx" "$payload/libstdcxx" -type f -exec /usr/bin/llvm-strip --strip-debug {} +
(
  cd "$payload"
  find . -type f ! -name MANIFEST.sha256 -print0 | sort -z | xargs -0 sha256sum > MANIFEST.sha256
)
tar --sort=name --mtime='UTC 2026-08-15' --owner=0 --group=0 --numeric-owner -czf "$archive" -C "$payload" .
printf 'ARCHIVE=%s\n' "$archive"
sha256sum "$archive"
du -h "$archive"
find "$payload" -type f -printf '%s %p\n' | sort -n
