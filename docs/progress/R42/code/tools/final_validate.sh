#!/bin/bash
set -eu

require() {
  description=$1
  shift
  printf 'ASSERT=%s\nCOMMAND=' "$description"
  printf '%q ' "$@"
  printf '\n'
  "$@"
  printf 'RESULT=PASS\n'
}

require 'x86 patched/control 14 cells exit zero' bash -c \
  '[ "$(awk '\''/CELL_BEGIN suite=(libcxx_patched|libstdcxx_control)/{wanted=1;next} wanted && /CELL_EXIT_CODE=0/{n++;wanted=0} END{print n+0}'\'' progress/R42/commands/040_x86_matrix.log)" -eq 14 ]'
require 'arm patched/control 14 cells exit zero' bash -c \
  '[ "$(awk '\''/CELL_BEGIN suite=(libcxx_patched|libstdcxx_control)/{wanted=1;next} wanted && /REMOTE_CELL_EXIT_CODE=0/{n++;wanted=0} END{print n+0}'\'' progress/R42/commands/054_board_matrix.log)" -eq 14 ]'
require 'two unpatched controls reproduce 134' bash -c \
  '[ "$(rg --no-filename "(CELL_EXIT_CODE|REMOTE_CELL_EXIT_CODE)=134" progress/R42/commands/040_x86_matrix.log progress/R42/commands/054_board_matrix.log | wc -l)" -eq 2 ]'
require 'arm trace cancel exits zero' rg -q '^REMOTE_TRACE_EXIT_CODE=0' progress/R42/commands/055_board_trace_cancel.log
require 'trace selects forced handler path' rg -q '^R42_EHABI_FORCED_HANDLER_PATH ' progress/R42/commands/055_board_trace_cancel.log
require 'trace true cleanup pops stack to nil' rg -q '^R42_END_CLEANUP_RETURN propagating=\(nil\)' progress/R42/commands/055_board_trace_cancel.log
require 'trace handler begins with clean propagation stack' rg -q '^R42_BEGIN_CATCH_ENTRY native=0 caught=\(nil\) propagating=\(nil\)' progress/R42/commands/055_board_trace_cancel.log
require 'trace-off arm contains no diagnostic strings' bash -c \
  '! strings tmp/R42/build-patched-armv7l/lib/libc++abi.so.1.0 | rg -q "R4[012]_"'
require 'trace-off x86 contains no diagnostic strings' bash -c \
  '! strings tmp/R42/build-patched-x86_64/lib/libc++abi.so.1.0 | rg -q "R4[012]_"'
require 'consumer link commands omit export-dynamic' bash -c \
  '! rg -q -- "--export-dynamic" progress/R42/commands/035_build_probes_x86.log progress/R42/commands/038_build_probes_arm_retry.log'
require 'cleanup guard remains in source' rg -q 'if \(NULL != globals->propagatingExceptions\)' tmp/R42/source-patched/libcxxabi/src/cxa_exception.cpp
require 'board file cleanup passed' rg -q '^FILE_CLEANUP=PASS' progress/R42/commands/058_board_cleanup_retry.log
require 'board process cleanup passed' bash -c \
  '[ "$(rg -c "^PIDOF_AFTER (s1_libcxx|s1_libstdcxx|unpatched_libcxx)=\\r?$" progress/R42/commands/058_board_cleanup_retry.log)" -eq 3 ]'
require 'all curated files below 5MB' bash -c \
  '[ -z "$(find docs/progress/R42 docs/upstream/libcxxabi-forced-unwind -type f -size +5M -print -quit)" ]'
require 'upstream status not submitted' rg -q '^`NOT_SUBMITTED`$' docs/upstream/libcxxabi-forced-unwind/STATUS.md

printf 'FINAL_VALIDATION=PASS\n'
