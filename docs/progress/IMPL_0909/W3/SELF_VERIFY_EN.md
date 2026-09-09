# How to verify independently

For headquarters and business teams. These are reproduction instructions; **this documentation
stage did not compile or run probes**. Use the already-measured
[four-wait probe](../../R111/code/condition_cancel_probe.cpp) and
[runtime-identity check](../../IMPL_0908/code/runtime_identity.cpp).
Source and actual execution were rechecked in [raw/003](raw/003_verified_reproducer.stdout).

## Version inspection

Run `readelf -V` and `readelf --dyn-syms --wide` on the final executable and every DSO containing
an actual waiting call, and on the libc++.so.1 actually loaded at runtime. Correlate the specific
undefined wait/system-clock timed-wait reference with its version node. A new node appearing
somewhere in a file does not establish every wait's binding.
[Complete measured-ELF example](raw/004_version_example.stdout).

| Observation | What it does and does not establish |
|---|---|
| Waiting reference `@LLVM_22` | Link time selected the old contract; replacing a runtime does not automatically reselect it |
| Waiting reference `@LLVM_22_TIZEN_1` | New entry selected; neither new-header compilation nor caller cleanup code is proved |
| Provider exports `@LLVM_22` and `@@LLVM_22_TIZEN_1` | Both contracts exist; double @ identifies the default version |
| `.o`/`.a` lacks version requirements | Final binding cannot be inferred; inspect final linked artifacts and build provenance |

Other interfaces, such as notifications, normally retain the old node; its presence does not mean
wait uses the old version. Missing a relevant DSO or checking a file that is not actually loaded
invalidates that extrapolation.

## Cancellation and cleanup checks

Run the [shared reproduction commands](SELF_VERIFY_COMMANDS.md) from this repository's root,
substituting actual absolute paths. Use the existing target-appropriate Clang or sysroot wrapper,
new headers and matched libc++/libc++abi. Do not replace system libraries. These commands are a
native Linux example, not an unvalidated cross-execution recipe. Save each exact command,
stdout/stderr, exit status, compiler version and source/library SHA256 values.

Require actual runtime maps and `RUNTIME_IDENTITY=PASS`, `event=wait_confirmed`, zero cancellation
and join codes, `canceled=1 cleanup_count=1`, and an actual cleanup event—not only exit status.
EXPECTED_RUNTIME must match the canonical directory shown in maps; an identity failure is not a
cancellation result. The probe's terminate marker exits with 86, not the default SIGABRT behavior.

The probe uses bare pthreads and deferred cancellation, without adding non-throwing barriers to
the waiting chain. Steady/custom paths are recompiled with the probe; a library-only update cannot
establish that old inline implementations changed. This verifies a particular cleanup object,
not arbitrary application state.

## Inspect old application objects separately

A newly compiled probe does not validate cleanup coverage in old application `.o`/`.a` objects.
Use the actual old objects, compiler options and link order for the application-shaped comparison;
recompiling old source is not testing an old object. Inspect the
[old/new-caller probe](../../R112/code/item1_oldcaller_cancel.cpp),
[measured build commands](../../IMPL_0908/W2/raw/binding_build/) and
[two-architecture binding results](../../IMPL_0908/W3/REPORT.md), which also assert actual native
mutex unlocking and acquisition by a contender.

A relinked quiet old object has shown canceled=1, cleanup/unlock=0 and contender timeout, while
an old variant with a nearby potentially throwing call may run cleanup. One successful sample
does not justify a commitment for all old objects. Joining a thread does not prove lock usability;
actual applications must assert their resources, states and values under the support preconditions.
