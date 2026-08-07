#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/T4"
RAW="$OUT/raw"
TMP="$REPO/tmp/T4/source_identity"
LEDGER="$OUT/commands/01_artifact_identity_and_source_delta.log"

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
record_to assert_fresh_identity_tmp "$RAW/assert_fresh_identity_tmp.stdout.txt" test ! -e "$TMP" || overall=1
if [[ $overall -ne 0 ]]; then
  printf 'ARTIFACT_IDENTITY=FAIL_TMP_NOT_FRESH\n' >"$OUT/artifact_identity.status"
  printf 'OVERALL_EXIT_CODE=1\n' >>"$LEDGER"
  exit 1
fi
record_to create_identity_tmp "$RAW/create_identity_tmp.stdout.txt" mkdir -p "$TMP" || overall=1

record_to expected_armv7l "$TMP/armv7l.expected.tsv" bash -o pipefail -c \
  'awk "{n=\$2; sub(\".*/\",\"\",n); if (n ~ /\\.rpm\$/) print n \"\\t\" \$1}" "$1" | LC_ALL=C sort' \
  _ "$REPO/progress/R6/armv7l/artifact_sha256.tsv" || overall=1
record_to expected_aarch64 "$TMP/aarch64.expected.tsv" bash -o pipefail -c \
  'awk "{n=\$2; sub(\".*/\",\"\",n); if (n ~ /\\.rpm\$/) print n \"\\t\" \$1}" "$1" | LC_ALL=C sort' \
  _ "$REPO/progress/T3R2/aarch64/artifact_sha256.tsv" || overall=1
record_to expected_x86_64 "$TMP/x86_64.expected.tsv" bash -o pipefail -c \
  'awk -F"\\t" "NR>1 {print \$1 \"\\t\" \$4}" "$1" | LC_ALL=C sort' \
  _ "$REPO/progress/T3R/x86_64/rpm_sha256.tsv" || overall=1

printf 'arch\tfilename\texpected_sha256\tactual_sha256\tresult\n' >"$OUT/artifact_identity.tsv"
for arch in armv7l aarch64 x86_64; do
  artifact="$REPO/artifacts/rebuild_2218/$arch"
  record_to "${arch}_actual_sha256" "$TMP/$arch.actual.tsv" bash -o pipefail -c \
    'sha256sum "$1"/*.rpm | awk "{n=\$2; sub(\".*/\",\"\",n); print n \"\\t\" \$1}" | LC_ALL=C sort' \
    _ "$artifact" || overall=1
  record_to "${arch}_hash_diff" "$RAW/${arch}_hash_diff.stdout.txt" \
    diff -u "$TMP/$arch.expected.tsv" "$TMP/$arch.actual.tsv" || overall=1
  record_to "${arch}_rpm_count" "$RAW/${arch}_rpm_count.stdout.txt" bash -o pipefail -c \
    'printf "binary=%s\\nsrpm=%s\\n" "$(find "$1" -maxdepth 1 -type f -name "*.rpm" ! -name "*.src.rpm" | wc -l)" "$(find "$1" -maxdepth 1 -type f -name "*.src.rpm" | wc -l)"' \
    _ "$artifact" || overall=1
  record_to "${arch}_assert_count" "$RAW/${arch}_assert_count.stdout.txt" bash -o pipefail -c \
    'test "$(find "$1" -maxdepth 1 -type f -name "*.rpm" ! -name "*.src.rpm" | wc -l)" -eq 7 && test "$(find "$1" -maxdepth 1 -type f -name "*.src.rpm" | wc -l)" -eq 1' \
    _ "$artifact" || overall=1
  record_to "${arch}_rpm_digest" "$RAW/${arch}_rpm_digest.stdout.txt" bash -o pipefail -c \
    'for f in "$1"/*.rpm; do rpm -K --nosignature "$f"; done' _ "$artifact" || overall=1
  awk -F'\t' -v arch="$arch" 'NR==FNR {e[$1]=$2; next} {print arch "\t" $1 "\t" e[$1] "\t" $2 "\t" (e[$1]==$2?"MATCH":"MISMATCH")}' \
    "$TMP/$arch.expected.tsv" "$TMP/$arch.actual.tsv" >>"$OUT/artifact_identity.tsv"
done
record_to assert_all_artifact_match "$RAW/assert_all_artifact_match.stdout.txt" bash -o pipefail -c \
  'test "$(awk -F"\t" "NR>1 && \$5 != \"MATCH\" {n++} END {print n+0}" "$1")" -eq 0 && test "$(tail -n +2 "$1" | wc -l)" -eq 24' \
  _ "$OUT/artifact_identity.tsv" || overall=1

record_to current_build_records "$RAW/current_build_records.stdout.txt" bash -o pipefail -c \
  'printf "===== armv7l R6 =====\\n"; cat progress/R6/build_armv7l_git_r3b.status.tsv; printf "===== aarch64 T3R2 =====\\n"; awk -F"\\t" "NR==1 || \$1==\"aarch64\"" progress/T3R2/build_status.tsv; printf "===== x86_64 T3R =====\\n"; cat progress/T3R/x86_64/status.tsv' \
  || overall=1
record_to assert_current_build_records "$RAW/assert_current_build_records.stdout.txt" bash -o pipefail -c \
  'rg -q "^GBS_EXIT_CODE=0$" progress/R6/build_armv7l_git_r3b.status.tsv && awk -F"\\t" "\$1==\"aarch64\" && \$2==0 && \$3==\"SUCCEEDED\" {ok=1} END {exit !ok}" progress/T3R2/build_status.tsv && awk -F"\\t" "\$1==\"x86_64\" && \$2==0 && \$3==\"SUCCEEDED\" {ok=1} END {exit !ok}" progress/T3R/x86_64/status.tsv' \
  || overall=1

mkdir -p "$OUT/provenance"
for arch in armv7l aarch64 x86_64; do
  rpmfile="$REPO/artifacts/rebuild_2218/$arch/libc++-devel-22.1.8-1.$arch.rpm"
  record_to "${arch}_source_provenance" "$OUT/provenance/$arch.SOURCE_PROVENANCE" bash -o pipefail -c \
    'rpm2cpio "$1" | cpio -i --quiet --to-stdout ./usr/share/doc/libcxx-runtimes/SOURCE_PROVENANCE' \
    _ "$rpmfile" || overall=1
done
record_to provenance_sha256 "$OUT/source_provenance_sha256.tsv" sha256sum \
  "$OUT/provenance/armv7l.SOURCE_PROVENANCE" "$OUT/provenance/aarch64.SOURCE_PROVENANCE" \
  "$OUT/provenance/x86_64.SOURCE_PROVENANCE" || overall=1
record_to assert_provenance_identical "$RAW/assert_provenance_identical.stdout.txt" bash -o pipefail -c \
  'test -s "$1" && cmp -s "$1" "$2" && cmp -s "$1" "$3"' _ \
  "$OUT/provenance/armv7l.SOURCE_PROVENANCE" "$OUT/provenance/aarch64.SOURCE_PROVENANCE" \
  "$OUT/provenance/x86_64.SOURCE_PROVENANCE" || overall=1

for arch in armv7l aarch64 x86_64; do
  srpm="$REPO/artifacts/rebuild_2218/$arch/libcxx-runtimes-22.1.8-1.src.rpm"
  mkdir -p "$TMP/$arch"
  record_to "${arch}_srpm_listing" "$RAW/${arch}_srpm_listing.stdout.txt" bash -o pipefail -c \
    'rpm2cpio "$1" | cpio -it' _ "$srpm" || overall=1
  record_to "${arch}_extract_source0" "$TMP/$arch/llvm-22.1.8.tar.gz" bash -o pipefail -c \
    'rpm2cpio "$1" | cpio -i --quiet --to-stdout ./llvm-22.1.8.tar.gz' _ "$srpm" || overall=1
  record_to "${arch}_source0_sha256" "$RAW/${arch}_source0_sha256.stdout.txt" sha256sum \
    "$TMP/$arch/llvm-22.1.8.tar.gz" || overall=1
  record_to "${arch}_source0_unpack" "$RAW/${arch}_source0_unpack.stdout.txt" tar -xf \
    "$TMP/$arch/llvm-22.1.8.tar.gz" -C "$TMP/$arch" || overall=1
done
record_to aarch64_vs_x86_source_tree "$RAW/aarch64_vs_x86_source_tree.diff" diff -qr --no-dereference \
  "$TMP/x86_64/llvm-22.1.8" "$TMP/aarch64/llvm-22.1.8" || overall=1
record_to armv7l_vs_x86_source_tree "$RAW/armv7l_vs_x86_source_tree.diff" diff -qr --no-dereference \
  "$TMP/x86_64/llvm-22.1.8" "$TMP/armv7l/llvm-22.1.8"
arm_tree_rc=$?
if [[ $arm_tree_rc -ne 1 ]]; then overall=1; fi
record_to armv7l_cxa_diff "$RAW/armv7l_cxa_personality.diff" diff -u \
  "$TMP/x86_64/llvm-22.1.8/libcxxabi/src/cxa_personality.cpp" \
  "$TMP/armv7l/llvm-22.1.8/libcxxabi/src/cxa_personality.cpp"
arm_cxa_rc=$?
if [[ $arm_cxa_rc -ne 1 ]]; then overall=1; fi
record_to armv7l_cxa_numstat "$RAW/armv7l_cxa_personality.numstat" git diff --no-index --numstat \
  "$TMP/x86_64/llvm-22.1.8/libcxxabi/src/cxa_personality.cpp" \
  "$TMP/armv7l/llvm-22.1.8/libcxxabi/src/cxa_personality.cpp"
arm_numstat_rc=$?
if [[ $arm_numstat_rc -ne 1 ]]; then overall=1; fi
record_to assert_expected_backport_delta "$RAW/assert_expected_backport_delta.stdout.txt" bash -o pipefail -c \
  'test "$(wc -l < "$1")" -eq 1 && rg -q "libcxxabi/src/cxa_personality.cpp" "$1" && awk "\$1==2 && \$2==0 {ok=1} END {exit !ok}" "$2" && test "$(rg -c "^\\+extern \\\"C\\\" _Unwind_Reason_Code __gnu_unwind_frame" "$3")" -eq 1 && test "$(rg -c "^-extern \\\"C\\\" _Unwind_Reason_Code __gnu_unwind_frame" "$3" || true)" -eq 0' \
  _ "$RAW/armv7l_vs_x86_source_tree.diff" "$RAW/armv7l_cxa_personality.numstat" \
  "$RAW/armv7l_cxa_personality.diff" || overall=1

{
  printf 'arch\tbuild_input\tsource_tree_delta\tclassification\n'
  printf 'x86_64\tplatform SRPM source\tbaseline\tBASELINE\n'
  printf 'aarch64\tplatform SRPM source\tbyte-tree identical to x86_64\tMATCH\n'
  printf 'armv7l\tgit commit 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b\tlibcxxabi/src/cxa_personality.cpp only; 2 additions 0 deletions\tEXPECTED_BACKPORT_DELTA\n'
} >"$OUT/source_input_delta.tsv"

if [[ $overall -eq 0 ]]; then
  printf 'ARTIFACT_IDENTITY=PASS\n' >"$OUT/artifact_identity.status"
else
  printf 'ARTIFACT_IDENTITY=FAIL\n' >"$OUT/artifact_identity.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
