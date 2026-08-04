#!/usr/bin/env bash
set -u

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/T3R2/raw"

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
    return "$rc"
}

cd "$repo" || exit 125
run_recorded 17_context_mentions rg -n -S 'WORKING_DIRECTORY|workdir|working directory|工作目录|obs-input|circle found in 389|circle found in 392' progress/T3R docs/progress/T3R
run_recorded 18_prior_report sed -n 1,280p docs/progress/T3R/T3R_report.md
run_recorded 19_prior_commands find progress/T3R/commands -maxdepth 1 -type f -printf '%f\n'
run_recorded 20_prior_command_files bash -o pipefail -c 'for f in progress/T3R/commands/*; do printf "FILE=%s\n" "$f"; sed -n "1,260p" "$f"; done'
run_recorded 21_source_locations find /var/tmp/toolchain-gbs /home/toolchain/development/libc++_replacement -xdev -type f -name llvm-22.1.8.tar.gz -printf '%p\n'
run_recorded 22_source_cache_locations find /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260804 -type f -name llvm-22.1.8.tar.gz -printf '%p\n'
run_recorded 23_external_listing find /var/tmp/toolchain-gbs/T3R-obs-input-89bc0b8/libcxx-runtimes -maxdepth 4 -printf '%y\t%p\t%l\n'
run_recorded 24_spec_sources bash -o pipefail -c "grep -nE '^Source[0-9]*:' /var/tmp/toolchain-gbs/T3R-obs-input-89bc0b8/libcxx-runtimes/packaging/libcxx-runtimes.spec"
exit 0
