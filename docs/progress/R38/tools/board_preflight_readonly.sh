#!/bin/sh
set +e

echo BOARD_PREFLIGHT_BEGIN
date -u '+UTC=%Y-%m-%dT%H:%M:%SZ'
uname -a
uname -m
echo TARGET_PIDS_BEGIN
for pid in 9448 9450; do
    if test -d "/proc/$pid"; then
        echo "PID_PRESENT=$pid"
        ps -o pid,ppid,user,state,etime,args -p "$pid" 2>&1
    else
        echo "PID_ABSENT=$pid"
    fi
done
echo TARGET_PIDS_END
echo PROJECT_PROCESS_SCAN_BEGIN
ps -ef 2>&1 | awk 'NR == 1 || /r36|r22|r38|s1_|libc\+\+abi|probe/ { print }'
echo PROJECT_PROCESS_SCAN_END
echo BOARD_PREFLIGHT_END
