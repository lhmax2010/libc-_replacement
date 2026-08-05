#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/T4"
RAW="$OUT/raw"
TMP="$REPO/tmp/T4/source_identity_retry"
LEDGER="$OUT/commands/01a_source_delta_retry.log"

: >"$LEDGER"

record_to() {
  local label=$1
  local outfile=$2
  shift 2
  {
    printf 'LABEL=%s\n' "$label"
    printf 'PWD=%s\n' "$PWD"
    printf 'COMMAND='
    printf '%q ' "$@"
    printf '\n'
  } >>"$LEDGER"
  "$@" >"$outfile" 2>"$RAW/$label.stderr.txt"
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

overall=0
cd "$REPO" || exit 125
record_to assert_retry_tmp_fresh "$RAW/assert_retry_tmp_fresh.stdout.txt" test ! -e "$TMP" || overall=1
if [[ $overall -ne 0 ]]; then
  printf 'SOURCE_DELTA_RETRY=FAIL_TMP_NOT_FRESH\n' >"$OUT/source_delta_retry.status"
  printf 'OVERALL_EXIT_CODE=1\n' >>"$LEDGER"
  exit 1
fi
record_to create_retry_tmp "$RAW/create_retry_tmp.stdout.txt" mkdir -p "$TMP" || overall=1

printf 'arch\tsource0_sha256\tsize_bytes\n' >"$OUT/source0_sha256.tsv"
for arch in armv7l aarch64 x86_64; do
  srpm="$REPO/artifacts/rebuild_2218/$arch/libcxx-runtimes-22.1.8-1.src.rpm"
  mkdir -p "$TMP/$arch"
  record_to "${arch}_extract_source0_retry" "$TMP/$arch/llvm-22.1.8.tar.gz" bash -o pipefail -c \
    'rpm2cpio "$1" | cpio -i --quiet --to-stdout llvm-22.1.8.tar.gz' _ "$srpm" || overall=1
  record_to "${arch}_assert_source0_retry" "$RAW/${arch}_assert_source0_retry.stdout.txt" bash -o pipefail -c \
    'test -s "$1" && gzip -t "$1"' _ "$TMP/$arch/llvm-22.1.8.tar.gz" || overall=1
  record_to "${arch}_source0_sha256_retry" "$RAW/${arch}_source0_sha256_retry.stdout.txt" sha256sum \
    "$TMP/$arch/llvm-22.1.8.tar.gz" || overall=1
  printf '%s\t%s\t%s\n' "$arch" \
    "$(sha256sum "$TMP/$arch/llvm-22.1.8.tar.gz" | cut -d' ' -f1)" \
    "$(stat -c %s "$TMP/$arch/llvm-22.1.8.tar.gz")" >>"$OUT/source0_sha256.tsv"
  record_to "${arch}_source0_unpack_retry" "$RAW/${arch}_source0_unpack_retry.stdout.txt" tar -xf \
    "$TMP/$arch/llvm-22.1.8.tar.gz" -C "$TMP/$arch" || overall=1
done
record_to assert_platform_source0_hashes "$RAW/assert_platform_source0_hashes.stdout.txt" bash -o pipefail -c \
  'test "$(awk -F"\\t" "\$1==\"aarch64\" {print \$2}" "$1")" = 16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e && test "$(awk -F"\\t" "\$1==\"x86_64\" {print \$2}" "$1")" = 16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e' \
  _ "$OUT/source0_sha256.tsv" || overall=1
record_to aarch64_vs_x86_source_tree_retry "$RAW/aarch64_vs_x86_source_tree_retry.diff" diff -qr --no-dereference \
  "$TMP/x86_64/llvm-22.1.8" "$TMP/aarch64/llvm-22.1.8" || overall=1
record_to armv7l_vs_x86_source_tree_retry "$RAW/armv7l_vs_x86_source_tree_retry.diff" diff -qr --no-dereference \
  "$TMP/x86_64/llvm-22.1.8" "$TMP/armv7l/llvm-22.1.8"
arm_tree_rc=$?
if [[ $arm_tree_rc -ne 1 ]]; then overall=1; fi
record_to armv7l_cxa_diff_retry "$RAW/armv7l_cxa_personality_retry.diff" diff -u \
  "$TMP/x86_64/llvm-22.1.8/libcxxabi/src/cxa_personality.cpp" \
  "$TMP/armv7l/llvm-22.1.8/libcxxabi/src/cxa_personality.cpp"
arm_cxa_rc=$?
if [[ $arm_cxa_rc -ne 1 ]]; then overall=1; fi
record_to armv7l_cxa_numstat_retry "$RAW/armv7l_cxa_personality_retry.numstat" git diff --no-index --numstat \
  "$TMP/x86_64/llvm-22.1.8/libcxxabi/src/cxa_personality.cpp" \
  "$TMP/armv7l/llvm-22.1.8/libcxxabi/src/cxa_personality.cpp"
arm_numstat_rc=$?
if [[ $arm_numstat_rc -ne 1 ]]; then overall=1; fi
record_to assert_expected_backport_delta_retry "$RAW/assert_expected_backport_delta_retry.stdout.txt" bash -o pipefail -c \
  'test "$(wc -l < "$1")" -eq 1 && rg -q "libcxxabi/src/cxa_personality.cpp" "$1" && awk "\$1==2 && \$2==0 {ok=1} END {exit !ok}" "$2" && test "$(rg -c "^\\+extern \\\"C\\\" _Unwind_Reason_Code __gnu_unwind_frame" "$3")" -eq 1 && test "$(rg -c "^-extern \\\"C\\\" _Unwind_Reason_Code __gnu_unwind_frame" "$3" || true)" -eq 0' \
  _ "$RAW/armv7l_vs_x86_source_tree_retry.diff" "$RAW/armv7l_cxa_personality_retry.numstat" \
  "$RAW/armv7l_cxa_personality_retry.diff" || overall=1
record_to assert_prior_identity_results "$RAW/assert_prior_identity_results.stdout.txt" bash -o pipefail -c \
  'test "$(tail -n +2 "$1" | wc -l)" -eq 24 && test "$(awk -F"\\t" "NR>1 && \$5 != \"MATCH\" {n++} END {print n+0}" "$1")" -eq 0 && test -s "$2" && cmp -s "$2" "$3" && cmp -s "$2" "$4"' \
  _ "$OUT/artifact_identity.tsv" "$OUT/provenance/armv7l.SOURCE_PROVENANCE" \
  "$OUT/provenance/aarch64.SOURCE_PROVENANCE" "$OUT/provenance/x86_64.SOURCE_PROVENANCE" || overall=1

if [[ $overall -eq 0 ]]; then
  printf 'SOURCE_DELTA_RETRY=PASS\n' >"$OUT/source_delta_retry.status"
  printf 'ARTIFACT_IDENTITY=PASS\n' >"$OUT/artifact_identity.status"
else
  printf 'SOURCE_DELTA_RETRY=FAIL\n' >"$OUT/source_delta_retry.status"
  printf 'ARTIFACT_IDENTITY=FAIL\n' >"$OUT/artifact_identity.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
