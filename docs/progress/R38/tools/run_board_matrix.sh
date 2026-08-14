#!/bin/bash
set -u

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r38_s1_20260814

run_case() {
    local implementation=$1
    local mode=$2
    local command
    if [ "$implementation" = patched_libcxxabi ]; then
        command="LD_LIBRARY_PATH='$remote/patched' '$remote/s1_libcxx_strict' '$mode'"
    elif [ "$implementation" = unpatched_libcxxabi ]; then
        command="LD_LIBRARY_PATH='$remote/unpatched' '$remote/s1_libcxx_strict' '$mode'"
    else
        command="'$remote/s1_libstdcxx_strict' '$mode'"
    fi
    printf '=== CASE implementation=%s mode=%s ===\n' "$implementation" "$mode"
    printf 'REMOTE_TEST_COMMAND=%s\n' "$command"
    "$sdb" -s "$serial" shell "set +e; $command; case_rc=\$?; printf 'REMOTE_CASE_EXIT_CODE=%d\\n' \"\$case_rc\"; if test \"\$case_rc\" -gt 128; then printf 'REMOTE_CASE_SIGNAL=%d\\n' \"\$((case_rc - 128))\"; else printf 'REMOTE_CASE_SIGNAL=0\\n'; fi; exit 0"
    local carrier_rc=$?
    printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier_rc"
    if [ "$carrier_rc" -ne 0 ]; then
        printf 'SDB_CONNECTION_FAILURE=YES\n'
        return "$carrier_rc"
    fi
    return 0
}

for implementation in patched_libcxxabi libstdcxx; do
    for mode in cancel exit real foreign cancel-no-catch exit-no-catch; do
        run_case "$implementation" "$mode" || exit $?
    done
done
run_case unpatched_libcxxabi cancel || exit $?
printf 'MATRIX_REQUESTED_CELLS=13\n'
printf 'MATRIX_CARRIER_COMPLETED_CELLS=13\n'
printf 'BOARD_TIMEOUT_COMMAND=NOT_AVAILABLE\n'
printf 'EXECUTION_CARRIER=DIRECT_SDB_SHELL\n'
