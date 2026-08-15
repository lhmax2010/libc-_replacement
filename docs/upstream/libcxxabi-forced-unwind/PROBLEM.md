# ARM EHABI foreign forced-unwind handlers are registered as cleanups

Status: not submitted upstream.

## Affected configuration

- 32-bit Arm using ARM EHABI.
- libc++abi with a non-LLVM unwinder (`LIBCXXABI_USE_LLVM_UNWINDER=OFF`, observed with libgcc_s).
- POSIX forced unwinding initiated by `pthread_cancel` or `pthread_exit`.
- A catch chain that first identifies and rethrows the foreign forced-unwind object, followed by a catch-all.

## Symptom

Without a typed forced-unwind handler, `catch (...)` intercepts the foreign forced unwind. If it is not rethrown, glibc prints `FATAL: exception not rethrown` and aborts.

After adding the forced type, mapping, and foreign-rethrow handling, ARM EHABI still aborts on the next real cleanup. The EHABI personality reports `_URC_HANDLER_FOUND` for the typed handler but calls `__cxa_begin_cleanup` before installing it. Compiler-generated catch landing pads call `__cxa_begin_catch`/`__cxa_end_catch`, not `__cxa_end_cleanup`, so the cleanup registration is left on `propagatingExceptions`.

## Minimal reproduction

See `REPRODUCER/forced_unwind_reproducer.cpp`. The full source actually used by the two-architecture matrix is `REPRODUCER/s1_rethrow_probe.cpp`.

## Expected and observed

Expected: the typed handler executes, rethrows, all cleanup destructors execute exactly once, `pthread_join` returns `PTHREAD_CANCELED`, and the process exits 0.

Before the fourth change on armv7l: the typed handler executes, but the next real cleanup sees a non-empty foreign cleanup stack and terminates.

After the fourth change: armv7l and x86_64 both match the libstdc++ control for the tested matrix. See `TESTING.md`.

## Why libstdc++ does not reproduce this failure

GCC/libstdc++ supplies all four semantic pieces: a `__forced_unwind` type, forced-foreign mapping, `_Unwind_Resume_or_Rethrow` on rethrow, and an EHABI personality that distinguishes `found_handler` from `found_cleanup`. It calls `__cxa_begin_cleanup` only for the latter.
