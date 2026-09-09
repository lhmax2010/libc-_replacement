# Second runtime-change batch: delivery notes

**For delivery after human review. Current status: PARTIAL. ARM completion and cleanup are
outstanding, the support statement needs those data, and Gerrit requires Change-Id while all five
commits lack it. This is not a release-approval record.**

## Source and five commits

Branch: `sandbox/lhmax2025/libcxx-noexcept-relief`.
Base: `c3f8578a4db871d9d6de96d751f4c2ea7b1638fa`.
Local and read-only remote HEAD: `f3c1af692b579add991861e1f7c4950f6af39932`.
There are five linear commits above that base and the source worktree is clean. This task did not
implement another code change. [Remote and commit records](raw/002_sandbox_readiness.stdout).

The five [patch snapshots](patches/) were exported from those commits. Commit identifiers, SHA256
and stable patch-id (a content-comparison identifier) match the originals. No additional git am
application was performed in another source tree. See [COMMIT_INDEX.tsv](COMMIT_INDEX.tsv) and
[verification output](raw/005_verify_delivery.stdout).

| Commit (full SHA in index) | Change and rationale | Validation basis |
|---|---|---|
| 753ed75f | Remove the relevant specifications for ordinary and system/steady/custom-clock waits, including declarations, definitions and C++03 mirrors; allow cancellation unwinding across those boundaries; leave notifications unchanged | [Complete-library builds and initial checks](../../IMPL_0908/W1/REPORT.md), [two-architecture wait/cleanup matrix](../../IMPL_0908/W3/REPORT.md) |
| 2b94110f | Add local rollback guards after ordinary/timed writers reserve the writer bit; interrupted exits preserve readers, clear that bit and notify queued waiters; completion and normal timeout dismiss the guard; no new object fields | [Implementation](../../IMPL_0908/W1/REPORT.md), [state, progress and normal paths](../../IMPL_0908/W3/REPORT.md) |
| aafbe73b | Stop the modern wbuffer_convert destructor calling __close, preserving resource deletion and the helper; do not rely on destruction for final synchronization | [Ownership and call-site inspection](../../R111/ITEM1_WBUFFER_CLOSE_REPORT.md), [three-way nine-byte sample](../W1/REPORT.md) |
| 72e7a0f2 | Retain LLVM_22 wrappers for ordinary/system-clock library waits and publish LLVM_22_TIZEN_1 defaults; use public native_handle, hide helpers in the version map and explicitly enable the platform option | [Real library and four bindings](../../IMPL_0908/W2/REPORT.md), [final symbols](../../IMPL_0908/W3/raw/035_alias_verification.stdout) |
| f3c1af69 | Correct the timed alias when ARM intmax_t is long long instead of the LP64 long spelling; assert the type without changing algorithms or version contracts | [Original defect and correction](../../IMPL_0908/W3/REPORT.md), [independent old/new timed-entry runs on both architectures](../../IMPL_0908/W3/timed_versions.tsv) |

## Relationship to the four existing patches

The first batch covers libc++abi foreign forced-unwind state, the GNU forced-unwind marker type,
twenty consumer-instantiated handlers and thirty library-delivered handlers. It makes the runtime
and catch-all handlers recognize and preserve cancellation instead of consuming or corrupting it.
[First delivery](../../R69/REPORT.md), [original patches](../../R69/code/patches/).

The second batch builds on those patches and the existing packaging baseline. It addresses
**non-throwing boundaries, writer state after unwinding, destructor synchronization and old/new
contract compatibility**. Forced unwinding walks back through a cancelled thread's call chain and
runs cleanup; noexcept forbids an exception from crossing a function boundary. The batches do not
replace one another. Installing only the second does not reproduce the validated configuration.
The first batch's covered source files were rechecked unchanged in
[the protection check](raw/006_original_patch_check.stdout).

Using the four implementation categories listed in this package and P1—specification removal,
writer rollback, modern no-flush destruction and dual versions—**all four are implemented**.
This is not acceptance of every configuration or product, nor proof that all Boost consumers have
migrated and been deployed; those migrations are not implemented by these five commits.
The C++03 conversion-buffer destructor was not changed by this batch.

## Compatibility and deployment constraints

An ABI is a binary agreement about calls and object representation. Shared-lock object fields are
unchanged. Dual versioning covers only the two library waiting entries and requires Linux ELF,
the __1 namespace, matched shared libc++/libc++abi with the GNU unwinder, and
LIBCXX_ENABLE_TIZEN_CONDVAR_VERSIONS enabled. It is not a static-library or arbitrary-namespace guarantee.

| Consumer history | Observed binding and outcome |
|---|---|
| Already-linked ELF requiring the old node | LLVM_22 retains the old non-throwing contract and termination path |
| Old objects only relinked | New default, but cleanup-layout-dependent; a quiet sample joins the thread while leaving the lock stuck |
| New-header rebuild, then dual-version link | New default; measured cleanup, unlocking and contender progress succeed |
| New-header object first linked to old single-version library, then runtime upgraded | Old node remains recorded and the old termination path is entered |

New headers must precede relevant object recompilation; the dual-version library must be present
at final link; the matched libraries must actually load at runtime. Replacing a library neither
reselects a recorded version nor regenerates missing old-object cleanup. Both header timed waits,
the conversion-buffer destructor and timed-writer rollback require consumer recompilation.
Old-object layout here means call-site/cleanup-table coverage, not Boost object memory layout.
Quiet and nearby-throwing-call variants compiled with old headers can clean up differently;
one passing example cannot establish all old binaries. See the
[bilingual support candidate](../W3/REPORT.md) and [independent verification steps](../W3/SELF_VERIFY_EN.md).

## Validation summary

| Item | Native x86_64 | Physical armv7l | Original evidence |
|---|---|---|---|
| Final directed matrix | 45 cells × 5 = 225 | 45 cells × 5 = 225 | [Native rows](../../IMPL_0908/W3/matrix_x86_64_final.tsv), [board rows](../../IMPL_0908/W3/matrix_armv7l_final.tsv) |
| Old/new timed-version supplement | 2 cells × 5 = 10 | 2 cells × 5 = 10 | [Per-run table](../../IMPL_0908/W3/timed_versions.tsv) |
| Wait cancellation | Four forms: canceled=1, cleanup=1 | Independent measurements of the four forms | [Report and raw-entry links](../../IMPL_0908/W3/REPORT.md) |
| Shared-lock rollback | Readers retained, writer bit cleared, queued progress and later reads/writes succeed | Corresponding independent measurements | [State and normal contention](../../IMPL_0908/W3/REPORT.md) |
| Normal paths | Predicate 42; notifications, spurious wakeups and locked timeouts; contention value=2000, checksum=14003 | Corresponding independent measurements | [Normal-path records](../../IMPL_0908/W3/REPORT.md) |
| Three-way destruction sample | 30 new runs; without explicit sync new/old/GNU = 3/9/0 bytes; all emit 9 with explicit sync | Three-way sample not rerun in this package; earlier old/new buffer and destructor-cancellation evidence retained | [New matrix](../W1/matrix.tsv), [earlier board evidence](../../IMPL_0908/W3/REPORT.md) |

The first two rows total 470 runs, including negative cases expecting old-entry termination and
old-quiet-object stuck locks. Matching an expected assertion is not a claim that every case is
problem-free. Normal-path equivalence **excludes intentionally changed destructor output**.
Shared-lock probes use _Exit after assertions, so they do not cover global-object destruction.
Arbitrary Clocks, facets, callbacks and all interleavings were not exhausted. Dynamic aarch64
validation remains NOT_OBSERVED.

### Official suites and failure classification: currently valid coverage

| Architecture | Valid total | PASS | FAIL | UNSUPPORTED | XFAIL | Missing |
|---|---:|---:|---:|---:|---:|---:|
| x86_64, both suites combined | 11402 | 10070 | 129 | 1176 | 27 | 0 |
| armv7l, currently libc++ only | 7848 | 6657 | 136 | 1027 | 28 | 3554 |

See the [native per-test comparison](../../IMPL_0908/W3/comparison_x86_64_new_full.tsv) and
[current derived ARM comparison](../../IMPL_0908/W3/comparison_armv7l_new_combined.tsv).
ARM uses R81's owner-normalized per-test records, whose equivalence to the four-patch side was
previously checked; [baseline provenance](raw/007_baseline_provenance.stdout) records the mapping.
Native coverage uses the already-frozen complete historical baseline.

The newly failing case within observed coverage is overflow: **accepted destructor-synchronization
behavior change**, not a noexcept-assumption case; its raw FAIL remains. This package's three-way
measurements satisfy the user's stated criterion, not byte-for-byte equivalence. Three earlier ARM
timeout FAILs also changed to PASS under a different execution window; they were not credited to
the patch. Another 3554 tests lack this package's completion results. They cannot be filled as PASS
or used to claim no other regressions. This table still needs W2's completed results.

## Gerrit preparation status

The five commits are linear, the source is clean, remote/local SHAs match, source diff checking
passes, and exported patch-ids match. Every message has a libcxx subject, rationale, Chinese
explanation, Validation and Evidence, consistent with the existing runtime-patch explanation
structure. **None of the five has a Change-Id footer.** Detection was positively controlled against
the packaging base commit's valid footer; see [format verification](raw/005_verify_delivery.stdout).
This does not establish all Gerrit receive requirements.

Follow-up read-only inspection obtained the complete configuration chain: platform/upstream/llvm →
scm/acls/domain_system/toolchain → scm/acls/domain_system → scm/acls/domains → All-Projects.
The nearest explicit value is `receive.requireChangeId=true` in scm/acls/domains, overriding the
root's false. See [configuration chain and revision SHAs](GERRIT_CONFIG_CHAIN.tsv) and
[actual queries](raw/019_read_config_chain.stdout). Therefore the five commits **do not satisfy at
least the Change-Id prerequisite for direct review submission**, based on static configuration inspection.
Gerrit's official documentation states that projects requiring Change-Id reject review submissions
missing it. No refs/for push was attempted; an actual rejection response is still NOT_OBSERVED,
not a fabricated server receipt. [Official Gerrit documentation](https://gerrit-review.googlesource.com/Documentation/error-missing-changeid.html).
Inheritance follows the [official project-configuration rules](https://gerrit-review.googlesource.com/Documentation/config-project-config.html#receive.requireChangeId).
Both public HTTP configuration URLs returned 404; refs/meta/config was then fetched read-only into
an isolated bare repository under tmp. No project configuration, source repository or server was
modified. Only parent-project and receive options were recorded, not access-group membership or credentials.
Successful sandbox pushing establishes source availability on that branch, not successful review
admission. Humans decide whether and when to prepare formal review. No amend/rebase, force push,
formal-branch push or LLVM-upstream push was performed.

## Remaining gaps and delivery boundary

ARM completion/cleanup and corresponding support-statement data remain outstanding. Gerrit's
Change-Id prerequisite is now established but is not met by the existing commits.
External-component denominators and rebuild policies are NOT_AVAILABLE;
product GBS/RPM and actual release-identity acceptance are NOT_OBSERVED. Historical
SOURCE_PROVENANCE anchors are not a release certification of this sandbox. These notes compile
observed facts and the new three-way results without filling gaps with fabricated success.
