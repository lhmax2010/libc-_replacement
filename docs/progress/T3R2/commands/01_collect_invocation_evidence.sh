#!/usr/bin/env bash
set -u

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/T3R2/raw"
mkdir -p "$out"

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

run_recorded 01_pwd pwd
run_recorded 02_git_status git status --short --branch
run_recorded 03_git_branch git branch --show-current
run_recorded 04_git_head git rev-parse HEAD
run_recorded 05_recent_history git log --oneline --decorate -8
run_recorded 06_input_tree find /var/tmp/toolchain-gbs/T3R-obs-input-89bc0b8/libcxx-runtimes -maxdepth 3 -type f -printf '%p\n'
run_recorded 07_input_hashes sha256sum \
    /var/tmp/toolchain-gbs/T3R-obs-input-89bc0b8/libcxx-runtimes/packaging/libcxx-runtimes.spec \
    /var/tmp/toolchain-gbs/T3R-obs-input-89bc0b8/libcxx-runtimes/packaging/libcxx-llvm22.map \
    /var/tmp/toolchain-gbs/T3R-obs-input-89bc0b8/libcxx-runtimes/packaging/libcxx-runtimes.manifest \
    /var/tmp/toolchain-gbs/T3R-obs-input-89bc0b8/libcxx-runtimes/packaging/SOURCE_PROVENANCE \
    /var/tmp/toolchain-gbs/T3R-obs-input-89bc0b8/libcxx-runtimes/llvm-22.1.8.tar.gz
run_recorded 08_config_hashes sha256sum \
    "$repo/progress/T3R/config/gbs_llvm.conf" \
    /var/tmp/toolchain-gbs/tizen_unified_standard.conf
run_recorded 09_x86_context bash -o pipefail -c \
    "grep -nE '^(WORKING_DIRECTORY|COMMAND_BEGIN|gbs |COMMAND_END)|depanneur.*--path=|start building packages from:|Checking dependencies for libcxx-runtimes|circle in [0-9]+ packages|/usr/bin/build' progress/T3R/build_x86_64.full.log"
run_recorded 10_arm_context bash -o pipefail -c \
    "grep -nE '^(WORKING_DIRECTORY|COMMAND_BEGIN|gbs |COMMAND_END)|depanneur.*--path=|start building packages from:|Checking dependencies for libcxx-runtimes|circle found in|circle in [0-9]+ packages|/usr/bin/build' progress/T3R/build_armv7l.full.log"
run_recorded 11_aarch_context bash -o pipefail -c \
    "grep -nE '^(WORKING_DIRECTORY|COMMAND_BEGIN|gbs |COMMAND_END)|depanneur.*--path=|start building packages from:|Checking dependencies for libcxx-runtimes|circle found in|circle in [0-9]+ packages|/usr/bin/build' progress/T3R/build_aarch64.full.log"
run_recorded 12_wrapper_current sed -n 1,240p progress/T3R/commands/run_build_arch.sh
run_recorded 13_wrapper_history git log --format=fuller -p -- progress/T3R/commands/run_build_arch.sh
run_recorded 14_existing_roots bash -o pipefail -c \
    "find tmp/GBS-ROOT -maxdepth 2 -type f -name RETIRED_SINGLE_USE -o -maxdepth 1 -type d -name 'LIBCXX-2218-*' | sort"
run_recorded 15_disk df -h "$repo" /var/tmp/toolchain-gbs
run_recorded 16_external_not_git git -C /var/tmp/toolchain-gbs/T3R-obs-input-89bc0b8/libcxx-runtimes rev-parse --show-toplevel

exit 0
