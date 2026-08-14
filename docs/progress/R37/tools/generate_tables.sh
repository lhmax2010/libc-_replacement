#!/bin/bash
set -euo pipefail

root=/home/toolchain/development/libc++_replacement
cd "$root"
mkdir -p progress/R37/tables

raw=progress/R37/commands/008_board_rpm_verify_retry.log
flagged=progress/R37/tables/rpm_verify_flagged.tsv
summary=progress/R37/tables/rpm_verify_summary.tsv

printf 'record_type\tflags\tconfig_marker\tpath_or_raw\tpermission_denied\n' > "$flagged"
awk '
  { sub(/\r$/, "") }
  /^RPM_VERIFY_BEGIN$/ { inside=1; next }
  /^RPM_VA_RC=/ { inside=0 }
  !inside { next }
  index($0, "missing") == 1 {
    raw=$0
    denied=(index(raw, "Permission denied") ? "YES" : "NO")
    gsub(/\t/, " ", raw)
    printf "MISSING\t-\t-\t%s\t%s\n", raw, denied
    next
  }
  length($0) >= 11 && substr($0, 10, 1) == " " {
    flags=substr($0, 1, 9)
    rest=substr($0, 12)
    config="NO"
    if (substr(rest, 1, 2) == "c ") {
      config="YES"
      rest=substr(rest, 3)
    }
    sub(/^[[:space:]]+/, "", rest)
    gsub(/\t/, " ", rest)
    printf "VERIFY_FLAGS\t%s\t%s\t%s\tNO\n", flags, config, rest
  }
' "$raw" >> "$flagged"

awk -F '\t' '
  NR == 1 { next }
  $1 == "MISSING" { missing++; if ($5 == "YES") denied++ }
  $1 == "VERIFY_FLAGS" {
    flagged++
    if ($3 == "YES") config++
    if ($4 ~ /^\/(lib|usr\/lib|bin|usr\/bin|sbin|usr\/sbin)\//) runtime++
    else other++
  }
  END {
    print "metric\tcount\tdenominator_or_scope"
    printf "missing_records\t%d\trpm_-Va_output_records\n", missing
    printf "missing_due_to_permission_records\t%d\tmissing_records\n", denied
    printf "verify_flag_records\t%d\trpm_-Va_output_records\n", flagged
    printf "config_marked_verify_records\t%d\tverify_flag_records\n", config
    printf "binary_or_library_path_verify_records\t%d\tverify_flag_records\n", runtime
    printf "other_path_verify_records\t%d\tverify_flag_records\n", other
  }
' "$flagged" > "$summary"

awk -F '\t' '
  BEGIN { print "flag\tcount\tscope" }
  NR > 1 && $1 == "VERIFY_FLAGS" && $4 ~ /^\/(lib|usr\/lib|bin|usr\/bin|sbin|usr\/sbin)\// {
    for (i=1; i<=9; i++) {
      c=substr($2, i, 1)
      if (c != ".") count[c]++
    }
  }
  END { for (c in count) printf "%s\t%d\tbinary_or_library_path_verify_records\n", c, count[c] }
' "$flagged" | { IFS= read -r header; printf '%s\n' "$header"; sort; } > progress/R37/tables/rpm_binary_flag_counts.tsv

awk -F '\t' '
  BEGIN { print "flags\tpath\tclassification" }
  NR > 1 && $1 == "VERIFY_FLAGS" && $4 ~ /^\/(lib|usr\/lib|bin|usr\/bin|sbin|usr\/sbin)\// && ($2 ~ /S/ || $2 ~ /5/) {
    printf "%s\t%s\tSIZE_OR_DIGEST_MISMATCH\n", $2, $4
  }
' "$flagged" > progress/R37/tables/rpm_binary_content_mismatches.tsv

index=progress/R37/tables/material_index.tsv
printf 'source_task\toriginal_path\tupload_path\tsha256\tstatus\tnote\n' > "$index"
while IFS= read -r upload; do
  rel=${upload#progress/R37/materials/}
  case "$rel" in
    R36/src/r34b_to_r36_actual_board_source.diff)
      task=R37
      original='progress/R34B/src/s1_rethrow_probe.cpp + progress/R36/src/s1_rethrow_probe_nostdlib.cpp'
      note='R37 由两份既有原件生成的请求差异；非 R36 原件'
      ;;
    R36/src/*)
      task=R36; original="progress/R36/src/${rel#R36/src/}"; note='逐字复制' ;;
    R36/libcxxabi/*)
      task=R36; original="progress/R36/code/libcxxabi/${rel#R36/libcxxabi/}"; note='逐字复制' ;;
    R36/code_tests/*)
      task=R36; original="progress/R36/code/tests/${rel#R36/code_tests/}"; note='逐字复制 R36 既有代码交付原件' ;;
    R36/evidence/*)
      task=R36; original="progress/R36/code/evidence/${rel#R36/evidence/}"; note='逐字复制 R36 既有身份原件' ;;
    R36/tools/*)
      task=R36; original="progress/R36/tools/${rel#R36/tools/}"; note='逐字复制' ;;
    R36/commands/*)
      task=R36; original="progress/R36/commands/${rel#R36/commands/}"; note='逐字复制' ;;
    R36/build/CMakeCache.txt)
      task=R36; original='tmp/R36/build-armv7l/CMakeCache.txt'; note='R37 只读复制现存构建缓存' ;;
    R34B/src/*)
      task=R34B; original="progress/R34B/src/${rel#R34B/src/}"; note='逐字复制参照原件' ;;
    *)
      task=UNKNOWN; original=NOT_AVAILABLE; note='UNMAPPED' ;;
  esac
  hash=$(sha256sum "$upload" | awk '{print $1}')
  printf '%s\t%s\t%s\t%s\tAVAILABLE\t%s\n' "$task" "$original" "$upload" "$hash" "$note"
done < <(find progress/R37/materials -type f -print | sort) >> "$index"

printf 'table\tsha256\trows_including_header\n' > progress/R37/tables/generated_table_identity.tsv
for file in "$flagged" "$summary" "$index" progress/R37/tables/rpm_binary_flag_counts.tsv progress/R37/tables/rpm_binary_content_mismatches.tsv; do
  printf '%s\t%s\t%s\n' "$file" "$(sha256sum "$file" | awk '{print $1}')" "$(wc -l < "$file")"
done >> progress/R37/tables/generated_table_identity.tsv
