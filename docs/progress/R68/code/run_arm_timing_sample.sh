#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
output_dir="$workspace/progress/R68/raw/capability/patched/armv7l/sample_actual_after_decode_fix"
lit="$workspace/tmp/R69/replay/llvm/utils/lit/lit.py"
suite="$workspace/tmp/R68/build-patched-armv7l/libcxx/test"
compiler="$workspace/progress/R68/tools/armv7l_lit_host_clangxx.sh"
executor="$workspace/progress/R68/tools/sdb_executor.py"
telemetry="$output_dir/executor_telemetry.tsv"
stdout_file="$output_dir/libcxx.stdout"
stderr_file="$output_dir/libcxx.stderr"
json_file="$output_dir/libcxx.json"
metadata_file="$output_dir/run_metadata.tsv"

mkdir -p "$output_dir"
: >"$telemetry"
: >"$stdout_file"
: >"$stderr_file"
: >"$metadata_file"

start_epoch=$(date +%s)
R68_SDB_TELEMETRY="$telemetry" \
  timeout --signal=TERM --kill-after=30s 1200s \
  nice -n 15 ionice -c 3 \
  python3 "$lit" \
    -j 2 \
    -D std=c++26 \
    -D compiler="$compiler" \
    -D executor="$executor" \
    --num-shards 100 \
    --run-shard 1 \
    --order=lexical \
    -v \
    -o "$json_file" \
    "$suite" \
    >"$stdout_file" 2>"$stderr_file"
lit_rc=$?
end_epoch=$(date +%s)
elapsed_seconds=$((end_epoch - start_epoch))

{
  printf 'command\t%s\n' "R68_SDB_TELEMETRY=$telemetry timeout --signal=TERM --kill-after=30s 1200s nice -n 15 ionice -c 3 python3 $lit -j 2 -D std=c++26 -D compiler=$compiler -D executor=$executor --num-shards 100 --run-shard 1 --order=lexical -v -o $json_file $suite"
  printf 'lit_exit_code\t%d\n' "$lit_rc"
  printf 'elapsed_seconds\t%d\n' "$elapsed_seconds"
  printf 'stdout\t%s\n' "$stdout_file"
  printf 'stderr\t%s\n' "$stderr_file"
  printf 'json\t%s\n' "$json_file"
  printf 'telemetry\t%s\n' "$telemetry"
} >"$metadata_file"

# lit 0 means all selected tests passed; lit 1 means selected test failures.
# Both are valid timing samples. Other codes are infrastructure failures.
if [[ "$lit_rc" -eq 0 || "$lit_rc" -eq 1 ]]; then
  exit 0
fi
exit "$lit_rc"
