#!/bin/bash
set -u

if [ "$#" -ne 4 ]; then
  printf 'usage: %s VARIANT ARCH EXPORT_MODE OUTPUT\n' "$0" >&2
  exit 2
fi
variant=$1
arch=$2
export_mode=$3
output=$4
workspace=/home/toolchain/development/libc++_replacement
source_file="$workspace/progress/R34B/src/s1_rethrow_probe.cpp"
build="$workspace/tmp/R40/build-$variant-$arch"
mkdir -p "$(dirname "$output")"

export_args=()
case "$export_mode" in
  none) ;;
  export-dynamic) export_args=(-Wl,--export-dynamic) ;;
  *) printf 'unsupported export mode: %s\n' "$export_mode" >&2; exit 3 ;;
esac

case "$arch" in
  x86_64)
    compiler="$workspace/progress/R33/tools/tizen-clang++"
    runtime="$workspace/tmp/R40/runtime/$variant/x86_64"
    args=(-O0 -g -pthread -stdlib=libc++ -nostdinc++ -I"$build/include/c++/v1" "$source_file"
          -L"$runtime" -Wl,-rpath,'$ORIGIN' -Wl,-z,now "${export_args[@]}" -lc++ -lc++abi -o "$output")
    split_compile_link=0
    ;;
  armv7l)
    compiler="$workspace/progress/R36/tools/armv7l-clang++"
    static_libcxx="$workspace/tmp/R38/build-unpatched-armv7l/lib/libc++.a"
    object="${output}.o"
    compile_args=(-O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ -I"$build/include/c++/v1"
                  -Dprivate_1=unwinder_cache.reserved1 -c "$source_file" -o "$object")
    link_args=("$object" -nostdlib++ "$static_libcxx" -L"$build/lib" -Wl,-rpath,'$ORIGIN' -Wl,-z,now
               "${export_args[@]}" -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$output")
    split_compile_link=1
    ;;
  *) printf 'unsupported arch: %s\n' "$arch" >&2; exit 4 ;;
esac

printf 'VARIANT=%s\nARCH=%s\nEXPORT_MODE=%s\nSTATIC_OR_DYNAMIC_LIBCXX=%s\n' \
  "$variant" "$arch" "$export_mode" "$([ "$arch" = armv7l ] && printf static || printf dynamic)"
printf 'CMAKE_BUILD_PARALLEL_LEVEL=2\nLLVM_PARALLEL_LINK_JOBS=1\n'
if [ "$split_compile_link" -eq 0 ]; then
  printf 'COMPILE_LINK_COMMAND='; printf ' %q' nice -n 15 ionice -c 3 "$compiler" "${args[@]}"; printf '\n'
  nice -n 15 ionice -c 3 "$compiler" "${args[@]}"
else
  printf 'COMPILE_COMMAND='; printf ' %q' nice -n 15 ionice -c 3 "$compiler" "${compile_args[@]}"; printf '\n'
  nice -n 15 ionice -c 3 "$compiler" "${compile_args[@]}" || exit $?
  printf 'LINK_COMMAND='; printf ' %q' nice -n 15 ionice -c 3 "$compiler" "${link_args[@]}"; printf '\n'
  nice -n 15 ionice -c 3 "$compiler" "${link_args[@]}"
fi
file "$output"
readelf -dW "$output"
printf 'DYNSYM_FORCED_BEGIN\n'; readelf --dyn-syms -W "$output" | grep -E '__forced_unwind|Num:' || true; printf 'DYNSYM_FORCED_END\n'
printf 'SYMTAB_FORCED_BEGIN\n'; readelf --syms -W "$output" | grep -E '__forced_unwind|Num:' || true; printf 'SYMTAB_FORCED_END\n'
sha256sum "$source_file" "$output"
