#!/bin/bash
set -euo pipefail

root=${1:?usage: final_gate.sh PROJECT_ROOT}
base="$root/docs/progress/BUILD_W3_0908"

"$base/code/validate_status_update.sh" "$root"

test "$(wc -l < "$base/PACKAGE_STATUS.tsv")" -eq 12
test "$(awk -F '\t' 'NR>1 {seen[$1]++} END {print length(seen)}' "$base/PACKAGE_STATUS.tsv")" -eq 11
test "$(awk -F '\t' 'NR>1 && $6 == "已完成并推送" {n++} END {print n+0}' "$base/PACKAGE_STATUS.tsv")" -eq 8
test "$(awk -F '\t' 'NR>1 && $6 ~ /^暂停/ {n++} END {print n+0}' "$base/PACKAGE_STATUS.tsv")" -eq 3
test "$(awk -F '\t' 'NR>1 && $3 ~ /^NOT_AVAILABLE/ {n++} END {print n+0}' "$base/PACKAGE_STATUS.tsv")" -eq 3

for package in abseil-cpp boost icu jsoncpp 'libsigc++' pcre taglib tensorflow2 llvm bcc-tools bpftrace; do
  awk -F '\t' -v package="$package" 'NR>1 && $1 == package {found=1} END {exit !found}' \
    "$base/PACKAGE_STATUS.tsv"
done

sha256sum "$base/PACKAGE_STATUS.tsv" "$base/QUICKBUILD_INPUT.md" \
  "$base/updated/base_adaptation_report_zh.md" \
  "$base/updated/base_adaptation_status_en.md" \
  "$base/updated/base_packages_zh.tsv" "$base/updated/base_packages_en.tsv"

printf 'package_rows=11\ncompleted=8\npaused=3\nno_adaptation_commit=3\nfinal_gate=PASS\n'
