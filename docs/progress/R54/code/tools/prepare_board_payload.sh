#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
payload="$workspace/tmp/R54/board-payload"
archive="$workspace/tmp/R54/r54_board_payload.tar"
rm -rf "$payload"
mkdir -p "$payload/bin" "$payload/runtime" "$payload/upstream/libcxx" "$payload/upstream/libstdcxx"
for variant in baseline a ab abc bc; do
  mkdir -p "$payload/runtime/$variant"
  cp -L "$workspace/tmp/R54/runtime/armv7l/$variant/libc++.so.1" "$payload/runtime/$variant/libc++.so.1"
  cp -L "$workspace/tmp/R54/runtime/armv7l/$variant/libc++abi.so.1" "$payload/runtime/$variant/libc++abi.so.1"
done
cp "$workspace/tmp/R54/probes/armv7l/baseline/facility_probe_libcxx" "$payload/bin/facility_baseline"
cp "$workspace/tmp/R54/probes/armv7l/full/facility_probe_libcxx" "$payload/bin/facility_full"
cp "$workspace/tmp/R54/probes/armv7l/full/facility_probe_libstdcxx" "$payload/bin/facility_reference"
cp "$workspace/tmp/R54/probes/armv7l/full/concurrency_probe_libcxx" "$payload/bin/concurrency_full"
cp "$workspace/tmp/R54/probes/armv7l/full/concurrency_probe_libstdcxx" "$payload/bin/concurrency_reference"
cp "$workspace/tmp/R54/probes/armv7l/baseline/catchall_rethrow_probe_libcxx" "$payload/bin/catchall"
cp "$workspace/tmp/R52/regression/armv7l/libcxx" "$payload/bin/abi_full"
cp "$workspace/tmp/R52/regression/armv7l/libstdcxx" "$payload/bin/abi_reference"
for file in "$workspace"/tmp/R51/tests/armv7l/libcxx/{forced_unwind{1,2,3,4},unwind_0{1,2,3,4,5,6}}; do cp "$file" "$payload/upstream/libcxx/"; done
for file in "$workspace"/tmp/R51/tests/armv7l/libstdcxx/{forced_unwind{1,2,3,4},unwind_0{1,2,3,4,5,6}}; do cp "$file" "$payload/upstream/libstdcxx/"; done
find "$payload" -type f -print0 | sort -z | xargs -0 sha256sum | sed "s|$payload/||" > "$payload/MANIFEST.sha256"
tar -C "$payload" -cf "$archive" .
sha256sum "$archive" "$payload/MANIFEST.sha256"
printf 'EXIT_CODE=0\n'
