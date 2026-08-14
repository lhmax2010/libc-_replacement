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

push_file() {
    local source=$1
    local target=$2
    printf 'SDB_PUSH_COMMAND='
    printf '%q ' "$sdb" -s "$serial" push "$source" "$target"
    printf '\n'
    "$sdb" -s "$serial" push "$source" "$target"
    local rc=$?
    printf 'SDB_PUSH_EXIT_CODE=%d\n' "$rc"
    return "$rc"
}

run_remote "test ! -e '$remote' && mkdir '$remote' && mkdir '$remote/patched' '$remote/unpatched' && chmod 700 '$remote' '$remote/patched' '$remote/unpatched'" || exit $?
push_file "$local_dir/s1_libcxx_strict" "$remote/s1_libcxx_strict" || exit $?
push_file "$local_dir/s1_libstdcxx_strict" "$remote/s1_libstdcxx_strict" || exit $?
push_file "$local_dir/patched/libc++abi.so.1.0" "$remote/patched/libc++abi.so.1" || exit $?
push_file "$local_dir/unpatched/libc++abi.so.1.0" "$remote/unpatched/libc++abi.so.1" || exit $?
run_remote "chmod 700 '$remote/s1_libcxx_strict' '$remote/s1_libstdcxx_strict' '$remote/patched/libc++abi.so.1' '$remote/unpatched/libc++abi.so.1' && find '$remote' -maxdepth 2 -ls && sha256sum '$remote/s1_libcxx_strict' '$remote/s1_libstdcxx_strict' '$remote/patched/libc++abi.so.1' '$remote/unpatched/libc++abi.so.1'" || exit $?
