#!/usr/bin/env bash
set -uo pipefail

out=${1:-progress/R23/inputs/input_identity.tsv}
mkdir -p "$(dirname "$out")"
printf 'role\tpath\tsha256\tsize_bytes\n' >"$out"

while IFS=$'\t' read -r role path; do
  if [ -f "$path" ]; then
    digest=$(sha256sum "$path" | awk '{print $1}')
    size=$(stat -c '%s' "$path")
    printf '%s\t%s\t%s\t%s\n' "$role" "$path" "$digest" "$size" >>"$out"
  else
    printf '%s\t%s\tNOT_FOUND\tNOT_FOUND\n' "$role" "$path" >>"$out"
  fi
done <<'EOF'
libgcc_exports	progress/R16/tables/libgcc_export_classification.tsv
libgcc_versions	progress/R16/tables/libgcc_symbol_versions.tsv
compiler_rt_missing	progress/R16/tables/compiler_rt_missing_helpers.tsv
missing_family_members	progress/R17/tables/missing_helper_family_members.tsv
missing_family_summary	progress/R17/tables/missing_helper_family_summary.tsv
r11_elf_inventory	progress/R11/tables/elf_inventory.tsv
r11_binary_packages	progress/R11/tables/binary_package_records.tsv
r11_binary_downloads	progress/R11/tables/binary_download_status.tsv
r11_source_packages	progress/R11/tables/source_package_records.tsv
r11_source_relations	progress/R11/tables/source_relations.tsv
r11_source_specs	progress/R11/tables/source_spec_inventory.tsv
r11_snapshot	progress/R11/tables/snapshot_identity.tsv
r11_repodata	progress/R11/tables/repodata_inventory.tsv
vendor_prebuilt	progress/R13/tables/vendor_prebuilt_closure_assignment.tsv
r22_migration	progress/R22/tables/migration_packages_full.tsv
r22_special_cases	progress/R22/tables/special_cases.tsv
r22_stage_summary	progress/R22/tables/stage_summary.tsv
r22_cross_checks	progress/R22/tables/cross_checks.tsv
r22_board_report	docs/progress/R22/R22_board_path_report.md
EOF
