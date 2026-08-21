#!/usr/bin/env bash
# R47:以补丁 istream overlay 重编 ios.instantiations.cpp.o 并重链 libc++.so.1.0。
# 改编自 R44b build_ios_spike.sh(x86_64 专用;源文件不改,补丁在 overlay 头文件中)。
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
out_dir="$workspace/tmp/R47/build/x86_64"
overlay_include="$workspace/tmp/R47/overlay/include"
abi_include="$workspace/tmp/R45/source-patched/libcxxabi/include"
version_script="$workspace/packaging/libcxx-llvm22.map"

buildroot="$workspace/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2/local/BUILD-ROOTS/scratch.x86_64.0"
r45_abi="$workspace/tmp/R45/runtime/x86_64/libc++abi.so.1.0"
buildroot_compiler=/bin/x86_64-tizen-linux-gnu-clang++
host_compiler="$workspace/progress/R33/tools/tizen-clang++"

source_root="$buildroot/home/abuild/rpmbuild/BUILD/llvm-22.1.8"
build_dir="$source_root/build"
object_out="$out_dir/ios.instantiations.cpp.o"
dep_out="$out_dir/ios.instantiations.cpp.o.d"
library_out="$out_dir/libc++.so.1.0"
link_dep_out="$out_dir/libc++.link.d"
mkdir -p "$out_dir"

compile_command=$(ninja -C "$build_dir" -t commands libcxx/src/CMakeFiles/cxx_shared.dir/ios.instantiations.cpp.o |
  awk '/ -c \/home\/abuild\/rpmbuild\/BUILD\/llvm-22[.]1[.]8\/libcxx\/src\/ios[.]instantiations[.]cpp$/ {line=$0} END {print line}')
if [[ -z $compile_command ]]; then
  printf 'compile command not found\n' >&2
  exit 3
fi

compile_command=${compile_command//\/home\/abuild\/rpmbuild\/BUILD\/llvm-22.1.8/$source_root}
compile_command=${compile_command/#$buildroot_compiler/${host_compiler@Q}}
# 前置 overlay(补丁 istream)与 R45 libcxxabi 头(__forced_unwind 所在的 cxxabi.h);
# 插在第一个 -D 之前,使其位于既有全部 -I 之前(与 R44b 同法)。
compile_command=${compile_command/ -D/ -I${overlay_include@Q} -I${abi_include@Q} -D}
compile_command=${compile_command//-MF libcxx\/src\/CMakeFiles\/cxx_shared.dir\/ios.instantiations.cpp.o.d/-MF ${dep_out@Q}}
compile_command=${compile_command//-o libcxx\/src\/CMakeFiles\/cxx_shared.dir\/ios.instantiations.cpp.o/-o ${object_out@Q}}

printf 'PWD=%q\n' "$build_dir"
printf 'COMPILE_COMMAND=cd %q && %s\n' "$build_dir" "$compile_command"
(cd "$build_dir" && eval "nice -n 15 ionice -c 3 $compile_command")
compile_rc=$?
printf 'COMPILE_EXIT_CODE=%s\n' "$compile_rc"
if (( compile_rc != 0 )); then
  exit "$compile_rc"
fi

link_command=$(ninja -C "$build_dir" -t commands cxx_shared |
  awk '/ -shared .* -o lib\/libc[+][+]\.so\.1\.0 / {line=$0} END {print line}')
if [[ -z $link_command ]]; then
  printf 'link command not found\n' >&2
  exit 4
fi

link_command=${link_command//-Wl,--version-script=\/home\/abuild\/rpmbuild\/SOURCES\/libcxx-llvm22.map/-Wl,--version-script=${version_script@Q}}
link_command=${link_command//$buildroot_compiler/${host_compiler@Q}}
link_command=${link_command//-Xlinker --dependency-file=libcxx\/src\/CMakeFiles\/cxx_shared.dir\/link.d/-Xlinker --dependency-file=${link_dep_out@Q}}
link_command=${link_command//-o lib\/libc++.so.1.0/-o ${library_out@Q}}
link_command=${link_command//libcxx\/src\/CMakeFiles\/cxx_shared.dir\/ios.instantiations.cpp.o/${object_out@Q}}
link_command=${link_command//lib\/libc++abi.so.1.0/${r45_abi@Q}}
link_command=${link_command//-Wl,-rpath,\/home\/abuild\/rpmbuild\/BUILD\/llvm-22.1.8\/build\/lib:/-Wl,-rpath,${out_dir@Q}:}
# ninja 输出的命令形如 “: && <link> && :”;剥掉前后哨兵,否则 nice/ionice 会去执行 “:”。
link_command=${link_command#: && }
link_command=${link_command% && :}

printf 'LINK_COMMAND=cd %q && %s\n' "$build_dir" "$link_command"
(cd "$build_dir" && eval "nice -n 15 ionice -c 3 $link_command")
link_rc=$?
printf 'LINK_EXIT_CODE=%s\n' "$link_rc"
if (( link_rc != 0 )); then
  exit "$link_rc"
fi

ln -sfn libc++.so.1.0 "$out_dir/libc++.so.1"
ln -sfn libc++.so.1 "$out_dir/libc++.so"
sha256sum "$object_out" "$library_out"
readelf -dW "$library_out" | head -20
