#!/bin/bash
set -u

if [ "$#" -ne 4 ]; then
    echo "usage: $0 OPT TRIPLE INPUT_LL OUTPUT_DIR" >&2
    exit 2
fi

opt_bin=$1
target_triple=$2
input_ll=$3
output_dir=$4
mkdir -p "$output_dir"

"$opt_bin" -mtriple="$target_triple" \
    -passes=scc-oz-module-inliner \
    -enable-ml-inliner=default -S "$input_ll" \
    -o "$output_dir/default.ll" \
    >"$output_dir/default.stdout" 2>"$output_dir/default.stderr"
default_rc=$?

"$opt_bin" -mtriple="$target_triple" \
    -passes=scc-oz-module-inliner \
    -enable-ml-inliner=release \
    -pass-remarks=inline-ml \
    -pass-remarks-missed=inline-ml \
    -pass-remarks-output="$output_dir/release.remarks.yaml" \
    -S "$input_ll" -o "$output_dir/release.ll" \
    >"$output_dir/release.stdout" 2>"$output_dir/release.stderr"
release_rc=$?

printf 'default_rc=%s\nrelease_rc=%s\n' "$default_rc" "$release_rc" \
    >"$output_dir/exitcodes.txt"

test "$default_rc" -eq 0 || exit 10
test "$release_rc" -eq 0 || exit 11
grep -q '^Pass: *inline-ml$' "$output_dir/release.remarks.yaml" || exit 12
grep -q 'ShouldInline:' "$output_dir/release.remarks.yaml" || exit 12
"$opt_bin" -passes=verify -disable-output "$output_dir/release.ll" || exit 13

if cmp -s "$output_dir/default.ll" "$output_dir/release.ll"; then
    printf 'decision_output_relation=IDENTICAL\n'
else
    printf 'decision_output_relation=DIFFERENT\n'
fi
printf 'AOT_MODEL_EXECUTION_PASS\n'
