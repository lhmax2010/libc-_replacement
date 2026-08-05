#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R6"
raw="$out/raw"
src="$repo/codes/llvm"
tarball="$repo/tmp/corpus/extracted/llvm-22.1.8-18.1.src.rpm/payload/llvm-22.1.8.tar.gz"
compare_parent="$repo/tmp/R6/source_compare"
compare_root="$compare_parent/llvm-22.1.8"
expected=16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e
ledger="$out/commands/02_tarball_source_compare.log"
: >"$ledger"

run_recorded() {
    local label=$1
    shift
    local rc
    {
        printf 'LABEL=%s\n' "$label"
        printf 'WORKING_DIRECTORY=%s\n' "$PWD"
        printf 'COMMAND_BEGIN\n'
        printf '%q ' "$@"
        printf '\nCOMMAND_END\n'
    } >>"$ledger"
    "$@" >>"$ledger" 2>&1
    rc=$?
    printf 'EXIT_CODE=%s\n' "$rc" >>"$ledger"
    printf '%s\t%s\n' "$label" "$rc"
    return "$rc"
}

cd "$repo" || exit 125
run_recorded TARBALL_EXISTS test -f "$tarball" || exit 90
run_recorded COMPARE_ROOT_FRESH test ! -e "$compare_parent" || exit 91
run_recorded TARBALL_SHA256 bash -o pipefail -c \
    "sha256sum '$tarball' | tee '$raw/platform_tarball_sha256.raw.txt'; test \"\$(sha256sum '$tarball' | awk '{print \$1}')\" = '$expected'" || exit 92
run_recorded TARBALL_TOPLEVEL bash -o pipefail -c \
    "tar -tzf '$tarball' | sed 's#/.*##' | LC_ALL=C sort -u > '$raw/platform_tarball_toplevel.raw.txt'; test \"\$(wc -l < '$raw/platform_tarball_toplevel.raw.txt')\" -eq 1; test \"\$(cat '$raw/platform_tarball_toplevel.raw.txt')\" = llvm-22.1.8" || exit 93
run_recorded EXTRACT_COMPARE_COPY bash -o pipefail -c \
    "mkdir -p '$compare_parent' && tar -xzf '$tarball' -C '$compare_parent'" || exit 94
run_recorded EXTRACTED_COMPONENTS_ASSERT test -d "$compare_root/libcxxabi" -a -d "$compare_root/libunwind" || exit 95
run_recorded MARK_COMPARE_COPY_READ_ONLY chmod -R a-w "$compare_parent" || exit 96
run_recorded LIBCXXABI_DIR_DIFF bash -o pipefail -c \
    "diff -qr --no-dereference '$compare_root/libcxxabi' '$src/libcxxabi' > '$raw/libcxxabi_directory_diff.summary.txt'"
libcxxabi_rc=$?
printf 'libcxxabi\t%s\n' "$libcxxabi_rc" >"$out/source_compare_exit.tsv"
test "$libcxxabi_rc" -eq 0 -o "$libcxxabi_rc" -eq 1 || exit 97
run_recorded LIBUNWIND_DIR_DIFF bash -o pipefail -c \
    "diff -qr --no-dereference '$compare_root/libunwind' '$src/libunwind' > '$raw/libunwind_directory_diff.summary.txt'"
libunwind_rc=$?
printf 'libunwind\t%s\n' "$libunwind_rc" >>"$out/source_compare_exit.tsv"
test "$libunwind_rc" -eq 0 -o "$libunwind_rc" -eq 1 || exit 98

if test "$libcxxabi_rc" -ne 0 -o "$libunwind_rc" -ne 0; then
    run_recorded LIBCXXABI_FULL_DIFF bash -o pipefail -c \
        "diff -ruN --no-dereference '$compare_root/libcxxabi' '$src/libcxxabi' > '$raw/libcxxabi_directory_diff.full.diff'"
    full_abi_rc=$?
    test "$full_abi_rc" -eq 0 -o "$full_abi_rc" -eq 1 || exit 99
    run_recorded LIBUNWIND_FULL_DIFF bash -o pipefail -c \
        "diff -ruN --no-dereference '$compare_root/libunwind' '$src/libunwind' > '$raw/libunwind_directory_diff.full.diff'"
    full_unwind_rc=$?
    test "$full_unwind_rc" -eq 0 -o "$full_unwind_rc" -eq 1 || exit 100
    printf 'SOURCE_DIVERGENT\n' >"$out/source_identity_conclusion.txt"
    printf 'R6\tRED_STOP\tSOURCE_DIVERGENT\t%s\n' "$(date --iso-8601=seconds)" >"$repo/progress/state/R6.tsv"
    exit 110
fi

printf 'SOURCE_IDENTICAL\n' >"$out/source_identity_conclusion.txt"
exit 0
