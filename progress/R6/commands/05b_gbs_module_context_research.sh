#!/usr/bin/env bash
set -u

OUT_ROOT=/home/toolchain/development/libc++_replacement/progress/R6
RAW_ROOT="$OUT_ROOT/raw"
LEDGER="$OUT_ROOT/commands/05b_gbs_module_context_research.log"
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
run_record gitbuildsys_location_system_python /usr/bin/python3 -c 'import os, gitbuildsys; print(os.path.dirname(gitbuildsys.__file__))' || overall=1
GBS_PYTHON_ROOT=$(cat "$RAW_ROOT/gitbuildsys_location_system_python.stdout.txt" 2>/dev/null || true)
printf 'GBS_PYTHON_ROOT=%s\n' "$GBS_PYTHON_ROOT" >"$RAW_ROOT/gbs_python_root.actual.txt"
if [[ -z "$GBS_PYTHON_ROOT" || ! -d "$GBS_PYTHON_ROOT" ]]; then
  printf 'EXIT_CODE=1\n' >>"$RAW_ROOT/gbs_python_root.actual.txt"
  overall=1
else
  printf 'EXIT_CODE=0\n' >>"$RAW_ROOT/gbs_python_root.actual.txt"
  run_record gitbuildsys_relevant_references rg -n -C 6 -- 'packaging[_-]dir|not.export.source|no.patch.export|export.only|worktree' "$GBS_PYTHON_ROOT" || {
    rc=$?
    if [[ $rc -ne 1 ]]; then overall=1; fi
  }
  run_record gitbuildsys_files find "$GBS_PYTHON_ROOT" -maxdepth 2 -type f -print || overall=1
fi

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
