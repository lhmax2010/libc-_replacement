#!/bin/bash
set -euo pipefail

if [ "$#" -lt 5 ] || [ "$#" -gt 6 ]; then
  echo "usage: $0 PACKAGE ARCH MODE BUILD_ROOT RELEASE [BUILD_LOG]" >&2
  exit 2
fi

package=$1
arch=$2
mode=$3
build_root=$4
release=$5
workspace=/home/toolchain/development/libc++_replacement
repo_root="$build_root/local/repos/tizen_unified_standard/$arch"
scratch="$build_root/local/BUILD-ROOTS/scratch.$arch.0"
extract="$workspace/progress/R104/work/verify/${package}_${arch}_${mode}"
build_log=${6:-$scratch/.build.log}

test -f "$build_log"
rm -rf "$extract"
mkdir -p "$extract"

mapfile -t rpm_names < <(sed -n \
  "s#^.*Wrote: /home/abuild/rpmbuild/RPMS/[^/]*/##p" "$build_log" | \
  sed -n "/-${release}\.${arch}\.rpm$/p" | \
  sed '/-debuginfo-/d; /-debugsource-/d' | sort -u)
rpms=()
for rpm_name in "${rpm_names[@]}"; do
  rpm_path="$repo_root/RPMS/$rpm_name"
  test -f "$rpm_path"
  rpms+=("$rpm_path")
done
test "${#rpms[@]}" -gt 0
printf 'package=%s\narch=%s\nmode=%s\nrpm_count=%s\n' \
  "$package" "$arch" "$mode" "${#rpms[@]}"
for rpm_file in "${rpms[@]}"; do
  echo "RPM $rpm_file"
  rpm2cpio "$rpm_file" | (cd "$extract" && cpio -idm --quiet)
done

if [ "$mode" = libcxx ]; then
  compiler_lines=$(rg -c 'clang version 22\.1\.8|Clang 22\.1\.8|clang\+\+|-[^ ]*-clang\+\+' "$build_log" || true)
  libcxx_flag_lines=$(rg -c -- '-stdlib=libc\+\+' "$build_log" || true)
  libcxxabi_flag_lines=$(rg -c -- '-lc\+\+abi' "$build_log" || true)
  compiler_lines=${compiler_lines:-0}
  libcxx_flag_lines=${libcxx_flag_lines:-0}
  libcxxabi_flag_lines=${libcxxabi_flag_lines:-0}
  test "$compiler_lines" -gt 0
  test "$libcxx_flag_lines" -gt 0
  test "$libcxxabi_flag_lines" -gt 0
else
  compiler_lines=$(rg -c 'gcc version 14\.2\.0|GNU 14\.2\.0|-[^ ]*-g\+\+' "$build_log" || true)
  libcxx_flag_lines=$(rg -c -- '-stdlib=libc\+\+' "$build_log" || true)
  libcxxabi_flag_lines=$(rg -c -- '-lc\+\+abi' "$build_log" || true)
  compiler_lines=${compiler_lines:-0}
  libcxx_flag_lines=${libcxx_flag_lines:-0}
  libcxxabi_flag_lines=${libcxxabi_flag_lines:-0}
  test "$compiler_lines" -gt 0
  test "$libcxx_flag_lines" -eq 0
  test "$libcxxabi_flag_lines" -eq 0
fi
printf 'compiler_evidence_lines=%s\nstdlib_libcxx_flag_lines=%s\nlibcxxabi_flag_lines=%s\n' \
  "$compiler_lines" "$libcxx_flag_lines" "$libcxxabi_flag_lines"

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
    echo "LIBCXX_NEEDED ${candidate#$extract/}"
  fi
  if printf '%s\n' "$needed" | rg -q 'libstdc\+\+\.so\.6'; then
    libstdcxx_needed=$((libstdcxx_needed + 1))
    echo "LIBSTDCXX_NEEDED ${candidate#$extract/}"
  fi
done < <(find "$extract" -type f -print0)
printf 'elf_count=%s\nlibcxx_needed=%s\nlibstdcxx_needed=%s\n' \
  "$elf_count" "$libcxx_needed" "$libstdcxx_needed"
test "$elf_count" -gt 0
if [ "$mode" = libcxx ]; then
  test "$libcxx_needed" -gt 0
  test "$libstdcxx_needed" -eq 0
else
  test "$libstdcxx_needed" -gt 0
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
