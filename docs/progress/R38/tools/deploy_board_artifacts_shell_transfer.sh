#!/bin/bash
set -u

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r38_s1_20260814
local_dir=/home/toolchain/development/libc++_replacement/tmp/R38/probes-armv7l

run_remote() {
    local command=$1
    printf 'SDB_COMMAND='
    printf '%q ' "$sdb" -s "$serial" shell "$command"
    printf '\n'
    "$sdb" -s "$serial" shell "$command"
    local rc=$?
    printf 'SDB_EXIT_CODE=%d\n' "$rc"
    return "$rc"
}

transfer_base64_chunks() {
    local source=$1
    local target=$2
    local encoded offset chunk
    encoded=$(gzip -n -c "$source" | base64 -w0) || return $?
    printf 'TRANSFER_SOURCE=%s\n' "$source"
    printf 'TRANSFER_TARGET=%s\n' "$target"
    printf 'TRANSFER_SOURCE_SHA256='
    sha256sum "$source" | awk '{print $1}'
    printf 'TRANSFER_ENCODING=gzip-n+base64\n'
    printf 'TRANSFER_ENCODED_BYTES=%d\n' "${#encoded}"
    printf 'TRANSFER_CHUNK_BYTES=512\n'
    run_remote ": > '$target.gz.b64'" || return $?
    offset=0
    while [ "$offset" -lt "${#encoded}" ]; do
        chunk=${encoded:offset:512}
        run_remote "printf '%s' '$chunk' >> '$target.gz.b64'" || return $?
        offset=$((offset + ${#chunk}))
    done
    run_remote "base64 -d '$target.gz.b64' > '$target.gz' && gzip -dc '$target.gz' > '$target' && rm -f '$target.gz.b64' '$target.gz' && chmod 700 '$target' && sha256sum '$target'" || return $?
}

run_remote "test ! -e '$remote' && mkdir '$remote' && mkdir '$remote/patched' '$remote/unpatched' && chmod 700 '$remote' '$remote/patched' '$remote/unpatched'" || exit $?
transfer_base64_chunks "$local_dir/s1_libcxx_strict" "$remote/s1_libcxx_strict" || exit $?
transfer_base64_chunks "$local_dir/s1_libstdcxx_strict" "$remote/s1_libstdcxx_strict" || exit $?
transfer_base64_chunks "$local_dir/patched/libc++abi.so.1.0" "$remote/patched/libc++abi.so.1" || exit $?
transfer_base64_chunks "$local_dir/unpatched/libc++abi.so.1.0" "$remote/unpatched/libc++abi.so.1" || exit $?
run_remote "find '$remote' -maxdepth 2 -ls && sha256sum '$remote/s1_libcxx_strict' '$remote/s1_libstdcxx_strict' '$remote/patched/libc++abi.so.1' '$remote/unpatched/libc++abi.so.1'" || exit $?
