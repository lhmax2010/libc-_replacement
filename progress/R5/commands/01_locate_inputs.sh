#!/usr/bin/env bash
set -u

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R5/raw"

run_recorded() {
    local id=$1
    shift
    local log="$out/${id}.log"
    local rc
    {
        printf 'WORKING_DIRECTORY=%s\n' "$PWD"
        printf 'COMMAND_BEGIN\n'
        printf '%q ' "$@"
        printf '\nCOMMAND_END\n'
    } >"$log"
    "$@" >>"$log" 2>&1
    rc=$?
    printf 'EXIT_CODE=%s\n' "$rc" >>"$log"
    printf '%s\t%s\n' "$id" "$rc"
    return 0
}

cd "$repo" || exit 125
run_recorded 01_pwd pwd
run_recorded 02_git_status git status --short --branch
run_recorded 03_git_head git rev-parse HEAD
run_recorded 04_home_dirs find /home/toolchain -maxdepth 2 -mindepth 1 -type d -printf '%p\n'
run_recorded 05_personality_candidates bash -o pipefail -c \
    "rg --files /home/toolchain/development/libc++_replacement /home/toolchain/plan_evaluation /home/toolchain/libc++_replacement_bak 2>/dev/null | rg '/libcxxabi/src/cxa_personality\\.cpp$'"
run_recorded 06_llvm_version_files bash -o pipefail -c \
    "rg --files /home/toolchain/development/libc++_replacement /home/toolchain/plan_evaluation /home/toolchain/libc++_replacement_bak 2>/dev/null | rg '/LLVMVersion\\.cmake$'"
run_recorded 07_arm_roots bash -o pipefail -c \
    "find '$repo/tmp/GBS-ROOT' /home/toolchain/libc++_replacement_bak -type d \\( -name 'scratch.armv7l.0' -o -name 'scratch.armv7l.*' -o -name '*armv7l*' \\) -print 2>/dev/null | LC_ALL=C sort"
run_recorded 08_211_logs bash -o pipefail -c \
    "rg --files /home/toolchain/plan_evaluation /home/toolchain/libc++_replacement_bak '$repo' 2>/dev/null | rg '(21\\.1\\.1|211|armv7l|stageA).*(log|txt)$|/(log|logs)/.*(libcxx|runtimes)' | LC_ALL=C sort"
run_recorded 09_t0_path_mentions bash -o pipefail -c \
    "rg -n -S 'T0|21\\.1\\.1.*source|cxa_personality|libcxx-runtimes-21\\.1\\.1|LIBCXX-211' docs progress /home/toolchain/plan_evaluation/p1/libcxx_packaging 2>/dev/null | head -n 2000"
run_recorded 10_disk df -h "$repo" /home/toolchain/libc++_replacement_bak /home/toolchain/plan_evaluation
exit 0
