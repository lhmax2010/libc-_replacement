#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
SOURCE="$REPO/codes/llvm"
PACKAGING="$REPO/packaging"
CONFIG="$REPO/progress/T3R/config/gbs_llvm.conf"
BUILDCONF=/var/tmp/toolchain-gbs/tizen_unified_standard.conf
ROOT="$REPO/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3"
ARTIFACT_DIR="$REPO/artifacts/rebuild_2218/armv7l"
OUT="$REPO/progress/R6"
RAW="$OUT/raw"
LEDGER="$OUT/commands/06_armv7l_git_build_preflight.log"
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

cd "$SOURCE" || exit 125
overall=0
run_record build_pwd pwd || overall=1
run_record assert_source_pwd test "$PWD" = "$SOURCE" || overall=1
run_record assert_fresh_root test ! -e "$ROOT" || overall=1
run_record source_head git rev-parse HEAD || overall=1
run_record source_branch git branch --show-current || overall=1
run_record source_status git status --porcelain=v1 || overall=1
run_record assert_source_clean bash -o pipefail -c 'test -z "$(git status --porcelain=v1)"' || overall=1
run_record assert_backport_head test "$(git rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b || overall=1
run_record assert_tizen_base test "$(git branch --show-current)" = tizen_base || overall=1
run_record assert_backport_one_file bash -o pipefail -c 'test "$(git diff-tree --no-commit-id --name-only -r HEAD | wc -l)" -eq 1 && test "$(git diff-tree --no-commit-id --name-only -r HEAD)" = libcxxabi/src/cxa_personality.cpp' || overall=1
run_record project_packaging_files find "$PACKAGING" -maxdepth 1 -type f -printf '%f\n' || overall=1
run_record assert_one_runtime_spec bash -o pipefail -c 'test "$(find "$1" -maxdepth 1 -type f -name "*.spec" | wc -l)" -eq 1 && grep -Eq "^Name:[[:space:]]+libcxx-runtimes[[:space:]]*$" "$1/libcxx-runtimes.spec"' _ "$PACKAGING" || overall=1
run_record assert_packaging_hashes bash -o pipefail -c 'cd "$1" && sha256sum -c - <<"EOF"
a8437cca668ee785906ba5349efcd5b4b91a049ffd4b53e5234a6c2c6066af74  libcxx-runtimes.spec
e806ab237e86bbe1e1d25c9bedbb129016ed67e9e72acb9259154b4b51a1304d  libcxx-llvm22.map
db262cd0f01ca668a019b79bf1bb1a9731a100205ff4d4b256d071b3d5304dc3  libcxx-runtimes.manifest
1ab73051dc45ee72e681ec0393889bdf527c4d7407df13eb3effa79c7dd4be74  SOURCE_PROVENANCE
EOF' _ "$PACKAGING" || overall=1
run_record assert_config_hashes bash -o pipefail -c 'printf "%s  %s\n%s  %s\n" "8fab16c1ca9db28d45f6b269881af47a71ef3d111d93527b602f57e1e37d49cf" "$1" "9eac0e23968b9bd7dabd463d400f0281d2464815fc2299431052f7b8cfb736a5" "$2" | sha256sum -c -' _ "$CONFIG" "$BUILDCONF" || overall=1
run_record assert_no_archived_arm_rpm bash -o pipefail -c 'test -z "$(find "$1" -maxdepth 1 -type f -name "*.rpm" -print -quit)"' _ "$ARTIFACT_DIR" || overall=1
run_record available_space_gb df -BG "$REPO" || overall=1
run_record assert_space_guard bash -o pipefail -c 'test "$(df -Pk "$1" | awk "NR==2 {print \\$4}")" -ge 83886080' _ "$REPO" || overall=1

{
  printf 'PRIOR_T3R2_WORKING_DIRECTORY=%s\n' /var/tmp/toolchain-gbs/T3R-obs-input-89bc0b8/libcxx-runtimes
  printf 'PRIOR_T3R2_SOURCE_MODE=preexported_platform_tarball\n'
  printf 'PRIOR_T3R2_STYLE=obs\n'
  printf 'CURRENT_WORKING_DIRECTORY=%s\n' "$SOURCE"
  printf 'CURRENT_SOURCE_MODE=git_worktree_HEAD\n'
  printf 'CURRENT_SOURCE_COMMIT=%s\n' "$(git rev-parse HEAD)"
  printf 'CURRENT_STYLE=git\n'
  printf 'CURRENT_INCLUDE_ALL_REASON=external_read_only_packaging_directory_is_not_tracked_in_codes_llvm\n'
  printf 'CURRENT_NO_PATCH_EXPORT_REASON=create_single_native_Source0_archive_from_selected_git_worktree\n'
  printf 'UNCHANGED_SPEC_SHA256=%s\n' a8437cca668ee785906ba5349efcd5b4b91a049ffd4b53e5234a6c2c6066af74
  printf 'UNCHANGED_GBS_CONF_SHA256=%s\n' 8fab16c1ca9db28d45f6b269881af47a71ef3d111d93527b602f57e1e37d49cf
  printf 'UNCHANGED_BUILDCONF_SHA256=%s\n' 9eac0e23968b9bd7dabd463d400f0281d2464815fc2299431052f7b8cfb736a5
} >"$OUT/build_input_delta.tsv"

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
