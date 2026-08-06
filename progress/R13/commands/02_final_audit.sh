#!/bin/bash
set -uo pipefail
audit_pwd=/home/toolchain/development/libc++_replacement
audit_result=0
cd "$audit_pwd"

run_audit_command() {
    audit_label=$1
    shift
    printf 'LABEL=%s\n' "$audit_label"
    printf 'PWD=%s\n' "$PWD"
    printf 'COMMAND='
    printf '%q ' "$@"
    printf '\n'
    "$@"
    audit_exit=$?
    printf 'EXIT_CODE=%s\n\n' "$audit_exit"
    if [ "$audit_exit" -ne 0 ]; then audit_result=1; fi
}

run_audit_command remove_generated_pycache_if_present bash -c 'if [ -d /home/toolchain/development/libc++_replacement/progress/R13/tools/__pycache__ ]; then rm -r /home/toolchain/development/libc++_replacement/progress/R13/tools/__pycache__; fi'
run_audit_command semantic_assertions python3 progress/R13/tools/final_audit.py
run_audit_command protected_tracked_tree_diff git diff --exit-code -- codes packaging artifacts
run_audit_command protected_tree_status git status --porcelain -- codes packaging artifacts
run_audit_command project_status git status --short --branch
run_audit_command largest_r13_files bash -c 'find progress/R13 -type f -printf "%s\t%p\n" | sort -nr | head -20'

printf 'FINAL_EXIT_CODE=%s\n' "$audit_result"
exit "$audit_result"
