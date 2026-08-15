#!/bin/bash
set -u

printf 'COMMAND=uname -a\n'; uname -a
printf 'COMMAND=uname -m\n'; uname -m
printf 'COMMAND=ps -ef\n'
ps -ef
rc=$?
printf 'PS_EXIT_CODE=%d\n' "$rc"
printf 'PROJECT_PROCESS_MATCHES_BEGIN\n'
ps -ef | grep -E '[r]41|[r]40|[s]1_(libcxx|libstdcxx)|[r]36|[r]38|[r]39'
match_rc=$?
printf 'PROJECT_PROCESS_GREP_EXIT_CODE=%d\n' "$match_rc"
printf 'PROJECT_PROCESS_MATCHES_END\n'
if [ "$match_rc" -eq 0 ]; then
  printf 'BOARD_PREFLIGHT_RESULT=RED_STOP_PROJECT_PROCESS_PRESENT\n'
  exit 20
fi
printf 'BOARD_PREFLIGHT_RESULT=PASS\n'

