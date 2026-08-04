#!/usr/bin/env bash
set -u
set -o pipefail

arch=${1:-}
case "$arch" in
    armv7l|aarch64) ;;
    *) printf 'USAGE: %s armv7l|aarch64\n' "$0" >&2; exit 64 ;;
esac

repo=/home/toolchain/development/libc++_replacement
source_input=/var/tmp/toolchain-gbs/T3R-obs-input-89bc0b8/libcxx-runtimes
config="$repo/progress/T3R/config/gbs_llvm.conf"
buildconf=/var/tmp/toolchain-gbs/tizen_unified_standard.conf
root="$repo/tmp/GBS-ROOT/LIBCXX-2218-${arch}-20260804-r2"
log="$repo/progress/T3R2/build_${arch}.full.log"
preflight="$repo/progress/T3R2/${arch}_preflight.log"
artifact_dir="$repo/artifacts/rebuild_2218/$arch"

: >"$preflight"
record_check() {
    local label=$1
    shift
    local rc
    {
        printf 'WORKING_DIRECTORY=%s\n' "$PWD"
        printf 'COMMAND_BEGIN\n'
        printf '%q ' "$@"
        printf '\nCOMMAND_END\n'
    } >>"$preflight"
    "$@" >>"$preflight" 2>&1
    rc=$?
    printf 'EXIT_CODE=%s\n' "$rc" >>"$preflight"
    printf '%s=%s\n' "$label" "$rc" | tee -a "$preflight"
    return "$rc"
}

cd "$source_input" || exit 125

record_check ASSERT_PWD test "$PWD" = "$source_input" || exit 90
record_check ASSERT_FRESH_ROOT test ! -e "$root" || exit 91
record_check ASSERT_NO_RPM_ALREADY_ARCHIVED bash -o pipefail -c "test -z \"\$(find '$artifact_dir' -maxdepth 1 -type f -name '*.rpm' -print -quit)\"" || exit 92
record_check ASSERT_NOT_GIT_WORKTREE bash -o pipefail -c "! git -C '$source_input' rev-parse --show-toplevel" || exit 93
record_check ASSERT_ONE_SPEC bash -o pipefail -c "test \"\$(find '$source_input/packaging' -maxdepth 1 -type f -name '*.spec' | wc -l)\" -eq 1" || exit 94
record_check ASSERT_PACKAGE_NAME bash -o pipefail -c "grep -Eq '^Name:[[:space:]]+libcxx-runtimes[[:space:]]*$' '$source_input/packaging/libcxx-runtimes.spec'" || exit 95
record_check ASSERT_INPUT_HASHES bash -o pipefail -c \
    "cd '$source_input/packaging' && sha256sum -c - <<'EOF'
a8437cca668ee785906ba5349efcd5b4b91a049ffd4b53e5234a6c2c6066af74  libcxx-runtimes.spec
e806ab237e86bbe1e1d25c9bedbb129016ed67e9e72acb9259154b4b51a1304d  libcxx-llvm22.map
db262cd0f01ca668a019b79bf1bb1a9731a100205ff4d4b256d071b3d5304dc3  libcxx-runtimes.manifest
1ab73051dc45ee72e681ec0393889bdf527c4d7407df13eb3effa79c7dd4be74  SOURCE_PROVENANCE
16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e  llvm-22.1.8.tar.gz
EOF" || exit 96
record_check ASSERT_CONFIG_HASHES bash -o pipefail -c \
    "printf '%s  %s\n%s  %s\n' '8fab16c1ca9db28d45f6b269881af47a71ef3d111d93527b602f57e1e37d49cf' '$config' '9eac0e23968b9bd7dabd463d400f0281d2464815fc2299431052f7b8cfb736a5' '$buildconf' | sha256sum -c -" || exit 97

{
    printf 'WORKING_DIRECTORY=%s\n' "$PWD"
    printf 'COMMAND_BEGIN\n'
    printf '%q ' gbs -c "$config" build -A "$arch" -B "$root" -C --debug --style obs "$source_input"
    printf '\nCOMMAND_END\n'
} >"$log"

gbs -c "$config" build -A "$arch" -B "$root" -C --debug --style obs "$source_input" 2>&1 | tee -a "$log"
rc=${PIPESTATUS[0]}
printf 'EXIT_CODE=%s\n' "$rc" | tee -a "$log"
exit "$rc"
