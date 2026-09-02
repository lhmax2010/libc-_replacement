#!/bin/bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
work="$workspace/progress/R92/work/packaging_only_check"
rm -rf "$work"
mkdir -p "$work"
for source_rpm in mic-bootstrap-1.0-1.src.rpm qemu-accel-0.4-1.4.src.rpm; do
  source_path=$(awk -F '\t' -v name="$source_rpm" 'NR > 1 && $1 == name {print $5}' "$workspace/progress/R92/tables/cpp_gap_140.tsv")
  package_dir="$work/$source_rpm"
  mkdir -p "$package_dir"
  (
    cd "$package_dir"
    rpm2cpio "$workspace/$source_path" | cpio -idm --quiet --no-absolute-filenames
  )
  printf 'SOURCE_RPM=%s\n' "$source_rpm"
  find "$package_dir" -maxdepth 1 -type f -printf 'PAYLOAD=%f\n' | sort
  rg -n '^(Source[0-9]*:|%build|%install)' "$package_dir"/*.spec || true
done
rm -rf "$work"
