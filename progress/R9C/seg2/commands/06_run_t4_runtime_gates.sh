#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9C/seg2"
RAW="$OUT/gates/raw"
OLD_TOOL=/home/toolchain/plan_evaluation/p1/libcxx_packaging/multiarch/run_runtime_gates.sh
LEDGER="$OUT/commands/06_run_t4_runtime_gates.log"
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
record_to old_runtime_tool_sha "$OUT/gates/old_runtime_tool.sha256" sha256sum "$OLD_TOOL" || overall=1
record_to qemu_carrier_inventory "$OUT/gates/qemu_carrier_inventory.txt" bash -o pipefail -c \
  'for q in qemu-arm-static qemu-arm-binfmt qemu-aarch64-static qemu-aarch64 qemu-aarch64-binfmt qemu-arm64-static qemu-arm64-binfmt; do printf "%s\t" "$q"; command -v "$q" || true; done; file /usr/bin/qemu-arm-static /usr/bin/qemu-arm-binfmt /usr/bin/qemu-arm64-static /usr/bin/qemu-arm64-binfmt; /usr/bin/qemu-arm64-static --version' || overall=1

printf 'architecture\texecution_mode\truntime_tool_exit\tavailability\n' >"$OUT/gates/runtime_carrier.tsv"
for arch in armv7l aarch64 x86_64; do
  root="$REPO/tmp/GBS-ROOT/LIBCXX-2218-${arch}-20260806-c2"
  mode=QEMU_ARM_BINFMT_GBS_CHROOT
  [[ $arch == aarch64 ]] && mode=QEMU_ARM64_BINFMT_GBS_CHROOT
  [[ $arch == x86_64 ]] && mode=NATIVE_X86_64_GBS_CHROOT
  record_to "${arch}_assert_runtime_root" "$RAW/${arch}_assert_runtime_root.stdout.txt" bash -o pipefail -c \
    'test -d "$1/local/BUILD-ROOTS/scratch.$2.0" && rg -q "RETIRED_SINGLE_USE" "$1/RETIRED_SINGLE_USE" && test "$(find "$3/artifacts" -maxdepth 1 -type f -name "*.rpm" | wc -l)" -eq 8' _ \
    "$root" "$arch" "$OUT/gates/$arch/static" || { overall=1; printf '%s\t%s\tNA\tNOT_AVAILABLE_INPUT_ASSERTION\n' "$arch" "$mode" >>"$OUT/gates/runtime_carrier.tsv"; continue; }
  record_to "${arch}_carrier_probe" "$RAW/${arch}_carrier_probe.stdout.txt" unshare -Ur chroot \
    "$root/local/BUILD-ROOTS/scratch.$arch.0" /bin/true
  probe_rc=$?
  if [[ $probe_rc -ne 0 ]]; then
    printf '%s\t%s\t%s\tNOT_AVAILABLE_CARRIER_PROBE_FAILED\n' "$arch" "$mode" "$probe_rc" >>"$OUT/gates/runtime_carrier.tsv"
    continue
  fi
  record_to "${arch}_old_runtime_gate_tool" "$RAW/${arch}_old_runtime_gate_tool.stdout.txt" \
    "$OLD_TOOL" "$arch" "$root" "$OUT/gates/$arch/static"
  rc=$?
  if [[ $rc -ne 0 ]]; then overall=1; fi
  printf '%s\t%s\t%s\tAVAILABLE\n' "$arch" "$mode" "$rc" >>"$OUT/gates/runtime_carrier.tsv"
done

if [[ $overall -eq 0 ]]; then
  printf 'RUNTIME_FIXTURE_CARRIERS=PASS_ALL_THREE\n' >"$OUT/gates/runtime_carrier.status"
else
  printf 'RUNTIME_FIXTURE_CARRIERS=RED_OR_PARTIAL\n' >"$OUT/gates/runtime_carrier.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
