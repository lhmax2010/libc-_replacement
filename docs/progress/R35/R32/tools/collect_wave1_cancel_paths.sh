#!/usr/bin/env bash
set -u

base=${1:?workspace root}
cd "$base" || exit 70

run() {
  printf '\nCOMMAND='
  printf '%q ' "$@"
  printf '\n'
  set +e
  "$@"
  rc=$?
  set -e
  printf 'EXIT_CODE=%d\n' "$rc"
}

printf 'SECTION=PROPAGATION_CHAIN_INPUT\n'
run sha256sum progress/R30/tables/forced_unwind_propagation_current.tsv
run sed -n '1,120p' progress/R30/tables/forced_unwind_propagation_current.tsv

for repo in codes/libcynara-commons codes/libcynara-dbus codes/security-manager; do
  printf '\nSECTION=REPO PATH=%s\n' "$repo"
  run git -C "$repo" status --porcelain
  run git -C "$repo" rev-parse HEAD
  run rg -n -C 12 --hidden --no-ignore \
    'pthread_cancel|pthread_create|pthread_setcancelstate|pthread_testcancel|std\s*::\s*thread|__forced_unwind|catch\s*\(\s*\.\s*\.\s*\.' "$repo"
done

printf '\nSECTION=DIRECT_HEADER_CONTEXTS\n'
run sed -n '35,95p' codes/libcynara-commons/src/common/exceptions/TryCatch.h
run sed -n '40,100p' codes/security-manager/src/common/include/utils.h
