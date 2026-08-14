#!/bin/bash
set -u

repo=/home/toolchain/development/libc++_replacement
progress="$repo/progress/R34B"
curated="$repo/docs/progress/R34B"

run() {
    printf '$'
    printf ' %q' "$@"
    printf '\n'
    "$@"
    rc=$?
    printf '[exit_code] %d\n' "$rc"
    return "$rc"
}

run cp -a "$progress/R34B_report.md" "$curated/R34B_report.md"
run cp -a "$progress/EXCLUDED.tsv" "$curated/EXCLUDED.tsv"
run cp -a "$progress/tools/finalize_evidence.sh" "$curated/tools/finalize_evidence.sh"

index_tmp=$(mktemp)
printf 'source_task\toriginal_path\tupload_path\tsha256\n' > "$index_tmp"
while IFS= read -r upload; do
    rel=${upload#"$curated/code/"}
    source_task=R34B
    case "$rel" in
        implementation/current/*)
            original="tmp/R34B/llvm-exp/${rel#implementation/current/}"
            ;;
        implementation/baseline/*)
            source_task=PLATFORM_SOURCE
            original="codes/llvm/${rel#implementation/baseline/}"
            ;;
        implementation/*.diff)
            original="progress/R34B/patches/${rel#implementation/}"
            ;;
        usecase/src/*)
            original="progress/R34B/src/${rel#usecase/src/}"
            ;;
        usecase/matrix/*)
            original="progress/R34B/results/matrix/${rel#usecase/matrix/}"
            ;;
        build/tools/tizen-clang|build/tools/tizen-clang++)
            source_task=R33
            original="progress/R33/tools/${rel#build/tools/}"
            ;;
        build/tools/*)
            original="progress/R34B/tools/${rel#build/tools/}"
            ;;
        build/config/*)
            source_task=R4_R9C
            original="packaging/${rel#build/config/}"
            ;;
        build/cmake/*)
            original="tmp/R34B/build-s1/${rel#build/cmake/}"
            ;;
        reference/*)
            original="progress/R34B/commands/${rel#reference/}"
            ;;
        README.md)
            original=GENERATED_R34B_CURATED
            ;;
        *)
            original=NOT_AVAILABLE
            ;;
    esac
    sha=$(sha256sum "$upload" | awk '{print $1}')
    printf '%s\t%s\t%s\t%s\n' "$source_task" "$original" \
        "docs/progress/R34B/code/$rel" "$sha" >> "$index_tmp"
done < <(find "$curated/code" -type f ! -name INDEX.tsv ! -name MANIFEST.sha256 | sort)
run mv "$index_tmp" "$curated/code/INDEX.tsv"

progress_manifest_tmp=$(mktemp)
(
    cd "$progress" || exit 1
    find . -type f ! -name MANIFEST.sha256 -print0 | sort -z | xargs -0 sha256sum
) > "$progress_manifest_tmp"
run mv "$progress_manifest_tmp" "$progress/MANIFEST.sha256"

curated_manifest_tmp=$(mktemp)
(
    cd "$curated" || exit 1
    find . -type f ! -name MANIFEST.sha256 -print0 | sort -z | xargs -0 sha256sum
) > "$curated_manifest_tmp"
run mv "$curated_manifest_tmp" "$curated/MANIFEST.sha256"

run find "$curated" -type f -size +5M -print
run cmp "$repo/codes/llvm/libcxxabi/include/cxxabi.h" \
    "$curated/code/implementation/baseline/libcxxabi/include/cxxabi.h"
run cmp "$repo/codes/llvm/libcxxabi/src/cxa_personality.cpp" \
    "$curated/code/implementation/baseline/libcxxabi/src/cxa_personality.cpp"
run cmp "$repo/codes/llvm/libcxxabi/src/cxa_exception.cpp" \
    "$curated/code/implementation/baseline/libcxxabi/src/cxa_exception.cpp"
run cmp "$repo/tmp/R34B/llvm-exp/libcxxabi/include/cxxabi.h" \
    "$curated/code/implementation/current/libcxxabi/include/cxxabi.h"
run cmp "$repo/tmp/R34B/llvm-exp/libcxxabi/src/cxa_personality.cpp" \
    "$curated/code/implementation/current/libcxxabi/src/cxa_personality.cpp"
run cmp "$repo/tmp/R34B/llvm-exp/libcxxabi/src/cxa_exception.cpp" \
    "$curated/code/implementation/current/libcxxabi/src/cxa_exception.cpp"
