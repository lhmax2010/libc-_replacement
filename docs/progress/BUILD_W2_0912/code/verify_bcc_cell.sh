#!/bin/bash
set -euo pipefail

if [ "$#" -ne 6 ]; then
  echo "usage: $0 ARCH MODE BUILD_ROOT RELEASE BUILD_LOG LLVM_RPM_OR_NONE" >&2
  exit 2
fi

arch=$1
mode=$2
build_root=$3
release=$4
build_log=$5
llvm_rpm=$6
workspace=/home/toolchain/development/libc++_replacement
out="$workspace/docs/progress/BUILD_W2_0912/checks/bcc_${arch}_${mode}"
repo=$(find "$build_root/local/repos" -mindepth 3 -maxdepth 3 -type d -path "*/$arch/RPMS" -print -quit)

test -f "$build_log"
test -n "$repo"
mkdir -p "$out"
extract=$(mktemp -d "$workspace/tmp/BUILD_W2_0912/bcc_extract_${arch}_${mode}.XXXXXX")
llvm_extract=
cleanup() {
  rm -rf "$extract"
  if [ -n "$llvm_extract" ]; then rm -rf "$llvm_extract"; fi
}
trap cleanup EXIT

mapfile -t rpms < <(find "$repo" -maxdepth 1 -type f -name "bcc-tools*-${release}.${arch}.rpm" ! -name '*-debuginfo-*' ! -name '*-debugsource-*' | sort)
test "${#rpms[@]}" -gt 0
printf 'arch=%s\nmode=%s\nrpm_count=%s\n' "$arch" "$mode" "${#rpms[@]}" | tee "$out/summary.txt"
for rpm_file in "${rpms[@]}"; do
  sha256sum "$rpm_file" | tee -a "$out/rpm_sha256.txt"
  rpm2cpio "$rpm_file" | (cd "$extract" && cpio -idm --quiet)
done

if [ "$mode" = libcxx ]; then
  rg -n -- '-stdlib=libc\+\+' "$build_log" >"$out/libcxx_flag_lines.txt"
  rg -n -- '-lc\+\+abi' "$build_log" >"$out/libcxxabi_flag_lines.txt"
  rg -n 'clang version 22\.1\.8|Clang 22\.1\.8|-[^ ]*-clang\+\+' "$build_log" >"$out/compiler_lines.txt"
else
  if rg -n -- '-stdlib=libc\+\+|-lc\+\+abi' "$build_log" >"$out/forbidden_libcxx_lines.txt"; then
    echo 'forbidden libc++ option present' >&2
    exit 1
  fi
  rg -n 'gcc version 14\.2\.0|GNU 14\.2\.0|-[^ ]*-g\+\+' "$build_log" >"$out/compiler_lines.txt"
fi

find "$extract" -type f -print0 | while IFS= read -r -d '' file_path; do
  file -b "$file_path" | grep -q '^ELF' || continue
  rel=${file_path#"$extract"/}
  printf 'ELF\t%s\n' "$rel"
  readelf -d "$file_path" 2>/dev/null | grep NEEDED | sed "s#^#NEEDED\t$rel\t#"
  nm -D --undefined-only "$file_path" 2>/dev/null | c++filt | grep -E 'std::__(1|cxx11)' | sed "s#^#STD_SYMBOL\t$rel\t#" || true
done >"$out/elf_evidence.txt"

libcxx_count=$(grep -c 'libc++\.so\.1' "$out/elf_evidence.txt" || true)
libstdcxx_count=$(grep -c 'libstdc++\.so\.6' "$out/elf_evidence.txt" || true)
std1_count=$(grep -c 'std::__1' "$out/elf_evidence.txt" || true)
stdcxx11_count=$(grep -c 'std::__cxx11' "$out/elf_evidence.txt" || true)
printf 'libcxx_needed=%s\nlibstdcxx_needed=%s\nstd___1=%s\nstd___cxx11=%s\n' \
  "$libcxx_count" "$libstdcxx_count" "$std1_count" "$stdcxx11_count" | tee -a "$out/summary.txt"

if [ "$mode" = libcxx ]; then
  test "$libcxx_count" -gt 0
  test "$libstdcxx_count" -eq 0
  test "$std1_count" -gt 0
  test "$llvm_rpm" != NONE
  test -f "$llvm_rpm"
  llvm_extract=$(mktemp -d "$workspace/tmp/BUILD_W2_0912/llvm_extract_${arch}.XXXXXX")
  rpm2cpio "$llvm_rpm" | (cd "$llvm_extract" && cpio -idm --quiet)
  libllvm=$(find "$llvm_extract" -type f -name 'libLLVM.so.22.1' -print -quit)
  test -n "$libllvm"
  readelf -d "$libllvm" | grep NEEDED >"$out/llvm_needed.txt"
  grep -q 'libc++\.so\.1' "$out/llvm_needed.txt"
  if grep -q 'libstdc++\.so\.6' "$out/llvm_needed.txt"; then exit 1; fi
else
  test "$libstdcxx_count" -gt 0
  test "$libcxx_count" -eq 0
fi

echo PASS | tee -a "$out/summary.txt"
