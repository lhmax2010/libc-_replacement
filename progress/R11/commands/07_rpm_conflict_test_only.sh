#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/07_rpm_conflict_test_only.log"
RAW="$ROOT/progress/R11/raw"
TEST_ROOT="$ROOT/tmp/R11/rpm_conflict_test_only"
mkdir -p "$RAW"
: > "$LOG"

run() {
    local label="$1"
    shift
    local stdout="$RAW/${label}.stdout.txt"
    local stderr="$RAW/${label}.stderr.txt"
    printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD" >> "$LOG"
    printf '%q ' "$@" >> "$LOG"
    printf '\n' >> "$LOG"
    "$@" >"$stdout" 2>"$stderr"
    local rc=$?
    printf 'STDOUT_FILE=%s\nSTDERR_FILE=%s\nEXIT_CODE=%s\n\n' "$stdout" "$stderr" "$rc" >> "$LOG"
    return "$rc"
}

cd "$ROOT" || exit 1
run create_test_only_root mkdir -p "$TEST_ROOT" || exit $?

for arch in armv7l aarch64 x86_64; do
    root="$TEST_ROOT/$arch"
    run "mkdir_test_root_$arch" mkdir -p "$root/var/lib/rpm" || exit $?
    run "init_empty_rpmdb_$arch" rpm --root "$root" --dbpath /var/lib/rpm --initdb || exit $?
    if [ "$arch" = armv7l ]; then
        nongnu_runtime=tmp/R11/rpms/base_binary/armv7l/libunwind-1.6.2-1.3.armv7l.rpm
        nongnu_devel=tmp/R11/rpms/base_binary/armv7l/libunwind-devel-1.6.2-1.3.armv7l.rpm
        llvm_runtime=tmp/R11/rpms/base_binary/armv7l/libc++-22.1.8-19.1.armv7l.rpm
        llvm_devel=tmp/R11/rpms/base_binary/armv7l/libc++-devel-22.1.8-19.1.armv7l.rpm
    elif [ "$arch" = aarch64 ]; then
        nongnu_runtime=tmp/R11/rpms/base_binary/aarch64/libunwind-1.6.2-1.2.aarch64.rpm
        nongnu_devel=tmp/R11/rpms/base_binary/aarch64/libunwind-devel-1.6.2-1.2.aarch64.rpm
        llvm_runtime=tmp/R11/rpms/base_binary/aarch64/libc++-22.1.8-19.1.aarch64.rpm
        llvm_devel=tmp/R11/rpms/base_binary/aarch64/libc++-devel-22.1.8-19.1.aarch64.rpm
    else
        nongnu_runtime=tmp/R11/rpms/base_binary/x86_64/libunwind-1.6.2-1.2.x86_64.rpm
        nongnu_devel=tmp/R11/rpms/base_binary/x86_64/libunwind-devel-1.6.2-1.2.x86_64.rpm
        llvm_runtime=tmp/R11/rpms/base_binary/x86_64/libc++-22.1.8-19.1.x86_64.rpm
        llvm_devel=tmp/R11/rpms/base_binary/x86_64/libc++-devel-22.1.8-19.1.x86_64.rpm
    fi
    # --test is an RPM transaction check only: it does not install payloads.
    run "rpm_test_coinstall_$arch" rpm --root "$root" --dbpath /var/lib/rpm \
        -i --test --nodeps --noscripts \
        "$nongnu_runtime" "$nongnu_devel" "$llvm_runtime" "$llvm_devel"
    test_rc=$?
    printf '%s\t%s\n' "$arch" "$test_rc" >> "$RAW/rpm_conflict_test_exit_codes.tsv"
done

{
    printf 'arch\texit_code\n'
    cat "$RAW/rpm_conflict_test_exit_codes.tsv"
} > "$ROOT/progress/R11/tables/rpm_conflict_test_exit_codes.tsv"
exit 0
