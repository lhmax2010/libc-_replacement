#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/T4"
RAW="$OUT/raw"
TMP="$REPO/tmp/T4/static"
TOOL="$REPO/gates/tools/run_static_gates.sh"
TOOL_SHA=d24f7c234d266bf970dbfd9c24939052681eba2105c186d0002009891c119885
LEDGER="$OUT/commands/02_run_frozen_static_gates.log"

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
record_to static_tmp_fresh "$RAW/static_tmp_fresh.stdout.txt" test ! -e "$TMP" || overall=1
record_to frozen_tool_identity "$RAW/frozen_tool_identity.stdout.txt" sha256sum "$TOOL" || overall=1
record_to assert_frozen_tool_identity "$RAW/assert_frozen_tool_identity.stdout.txt" bash -o pipefail -c \
  'test "$(sha256sum "$1" | cut -d" " -f1)" = "$2" && test "$(wc -l < "$3")" -eq 145 && (cd "$4/gates" && sha256sum -c FREEZE_MANIFEST.sha256)' \
  _ "$TOOL" "$TOOL_SHA" "$REPO/gates/FREEZE_MANIFEST.sha256" "$REPO" || overall=1
if [[ $overall -ne 0 ]]; then
  printf 'STATIC_GATE_PREFLIGHT=FAIL\n' >"$OUT/static_gate.status"
  printf 'OVERALL_EXIT_CODE=1\n' >>"$LEDGER"
  exit 1
fi
record_to static_tmp_create "$RAW/static_tmp_create.stdout.txt" mkdir -p "$TMP" || overall=1

printf 'architecture\toriginal_filename\tadapter_filename\tsha256\tcontent_identity\n' >"$OUT/static_tool_adapter.tsv"
printf 'architecture\tfrozen_tool\ttool_exit\tP02_RPM_SET\tP03_ELF_MACHINE\tP04_DWARF_PRODUCER\tP05_VERSION_LIBCXX\tP06_VERSION_LIBCXXABI\tP07_DIRECT_NEEDED\tP08_BIND_NOW\tP14_HEADER_LAYOUT\toverall\n' >"$OUT/static_gate_results.tsv"

for arch in armv7l aarch64 x86_64; do
  arch_fail=0
  carrier="$TMP/carrier/$arch/local/repos/tizen_unified_standard/$arch"
  static_out="$OUT/$arch/static"
  dwarf_dir="$TMP/dwarf/$arch"
  record_to "${arch}_carrier_create" "$RAW/${arch}_carrier_create.stdout.txt" mkdir -p "$carrier/RPMS" "$carrier/SRPMS" "$dwarf_dir" || arch_fail=1
  while IFS= read -r rpm_file; do
    base=$(basename "$rpm_file")
    alias=${base/22.1.8-1/21.1.1-2}
    if [[ $base == *.src.rpm ]]; then target="$carrier/SRPMS/$alias"; else target="$carrier/RPMS/$alias"; fi
    record_to "${arch}_adapter_${base//+/_}" "$RAW/${arch}_adapter_${base//+/_}.stdout.txt" cp --reflink=auto -p "$rpm_file" "$target" || arch_fail=1
    original_sha=$(sha256sum "$rpm_file" | cut -d' ' -f1)
    adapter_sha=$(sha256sum "$target" | cut -d' ' -f1)
    identity=MATCH
    if [[ $original_sha != "$adapter_sha" ]]; then identity=MISMATCH; arch_fail=1; fi
    printf '%s\t%s\t%s\t%s\t%s\n' "$arch" "$base" "$alias" "$original_sha" "$identity" >>"$OUT/static_tool_adapter.tsv"
  done < <(find "$REPO/artifacts/rebuild_2218/$arch" -maxdepth 1 -type f -name '*.rpm' | sort)
  record_to "${arch}_assert_adapter" "$RAW/${arch}_assert_adapter.stdout.txt" bash -o pipefail -c \
    'test "$(find "$1/RPMS" -maxdepth 1 -type f -name "*.rpm" | wc -l)" -eq 7 && test "$(find "$1/SRPMS" -maxdepth 1 -type f -name "*.rpm" | wc -l)" -eq 1' \
    _ "$carrier" || arch_fail=1

  record_to "${arch}_frozen_static_tool" "$RAW/${arch}_frozen_static_tool.stdout.txt" "$TOOL" "$arch" "$TMP/carrier/$arch" "$static_out"
  tool_rc=$?
  if [[ $tool_rc -ne 0 ]]; then arch_fail=1; fi

  p02=PASS; p03=PASS; p04=PASS; p05=PASS; p06=PASS; p07=PASS; p08=PASS; p14=PASS
  record_to "${arch}_assert_rpm_set" "$RAW/${arch}_assert_rpm_set.stdout.txt" bash -o pipefail -c \
    'test "$(tail -n +2 "$1" | wc -l)" -eq 8 && test "$(awk -F"\t" "NR>1 && \$3 !~ /22\\.1\\.8-1/ {n++} END {print n+0}" "$1")" -eq 0' \
    _ "$static_out/rpm_manifest.tsv" || { p02=FAIL; arch_fail=1; }
  expected_machine=ARM
  [[ $arch == aarch64 ]] && expected_machine=AArch64
  [[ $arch == x86_64 ]] && expected_machine='Advanced Micro Devices X86-64'
  record_to "${arch}_assert_elf_machine" "$RAW/${arch}_assert_elf_machine.stdout.txt" bash -o pipefail -c \
    'test "$(tail -n +2 "$1" | wc -l)" -eq 2 && test "$(awk -F"\t" -v e="$2" "NR>1 && \$2!=e {n++} END {print n+0}" "$1")" -eq 0' \
    _ "$static_out/elf_identity.tsv" "$expected_machine" || { p03=FAIL; arch_fail=1; }
  record_to "${arch}_assert_version_libcxx" "$RAW/${arch}_assert_version_libcxx.stdout.txt" bash -o pipefail -c \
    'awk -F"\t" "NR>1 && \$1==\"libc++.so.1.0\" && \$7==\"PASS\" && \$2>0 && \$4==0 && \$5==0 {ok=1} END {exit !ok}" "$1"' \
    _ "$static_out/versioning_gate.tsv" || { p05=FAIL; arch_fail=1; }
  record_to "${arch}_assert_version_libcxxabi" "$RAW/${arch}_assert_version_libcxxabi.stdout.txt" bash -o pipefail -c \
    'awk -F"\t" "NR>1 && \$1==\"libc++abi.so.1.0\" && \$7==\"PASS\" && \$2>0 && \$4==0 && \$5==0 {ok=1} END {exit !ok}" "$1"' \
    _ "$static_out/versioning_gate.tsv" || { p06=FAIL; arch_fail=1; }
  record_to "${arch}_assert_direct_needed" "$RAW/${arch}_assert_direct_needed.stdout.txt" bash -o pipefail -c \
    'rg -q "^libc\\+\\+\\.so\\.1\\.0\\tlibc\\+\\+abi\\.so\\.1$" "$1" && rg -q "^libc\\+\\+\\.so\\.1\\.0\\tlibgcc_s\\.so\\.1$" "$1" && rg -q "^libc\\+\\+abi\\.so\\.1\\.0\\tlibgcc_s\\.so\\.1$" "$1" && ! rg -q "libstdc\\+\\+|libunwind" "$1"' \
    _ "$static_out/direct_needed.tsv" || { p07=FAIL; arch_fail=1; }
  record_to "${arch}_assert_bind_now" "$RAW/${arch}_assert_bind_now.stdout.txt" bash -o pipefail -c \
    'test "$(find "$1" -maxdepth 1 -type f -name "*_dynamic.log" | wc -l)" -eq 2 && for f in "$1"/*_dynamic.log; do rg -q "\\(BIND_NOW\\)|FLAGS_1.*NOW" "$f" || exit 1; done' \
    _ "$static_out/evidence" || { p08=FAIL; arch_fail=1; }

  devel_rpm=$(find "$REPO/artifacts/rebuild_2218/$arch" -maxdepth 1 -type f -name 'libc++-devel-*.rpm' -print -quit)
  record_to "${arch}_header_file_list" "$static_out/evidence/header_file_list.log" rpm -qpl "$devel_rpm" || { p14=FAIL; arch_fail=1; }
  record_to "${arch}_assert_header_layout" "$RAW/${arch}_assert_header_layout.stdout.txt" bash -o pipefail -c \
    'rg -q "^/usr/include/c\\+\\+/v1(/|$)" "$1" && test -d "$2/usr/include/c++/v1" && test -f "$2/usr/include/c++/v1/string"' \
    _ "$static_out/evidence/header_file_list.log" "$static_out/extracted" || { p14=FAIL; arch_fail=1; }

  for pkg in libc++ libc++abi; do
    debug_rpm=$(find "$REPO/artifacts/rebuild_2218/$arch" -maxdepth 1 -type f -name "$pkg-debuginfo-*.rpm" -print -quit)
    record_to "${arch}_${pkg//+/_}_debug_extract" "$RAW/${arch}_${pkg//+/_}_debug_extract.stdout.txt" bash -o pipefail -c \
      'cd "$1" && rpm2cpio "$2" | cpio -idmuv' _ "$dwarf_dir" "$debug_rpm" || { p04=FAIL; arch_fail=1; }
  done
  mapfile -t debug_files < <(find "$dwarf_dir" -type f -name '*.debug' | sort)
  if [[ ${#debug_files[@]} -ne 2 ]]; then
    p04=FAIL; arch_fail=1
  else
    for debug_file in "${debug_files[@]}"; do
      stem=$(basename "$debug_file" .debug)
      record_to "${arch}_${stem//+/_}_dwarf_producer" "$static_out/evidence/${stem//+/_}_dwarf_producer.log" \
        readelf --debug-dump=info --dwarf-depth=1 "$debug_file" || { p04=FAIL; arch_fail=1; }
      record_to "${arch}_${stem//+/_}_assert_dwarf_producer" "$RAW/${arch}_${stem//+/_}_assert_dwarf_producer.stdout.txt" bash -o pipefail -c \
        'test "$(rg -c "DW_AT_producer" "$1")" -gt 0 && test "$(rg "DW_AT_producer" "$1" | rg -vc "clang version 22\\.1\\.8" || true)" -eq 0' \
        _ "$static_out/evidence/${stem//+/_}_dwarf_producer.log" || { p04=FAIL; arch_fail=1; }
    done
  fi

  arch_overall=PASS
  [[ $arch_fail -ne 0 ]] && arch_overall=RED
  printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' \
    "$arch" "$TOOL_SHA" "$tool_rc" "$p02" "$p03" "$p04" "$p05" "$p06" "$p07" "$p08" "$p14" "$arch_overall" \
    >>"$OUT/static_gate_results.tsv"
  [[ $arch_fail -ne 0 ]] && overall=1
done

if [[ $overall -eq 0 ]]; then
  printf 'STATIC_GATES=PASS\n' >"$OUT/static_gate.status"
else
  printf 'STATIC_GATES=RED\n' >"$OUT/static_gate.status"
fi
printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
