#!/bin/bash
set -euo pipefail

if [ "$#" -ne 6 ]; then
  echo "usage: $0 ARCH MODE BUILD_ROOT RELEASE BUILD_LOG OUTPUT_DIR" >&2
  exit 2
fi

arch=$1
mode=$2
build_root=$3
release=$4
build_log=$5
output_dir=$6
workspace=/home/toolchain/development/libc++_replacement
repo_root="$build_root/local/repos/tizen_unified_standard/$arch"
scratch="$build_root/local/BUILD-ROOTS/scratch.$arch.0"

test -f "$build_log"
test ! -e "$output_dir"
mkdir -p "$output_dir/payload" "$output_dir/symbols"

mapfile -t rpm_names < <(sed -n \
  "s#^.*Wrote: /home/abuild/rpmbuild/RPMS/[^/]*/##p" "$build_log" | \
  sed -n "/-${release}\.${arch}\.rpm$/p" | \
  sed '/-debuginfo-/d; /-debugsource-/d' | sort -u)
test "${#rpm_names[@]}" -gt 0
printf 'arch=%s\nmode=%s\nrpm_count=%s\n' "$arch" "$mode" "${#rpm_names[@]}"
for rpm_name in "${rpm_names[@]}"; do
  rpm_path="$repo_root/RPMS/$rpm_name"
  test -f "$rpm_path"
  echo "RPM $rpm_path"
  rpm2cpio "$rpm_path" | (cd "$output_dir/payload" && cpio -idm --quiet)
done

compiler_lines=$(rg -c 'clang version 22\.1\.8|Clang 22\.1\.8|clang\+\+' "$build_log" || true)
libcxx_flag_lines=$(rg -c -- '-stdlib=libc\+\+' "$build_log" || true)
libcxxabi_flag_lines=$(rg -c -- '-lc\+\+abi' "$build_log" || true)
compiler_lines=${compiler_lines:-0}
libcxx_flag_lines=${libcxx_flag_lines:-0}
libcxxabi_flag_lines=${libcxxabi_flag_lines:-0}
printf 'clang_evidence_lines=%s\nstdlib_libcxx_flag_lines=%s\nlibcxxabi_flag_lines=%s\n' \
  "$compiler_lines" "$libcxx_flag_lines" "$libcxxabi_flag_lines"
test "$compiler_lines" -gt 0
if [ "$mode" = libcxx ]; then
  test "$libcxx_flag_lines" -gt 0
  test "$libcxxabi_flag_lines" -gt 0
else
  test "$libcxx_flag_lines" -eq 0
  test "$libcxxabi_flag_lines" -eq 0
fi

mapfile -d '' archives < <(find "$output_dir/payload" -type f -name '*.a' -print0 | sort -z)
test "${#archives[@]}" -gt 0
printf 'archive_count=%s\n' "${#archives[@]}"
for archive in "${archives[@]}"; do
  relative=${archive#"$output_dir/payload/"}
  symbol_file="$output_dir/symbols/$(basename "$archive").nm"
  nm -A -P -g --defined-only "$archive" >"$symbol_file"
  member_count=$(ar t "$archive" | wc -l)
  strong_count=$(awk '$3 ~ /^[TDRBSG]$/ {n++} END {print n+0}' "$symbol_file")
  libcxx_names=$(rg -c 'NSt3__1|St3__1' "$symbol_file" || true)
  libstdcxx_names=$(rg -c 'NSt7__cxx11|St7__cxx11' "$symbol_file" || true)
  libcxx_names=${libcxx_names:-0}
  libstdcxx_names=${libstdcxx_names:-0}
  printf 'ARCHIVE %s members=%s strong=%s std___1=%s std___cxx11=%s\n' \
    "$relative" "$member_count" "$strong_count" "$libcxx_names" "$libstdcxx_names"
  rg -m 5 'NSt3__1|St3__1|NSt7__cxx11|St7__cxx11' "$symbol_file" || true
done

elf_count=0
libcxx_needed=0
libstdcxx_needed=0
while IFS= read -r -d '' candidate; do
  if ! file -b "$candidate" | rg -q '^ELF'; then
    continue
  fi
  elf_count=$((elf_count + 1))
  needed=$(readelf -d "$candidate" 2>/dev/null | rg 'NEEDED' || true)
  if printf '%s\n' "$needed" | rg -q 'libc\+\+\.so\.1'; then
    libcxx_needed=$((libcxx_needed + 1))
    echo "LIBCXX_NEEDED ${candidate#$output_dir/payload/}"
  fi
  if printf '%s\n' "$needed" | rg -q 'libstdc\+\+\.so\.6'; then
    libstdcxx_needed=$((libstdcxx_needed + 1))
    echo "LIBSTDCXX_NEEDED ${candidate#$output_dir/payload/}"
  fi
done < <(find "$output_dir/payload" -type f -print0)
printf 'elf_count=%s\nlibcxx_needed=%s\nlibstdcxx_needed=%s\n' \
  "$elf_count" "$libcxx_needed" "$libstdcxx_needed"
test "$elf_count" -gt 0
if [ "$mode" = libcxx ]; then
  test "$libcxx_needed" -gt 0
  test "$libstdcxx_needed" -eq 0
else
  test "$libcxx_needed" -eq 0
fi

if [ "$mode" = libcxx ]; then
  headers=(
    usr/include/c++/v1/__ostream/basic_ostream.h
    usr/include/c++/v1/future
    usr/include/c++/v1/istream
    usr/include/c++/v1/string
    usr/include/c++/v1/cxxabi.h
  )
  for header in "${headers[@]}"; do
    test -f "$scratch/$header"
    if [ "$header" = usr/include/c++/v1/cxxabi.h ]; then
      rg -n 'class .*__forced_unwind' "$scratch/$header"
    else
      rg -n '__forced_unwind' "$scratch/$header"
    fi
  done
fi

echo PASS
