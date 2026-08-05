#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/T4"
RAW="$OUT/raw"
WRAPPER="$OUT/probes/rtld-armv7l-qemu-wrapper.bin"
LEDGER="$OUT/commands/04a_retry_wrapper_identity_assertion.log"

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

cd "$REPO" || exit 125
overall=0
record_to retry_arm_wrapper_identity "$RAW/retry_arm_wrapper_identity.stdout.txt" bash -o pipefail -c \
  'file "$1" | rg "x86-64.*statically linked" && rg -a "/usr/bin/qemu-arm-static" "$1" && rg -a "scratch.armv7l.0" "$1" && rg -a "progress/T4/armv7l/static/extracted/usr/lib" "$1"' \
  _ "$WRAPPER" || overall=1
record_to verify_04_successful_subcommands "$RAW/verify_04_successful_subcommands.stdout.txt" bash -o pipefail -c \
  'awk '\''BEGIN {want["arm_explicit_qemu_ldd_r_libcxx"]=1; want["arm_explicit_qemu_ldd_r_libcxxabi"]=1; want["assert_arm_explicit_ldd_results"]=1; want["armv7l_assert_provider_family"]=1; want["x86_64_assert_provider_family"]=1; want["assert_x86_native_ldd_results"]=1} /^LABEL=/ {label=substr($0,7); seen=(label in want); next} seen && /^EXIT_CODE=/ {if($0!="EXIT_CODE=0") exit 1; found++; seen=0} END {if(found!=6) exit 1}'\'' "$1"' \
  _ "$OUT/commands/04_explicit_ldd_and_provider_verdicts.log" || overall=1

if [[ $overall -eq 0 ]]; then
  printf 'EXPLICIT_LDD_AND_PROVIDER=PASS_ARM_X86_AARCH64_NOT_AVAILABLE_AFTER_ASSERTION_RETRY\n' >"$OUT/explicit_ldd_provider.status"
else
  printf 'EXPLICIT_LDD_AND_PROVIDER=RED\n' >"$OUT/explicit_ldd_provider.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
