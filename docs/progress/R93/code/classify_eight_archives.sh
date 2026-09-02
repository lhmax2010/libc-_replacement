#!/bin/bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
status="$workspace/docs/progress/R92/tables/archive_status.tsv"
manifest="$workspace/docs/progress/R92/tables/cpp_gap_140.tsv"
work="$workspace/progress/R93/work/member_tables"
rm -rf "$work"
mkdir -p "$work"

index=0
while IFS=$'\t' read -r source_rpm archive result skipped_members; do
  test "$source_rpm" = source_rpm && continue
  case "$result" in
    *SKIPPED*) ;;
    *) continue ;;
  esac
  index=$((index + 1))
  srpm_rel=$(awk -F '\t' -v name="$source_rpm" 'NR > 1 && $1 == name {print $5}' "$manifest")
  srpm="$workspace/$srpm_rel"
  output=$(printf '%s/%02d.tsv' "$work" "$index")
  if test "$source_rpm" = chromium-efl-1.1.144-1.src.rpm; then
    rpm2archive "$srpm" |
      tar -xzO --wildcards -f - "*/$archive" |
      python3 "$workspace/progress/R93/code/classify_tar_members.py" \
        --source-rpm "$source_rpm" --archive "$archive" --output "$output"
    pipeline_status=("${PIPESTATUS[@]}")
  else
    rpm2cpio "$srpm" |
      cpio -i --quiet --to-stdout "$archive" |
      python3 "$workspace/progress/R93/code/classify_tar_members.py" \
        --source-rpm "$source_rpm" --archive "$archive" --output "$output"
    pipeline_status=("${PIPESTATUS[@]}")
  fi
  printf 'pipeline_status=%s expected_excluded=%s\n' "${pipeline_status[*]}" "$skipped_members"
  for code in "${pipeline_status[@]}"; do
    test "$code" -eq 0
  done
  actual=$(awk 'END { print NR - 1 }' "$output")
  test "$actual" -eq "$skipped_members"
done < "$status"

python3 "$workspace/progress/R93/code/merge_member_tables.py" "$work" "$workspace/progress/R93/tables"
rm -rf "$work"
