#!/usr/bin/env bash
# R47 x86_64 对照矩阵:宿主原生执行(GBS buildroot loader,无 QEMU)。
# 单格失败不中止(需要完整矩阵定性)。
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
buildroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0"
loader="$buildroot/lib64/ld-linux-x86-64.so.2"
syslibs="$buildroot/usr/lib64:$buildroot/lib64"
gbs_lib="$buildroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/lib"
r45_abi="$workspace/tmp/R45/runtime/x86_64/libc++abi.so.1.0"
r47_lib="$workspace/tmp/R47/build/x86_64/libc++.so.1.0"
tests="$workspace/tmp/R47/tests/x86_64"
runtime="$workspace/tmp/R47/runtime"

mkdir -p "$runtime"/{unpatched,layer_ab,key}
ln -sfn "$gbs_lib/libc++.so.1.0"    "$runtime/unpatched/libc++.so.1"
ln -sfn "$gbs_lib/libc++abi.so.1.0" "$runtime/unpatched/libc++abi.so.1"
ln -sfn "$gbs_lib/libc++.so.1.0"    "$runtime/layer_ab/libc++.so.1"
ln -sfn "$r45_abi"                  "$runtime/layer_ab/libc++abi.so.1"
ln -sfn "$r47_lib"                  "$runtime/key/libc++.so.1"
ln -sfn "$r45_abi"                  "$runtime/key/libc++abi.so.1"

printf 'IDENTITY_BEGIN\n'
sha256sum "$gbs_lib/libc++.so.1.0" "$gbs_lib/libc++abi.so.1.0" "$r45_abi" "$r47_lib" \
  "$tests/consumer_unpatched_O0" "$tests/consumer_unpatched_O2" \
  "$tests/consumer_patched_O0" "$tests/consumer_libstdcxx"
printf 'IDENTITY_END\n'

run_cell() {
  local mode=$1 suite=$2 library_path=$3 binary=$4 ld_debug=${5:-0}
  local rc
  printf '\nCELL_BEGIN arch=x86_64 mode=%s suite=%s binary=%s\n' "$mode" "$suite" "$(basename "$binary")"
  printf 'EXECUTION_MODE=HOST_NATIVE_X86_64_NO_QEMU\n'
  if [[ $ld_debug == 1 ]]; then
    printf 'LD_DEBUG_BEGIN\n'
    LD_DEBUG=libs /usr/bin/timeout --preserve-status 20s "$loader" \
      --library-path "$library_path" "$binary" "$mode" 2>&1 | grep -E "calling init|find library|needed by|libc\+\+" | head -30
    printf 'LD_DEBUG_END\n'
  fi
  printf 'RUN_COMMAND='
  printf '%q ' /usr/bin/timeout --preserve-status 20s "$loader" \
    --library-path "$library_path" "$binary" "$mode"
  printf '\n'
  /usr/bin/timeout --preserve-status 20s "$loader" \
    --library-path "$library_path" "$binary" "$mode"
  rc=$?
  printf 'CELL_EXIT_CODE=%d\n' "$rc"
  if (( rc > 128 )); then printf 'CELL_SIGNAL=%d\n' "$((rc - 128))"; else printf 'CELL_SIGNAL=0\n'; fi
  printf 'CELL_END arch=x86_64 mode=%s suite=%s\n' "$mode" "$suite"
}

for mode in getline_member read_member; do
  run_cell "$mode" unpatched            "$runtime/unpatched:$syslibs" "$tests/consumer_unpatched_O0"
  run_cell "$mode" layer_ab             "$runtime/layer_ab:$syslibs"  "$tests/consumer_unpatched_O0"
  run_cell "$mode" key_O0               "$runtime/key:$syslibs"       "$tests/consumer_unpatched_O0" 1
  run_cell "$mode" key_O2               "$runtime/key:$syslibs"       "$tests/consumer_unpatched_O2" 1
  run_cell "$mode" mirror_patched_consumer_old_lib "$runtime/layer_ab:$syslibs" "$tests/consumer_patched_O0"
  run_cell "$mode" patched_both         "$runtime/key:$syslibs"       "$tests/consumer_patched_O0"
  printf '\nCELL_BEGIN arch=x86_64 mode=%s suite=libstdcxx_control binary=consumer_libstdcxx\n' "$mode"
  printf 'EXECUTION_MODE=HOST_NATIVE_X86_64_NO_QEMU\n'
  printf 'RUN_COMMAND='
  printf '%q ' /usr/bin/timeout --preserve-status 20s "$tests/consumer_libstdcxx" "$mode"
  printf '\n'
  /usr/bin/timeout --preserve-status 20s "$tests/consumer_libstdcxx" "$mode"
  rc=$?
  printf 'CELL_EXIT_CODE=%d\n' "$rc"
  if (( rc > 128 )); then printf 'CELL_SIGNAL=%d\n' "$((rc - 128))"; else printf 'CELL_SIGNAL=0\n'; fi
  printf 'CELL_END arch=x86_64 mode=%s suite=libstdcxx_control\n' "$mode"
done
