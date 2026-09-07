#!/bin/bash
set -euo pipefail

root=$(cd "$(dirname "$0")/../../../.." && pwd)
out="$root/docs/progress/R110/build/w3"
log="$out/build_commands.tsv"
mkdir -p "$out"
: >"$log"
printf 'label\tcommand\texit_code\n' >>"$log"

buildroot=/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/R105-boost-aarch64/local/BUILD-ROOTS/scratch.aarch64.0
clang_root=/home/toolchain/development/libc++_replacement/progress/R80/build/cached-clang
tmp_root=/tmp/r110_w3_build
tmp_out="$tmp_root/bin"
mkdir -p "$tmp_out"
libcxx="$tmp_root/root/usr/lib64"
libstdcxx="$tmp_root/root/usr/lib64"
libgcc="$tmp_root/root/usr/lib64"
overlay="$tmp_root/root"
provider_overlay="$tmp_root/root/usr/lib64"
boost_thread="$provider_overlay/libboost_thread.so.1.83.0"
boost_filesystem="$provider_overlay/libboost_filesystem.so.1.83.0"
boost_iostreams="$provider_overlay/libboost_iostreams.so.1.83.0"
boost_program_options="$provider_overlay/libboost_program_options.so.1.83.0"
gcc_overlay="$tmp_root/root"
gcc_headers="$gcc_overlay/usr/lib64/gcc/aarch64-tizen-linux-gnu/14.2.0/include/c++"
clang="$clang_root/usr/bin/clang++"

for input in "$clang" "$libcxx/libc++.so.1.0" "$libcxx/libc++abi.so.1.0" \
  "$libstdcxx/libstdc++.so.6.0.33" "$libgcc/libgcc_s.so.1" "$boost_thread" \
  "$boost_filesystem" "$boost_iostreams" "$boost_program_options" \
  "$overlay/usr/include/c++/v1/string" "$overlay/usr/include/boost/thread/once.hpp" \
  "$gcc_headers/string"; do
  test -r "$input"
done

run_build() {
  local label=$1
  shift
  local stdout="$out/${label}.build.stdout"
  local stderr="$out/${label}.build.stderr"
  local rendered
  printf -v rendered '%q ' "$@"
  set +e
  "$@" >"$stdout" 2>"$stderr"
  local rc=$?
  set -e
  printf '%s\t%s\t%s\n' "$label" "$rendered" "$rc" >>"$log"
  printf '%s\n' "$rc" >"$out/${label}.build.exitcode"
  if [ "$rc" -ne 0 ]; then
    return "$rc"
  fi
}

common=(env "LD_LIBRARY_PATH=$clang_root/usr/lib64" "$clang" --no-default-config
  --target=aarch64-tizen-linux-gnu --sysroot="$buildroot" -fuse-ld=lld
  -resource-dir="$tmp_root/clang_resource"
  -std=c++20 -O0 -g0 -nostdinc++ -nostdlib++ -pthread)
mixed_headers=(-isystem "$overlay/usr/include/c++/v1" -isystem "$overlay/usr/include")
control_headers=(-isystem "$gcc_headers" -isystem "$gcc_headers/aarch64-tizen-linux-gnu"
  -isystem "$gcc_headers/backward" -isystem "$overlay/usr/include")

run_build boost_test_mixed_aarch64 nice -n 15 ionice -c 3 "${common[@]}" \
  "${mixed_headers[@]}" "$tmp_root/w3_boost_test_portable.cpp" \
  "$libcxx/libc++.so.1.0" "$libcxx/libc++abi.so.1.0" -ldl \
  -o "$tmp_out/boost_test_mixed_aarch64"
run_build boost_test_control_aarch64 nice -n 15 ionice -c 3 "${common[@]}" \
  "${control_headers[@]}" "$tmp_root/w3_boost_test_portable.cpp" \
  "$libstdcxx/libstdc++.so.6.0.33" "$libgcc/libgcc_s.so.1" -ldl \
  -o "$tmp_out/boost_test_control_aarch64"
run_build boost_thread_mixed_aarch64 nice -n 15 ionice -c 3 "${common[@]}" \
  "${mixed_headers[@]}" -include "$tmp_root/w3_maps_probe.hpp" \
  '-DW3_PROVIDER_TOKEN="libboost_thread.so.1.83.0"' \
  "$tmp_root/w3_boost_thread_boundary.cpp" "$boost_thread" \
  "$libcxx/libc++.so.1.0" "$libcxx/libc++abi.so.1.0" \
  "$libstdcxx/libstdc++.so.6.0.33" \
  -o "$tmp_out/boost_thread_mixed_aarch64"
run_build boost_thread_control_aarch64 nice -n 15 ionice -c 3 "${common[@]}" \
  "${control_headers[@]}" -include "$tmp_root/w3_maps_probe.hpp" \
  '-DW3_PROVIDER_TOKEN="libboost_thread.so.1.83.0"' \
  "$tmp_root/w3_boost_thread_boundary.cpp" "$boost_thread" \
  "$libstdcxx/libstdc++.so.6.0.33" "$libgcc/libgcc_s.so.1" \
  -o "$tmp_out/boost_thread_control_aarch64"

run_build boost_filesystem_mixed_aarch64 nice -n 15 ionice -c 3 "${common[@]}" \
  "${mixed_headers[@]}" -include "$tmp_root/w3_maps_probe.hpp" \
  '-DW3_PROVIDER_TOKEN="libboost_filesystem.so.1.83.0"' \
  "$tmp_root/filesystem_boundary.cpp" "$boost_filesystem" \
  "$libcxx/libc++.so.1.0" "$libcxx/libc++abi.so.1.0" \
  "$libstdcxx/libstdc++.so.6.0.33" "$libgcc/libgcc_s.so.1" \
  -o "$tmp_out/boost_filesystem_mixed_aarch64"
run_build boost_filesystem_control_aarch64 nice -n 15 ionice -c 3 "${common[@]}" \
  "${control_headers[@]}" -include "$tmp_root/w3_maps_probe.hpp" \
  '-DW3_PROVIDER_TOKEN="libboost_filesystem.so.1.83.0"' \
  "$tmp_root/filesystem_boundary.cpp" "$boost_filesystem" \
  "$libstdcxx/libstdc++.so.6.0.33" "$libgcc/libgcc_s.so.1" \
  -o "$tmp_out/boost_filesystem_control_aarch64"
run_build boost_iostreams_mixed_aarch64 nice -n 15 ionice -c 3 "${common[@]}" \
  "${mixed_headers[@]}" -include "$tmp_root/w3_maps_probe.hpp" \
  '-DW3_PROVIDER_TOKEN="libboost_iostreams.so.1.83.0"' \
  "$tmp_root/iostreams_boundary.cpp" "$boost_iostreams" \
  "$libcxx/libc++.so.1.0" "$libcxx/libc++abi.so.1.0" \
  "$libstdcxx/libstdc++.so.6.0.33" "$libgcc/libgcc_s.so.1" \
  -o "$tmp_out/boost_iostreams_mixed_aarch64"
run_build boost_iostreams_control_aarch64 nice -n 15 ionice -c 3 "${common[@]}" \
  "${control_headers[@]}" -include "$tmp_root/w3_maps_probe.hpp" \
  '-DW3_PROVIDER_TOKEN="libboost_iostreams.so.1.83.0"' \
  "$tmp_root/iostreams_boundary.cpp" "$boost_iostreams" \
  "$libstdcxx/libstdc++.so.6.0.33" "$libgcc/libgcc_s.so.1" \
  -o "$tmp_out/boost_iostreams_control_aarch64"
run_build boost_program_options_mixed_aarch64 nice -n 15 ionice -c 3 "${common[@]}" \
  "${mixed_headers[@]}" -include "$tmp_root/w3_maps_probe.hpp" \
  '-DW3_PROVIDER_TOKEN="libboost_program_options.so.1.83.0"' \
  "$tmp_root/program_options_boundary.cpp" "$boost_program_options" \
  "$libcxx/libc++.so.1.0" "$libcxx/libc++abi.so.1.0" \
  "$libstdcxx/libstdc++.so.6.0.33" "$libgcc/libgcc_s.so.1" \
  -o "$tmp_out/boost_program_options_mixed_aarch64"
run_build boost_program_options_control_aarch64 nice -n 15 ionice -c 3 "${common[@]}" \
  "${control_headers[@]}" -include "$tmp_root/w3_maps_probe.hpp" \
  '-DW3_PROVIDER_TOKEN="libboost_program_options.so.1.83.0"' \
  "$tmp_root/program_options_boundary.cpp" "$boost_program_options" \
  "$libstdcxx/libstdc++.so.6.0.33" "$libgcc/libgcc_s.so.1" \
  -o "$tmp_out/boost_program_options_control_aarch64"

cp "$tmp_out"/*_aarch64 "$out/"
file "$out"/*_aarch64
for binary in "$out"/*_aarch64; do
  readelf -d "$binary"
done
