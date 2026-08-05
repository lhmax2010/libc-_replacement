#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R8"
RAW="$OUT/raw"
LOG="$REPO/progress/R6/build_armv7l_git_r3b.full.log"
ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b/local/BUILD-ROOTS/scratch.armv7l.0"
BUILD="$ROOT/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build"
NINJA="$BUILD/build.ninja"
DIFFS="$OUT/source_difference_files.tsv"
LEDGER="$OUT/commands/02_r6_compile_dependency_intersection.log"
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
record_to retired_root_and_build_state "$RAW/retired_root_and_build_state.stdout.txt" bash -o pipefail -c '
  printf "ROOT=%s\nBUILD=%s\n" "$1" "$2"
  test -d "$1" && test -f "$2/build.ninja" && test -f "$2/.ninja_deps" && test -f "$3"
  find "$1" -maxdepth 3 -type f -name "RETIRED_SINGLE_USE*" -print
  sha256sum "$2/build.ninja" "$2/.ninja_deps" "$3"
' _ "$ROOT" "$BUILD" "$LOG" || overall=1

record_to r6_build_log_compiled_objects "$RAW/r6_build_log_compiled_objects.numbered.txt" bash -o pipefail -c \
  'nl -ba "$1" | rg "Building (CXX|C|ASM) object"' _ "$LOG" || overall=1
record_to assert_r6_compile_object_count "$RAW/assert_r6_compile_object_count.stdout.txt" bash -o pipefail -c \
  'test "$(rg -c "Building (CXX|C|ASM) object" "$1")" -eq 78' _ "$LOG" || overall=1

record_to map_r6_objects_to_translation_units "$OUT/r6_actual_compiled_tu.tsv" awk '
  BEGIN { print "log_line\tobject\tsource" }
  FNR==NR {
    if ($1=="build") { target=$2; sub(/:$/, "", target); source[target]=$4 }
    next
  }
  /Building (CXX|C|ASM) object/ {
    p=index($0,"Building "); obj=substr($0,p)
    sub(/^Building (CXX|C|ASM) object /,"",obj)
    printf "%d\t%s\t%s\n", FNR,obj,source[obj]
  }
' "$NINJA" "$LOG" || overall=1
record_to assert_tu_mapping_complete "$RAW/assert_tu_mapping_complete.stdout.txt" bash -o pipefail -c '
  test "$(tail -n +2 "$1" | wc -l)" -eq 78 &&
  test "$(awk -F"\t" "NR>1 && \$3==\"\" {n++} END {print n+0}" "$1")" -eq 0 &&
  test "$(awk -F"\t" "NR>1 && \$3 ~ /libcxxabi\\/src\\/cxa_personality.cpp$/ {n++} END {print n+0}" "$1")" -eq 1
' _ "$OUT/r6_actual_compiled_tu.tsv" || overall=1

record_to ninja_actual_header_dependencies "$RAW/ninja_all_dependencies.stdout.txt" ninja -C "$BUILD" -t deps || overall=1
record_to select_actual_object_dependencies "$OUT/r6_actual_object_dependencies.tsv" awk -F'\t' '
  FNR==NR { if (FNR>1) wanted[$2]=1; next }
  $0 !~ /^ / {
    target=$0; sub(/: #deps.*/,"",target); active=(target in wanted); next
  }
  active && /^    / {
    dep=$0; sub(/^    /,"",dep); print target "\t" dep
  }
' "$OUT/r6_actual_compiled_tu.tsv" "$RAW/ninja_all_dependencies.stdout.txt" || overall=1

record_to ninja_cmake_consumed_inputs "$RAW/ninja_build_ninja_query.stdout.txt" ninja -C "$BUILD" -t query build.ninja || overall=1
record_to select_cmake_consumed_source_inputs "$OUT/r6_cmake_consumed_inputs.tsv" bash -o pipefail -c '
  printf "relative_path\traw_path\n"
  sed -n "s#^[[:space:]]*| /home/abuild/rpmbuild/BUILD/llvm-22.1.8/##p" "$1" |
    LC_ALL=C sort -u | while IFS= read -r p; do printf "%s\t%s%s\n" "$p" "/home/abuild/rpmbuild/BUILD/llvm-22.1.8/" "$p"; done
' _ "$RAW/ninja_build_ninja_query.stdout.txt" || overall=1

record_to normalize_actual_consumed_source_paths "$OUT/r6_consumed_source_paths.tsv" bash -o pipefail -c '
  printf "relative_path\tevidence_kind\tevidence_object\n"
  awk -F"\t" "NR>1 {p=\$3; sub(/^.*\\/llvm-22\\.1\\.8\\//,\"\",p); print p \"\\tDIRECT_TU\\t\" \$2}" "$1"
  awk -F"\t" "{p=\$2; if (p ~ /\\/llvm-22\\.1\\.8\\//) {sub(/^.*\\/llvm-22\\.1\\.8\\//,\"\",p); print p \"\\tINCLUDE_DEPENDENCY\\t\" \$1}}" "$2"
  awk -F"\t" "NR>1 {print \$1 \"\\tCMAKE_CONSUMED\\tbuild.ninja:RERUN_CMAKE\"}" "$3"
' _ "$OUT/r6_actual_compiled_tu.tsv" "$OUT/r6_actual_object_dependencies.tsv" "$OUT/r6_cmake_consumed_inputs.tsv" || overall=1

record_to intersect_differences_with_consumed_paths "$OUT/difference_build_intersection.tsv" awk -F'\t' '
  FNR==NR { if (FNR>1) { scope[$2]=$1; kind[$2]=$3 } next }
  FNR>1 && ($1 in scope) { print scope[$1] "\t" $1 "\t" kind[$1] "\t" $2 "\t" $3 }
' "$DIFFS" "$OUT/r6_consumed_source_paths.tsv" || overall=1

record_to summarize_build_intersection "$OUT/difference_build_intersection_summary.tsv" bash -o pipefail -c '
  printf "question\tconclusion\tevidence\n"
  all=$(cut -f2 "$1" | LC_ALL=C sort -u | paste -sd, -)
  nonruntime=$(awk -F"\t" "\$1 ~ /^nonruntime_/ {print \$2}" "$1" | LC_ALL=C sort -u | paste -sd, -)
  runtime=$(awk -F"\t" "\$1 ~ /^runtime_/ {print \$2}" "$1" | LC_ALL=C sort -u | paste -sd, -)
  test -n "$all" || all=NONE
  test -n "$nonruntime" || nonruntime=NONE
  test -n "$runtime" || runtime=NONE
  printf "all_discovered_differences\t%s\t%s\n" "$(test "$all" = NONE && echo NO_INTERSECTION || echo INTERSECTION_FOUND)" "$all"
  printf "nonruntime_tizen_downstream_differences\t%s\t%s\n" "$(test "$nonruntime" = NONE && echo NO_INTERSECTION || echo INTERSECTION_FOUND)" "$nonruntime"
  printf "runtime_differences\t%s\t%s\n" "$(test "$runtime" = NONE && echo NO_INTERSECTION || echo INTERSECTION_FOUND)" "$runtime"
' _ "$OUT/difference_build_intersection.tsv" || overall=1
record_to assert_expected_measured_intersection "$RAW/assert_expected_measured_intersection.stdout.txt" bash -o pipefail -c '
  test "$(cut -f2 "$1" | sort -u | wc -l)" -eq 1 &&
  test "$(cut -f2 "$1" | sort -u)" = libcxxabi/src/cxa_personality.cpp &&
  test "$(awk -F"\t" "\$1 ~ /^nonruntime_/ {n++} END {print n+0}" "$1")" -eq 0
' _ "$OUT/difference_build_intersection.tsv" || overall=1

if [[ $overall -eq 0 ]]; then
  printf 'BUILD_INTERSECTION=PASS_ONLY_EHABI_BACKPORT;NONRUNTIME_TIZEN_DELTA=NO_INTERSECTION\n' >"$OUT/build_intersection.status"
else
  printf 'BUILD_INTERSECTION=FAIL_OR_UNDETERMINED\n' >"$OUT/build_intersection.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
