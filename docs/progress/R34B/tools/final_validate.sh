#!/bin/bash
set -u

repo=/home/toolchain/development/libc++_replacement
progress="$repo/progress/R34B"
curated="$repo/docs/progress/R34B"

run() {
    printf '$'
    printf ' %q' "$@"
    printf '\n'
    "$@"
    rc=$?
    printf '[exit_code] %d\n' "$rc"
    return "$rc"
}

run_shell() {
    printf '$ %s\n' "$1"
    bash -o pipefail -c "$1"
    rc=$?
    printf '[exit_code] %d\n' "$rc"
    return "$rc"
}

printf '[note] 先前一次 sha256sum -c 从仓根执行，因清单路径相对各自目录而退出非零；以下使用正确目录，并在最终清单生成后再次复核。\n'
run cmp "$repo/codes/llvm/libcxxabi/include/cxxabi.h" \
    "$curated/code/implementation/baseline/libcxxabi/include/cxxabi.h"
run cmp "$repo/codes/llvm/libcxxabi/src/cxa_personality.cpp" \
    "$curated/code/implementation/baseline/libcxxabi/src/cxa_personality.cpp"
run cmp "$repo/codes/llvm/libcxxabi/src/cxa_exception.cpp" \
    "$curated/code/implementation/baseline/libcxxabi/src/cxa_exception.cpp"
run cmp "$repo/tmp/R34B/llvm-exp/libcxxabi/include/cxxabi.h" \
    "$curated/code/implementation/current/libcxxabi/include/cxxabi.h"
run cmp "$repo/tmp/R34B/llvm-exp/libcxxabi/src/cxa_personality.cpp" \
    "$curated/code/implementation/current/libcxxabi/src/cxa_personality.cpp"
run cmp "$repo/tmp/R34B/llvm-exp/libcxxabi/src/cxa_exception.cpp" \
    "$curated/code/implementation/current/libcxxabi/src/cxa_exception.cpp"
run_shell "test \$(find '$progress/results/matrix' -type f -name '*.log' | wc -l) -eq 12"
run_shell "awk -F '\\t' 'NR > 1 && (\$3 != 0 || \$4 != 0 || \$7 != \"PASS\") {bad=1} END {exit bad}' '$progress/tables/matrix_results.tsv'"
run_shell "grep -F 'tmp/R34B/runtime/libc++abi.so.1' '$progress/commands/017_runtime_identity.log'"
run_shell "test -z \"\$(git -C '$repo/codes/llvm' status --porcelain)\""
run_shell "test -z \"\$(find '$curated' -type f -size +5M -print -quit)\""
run_shell "test \$(find '$curated/code/implementation/current' -type f | wc -l) -eq 3"
run_shell "test \$(find '$curated/code/implementation/baseline' -type f | wc -l) -eq 3"
