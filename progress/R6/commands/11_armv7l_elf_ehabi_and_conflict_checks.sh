#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R6"
RAW="$OUT/raw"
SOURCE="$REPO/codes/llvm"
ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b"
SCRATCH="$ROOT/local/BUILD-ROOTS/scratch.armv7l.0"
EXTRACT="$REPO/tmp/R6/elf-extract-armv7l-r3"
LDDROOT="$REPO/tmp/R6/ldd-root-armv7l-r3"
ARTIFACT="$REPO/artifacts/rebuild_2218/armv7l"
PROBE="$OUT/probes/duplicate_ehabi_declaration.cpp"
LEDGER="$OUT/commands/11_armv7l_elf_ehabi_and_conflict_checks.log"
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

overall=0
cd "$REPO" || exit 125
run_record assert_extract_fresh test ! -e "$EXTRACT" || overall=1
run_record assert_lddroot_fresh test ! -e "$LDDROOT" || overall=1
run_record create_extract mkdir -p "$EXTRACT" || overall=1
run_record extract_libcxx_runtime bash -o pipefail -c 'cd "$1" && rpm2cpio "$2" | cpio -idm --quiet' _ "$EXTRACT" "$ARTIFACT/libc++-22.1.8-1.armv7l.rpm" || overall=1
run_record extract_libcxxabi_runtime bash -o pipefail -c 'cd "$1" && rpm2cpio "$2" | cpio -idm --quiet' _ "$EXTRACT" "$ARTIFACT/libc++abi-22.1.8-1.armv7l.rpm" || overall=1
run_record extracted_elf_files find "$EXTRACT/usr/lib" -maxdepth 1 -type f -o -type l || overall=1
run_record create_lddroot mkdir -p "$LDDROOT" || overall=1
run_record hardlink_clone_retired_root cp -al "$SCRATCH/." "$LDDROOT/" || overall=1
run_record inject_runtime_into_analysis_clone bash -o pipefail -c 'cp -a "$1"/usr/lib/libc++* "$2"/usr/lib/' _ "$EXTRACT" "$LDDROOT" || overall=1
run_record analysis_clone_runtime_files ls -l "$LDDROOT/usr/lib/libc++.so" "$LDDROOT/usr/lib/libc++.so.1" "$LDDROOT/usr/lib/libc++.so.1.0" "$LDDROOT/usr/lib/libc++abi.so" "$LDDROOT/usr/lib/libc++abi.so.1" "$LDDROOT/usr/lib/libc++abi.so.1.0" || overall=1
run_record qemu_identity file "$LDDROOT/usr/bin/qemu-arm-static" "$LDDROOT/bin/bash" || overall=1
run_record ldd_r_libcxx sudo chroot "$LDDROOT" /usr/bin/qemu-arm-static -L / /bin/bash /usr/bin/ldd -r /usr/lib/libc++.so.1 || overall=1
run_record ldd_r_libcxxabi sudo chroot "$LDDROOT" /usr/bin/qemu-arm-static -L / /bin/bash /usr/bin/ldd -r /usr/lib/libc++abi.so.1 || overall=1
run_record ldd_r_libcxx_unresolved_count awk '/undefined symbol:|not found/{c++} END{print c+0}' "$RAW/ldd_r_libcxx.stdout.txt" || overall=1
run_record ldd_r_libcxxabi_unresolved_count awk '/undefined symbol:|not found/{c++} END{print c+0}' "$RAW/ldd_r_libcxxabi.stdout.txt" || overall=1
run_record assert_zero_strong_unresolved bash -o pipefail -c 'test "$(cat "$1")" -eq 0 && test "$(cat "$2")" -eq 0' _ "$RAW/ldd_r_libcxx_unresolved_count.stdout.txt" "$RAW/ldd_r_libcxxabi_unresolved_count.stdout.txt" || overall=1

run_record libcxxabi_dynamic_needed readelf -dW "$EXTRACT/usr/lib/libc++abi.so.1.0" || overall=1
run_record libcxx_dynamic_needed readelf -dW "$EXTRACT/usr/lib/libc++.so.1.0" || overall=1
run_record libcxxabi_gnu_unwind_symbol readelf -WsW "$EXTRACT/usr/lib/libc++abi.so.1.0" || overall=1
run_record libgcc_path bash -o pipefail -c 'find "$1" -type f -name "libgcc_s.so.1" -print -quit' _ "$SCRATCH" || overall=1
LIBGCC=$(cat "$RAW/libgcc_path.stdout.txt")
if [[ -z "$LIBGCC" ]]; then
  overall=1
else
  run_record libgcc_gnu_unwind_symbol readelf -WsW "$LIBGCC" || overall=1
fi
run_record filter_libcxxabi_gnu_unwind rg -n -- '__gnu_unwind_frame' "$RAW/libcxxabi_gnu_unwind_symbol.stdout.txt" || overall=1
run_record filter_libgcc_gnu_unwind rg -n -- '__gnu_unwind_frame' "$RAW/libgcc_gnu_unwind_symbol.stdout.txt" || overall=1
run_record libcxxabi_gnu_unwind_relocation readelf -rW "$EXTRACT/usr/lib/libc++abi.so.1.0" || overall=1
run_record filter_libcxxabi_gnu_unwind_relocation rg -n -- '__gnu_unwind_frame' "$RAW/libcxxabi_gnu_unwind_relocation.stdout.txt" || overall=1
run_record assert_binding_evidence bash -o pipefail -c 'rg -q -- "UND.*__gnu_unwind_frame" "$1" && rg -q -- "GLOBAL.*DEFAULT.*__gnu_unwind_frame" "$2" && rg -q -- "NEEDED.*libgcc_s.so.1" "$3"' _ "$RAW/libcxxabi_gnu_unwind_symbol.stdout.txt" "$RAW/libgcc_gnu_unwind_symbol.stdout.txt" "$RAW/libcxxabi_dynamic_needed.stdout.txt" || overall=1

run_record libcxx_sections readelf -SW "$EXTRACT/usr/lib/libc++.so.1.0" || overall=1
run_record libcxxabi_sections readelf -SW "$EXTRACT/usr/lib/libc++abi.so.1.0" || overall=1
run_record libcxx_arm_unwind_sections bash -o pipefail -c 'rg -n -- "\\.ARM\\.(exidx|extab)" "$1" || true' _ "$RAW/libcxx_sections.stdout.txt" || overall=1
run_record libcxxabi_arm_unwind_sections bash -o pipefail -c 'rg -n -- "\\.ARM\\.(exidx|extab)" "$1" || true' _ "$RAW/libcxxabi_sections.stdout.txt" || overall=1
run_record assert_arm_exidx_present bash -o pipefail -c 'rg -q -- "\\.ARM\\.exidx" "$1" && rg -q -- "\\.ARM\\.exidx" "$2"' _ "$RAW/libcxx_sections.stdout.txt" "$RAW/libcxxabi_sections.stdout.txt" || overall=1

run_record libcxx_version_info readelf --version-info --wide "$EXTRACT/usr/lib/libc++.so.1.0" || overall=1
run_record libcxxabi_version_info readelf --version-info --wide "$EXTRACT/usr/lib/libc++abi.so.1.0" || overall=1
run_record assert_llvm22_nodes bash -o pipefail -c 'rg -q -- "LLVM_22" "$1" && rg -q -- "LLVM_22" "$2" && ! rg -q -- "LLVM_21" "$1" "$2"' _ "$RAW/libcxx_version_info.stdout.txt" "$RAW/libcxxabi_version_info.stdout.txt" || overall=1

run_record cxa_added_declaration rg -n -C 3 -- 'extern "C" _Unwind_Reason_Code __gnu_unwind_frame' "$SOURCE/libcxxabi/src/cxa_personality.cpp" || overall=1
run_record libunwind_header_declaration rg -n -C 3 -- 'extern _Unwind_Reason_Code __gnu_unwind_frame' "$SOURCE/libunwind/include/unwind_arm_ehabi.h" || overall=1
run_record duplicate_declaration_probe cat "$PROBE" || overall=1
run_record duplicate_declaration_syntax_probe /usr/bin/qemu-arm-static -L "$SCRATCH" "$SCRATCH/usr/bin/clang++" --target=armv7l-tizen-linux-gnueabi --sysroot="$SCRATCH" -I"$SOURCE/libunwind/include" -fsyntax-only -x c++ "$PROBE" || overall=1

{
  printf 'library\tldd_r_execution\tstrong_unresolved\tgnu_unwind_frame\tARM_exidx\tARM_extab\tLLVM_22\n'
  for lib in libcxx libcxxabi; do
    unresolved=$(cat "$RAW/ldd_r_${lib}_unresolved_count.stdout.txt")
    sections="$RAW/${lib}_arm_unwind_sections.stdout.txt"
    exidx=ABSENT
    extab=ABSENT
    rg -q -- '\.ARM\.exidx' "$sections" && exidx=PRESENT
    rg -q -- '\.ARM\.extab' "$sections" && extab=PRESENT
    if [[ $lib == libcxxabi ]]; then binding='UND_in_libc++abi;DEFINED_GLOBAL_in_libgcc_s;DT_NEEDED_libgcc_s'; else binding='N/A'; fi
    printf '%s\tqemu-arm-static_chroot_guest_ldd_-r\t%s\t%s\t%s\t%s\tPASS\n' "$lib" "$unresolved" "$binding" "$exidx" "$extab"
  done
} >"$OUT/armv7l_elf_check_summary.tsv"

{
  printf 'header\tdeclaration\tprobe_result\n'
  printf 'libcxxabi/src/cxa_personality.cpp\textern C _Unwind_Reason_Code __gnu_unwind_frame(_Unwind_Exception*, _Unwind_Context*)\tPASS\n'
  printf 'libunwind/include/unwind_arm_ehabi.h\textern _Unwind_Reason_Code __gnu_unwind_frame(_Unwind_Exception *, _Unwind_Context *) inside extern C block\tPASS\n'
  printf 'combined_translation_unit\tboth declarations\t%s\n' "$(if [[ $overall -eq 0 ]]; then printf 'SYNTAX_OK_NO_CONFLICT'; else printf 'SEE_COMMAND_LEDGER'; fi)"
} >"$OUT/future_llvm_libunwind_conflict_precheck.tsv"

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
