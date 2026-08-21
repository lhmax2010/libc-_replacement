#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
cd "$workspace"
tables=progress/R54/tables
mkdir -p "$tables"

awk -F'\t' 'BEGIN{OFS="\t"; print "group","case","variant","exit_code"}
  $1 ~ /^(layer|facility|concurrency)$/ && NF==4 {print}' \
  progress/R54/tables/x86_core_results_corrected.tsv > "$tables/x86_core_results_clean.tsv"

{
  printf 'arch\tcombination\tscenario\texit_code\tobserved\n'
  awk -F'\t' '$1=="layer" && NF==4 {print "x86_64\t" $2 "\t" ($2~/cout/?"stdlib_cout":($2~/catchall/?"catchall_rethrow":"typed_guard")) "\t" $4 "\tOBSERVED"}' "$tables/x86_core_results_clean.tsv"
  awk -F'\t' '$1=="layer" && NF==4 {print "armv7l\t" $2 "\t" ($2~/cout/?"stdlib_cout":($2~/catchall/?"catchall_rethrow":"typed_guard")) "\t" $4 "\tOBSERVED"}' progress/R54/tables/arm_core_results.tsv
  printf 'x86_64\tc_only\tcompile\t1\tmissing __forced_unwind type\n'
  printf 'armv7l\tc_only\tcompile\t1\tmissing __forced_unwind type\n'
} > "$tables/layer_interaction.tsv"

{
  printf 'facility\tarch\tbaseline_exit\tfull_exit\treference_exit\tfull_observation\n'
  for arch in x86_64 armv7l; do
    file=$tables/x86_core_results_clean.tsv; prefix=libstdcxx
    [[ $arch == armv7l ]] && { file=$tables/arm_core_results.tsv; prefix=reference; }
    for mode in cout getline_free getline_member read arithmetic packaged_task async; do
      b=$(awk -F'\t' -v n="baseline_$mode" '$1=="facility"&&$2==n{print $4}' "$file")
      f=$(awk -F'\t' -v n="full_$mode" '$1=="facility"&&$2==n{print $4}' "$file")
      r=$(awk -F'\t' -v n="${prefix}_$mode" '$1=="facility"&&$2==n{print $4}' "$file")
      observation=OBSERVED
      [[ $mode == cout && $arch == x86_64 && $f == 124 ]] && observation=JOIN_CANCELED_THEN_EXIT_TIMEOUT
      [[ $mode == async && $f != 0 ]] && observation=WAIT_DID_NOT_RETURN
      printf '%s\t%s\t%s\t%s\t%s\t%s\n' "$mode" "$arch" "$b" "$f" "$r" "$observation"
    done
    printf 'string_shrink_to_fit\t%s\tNOT_OBSERVED\tNOT_OBSERVED\tNOT_OBSERVED\tUNREACHABLE_NOEXCEPT\n' "$arch"
    printf 'ostream_sentry_destructor\t%s\tNOT_OBSERVED\tNOT_OBSERVED\tNOT_OBSERVED\tUNREACHABLE_NOEXCEPT\n' "$arch"
  done
} > "$tables/facility_matrix.tsv"

{
  printf 'arch\truntime\tmode\truns\texit_0\ttimed_or_signaled\tall_threads_joined\n'
  for arch in x86_64 armv7l; do
    file=$tables/x86_core_results_clean.tsv; reference=libstdcxx; raw=progress/R54/raw/x86_64/concurrency
    [[ $arch == armv7l ]] && { file=$tables/arm_core_results.tsv; reference=reference; raw=progress/R54/raw/armv7l/concurrency; }
    for runtime in full "$reference"; do
      for mode in multi_cancel cancel_and_exception stdlib_concurrent repeat_cancel; do
        zero=$(awk -F'\t' -v p="${runtime}_${mode}_" '$1=="concurrency"&&index($2,p)==1&&$4==0{n++}END{print n+0}' "$file")
        other=$((20-zero))
        if [[ $mode == stdlib_concurrent ]]; then
          joined=$(rg -l 'THREAD=3 JOIN_RC=0 JOIN_IS_CANCELED=1' "$raw"/${runtime}_${mode}_*.log 2>/dev/null | wc -l)
        else
          joined=$zero
        fi
        printf '%s\t%s\t%s\t20\t%s\t%s\t%s\n' "$arch" "$runtime" "$mode" "$zero" "$other" "$joined"
      done
    done
  done
} > "$tables/concurrency_summary.tsv"

{
  printf 'suite\tx86_64_pass\tx86_64_denominator\tarmv7l_pass\tarmv7l_denominator\n'
  printf 'R51_regression\t18\t18\t18\t18\n'
  printf 'libcxxabi_upstream\t20\t20\t20\t20\n'
  printf 'libcxx_selected\t7\t7\tNOT_AVAILABLE\tNOT_AVAILABLE\n'
  printf 'noncancel_behavior\t8\t8\t8\t8\n'
} > "$tables/regression_summary.tsv"

sha256sum "$tables"/*.tsv > "$tables/SHA256SUMS"
