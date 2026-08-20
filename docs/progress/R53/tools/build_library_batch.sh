#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
patched_include="$workspace/tmp/R53/overlay/include"
patched_ios="$workspace/tmp/R53/source-layer-c-full/libcxx/src/ios.cpp"
abi_include="$workspace/tmp/R51/source-product-final/libcxxabi/include"
version_script="$workspace/packaging/libcxx-llvm22.map"
out_dir="$workspace/tmp/R53/build/$arch"

case "$arch" in
  x86_64)
    buildroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0"
    buildroot_compiler=/bin/x86_64-tizen-linux-gnu-clang++
    host_compiler="$workspace/progress/R33/tools/tizen-clang++"
    product_abi="$workspace/tmp/R51/runtime/x86_64/libc++abi.so.1.0"
    ;;
  armv7l)
    buildroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2/local/BUILD-ROOTS/scratch.armv7l.0"
    buildroot_compiler=/bin/armv7l-tizen-linux-gnueabi-clang++
    host_compiler="$workspace/progress/R36/tools/armv7l-clang++"
    product_abi="$workspace/tmp/R51/build-product-armv7l/lib/libc++abi.so.1.0"
    ;;
  *) printf 'unsupported architecture: %s\n' "$arch" >&2; exit 2 ;;
esac

source_root="$buildroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8"
build_dir="$source_root/build"
mkdir -p "$out_dir"

compile_one() {
  local source_name=$1
  local source_override=$2
  local object_rel="libcxx/src/CMakeFiles/cxx_shared.dir/${source_name}.o"
  local object_out="$out_dir/${source_name}.o"
  local dep_out="$out_dir/${source_name}.o.d"
  local command
  command=$(ninja -C "$build_dir" -t commands "$object_rel" | awk -v n="$source_name" '$0 ~ " -c .*libcxx/src/" n "$" {line=$0} END {print line}')
  [[ -n "$command" ]] || { printf 'compile command not found: %s\n' "$source_name" >&2; exit 3; }
  command=${command//\/home\/abuild\/rpmbuild\/BUILD\/llvm-22.1.8/$source_root}
  command=${command/#$buildroot_compiler/${host_compiler@Q}}
  command=${command/ -D/ -I${patched_include@Q} -I${abi_include@Q} -D}
  command=${command//-MF ${object_rel}.d/-MF ${dep_out@Q}}
  command=${command//-o ${object_rel}/-o ${object_out@Q}}
  if [[ -n "$source_override" ]]; then
    command=${command//${source_root}\/libcxx\/src\/${source_name}/${source_override@Q}}
  fi
  printf 'COMPILE_TARGET=%s\n' "$source_name"
  printf 'COMPILE_COMMAND=cd %q && nice -n 15 ionice -c 3 %s\n' "$build_dir" "$command"
  (cd "$build_dir" && nice -n 15 ionice -c 3 bash -c "$command")
  printf 'COMPILE_EXIT_CODE=0\n'
}

compile_one ios.cpp "$patched_ios"
compile_one ios.instantiations.cpp ""

link_command=$(ninja -C "$build_dir" -t commands cxx_shared | awk '/ -shared .* -o lib\/libc[+][+]\.so\.1\.0 / {line=$0} END {print line}')
[[ -n "$link_command" ]] || { printf 'link command not found\n' >&2; exit 4; }
link_command=${link_command//-Wl,--version-script=\/home\/abuild\/rpmbuild\/SOURCES\/libcxx-llvm22.map/-Wl,--version-script=${version_script@Q}}
link_command=${link_command//$buildroot_compiler/${host_compiler@Q}}
link_command=${link_command//-Xlinker --dependency-file=libcxx\/src\/CMakeFiles\/cxx_shared.dir\/link.d/-Xlinker --dependency-file=${out_dir@Q}\/libc++.link.d}
link_command=${link_command//-o lib\/libc++.so.1.0/-o ${out_dir@Q}\/libc++.so.1.0}
link_command=${link_command//libcxx\/src\/CMakeFiles\/cxx_shared.dir\/ios.cpp.o/${out_dir@Q}\/ios.cpp.o}
link_command=${link_command//libcxx\/src\/CMakeFiles\/cxx_shared.dir\/ios.instantiations.cpp.o/${out_dir@Q}\/ios.instantiations.cpp.o}
link_command=${link_command//lib\/libc++abi.so.1.0/${product_abi@Q}}
link_command=${link_command//-Wl,-rpath,\/home\/abuild\/rpmbuild\/BUILD\/llvm-22.1.8\/build\/lib:/-Wl,-rpath,${out_dir@Q}:}
link_command=${link_command#: && }
link_command=${link_command% && :}
printf 'LINK_COMMAND=cd %q && nice -n 15 ionice -c 3 %s\n' "$build_dir" "$link_command"
(cd "$build_dir" && nice -n 15 ionice -c 3 bash -c "$link_command")
printf 'LINK_EXIT_CODE=0\n'

ln -sfn libc++.so.1.0 "$out_dir/libc++.so.1"
ln -sfn libc++.so.1 "$out_dir/libc++.so"
sha256sum "$out_dir/ios.cpp.o" "$out_dir/ios.instantiations.cpp.o" "$out_dir/libc++.so.1.0" "$product_abi"
readelf -dW "$out_dir/libc++.so.1.0"
