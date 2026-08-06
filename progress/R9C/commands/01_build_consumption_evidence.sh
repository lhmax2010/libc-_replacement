#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
LLVM="$REPO/codes/llvm"
OUT="$REPO/progress/R9C"
RAW="$OUT/raw"
LEDGER="$OUT/commands/01_build_consumption_evidence.attempt2.log"
R6LOG="$REPO/progress/R6/build_armv7l_git_r3b.full.log"
R6ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b"
R6TAR="$R6ROOT/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1/llvm-22.1.8.tar.gz"
R8="$REPO/progress/R8"
R9B="$REPO/progress/R9B"

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
record_expected() {
    local expected_rc=$1 label=$2 outfile=$3
    shift 3
    {
        printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
        printf '%q ' "$@"
        printf '\n'
    } >>"$LEDGER"
    "$@" >"$outfile" 2>"$RAW/$label.stderr.txt"
    local rc=$?
    printf 'EXIT_CODE=%s\nEXPECTED_EXIT_CODE=%s\n\n' "$rc" "$expected_rc" >>"$LEDGER"
    [[ $rc -eq $expected_rc ]]
}

cd "$REPO" || exit 125
overall=0
record_to gitattributes_numbered "$RAW/gitattributes_numbered.stdout.txt" nl -ba "$LLVM/.gitattributes" || overall=1
record_to version_from_vcs_numbered "$RAW/version_from_vcs_numbered.stdout.txt" nl -ba "$LLVM/llvm/cmake/modules/VersionFromVCS.cmake" || overall=1
record_to generate_version_from_vcs_numbered "$RAW/generate_version_from_vcs_numbered.stdout.txt" nl -ba "$LLVM/llvm/cmake/modules/GenerateVersionFromVCS.cmake" || overall=1
record_to find_vc_file_numbered "$RAW/find_vc_file_numbered.stdout.txt" bash -o pipefail -c \
    'nl -ba "$1" | sed -n "2613,2678p"' _ "$LLVM/llvm/cmake/modules/AddLLVM.cmake" || overall=1
record_to version_generation_callers "$RAW/version_generation_callers.stdout.txt" bash -o pipefail -c \
    'for spec in "$@"; do printf "FILE=%s\\n" "$spec"; nl -ba "$spec" | sed -n "1,62p"; done' _ \
    "$LLVM/clang/lib/Basic/CMakeLists.txt" "$LLVM/llvm/include/llvm/Support/CMakeLists.txt" || overall=1
record_to assert_no_archival_reference_outside_attributes "$RAW/assert_no_archival_reference_outside_attributes.stdout.txt" bash -o pipefail -c \
    'matches=$(rg -n --hidden --glob "!.git/**" --glob "!**/.git_archival.txt" "\\.git_archival\\.txt|git_archival" "$1"); printf "%s\\n" "$matches"; test "$(printf "%s\\n" "$matches" | sed "/^$/d" | wc -l)" -eq 1 && printf "%s\\n" "$matches" | rg -q "/.gitattributes:1:"' _ "$LLVM" || overall=1
record_to r8_consumption_inventory_counts "$RAW/r8_consumption_inventory_counts.stdout.txt" bash -o pipefail -c \
    'for f in "$@"; do printf "FILE=%s\\n" "$f"; wc -l "$f"; sed -n "1,5p" "$f"; done' _ \
    "$R8/r6_actual_compiled_tu.tsv" "$R8/r6_actual_object_dependencies.tsv" "$R8/r6_cmake_consumed_inputs.tsv" "$R8/r6_consumed_source_paths.tsv" || overall=1
record_to r8_consumption_method_original "$RAW/r8_consumption_method_original.stdout.txt" bash -o pipefail -c \
    'nl -ba "$1" | sed -n "20,100p"; nl -ba "$2" | sed -n "68,92p"' _ \
    "$R8/commands/02_r6_compile_dependency_intersection.sh" "$R8/R8_report.md" || overall=1
record_expected 1 archival_in_r8_consumed_paths "$RAW/archival_in_r8_consumed_paths.stdout.txt" rg -n \
    '(^|/)clang/bindings/python/\.git_archival\.txt|git_archival' "$R8/r6_consumed_source_paths.tsv" || overall=1
record_expected 1 archival_in_r8_tu_deps_cmake "$RAW/archival_in_r8_tu_deps_cmake.stdout.txt" rg -n \
    '(^|/)clang/bindings/python/\.git_archival\.txt|git_archival' \
    "$R8/r6_actual_compiled_tu.tsv" "$R8/r6_actual_object_dependencies.tsv" "$R8/r6_cmake_consumed_inputs.tsv" || overall=1
record_to r6_source0_sha256 "$RAW/r6_source0_sha256.stdout.txt" sha256sum "$R6TAR" || overall=1
record_to r6_source0_archival_membership "$RAW/r6_source0_archival_membership.stdout.txt" bash -o pipefail -c \
    'tar -tzf "$1" | rg -n "^llvm-22\\.1\\.8/clang/bindings/python/\\.git_archival\\.txt$"' _ "$R6TAR" || overall=1
record_to r6_source0_archival_content "$RAW/r6_source0_archival_content.stdout.txt" tar -xOzf "$R6TAR" \
    llvm-22.1.8/clang/bindings/python/.git_archival.txt || overall=1
record_to r6_extracted_tree_archival_presence "$RAW/r6_extracted_tree_archival_presence.stdout.txt" bash -o pipefail -c \
    'find "$1" -type f -path "*/llvm-22.1.8/clang/bindings/python/.git_archival.txt" -printf "%p\\t%s\\n"' _ \
    "$R6ROOT/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD" || overall=1
record_expected 1 archival_in_r6_build_log "$RAW/archival_in_r6_build_log.stdout.txt" rg -n \
    '\.git_archival\.txt|git_archival' "$R6LOG" || overall=1
record_to r6_runtime_cmake_invocation "$RAW/r6_runtime_cmake_invocation.stdout.txt" rg -n -C 3 \
    'cmake .*llvm-22\.1\.8/runtimes|LLVM_ENABLE_RUNTIMES|Building (CXX|C|ASM) object' "$R6LOG" || overall=1
record_to r9b_no_build_status "$RAW/r9b_no_build_status.stdout.txt" bash -o pipefail -c \
    'nl -ba "$1"; nl -ba "$2" | sed -n "1,35p"; test "$(rg -c "NOT_RUN_COMMON_INPUT_RED_STOP" "$1")" -eq 4' _ \
    "$R9B/execution_status.tsv" "$R9B/R9B_report.md" || overall=1
record_expected 1 archival_in_r9b_build_logs "$RAW/archival_in_r9b_build_logs.stdout.txt" rg -n \
    '\.git_archival\.txt|git_archival|Building (CXX|C|ASM) object' "$R9B/build" || overall=1
record_to assert_llvm_unchanged "$RAW/assert_llvm_unchanged_after_consumption.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b && test -z "$(git -C "$1" status --porcelain=v1)"' _ "$LLVM" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
