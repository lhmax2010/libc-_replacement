#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
SOURCE="$REPO/codes/llvm"
PACKAGING="$REPO/packaging"
CONFIG="$REPO/progress/T3R/config/gbs_llvm.conf"
ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3"
OUT="$REPO/progress/R6"
LOG="$OUT/build_armv7l_git.full.log"
STATUS="$OUT/build_armv7l_git.status.tsv"

cd "$SOURCE" || exit 125
if [[ -e "$ROOT" ]]; then
  {
    printf 'WORKING_DIRECTORY=%s\n' "$PWD"
    printf 'ASSERT_FRESH_ROOT=FAIL\n'
    printf 'ROOT=%s\n' "$ROOT"
    printf 'EXIT_CODE=91\n'
  } >"$LOG"
  exit 91
fi
if [[ -n $(git status --porcelain=v1) ]]; then
  {
    printf 'WORKING_DIRECTORY=%s\n' "$PWD"
    printf 'ASSERT_CLEAN_GIT_WORKTREE=FAIL\n'
    git status --porcelain=v1
    printf 'EXIT_CODE=92\n'
  } >"$LOG"
  exit 92
fi

BUILD_COMMAND=(
  gbs -c "$CONFIG" build
  -A armv7l
  -B "$ROOT"
  -C
  --debug
  --style git
  --include-all
  --no-patch-export
  --packaging-dir "$PACKAGING"
  --spec libcxx-runtimes.spec
  "$SOURCE"
)

{
  printf 'WORKING_DIRECTORY=%s\n' "$PWD"
  printf 'SOURCE_MODE=GIT_WORKTREE\n'
  printf 'SOURCE_BRANCH=%s\n' "$(git branch --show-current)"
  printf 'SOURCE_COMMIT=%s\n' "$(git rev-parse HEAD)"
  printf 'PACKAGING_DIRECTORY=%s\n' "$PACKAGING"
  printf 'PACKAGING_MODE=EXTERNAL_UNCHANGED_PROJECT_INPUT\n'
  printf 'BUILDROOT=%s\n' "$ROOT"
  printf 'COMMAND_BEGIN\n'
  printf '%q ' "${BUILD_COMMAND[@]}"
  printf '\nCOMMAND_END\n'
} >"$LOG"

"${BUILD_COMMAND[@]}" 2>&1 | tee -a "$LOG"
rc=${PIPESTATUS[0]}
printf 'EXIT_CODE=%s\n' "$rc" | tee -a "$LOG"

{
  printf 'ARCH=armv7l\n'
  printf 'SOURCE_COMMIT=%s\n' "$(git rev-parse HEAD)"
  printf 'ROOT=%s\n' "$ROOT"
  printf 'GBS_EXIT_CODE=%s\n' "$rc"
} >"$STATUS"

if [[ -d "$ROOT" ]]; then
  {
    printf 'RETIRED_SINGLE_USE\n'
    printf 'arch=armv7l\n'
    printf 'source_commit=%s\n' "$(git rev-parse HEAD)"
    printf 'gbs_exit_code=%s\n' "$rc"
    printf 'root=%s\n' "$ROOT"
  } >"$ROOT/RETIRED_SINGLE_USE"
fi

exit "$rc"
