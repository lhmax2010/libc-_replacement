#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LOG="$ROOT/progress/R11/commands/23_readonly_final_audit.log"
RAW="$ROOT/progress/R11/raw"
: > "$LOG"
run() {
    local label="$1"; shift
    local stdout="$RAW/${label}.stdout.txt" stderr="$RAW/${label}.stderr.txt"
    printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD" >> "$LOG"; printf '%q ' "$@" >> "$LOG"; printf '\n' >> "$LOG"
    "$@" >"$stdout" 2>"$stderr"; local rc=$?
    printf 'STDOUT_FILE=%s\nSTDERR_FILE=%s\nEXIT_CODE=%s\n\n' "$stdout" "$stderr" "$rc" >> "$LOG"; return "$rc"
}
cd "$ROOT" || exit 1
run llvm_branch git -C codes/llvm branch --show-current || exit $?
run llvm_head git -C codes/llvm rev-parse HEAD || exit $?
run llvm_status_porcelain git -C codes/llvm status --porcelain || exit $?
run llvm_remote git -C codes/llvm remote -v || exit $?
run assert_llvm_identity bash -o pipefail -c 'test "$(git -C codes/llvm branch --show-current)" = tizen_base && test "$(git -C codes/llvm rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b && test -z "$(git -C codes/llvm status --porcelain)"' || exit $?
run project_artifact_sha256 bash -o pipefail -c 'find artifacts/rebuild_2218_c2 -type f -name "*.rpm" -print0 | sort -z | xargs -0 sha256sum' || exit $?
run readelf_version readelf --version || exit $?
run rpm_version rpm --version || exit $?
run python_version python3 --version || exit $?
run temp_download_inventory bash -o pipefail -c 'find tmp/R11/rpms tmp/R11/srpms -type f -printf "%p\t%s\n" | sort' || exit $?
run assert_report_no_placeholders bash -o pipefail -c '! grep -n "@@" progress/R11/R11_report.md' || exit $?
run largest_progress_files bash -o pipefail -c 'find progress/R11 -type f -printf "%s\t%p\n" | sort -nr | head -n 30' || exit $?
run assert_no_progress_file_over_99mb bash -o pipefail -c '! find progress/R11 -type f -size +99M -print -quit | grep -q .' || exit $?
run project_status_before_delivery git status --short --branch || exit $?
