#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
arch=${1:?architecture required}
overlay_include="$workspace/tmp/R52/overlay/include"
abi_include="$workspace/tmp/R51/source-product-final/libcxxabi/include"
version_script="$workspace/packaging/libcxx-llvm22.map"
out_dir="$workspace/tmp/R52/rebuilt-libcxx/$arch"

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
  *)
    printf 'unsupported architecture: %s\n' "$arch" >&2
    exit 2
    ;;
esac

source_root="$buildroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8"
build_dir="$source_root/build"
object_out="$out_dir/ios.instantiations.cpp.o"
dep_out="$out_dir/ios.instantiations.cpp.o.d"
library_out="$out_dir/libc++.so.1.0"
link_dep_out="$out_dir/libc++.link.d"
mkdir -p "$out_dir"

compile_command=$(ninja -C "$build_dir" -t commands libcxx/src/CMakeFiles/cxx_shared.dir/ios.instantiations.cpp.o |
  awk '/ -c \/home\/abuild\/rpmbuild\/BUILD\/llvm-22[.]1[.]8\/libcxx\/src\/ios[.]instantiations[.]cpp$/ {line=$0} END {print line}')
[[ -n "$compile_command" ]] || { printf 'compile command not found\n' >&2; exit 3; }
compile_command=${compile_command//\/home\/abuild\/rpmbuild\/BUILD\/llvm-22.1.8/$source_root}
compile_command=${compile_command/#$buildroot_compiler/${host_compiler@Q}}
compile_command=${compile_command/ -D/ -I${overlay_include@Q} -I${abi_include@Q} -D}
compile_command=${compile_command//-MF libcxx\/src\/CMakeFiles\/cxx_shared.dir\/ios.instantiations.cpp.o.d/-MF ${dep_out@Q}}
compile_command=${compile_command//-o libcxx\/src\/CMakeFiles\/cxx_shared.dir\/ios.instantiations.cpp.o/-o ${object_out@Q}}
printf 'COMPILE_COMMAND=cd %q && %s\n' "$build_dir" "$compile_command"
set +e
(cd "$build_dir" && eval "nice -n 15 ionice -c 3 $compile_command")
compile_rc=$?
set -e
printf 'COMPILE_EXIT_CODE=%d\n' "$compile_rc"
(( compile_rc == 0 )) || exit "$compile_rc"

link_command=$(ninja -C "$build_dir" -t commands cxx_shared |
  awk '/ -shared .* -o lib\/libc[+][+]\.so\.1\.0 / {line=$0} END {print line}')
[[ -n "$link_command" ]] || { printf 'link command not found\n' >&2; exit 4; }
link_command=${link_command//-Wl,--version-script=\/home\/abuild\/rpmbuild\/SOURCES\/libcxx-llvm22.map/-Wl,--version-script=${version_script@Q}}
link_command=${link_command//$buildroot_compiler/${host_compiler@Q}}
link_command=${link_command//-Xlinker --dependency-file=libcxx\/src\/CMakeFiles\/cxx_shared.dir\/link.d/-Xlinker --dependency-file=${link_dep_out@Q}}
link_command=${link_command//-o lib\/libc++.so.1.0/-o ${library_out@Q}}
link_command=${link_command//libcxx\/src\/CMakeFiles\/cxx_shared.dir\/ios.instantiations.cpp.o/${object_out@Q}}
link_command=${link_command//lib\/libc++abi.so.1.0/${product_abi@Q}}
link_command=${link_command//-Wl,-rpath,\/home\/abuild\/rpmbuild\/BUILD\/llvm-22.1.8\/build\/lib:/-Wl,-rpath,${out_dir@Q}:}
link_command=${link_command#: && }
link_command=${link_command% && :}
printf 'LINK_COMMAND=cd %q && %s\n' "$build_dir" "$link_command"
set +e
(cd "$build_dir" && eval "nice -n 15 ionice -c 3 $link_command")
link_rc=$?
set -e
printf 'LINK_EXIT_CODE=%d\n' "$link_rc"
(( link_rc == 0 )) || exit "$link_rc"

ln -sfn libc++.so.1.0 "$out_dir/libc++.so.1"
ln -sfn libc++.so.1 "$out_dir/libc++.so"
sha256sum "$object_out" "$library_out" "$product_abi"
readelf -dW "$library_out"
