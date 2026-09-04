#!/bin/bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
base="$workspace/progress/R104/work/verify"

for arch in armv7l aarch64 x86_64; do
  echo "===== $arch ====="
  if [ "$arch" = armv7l ]; then libdir=lib; else libdir=lib64; fi
  libcxx="$base/libsigc++_${arch}_libcxx/usr/$libdir/libsigc-2.0.so.0.0.0"
  gcc="$base/libsigc++_${arch}_gcc/usr/$libdir/libsigc-2.0.so.0.0.0"
  for mode in libcxx gcc; do
    if [ "$mode" = libcxx ]; then file_path=$libcxx; else file_path=$gcc; fi
    echo "--- $mode ---"
    test -f "$file_path"
    file "$file_path"
    stat -c 'size=%s' "$file_path"
    sha256sum "$file_path"
    readelf -d "$file_path" | sed -n '/NEEDED/p'
    printf 'std___1_dynamic_symbols='
    nm -D "$file_path" | c++filt | rg -c 'std::__1::' || true
    printf 'std___cxx11_dynamic_symbols='
    nm -D "$file_path" | c++filt | rg -c 'std::__cxx11::' || true
    printf 'libcxx_mangled_markers='
    nm -D "$file_path" | rg -c 'NSt3__1|St3__1' || true
    printf 'libstdcxx_mangled_markers='
    nm -D "$file_path" | rg -c 'NSt7__cxx11|St7__cxx11|GLIBCXX_' || true
    readelf -p .comment "$file_path" 2>&1 || true
  done
  echo "--- exported dynamic symbol set diff (libcxx -> gcc) ---"
  nm -D -P --defined-only "$libcxx" | awk '{print $1, $2}' | sort -u > "/tmp/r105_libsigc_${arch}_libcxx.syms"
  nm -D -P --defined-only "$gcc" | awk '{print $1, $2}' | sort -u > "/tmp/r105_libsigc_${arch}_gcc.syms"
  diff -u "/tmp/r105_libsigc_${arch}_libcxx.syms" "/tmp/r105_libsigc_${arch}_gcc.syms" || true
done

echo "===== actual command evidence from all six R104 builds ====="
for spec in \
  '239 build_libsigc_armv7l_libcxx_retry libcxx' \
  '360 build_libsigc_armv7l_gcc_retry gcc' \
  '362 build_libsigc_aarch64_libcxx_retry libcxx' \
  '364 build_libsigc_aarch64_gcc_retry gcc' \
  '366 build_libsigc_x86_64_libcxx_retry libcxx' \
  '368 build_libsigc_x86_64_gcc_retry gcc'; do
  read -r number label mode <<<"$spec"
  log="$workspace/progress/R104/raw/${number}_${label}.stdout"
  test -f "$log"
  echo "--- $number $mode ---"
  rg -m 1 'C\+\+ compiler for the host machine:' "$log"
  if [ "$mode" = libcxx ]; then
    rg -m 1 -- '-stdlib=libc\+\+' "$log"
    rg -m 1 -- '-lc\+\+abi' "$log"
  else
    if rg -q -- '-stdlib=libc\+\+|-lc\+\+abi' "$log"; then
      echo 'UNEXPECTED_LIBCXX_FLAG'
      exit 1
    fi
    echo 'libcxx_flags=0'
  fi
done
