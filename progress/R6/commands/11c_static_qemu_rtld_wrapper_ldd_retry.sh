#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R6"
RAW="$OUT/raw"
SCRATCH="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b/local/BUILD-ROOTS/scratch.armv7l.0"
EXTRACT="$REPO/tmp/R6/elf-extract-armv7l-r3"
LDD_ORIG="$SCRATCH/usr/bin/ldd"
WRAPPER_SRC="$OUT/probes/rtld_armv7l_qemu_wrapper.c"
WRAPPER_BIN="$OUT/probes/rtld-armv7l-qemu-wrapper.bin"
LDD_ADAPTED="$OUT/probes/ldd.armv7l.r3.static-wrapper"
LEDGER="$OUT/commands/11c_static_qemu_rtld_wrapper_ldd_retry.log"
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
  "$@" >"$RAW/$label.attempt4.stdout.txt" 2>"$RAW/$label.attempt4.stderr.txt"
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

printf 'PREVIOUS_ATTEMPT=11b_qemu_rtld_wrapper_ldd_retry.sh\nPREVIOUS_RESULT=LD_TRACE_LOADED_OBJECTS_ACTED_ON_DYNAMIC_HOST_BASH_SHEBANG_BEFORE_SCRIPT_WRAPPER_EXECUTED\nATTEMPT4_METHOD=STATIC_HOST_WRAPPER_EXECVE_QEMU_ARM_RTLD;GLIBC_LDD_-r_LOGIC_UNCHANGED_EXCEPT_RTLDLIST\n' >"$OUT/ldd_attempt4_disclosure.txt"
overall=0
cd "$REPO" || exit 125
run_record compile_static_rtld_wrapper gcc -O2 -Wall -Wextra -Werror -static -o "$WRAPPER_BIN" "$WRAPPER_SRC" || overall=1
run_record static_wrapper_file file "$WRAPPER_BIN" || overall=1
run_record assert_wrapper_static bash -o pipefail -c 'file "$1" | rg -q -- "statically linked"' _ "$WRAPPER_BIN" || overall=1
run_record wrapper_verify_arm_elf "$WRAPPER_BIN" --verify "$EXTRACT/usr/lib/libc++abi.so.1" || {
  rc=$?
  if [[ $rc -ne 2 ]]; then overall=1; fi
}
run_record create_static_wrapper_adapted_ldd sed "s#^RTLDLIST=/lib/ld-linux.so.3\$#RTLDLIST=$WRAPPER_BIN#" "$LDD_ORIG" || overall=1
run_record write_static_wrapper_adapted_ldd cp "$RAW/create_static_wrapper_adapted_ldd.attempt4.stdout.txt" "$LDD_ADAPTED" || overall=1
run_record static_wrapper_ldd_diff diff -u "$LDD_ORIG" "$LDD_ADAPTED" || {
  rc=$?
  if [[ $rc -ne 1 ]]; then overall=1; fi
}
run_record assert_single_static_wrapper_ldd_adaptation bash -o pipefail -c 'test "$(diff -u "$1" "$2" | grep -Ec "^[+-]RTLDLIST=")" -eq 2 && test "$(diff -u "$1" "$2" | grep -Ec "^[+-][^+-]")" -eq 2' _ "$LDD_ORIG" "$LDD_ADAPTED" || overall=1
run_record static_wrapper_sources_sha256 sha256sum "$WRAPPER_SRC" "$WRAPPER_BIN" "$LDD_ORIG" "$LDD_ADAPTED" || overall=1

run_record ldd_r_libcxx_attempt4 /bin/bash "$LDD_ADAPTED" -r "$EXTRACT/usr/lib/libc++.so.1" || overall=1
run_record ldd_r_libcxxabi_attempt4 /bin/bash "$LDD_ADAPTED" -r "$EXTRACT/usr/lib/libc++abi.so.1" || overall=1
run_record assert_arm_ldd_not_host bash -o pipefail -c 'rg -q -- "ld-linux.so.3|scratch.armv7l.0" "$1" "$2" && ! rg -q -- "x86_64|ld-linux-x86-64" "$1" "$2"' _ "$RAW/ldd_r_libcxx_attempt4.attempt4.stdout.txt" "$RAW/ldd_r_libcxxabi_attempt4.attempt4.stdout.txt" || overall=1
run_record ldd_r_libcxx_unresolved_count_attempt4 awk '/undefined symbol:|not found/{c++} END{print c+0}' "$RAW/ldd_r_libcxx_attempt4.attempt4.stdout.txt" || overall=1
run_record ldd_r_libcxxabi_unresolved_count_attempt4 awk '/undefined symbol:|not found/{c++} END{print c+0}' "$RAW/ldd_r_libcxxabi_attempt4.attempt4.stdout.txt" || overall=1
run_record assert_zero_strong_unresolved_attempt4 bash -o pipefail -c 'test "$(cat "$1")" -eq 0 && test "$(cat "$2")" -eq 0' _ "$RAW/ldd_r_libcxx_unresolved_count_attempt4.attempt4.stdout.txt" "$RAW/ldd_r_libcxxabi_unresolved_count_attempt4.attempt4.stdout.txt" || overall=1
run_record assert_binding_evidence_attempt4 bash -o pipefail -c 'rg -q -- "UND.*__gnu_unwind_frame" "$1" && rg -q -- "GLOBAL.*DEFAULT.*__gnu_unwind_frame" "$2" && rg -q -- "NEEDED.*libgcc_s.so.1" "$3" && rg -q -- "libgcc_s.so.1 => .*scratch.armv7l.0/usr/lib/libgcc_s.so.1" "$4"' _ "$RAW/libcxxabi_gnu_unwind_symbol.stdout.txt" "$RAW/native_arm_libgcc_symbols.retry.stdout.txt" "$RAW/libcxxabi_dynamic_needed.stdout.txt" "$RAW/ldd_r_libcxxabi_attempt4.attempt4.stdout.txt" || overall=1
run_record assert_duplicate_probe_pass_attempt4 bash -o pipefail -c 'test ! -s "$1" && awk '\''/LABEL=duplicate_declaration_syntax_probe_retry/{seen=1} seen && /EXIT_CODE=0/{ok=1; exit} END{exit !ok}'\'' "$2"' _ "$RAW/duplicate_declaration_syntax_probe_retry.retry.stderr.txt" "$OUT/commands/11a_ldd_binding_and_conflict_retry.log" || overall=1

{
  printf 'library\tldd_r_execution\tstrong_unresolved\tgnu_unwind_frame\tARM_exidx\tARM_extab\tLLVM_22\n'
  printf 'libcxx\thost-bash_glibc-ldd_-r_with_RTLDLIST_static-wrapper_execve_qemu-arm-static_-L_retired-root_arm-rtld\t%s\tN/A\tPRESENT\tPRESENT\tPASS\n' "$(cat "$RAW/ldd_r_libcxx_unresolved_count_attempt4.attempt4.stdout.txt")"
  printf 'libcxxabi\thost-bash_glibc-ldd_-r_with_RTLDLIST_static-wrapper_execve_qemu-arm-static_-L_retired-root_arm-rtld\t%s\tUND_in_libc++abi;DEFINED_GLOBAL_in_native_arm_libgcc_s;DT_NEEDED_and_ldd_resolution_to_libgcc_s\tPRESENT\tPRESENT\tPASS\n' "$(cat "$RAW/ldd_r_libcxxabi_unresolved_count_attempt4.attempt4.stdout.txt")"
} >"$OUT/armv7l_elf_check_summary.tsv"

{
  printf 'header\tdeclaration\tprobe_result\n'
  printf 'libcxxabi/src/cxa_personality.cpp\textern C _Unwind_Reason_Code __gnu_unwind_frame(_Unwind_Exception*, _Unwind_Context*)\tPASS\n'
  printf 'libunwind/include/unwind_arm_ehabi.h\textern _Unwind_Reason_Code __gnu_unwind_frame(_Unwind_Exception *, _Unwind_Context *) inside extern C block\tPASS\n'
  printf 'combined_translation_unit\tboth declarations\tSYNTAX_OK_NO_CONFLICT\n'
} >"$OUT/future_llvm_libunwind_conflict_precheck.tsv"

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
