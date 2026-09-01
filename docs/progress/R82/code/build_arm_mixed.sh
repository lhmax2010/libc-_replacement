#!/bin/bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
compiler="$workspace/progress/R68/tools/armv7l_lit_host_clangxx.sh"
include_boost="$workspace/tmp/corpus/extracted/boost-1.83.0-2.2.src.rpm/unpacked/boost_1_83_0.tar.bz2/boost_1.83.0"
boost_root="$workspace/progress/R82/build/rpm_roots/armv7l"
output="$workspace/progress/R82/build/armv7l"
remote_library=/opt/usr/r82_boost_boundary/lib

mkdir -p "$output"
common=(
  -std=c++20 -O0 -g -stdlib=libc++ -isystem "$include_boost"
  -Wl,-rpath,"$remote_library"
  -Wl,-rpath-link,"$boost_root/boost-atomic/usr/lib"
)

"$compiler" "${common[@]}" \
  "$workspace/progress/R82/code/filesystem_boundary.cpp" \
  "$boost_root/boost-filesystem/usr/lib/libboost_filesystem.so.1.83.0" \
  -o "$output/filesystem_boundary"

"$compiler" "${common[@]}" \
  "$workspace/progress/R82/code/iostreams_boundary.cpp" \
  "$boost_root/boost-iostreams/usr/lib/libboost_iostreams.so.1.83.0" \
  -o "$output/iostreams_boundary"

"$compiler" "${common[@]}" \
  "$workspace/progress/R82/code/program_options_boundary.cpp" \
  "$boost_root/boost-program-options/usr/lib/libboost_program_options.so.1.83.0" \
  -o "$output/program_options_boundary"

for binary in "$output/filesystem_boundary" "$output/iostreams_boundary" "$output/program_options_boundary"; do
  printf 'BUILT %s\n' "$binary"
  file "$binary"
  readelf -d "$binary" | sed -n '/NEEDED/p;/RUNPATH/p'
done
