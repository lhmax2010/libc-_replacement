#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
output_root="$workspace/progress/R81/raw/phase2/results/baseline_armv7l/libcxx"
batch_output="$output_root/batch-023"
tests_file="$workspace/docs/progress/R77/raw/batches/baseline_armv7l/libcxx/batch-023/tests.txt"
filter_file="$workspace/docs/progress/R77/raw/batches/baseline_armv7l/libcxx/batch-023/filter.regex"
lit="$workspace/tmp/R68/source-baseline/llvm/utils/lit/lit.py"
suite="$workspace/tmp/R68/build-baseline-armv7l/libcxx/test"
compiler="$workspace/progress/R68/tools/armv7l_lit_host_clangxx.sh"
executor="$workspace/progress/R81/code/sdb_executor_phase2_owner_baseline.sh"
deadline=$(awk -F '\t' '$1 == "phase2_deadline_epoch" {print $2}' \
  "$workspace/progress/R81/checkpoints/phase2_deadline.tsv")
memory_stop_kib=$((8 * 1024 * 1024))
load_stop=20
expected_tests=$(wc -l < "$tests_file")
filter=$(cat "$filter_file")

result="$batch_output/result.json"
stdout_file="$batch_output/lit.stdout"
stderr_file="$batch_output/lit.stderr"
telemetry="$batch_output/executor_telemetry.tsv"
resources="$batch_output/resources.log"
command_file="$batch_output/command.txt"
complete="$batch_output/COMPLETE.tsv"

mkdir -p "$batch_output"
test ! -e "$complete"
printf '%s\n' \
  "COMMAND=env R77_SDB_TEST_TIMEOUT_SECONDS=110 R77_SDB_TELEMETRY=$telemetry nice -n 15 ionice -c 3 python3 $lit -j 1 -D std=c++26 -D compiler=$compiler -D executor=$executor --order=lexical --show-unsupported --show-skipped --time-tests -v --filter=<archived batch-023/filter.regex> -o $result $suite" \
  > "$command_file"

sample_resources() {
  local available load1 load5 load15 now
  available=$(awk '$1 == "MemAvailable:" {print $2}' /proc/meminfo)
  read -r load1 load5 load15 _ < /proc/loadavg
  now=$(date -Ins)
  printf 'timestamp=%s mem_available_kib=%s load1=%s load5=%s load15=%s\n' \
    "$now" "$available" "$load1" "$load5" "$load15" | tee -a "$resources"
  if (( available < memory_stop_kib )); then
    printf 'threshold=MEMORY_8GIB observed_mem_available_kib=%s\n' "$available" \
      | tee -a "$resources"
    return 20
  fi
  if awk -v observed="$load1" -v limit="$load_stop" \
    'BEGIN { exit !(observed > limit) }'; then
    printf 'threshold=LOAD1_20 observed_load1=%s\n' "$load1" \
      | tee -a "$resources"
    return 20
  fi
  return 0
}

sample_resources || {
  printf 'RED_STOP_RESOURCE_BEFORE_START\n'
  exit 20
}

started=$(date +%s)
setsid env R77_SDB_TEST_TIMEOUT_SECONDS=110 R77_SDB_TELEMETRY="$telemetry" \
  nice -n 15 ionice -c 3 python3 "$lit" -j 1 \
  -D std=c++26 -D "compiler=$compiler" -D "executor=$executor" \
  --order=lexical --show-unsupported --show-skipped --time-tests -v \
  --filter "$filter" -o "$result" "$suite" \
  > "$stdout_file" 2> "$stderr_file" &
pid=$!

stop_reason=
while kill -0 "$pid" 2>/dev/null; do
  sleep 60
  if ! kill -0 "$pid" 2>/dev/null; then
    break
  fi
  if ! sample_resources; then
    stop_reason=RED_STOP_RESOURCE_8GIB_OR_LOAD20
  elif (( $(date +%s) >= deadline )); then
    stop_reason=TASK_DEADLINE_REACHED
  fi
  if [[ -n "$stop_reason" ]]; then
    printf '%s\n' "$stop_reason"
    kill -TERM -- "-$pid" 2>/dev/null || true
    for _ in $(seq 1 10); do
      kill -0 "$pid" 2>/dev/null || break
      sleep 1
    done
    if kill -0 "$pid" 2>/dev/null; then
      kill -KILL -- "-$pid" 2>/dev/null || true
    fi
    wait "$pid" || true
    [[ "$stop_reason" == RED_STOP_RESOURCE_8GIB_OR_LOAD20 ]] && exit 20
    exit 75
  fi
done

set +e
wait "$pid"
lit_exit=$?
set -e
sample_resources || {
  printf 'RED_STOP_RESOURCE_AFTER_COMPLETION\n'
  exit 20
}

if [[ "$lit_exit" -ne 0 && "$lit_exit" -ne 1 ]]; then
  printf 'TECHNICAL_LIT_EXIT\t%s\n' "$lit_exit"
  exit "$lit_exit"
fi
test -s "$result" || {
  printf 'MISSING_RESULT_JSON\n'
  exit 13
}
observed=$(jq '.tests | length' "$result")
if [[ "$observed" -ne "$expected_tests" ]]; then
  printf 'RESULT_COUNT_MISMATCH\texpected=%s\tobserved=%s\n' \
    "$expected_tests" "$observed"
  exit 14
fi
if grep -Eq 'ERROR: SDB carrier exited|device not found|device offline|failed to connect|remote exit marker was not observed' \
  "$stdout_file" "$stderr_file"; then
  printf 'SDB_CONNECTION_FAILURE\n'
  exit 90
fi

elapsed=$(( $(date +%s) - started ))
digest=$(sha256sum "$result" | awk '{print $1}')
printf 'field\tvalue\nbatch\tbatch-023\ntests\t%s\nlit_exit\t%s\nelapsed_seconds\t%s\nresult_sha256\t%s\n' \
  "$expected_tests" "$lit_exit" "$elapsed" "$digest" > "$complete"
printf 'BATCH_COMPLETE\tbatch-023\tlit_exit=%s\ttests=%s\telapsed=%s\n' \
  "$lit_exit" "$expected_tests" "$elapsed"
