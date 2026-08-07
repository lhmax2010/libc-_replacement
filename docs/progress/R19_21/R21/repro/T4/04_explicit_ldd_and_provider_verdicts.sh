#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/T4"
RAW="$OUT/raw"
PROBES="$OUT/probes"
ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b/local/BUILD-ROOTS/scratch.armv7l.0"
EXTRACT="$OUT/armv7l/static/extracted"
LDD_ORIG="$ROOT/usr/bin/ldd"
WRAPPER_SRC="$PROBES/rtld_armv7l_qemu_wrapper.c"
WRAPPER_BIN="$PROBES/rtld-armv7l-qemu-wrapper.bin"
LDD_ADAPTED="$PROBES/ldd.armv7l.qemu-static"
LEDGER="$OUT/commands/04_explicit_ldd_and_provider_verdicts.log"

mkdir -p "$PROBES"
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
record_to compile_arm_static_rtld_wrapper "$RAW/compile_arm_static_rtld_wrapper.stdout.txt" gcc -O2 -Wall -Wextra -Werror -static -o "$WRAPPER_BIN" "$WRAPPER_SRC" || overall=1
record_to arm_static_rtld_wrapper_identity "$RAW/arm_static_rtld_wrapper_identity.stdout.txt" file "$WRAPPER_BIN" || overall=1
record_to assert_arm_static_rtld_wrapper "$RAW/assert_arm_static_rtld_wrapper.stdout.txt" bash -o pipefail -c \
  'file "$1" | rg -q "x86-64.*statically linked" && strings "$1" | rg -q "/usr/bin/qemu-arm-static" && strings "$1" | rg -q "scratch.armv7l.0" && strings "$1" | rg -q "progress/T4/armv7l/static/extracted/usr/lib"' \
  _ "$WRAPPER_BIN" || overall=1
record_to arm_wrapper_verify_candidate "$RAW/arm_wrapper_verify_candidate.stdout.txt" "$WRAPPER_BIN" --verify "$EXTRACT/usr/lib/libc++abi.so.1"
verify_rc=$?
if [[ $verify_rc -ne 2 ]]; then overall=1; fi
record_to create_arm_adapted_ldd "$RAW/create_arm_adapted_ldd.stdout.txt" sed \
  "s#^RTLDLIST=/lib/ld-linux.so.3\$#RTLDLIST=$WRAPPER_BIN#" "$LDD_ORIG" || overall=1
record_to write_arm_adapted_ldd "$RAW/write_arm_adapted_ldd.stdout.txt" cp "$RAW/create_arm_adapted_ldd.stdout.txt" "$LDD_ADAPTED" || overall=1
record_to arm_adapted_ldd_diff "$RAW/arm_adapted_ldd.diff" diff -u "$LDD_ORIG" "$LDD_ADAPTED"
diff_rc=$?
if [[ $diff_rc -ne 1 ]]; then overall=1; fi
record_to assert_single_arm_ldd_adaptation "$RAW/assert_single_arm_ldd_adaptation.stdout.txt" bash -o pipefail -c \
  'test "$(diff -u "$1" "$2" | rg -c "^[+-]RTLDLIST=")" -eq 2 && test "$(diff -u "$1" "$2" | rg -c "^[+-][^+-]")" -eq 2' \
  _ "$LDD_ORIG" "$LDD_ADAPTED" || overall=1
record_to arm_ldd_components_sha256 "$RAW/arm_ldd_components_sha256.stdout.txt" sha256sum \
  /usr/bin/qemu-arm-static "$WRAPPER_SRC" "$WRAPPER_BIN" "$LDD_ORIG" "$LDD_ADAPTED" || overall=1
record_to arm_explicit_qemu_ldd_r_libcxx "$OUT/armv7l/static/evidence/explicit_qemu_ldd_r_libcxx.log" /bin/bash "$LDD_ADAPTED" -r "$EXTRACT/usr/lib/libc++.so.1" || overall=1
record_to arm_explicit_qemu_ldd_r_libcxxabi "$OUT/armv7l/static/evidence/explicit_qemu_ldd_r_libcxxabi.log" /bin/bash "$LDD_ADAPTED" -r "$EXTRACT/usr/lib/libc++abi.so.1" || overall=1
record_to assert_arm_explicit_ldd_results "$RAW/assert_arm_explicit_ldd_results.stdout.txt" bash -o pipefail -c \
  'for f in "$1" "$2"; do ! rg -q "undefined symbol:|not found|libstdc\\+\\+|libunwind" "$f" || exit 1; rg -q "ld-linux.so.3|scratch.armv7l.0" "$f" || exit 1; done' \
  _ "$OUT/armv7l/static/evidence/explicit_qemu_ldd_r_libcxx.log" "$OUT/armv7l/static/evidence/explicit_qemu_ldd_r_libcxxabi.log" || overall=1

printf 'architecture\t__cxa_finalize\t_Unwind_RaiseException\t__cxa_throw\t__cxa_allocate_exception\t__gxx_personality_v0\tverdict\n' >"$OUT/provider_family_results.tsv"
for arch in armv7l x86_64; do
  bindings="$OUT/$arch/static/evidence/smoke_provider_bindings.log"
  record_to "${arch}_assert_provider_family" "$RAW/${arch}_assert_provider_family.stdout.txt" bash -o pipefail -c \
    'rg -q "to /lib(64)?/libc\\.so\\.6.*__cxa_finalize" "$1" && rg -q "to /lib(64)?/libgcc_s\\.so\\.1.*_Unwind_RaiseException" "$1" && rg -q "to .*sysroot/usr/lib(64)?/libc\\+\\+abi\\.so\\.1.*__cxa_throw" "$1" && rg -q "to .*sysroot/usr/lib(64)?/libc\\+\\+abi\\.so\\.1.*__cxa_allocate_exception" "$1" && rg -q "to .*sysroot/usr/lib(64)?/libc\\+\\+abi\\.so\\.1.*__gxx_personality_v0" "$1"' \
    _ "$bindings" || overall=1
  printf '%s\tglibc\tlibgcc_s\tcandidate_libc++abi\tcandidate_libc++abi\tcandidate_libc++abi\tPASS\n' "$arch" >>"$OUT/provider_family_results.tsv"
done
printf 'aarch64\tNOT_AVAILABLE\tNOT_AVAILABLE\tNOT_AVAILABLE\tNOT_AVAILABLE\tNOT_AVAILABLE\tNOT_AVAILABLE_NO_QEMU\n' >>"$OUT/provider_family_results.tsv"

record_to assert_x86_native_ldd_results "$RAW/assert_x86_native_ldd_results.stdout.txt" bash -o pipefail -c \
  '! rg -q "undefined symbol:|not found|libstdc\\+\\+|libunwind" "$1" && rg -q "NATIVE_X86_64_GBS_CHROOT" "$2"' \
  _ "$OUT/x86_64/static/evidence/needed_closure_ldd_r.log" "$OUT/x86_64/static/runtime_smoke.tsv" || overall=1

if [[ $overall -eq 0 ]]; then
  printf 'EXPLICIT_LDD_AND_PROVIDER=PASS_ARM_X86_AARCH64_NOT_AVAILABLE\n' >"$OUT/explicit_ldd_provider.status"
else
  printf 'EXPLICIT_LDD_AND_PROVIDER=RED\n' >"$OUT/explicit_ldd_provider.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
