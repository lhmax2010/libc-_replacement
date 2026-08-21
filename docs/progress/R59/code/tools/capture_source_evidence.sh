#!/usr/bin/env bash
set -euo pipefail
root=/home/toolchain/development/libc++_replacement
out="$root/progress/R59/raw/source"
mkdir -p "$out"

capture() {
  local output=$1 file=$2 range=$3
  {
    printf 'SOURCE=%s\n' "$file"
    printf 'SHA256='
    sha256sum "$file" | awk '{print $1}'
    nl -ba "$file" | sed -n "$range"
  } > "$out/$output"
}

capture current_ios.txt "$root/docs/progress/R58/code/current/libcxx/src/ios.cpp" '345,400p'
capture baseline_ios.txt "$root/docs/progress/R58/code/baseline/libcxx/src/ios.cpp" '345,390p'
capture current_istream_f2.txt "$root/docs/progress/R58/code/current/libcxx/include/istream" '760,815p'
capture baseline_istream_f2.txt "$root/docs/progress/R58/code/baseline/libcxx/include/istream" '760,810p'
capture current_future_m1_m2.txt "$root/docs/progress/R58/code/current/libcxx/include/future" '760,925p'
capture baseline_future_m1_m2.txt "$root/docs/progress/R58/code/baseline/libcxx/include/future" '760,905p'
capture libcxx_future_state.txt "$root/codes/llvm/libcxx/src/future.cpp" '55,135p'
capture libstdcxx_istream_reference.txt "$root/codes/gcc/libstdc++-v3/include/bits/istream.tcc" '370,430p'
capture libstdcxx_future_state_core.txt "$root/codes/gcc/libstdc++-v3/include/std/future" '420,485p'
capture libstdcxx_future_task_setter.txt "$root/codes/gcc/libstdc++-v3/include/std/future" '1390,1460p'
capture libstdcxx_future_deferred.txt "$root/codes/gcc/libstdc++-v3/include/std/future" '1660,1730p'
capture libstdcxx_future_async.txt "$root/codes/gcc/libstdc++-v3/include/std/future" '1750,1810p'

sha256sum "$out"/*.txt > "$out/SHA256SUMS"
