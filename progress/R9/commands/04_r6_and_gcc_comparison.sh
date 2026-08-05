#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
R6="$ROOT/progress/R6"
GCC="$ROOT/tmp/corpus/extracted/gcc-14.2.0-1.14.src.rpm/unpacked/gcc-14.2.0.tar.bz2/gcc-14.2.0/packaging"
LLVM="$ROOT/codes/llvm"

record() {
    local command="$1"
    printf 'COMMAND=%s\n' "$command"
    (cd "$ROOT" && bash -o pipefail -c "$command")
    local rc=$?
    printf 'EXIT_CODE=%d\n\n' "$rc"
    return 0
}

record "nl -ba '$R6/build_armv7l_git_r3b.full.log' | sed -n '1,45p'"
record "nl -ba '$R6/build_input_delta.tsv'"
record "nl -ba '$R6/raw/exported_source_sha256.stdout.txt'"
record "nl -ba '$R6/repair_validation_report.md' | sed -n '72,92p'"
record "nl -ba '$ROOT/progress/T3R/build_commands.tsv'"

record "test -d '$GCC'"
record "find '$GCC' -maxdepth 1 -type f -printf '%f\\n' | LC_ALL=C sort"
record "nl -ba '$GCC/pre_checkin.sh'"
record "rg -n '^Name:' '$GCC'/gcc*.spec"
record "for f in '$GCC'/gcc-{armv7l,armv7hl,aarch64,riscv64}.spec; do echo FILE:\$f; sed -n '1,4p' \"\$f\" | nl -ba; done"

record "find '$LLVM/packaging' -maxdepth 1 -type f -printf '%f\\n' | LC_ALL=C sort"
record "find '$LLVM/packaging' -maxdepth 1 -type f \\( -name 'pre_checkin.sh' -o -name '*.spec' \\) -printf '%f\\n' | LC_ALL=C sort"
record "rg -n '^Name:' '$LLVM/packaging'/*.spec"
record "git -C '$LLVM' status --porcelain=v1"
