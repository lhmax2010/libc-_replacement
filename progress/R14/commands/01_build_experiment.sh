#!/usr/bin/env bash
set -euo pipefail
set -x

workspace=/home/toolchain/development/libc++_replacement
tmp_root="$workspace/tmp/R14"
src="$workspace/progress/R14/experiment_sources"
platform_b_rpm="$workspace/tmp/R12/downloads/libc++-22.1.8-19.1.x86_64.rpm"
platform_libstdcpp_rpm="$workspace/tmp/R11/rpms/base_binary/x86_64/libstdc++-14.2.0-1.13.x86_64.rpm"
platform_libgcc_rpm="$workspace/tmp/R12/downloads/libgcc-14.2.0-1.13.x86_64.rpm"
scheme_a_libcxx_rpm="$workspace/artifacts/rebuild_2218_c2/x86_64/libc++-22.1.8-1.x86_64.rpm"
scheme_a_libcxxabi_rpm="$workspace/artifacts/rebuild_2218_c2/x86_64/libc++abi-22.1.8-1.x86_64.rpm"

mkdir -p "$tmp_root/downloads" "$tmp_root/extracted/platform_b" \
  "$tmp_root/extracted/platform_libstdcpp" "$tmp_root/extracted/platform_libgcc" \
  "$tmp_root/extracted/scheme_a_libcxx" "$tmp_root/extracted/scheme_a_libcxxabi" \
  "$tmp_root/obj" "$tmp_root/runtime/A" "$tmp_root/runtime/B" "$tmp_root/bin"

cp -p "$platform_b_rpm" "$tmp_root/downloads/"
cp -p "$platform_libstdcpp_rpm" "$tmp_root/downloads/"
cp -p "$platform_libgcc_rpm" "$tmp_root/downloads/"
cp -p "$scheme_a_libcxx_rpm" "$tmp_root/downloads/"
cp -p "$scheme_a_libcxxabi_rpm" "$tmp_root/downloads/"

for rpm_path in \
  "$platform_b_rpm" "$platform_libstdcpp_rpm" "$platform_libgcc_rpm" \
  "$scheme_a_libcxx_rpm" "$scheme_a_libcxxabi_rpm"; do
  rpm -qp --qf '%{NAME}-%{EPOCHNUM}:%{VERSION}-%{RELEASE}.%{ARCH}\n' "$rpm_path"
  sha256sum "$rpm_path"
done

(cd "$tmp_root/extracted/platform_b" && rpm2cpio "$platform_b_rpm" | cpio -idm --quiet)
(cd "$tmp_root/extracted/platform_libstdcpp" && rpm2cpio "$platform_libstdcpp_rpm" | cpio -idm --quiet)
(cd "$tmp_root/extracted/platform_libgcc" && rpm2cpio "$platform_libgcc_rpm" | cpio -idm --quiet)
(cd "$tmp_root/extracted/scheme_a_libcxx" && rpm2cpio "$scheme_a_libcxx_rpm" | cpio -idm --quiet)
(cd "$tmp_root/extracted/scheme_a_libcxxabi" && rpm2cpio "$scheme_a_libcxxabi_rpm" | cpio -idm --quiet)

cp -a "$tmp_root/extracted/platform_b/usr/lib64/libc++.so.1" \
  "$tmp_root/extracted/platform_b/usr/lib64/libc++.so.1.0" \
  "$tmp_root/extracted/platform_b/usr/lib64/libc++abi.so.1" \
  "$tmp_root/extracted/platform_b/usr/lib64/libc++abi.so.1.0" \
  "$tmp_root/extracted/platform_b/usr/lib64/libunwind.so.1" \
  "$tmp_root/extracted/platform_b/usr/lib64/libunwind.so.1.0" \
  "$tmp_root/runtime/B/"
cp -a "$tmp_root/extracted/platform_libstdcpp/usr/lib64/libstdc++.so.6" \
  "$tmp_root/extracted/platform_libstdcpp/usr/lib64/libstdc++.so.6.0.33" \
  "$tmp_root/extracted/platform_libgcc/usr/lib64/libgcc_s.so.1" \
  "$tmp_root/runtime/B/"

cp -a "$tmp_root/extracted/scheme_a_libcxx/usr/lib64/libc++.so.1" \
  "$tmp_root/extracted/scheme_a_libcxx/usr/lib64/libc++.so.1.0" \
  "$tmp_root/extracted/scheme_a_libcxxabi/usr/lib64/libc++abi.so.1" \
  "$tmp_root/extracted/scheme_a_libcxxabi/usr/lib64/libc++abi.so.1.0" \
  "$tmp_root/runtime/A/"
cp -a "$tmp_root/extracted/platform_libstdcpp/usr/lib64/libstdc++.so.6" \
  "$tmp_root/extracted/platform_libstdcpp/usr/lib64/libstdc++.so.6.0.33" \
  "$tmp_root/extracted/platform_libgcc/usr/lib64/libgcc_s.so.1" \
  "$tmp_root/runtime/A/"

g++ -std=c++17 -O0 -g -fPIC -fno-stack-protector -fvisibility=hidden \
  -DR14_NEW_SIDE -I"$src" -c "$src/side.cpp" -o "$tmp_root/obj/new.o"
g++ -std=c++17 -O0 -g -fPIC -fno-stack-protector -fvisibility=hidden \
  -DR14_LEGACY_SIDE -I"$src" -c "$src/side.cpp" -o "$tmp_root/obj/legacy.o"

ld -shared --no-as-needed -soname libnew_B.so -o "$tmp_root/runtime/B/libnew_B.so" \
  "$tmp_root/obj/new.o" \
  "$tmp_root/runtime/B/libc++abi.so.1.0" \
  "$tmp_root/runtime/B/libunwind.so.1.0" \
  -L/lib/x86_64-linux-gnu -lc
ld -shared --no-as-needed -soname liblegacy.so -o "$tmp_root/runtime/B/liblegacy.so" \
  "$tmp_root/obj/legacy.o" \
  "$tmp_root/runtime/B/libstdc++.so.6.0.33" \
  "$tmp_root/runtime/B/libgcc_s.so.1" \
  -L/lib/x86_64-linux-gnu -lc -lm

ld -shared --no-as-needed -soname libnew_A.so -o "$tmp_root/runtime/A/libnew_A.so" \
  "$tmp_root/obj/new.o" \
  "$tmp_root/runtime/A/libc++abi.so.1.0" \
  "$tmp_root/runtime/A/libgcc_s.so.1" \
  -L/lib/x86_64-linux-gnu -lc -lm
cp -p "$tmp_root/runtime/B/liblegacy.so" "$tmp_root/runtime/A/liblegacy.so"

gcc -std=c11 -O0 -g "$src/main.c" -ldl -o "$tmp_root/bin/main_dlopen"

gcc -std=c11 -O0 -g "$src/main.c" -ldl -Wl,--no-as-needed \
  "$tmp_root/runtime/B/liblegacy.so" "$tmp_root/runtime/B/libnew_B.so" \
  -Wl,--allow-shlib-undefined -o "$tmp_root/bin/main_dt_B_legacy_new"
gcc -std=c11 -O0 -g "$src/main.c" -ldl -Wl,--no-as-needed \
  "$tmp_root/runtime/B/libnew_B.so" "$tmp_root/runtime/B/liblegacy.so" \
  -Wl,--allow-shlib-undefined -o "$tmp_root/bin/main_dt_B_new_legacy"
gcc -std=c11 -O0 -g "$src/main.c" -ldl -Wl,--no-as-needed \
  "$tmp_root/runtime/A/liblegacy.so" "$tmp_root/runtime/A/libnew_A.so" \
  -Wl,--allow-shlib-undefined -o "$tmp_root/bin/main_dt_A_legacy_new"
gcc -std=c11 -O0 -g "$src/main.c" -ldl -Wl,--no-as-needed \
  "$tmp_root/runtime/A/libnew_A.so" "$tmp_root/runtime/A/liblegacy.so" \
  -Wl,--allow-shlib-undefined -o "$tmp_root/bin/main_dt_A_new_legacy"

sha256sum "$src/shared.hpp" "$src/side.cpp" "$src/main.c" \
  "$tmp_root/obj/new.o" "$tmp_root/obj/legacy.o" \
  "$tmp_root/runtime/B/libnew_B.so" "$tmp_root/runtime/B/liblegacy.so" \
  "$tmp_root/runtime/A/libnew_A.so" "$tmp_root/runtime/A/liblegacy.so" \
  "$tmp_root/bin/main_dlopen" "$tmp_root/bin/main_dt_B_legacy_new" \
  "$tmp_root/bin/main_dt_B_new_legacy" "$tmp_root/bin/main_dt_A_legacy_new" \
  "$tmp_root/bin/main_dt_A_new_legacy"
