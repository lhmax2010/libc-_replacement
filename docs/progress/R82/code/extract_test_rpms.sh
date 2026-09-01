#!/bin/bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
destination="$workspace/progress/R82/build/rpm_roots"
mkdir -p "$destination"

for arch in x86_64 armv7l; do
  arch_root="$destination/$arch"
  mkdir -p "$arch_root"
  for package in boost-filesystem boost-iostreams boost-program-options boost-atomic; do
    rpm="$workspace/tmp/R11/rpms/base_binary/$arch/${package}-1.83.0-2.2.$arch.rpm"
    test -f "$rpm"
    mkdir -p "$arch_root/$package"
    rpm2cpio "$rpm" | cpio -idm --quiet --directory "$arch_root/$package"
    printf '%s\t%s\t%s\n' "$arch" "$package" "$rpm"
  done
done
