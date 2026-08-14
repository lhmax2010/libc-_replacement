#!/bin/bash
set -u

sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r38_s1_20260814
command="set +e; if test -e '$remote'; then printf 'DEPLOY_PATH_PRESENT=YES\\n'; find '$remote' -maxdepth 3 -ls; path_rc=1; else printf 'DEPLOY_PATH_PRESENT=NO\\n'; path_rc=0; fi; residual=0; printf 'RESIDUAL_PROCESS_SCAN_BEGIN\\n'; for proc in /proc/[0-9]*; do pid=\${proc##*/}; test \"\$pid\" = \"\$\$\" && continue; test \"\$pid\" = \"\$PPID\" && continue; cmdline=\$(tr '\\000' ' ' < \"\$proc/cmdline\" 2>/dev/null); case \"\$cmdline\" in *r38_s1_20260814*|*s1_libcxx_strict*|*s1_libstdcxx_strict*) printf 'PID=%s CMD=%s\\n' \"\$pid\" \"\$cmdline\"; residual=1;; esac; done; printf 'RESIDUAL_PROCESS_SCAN_END\\nRESIDUAL_PROCESS_COUNT_NONZERO=%d\\n' \"\$residual\"; test \"\$path_rc\" -eq 0 -a \"\$residual\" -eq 0"
printf 'SDB_COMMAND='
printf '%q ' "$sdb" -s "$serial" shell "$command"
printf '\n'
"$sdb" -s "$serial" shell "$command"
rc=$?
printf 'SDB_EXIT_CODE=%d\n' "$rc"
exit "$rc"
