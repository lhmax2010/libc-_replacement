#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R8"
RAW="$OUT/raw"
PRISTINE="$REPO/tmp/corpus/extracted/llvm-22.1.8-18.1.src.rpm/unpacked/llvm-22.1.8.tar.gz/llvm-22.1.8"
CODE="$REPO/codes/llvm"
LEDGER="$OUT/commands/01_source_tree_differences.log"
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
record_to source_roots_identity "$RAW/source_roots_identity.stdout.txt" bash -o pipefail -c '
  printf "PRISTINE=%s\nCODE=%s\n" "$1" "$2"
  test -d "$1" && test -d "$2"
  sha256sum "$3"
  git -C "$2" rev-parse HEAD
  git -C "$2" status --porcelain
' _ "$PRISTINE" "$CODE" "$REPO/tmp/R8/platform_srpm/payload/llvm-22.1.8.tar.gz" || overall=1

for dir in libcxx libcxxabi libunwind runtimes cmake third-party; do
  label=${dir//-/_}
  record_to "presence_$label" "$RAW/${label}_presence.stdout.txt" bash -o pipefail -c '
    printf "pristine=%s\ncode=%s\n" "$(test -d "$1/$3" && echo PRESENT || echo ABSENT)" "$(test -d "$2/$3" && echo PRESENT || echo ABSENT)"
    test -d "$1/$3" && test -d "$2/$3"
  ' _ "$PRISTINE" "$CODE" "$dir" || overall=1
  record_to "diff_qr_$label" "$RAW/${label}.diff_qr.txt" diff -qr --exclude=.git "$PRISTINE/$dir" "$CODE/$dir"
  rc=$?
  if [[ "$dir" = libcxxabi ]]; then
    [[ $rc -eq 1 ]] || overall=1
  else
    [[ $rc -eq 0 ]] || overall=1
  fi
done

record_to cxa_personality_full_diff "$RAW/cxa_personality.platform_to_tizen.full.diff" diff -u \
  "$PRISTINE/libcxxabi/src/cxa_personality.cpp" "$CODE/libcxxabi/src/cxa_personality.cpp"
rc=$?
[[ $rc -eq 1 ]] || overall=1
record_to ehabi_backport_git_show "$RAW/ehabi_backport_git_show.stdout.txt" git -C "$CODE" show --format=fuller --stat --patch \
  3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b || overall=1
record_to assert_runtime_diff_scope "$RAW/assert_runtime_diff_scope.stdout.txt" bash -o pipefail -c '
  test "$(wc -l < "$1")" -eq 1 &&
  rg -q "libcxxabi/src/cxa_personality.cpp" "$1" &&
  test "$(rg -c "^\\+extern \\\"C\\\" _Unwind_Reason_Code __gnu_unwind_frame" "$2")" -eq 1 &&
  test "$(rg -c "^-extern \\\"C\\\" _Unwind_Reason_Code __gnu_unwind_frame" "$2" || true)" -eq 0
' _ "$RAW/libcxxabi.diff_qr.txt" "$RAW/cxa_personality.platform_to_tizen.full.diff" || overall=1

for dir in clang compiler-rt llvm; do
  label=${dir//-/_}
  record_to "diff_qr_nonruntime_$label" "$RAW/${label}.nonruntime.diff_qr.txt" \
    diff -qr --exclude=.git "$PRISTINE/$dir" "$CODE/$dir"
  rc=$?
  [[ $rc -eq 1 ]] || overall=1
done

record_to normalize_all_difference_files "$OUT/source_difference_files.tsv" bash -o pipefail -c '
  printf "scope\trelative_path\tdifference_kind\n"
  for pair in \
    "runtime_libcxx:$3/libcxx.diff_qr.txt" \
    "runtime_libcxxabi:$3/libcxxabi.diff_qr.txt" \
    "runtime_libunwind:$3/libunwind.diff_qr.txt" \
    "runtime_runtimes:$3/runtimes.diff_qr.txt" \
    "runtime_cmake:$3/cmake.diff_qr.txt" \
    "runtime_third-party:$3/third_party.diff_qr.txt" \
    "nonruntime_clang:$3/clang.nonruntime.diff_qr.txt" \
    "nonruntime_compiler-rt:$3/compiler_rt.nonruntime.diff_qr.txt" \
    "nonruntime_llvm:$3/llvm.nonruntime.diff_qr.txt"; do
    scope=${pair%%:*}; file=${pair#*:}
    while IFS= read -r line; do
      case "$line" in
        Files\ *\ and\ *\ differ)
          p=${line#Files "$1"/}; p=${p%% and *}
          printf "%s\t%s\tCONTENT_DIFF\n" "$scope" "$p" ;;
        Only\ in\ "$1"/*)
          left=${line#Only in "$1"/}; dir=${left%%: *}; base=${left#*: }
          printf "%s\t%s/%s\tONLY_PRISTINE\n" "$scope" "$dir" "$base" ;;
        Only\ in\ "$2"/*)
          left=${line#Only in "$2"/}; dir=${left%%: *}; base=${left#*: }
          printf "%s\t%s/%s\tONLY_TIZEN_BASE\n" "$scope" "$dir" "$base" ;;
      esac
    done <"$file"
  done | LC_ALL=C sort -u
' _ "$PRISTINE" "$CODE" "$RAW" || overall=1

record_to source_difference_file_counts "$OUT/source_difference_counts.tsv" bash -o pipefail -c '
  printf "scope\tcount\n"
  awk -F"\t" "NR>1 {n[\$1]++} END {for (k in n) print k \"\\t\" n[k]}" "$1" | LC_ALL=C sort
' _ "$OUT/source_difference_files.tsv" || overall=1

if [[ $overall -eq 0 ]]; then
  printf 'RUNTIME_TREE_DIFF=PASS_ONLY_EHABI_BACKPORT\n' >"$OUT/runtime_tree_diff.status"
else
  printf 'RUNTIME_TREE_DIFF=FAIL_OR_EXTRA_DIFFERENCE\n' >"$OUT/runtime_tree_diff.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
