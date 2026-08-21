#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
out="$workspace/tmp/R55/x86_64/diagnosis"
raw="$workspace/progress/R55/raw/x86_64/diagnosis"
cxx="$workspace/progress/R33/tools/tizen-clang++"
sysroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0"
baseline="$sysroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build"
headers="$workspace/tmp/R54/build-all-x86_64/include/c++/v1"
libs="$workspace/tmp/R54/build-all-x86_64/lib"
runtime="$workspace/tmp/R54/runtime/x86_64/abc:$baseline/lib"

mkdir -p "$out" "$raw"

run_logged() {
  local log=$1
  shift
  {
    printf 'COMMAND='
    printf '%q ' "$@"
    printf '\n'
  } > "$log"
  set +e
  "$@" >> "$log" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE=%d\n' "$rc" >> "$log"
  return "$rc"
}

run_logged "$raw/compile.log" nice -n 15 ionice -c 3 \
  "$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ \
  -I"$headers" "$workspace/progress/R55/src/async_state_probe.cpp" \
  -L"$libs" -L"$baseline/lib" -Wl,-z,now -Wl,-rpath-link,"$libs" \
  -Wl,-rpath-link,"$baseline/lib" -lc++ -lc++abi -o "$out/async_state_probe"

set +e
run_logged "$raw/state_probe.log" env LD_LIBRARY_PATH="$runtime" \
  timeout 8s "$out/async_state_probe" async
state_probe_rc=$?
set -e
printf 'STATE_PROBE_EXIT_CODE=%d\n' "$state_probe_rc" >> "$raw/state_probe.log"

for mode in async packaged_task; do
  for variant in abc; do
    matrix_rc=0
    run_logged "$raw/state_${mode}_${variant}.log" env \
      LD_LIBRARY_PATH="$workspace/tmp/R54/runtime/x86_64/$variant:$baseline/lib" \
      timeout 8s "$out/async_state_probe" "$mode" || matrix_rc=$?
    printf 'MATRIX_EXIT_CODE=%d\n' "$matrix_rc" >> "$raw/state_${mode}_${variant}.log"
  done
done

run_logged "$raw/compile_libstdcxx.log" nice -n 15 ionice -c 3 \
  "$cxx" -std=c++17 -O0 -g -fexceptions -pthread \
  "$workspace/progress/R55/src/async_state_probe.cpp" -Wl,-z,now \
  -o "$out/async_state_probe_libstdcxx"
for mode in async packaged_task; do
  matrix_rc=0
  run_logged "$raw/state_${mode}_libstdcxx.log" timeout 8s \
    "$out/async_state_probe_libstdcxx" "$mode" || matrix_rc=$?
  printf 'MATRIX_EXIT_CODE=%d\n' "$matrix_rc" >> "$raw/state_${mode}_libstdcxx.log"
done

full_probe="$workspace/tmp/R54/probes/x86_64/full/facility_probe_libcxx"
hang_log="$raw/hung_process.log"
printf 'COMMAND=env LD_LIBRARY_PATH=%q %q async\n' "$runtime" "$full_probe" > "$hang_log"
env LD_LIBRARY_PATH="$runtime" "$full_probe" async >> "$hang_log" 2>&1 &
pid=$!
printf 'PID=%d\n' "$pid" >> "$hang_log"
sleep 2
ps -L -o pid,tid,stat,wchan:32,comm -p "$pid" > "$raw/thread_states.log"
for status in /proc/"$pid"/task/*/status; do
  printf 'FILE=%s\n' "$status"
  sed -n '/^Name:/p;/^State:/p;/^Pid:/p;/^Tgid:/p' "$status"
done > "$raw/proc_task_status.log"

set +e
gdb -q -batch -p "$pid" -ex 'set pagination off' -ex 'thread apply all bt' \
  > "$raw/gdb_backtrace.log" 2>&1
gdb_rc=$?
set -e
printf 'EXIT_CODE=%d\n' "$gdb_rc" >> "$raw/gdb_backtrace.log"

kill -TERM "$pid" 2>/dev/null || true
for _ in $(seq 1 20); do
  kill -0 "$pid" 2>/dev/null || break
  sleep 0.1
done
if kill -0 "$pid" 2>/dev/null; then
  kill -KILL "$pid" 2>/dev/null || true
fi
set +e
wait "$pid"
wait_rc=$?
set -e
printf 'WAIT_EXIT_CODE=%d\n' "$wait_rc" >> "$hang_log"
if kill -0 "$pid" 2>/dev/null; then
  printf 'CLEANUP=FAILED\n' >> "$hang_log"
  exit 90
fi
printf 'CLEANUP=PASS\n' >> "$hang_log"

run_logged "$raw/ldd.log" env LD_LIBRARY_PATH="$runtime" ldd "$out/async_state_probe"
run_logged "$raw/ld_debug.log" env LD_DEBUG=libs LD_LIBRARY_PATH="$runtime" \
  "$out/async_state_probe" async
