#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
source_root=$workspace/tmp/R68/source-baseline/libcxx/test
test_list=$workspace/progress/R81/tables/r77_28_tests.tsv

printf 'test\tconstructors\tcreated_objects\tpermission_operations\tenvironment_or_external_paths\n'
while IFS=$'\t' read -r test _; do
  [[ "$test" == test ]] && continue
  source=$source_root/$test
  constructors=$({ rg -o 'static_test_env|scoped_test_env|get_temp_file_name' "$source" 2>/dev/null || true; } | sort -u | paste -sd, -)
  created=$({ rg -o 'create_(file|dir|symlink|directory_symlink|hardlink|fifo|socket)' "$source" 2>/dev/null || true; } | sort -u | paste -sd, -)
  permissions=$({ rg -o 'permissions|chmod' "$source" 2>/dev/null || true; } | sort -u | paste -sd, -)
  environment=$({ rg -o 'TMPDIR|TMP|TEMP|temp_directory_path|current_path|/dev/[[:alnum:]_/.-]+' "$source" 2>/dev/null || true; } | sort -u | paste -sd, -)
  printf '%s\t%s\t%s\t%s\t%s\n' \
    "$test" "${constructors:-NONE}" "${created:-NONE}" \
    "${permissions:-NONE}" "${environment:-NONE}"
done < "$test_list"
