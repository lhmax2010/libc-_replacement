# Proposed fix (not submitted)

The complete tested experimental diff is `PROPOSED_FIX.diff`. It includes the four semantic changes plus build-time-disabled observation points retained for validation.

1. Define `__cxxabiv1::__forced_unwind` in `cxxabi.h`.
2. Provide an out-of-line key function in `cxa_exception.cpp`, yielding one exported strong typeinfo definition; map foreign forced unwind to that type in `scan_eh_tab`.
3. Use `_Unwind_Resume_or_Rethrow` for non-SJLJ foreign rethrow while preserving the native and SJLJ paths.
4. In the ARM EHABI `_URC_HANDLER_FOUND` path, recognize the narrowly tagged foreign forced typed handler and install its catch landing pad without calling `__cxa_begin_cleanup`.

The fourth change does not weaken or alter the non-empty protection in `__cxa_begin_cleanup`. True cleanup frames continue to use the original begin/end cleanup path.

The semantic scope of the new handler branch is the conjunction: foreign exception, `_UA_FORCE_UNWIND`, ARM EHABI, successful match to `__forced_unwind`, `_URC_HANDLER_FOUND`. Native exceptions, non-forced foreign exceptions, ordinary cleanup frames, SJLJ, and Itanium control flow are outside that branch.

GCC comparison: `eh_arm.cc` performs the forced-type mapping; `eh_personality.cc` retains `found_handler` versus `found_cleanup` and invokes `__cxa_begin_cleanup` only for `found_cleanup`.
