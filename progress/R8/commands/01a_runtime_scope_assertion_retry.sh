#!/usr/bin/env bash
set -u
set -o pipefail
REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R8"
RAW="$OUT/raw"
LEDGER="$OUT/commands/01a_runtime_scope_assertion_retry.log"
: >"$LEDGER"
cd "$REPO" || exit 125
{
  printf 'LABEL=runtime_scope_assertion_retry\nPWD=%s\nCOMMAND=' "$PWD"
  printf '%q ' bash -o pipefail -c 'test "$(wc -l < "$1")" -eq 1 && grep -Fq "libcxxabi/src/cxa_personality.cpp" "$1" && test "$(grep -Fc "+extern \"C\" _Unwind_Reason_Code __gnu_unwind_frame" "$2")" -eq 1 && test "$(grep -Fc -- "-extern \"C\" _Unwind_Reason_Code __gnu_unwind_frame" "$2")" -eq 0' _ "$RAW/libcxxabi.diff_qr.txt" "$RAW/cxa_personality.platform_to_tizen.full.diff"
  printf '\n'
} >>"$LEDGER"
bash -o pipefail -c 'test "$(wc -l < "$1")" -eq 1 && grep -Fq "libcxxabi/src/cxa_personality.cpp" "$1" && test "$(grep -Fc "+extern \"C\" _Unwind_Reason_Code __gnu_unwind_frame" "$2")" -eq 1 && test "$(grep -Fc -- "-extern \"C\" _Unwind_Reason_Code __gnu_unwind_frame" "$2")" -eq 0' _ "$RAW/libcxxabi.diff_qr.txt" "$RAW/cxa_personality.platform_to_tizen.full.diff" >"$RAW/runtime_scope_assertion_retry.stdout.txt" 2>"$RAW/runtime_scope_assertion_retry.stderr.txt"
rc=$?
printf 'EXIT_CODE=%s\nOVERALL_EXIT_CODE=%s\n' "$rc" "$rc" >>"$LEDGER"
if [[ $rc -eq 0 ]]; then
  printf 'RUNTIME_TREE_DIFF=PASS_ONLY_EHABI_BACKPORT\n' >"$OUT/runtime_tree_diff.status"
else
  printf 'RUNTIME_TREE_DIFF=FAIL_OR_EXTRA_DIFFERENCE\n' >"$OUT/runtime_tree_diff.status"
fi
exit "$rc"
