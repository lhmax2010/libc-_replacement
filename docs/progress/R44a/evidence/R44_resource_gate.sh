#!/usr/bin/env bash
set -u

out_dir=${1:?output directory required}
mkdir -p "$out_dir"

{
  echo '$ nproc'
  nproc
  echo "[exit_code=$?]"
  echo '$ free -h'
  free -h
  echo "[exit_code=$?]"
  echo '$ uptime'
  uptime
  echo "[exit_code=$?]"
  echo '$ df -h /home/toolchain/development/libc++_replacement'
  df -h /home/toolchain/development/libc++_replacement
  echo "[exit_code=$?]"
  echo '$ top -bn1 | head -20'
  set +o pipefail
  top -bn1 | head -20
  top_rc=${PIPESTATUS[0]}
  head_rc=${PIPESTATUS[1]}
  set -o pipefail
  echo "[pipeline_exit_codes top=$top_rc head=$head_rc]"
} >"$out_dir/preflight.raw.log" 2>&1

cpu_count=$(nproc)
mem_kib=$(awk '/^MemAvailable:/ {print $2}' /proc/meminfo)
load_one=$(awk '{print $1}' /proc/loadavg)
load_limit=$(awk -v n="$cpu_count" 'BEGIN {printf "%.6f", n * 1.5}')

{
  printf 'cpu_count\t%s\n' "$cpu_count"
  printf 'mem_available_kib\t%s\n' "$mem_kib"
  printf 'mem_threshold_kib\t%s\n' 2097152
  printf 'load_one\t%s\n' "$load_one"
  printf 'load_limit\t%s\n' "$load_limit"
} >"$out_dir/gate_values.tsv"

if (( mem_kib < 2097152 )); then
  printf 'RED_STOP_RESOURCE\tMEMORY\t%s\n' "$mem_kib" >"$out_dir/gate_result.tsv"
  exit 20
fi

if awk -v actual="$load_one" -v limit="$load_limit" 'BEGIN {exit !(actual > limit)}'; then
  printf 'RED_STOP_RESOURCE\tLOAD\t%s\t%s\n' "$load_one" "$load_limit" >"$out_dir/gate_result.tsv"
  exit 21
fi

printf 'PASS\t%s\t%s\n' "$mem_kib" "$load_one" >"$out_dir/gate_result.tsv"
exit 0
