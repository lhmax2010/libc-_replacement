#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
variant=${1:?patched or baseline required}
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote_root=/opt/usr/home/owner/share/tmp/r77_lit_20260829

case "$variant" in
  patched)
    build="$workspace/tmp/R68/build-patched-armv7l"
    ;;
  baseline)
    build="$workspace/tmp/R68/build-baseline-armv7l"
    ;;
  *)
    printf 'ERROR: invalid variant: %s\n' "$variant" >&2
    exit 10
    ;;
esac

libcxx="$build/libcxx/test-suite-install/lib/libc++.so.1.0"
libcxxabi="$build/libcxx/test-suite-install/lib/libc++abi.so.1.0"
for path in "$sdb" "$libcxx" "$libcxxabi"; do
  if [[ ! -e "$path" ]]; then
    printf 'ERROR: required path is absent: %s\n' "$path" >&2
    exit 11
  fi
done

host_libcxx_sha=$(sha256sum "$libcxx" | awk '{print $1}')
host_libcxxabi_sha=$(sha256sum "$libcxxabi" | awk '{print $1}')
printf 'VARIANT=%s\nHOST_LIBCXX_SHA256=%s\nHOST_LIBCXXABI_SHA256=%s\n' \
  "$variant" "$host_libcxx_sha" "$host_libcxxabi_sha"

"$sdb" -s "$serial" shell "ps -ef; printf '\n__R77_DF__\n'; df -k /opt/usr; printf '\n__R77_ROOT__\n'; if test -e '$remote_root'; then find '$remote_root' -maxdepth 3 -print; else echo ABSENT; fi"

process_snapshot=$("$sdb" -s "$serial" shell "ps -ef")
if grep -E 'r77_lit_20260829|lit\.py|sdb_executor\.py' <<<"$process_snapshot" >/dev/null; then
  printf 'ERROR: R77-related process exists on board before deployment\n' >&2
  printf '%s\n' "$process_snapshot" >&2
  exit 12
fi

root_state=$("$sdb" -s "$serial" shell "if test -e '$remote_root'; then echo PRESENT; else echo ABSENT; fi" | tr -d '\r')
if [[ "$root_state" != "ABSENT" ]]; then
  printf 'ERROR: task root already exists before deployment: %s\n' "$root_state" >&2
  exit 12
fi

"$sdb" -s "$serial" shell "set -e; mkdir -p '$remote_root/runtime' '$remote_root/runs'"
"$sdb" -s "$serial" push "$libcxx" "$remote_root/runtime/libc++.so.1.0.tmp"
"$sdb" -s "$serial" push "$libcxxabi" "$remote_root/runtime/libc++abi.so.1.0.tmp"
"$sdb" -s "$serial" shell "set -e; cd '$remote_root/runtime'; mv libc++.so.1.0.tmp libc++.so.1.0; mv libc++abi.so.1.0.tmp libc++abi.so.1.0; ln -sfn libc++.so.1.0 libc++.so.1; ln -sfn libc++.so.1 libc++.so; ln -sfn libc++abi.so.1.0 libc++abi.so.1; ln -sfn libc++abi.so.1 libc++abi.so; sha256sum libc++.so.1.0 libc++abi.so.1.0"

remote_hashes=$("$sdb" -s "$serial" shell "cd '$remote_root/runtime' && sha256sum libc++.so.1.0 libc++abi.so.1.0" | tr -d '\r')
remote_libcxx_sha=$(awk '$2 == "libc++.so.1.0" {print $1}' <<<"$remote_hashes")
remote_libcxxabi_sha=$(awk '$2 == "libc++abi.so.1.0" {print $1}' <<<"$remote_hashes")
if [[ "$remote_libcxx_sha" != "$host_libcxx_sha" || "$remote_libcxxabi_sha" != "$host_libcxxabi_sha" ]]; then
  printf 'ERROR: board runtime SHA mismatch\n%s\n' "$remote_hashes" >&2
  exit 13
fi
printf 'REMOTE_LIBCXX_SHA256=%s\nREMOTE_LIBCXXABI_SHA256=%s\nRESULT=PASS\n' \
  "$remote_libcxx_sha" "$remote_libcxxabi_sha"
