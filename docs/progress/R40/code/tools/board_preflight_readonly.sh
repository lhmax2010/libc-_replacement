#!/bin/sh
set -u

printf 'BOARD_PREFLIGHT_BEGIN\n'
date -u +UTC=%Y-%m-%dT%H:%M:%SZ
uname -a
printf 'PROJECT_PROCESS_SCAN_BEGIN\n'
for p in /proc/[0-9]*; do
  exe=$(readlink "$p/exe" 2>/dev/null) || continue
  case "$exe" in
    *r36*|*r38*|*r39*|*r40*|*s1_*) printf '%s %s\n' "${p#/proc/}" "$exe" ;;
  esac
done
printf 'PROJECT_PROCESS_SCAN_END\n'
printf 'BOARD_PREFLIGHT_END\n'
