#!/usr/bin/env bash
set -u

OUT_ROOT=/home/toolchain/development/libc++_replacement/progress/R6
RAW_ROOT="$OUT_ROOT/raw"
LEDGER="$OUT_ROOT/commands/05_gbs_git_context_research.log"
: >"$LEDGER"

run_record() {
  local label=$1
  shift
  {
    printf 'LABEL=%s\n' "$label"
    printf 'PWD=%s\n' "$PWD"
    printf 'COMMAND='
    printf '%q ' "$@"
    printf '\n'
  } >>"$LEDGER"
  "$@" >"$RAW_ROOT/$label.stdout.txt" 2>"$RAW_ROOT/$label.stderr.txt"
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

overall=0
run_record gbs_version gbs --version || overall=1
run_record gbs_build_help gbs build --help || overall=1
run_record gbs_rpm_files rpm -ql gbs || overall=1

GBS_PYTHON_ROOT=$(rpm -ql gbs | awk '/site-packages\/gbs$/ {print; exit}')
if [[ -z "$GBS_PYTHON_ROOT" ]]; then
  GBS_PYTHON_ROOT=$(rpm -ql gbs | awk '/site-packages\/gbs\/build\.py$/ {sub("/build.py$", ""); print; exit}')
fi
printf 'GBS_PYTHON_ROOT=%s\n' "$GBS_PYTHON_ROOT" >"$RAW_ROOT/gbs_python_root.txt"
if [[ -z "$GBS_PYTHON_ROOT" || ! -d "$GBS_PYTHON_ROOT" ]]; then
  printf 'EXIT_CODE=1\n' >>"$RAW_ROOT/gbs_python_root.txt"
  overall=1
else
  printf 'EXIT_CODE=0\n' >>"$RAW_ROOT/gbs_python_root.txt"
  run_record gbs_packaging_dir_references rg -n -C 5 -- 'packaging[_-]dir|not.export.source|no.patch.export|export.only|worktree' "$GBS_PYTHON_ROOT" || {
    rc=$?
    if [[ $rc -ne 1 ]]; then overall=1; fi
  }
  run_record gbs_build_source_excerpt sed -n '1,360p' "$GBS_PYTHON_ROOT/build.py" || overall=1
fi

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
