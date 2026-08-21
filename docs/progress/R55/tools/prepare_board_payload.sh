#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
payload="$workspace/tmp/R55/board-payload"
archive="$workspace/tmp/R55/r55-board-payload.tar"
rm -rf "$payload"
mkdir -p "$payload/bin" "$payload/runtime/baseline" "$payload/runtime/full" \
  "$payload/regression" "$payload/upstream/libcxx" "$payload/upstream/libstdcxx"

cp "$workspace/tmp/R55/probes/armv7l/async_probe_baseline_libcxx" "$payload/bin/async_baseline"
cp "$workspace/tmp/R55/probes/armv7l/async_probe_prefix_libcxx" "$payload/bin/async_prefix"
cp "$workspace/tmp/R55/probes/armv7l/async_probe_fixed_libcxx" "$payload/bin/async_fixed"
cp "$workspace/tmp/R55/probes/armv7l/async_probe_libstdcxx" "$payload/bin/async_libstdcxx"
cp "$workspace/tmp/R55/probes/armv7l/facility_probe_baseline_libcxx" "$payload/bin/facility_baseline"
cp "$workspace/tmp/R54/probes/armv7l/full/facility_probe_libcxx" "$payload/bin/facility_prefix"
cp "$workspace/tmp/R55/probes/armv7l/facility_probe_fixed_libcxx" "$payload/bin/facility_fixed"
cp "$workspace/tmp/R55/probes/armv7l/facility_probe_libstdcxx" "$payload/bin/facility_libstdcxx"
cp "$workspace/tmp/R54/board-payload/runtime/baseline/libc++.so.1" "$payload/runtime/baseline/"
cp "$workspace/tmp/R54/board-payload/runtime/baseline/libc++abi.so.1" "$payload/runtime/baseline/"
cp "$workspace/tmp/R54/board-payload/runtime/abc/libc++.so.1" "$payload/runtime/full/"
cp "$workspace/tmp/R54/board-payload/runtime/abc/libc++abi.so.1" "$payload/runtime/full/"
cp "$workspace/tmp/R52/regression/armv7l/libcxx" "$payload/regression/libcxx"
cp "$workspace/tmp/R52/regression/armv7l/libstdcxx" "$payload/regression/libstdcxx"
cp "$workspace/tmp/R54/board-payload/upstream/libcxx/"* "$payload/upstream/libcxx/"
cp "$workspace/tmp/R54/board-payload/upstream/libstdcxx/"* "$payload/upstream/libstdcxx/"
chmod 0755 "$payload/bin"/* "$payload/runtime"/*/*
chmod 0755 "$payload/regression/"* "$payload/upstream/libcxx/"* "$payload/upstream/libstdcxx/"*
sha256sum "$payload"/bin/* "$payload"/runtime/*/* "$payload"/regression/* \
  "$payload"/upstream/libcxx/* "$payload"/upstream/libstdcxx/* \
  > "$workspace/progress/R55/raw/armv7l/payload_sha256.txt"
tar -C "$payload" -cf "$archive" .
sha256sum "$archive" >> "$workspace/progress/R55/raw/armv7l/payload_sha256.txt"
