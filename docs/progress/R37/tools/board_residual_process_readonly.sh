#!/bin/sh
set +e
pid=9448
echo RESIDUAL_PROCESS_BEGIN
ps -ef 2>&1 | awk -v p="$pid" 'NR == 1 || $2 == p || $3 == p'
for f in status cmdline environ cwd root exe fd; do
    path="/proc/$pid/$f"
    echo "PROC_ITEM=$path"
    if test "$f" = cmdline -o "$f" = environ; then
        if test -r "$path"; then tr '\000' '\n' < "$path" 2>&1; else ls -ld "$path" 2>&1; echo NOT_READABLE; fi
    elif test -d "$path"; then
        ls -la "$path" 2>&1
    elif test -r "$path"; then
        cat "$path" 2>&1
    else
        ls -ld "$path" 2>&1
        echo NOT_READABLE
    fi
done
echo RESIDUAL_PROCESS_END
