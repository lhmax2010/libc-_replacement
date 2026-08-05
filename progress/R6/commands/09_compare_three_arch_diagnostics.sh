#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R6"
RAW="$OUT/raw"
LEDGER="$OUT/commands/09_compare_three_arch_diagnostics.log"
ARM="$OUT/build_armv7l_git_r3b.full.log"
AARCH="$REPO/progress/T3R2/build_aarch64.full.log"
X86="$REPO/progress/T3R/build_x86_64.full.log"
: >"$LEDGER"

run_record() {
  local label=$1
  shift
  {
    printf 'LABEL=%s\n' "$label"
    printf 'PWD=%s\n' "$PWD"
    printf 'COMMAND='
    printf '%q ' "$@"
    printf '\n'
  } >>"$LEDGER"
  "$@" >"$RAW/$label.stdout.txt" 2>"$RAW/$label.stderr.txt"
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

DIAG_RE='(^|[[:space:]#])(gbs:)?(warning|error):[[:space:]]|\*\*\* WARNING:|mv: cannot stat|chroot: failed|cpio: .*Cannot stat'
overall=0
run_record armv7l_diagnostics rg -n -i -- "$DIAG_RE" "$ARM" || overall=1
run_record aarch64_diagnostics rg -n -i -- "$DIAG_RE" "$AARCH" || overall=1
run_record x86_64_diagnostics rg -n -i -- "$DIAG_RE" "$X86" || overall=1
run_record armv7l_compiler_warning_count bash -o pipefail -c 'rg -c -- ":[0-9]+:[0-9]+: warning:" "$1" || test $? -eq 1' _ "$ARM" || overall=1
run_record armv7l_compiler_error_count bash -o pipefail -c 'rg -c -- ":[0-9]+:[0-9]+: error:" "$1" || test $? -eq 1' _ "$ARM" || overall=1
run_record armv7l_fatal_absence bash -o pipefail -c '! rg -n -- "FAILED:|ninja: build stopped|rpmbuild.*failed|gbs:error" "$1"' _ "$ARM" || overall=1
run_record armv7l_unique_profile_warning rg -n -- 'warning: /etc/profile created as /etc/profile.rpmnew' "$ARM" || overall=1
run_record assert_profile_warning_absent_other bash -o pipefail -c '! rg -n -- "warning: /etc/profile created as /etc/profile.rpmnew" "$1" "$2"' _ "$AARCH" "$X86" || overall=1

count_pattern() {
  local pattern=$1
  local file=$2
  awk -v p="$pattern" 'BEGIN{IGNORECASE=1; c=0} $0 ~ p {c++} END{print c}' "$file"
}

{
  printf 'category\tarmv7l\taarch64\tx86_64\n'
  for item in \
    'GBS_NO_LOCAL_REPO|gbs:warning: No local package repository' \
    'RPM_PROFILE_RPMNEW|warning: /etc/profile created as /etc/profile.rpmnew' \
    'MISSING_BUILD_ID|WARNING: No build ID note found|warning: Missing build-id' \
    'DEBUGSOURCE_CPIO_MISSING|cpio: libcxx/include/cxxabi.h: Cannot stat' \
    'QEMU_POSTINSTALL_LTO_PLUGIN|mv: cannot stat .*liblto_plugin.so' \
    'QEMU_POSTINSTALL_LUA|chroot: failed to run command .*/usr/bin/lua' \
    'COMPILER_WARNING|:[0-9]+:[0-9]+: warning:' \
    'COMPILER_ERROR|:[0-9]+:[0-9]+: error:' \
    'FATAL_BUILD_ERROR|FAILED:|ninja: build stopped|gbs:error'; do
    category=${item%%|*}
    pattern=${item#*|}
    printf '%s\t%s\t%s\t%s\n' "$category" \
      "$(count_pattern "$pattern" "$ARM")" \
      "$(count_pattern "$pattern" "$AARCH")" \
      "$(count_pattern "$pattern" "$X86")"
  done
} >"$OUT/three_arch_diagnostic_category_counts.tsv"

{
  printf 'arch\tline\tdiagnostic\n'
  sed 's/^/armv7l\t/' "$RAW/armv7l_unique_profile_warning.stdout.txt"
} >"$OUT/armv7l_exclusive_diagnostics.tsv"

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
