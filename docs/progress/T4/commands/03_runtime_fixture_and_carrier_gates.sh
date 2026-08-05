#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/T4"
RAW="$OUT/raw"
OLD_TOOL=/home/toolchain/plan_evaluation/p1/libcxx_packaging/multiarch/run_runtime_gates.sh
ARM_GBS="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b"
X86_GBS="$REPO/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260804"
LEDGER="$OUT/commands/03_runtime_fixture_and_carrier_gates.log"

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
record_to old_runtime_tool_sha256 "$RAW/old_runtime_tool_sha256.stdout.txt" sha256sum "$OLD_TOOL" || overall=1
record_to old_runtime_tool_identity_match "$RAW/old_runtime_tool_identity_match.stdout.txt" bash -o pipefail -c \
  'test "$(sha256sum "$1" | cut -d" " -f1)" = "$(awk "\$2==\"$1\" {print \$1}" "$2")"' \
  _ "$OLD_TOOL" "$OUT/raw/old_reference_sha256.stdout.txt" || overall=1
record_to arm_runtime_carriers "$RAW/arm_runtime_carriers.stdout.txt" file /usr/bin/qemu-arm-static /usr/bin/qemu-arm-binfmt || overall=1
record_to x86_runtime_carrier "$RAW/x86_runtime_carrier.stdout.txt" uname -m || overall=1
record_to assert_runtime_roots "$RAW/assert_runtime_roots.stdout.txt" test -d \
  "$ARM_GBS/local/BUILD-ROOTS/scratch.armv7l.0" || overall=1
record_to assert_x86_runtime_root "$RAW/assert_x86_runtime_root.stdout.txt" test -d \
  "$X86_GBS/local/BUILD-ROOTS/scratch.x86_64.0" || overall=1
record_to aarch64_qemu_path_lookup "$RAW/aarch64_qemu_path_lookup.stdout.txt" bash -o pipefail -c \
  'command -v qemu-aarch64-static || true; command -v qemu-aarch64 || true; command -v qemu-aarch64-binfmt || true; find /usr /home/toolchain -xdev -type f \( -name "qemu-aarch64-static" -o -name "qemu-aarch64" -o -name "qemu-aarch64-binfmt" \) -print 2>/dev/null' || overall=1
record_to assert_aarch64_qemu_not_available "$RAW/assert_aarch64_qemu_not_available.stdout.txt" bash -o pipefail -c \
  'test ! -s "$1" && test ! -x /usr/bin/qemu-aarch64-static && test ! -x /usr/bin/qemu-aarch64-binfmt' \
  _ "$RAW/aarch64_qemu_path_lookup.stdout.txt" || overall=1

printf 'architecture\texecution_mode\truntime_tool_exit\tavailability\n' >"$OUT/runtime_carrier.tsv"
for arch in armv7l x86_64; do
  gbs_root=$ARM_GBS
  mode=QEMU_ARM_BINFMT_GBS_CHROOT
  [[ $arch == x86_64 ]] && { gbs_root=$X86_GBS; mode=NATIVE_X86_64_GBS_CHROOT; }
  record_to "${arch}_old_runtime_gate_tool" "$RAW/${arch}_old_runtime_gate_tool.stdout.txt" \
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
