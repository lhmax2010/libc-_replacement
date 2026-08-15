#!/bin/bash
set -u
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r41_s1_20260815
cmd="set +e; printf 'PRE_CLEAN_PROCESS_MATCHES_BEGIN\\n'; pre_count=0; for proc in /proc/[0-9]*; do pid=\${proc##*/}; test \"\$pid\" = \"\$\$\" && continue; test \"\$pid\" = \"\$PPID\" && continue; line=\$(tr '\\000' ' ' < \"\$proc/cmdline\" 2>/dev/null); case \"\$line\" in *r41_s1_20260815*|*s1_libcxx*) printf 'PID=%s CMD=%s\\n' \"\$pid\" \"\$line\"; pre_count=\$((pre_count+1));; esac; done; printf 'PRE_CLEAN_PROCESS_COUNT=%d\\nPRE_CLEAN_PROCESS_MATCHES_END\\n' \"\$pre_count\"; rm -rf '$remote'; rm_rc=\$?; printf 'RM_EXIT_CODE=%d\\n' \"\$rm_rc\"; if test -e '$remote'; then printf 'FILE_DELETE_VERIFICATION=PRESENT\\n'; file_rc=1; else printf 'FILE_DELETE_VERIFICATION=ABSENT\\n'; file_rc=0; fi; printf 'POST_CLEAN_PROCESS_MATCHES_BEGIN\\n'; post_count=0; for proc in /proc/[0-9]*; do pid=\${proc##*/}; test \"\$pid\" = \"\$\$\" && continue; test \"\$pid\" = \"\$PPID\" && continue; line=\$(tr '\\000' ' ' < \"\$proc/cmdline\" 2>/dev/null); case \"\$line\" in *r41_s1_20260815*|*s1_libcxx*) printf 'PID=%s CMD=%s\\n' \"\$pid\" \"\$line\"; post_count=\$((post_count+1));; esac; done; printf 'POST_CLEAN_PROCESS_COUNT=%d\\nPOST_CLEAN_PROCESS_MATCHES_END\\n' \"\$post_count\"; test \"\$rm_rc\" -eq 0 -a \"\$file_rc\" -eq 0 -a \"\$post_count\" -eq 0"
printf 'SDB_COMMAND='; printf '%q ' "$sdb" -s "$serial" shell "$cmd"; printf '\n'
"$sdb" -s "$serial" shell "$cmd"
