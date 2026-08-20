#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
payload="$workspace/tmp/R51/board-payload"
archive="$workspace/tmp/R51/r51_board_payload.tar"

mkdir -p "$payload"/{bin/libcxx,bin/libstdcxx,runtime/unpatched,runtime/prestrip,runtime/product}
cp "$workspace/tmp/R51/tests/armv7l/libcxx/"{forced_unwind1,forced_unwind2,forced_unwind3,forced_unwind4,unwind_01,unwind_02,unwind_03,unwind_04,unwind_05,unwind_06,s1_rethrow_probe,unpatched_cancel_probe} "$payload/bin/libcxx/"
cp "$workspace/tmp/R51/tests/armv7l/libstdcxx/"{forced_unwind1,forced_unwind2,forced_unwind3,forced_unwind4,unwind_01,unwind_02,unwind_03,unwind_04,unwind_05,unwind_06,s1_rethrow_probe} "$payload/bin/libstdcxx/"
cp "$workspace/tmp/R38/build-unpatched-armv7l/lib/libc++abi.so.1.0" "$payload/runtime/unpatched/libc++abi.so.1"
cp "$workspace/tmp/R45/build-armv7l/lib/libc++abi.so.1.0" "$payload/runtime/prestrip/libc++abi.so.1"
cp "$workspace/tmp/R51/build-product-armv7l/lib/libc++abi.so.1.0" "$payload/runtime/product/libc++abi.so.1"

find "$payload" -type f -exec chmod 0755 {} +
printf 'PAYLOAD_FILES_BEGIN\n'
find "$payload" -type f -print0 | sort -z | xargs -0 sha256sum
printf 'PAYLOAD_FILES_END\n'
tar -C "$payload" -cf "$archive" .
sha256sum "$archive"
