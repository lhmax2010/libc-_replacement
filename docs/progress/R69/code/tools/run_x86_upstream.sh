#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
root="$workspace/tmp/R69/upstream/x86_64"
fixed="$workspace/tmp/R69/build-x86_64/lib"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
raw="$workspace/progress/R69/raw/x86_64/upstream"
mkdir -p "$raw"

run_case() {
  local family=$1 name=$2 binary=$3 library_path=${4:-} seconds=${5:-30} rc=0
  local log="$raw/${family}_${name}.log"
  if [[ -n $library_path ]]; then
    printf 'COMMAND=env LD_LIBRARY_PATH=%q timeout %ss %q\n' "$library_path" "$seconds" "$binary" > "$log"
    env LD_LIBRARY_PATH="$library_path" timeout "${seconds}s" "$binary" >> "$log" 2>&1 || rc=$?
  else
    printf 'COMMAND=timeout %ss %q\n' "$seconds" "$binary" > "$log"
    timeout "${seconds}s" "$binary" >> "$log" 2>&1 || rc=$?
  fi
  printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
  printf '%s\t%s\t%d\n' "$family" "$name" "$rc"
}

printf 'family\tcase\texit_code\n'
for runtime in libcxxabi libstdcxx; do
  path=""
  [[ $runtime == libcxxabi ]] && path="$fixed:$baseline"
  for test in forced_unwind{1,2,3,4} unwind_0{1,2,3,4,5,6}; do
    run_case "$runtime" "$test" "$root/$runtime/$test" "$path"
  done
done
for test in future_async future_async_race future_async_race_38682 future_get_future future_task_operator future_thread_create_failure future_wait_on_destruct istream_getline istream_read istream_ws string_shrink_to_fit; do
  run_case libcxx "$test" "$root/libcxx/$test" "$fixed:$baseline"
done
