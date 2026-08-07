#!/usr/bin/env bash
set -euxo pipefail

workspace=/home/toolchain/development/libc++_replacement
tmp_root="$workspace/tmp/R14b"
source_root="$workspace/progress/R14b/experiment_sources"
object_root="$tmp_root/L2/obj"
binary_root="$tmp_root/L2/bin"
runtime_a="$tmp_root/runtime/L2A"
runtime_b="$tmp_root/runtime/L2B"
libcxxabi_a="$tmp_root/runtime/A/libc++abi.so.1.0"
libgcc="$tmp_root/runtime/A/libgcc_s.so.1"
libcxxabi_b="$tmp_root/runtime/B/libc++abi.so.1.0"
libunwind_b="$tmp_root/runtime/B/libunwind.so.1.0"
libstdcpp="$workspace/tmp/R11/extracted/b8/b8155206a459a575f0f701c66a553cd2029728963fa3938768131e9028e4ee6b/usr/lib64/libstdc++.so.6.0.33"

cp --preserve=mode,timestamps "$libcxxabi_a" "$runtime_a/libc++abi.so.1.0"
cp --preserve=mode,timestamps "$libgcc" "$runtime_a/libgcc_s.so.1"
cp --preserve=mode,timestamps "$libstdcpp" "$runtime_a/libstdc++.so.6.0.33"
ln -s libc++abi.so.1.0 "$runtime_a/libc++abi.so.1"
ln -s libstdc++.so.6.0.33 "$runtime_a/libstdc++.so.6"

cp --preserve=mode,timestamps "$libcxxabi_b" "$runtime_b/libc++abi.so.1.0"
cp --preserve=mode,timestamps "$libunwind_b" "$runtime_b/libunwind.so.1.0"
cp --preserve=mode,timestamps "$libgcc" "$runtime_b/libgcc_s.so.1"
cp --preserve=mode,timestamps "$libstdcpp" "$runtime_b/libstdc++.so.6.0.33"
ln -s libc++abi.so.1.0 "$runtime_b/libc++abi.so.1"
ln -s libunwind.so.1.0 "$runtime_b/libunwind.so.1"
ln -s libstdc++.so.6.0.33 "$runtime_b/libstdc++.so.6"

g++ -std=c++17 -O0 -g -fexceptions -fno-omit-frame-pointer -fPIC \
  -fvisibility=hidden -DR14B_NEW_SIDE -I"$source_root" \
  -c "$source_root/side.cpp" -o "$object_root/new.o"
g++ -std=c++17 -O0 -g -fexceptions -fno-omit-frame-pointer -fPIC \
  -fvisibility=hidden -DR14B_LEGACY_SIDE -I"$source_root" \
  -c "$source_root/side.cpp" -o "$object_root/legacy.o"
gcc -O0 -g -fPIC -c "$source_root/main.c" -o "$object_root/main.o"
gcc -shared -fPIC -O2 -Wall -Wextra "$source_root/audit.c" \
  -o "$binary_root/r14b_audit.so"

g++ -shared -nodefaultlibs -Wl,-z,defs -Wl,--no-as-needed \
  -Wl,-soname,libnew_A.so "$object_root/new.o" \
  "$libcxxabi_a" "$libgcc" -lc -lm -lpthread -ldl \
  -o "$runtime_a/libnew_A.so"
g++ -shared -nodefaultlibs -Wl,-z,defs -Wl,--no-as-needed \
  -Wl,-soname,libnew_B.so "$object_root/new.o" \
  "$libcxxabi_b" "$libunwind_b" -lc -lm -lpthread -ldl \
  -o "$runtime_b/libnew_B.so"
g++ -shared -nodefaultlibs -Wl,-z,defs -Wl,--no-as-needed \
  -Wl,-soname,liblegacy.so "$object_root/legacy.o" \
  "$libstdcpp" "$libgcc" -lc -lm -lpthread -ldl \
  -o "$binary_root/liblegacy.so"
cp "$binary_root/liblegacy.so" "$runtime_a/liblegacy.so"
cp "$binary_root/liblegacy.so" "$runtime_b/liblegacy.so"

gcc "$object_root/main.o" -ldl -o "$binary_root/main_dlopen"
gcc "$object_root/main.o" -Wl,--no-as-needed \
  -Wl,-rpath-link,"$runtime_a" -L"$runtime_a" \
  -l:liblegacy.so -l:libnew_A.so -ldl \
  -o "$binary_root/main_dt_A_legacy_new"
gcc "$object_root/main.o" -Wl,--no-as-needed \
  -Wl,-rpath-link,"$runtime_a" -L"$runtime_a" \
  -l:libnew_A.so -l:liblegacy.so -ldl \
  -o "$binary_root/main_dt_A_new_legacy"
gcc "$object_root/main.o" -Wl,--no-as-needed \
  -Wl,-rpath-link,"$runtime_b" -L"$runtime_b" \
  -l:liblegacy.so -l:libnew_B.so -ldl \
  -o "$binary_root/main_dt_B_legacy_new"
gcc "$object_root/main.o" -Wl,--no-as-needed \
  -Wl,-rpath-link,"$runtime_b" -L"$runtime_b" \
  -l:libnew_B.so -l:liblegacy.so -ldl \
  -o "$binary_root/main_dt_B_new_legacy"

readelf --wide --sections "$runtime_a/libnew_A.so" | grep -E '\.eh_frame_hdr|\.eh_frame'
readelf --wide --sections "$runtime_b/libnew_B.so" | grep -E '\.eh_frame_hdr|\.eh_frame'
readelf --wide --sections "$runtime_a/liblegacy.so" | grep -E '\.eh_frame_hdr|\.eh_frame'
