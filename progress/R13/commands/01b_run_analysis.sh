#!/bin/bash
set -uo pipefail
analysis_pwd=/home/toolchain/development/libc++_replacement
analysis_cmd=(python3 progress/R13/tools/analyze_closures.py --workspace /home/toolchain/development/libc++_replacement --output /home/toolchain/development/libc++_replacement/progress/R13)
printf 'PWD=%s\n' "$analysis_pwd"
printf 'COMMAND='
printf '%q ' "${analysis_cmd[@]}"
printf '\n'
cd "$analysis_pwd"
"${analysis_cmd[@]}"
analysis_exit=$?
printf 'EXIT_CODE=%s\n' "$analysis_exit"
exit "$analysis_exit"
