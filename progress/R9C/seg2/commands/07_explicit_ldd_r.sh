#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9C/seg2"
RAW="$OUT/gates/raw"
PROBES="$OUT/probes"
LEDGER="$OUT/commands/07_explicit_ldd_r.log"
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
printf 'architecture\texecution_method\tlibcxx_exit\tlibcxxabi_exit\tstrong_unresolved\tforbidden_runtime\tverdict\n' >"$OUT/gates/explicit_ldd_r.tsv"

for arch in armv7l aarch64; do
  root="$REPO/tmp/GBS-ROOT/LIBCXX-2218-${arch}-20260806-c2/local/BUILD-ROOTS/scratch.${arch}.0"
  extract="$OUT/gates/$arch/static/extracted"
  src="$PROBES/rtld_${arch}_qemu_wrapper.c"
  bin="$PROBES/rtld-${arch}-qemu-wrapper.bin"
  adapted="$PROBES/ldd.${arch}.qemu-static"
  libdir=lib
  rtld='RTLDLIST=/lib/ld-linux.so.3'
  qemu=/usr/bin/qemu-arm-static
  method='qemu-arm-static_-L_target_root_via_rtld_wrapper'
  if [[ $arch == aarch64 ]]; then
    libdir=lib64
    rtld='RTLDLIST=/lib64/ld-linux-aarch64.so.1'
    qemu=/usr/bin/qemu-arm64-static
    method='qemu-arm64-static_-L_target_root_via_rtld_wrapper'
  fi
  ldd_orig="$root/usr/bin/ldd"
  record_to "${arch}_compile_static_rtld_wrapper" "$RAW/${arch}_compile_static_rtld_wrapper.stdout.txt" gcc -O2 -Wall -Wextra -Werror -static -o "$bin" "$src" || overall=1
  record_to "${arch}_wrapper_identity" "$RAW/${arch}_wrapper_identity.stdout.txt" bash -o pipefail -c \
    'file "$1"; sha256sum "$1" "$2" "$3" "$4"' _ "$bin" "$src" "$qemu" "$ldd_orig" || overall=1
  record_to "${arch}_create_adapted_ldd" "$RAW/${arch}_adapted_ldd.generated.txt" sed \
    "s#^${rtld}\$#RTLDLIST=$bin#" "$ldd_orig" || overall=1
  record_to "${arch}_write_adapted_ldd" "$RAW/${arch}_write_adapted_ldd.stdout.txt" cp "$RAW/${arch}_adapted_ldd.generated.txt" "$adapted" || overall=1
  record_to "${arch}_adapted_ldd_diff" "$RAW/${arch}_adapted_ldd.diff" diff -u "$ldd_orig" "$adapted"
  diff_rc=$?
  [[ $diff_rc -eq 1 ]] || overall=1
  record_to "${arch}_assert_single_ldd_adaptation" "$RAW/${arch}_assert_single_ldd_adaptation.stdout.txt" bash -o pipefail -c \
    'test "$(diff -u "$1" "$2" | rg -c "^[+-]RTLDLIST=")" -eq 2 && test "$(diff -u "$1" "$2" | rg -c "^[+-][^+-]")" -eq 2' _ \
    "$ldd_orig" "$adapted" || overall=1
  record_to "${arch}_explicit_qemu_ldd_r_libcxx" "$OUT/gates/$arch/static/evidence/explicit_qemu_ldd_r_libcxx.log" \
    /bin/bash "$adapted" -r "$extract/usr/$libdir/libc++.so.1"
  libcxx_rc=$?
  record_to "${arch}_explicit_qemu_ldd_r_libcxxabi" "$OUT/gates/$arch/static/evidence/explicit_qemu_ldd_r_libcxxabi.log" \
    /bin/bash "$adapted" -r "$extract/usr/$libdir/libc++abi.so.1"
  abi_rc=$?
  unresolved=$(rg -i 'undefined symbol:|not found' "$OUT/gates/$arch/static/evidence/explicit_qemu_ldd_r_"*.log 2>/dev/null | wc -l)
  forbidden=$(rg -i 'libstdc\+\+|libunwind' "$OUT/gates/$arch/static/evidence/explicit_qemu_ldd_r_"*.log 2>/dev/null | wc -l)
  verdict=PASS
  if [[ $libcxx_rc -ne 0 || $abi_rc -ne 0 || $unresolved -ne 0 || $forbidden -ne 0 ]]; then verdict=FAIL; overall=1; fi
  printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$arch" "$method" "$libcxx_rc" "$abi_rc" "$unresolved" "$forbidden" "$verdict" >>"$OUT/gates/explicit_ldd_r.tsv"
done

x86_log="$OUT/gates/x86_64/static/evidence/needed_closure_ldd_r.log"
record_to x86_64_assert_native_ldd "$RAW/x86_64_assert_native_ldd.stdout.txt" bash -o pipefail -c \
  'rg -q "libcxx_ldd_exit=0" "$1" && rg -q "libcxxabi_ldd_exit=0" "$1" && ! rg -qi "undefined symbol:|not found|libstdc\\+\\+|libunwind" "$1"' _ "$x86_log" || overall=1
x86_unresolved=$(( $(rg -i 'undefined symbol:|not found' "$x86_log" 2>/dev/null | wc -l) ))
x86_forbidden=$(( $(rg -i 'libstdc\+\+|libunwind' "$x86_log" 2>/dev/null | wc -l) ))
x86_verdict=PASS
[[ $x86_unresolved -eq 0 && $x86_forbidden -eq 0 ]] || x86_verdict=FAIL
printf 'x86_64\tnative_x86_64_GBS_chroot_/usr/bin/ldd_-r\t0\t0\t%s\t%s\t%s\n' "$x86_unresolved" "$x86_forbidden" "$x86_verdict" >>"$OUT/gates/explicit_ldd_r.tsv"

if [[ $overall -eq 0 ]]; then
  printf 'EXPLICIT_LDD_R=PASS_ALL_THREE\n' >"$OUT/gates/explicit_ldd_r.status"
else
  printf 'EXPLICIT_LDD_R=RED\n' >"$OUT/gates/explicit_ldd_r.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
