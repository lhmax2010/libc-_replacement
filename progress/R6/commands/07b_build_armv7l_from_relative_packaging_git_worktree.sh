#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
SOURCE="$REPO/tmp/R6/gbs-source-worktree-r3b"
REL_PACKAGING=r6-packaging
CONFIG="$REPO/progress/T3R/config/gbs_llvm.conf"
ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b"
OUT="$REPO/progress/R6"
LOG="$OUT/build_armv7l_git_r3b.full.log"
STATUS="$OUT/build_armv7l_git_r3b.status.tsv"

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
if [[ $(git status --porcelain=v1) != '?? r6-packaging/' ]]; then
  {
    printf 'WORKING_DIRECTORY=%s\n' "$PWD"
    printf 'ASSERT_ONLY_UNTRACKED_PACKAGING_COPY=FAIL\n'
    git status --porcelain=v1
    printf 'EXIT_CODE=92\n'
  } >"$LOG"
  exit 92
fi
if ! git diff --exit-code >/dev/null; then
  {
    printf 'WORKING_DIRECTORY=%s\n' "$PWD"
    printf 'ASSERT_NO_TRACKED_SOURCE_DIFF=FAIL\n'
    git diff
    printf 'EXIT_CODE=93\n'
  } >"$LOG"
  exit 93
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
  --packaging-dir "$REL_PACKAGING"
  --spec libcxx-runtimes.spec
  "$SOURCE"
)

{
  printf 'WORKING_DIRECTORY=%s\n' "$PWD"
  printf 'SOURCE_MODE=DISPOSABLE_GIT_WORKTREE\n'
  printf 'SOURCE_COMMIT=%s\n' "$(git rev-parse HEAD)"
  printf 'PACKAGING_DIRECTORY=%s\n' "$REL_PACKAGING"
  printf 'PACKAGING_MODE=RELATIVE_BYTE_IDENTICAL_COPY_IN_DISPOSABLE_WORKTREE\n'
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
