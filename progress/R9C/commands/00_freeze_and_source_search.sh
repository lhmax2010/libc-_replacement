#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
LLVM="$REPO/codes/llvm"
OUT="$REPO/progress/R9C"
RAW="$OUT/raw"
LEDGER="$OUT/commands/00_freeze_and_source_search.log"
PLATFORM_TAR="$REPO/tmp/R9B/input/canonical/llvm-22.1.8.tar.gz"
LOCAL_TAR="$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260805-std-r2/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1/llvm-22.1.8.tar.gz"
ARCHIVAL_PATH=llvm-22.1.8/clang/bindings/python/.git_archival.txt
EXPECTED_PLATFORM=16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e
EXPECTED_LOCAL=1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2

mkdir -p "$OUT/commands" "$OUT/raw" "$OUT/inputs" "$OUT/build_impact" "$OUT/tarball_features"
: >"$LEDGER"
record_to() {
    local label=$1 outfile=$2
    shift 2
    {
        printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
        printf '%q ' "$@"
        printf '\n'
    } >>"$LEDGER"
    "$@" >"$outfile" 2>"$RAW/$label.stderr.txt"
    local rc=$?
    printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
    return "$rc"
}

cd "$REPO" || exit 125
overall=0
record_to project_status_start "$RAW/project_status_start.stdout.txt" git status --short --untracked-files=all || overall=1
record_to llvm_identity "$RAW/llvm_identity.stdout.txt" bash -o pipefail -c \
    'printf "toplevel="; git -C "$1" rev-parse --show-toplevel; printf "branch="; git -C "$1" branch --show-current; printf "head="; git -C "$1" rev-parse HEAD; printf "status_begin\\n"; git -C "$1" status --porcelain=v1; printf "status_end\\n"' _ "$LLVM" || overall=1
record_to assert_llvm_frozen_clean "$RAW/assert_llvm_frozen_clean.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" branch --show-current)" = tizen_base && test "$(git -C "$1" rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b && test -z "$(git -C "$1" status --porcelain=v1)"' _ "$LLVM" || overall=1
record_to input_tarball_stat "$RAW/input_tarball_stat.stdout.txt" stat -c '%n\t%s\t%y' "$PLATFORM_TAR" "$LOCAL_TAR" || overall=1
record_to input_tarball_sha256 "$RAW/input_tarball_sha256.stdout.txt" sha256sum "$PLATFORM_TAR" "$LOCAL_TAR" || overall=1
record_to assert_input_tarball_sha256 "$RAW/assert_input_tarball_sha256.stdout.txt" bash -o pipefail -c \
    'test "$(sha256sum "$1" | cut -d" " -f1)" = "$3" && test "$(sha256sum "$2" | cut -d" " -f1)" = "$4"' _ "$PLATFORM_TAR" "$LOCAL_TAR" "$EXPECTED_PLATFORM" "$EXPECTED_LOCAL" || overall=1
record_to extract_platform_archival "$OUT/inputs/platform.git_archival.txt" tar -xOzf "$PLATFORM_TAR" "$ARCHIVAL_PATH" || overall=1
record_to extract_local_archival "$OUT/inputs/local_standard_export.git_archival.txt" tar -xOzf "$LOCAL_TAR" "$ARCHIVAL_PATH" || overall=1
record_to archival_sha256 "$RAW/archival_sha256.stdout.txt" sha256sum "$OUT/inputs/platform.git_archival.txt" "$OUT/inputs/local_standard_export.git_archival.txt" || overall=1
record_to archival_numbered "$RAW/archival_numbered.stdout.txt" bash -o pipefail -c \
    'for f in "$@"; do printf "FILE=%s\\n" "$f"; nl -ba "$f"; done' _ "$OUT/inputs/platform.git_archival.txt" "$OUT/inputs/local_standard_export.git_archival.txt" || overall=1
record_to archival_diff "$OUT/inputs/platform_vs_local.git_archival.diff" diff -u "$OUT/inputs/platform.git_archival.txt" "$OUT/inputs/local_standard_export.git_archival.txt" || test $? -eq 1 || overall=1
record_to gitattributes_export_subst "$RAW/gitattributes_export_subst.stdout.txt" rg -n --hidden --glob '!.git/**' 'export-subst|\.git_archival\.txt|git_archival' "$LLVM" || overall=1
record_to source_filename_references "$RAW/source_filename_references.stdout.txt" rg -n --hidden --glob '!.git/**' --glob '!**/.git_archival.txt' '\.git_archival\.txt|git_archival' "$LLVM" || true
record_to version_vcs_mechanism_files "$RAW/version_vcs_mechanism_files.stdout.txt" find "$LLVM" -type f \( -iname '*GenerateVersionFromVCS*' -o -iname '*VersionFromVCS*' -o -iname '*VCSVersion*' -o -iname '*VersionInc*' \) -print || overall=1
record_to version_vcs_text_search "$RAW/version_vcs_text_search.stdout.txt" rg -n --hidden --glob '!.git/**' 'GenerateVersionFromVCS|VersionFromVCS|VCSVersion|git describe|git_archival|\.git_archival\.txt' "$LLVM/cmake" "$LLVM/llvm" "$LLVM/clang" "$LLVM/libcxx" "$LLVM/libcxxabi" "$LLVM/runtimes" || true
record_to archival_path_git_attributes "$RAW/archival_path_git_attributes.stdout.txt" git -C "$LLVM" check-attr -a -- clang/bindings/python/.git_archival.txt || overall=1
record_to archival_file_git_identity "$RAW/archival_file_git_identity.stdout.txt" bash -o pipefail -c \
    'git -C "$1" ls-files -s clang/bindings/python/.git_archival.txt; git -C "$1" log -1 --format=fuller -- clang/bindings/python/.git_archival.txt; git -C "$1" show HEAD:clang/bindings/python/.git_archival.txt | nl -ba' _ "$LLVM" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
