#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R6"
raw="$out/raw"
up="$out/upstream"
src="$repo/codes/llvm"
commit=8119f1854948b50358bbfaea08f207f51970f06c
url="https://github.com/llvm/llvm-project/commit/$commit.patch"
patch="$up/$commit.github.patch"
ledger="$out/commands/03_fetch_validate_upstream_patch.log"
mkdir -p "$up"
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

cd "$src" || exit 125
run_recorded PATCH_TARGET_FRESH test ! -e "$patch" || exit 90
run_recorded CURL_UPSTREAM_PATCH curl --fail --location --silent --show-error --proto =https "$url" --output "$patch" || {
    printf 'NOT_AVAILABLE\n' >"$out/upstream_patch_status.txt"
    printf 'R6\tRED_STOP\tUPSTREAM_PATCH_NOT_AVAILABLE\t%s\n' "$(date --iso-8601=seconds)" >"$repo/progress/state/R6.tsv"
    exit 91
}
run_recorded PATCH_SHA256 bash -o pipefail -c \
    "sha256sum '$patch' | tee '$out/upstream_patch_sha256.tsv'" || exit 92
run_recorded PATCH_HEADER bash -o pipefail -c \
    "sed -n '1,/^---$/p' '$patch' > '$raw/upstream_patch_header.raw.txt'" || exit 93
run_recorded PATCH_COMMIT_ASSERT bash -o pipefail -c \
    "test \"\$(sed -n '1s/^From \\([0-9a-f]\\{40\\}\\) .*/\\1/p' '$patch')\" = '$commit'" || exit 94
run_recorded PATCH_NUMSTAT bash -o pipefail -c \
    "git apply --numstat '$patch' | tee '$raw/upstream_patch_numstat.raw.txt'" || exit 95
run_recorded PATCH_SCOPE_ASSERT bash -o pipefail -c \
    "test \"\$(git apply --numstat '$patch')\" = \$'2\\t0\\tlibcxxabi/src/cxa_personality.cpp'" || exit 96
run_recorded PATCH_CHECK git apply --check "$patch" || exit 97
run_recorded PATCH_DIFF_BODY bash -o pipefail -c \
    "sed -n '/^diff --git /,\$p' '$patch' > '$raw/upstream_patch_diff_body.raw.patch'" || exit 98
run_recorded PATCH_ADDED_LINES bash -o pipefail -c \
    "awk 'BEGIN{in_diff=0} /^diff --git /{in_diff=1} in_diff && /^\\+[^+]/{print NR \":\" \$0}' '$patch' > '$raw/upstream_patch_added_lines.raw.txt'; test \"\$(wc -l < '$raw/upstream_patch_added_lines.raw.txt')\" -eq 2; rg -q 'extern "C" _Unwind_Reason_Code __gnu_unwind_frame' '$raw/upstream_patch_added_lines.raw.txt'; rg -q '^.*:\+ *.*_Unwind_Context\\*' '$raw/upstream_patch_added_lines.raw.txt'" || exit 99
printf 'AVAILABLE_AND_VALIDATED\n' >"$out/upstream_patch_status.txt"
exit 0
