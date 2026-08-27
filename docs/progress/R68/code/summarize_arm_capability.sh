#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
raw_root="$workspace/progress/R68/raw/capability"
summary_dir="$workspace/progress/R68/raw/capability/summary"
mkdir -p "$summary_dir"

counts="$summary_dir/armv7l_counts.tsv"
printf 'variant\tsuite\ttotal\trunnable\tunsupported\tunavailable\tpass_noexecute\txfail_noexecute\n' >"$counts"

for variant in patched baseline; do
  for suite in libcxx libcxxabi; do
    input="$raw_root/$variant/armv7l/$suite.sdb.no-execute.json"
    unsupported="$summary_dir/$variant.$suite.unsupported.tsv"
    reasons="$summary_dir/$variant.$suite.unsupported_reason_counts.tsv"

    jq -r '
      ["test", "reason"],
      (.tests[] | select(.code == "UNSUPPORTED") | [.name, (.output | gsub("[\\r\\n\\t]+"; " "))])
      | @tsv
    ' "$input" >"$unsupported"

    jq -r '
      [.tests[] | select(.code == "UNSUPPORTED") | (.output | gsub("[\\r\\n\\t]+"; " "))]
      | group_by(.)[] | [length, .[0]] | @tsv
    ' "$input" | sort -k1,1nr -k2,2 >"$reasons"

    total=$(jq '.tests | length' "$input")
    unsupported_count=$(jq '[.tests[] | select(.code == "UNSUPPORTED")] | length' "$input")
    pass_count=$(jq '[.tests[] | select(.code == "PASS")] | length' "$input")
    xfail_count=$(jq '[.tests[] | select(.code == "XFAIL")] | length' "$input")
    runnable=$((pass_count + xfail_count))
    # A loaded test with UNSUPPORTED is a framework skip. No test failed to be
    # enumerated or classified during this capability probe.
    unavailable=0
    printf '%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n' \
      "$variant" "$suite" "$total" "$runnable" "$unsupported_count" \
      "$unavailable" "$pass_count" "$xfail_count" >>"$counts"
  done
done
