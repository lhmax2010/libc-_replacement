#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
payload="$workspace/tmp/R53/board-payload"
archive="$workspace/tmp/R53/r53_board_payload.tar"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
abi="$workspace/tmp/R51/build-product-armv7l/lib/libc++abi.so.1.0"
full="$workspace/tmp/R53/build/armv7l/libc++.so.1.0"
tests="$workspace/tmp/R53/tests/armv7l-dso"
mkdir -p "$payload"/{unpatched,layer_ab,full,dso,bin}
ln -sfn "$baseline/libc++.so.1.0" "$payload/unpatched/libc++.so.1"
ln -sfn "$baseline/libc++abi.so.1.0" "$payload/unpatched/libc++abi.so.1"
ln -sfn "$baseline/libc++.so.1.0" "$payload/layer_ab/libc++.so.1"
ln -sfn "$abi" "$payload/layer_ab/libc++abi.so.1"
ln -sfn "$full" "$payload/full/libc++.so.1"
ln -sfn "$abi" "$payload/full/libc++abi.so.1"
cp "$tests"/lib*.so "$payload/dso/"
cp "$tests/dso_launcher" "$payload/bin/"
(cd "$payload" && find . -type f -print0 | sort -z | xargs -0 sha256sum > MANIFEST.sha256)
tar -C "$payload" -chf "$archive" .
sha256sum "$archive" "$payload"/MANIFEST.sha256

