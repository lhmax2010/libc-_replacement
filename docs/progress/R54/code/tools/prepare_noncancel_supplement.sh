#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
payload="$workspace/tmp/R54/noncancel-supplement"
archive="$workspace/tmp/R54/r54_noncancel_supplement.tar"
test ! -e "$payload"
mkdir -p "$payload"
cp "$workspace/tmp/R52/noncancel/armv7l/launcher" "$payload/noncancel_launcher"
cp "$workspace/tmp/R52/noncancel/armv7l/before.so" "$payload/noncancel_before.so"
cp "$workspace/tmp/R52/noncancel/armv7l/after.so" "$payload/noncancel_after.so"
chmod 0755 "$payload"/*
find "$payload" -type f -print0 | sort -z | xargs -0 sha256sum | sed "s|$payload/||" > "$payload/MANIFEST.sha256"
tar -C "$payload" -cf "$archive" .
sha256sum "$archive"
