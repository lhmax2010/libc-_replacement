#!/usr/bin/env bash
set -euo pipefail

state=/home/toolchain/development/libc++_replacement/progress/R75/raw/033_debug_inventory_process_state.txt
pid=$(awk 'NR==1 {print $1}' "${state}")
[[ "${pid}" =~ ^[0-9]+$ ]]
command=$(ps -p "${pid}" -o args=)
[[ "${command}" == *032_arm_debug_capabilities* ]]
kill -TERM "${pid}"
printf 'stopped_pid=%s\n' "${pid}"
