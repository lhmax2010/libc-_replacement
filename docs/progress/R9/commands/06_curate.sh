#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
SRC="$ROOT/progress/R9"
DST="$ROOT/docs/progress/R9"

record() {
    local command="$1"
    printf 'COMMAND=%s\n' "$command"
    (cd "$ROOT" && bash -o pipefail -c "$command")
    local rc=$?
    printf 'EXIT_CODE=%d\n\n' "$rc"
    return 0
}

record "if test -e '$DST'; then find '$DST' -mindepth 1 -print -quit | grep -q . && exit 17 || true; fi"
record "mkdir -p '$DST'"
record "cp -a '$SRC'/R9_report.md '$SRC'/*.tsv '$SRC'/search_scope.md '$SRC'/commands '$SRC'/generated '$DST'/"
record "diff -qr --exclude='06_curate.log' '$SRC' '$DST'"
