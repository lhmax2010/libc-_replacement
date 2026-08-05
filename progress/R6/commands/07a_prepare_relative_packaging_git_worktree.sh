#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
SOURCE="$REPO/codes/llvm"
WORKTREE="$REPO/tmp/R6/gbs-source-worktree-r3b"
REL_PACKAGING=r6-packaging
INPUT="$REPO/packaging"
OLD_ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3"
NEW_ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b"
OUT="$REPO/progress/R6"
RAW="$OUT/raw"
LEDGER="$OUT/commands/07a_prepare_relative_packaging_git_worktree.log"
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
  "$@" >"$RAW/$label.stdout.txt" 2>"$RAW/$label.stderr.txt"
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

overall=0
cd "$REPO" || exit 125
run_record assert_first_root_retired test -f "$OLD_ROOT/RETIRED_SINGLE_USE" || overall=1
run_record assert_first_failure_before_build rg -n -- 'No source package found|Can.t stat .*/codes/llvm//home/.*/packaging' "$OUT/build_armv7l_git.full.log" || overall=1
run_record assert_first_no_circle bash -o pipefail -c '! rg -n -- "circle found" "$1"' _ "$OUT/build_armv7l_git.full.log" || overall=1
run_record assert_first_no_compile bash -o pipefail -c '! rg -n -- "cxa_personality.cpp|ninja:|clang\\+\\+" "$1"' _ "$OUT/build_armv7l_git.full.log" || overall=1
run_record assert_new_root_fresh test ! -e "$NEW_ROOT" || overall=1
run_record assert_worktree_path_fresh test ! -e "$WORKTREE" || overall=1
run_record add_detached_build_worktree git -C "$SOURCE" worktree add --detach "$WORKTREE" 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b || overall=1
run_record create_relative_packaging_dir mkdir "$WORKTREE/$REL_PACKAGING" || overall=1
run_record copy_spec cp -a "$INPUT/libcxx-runtimes.spec" "$WORKTREE/$REL_PACKAGING/" || overall=1
run_record copy_map cp -a "$INPUT/libcxx-llvm22.map" "$WORKTREE/$REL_PACKAGING/" || overall=1
run_record copy_manifest cp -a "$INPUT/libcxx-runtimes.manifest" "$WORKTREE/$REL_PACKAGING/" || overall=1
run_record copy_provenance cp -a "$INPUT/SOURCE_PROVENANCE" "$WORKTREE/$REL_PACKAGING/" || overall=1
run_record worktree_identity git -C "$WORKTREE" rev-parse HEAD || overall=1
run_record worktree_status git -C "$WORKTREE" status --porcelain=v1 || overall=1
run_record assert_expected_untracked_packaging bash -o pipefail -c 'test "$(git -C "$1" status --porcelain=v1)" = "?? r6-packaging/"' _ "$WORKTREE" || overall=1
run_record assert_no_tracked_source_diff git -C "$WORKTREE" diff --exit-code || overall=1
run_record assert_relative_packaging_hashes bash -o pipefail -c 'cd "$1" && sha256sum -c - <<"EOF"
a8437cca668ee785906ba5349efcd5b4b91a049ffd4b53e5234a6c2c6066af74  libcxx-runtimes.spec
e806ab237e86bbe1e1d25c9bedbb129016ed67e9e72acb9259154b4b51a1304d  libcxx-llvm22.map
db262cd0f01ca668a019b79bf1bb1a9731a100205ff4d4b256d071b3d5304dc3  libcxx-runtimes.manifest
1ab73051dc45ee72e681ec0393889bdf527c4d7407df13eb3effa79c7dd4be74  SOURCE_PROVENANCE
EOF' _ "$WORKTREE/$REL_PACKAGING" || overall=1
run_record worktree_list git -C "$SOURCE" worktree list --porcelain || overall=1

{
  printf 'FIELD\tFAILED_INVOCATION\tCORRECTED_INVOCATION\tBASIS\n'
  printf 'buildroot\t%s\t%s\told root retired after parser-only failure\n' "$OLD_ROOT" "$NEW_ROOT"
  printf 'git_source\t%s\t%s\tsame commit 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b\n' "$SOURCE" "$WORKTREE"
  printf 'packaging_parameter\t%s\t%s\tdepanneur error showed absolute path concatenated under source path\n' "$INPUT" "$REL_PACKAGING"
  printf 'packaging_content\texternal originals\tbyte-identical disposable copies\tSHA256 verified unchanged\n'
  printf 'style\tgit\tgit\tunchanged\n'
  printf 'spec\tlibcxx-runtimes.spec\tlibcxx-runtimes.spec\tunchanged\n'
  printf 'classification\tABSOLUTE_PACKAGING_DIR_INVOCATION_ERROR\tPENDING_BUILD\tno full scan, no circle, no compiler invocation\n'
} >"$OUT/invocation_retry_basis.tsv"

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
