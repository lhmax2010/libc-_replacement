#!/bin/bash
set -u

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote_script='set -u
printf "=== identity ===\n"
uname -a
cat /etc/tizen-release
printf "=== runtime library inventory ===\n"
for root in /usr/lib /usr/lib32 /lib /lib32; do
  for pattern in "libc++*.so*" "libstdc++*.so*" "libgcc_s.so*" "libunwind*.so*"; do
    find "$root" -maxdepth 2 -name "$pattern" -exec ls -lZ {} \; 2>&1
  done
done
printf "=== runtime hashes ===\n"
for root in /usr/lib /usr/lib32 /lib /lib32; do
  for pattern in "libc++*.so*" "libstdc++*.so*" "libgcc_s.so*" "libunwind*.so*"; do
    find "$root" -maxdepth 2 -name "$pattern" -type f -exec sha256sum {} \; 2>&1
  done
done
printf "=== package inventory ===\n"
rpm -qa | grep -E "^(libc\\+\\+|libcxx|libstdc\\+\\+|libgcc|libunwind)" | LC_ALL=C sort || true
printf "=== loader tools ===\n"
command -v ldd || true
command -v readelf || true
command -v sha256sum || true
printf "=== mounts ===\n"
mount
printf "REMOTE_EXIT_CODE=0\n"'
encoded=$(printf '%s' "$remote_script" | base64 -w0)
printf 'REMOTE_SCRIPT_BASE64=%s\n' "$encoded"
"$sdb" -s "$serial" shell "printf %s '$encoded' | base64 -d | sh"
