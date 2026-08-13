#!/usr/bin/env bash
set -u

base=${1:?workspace root}
cd "$base" || exit 70

run() {
  printf '\nCOMMAND='
  printf '%q ' "$@"
  printf '\n'
  set +e
  "$@"
  rc=$?
  set -e
  printf 'EXIT_CODE=%d\n' "$rc"
}

printf 'SECTION=R30_EXACT_ERRORS\n'
run sed -n '1569,1576p' progress/R30/commands/024_build_patched_libcynara_libcxx_x86_64.full.log
run sed -n '1440,1458p' progress/R30/commands/053_build_final_security_libcxx_aarch64.full.log
run sed -n '1566,1582p' progress/R30/commands/045_build_patched_security_libcxx_x86_64.full.log

printf '\nSECTION=TARGET_SOURCE_CONTEXT\n'
run nl -ba codes/libcynara-commons/test/cyad/commands_dispatcher.cpp
run nl -ba codes/security-manager/src/dpl/core/include/dpl/fstream_accessors.h

printf '\nSECTION=LIBSTDCXX_DEFINITIONS\n'
run rg -n -C 5 '_S_badbit' codes/gcc/libstdc++-v3/include/bits/ios_base.h
run rg -n -C 5 '__filebuf_type' codes/gcc/libstdc++-v3/include/std/fstream

printf '\nSECTION=LIBCXX_ABSENCE\n'
run rg -n '_S_badbit|__filebuf_type' codes/llvm/libcxx/include

printf '\nSECTION=T1_PATTERN_SET_AND_HITS\n'
run sed -n '1,240p' progress/T1/patterns.tsv
run rg -n '_S_badbit|__filebuf_type' progress/T1/corpus_hits.tsv progress/R1/package_three_tiers.tsv

printf '\nSECTION=BOOST_BINARY_ABI_IDENTITY\n'
for arch in x86_64 aarch64 armv7l; do
  root="$base/tmp/GBS-ROOT/R30-final-security-libcxx-${arch}-20260812-01/local/BUILD-ROOTS/scratch.${arch}.0"
  if [[ -e "$root/usr/lib64/libboost_unit_test_framework.so" ]]; then
    lib="$root/usr/lib64/libboost_unit_test_framework.so"
  else
    lib="$root/usr/lib/libboost_unit_test_framework.so"
  fi
  printf 'ARCH=%s LIB=%s\n' "$arch" "$lib"
  run readelf -dW "$lib"
  run readelf --dyn-syms -W "$lib"
done
