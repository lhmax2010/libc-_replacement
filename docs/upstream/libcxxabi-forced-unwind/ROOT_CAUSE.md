# Root cause chain

## 1. A catch-all intercepts forced unwind

Observed directly in both unpatched controls: `unpatched:catch-all`, then glibc `FATAL: exception not rethrown`, exit 134/SIGABRT. Evidence: `docs/progress/R42/code/evidence/040_x86_matrix.log:176-187` and `054_board_matrix.log:191-202`.

This is a foreign exception path from libc++abi's point of view.

## 2. libc++abi lacks the libstdc++ forced-unwind identity

The upstream baseline has no public `__cxxabiv1::__forced_unwind` type and no foreign+force mapping to it. GCC supplies the type and maps the forced foreign object in its ARM exception support. Baseline and reference source with line numbers are archived in `010_static_reference_extract.log` and the complete diff in `PROPOSED_FIX.diff`.

The proposed type has an out-of-line key function, so its typeinfo is a unique strong definition in libc++abi. Consumers hold a `GLOBAL DEFAULT UND` reference and need no `--export-dynamic`.

## 3. Foreign rethrow uses the wrong unwind entry point

The baseline libc++abi calls `_Unwind_RaiseException` for foreign rethrow. That starts a new raise instead of continuing an active forced unwind. GCC uses `_Unwind_Resume_or_Rethrow`; when forced state is active it continues phase 2, and for a non-forced foreign exception it behaves as a new raise.

The complete S1 implementation switches only the non-SJLJ foreign branch. R42 verifies cancel, pthread_exit, real C++ exception, and a non-forced custom foreign exception on both architectures.

## 4. EHABI loses the handler/cleanup distinction

Before the fourth change, the EHABI `_URC_HANDLER_FOUND` path always called `__cxa_begin_cleanup`. R41's observed failure sequence showed a typed handler (`ttype=2`) registered on `propagatingExceptions`, entered through catch code, rethrown without `__cxa_end_cleanup`, then followed by a real cleanup (`ttype=0`) whose second begin-cleanup terminated on the non-empty stack.

R42's direct trace after the change is:

1. forced type match (`reason=6`, `ttype=2`);
2. `R42_EHABI_FORCED_HANDLER_PATH`, with no begin-cleanup;
3. `__cxa_begin_catch` sees `propagating=nil`;
4. rethrow keeps `reserved1` nonzero;
5. the next true cleanup performs `nil -> exception -> nil` through begin/end cleanup;
6. join is `PTHREAD_CANCELED`, exit 0.

Evidence: `docs/progress/R42/code/evidence/055_board_trace_cancel.log:94-116`.

The Arm EHABI specification distinguishes catching handlers from cleanup landing pads and specifies separate begin/end protocols. It does not itself specify the GCC forced-unwind extension; the forced typed mapping is cross-checked against GCC's implementation.
