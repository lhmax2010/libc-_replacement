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
repo_root="$build_root/local/repos/tizen_unified_standard/$arch/RPMS"
scratch="$build_root/local/BUILD-ROOTS/scratch.$arch.0"

test -f "$build_log"
test ! -e "$output_dir"
mkdir -p "$output_dir"

mapfile -t rpm_names < <(sed -n \
  "s#^.*Wrote: /home/abuild/rpmbuild/RPMS/[^/]*/##p" "$build_log" | \
  sed -n "/-${release}\.\(noarch\|${arch}\)\.rpm$/p" | \
  sed '/-debuginfo-/d; /-debugsource-/d' | sort -u)
test "${#rpm_names[@]}" -gt 0

echo "arch=$arch"
echo "mode=$mode"
echo "rpm_count=${#rpm_names[@]}"
if [ "$mode" = libcxx ]; then
  rg -m 1 'clang version 22\.1\.8' "$build_log"
  install_line=$(rg -m 1 '^\[[^]]+\] \+ \./b2 install .*cxxflags=' "$build_log")
  printf '%s\n' "$install_line"
  printf '%s\n' "$install_line" | rg -- '-stdlib=libc\+\+'
  printf '%s\n' "$install_line" | rg -- '-lc\+\+abi'
else
  rg -m 1 'gcc version 14\.2\.0|Tizen GCC 14\.2\.0' "$build_log"
  if rg -q -- '-stdlib=libc\+\+|-lc\+\+abi' "$build_log"; then
    echo 'unexpected libc++ option in GCC cell' >&2
    exit 1
  fi
fi

elf_count=0
libcxx_needed=0
libstdcxx_needed=0
neutral_elf=0
std1_symbols=0
stdcxx11_symbols=0
for rpm_name in "${rpm_names[@]}"; do
  rpm_path="$repo_root/$rpm_name"
  test -f "$rpm_path"
  package_dir="$output_dir/${rpm_name%.rpm}"
  mkdir -p "$package_dir"
  rpm2cpio "$rpm_path" | (cd "$package_dir" && cpio -idm --quiet)
  package_elf=0
  package_libcxx=0
  package_libstdcxx=0
  while IFS= read -r -d '' candidate; do
    file -b "$candidate" | rg -q '^ELF' || continue
    package_elf=$((package_elf + 1))
    elf_count=$((elf_count + 1))
    needed=$(readelf -d "$candidate" 2>/dev/null | rg 'NEEDED' || true)
    has_libcxx=0
    has_libstdcxx=0
    if printf '%s\n' "$needed" | rg -q 'libc\+\+\.so\.1'; then
      has_libcxx=1
      package_libcxx=$((package_libcxx + 1))
      libcxx_needed=$((libcxx_needed + 1))
    fi
    if printf '%s\n' "$needed" | rg -q 'libstdc\+\+\.so\.6'; then
      has_libstdcxx=1
      package_libstdcxx=$((package_libstdcxx + 1))
      libstdcxx_needed=$((libstdcxx_needed + 1))
    fi
    if [ "$has_libcxx" -eq 0 ] && [ "$has_libstdcxx" -eq 0 ]; then
      neutral_elf=$((neutral_elf + 1))
    fi
    one=$(nm -D "$candidate" 2>/dev/null | rg -c 'NSt3__1|St3__1' || true)
    eleven=$(nm -D "$candidate" 2>/dev/null | rg -c 'NSt7__cxx11|St7__cxx11|GLIBCXX_' || true)
    std1_symbols=$((std1_symbols + ${one:-0}))
    stdcxx11_symbols=$((stdcxx11_symbols + ${eleven:-0}))
    printf 'ELF\t%s\tlibcxx=%s\tlibstdcxx=%s\tstd___1=%s\tstd___cxx11=%s\n' \
      "${candidate#$package_dir/}" "$has_libcxx" "$has_libstdcxx" "${one:-0}" "${eleven:-0}"
  done < <(find "$package_dir" -type f -print0)
  printf 'RPM_SUMMARY\t%s\telf=%s\tlibcxx=%s\tlibstdcxx=%s\n' \
    "$rpm_name" "$package_elf" "$package_libcxx" "$package_libstdcxx"
done

printf 'TOTAL elf=%s libcxx_needed=%s libstdcxx_needed=%s neutral_elf=%s std___1=%s std___cxx11=%s\n' \
  "$elf_count" "$libcxx_needed" "$libstdcxx_needed" "$neutral_elf" "$std1_symbols" "$stdcxx11_symbols"
test "$elf_count" -gt 0
if [ "$mode" = libcxx ]; then
  test "$libcxx_needed" -gt 0
  test "$libstdcxx_needed" -eq 0
  test "$std1_symbols" -gt 0
  test "$stdcxx11_symbols" -eq 0
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
else
  test "$libstdcxx_needed" -gt 0
  test "$libcxx_needed" -eq 0
  test "$std1_symbols" -eq 0
fi

echo PASS
