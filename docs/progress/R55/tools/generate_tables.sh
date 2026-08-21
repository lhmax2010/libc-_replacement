#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
out="$workspace/progress/R55/tables"
mkdir -p "$out"

count_pattern() {
  local pattern=$1
  shift
  local count=0 file
  for file in "$@"; do
    grep -q "$pattern" "$file" && count=$((count + 1))
  done
  printf '%d' "$count"
}

{
  printf 'arch\tvariant\tdenominator_runs\texit_0\texit_124\texit_134\texit_143\twait_returned\tbroken_promise\tget_value\n'
  for arch in x86_64 armv7l; do
    for variant in baseline prefix fixed libstdcxx; do
      files=("$workspace/progress/R55/raw/$arch/async20/${variant}_"*.log)
      if [[ $arch == x86_64 ]]; then
        codes=$(awk -F= '/^EXIT_CODE=/{print $2}' "${files[@]}")
      else
        codes=$(awk -F= '/^REMOTE_CELL_EXIT_CODE=/{gsub(/\r/,"",$2); print $2}' "${files[@]}")
      fi
      c0=$(printf '%s\n' "$codes" | awk '$1==0{n++} END{print n+0}')
      c124=$(printf '%s\n' "$codes" | awk '$1==124{n++} END{print n+0}')
      c134=$(printf '%s\n' "$codes" | awk '$1==134{n++} END{print n+0}')
      c143=$(printf '%s\n' "$codes" | awk '$1==143{n++} END{print n+0}')
      wait_count=$(count_pattern 'WAIT_RETURNED=1' "${files[@]}")
      broken_count=$(count_pattern 'GET=future_error CODE=4' "${files[@]}")
      value_count=$(count_pattern 'GET=value' "${files[@]}")
      printf '%s\t%s\t20\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n' \
        "$arch" "$variant" "$c0" "$c124" "$c134" "$c143" \
        "$wait_count" "$broken_count" "$value_count"
    done
  done
} > "$out/async_specialty_20runs.tsv"

{
  printf 'arch\tfacility\tbaseline_rc\tprefix_full_rc\tfixed_full_rc\tlibstdcxx_rc\n'
  for arch in x86_64 armv7l; do
    for mode in cout getline_free getline_member read arithmetic packaged_task async; do
      values=()
      for variant in baseline prefix fixed libstdcxx; do
        file="$workspace/progress/R55/raw/$arch/facility/${variant}_${mode}.log"
        if [[ $arch == x86_64 ]]; then
          value=$(awk -F= '/^EXIT_CODE=/{print $2}' "$file")
        else
          value=$(awk -F= '/^REMOTE_CELL_EXIT_CODE=/{gsub(/\r/,"",$2); print $2}' "$file")
        fi
        values+=("$value")
      done
      printf '%s\t%s\t%s\t%s\t%s\t%s\n' "$arch" "$mode" "${values[@]}"
    done
  done
} > "$out/facility_matrix.tsv"

{
  printf 'arch\tmode\tbaseline_rc\tprefix_full_rc\tfixed_full_rc\tlibstdcxx_rc\n'
  for arch in x86_64 armv7l; do
    for mode in normal exception; do
      values=()
      for variant in baseline prefix fixed libstdcxx; do
        file="$workspace/progress/R55/raw/$arch/noncancel/${variant}_${mode}.log"
        if [[ $arch == x86_64 ]]; then
          value=$(awk -F= '/^EXIT_CODE=/{print $2}' "$file")
        else
          value=$(awk -F= '/^REMOTE_CELL_EXIT_CODE=/{gsub(/\r/,"",$2); print $2}' "$file")
        fi
        values+=("$value")
      done
      printf '%s\t%s\t%s\t%s\t%s\t%s\n' "$arch" "$mode" "${values[@]}"
    done
  done
} > "$out/noncancel_async.tsv"

{
  printf 'arch\tsuite\tdenominator_cells\tpass_exit_0\tfail_nonzero\n'
  for arch in x86_64 armv7l; do
    for suite in regression upstream; do
      files=("$workspace/progress/R55/raw/$arch/$suite/"*.log)
      if [[ $arch == x86_64 ]]; then
        codes=$(awk -F= '/^EXIT_CODE=/{print $2}' "${files[@]}")
      else
        codes=$(awk -F= '/^REMOTE_CELL_EXIT_CODE=/{gsub(/\r/,"",$2); print $2}' "${files[@]}")
      fi
      total=$(printf '%s\n' "$codes" | awk 'NF{n++} END{print n+0}')
      passed=$(printf '%s\n' "$codes" | awk '$1==0{n++} END{print n+0}')
      printf '%s\t%s\t%d\t%d\t%d\n' "$arch" "$suite" "$total" "$passed" "$((total-passed))"
    done
  done
} > "$out/regression_summary.tsv"

sha256sum "$out"/*.tsv > "$out/SHA256SUMS"
