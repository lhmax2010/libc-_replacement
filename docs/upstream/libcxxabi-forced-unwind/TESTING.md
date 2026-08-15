# Testing

## Result

`S1_COMPLETE_BOTH_ARCH` for the explicitly tested scope.

- x86_64 native: patched libc++ 7/7 exit 0; libstdc++ control 7/7 exit 0; unpatched catch-all control exits 134/SIGABRT.
- armv7l native board: same result.
- armv7l trace-on cancel: exits 0 and shows no cleanup registration for the typed handler; the later real cleanup push/pop is paired.
- Consumers were linked without `--export-dynamic`.

Cases per patched/control suite: cancel, pthread_exit, `std::runtime_error`, non-forced custom foreign exception, cancel without catch, pthread_exit without catch, and a nested handler-cleanup/rethrow case.

Full table: `TESTING.tsv`. Raw outputs and exact commands: `docs/progress/R42/code/evidence/040_x86_matrix.log`, `054_board_matrix.log`, and `055_board_trace_cancel.log`.

## Not covered

- LLVM libunwind as the active unwinder.
- aarch64, SJLJ, Windows SEH, Darwin, or other operating systems.
- Real product applications, long-running stress, concurrent cancellation storms, signal-handler interaction, sanitizer combinations, or ABI-compliance suites beyond the targeted symbol checks.
- The reduced upstream reproducer was derived from the tested full source and was not separately rerun.
