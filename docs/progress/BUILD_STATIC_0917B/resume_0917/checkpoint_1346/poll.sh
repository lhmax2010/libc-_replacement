#!/usr/bin/env bash
set -u
root=progress/BUILD_STATIC_0917B/resume_0917
stamp=$(date +%s)
bash "$root/run_logged.sh" "poll_$stamp" bash -c 'tail -n 4 progress/BUILD_STATIC_0917B/resume_0917/llvm-aarch64.log; tail -n 1 progress/BUILD_STATIC_0917B/resume_0917/events.jsonl; date --iso-8601=seconds'
rc=$?
sed -n '1,30p' "$root/raw/poll_$stamp.stdout.txt"
exit "$rc"
