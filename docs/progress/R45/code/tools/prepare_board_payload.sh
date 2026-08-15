#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
payload="$workspace/tmp/R45/board-payload"
archive="$workspace/tmp/R45/r45_board_payload.tar"
arm_baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"

mkdir -p "$payload"/{bin/libcxx,bin/libstdcxx,runtime/unpatched,runtime/r42,runtime/r45,aggregation/bin,aggregation/unpatched,aggregation/layer1,aggregation/fixed}
cp "$workspace/tmp/R45/tests/armv7l/libcxx/"{forced_unwind1,forced_unwind2,forced_unwind3,forced_unwind4,unwind_01,unwind_02,unwind_03,unwind_04,unwind_05,unwind_06,s1_rethrow_probe,unpatched_cancel_probe} "$payload/bin/libcxx/"
cp "$workspace/tmp/R45/tests/armv7l/libstdcxx/"{forced_unwind1,forced_unwind2,forced_unwind3,forced_unwind4,unwind_01,unwind_02,unwind_03,unwind_04,unwind_05,unwind_06,s1_rethrow_probe} "$payload/bin/libstdcxx/"
cp "$workspace/tmp/R38/build-unpatched-armv7l/lib/libc++abi.so.1.0" "$payload/runtime/unpatched/libc++abi.so.1"
cp "$workspace/tmp/R42/build-patched-armv7l/lib/libc++abi.so.1.0" "$payload/runtime/r42/libc++abi.so.1"
cp "$workspace/tmp/R45/build-armv7l/lib/libc++abi.so.1.0" "$payload/runtime/r45/libc++abi.so.1"

cp "$workspace/tmp/R44b/tests/armv7l/libcxx/stdlib_cancel_probe" "$payload/aggregation/bin/libcxx_probe"
cp "$workspace/tmp/R44b/tests/armv7l/libstdcxx/stdlib_cancel_probe" "$payload/aggregation/bin/libstdcxx_probe"
cp "$arm_baseline/libc++.so.1.0" "$payload/aggregation/unpatched/libc++.so.1"
cp "$workspace/tmp/R38/build-unpatched-armv7l/lib/libc++abi.so.1.0" "$payload/aggregation/unpatched/libc++abi.so.1"
cp "$arm_baseline/libc++.so.1.0" "$payload/aggregation/layer1/libc++.so.1"
cp "$workspace/tmp/R45/build-armv7l/lib/libc++abi.so.1.0" "$payload/aggregation/layer1/libc++abi.so.1"
cp "$workspace/tmp/R44b/build/armv7l/libc++.so.1.0" "$payload/aggregation/fixed/libc++.so.1"
cp "$workspace/tmp/R45/build-armv7l/lib/libc++abi.so.1.0" "$payload/aggregation/fixed/libc++abi.so.1"

find "$payload" -type f -exec chmod 0755 {} +
printf 'PAYLOAD_FILES_BEGIN\n'
find "$payload" -type f -print0 | sort -z | xargs -0 sha256sum
printf 'PAYLOAD_FILES_END\n'
tar -C "$payload" -cf "$archive" .
sha256sum "$archive"
