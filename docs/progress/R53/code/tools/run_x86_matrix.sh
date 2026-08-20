#!/usr/bin/env bash
set -u
set -o pipefail
workspace=/home/toolchain/development/libc++_replacement
sysroot="$workspace/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0"
loader="$sysroot/lib64/ld-linux-x86-64.so.2"
baseline="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
abi="$workspace/tmp/R51/runtime/x86_64/libc++abi.so.1.0"
full="$workspace/tmp/R53/build/x86_64/libc++.so.1.0"
tests="$workspace/tmp/R53/tests/x86_64"
runtime="$workspace/tmp/R53/runtime/x86_64"
syslibs="$sysroot/usr/lib64:$sysroot/lib64"
mkdir -p "$runtime"/{unpatched,layer_ab,full}
ln -sfn "$baseline/libc++.so.1.0" "$runtime/unpatched/libc++.so.1"
ln -sfn "$baseline/libc++abi.so.1.0" "$runtime/unpatched/libc++abi.so.1"
ln -sfn "$baseline/libc++.so.1.0" "$runtime/layer_ab/libc++.so.1"
ln -sfn "$abi" "$runtime/layer_ab/libc++abi.so.1"
ln -sfn "$full" "$runtime/full/libc++.so.1"
ln -sfn "$abi" "$runtime/full/libc++abi.so.1"

run_cell() {
  local scenario=$1 suite=$2 path=$3 binary=$4 arg=${5-}
  printf '\nCELL_BEGIN arch=x86_64 scenario=%s suite=%s\n' "$scenario" "$suite"
  printf 'EXECUTION_MODE=HOST_NATIVE_X86_64_NO_QEMU\nRUN_COMMAND='
  printf '%q ' /usr/bin/timeout --preserve-status 15s "$loader" --library-path "$path" "$binary"
  [[ -n $arg ]] && printf '%q ' "$arg"
  printf '\n'
  if [[ -n $arg ]]; then /usr/bin/timeout --preserve-status 15s "$loader" --library-path "$path" "$binary" "$arg"
  else /usr/bin/timeout --preserve-status 15s "$loader" --library-path "$path" "$binary"; fi
  rc=$?
  printf 'CELL_EXIT_CODE=%d\nCELL_SIGNAL=%d\nCELL_END arch=x86_64 scenario=%s suite=%s\n' "$rc" "$((rc>128 ? rc-128 : 0))" "$scenario" "$suite"
}

printf 'IDENTITY_BEGIN\n'
sha256sum "$baseline/libc++.so.1.0" "$baseline/libc++abi.so.1.0" "$abi" "$full" "$tests"/*
"$loader" --library-path "$runtime/full:$syslibs" --list "$tests/istream_stale"
printf 'LOADER_LIST_EXIT_CODE=%d\n' "$?"
LD_DEBUG=libs "$loader" --library-path "$runtime/full:$syslibs" "$tests/istream_stale" read_member </dev/null || true
printf 'IDENTITY_END\n'
for item in cout:cout failbit: getline_member:getline_member read_member:read_member; do
  scenario=${item%%:*}; arg=${item#*:}; base=${scenario%%_*}; [[ $base == getline || $base == read ]] && base=istream
  run_cell "$scenario" unpatched "$runtime/unpatched:$syslibs" "$tests/${base}_stale" "$arg"
  run_cell "$scenario" layer_ab "$runtime/layer_ab:$syslibs" "$tests/${base}_stale" "$arg"
  run_cell "$scenario" library_batch_stale_consumer "$runtime/full:$syslibs" "$tests/${base}_stale" "$arg"
  run_cell "$scenario" library_batch_rebuilt_consumer "$runtime/full:$syslibs" "$tests/${base}_rebuilt" "$arg"
  run_cell "$scenario" libstdcxx_control "$syslibs" "$tests/${base}_libstdcxx" "$arg"
done
exit 0
