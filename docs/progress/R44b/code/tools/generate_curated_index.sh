#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
root="$workspace/docs/progress/R44b"
index="$root/INDEX.tsv"

printf 'source\tupload_path\tsha256\n' > "$index"
while IFS= read -r file; do
  rel=${file#"$workspace"/}
  case "$rel" in
    docs/progress/R44b/code/libcxx/src/ios.cpp)
      source=tmp/R44b/experiment/libcxx/src/ios.cpp ;;
    docs/progress/R44b/code/libcxx/src/ios.cpp.upstream)
      source=codes/llvm/libcxx/src/ios.cpp ;;
    docs/progress/R44b/code/libcxx/src/ios.cpp.diff)
      source=progress/R44b/diff/ios.cpp.diff.log ;;
    docs/progress/R44b/code/tests/stdlib_cancel_probe.cpp)
      source=progress/R43b/src/stdlib_cancel_probe.cpp ;;
    docs/progress/R44b/code/config/libcxx-llvm22.map)
      source=packaging/libcxx-llvm22.map ;;
    docs/progress/R44b/code/config/R42_x86_64_CMakeCache.txt)
      source=tmp/R42/build-patched-x86_64/CMakeCache.txt ;;
    docs/progress/R44b/code/config/R42_armv7l_CMakeCache.txt)
      source=tmp/R42/build-patched-armv7l/CMakeCache.txt ;;
    docs/progress/R44b/code/config/R9C_x86_64_CMakeCache.txt)
      source=tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/CMakeCache.txt ;;
    docs/progress/R44b/code/config/R9C_armv7l_CMakeCache.txt)
      source=tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/CMakeCache.txt ;;
    docs/progress/R44b/code/tools/tizen-clang++)
      source=progress/R33/tools/tizen-clang++ ;;
    docs/progress/R44b/code/tools/armv7l-clang++)
      source=progress/R36/tools/armv7l-clang++ ;;
    docs/progress/R44b/code/tools/armv7l-ld)
      source=progress/R36/tools/armv7l-ld ;;
    docs/progress/R44b/code/tools/*)
      source="progress/R44b/tools/${rel##*/}" ;;
    docs/progress/R44b/tables/*)
      source="progress/R44b/tables/${rel##*/}" ;;
    docs/progress/R44b/raw/*)
      candidate=$(find "$workspace/progress/R44b" -type f -name "${rel##*/}" -print | sort | head -n1)
      if [[ -n $candidate ]]; then source=${candidate#"$workspace"/}; else source=GENERATED_R44B; fi ;;
    *)
      source=GENERATED_R44B ;;
  esac
  sha=$(sha256sum "$file" | awk '{print $1}')
  printf '%s\t%s\t%s\n' "$source" "$rel" "$sha" >> "$index"
done < <(find "$root" -type f ! -name INDEX.tsv ! -name MANIFEST.sha256 -print | sort)
