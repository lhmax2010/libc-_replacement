#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
payload="$workspace/tmp/R67/board-payload"
archive="$workspace/tmp/R67/r67-board-payload.tar"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
fixed="$workspace/tmp/R67/build-armv7l/lib"

rm -rf "$payload"
mkdir -p "$payload/bin" "$payload/runtime/baseline" "$payload/runtime/fixed" \
  "$payload/upstream/libcxx" "$payload/upstream/libcxxabi" "$payload/upstream/libstdcxx"

for variant in baseline fixed; do
  source_dir=$baseline
  [[ $variant == fixed ]] && source_dir=$fixed
  cp -L "$source_dir/libc++.so.1" "$payload/runtime/$variant/libc++.so.1"
  cp -L "$source_dir/libc++abi.so.1" "$payload/runtime/$variant/libc++abi.so.1"
done

for file in "$workspace"/tmp/R67/probes/armv7l/*; do
  [[ -f $file && -x $file ]] || continue
  cp "$file" "$payload/bin/"
done
for file in "$workspace"/tmp/R67/upstream/armv7l/libcxx/*; do
  [[ -f $file && -x $file ]] || continue
  cp "$file" "$payload/upstream/libcxx/"
done
for runtime in libcxxabi libstdcxx; do
  for file in "$workspace"/tmp/R67/upstream/armv7l/$runtime/*; do
    [[ -f $file && -x $file ]] || continue
    cp "$file" "$payload/upstream/$runtime/"
  done
done

find "$payload" -type f ! -name MANIFEST.sha256 -print0 | sort -z | xargs -0 sha256sum \
  | sed "s|$payload/||" > "$payload/MANIFEST.sha256"
tar -C "$payload" -cf "$archive" .
sha256sum "$archive" "$payload/MANIFEST.sha256"
