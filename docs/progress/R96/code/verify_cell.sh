#!/bin/bash
set -euo pipefail

if [ "$#" -lt 4 ] || [ "$#" -gt 5 ]; then
  echo "usage: $0 CELL BUILDROOT ARCH LIBDIR [RELEASE]" >&2
  exit 2
fi

cell=$1
buildroot=$2
arch=$3
libdir=$4
release=${5:-96.1}
workspace=$(cd "$(dirname "$0")/../../.." && pwd)
repo="$buildroot/local/repos/tizen_unified_standard/$arch/RPMS"
extract="$workspace/progress/R96/work/verify/$cell"

if [ -e "$extract" ]; then
  echo "verification directory already exists: $extract" >&2
  exit 3
fi
mkdir -p "$extract"

select_one() {
  local pattern=$1
  local -a matches
  mapfile -t matches < <(find "$repo" -maxdepth 1 -type f -name "$pattern" -print | sort)
  if [ "${#matches[@]}" -ne 1 ]; then
    printf 'expected exactly one RPM for %s, found %d\n' "$pattern" "${#matches[@]}" >&2
    printf '%s\n' "${matches[@]}" >&2
    return 1
  fi
  printf '%s\n' "${matches[0]}"
}

devel=$(select_one "libc++-devel-22.1.8-$release.$arch.rpm")
libcxx=$(select_one "libc++-22.1.8-$release.$arch.rpm")
libcxxabi=$(select_one "libc++abi-22.1.8-$release.$arch.rpm")

echo "cell=$cell"
echo "repo=$repo"
echo "devel=$devel"
echo "libcxx=$libcxx"
echo "libcxxabi=$libcxxabi"
sha256sum "$devel" "$libcxx" "$libcxxabi"

for rpm in "$devel" "$libcxx" "$libcxxabi"; do
  rpm_abs=$(readlink -f "$rpm")
  echo "extracting=$rpm_abs"
  (cd "$extract" && rpm2cpio "$rpm_abs" | cpio -idm)
done

check_match() {
  local label=$1
  local file=$2
  local pattern=$3
  echo "===== $label: $file ====="
  test -f "$file"
  rg -n -C 2 "$pattern" "$file"
}

inc="$extract/usr/include/c++/v1"
check_match basic_ostream "$inc/__ostream/basic_ostream.h" '__forced_unwind'
check_match future "$inc/future" '__forced_unwind'
check_match istream "$inc/istream" '__forced_unwind'
check_match string "$inc/string" '__forced_unwind'
check_match cxxabi "$inc/cxxabi.h" 'class .*__forced_unwind'

libcxx_so="$extract/usr/$libdir/libc++.so.1.0"
libcxxabi_so="$extract/usr/$libdir/libc++abi.so.1.0"
for so in "$libcxx_so" "$libcxxabi_so"; do
  echo "===== dynamic dependencies: $so ====="
  test -f "$so"
  file "$so"
  readelf -d "$so" | rg 'NEEDED|SONAME'
  readelf -d "$so" | rg -q 'Shared library: \[libgcc_s\.so\.1\]'
done

echo "verification_result=PASS"
