#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
tree="${workspace}/tmp/R76/apply-check"
patch_root="${workspace}/progress/R69/patches"
out="${workspace}/progress/R76/raw/apply_preflight"
mkdir -p "${out}"

patches=(
  0001-Layer-A-fix-foreign-forced-unwind-resume.patch
  0002-Layer-B-add-GNU-forced-unwind-marker.patch
  0003-Layer-C-header-batch-20-sites.patch
  0004-Layer-C-library-batch-30-sites.patch
)

record() {
  local name=$1
  shift
  {
    printf 'COMMAND:'
    printf ' %q' "$@"
    printf '\n'
  } > "${out}/${name}.log"
  set +e
  "$@" >> "${out}/${name}.log" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE: %s\n' "${rc}" >> "${out}/${name}.log"
  return "${rc}"
}

record 001_baseline git -C "${tree}" rev-parse HEAD
record 002_clean git -C "${tree}" diff --quiet
record 003_cached_clean git -C "${tree}" diff --cached --quiet
record 004_patch_identity bash -c "cd '${patch_root}' && sha256sum -c SHA256SUMS"

index=10
for patch in "${patches[@]}"; do
  stem=$(printf '%03d' "${index}")
  record "${stem}_${patch%.patch}_check" git -C "${tree}" apply --check "${patch_root}/${patch}"
  index=$((index + 1))
  stem=$(printf '%03d' "${index}")
  record "${stem}_${patch%.patch}_apply" git -C "${tree}" apply --index "${patch_root}/${patch}"
  index=$((index + 1))
done

record 020_diff_check git -C "${tree}" diff --cached --check
record 021_status git -C "${tree}" status --short
record 022_diff_stat git -C "${tree}" diff --cached --stat
record 023_patch_identity_after bash -c "cd '${patch_root}' && sha256sum -c SHA256SUMS"

printf 'R76_APPLY_PREFLIGHT=PASS\n' > "${out}/STATUS"
