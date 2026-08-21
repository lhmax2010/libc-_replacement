#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
payload="$workspace/tmp/R58/board-payload"
archive="$workspace/tmp/R58/r58-board-payload.tar"
rm -rf "$payload"
mkdir -p "$payload/bin" "$payload/runtime" "$payload/upstream/libcxx" "$payload/upstream/libstdcxx"
for variant in baseline a ab abc bc; do
  mkdir -p "$payload/runtime/$variant"
  cp -L "$workspace/tmp/R58/runtime/armv7l/$variant/libc++.so.1" "$payload/runtime/$variant/libc++.so.1"
  cp -L "$workspace/tmp/R58/runtime/armv7l/$variant/libc++abi.so.1" "$payload/runtime/$variant/libc++abi.so.1"
done
for name in facility_baseline facility_full facility_reference concurrency_full concurrency_reference catchall_baseline async_baseline async_full async_reference normal_baseline normal_full normal_reference; do
  cp "$workspace/tmp/R58/probes/armv7l/$name" "$payload/bin/$name"
done
cp "$workspace/tmp/R52/regression/armv7l/libcxx" "$payload/bin/abi_full"
cp "$workspace/tmp/R52/regression/armv7l/libstdcxx" "$payload/bin/abi_reference"
for file in "$workspace"/tmp/R51/tests/armv7l/libcxx/{forced_unwind{1,2,3,4},unwind_0{1,2,3,4,5,6}}; do cp "$file" "$payload/upstream/libcxx/"; done
for file in "$workspace"/tmp/R51/tests/armv7l/libstdcxx/{forced_unwind{1,2,3,4},unwind_0{1,2,3,4,5,6}}; do cp "$file" "$payload/upstream/libstdcxx/"; done
find "$payload" -type f -print0 | sort -z | xargs -0 sha256sum | sed "s|$payload/||" > "$payload/MANIFEST.sha256"
tar -C "$payload" -cf "$archive" .
sha256sum "$archive" "$payload/MANIFEST.sha256"

