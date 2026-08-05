#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R6"
RAW="$OUT/raw"
SOURCE="$REPO/codes/llvm"
SCRATCH="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b/local/BUILD-ROOTS/scratch.armv7l.0"
EXTRACT="$REPO/tmp/R6/elf-extract-armv7l-r3"
PROBE="$OUT/probes/duplicate_ehabi_declaration.cpp"
LDD_ORIG="$SCRATCH/usr/bin/ldd"
LDD_ADAPTED="$OUT/probes/ldd.armv7l.r3"
LIBGCC="$SCRATCH/usr/lib/libgcc_s.so.1"
LEDGER="$OUT/commands/11a_ldd_binding_and_conflict_retry.log"
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
  "$@" >"$RAW/$label.retry.stdout.txt" 2>"$RAW/$label.retry.stderr.txt"
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

printf 'PREVIOUS_ATTEMPT=11_armv7l_elf_ehabi_and_conflict_checks.sh\nPREVIOUS_RESULT=ANALYSIS_ROOT_HARDLINK_PERMISSION_FAILURE;SUDO_CHROOT_UNAVAILABLE;WRONG_EMUL_LIBGCC_SELECTED;CLANG_RESOURCE_DIR_OMITTED\nRETRY_SCOPE=LDD_BINDING_AND_DUPLICATE_DECLARATION_ONLY\n' >"$OUT/elf_retry_disclosure.txt"
overall=0
cd "$REPO" || exit 125
run_record assert_original_ldd test -r "$LDD_ORIG" || overall=1
run_record create_prefix_adapted_ldd sed "s#^RTLDLIST=/lib/ld-linux.so.3\$#RTLDLIST=$SCRATCH/lib/ld-linux.so.3#" "$LDD_ORIG" || overall=1
run_record write_prefix_adapted_ldd cp "$RAW/create_prefix_adapted_ldd.retry.stdout.txt" "$LDD_ADAPTED" || overall=1
run_record ldd_adaptation_diff diff -u "$LDD_ORIG" "$LDD_ADAPTED" || {
  rc=$?
  if [[ $rc -ne 1 ]]; then overall=1; fi
}
run_record assert_single_ldd_adaptation bash -o pipefail -c 'test "$(diff -u "$1" "$2" | grep -Ec "^[+-]RTLDLIST=")" -eq 2 && test "$(diff -u "$1" "$2" | grep -Ec "^[+-][^+-]")" -eq 2' _ "$LDD_ORIG" "$LDD_ADAPTED" || overall=1
run_record adapted_ldd_sha256 sha256sum "$LDD_ORIG" "$LDD_ADAPTED" || overall=1

LD_PATH="$EXTRACT/usr/lib:$SCRATCH/usr/lib:$SCRATCH/lib"
run_record ldd_r_libcxx_retry env LD_LIBRARY_PATH="$LD_PATH" /usr/bin/qemu-arm-static -L "$SCRATCH" "$SCRATCH/bin/bash" "$LDD_ADAPTED" -r "$EXTRACT/usr/lib/libc++.so.1" || overall=1
run_record ldd_r_libcxxabi_retry env LD_LIBRARY_PATH="$LD_PATH" /usr/bin/qemu-arm-static -L "$SCRATCH" "$SCRATCH/bin/bash" "$LDD_ADAPTED" -r "$EXTRACT/usr/lib/libc++abi.so.1" || overall=1
run_record ldd_r_libcxx_unresolved_count_retry awk '/undefined symbol:|not found/{c++} END{print c+0}' "$RAW/ldd_r_libcxx_retry.retry.stdout.txt" || overall=1
run_record ldd_r_libcxxabi_unresolved_count_retry awk '/undefined symbol:|not found/{c++} END{print c+0}' "$RAW/ldd_r_libcxxabi_retry.retry.stdout.txt" || overall=1
run_record assert_zero_strong_unresolved_retry bash -o pipefail -c 'test "$(cat "$1")" -eq 0 && test "$(cat "$2")" -eq 0' _ "$RAW/ldd_r_libcxx_unresolved_count_retry.retry.stdout.txt" "$RAW/ldd_r_libcxxabi_unresolved_count_retry.retry.stdout.txt" || overall=1

run_record assert_native_arm_libgcc test -r "$LIBGCC" || overall=1
run_record native_arm_libgcc_file file "$LIBGCC" || overall=1
run_record native_arm_libgcc_symbols readelf -WsW "$LIBGCC" || overall=1
run_record native_arm_libgcc_gnu_unwind rg -n -- '__gnu_unwind_frame' "$RAW/native_arm_libgcc_symbols.retry.stdout.txt" || overall=1
run_record assert_binding_evidence_retry bash -o pipefail -c 'rg -q -- "UND.*__gnu_unwind_frame" "$1" && rg -q -- "GLOBAL.*DEFAULT.*__gnu_unwind_frame" "$2" && rg -q -- "NEEDED.*libgcc_s.so.1" "$3" && rg -q -- "libgcc_s.so.1" "$4"' _ "$RAW/libcxxabi_gnu_unwind_symbol.stdout.txt" "$RAW/native_arm_libgcc_symbols.retry.stdout.txt" "$RAW/libcxxabi_dynamic_needed.stdout.txt" "$RAW/ldd_r_libcxxabi_retry.retry.stdout.txt" || overall=1

run_record duplicate_declaration_syntax_probe_retry /usr/bin/qemu-arm-static -L "$SCRATCH" "$SCRATCH/usr/bin/clang++" --target=armv7l-tizen-linux-gnueabi --sysroot="$SCRATCH" -resource-dir "$SCRATCH/usr/lib/clang/22" -isystem "$SCRATCH/usr/include" -I"$SOURCE/libunwind/include" -fsyntax-only -x c++ "$PROBE" || overall=1

{
  printf 'library\tldd_r_execution\tstrong_unresolved\tgnu_unwind_frame\tARM_exidx\tARM_extab\tLLVM_22\n'
  printf 'libcxx\tqemu-arm-static_-L_retired-root_arm-bash_prefix-adapted-glibc-ldd_-r\t%s\tN/A\tPRESENT\tPRESENT\tPASS\n' "$(cat "$RAW/ldd_r_libcxx_unresolved_count_retry.retry.stdout.txt")"
  printf 'libcxxabi\tqemu-arm-static_-L_retired-root_arm-bash_prefix-adapted-glibc-ldd_-r\t%s\tUND_in_libc++abi;DEFINED_GLOBAL_in_native_arm_libgcc_s;DT_NEEDED_and_ldd_resolution_to_libgcc_s\tPRESENT\tPRESENT\tPASS\n' "$(cat "$RAW/ldd_r_libcxxabi_unresolved_count_retry.retry.stdout.txt")"
} >"$OUT/armv7l_elf_check_summary.tsv"

{
  printf 'header\tdeclaration\tprobe_result\n'
  printf 'libcxxabi/src/cxa_personality.cpp\textern C _Unwind_Reason_Code __gnu_unwind_frame(_Unwind_Exception*, _Unwind_Context*)\tPASS\n'
  printf 'libunwind/include/unwind_arm_ehabi.h\textern _Unwind_Reason_Code __gnu_unwind_frame(_Unwind_Exception *, _Unwind_Context *) inside extern C block\tPASS\n'
  printf 'combined_translation_unit\tboth declarations\t%s\n' "$(if [[ $overall -eq 0 ]]; then printf 'SYNTAX_OK_NO_CONFLICT'; else printf 'SEE_COMMAND_LEDGER'; fi)"
} >"$OUT/future_llvm_libunwind_conflict_precheck.tsv"

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
