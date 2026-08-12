#!/usr/bin/env bash
set -euo pipefail
output=${1:?output TSV}
shift
printf 'log\twarning_lines\terror_lines\tforced_unwind_error_lines\n' >"$output"
for log in "$@"; do
    warnings=$(grep -Ec '^\[[[:space:]]*[0-9]+s\].*warning:' "$log" || true)
    errors=$(grep -Ec '^\[[[:space:]]*[0-9]+s\].*(error:|undefined reference)' "$log" || true)
    forced=$(grep -Ec '^\[[[:space:]]*[0-9]+s\].*(error:.*__forced_unwind|no type named .__forced_unwind)' "$log" || true)
    printf '%s\t%s\t%s\t%s\n' "$log" "$warnings" "$errors" "$forced" >>"$output"
done
