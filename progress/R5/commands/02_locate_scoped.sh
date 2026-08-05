#!/usr/bin/env bash
set -u

repo=/home/toolchain/development/libc++_replacement
out="$repo/progress/R5/raw"
src211=/home/toolchain/plan_evaluation/p1/libcxx_packaging/source/libcxx-runtimes-21.1.1
bak=/home/toolchain/development/libc++_replacement_bak
root211=/home/toolchain/GBS-ROOT-LIBCXX-MULTIARCH-armv7l
root2218="$repo/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2"

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
run_recorded 11_known_paths bash -o pipefail -c \
    "for p in '$src211' '$bak' '$root211' '$root2218'; do if test -e \"\$p\"; then printf 'FOUND\\t%s\\n' \"\$p\"; else printf 'NOT_FOUND\\t%s\\n' \"\$p\"; fi; done"
run_recorded 12_source211_top find "$src211" -maxdepth 3 -printf '%y\t%p\t%l\n'
run_recorded 13_source211_personality find "$src211" -path '*/libcxxabi/src/cxa_personality.cpp' -type f -print
run_recorded 14_root2218_personality find "$root2218/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD" -path '*/libcxxabi/src/cxa_personality.cpp' -type f -print
run_recorded 15_root211_personality find "$root211/local/BUILD-ROOTS" -path '*/libcxxabi/src/cxa_personality.cpp' -type f -print
run_recorded 16_root211_logs find "$root211/local" -maxdepth 8 -type f \\( -name 'log.txt' -o -name '*.log' -o -name '_statistics' \\) -printf '%p\n'
run_recorded 17_bak_top find "$bak" -maxdepth 4 -type d -printf '%p\n'
run_recorded 18_bak_stagea_logs bash -o pipefail -c \
    "find '$bak' -path '*/stageA/*' -type f \\( -name '*.log' -o -name 'log.txt' -o -name '*.txt' \\) -printf '%p\\n' | LC_ALL=C sort"
run_recorded 19_bak_211_names bash -o pipefail -c \
    "find '$bak' -type f \\( -iname '*libcxx*21*' -o -iname '*21.1.1*' -o -path '*/libcxx-runtimes-21.1.1/*' \\) -printf '%p\\n' | LC_ALL=C sort"
run_recorded 20_identity_sha bash -o pipefail -c \
    "sha256sum '$src211/libcxxabi/src/cxa_personality.cpp' '$root2218/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/libcxxabi/src/cxa_personality.cpp'"
exit 0
