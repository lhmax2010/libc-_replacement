#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/T4"
RAW="$OUT/raw"
OLD_TOOL=/home/toolchain/plan_evaluation/p1/libcxx_packaging/multiarch/run_runtime_gates.sh
ARM_GBS="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b"
X86_GBS="$REPO/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260804"
LEDGER="$OUT/commands/03a_runtime_fixture_and_carrier_gates_retry.log"

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
record_to aarch64_qemu_path_retry "$RAW/aarch64_qemu_path_retry.stdout.txt" bash -o pipefail -c \
  'command -v qemu-aarch64-static || true; command -v qemu-aarch64 || true; command -v qemu-aarch64-binfmt || true; whereis qemu-aarch64-static qemu-aarch64 qemu-aarch64-binfmt; find /usr/bin /usr/sbin /usr/local/bin /usr/local/sbin /opt -xdev -type f \( -name "qemu-aarch64-static" -o -name "qemu-aarch64" -o -name "qemu-aarch64-binfmt" \) -print 2>/dev/null; rpm -qa | LC_ALL=C sort | rg -i "^qemu|qemu.*aarch64" || true' || overall=1
record_to assert_aarch64_qemu_not_available_retry "$RAW/assert_aarch64_qemu_not_available_retry.stdout.txt" bash -o pipefail -c \
  '! rg -q "^/.*qemu-aarch64" "$1" && test ! -x /usr/bin/qemu-aarch64-static && test ! -x /usr/bin/qemu-aarch64 && test ! -x /usr/bin/qemu-aarch64-binfmt' \
  _ "$RAW/aarch64_qemu_path_retry.stdout.txt" || overall=1
record_to arm_qemu_identity_retry "$RAW/arm_qemu_identity_retry.stdout.txt" file /usr/bin/qemu-arm-static /usr/bin/qemu-arm-binfmt || overall=1
record_to x86_native_identity_retry "$RAW/x86_native_identity_retry.stdout.txt" uname -m || overall=1
record_to assert_runtime_input_identity_retry "$RAW/assert_runtime_input_identity_retry.stdout.txt" bash -o pipefail -c \
  'test -d "$1/local/BUILD-ROOTS/scratch.armv7l.0" && test -d "$2/local/BUILD-ROOTS/scratch.x86_64.0" && test "$(awk -F"\t" "NR>1 && \$5!=\"MATCH\" {n++} END {print n+0}" "$3")" -eq 0' \
  _ "$ARM_GBS" "$X86_GBS" "$OUT/artifact_identity.tsv" || overall=1

printf 'architecture\texecution_mode\truntime_tool_exit\tavailability\n' >"$OUT/runtime_carrier.tsv"
for arch in armv7l x86_64; do
  gbs_root=$ARM_GBS
  mode=QEMU_ARM_BINFMT_GBS_CHROOT
  [[ $arch == x86_64 ]] && { gbs_root=$X86_GBS; mode=NATIVE_X86_64_GBS_CHROOT; }
  record_to "${arch}_old_runtime_gate_tool_retry" "$RAW/${arch}_old_runtime_gate_tool_retry.stdout.txt" \
    "$OLD_TOOL" "$arch" "$gbs_root" "$OUT/$arch/static"
  rc=$?
  if [[ $rc -ne 0 ]]; then overall=1; fi
  printf '%s\t%s\t%s\tAVAILABLE\n' "$arch" "$mode" "$rc" >>"$OUT/runtime_carrier.tsv"
done
printf 'aarch64\tNO_QEMU_AARCH64\tNA\tNOT_AVAILABLE\n' >>"$OUT/runtime_carrier.tsv"

if [[ $overall -eq 0 ]]; then
  printf 'RUNTIME_FIXTURE_CARRIERS=PASS_ARM_X86_AARCH64_NOT_AVAILABLE\n' >"$OUT/runtime_carrier.status"
else
  printf 'RUNTIME_FIXTURE_CARRIERS=RED\n' >"$OUT/runtime_carrier.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
