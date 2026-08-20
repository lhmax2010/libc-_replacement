#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
curated="$workspace/docs/progress/R52"
index="$curated/INDEX.tsv"
manifest="$curated/MANIFEST.sha256"

printf 'source_task\toriginal_path\tuploaded_path\tsha256\n' > "$index"
while IFS= read -r path; do
  relative=${path#"$curated/"}
  [[ $relative == INDEX.tsv || $relative == MANIFEST.sha256 ]] && continue
  case "$relative" in
    code/modified/libcxx/include/*)
      original="tmp/R52/source-layer-c/libcxx/include/${relative##*/}" ;;
    code/build-config/x86_64.CMakeCache.txt)
      original="tmp/R51/build-product-x86_64/CMakeCache.txt" ;;
    code/build-config/armv7l.CMakeCache.txt)
      original="tmp/R51/build-product-armv7l/CMakeCache.txt" ;;
    code/tests/s1_rethrow_probe.cpp)
      original="progress/R51/src/s1_rethrow_probe.cpp" ;;
    code/tests/upstream/istream_getline.pass.cpp)
      original="codes/llvm/libcxx/test/std/input.output/iostream.format/input.streams/istream.unformatted/getline_pointer_size.pass.cpp" ;;
    code/tests/upstream/istream_ws.pass.cpp)
      original="codes/llvm/libcxx/test/std/input.output/iostream.format/input.streams/istream.manip/ws.pass.cpp" ;;
    code/tests/upstream/future_get_future.pass.cpp)
      original="codes/llvm/libcxx/test/std/thread/futures/futures.task/futures.task.members/get_future.pass.cpp" ;;
    code/tests/upstream/future_operator.pass.cpp)
      original="codes/llvm/libcxx/test/std/thread/futures/futures.task/futures.task.members/operator.pass.cpp" ;;
    code/tests/upstream/future_async.pass.cpp)
      original="codes/llvm/libcxx/test/std/thread/futures/futures.async/async.pass.cpp" ;;
    code/tests/upstream/string_shrink_to_fit.pass.cpp)
      original="codes/llvm/libcxx/test/std/strings/basic.string/string.capacity/shrink_to_fit.pass.cpp" ;;
    code/tests/type_matrix/*)
      original="progress/R52/type_matrix/${relative##*/}" ;;
    code/tests/*)
      original="progress/R52/src/${relative##*/}" ;;
    code/tools/*)
      original="progress/R52/tools/${relative##*/}" ;;
    code/0001-*.patch)
      original="progress/R52/patches/${relative##*/}" ;;
    code/patch_commit_message.txt)
      original="progress/R52/patch_commit_message.txt" ;;
    *) original="progress/R52/$relative" ;;
  esac
  digest=$(sha256sum "$path" | awk '{print $1}')
  printf 'R52\t%s\tdocs/progress/R52/%s\t%s\n' "$original" "$relative" "$digest" >> "$index"
done < <(find "$curated" -type f | sort)

(cd "$curated" && find . -type f ! -name MANIFEST.sha256 -print0 | sort -z | xargs -0 sha256sum > MANIFEST.sha256)
printf 'INDEX_ROWS=%d\n' "$(( $(wc -l < "$index") - 1 ))"
sha256sum "$index" "$manifest"
