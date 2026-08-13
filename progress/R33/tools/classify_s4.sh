#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
input=$workspace/progress/R33/tables/s4_guard_locations.tsv
output=$workspace/progress/R33/tables/s4_classification.tsv
summary=$workspace/progress/R33/tables/s4_summary.tsv
packages=$workspace/progress/R33/tables/s4_packages.tsv
awk -F '\t' 'BEGIN {
  OFS="\t";
  print "source_rpm","package_name","stage","file","line","scope","B_specific_types","C_disable_cancel","D_unconditional_rethrow","classification","context_fact","change_nature"
}
NR > 1 {
  fact=""; nature="MULTILINE_SEMANTIC_CHANGE_OR_RUNTIME_SUPPORT";
  if ($3=="boost" && $6 ~ /coroutine/) fact="catch-all stores arbitrary current_exception for later propagation";
  else if ($3=="boost" && $6 ~ /condition_variable/) fact="catch-all calls terminate for every non-forced exception";
  else if ($3=="boost" && $6 ~ /task_object/) fact="catch-all stores arbitrary current_exception into task state";
  else if ($3=="gcc" && $6 ~ /forced.C/) {fact="test uses catch-all to detect an unexpected handler selection"; nature="TEST_ONLY_REWRITE_OUTSIDE_BCD";}
  else if ($3=="gcc") fact="libstdc++ implementation deliberately swallows arbitrary exception in this operation";
  else if ($3=="key-manager" && $6 ~ /unit-tests/) {fact="test observes and rethrows only forced unwind; no following catch-all at this location"; nature="TEST_ONLY_REWRITE_OUTSIDE_BCD";}
  else if ($3=="key-manager") fact="catch-all maps arbitrary exception to logging, callback, or error return";
  else if ($3=="libcynara-commons") fact="catch-all maps arbitrary exception to CYNARA_API_UNKNOWN_ERROR";
  else if ($3=="llvm" || $3=="tensorflow2") fact="pybind11 catch-all runs arbitrary registered exception translators";
  else if ($3=="security-manager") fact="typed handlers are followed by a required unknown-exception fallback";
  else if ($3=="webauthn") fact="typed handlers are followed by a required unknown-exception fallback";
  else fact="context does not prove a finite exception set, dispensable cancellation, or unconditional propagation";
  print $2,$3,$4,$6,$7,$8,"NOT_ESTABLISHED","NOT_ESTABLISHED","NOT_ESTABLISHED","NO_BCD_OPTION_CONFIRMED",fact,nature;
}' "$input" > "$output"
{
  printf 'metric\tcount\tdenominator\tunit\n'
  printf 'locations_total\t%s\t50\tguard_location\n' "$(tail -n +2 "$output" | wc -l)"
  printf 'B_established\t%s\t50\tguard_location\n' "$(awk -F '\t' 'NR>1&&$7=="ESTABLISHED"{n++} END{print n+0}' "$output")"
  printf 'C_established\t%s\t50\tguard_location\n' "$(awk -F '\t' 'NR>1&&$8=="ESTABLISHED"{n++} END{print n+0}' "$output")"
  printf 'D_established\t%s\t50\tguard_location\n' "$(awk -F '\t' 'NR>1&&$9=="ESTABLISHED"{n++} END{print n+0}' "$output")"
  printf 'no_BCD_option_confirmed\t%s\t50\tguard_location\n' "$(awk -F '\t' 'NR>1&&$10=="NO_BCD_OPTION_CONFIRMED"{n++} END{print n+0}' "$output")"
  printf 'source_rpm_identities\t%s\t10\tsource_rpm_identity\n' "$(tail -n +2 "$output" | cut -f1 | sort -u | wc -l)"
  printf 'unique_package_names\t%s\t8\tpackage_name\n' "$(tail -n +2 "$output" | cut -f2 | sort -u | wc -l)"
} > "$summary"
{
  printf 'source_rpm\tpackage_name\tstage\tlocations\towner_team\n'
  tail -n +2 "$output" | awk -F '\t' 'BEGIN{OFS="\t"} {key=$1 FS $2 FS $3; n[key]++} END{for(key in n) print key,n[key],"NOT_AVAILABLE_NO_OWNER_METADATA_IN_INPUT"}' | sort
} > "$packages"
cat "$summary"
cat "$packages"
sha256sum "$input" "$output" "$summary" "$packages"
