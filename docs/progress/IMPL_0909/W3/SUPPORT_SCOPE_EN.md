# Tizen libc++ cancellation support statement (final-review candidate)

> Branch consolidation, 2026-09-11: the current LLVM implementation branch is `sandbox/lhmax2025/libcxx-ehabi-backport`. The same five commits, trees and Change-Ids are retained; no tests were rerun. Historical logs describe the branch used at the time, not a current dependency. [Branch mapping and verification](../../P6B_0909/BRANCH_MAPPING.md).

> Metadata update, 2026-09-11: commit references now identify tree-identical commits with Change-Id footers. Original test dates, results and support limits are unchanged; historical raw records identify the old commits. [Mapping and tree checks](../../P6_0909/resume/SHA_MAPPING.tsv).

**Pending final human review. Both official-suite result denominators are complete; this does not mean every test passed or product-release acceptance is complete.**

**For headquarters and business-team review; not a product-release guarantee.** This draft separates
the proposed, evidence-bounded commitments from known limitations. A thread exiting, or a process
not crashing, does not establish that its resources and shared state were cleaned up correctly.

The implementation is on `sandbox/lhmax2025/libcxx-ehabi-backport`, commit
`c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13`, on top of the four existing runtime patches.
This work used isolated builds and did not replace product system libraries. The destructor-flush
failure is now an accepted behavior change under this round’s three-way criterion; its raw FAIL is retained.
The original 7,848 valid ARM results were preserved and the other 3,554 completed, giving 11,402 results with zero missing.
The board has been cleaned and released. See the [earlier directed validation](../../IMPL_0908/W3/REPORT.md) and
[complete official-suite comparison and cleanup](../W2/REPORT.md).

## What changes

**Let cancellation cross the waiting boundary, and restore shared-lock writer state.** During
cancellation, the runtime walks back through the call chain and runs cleanup code; this is called
*forced unwinding*. A `noexcept` contract forbids an exception from leaving a function and previously
caused termination at the affected waiting boundary. The change removes that contract from ordinary
waiting and three timed-wait variants. It also adds rollback to the ordinary `shared_mutex` writer
and timed `shared_timed_mutex` writer: an interrupted wait clears the writer's flag and notifies
queued waiters without erasing the state of readers that still legitimately hold the lock.
Validation basis: [implementation and builds](../../IMPL_0908/W1/REPORT.md),
[state and value assertions on both architectures](../../IMPL_0908/W3/REPORT.md).

**Keep an old entry point for already-linked old programs.** Symbol versioning lets the same library
retain old and new contracts. Only ordinary waiting and the system-clock timed-wait library function
receive an old `LLVM_22` entry and a new default `LLVM_22_TIZEN_1` entry. This does not recompile
old programs or generate cleanup code missing from them. Validation basis:
[versioning design](../../IMPL_0908/W2/REPORT.md), [final symbol checks](../../IMPL_0908/W3/raw/035_alias_verification.stdout),
and [timed-entry measurements](../../IMPL_0908/W3/timed_versions.tsv).

**Stop automatically flushing the conversion buffer during destruction.** `wbuffer_convert`
converts wide characters and writes them to an underlying buffer. Its modern-header destructor
no longer requests automatic synchronization. Resource release and explicit `pubsync()` remain.
Code requiring complete output can no longer treat destruction as the final flush. Flushing here
means passing converted buffered output downstream, not a guarantee of durable physical storage.
Validation basis: [call sites and resource ownership](../../R111/ITEM1_WBUFFER_CLOSE_REPORT.md),
[old/new output-value comparison](../../IMPL_0908/W3/REPORT.md).

**Use compatible C++ object conventions on both sides of Boost boundaries.** An ABI is the binary
agreement for calls, object representation, and ownership. The established approach is to use
libc++ for the relevant Boost providers and migrated consumers, avoiding different libstdc++/libc++
interpretations of the same object. Existing adaptation covers the Boost providers under the Clang
toolchain condition. This round did not redo Boost migration or obtain evidence that all 13 consumers
have been rebuilt and deployed together. Being in the migration plan is not evidence of completion.
Validation basis: [90 edges, 13 consumers, and build conditions](../../R111/ITEM2_BOOST_TRANSITION_REPORT.md).

## Preconditions for the commitments

The following are **limited proposed commitments**, not cancellation safety for arbitrary C++ code.
They assume the tested, matched libc++/libc++abi and GNU unwinder configuration; the Linux `__1`
shared-library configuration with `LIBCXX_ENABLE_TIZEN_CONDVAR_VERSIONS` enabled; all relevant
call-chain objects rebuilt with consistent new headers and linked against the dual-version library;
and C++ exceptions and the corresponding cleanup information enabled.

They apply to **deferred cancellation**, which handles a request at a cancellation point, not
asynchronous cancellation. The call chain must not contain additional `noexcept` barriers, and
callers must not swallow forced unwinding. Normal API preconditions, including lock ownership,
still apply. Measured threads were created with bare `pthread_create`; equivalence of other
thread wrappers to this method was not established in this round.
Configuration and method basis: [build design](../../IMPL_0908/W2/REPORT.md),
[final measurements and limitations](../../IMPL_0908/W3/REPORT.md).

| Commitment, with validation basis | Known limitation, and why |
|---|---|
| Under these preconditions and in the tested forms, cancellation crosses ordinary, system-clock, steady-clock, and custom-Clock waits; cleanup count is 1 and the creator joins the cancelled thread. Basis: [native matrix](../../IMPL_0908/W3/matrix_x86_64_final.tsv), [physical-board matrix](../../IMPL_0908/W3/matrix_armv7l_final.tsv), five repetitions per cell. | “Clean exit” means these asserted events: crossing the boundary, running the observed cleanup, and joining the thread. Why: arbitrary application state, callbacks, and every scheduling interleaving were not exhausted. Successful joining alone does not prove process-wide correctness. |
| After cancellation of the tested ordinary/timed shared-lock writer, reader state is retained, the writer flag is cleared, queued threads make progress, and subsequent readers/writers actually acquire and release the lock. Basis: [two-architecture state and contention comparisons](../../IMPL_0908/W3/REPORT.md), five repetitions per cell. | Timed rollback covers the tested steady-clock instance, not arbitrary Clocks, template arguments, callbacks, or scheduling fairness. Why: finite instances and contention runs cannot establish these wider properties; upgrading a library cannot add header-resident rollback to old consumer objects. |
| The tested `timed_mutex`, ordinary `future::wait`, and `condition_variable_any` with a non-throwing user lock execute cleanup and remain reusable; the future yields exactly 42. Basis: [directed matrices](../../IMPL_0908/W3/REPORT.md). | This excludes final shared-state destruction for `std::async` and cv-any user locks that throw when relocked. Why: the former still has additional non-throwing frames; the latter calls user `lock()` inside a non-throwing guard destructor. Basis: [destructor call-chain inspection](../../R112/REPORT.md), [physical-board throwing-lock comparison](../../R115/W3/REPORT.md). |
| Tested normal waits, notifications, timeouts, spurious wakeups, and shared-lock acquisition/release and multi-reader/multi-writer contention retain the same concrete values and states as the matched old baseline. Basis: [normal-path comparisons](../../IMPL_0908/W3/REPORT.md), five repetitions per cell on each architecture. | This is not a full product-regression pass or performance guarantee. Why: the official suites retain existing failures and unsupported cases; real workloads, all optimization combinations, and stable production performance were not established. |
| The tested conversion buffer produces complete output after successful explicit `pubsync()`; cancellation no longer enters the removed destructor-flush call. Basis: [buffer values and destructor-cancellation comparison](../../IMPL_0908/W3/REPORT.md). | Unflushed output is not guaranteed to survive, and arbitrary custom conversion facets are not covered. Why: removing automatic flushing is an intentional behavior change; user-defined resource destructors may still execute user code. |

## Objects that were not rebuilt: four binding cases

ELF is the binary format used for Linux executables and shared libraries. A `.o` is a compiled
object file; a `.a` is an archive of object files. **Compilation determines call-site and cleanup
code; linking selects symbol versions. Neither step substitutes for the other.**

Here, an “old ELF” specifically has an existing `LLVM_22` requirement for the waiting reference.
This is not a guarantee for every unversioned old file or arbitrary dynamic-symbol lookup.
Each ordinary-wait case below was tested five times natively and five times on the physical board;
old/new system-clock entries were also tested five times each. Basis:
[final two-architecture matrices](../../IMPL_0908/W3/REPORT.md), [versions and observed behavior](../../IMPL_0908/W2/REPORT.md),
[timed supplement](../../IMPL_0908/W3/timed_versions.tsv).

| Compilation and linking history | Runtime entry | Cancellation outcome and boundary |
|---|---|---|
| Already-linked old program whose reference records the old node | `LLVM_22` | The old non-throwing contract is retained; termination is entered. Probes use a marker handler that exits with 86; 86 is not the name of a default operating-system signal. |
| Old `.o`/objects from `.a`, relinked against the dual-version library without recompilation | New default `LLVM_22_TIZEN_1` | **Depends on existing machine-code and exception-cleanup-table coverage.** Cleanup may run, or the thread may be joined while the lock remains stuck. In this round's quiet frame, cleanup/unlock counts were 0 and a contender could not acquire the lock. No clean-exit commitment applies. |
| Objects rebuilt with new headers and linked against the dual-version library | New default `LLVM_22_TIZEN_1` | Under the stated call-chain preconditions, cleanup/unlock counts are 1, a contender acquires the lock, and the cancelled thread is joined cleanly. |
| Objects rebuilt with new headers but linked against the old single-version library before the runtime library is upgraded | `LLVM_22` | The already-recorded old version requirement remains, so the old termination path is entered. A later runtime upgrade does not reselect that link-time requirement. |

Why relinked old objects have no uniform predicted outcome: the old compiler could assume the
call would not throw and omit its cleanup region. Nearby potentially throwing calls, compiler
options, and optimization can change the machine-code range covered by cleanup tables. Relinking
does not generate the missing code. Earlier old-header “nearby throwing call” and quiet variants
demonstrated both cleanup and stuck-lock outcomes on native x86_64 and physical armv7l.
This is not a random probability, nor a claim that every old object must leave a lock stuck.
Here, **layout means call-site and cleanup-table coverage**, distinct from Boost's C++ object-memory
layout issue. Validation basis: [old-caller variants and native mutex state](../../R115/W3/REPORT.md).

## Three header implementations cannot be repaired by a shared-library upgrade

“Inline” here means implementation compiled into the consumer from a header; it does not require
the function to disappear from the final assembly. Updating a shared library cannot rewrite those
implementations already present in old objects. Reliable adoption requires rebuilding every relevant
translation unit, meaning each source-file compilation.

| Header implementation | Why old objects must be rebuilt |
|---|---|
| Steady-clock timed wait | The non-throwing contract and call-site information are already in the consumer; library symbol versioning has no corresponding reliable repair point. |
| Custom-Clock timed-wait template | The consumer generates the template instance; replacing a library does not instantiate the old code again. |
| `wbuffer_convert` destructor | The old automatic synchronization call is already compiled into the consumer; the new library cannot remove that old code. |

The **timed shared-lock writer's rollback guard** is also in a header and requires rebuilding its
relevant objects. Accidentally selecting one copy from mixed old/new-header objects is not a
compatibility guarantee: earlier experiments observed link-order effects when same-name template
copies were merged. Basis: [inline and mixed-object inspection](../../R112/REPORT.md),
[current versioning boundary](../../IMPL_0908/W2/REPORT.md).

## Exclusions

| Commitment boundary | Known limitation, and why |
|---|---|
| Only the stated deferred-cancellation forms. | **Asynchronous cancellation is excluded.** Conditional paths in lightweight-web-engine and OpenMP enable it; a request may arrive in another non-throwing frame or midway through a state update. An independent x86_64 `atomic<int>::wait` probe still terminated while avoiding the modified entry points. Fixing those entries therefore does not protect these components generally. Basis: [conditional reachability and independent measurement](../../R108/W3_REPORT.md). This does not assert that every product triggers it or extend that independent probe to ARM. |
| The measured ordinary future-wait forms. | **`std::async` destruction is excluded.** Final shared-state release still crosses non-throwing destructor/callback frames; changing the lower waiting contract does not remove them. Basis: [static call-chain inspection](../../R112/REPORT.md). |
| Cv-any with an ordinary, non-throwing user lock. | **A user lock throwing during relocking is excluded.** The guard destructor remains non-throwing, and measurements entered termination. Changing the waiting entry does not change the user's lock contract. Basis: [native and physical-board comparison](../../R115/W3/REPORT.md). |
| Only the configurations and callback instances actually validated. | **Objects built without C++ exceptions, additional non-throwing frames, swallowed forced unwinding, and arbitrary template arguments/Clocks/facets/callbacks are excluded.** Why: cleanup information may be absent or another barrier may remain. Old no-exception samples demonstrated “joined thread, stuck lock.” Basis: [old-object physical-board evidence](../../R115/W3/REPORT.md). |
| No change to the waiting side of `call_once` itself. | **No commitment to repair its existing cancellation risk.** Its inspected waiting chain directly uses native waiting and does not cross the changed entries; no rollback was added. A residual global lock on that path has not been dynamically demonstrated. Basis: [call-chain inspection](../../R115/W4/CALL_ONCE.md). |

## Externally observable behavior changes

In the `abcdef` buffer sample, explicit synchronization produces all six bytes in both old and new
builds. Without it, the old build produces six bytes and the new build only `a`: **five bytes are
not emitted in this sample**, not a universal fixed loss. The official `overflow.pass.cpp` relies
on destructor flushing and fails with the new library on both architectures; the matched old
baseline passes the individual test on both. This is not a `noexcept`-assumption test. Under the explicit three-way criterion for this round,
it is now classified as an accepted behavior change while its raw FAIL is retained. Basis: [output values and official comparisons](../../IMPL_0908/W3/REPORT.md).

Ordinary errors can throw `system_error`, an exception carrying system error information, through
the **new entry**. The old entry still terminates under its non-throwing contract. This does not
make lock-precondition violations supported normal API usage. **An uncaught ordinary exception
still ultimately terminates the process, and cleanup is not guaranteed.** Earlier native x86_64
measurements observed SIGABRT, identical diagnostics, and zero local cleanup in both builds using
the default handler. The difference was the termination boundary and propagation path. Successful
cancellation cleanup must not be presented as an unconditional improvement to ordinary errors.
Basis: [uncaught-error backtrace, signal, and cleanup comparisons](../../R111/ITEM3_CONDVAR_UNWIND_REPORT.md).

## Architecture coverage and strength of evidence

| Architecture/material | Coverage and limits on interpretation |
|---|---|
| x86_64 | Native host. 235 final directed runs; all 11,402 official libc++/libc++abi tests have results. The sole new failure is the destructor-flushing case above. |
| armv7l | Raspberry Pi 4 Model B Rev 1.5 **physical board**. 235 final directed runs; all 11,321 libc++ and 81 libc++abi tests have results, with zero missing. The original 7,848 were not rerun; 3,554 were completed. The board has been cleaned and released. These are not QEMU user-mode execution results. |
| aarch64 | Dynamic validation of this implementation is `NOT_OBSERVED`. No equivalent-result commitment is made. Existing static Boost-artifact evidence does not fill this gap. |
| External product/application components | Their denominator and rebuild policies remain `NOT_AVAILABLE`. A zero-use finding within the frozen repositories does not establish zero use or compatibility across the platform. |

Directed probes use C++20; the official suite uses the established C++26 configuration. C++03
waiting declarations were updated; this round did not change the C++03 `wbuffer_convert` destructor.
The modern destructor change and limited dynamic results
do not establish every language mode, compiler option, linker, or distribution configuration.
Dual versioning here is a particular shared-ELF-library design; static linking, unversioned old ELFs,
other ABI namespaces, and arbitrary `dlsym` usage have no equivalent commitment.
Basis: [earlier configuration and matrices](../../IMPL_0908/W3/REPORT.md), [complete ARM official comparison](../W2/ARM_R81_COMPARISON.tsv),
[external denominator and required materials](../../R110/W1_REPORT.md).

## Deployment ordering and release boundaries

To enter the “new headers, new node” case, **new headers must precede rebuilding the relevant
objects, and the dual-version library must be available before final linking**. A later runtime
replacement cannot rewrite an old version requirement or generate old-object cleanup code.
The matched libc++/libc++abi, including the existing runtime patches, must also be deployed;
deploying only an individual changed entry does not reproduce the validated configuration.
Basis: [four real binding cases and loaded-library identity](../../IMPL_0908/W2/REPORT.md),
[two-architecture revalidation](../../IMPL_0908/W3/REPORT.md).

Boost providers and consumers must actually use compatible ABIs at each object boundary to meet
the unified-object-convention premise. Replacing only Boost while old consumers remain on
libstdc++ can create a mixed window; completing a build and deploying it in the same product image
are different events. This statement does not prove all 13 consumers migrated, or that the release
system supports atomic switching or rollback. Basis:
[migration scope, build conditions, and transition evidence](../../R111/ITEM2_BOOST_TRANSITION_REPORT.md).

Release acceptance must still address disclosure of the accepted destructor behavior change, existing official-suite failures and unsupported cases,
external-component materials, and actual product
package/deployment identity. This round did not perform product GBS/RPM acceptance; the historical
`SOURCE_PROVENANCE` file is not a new release certification for this sandbox. This draft does not
replace those outstanding checks.

## Three-way destruction comparison and current official-suite status

The native file-stream sample writes three wide characters with a complete nine-byte UTF-8 encoding.
Without explicit synchronization: new libc++ emits three bytes, old libc++ nine, and libstdc++ zero,
five out of five each. With explicit pubsync all three emit nine bytes, five out of five each.
Under the user’s stated criterion—accept if libstdc++ also fails the original destructor-output
expectation—the no-destructor-flush decision is retained. New libc++ and libstdc++ **align in not
automatically flushing on destruction**, not in all observable behavior. The measured byte counts are
3 for new libc++, 9 for old libc++, and 0 for libstdc++. The difference between new libc++ and libstdc++
comes from their existing buffering strategies; this change did not introduce a new buffering strategy.
Removing the destructor flush exposes data left buffered by those strategies. The change from 9 bytes
in old libc++ to 3 in new libc++ is, however, a consequence of this destructor change. This distinction
must not be read as saying that the change leaves output byte counts unchanged.
Static evidence: the [complete destructor patch](../W4/patches/0003-libcxx-make-wbuffer_convert-destruction-independent-.patch)
only removes the destructor's `__close()` call and adds comments; it does not modify the writing or buffering algorithms.
N4861 D.21.2 specifies disposal of the conversion object but does not require a
destructor flush. See [three-way results](../W1/REPORT.md) and [standard/expectation analysis](../W1/STANDARD_ANALYSIS.md).

Native official coverage: 11,402 tests; libc++ has 10,009 PASS, 129 FAIL, 1,156 UNSUPPORTED and 27 XFAIL;
libc++abi has 61 PASS and 20 UNSUPPORTED. ARM now combines the original 7,848 results with 3,554 completed tests:
**11,402 total, 10,080 PASS, 211 FAIL, 1,082 UNSUPPORTED, 29 XFAIL, zero missing**.
ARM libc++ has 10,020 PASS, 211 FAIL, 1,061 UNSUPPORTED and 29 XFAIL; libc++abi has 60 PASS and 21 UNSUPPORTED.
The only new FAIL on either architecture is overflow, classified as an accepted behavior change without changing the raw FAIL.
There are zero new noexcept-assumption failures and zero other new FAILs. This is a per-test status comparison,
not proof of identical causes for every existing failure or of problem-free business workloads. The three historical
timeout FAIL-to-PASS changes retain the different-timeout-window qualification and are not credited to the patch.
See the [complete two-architecture table](../W2/TWO_ARCHITECTURES.tsv), [new-failure classification](../W2/NEW_FAILURE_CLASSIFICATION.tsv)
and [current status](../../IMPL_STATUS_0909.md).
All fourteen old residual targets, the new task directory and 74 new diagnostic ZIPs were removed and checked;
both the old eight ZIPs and the new diagnostics have verified host backups. The board has been cleaned and released.
See [complete results, limitations and cleanup evidence](../W2/REPORT.md).

## How to verify independently

See [reproduction steps and the existing probes](SELF_VERIFY_EN.md), including the specific wait
reference’s version requirement, actual loaded-library maps, and cleanup assertions for four bare-pthread
waiting forms. A version-node name or exit code alone does not establish application safety.
