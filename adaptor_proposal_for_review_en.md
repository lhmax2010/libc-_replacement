# Cross-standard-library adaptors: evidence, deployment variants and decision boundaries

For human review. This document incorporates the formal analysis, three reviews and the additional version-routing and exception-runtime experiments. It authorizes no platform implementation and ranks no options. Measurements, static checks and inferences are distinguished. An untested case is not a failure, and a passing sample is not general compatibility. [Source analysis](docs/progress/ADAPTOR_0912/W4/adaptor_analysis_zh.md), [additional evidence](docs/progress/P11_0916/REPORT.md)

## 1. Problem, scope and the decision already made

libstdc++ (the GNU C++ standard library) and libc++ (the LLVM C++ standard library) can represent strings and other objects differently in memory. The ABI, or application binary interface, also includes symbols, calling conventions, exception type identity and object lifetimes; matching sizes alone is insufficient. `extern "C"` provides a common entry-point name, not a conversion of `std::string` arguments into C data. Some mismatches prevent linking; others resolve a common symbol and subsequently misinterpret or incorrectly destroy objects.

The component owner has decided to migrate the **entire `chromium-efl` component to libc++**, without retaining a GNU-built complete component. Here, **Option A means selective retention of a GNU-built component and is excluded by that decision**, not disproved technically. Small GNU-built adaptor interface layers remain permitted. **Option B means two ABI facades in one DSO (dynamic shared object), with symbol-version routing**; it is also permitted. The reviewers used A/B for different proposals. This document maps their mechanisms rather than conflating their labels.

For old Native C++ clients, “zero application changes” means no application source edits, rebuilding, relinking or launch changes, while preserving the required function. Header-inline marshaling can avoid source edits but still requires rebuilding. For C#/JavaScript applications already crossing C or managed boundaries, coordinated migration inside the platform may also leave the application unchanged. The comparison for old C++ binaries must not be generalized to those applications.

The scope is the identified interfaces and available artifacts, not a new SDK inventory or a claim of complete SDK coverage. The earlier detailed analysis covered 8 Dali/NUI package families and 3 Web families; 12 Native families received a quick check, and 4 others were not expanded. This was not a detailed analysis of all 27 families. [Scope record](docs/progress/P7_0909/stage2/REPORT.md)

## 2. Mechanism and implementation forms that must be distinguished

### 2.1 Each side operates on its own objects; data crosses the bridge

A GNU-compiled translation unit reads GNU objects, while a libc++-compiled unit constructs libc++ objects. Pointers, lengths, integers and status codes cross a simple data protocol, called the POD protocol here. Each side constructs and destroys its own objects. Pointers are valid only within the same process and the agreed borrowing period; this is not an interprocess protocol. State, exceptions and reference lifetimes are part of the contract, not just final field values.

**Fact chain.** [Measured] Real Boost.Program_options providers were used in both mixed directions and same-side controls: 4 cells, 5 runs per cell, 20 runs and 100 input comparisons. Five inputs matched byte for byte. Two separately compiled `.o` files were already linked into one `.so`; mappings, entry ownership and binding diagnostics are available. [Limit] This establishes a controlled conversion mechanism, not EWK, a real browser callback or full Dali object compatibility. [Mechanism, builds and evidence index](docs/progress/API_0911/W2/REPORT.md)

**Excluded interpretation.** Two translation units (TUs) do not imply isolated runtimes. Standard-library namespaces distinguish some symbols, but this shim's `__gxx_personality_v0` actually bound to LLVM libc++abi. Each TU cannot be described as having its own exclusive exception runtime. Renaming a wrapper around an available provider does not substitute for testing an unavailable provider built with the other library.

**What would refute or extend the conclusion?** A byte or destruction discrepancy with the same real provider, runtimes, inputs and constraints would refute the sample result. Transparent deployment for existing applications additionally requires evidence that original entry points are routed correctly and all relevant state and lifetime contracts are preserved, not merely that a new test program can call the bridge.

### 2.2 Corrections to the previously “invalid approaches”

| Form | Fact chain, exclusion boundary and refutation condition |
| --- | --- |
| Substitute a custom “layout-compatible” type for `std::string` | [Static] Ordinary type substitution does not turn the real `std::string` in existing headers into a custom type. This excludes ordinary substitution, not every hand-crafted ABI emulation. Especially for a C-named entry, a complete demonstration covering calling conventions, layout, special members, allocation and lifetimes would rebut a stronger impossibility claim. No such complete demonstration is available. |
| One TU interprets the same standard-library object using both layouts | [Mechanism limit] The approach using each library's standard headers needs separately compiled sides. A single TU can still call the other side through C functions, and the final ELF can link both libraries. “One TU can link only one library” is inaccurate. The excluded form is reinterpretation without conversion; a verified conversion layer is a different form. |
| A provider exports two interfaces | [Measurement versus design] Two sets of `.o` files in one `.so` have already been demonstrated. Two complete providers differ from one libc++ core with two thin ABI facades; the latter is permitted Option B. Version nodes route calls; they do not convert objects. The three production contracts below remain open, so the mechanism sample is not transparent deployment of the old EWK entry. |

Option B has three unresolved production contracts: (1) same-name weak definitions and COMDAT selection must remain correct for actual link order, optimization and LTO; (2) dynamic interposition of `operator new/delete` must respect construction and release contracts; and (3) coexisting exception runtimes must preserve the intended ordinary-exception, cancellation and cleanup behavior. COMDAT is a linker mechanism for selecting among corresponding definitions. A prototype failure already exists for the first issue; what remains unverified is the complete deployment guarantee. The other two must not be rewritten as failures already observed in all combinations.

## 3. Evidence: passes, failures and the weight of five repetitions

### 3.1 Four observed outcome shapes must remain visible

These are existing measurements of real Boost boundaries, not a new adaptor matrix. Architecture and observation stage are retained. [Detailed background and raw-evidence entry points](docs/progress/R110/DECISION_BACKGROUND.md)

| Outcome | Existing observation | What it establishes |
| --- | --- | --- |
| Return with silently wrong values | Filesystem: x86_64 native and armv7l QEMU user mode; selected cells each produced wrong values in 5/5 runs, with destruction completed. | Normal exit and completed destruction do not replace value assertions. |
| Termination before value assertions | Iostreams: exit 134 on x86_64 and exit 139 on armv7l QEMU user mode, each in 5/5 selected runs. | Unreached value and destruction checks cannot count as passes. |
| Correct values followed by failed destruction | Program_options: aarch64 QEMU user mode, exact values in 5/5 runs followed by exit 139 during destruction. | Correct conversion of values does not establish correct lifetime handling. |
| Exact passes for finite samples | Thread: x86_64 single-thread once state `1,1,0`, 5/5; the selected aarch64 QEMU cell also passed exactly in 5/5 runs. | An ABI-coupled interface does not necessarily fail. This is not general evidence for concurrency, other edges or architectures. |

Five repetitions support reproducibility for the fixed build, environment and selected inputs. They are not an independent random sample for estimating production reliability or failure probability, and they do not cover unselected semantic branches. Passing positive controls and failing deliberately state-deficient negative controls show that the method distinguishes these particular errors, not every possible error.

### 3.2 EWK: three separate prototype episodes and the normative matrix

**Fact chain.** [Static] The real `ewk_parse_cookie` writes name/value after accepting the input. Domain/Path are written only when present; otherwise their old values remain. Rejection returns before field writes. Expiry is calculated after acceptance. The measurement history has three distinct episodes. [Original reuse issue](docs/progress/API_0911/W1_CONT/REPORT.md), [later prototype record](docs/progress/ADAPTOR_0912/W2/REPORT.md), [numbered real source](docs/progress/ADAPTOR_0912/W1/raw/008_targeted.stdout)

[Direct-call measurement] In the tested x86_64 configuration, GNU/libc++ strings were 32/24 bytes and EWK output structures 136/104 bytes. Unconverted libc++-to-GNU calls produced SIGSEGV in 5/5 runs on both x86_64 native and an armv7l physical board; GNU same-side calls returned correct fields in 5/5 on each. The ARM probe recorded the signal and exited 139; this was not a default core-dump observation. These results do not predict the same failure in the reverse direction. [Original direct-call matrix](docs/progress/API_0911/W1_CONT/MATRIX.md)

1. The early input/final-output-only prototype passed a fixed input but returned success with incorrectly changed retained fields in 5/5 reuse runs. This does not mean all four fields were wrong in every run.
2. In the first subsequent state-aware prototype, both sides instantiated temporary `EWKCookieContents` objects. Same-name WEAK HIDDEN destructors coalesced, and the GNU object entered an incorrect destruction/release path. Both adaptor modes produced SIGSEGV in 5 runs each; the same-side control passed. These failures are not included in the later normative matrix.
3. The front was revised to use four independent local strings, avoiding generation of the same wrapper-class destructor on both sides. The table below records this version. The negative mode deliberately disables initial-state copying but still calls the real provider. [Object symbols](docs/progress/ADAPTOR_0912/W2/raw/007_object_symbols.stdout), [failure backtrace](docs/progress/ADAPTOR_0912/W2/raw/006_debug_state.stdout)

| Environment | GNU same-side control | Revised adaptor: libc++ to GNU | Deliberately omitted initial state |
| --- | --- | --- | --- |
| x86_64 native | 60/60 scenarios correct | 60/60 correct | 20/60 correct; 40/60 wrong |
| armv7l physical board | 60/60 scenarios correct | 60/60 correct | 20/60 correct; 40/60 wrong |

Each cell comprises 5 processes, each executing 12 scenarios: 6 cells, 30 runs and 360 scenario observations overall, comprising 120 revised-adaptor, 120 same-side and 120 negative-control observations. **This is not 360 successful adaptor calls or 360 independent processes.** All 60 consumer-side scope destructions completed in every path on each architecture, including the wrong-value negative mode. Normative runs retained `/proc/self/maps`; four strings were checked by length and hexadecimal bytes, and time values against the interval of the same invocation. [Matrix](docs/progress/ADAPTOR_0912/W2/MATRIX.md), [native index](docs/progress/ADAPTOR_0912/W2/v2native_matrix.json), [board index](docs/progress/ADAPTOR_0912/W2/board_matrix.json)

The 12 scenarios cover full input, missing/single attributes, empty Domain, three rejection cases, long strings, fixed Expires, Max-Age=60 and consecutive successful reuse. Two important observable behaviors remain outside this coverage:

- [Static] The public structure provides no default member initializer for `expiry_date_utc`. With `EWKCookieContents c;`, the original function can write it on success without first reading it, whereas the prototype unconditionally reads old expiry before calling the provider. Existing tests initialized the output object and did not exercise this indeterminate-value read. Its potential undefined behavior must not be described as an observed crash. [Public structure](docs/progress/P7_0909/stage2/snapshots/evidence/chromium-efl/55f75e1f273b_ewk_cookie_parser.h), [front lines 10-11](docs/progress/ADAPTOR_0912/W2/frontend.cpp)
- [Static plus inference] The original function does not modify the domain string when Domain is absent; the prototype unconditionally swaps all four strings on success. A previously borrowed long-string `c_str()` pointer may become invalid when the temporary is destroyed, even though field bytes match. Address/reference lifetimes have not been tested. Equal values do not establish equivalent behavior for an unmodified field. [Front line 20](docs/progress/ADAPTOR_0912/W2/frontend.cpp)

Additional untested branches include Max-Age=0 (this source uses the current time, not session expiry 0); precedence when Max-Age and Expires coexist; invalid, empty, negative or trailing-nondigit Max-Age without fallback to Expires; empty Path; duplicate attributes; non-UTF-8, embedded NUL and length boundaries; and success followed by rejection on the same object. Source rules are distinguished from runtime results. A Boost NUL sample does not substitute for EWK measurements.

**Excluded approaches.** Omitting initial state, merely adding hidden visibility, or placing the same named wrapper object in both TUs fails the observed cases. The experimental catch-all-to-status-to-local-`runtime_error` policy does not preserve original exception types, partial writes or cancellation. It is not a production error contract.

**What would refute or extend the conclusion?** A field discrepancy for the same identities, input and initial state would refute the limited field-repair result. To rebut “transparent deployment has not been established,” evidence must cover state, references, errors, original-entry routing and the actual migration direction, **GNU consumer to real libc++ EWK provider**. That direction remains `NOT_OBSERVED`. The latest retry of the real parser compilation still fails on missing `base/functional/callback_forward.h`; the required provider is `NOT_AVAILABLE`, not a measured reverse-direction failure. [Build prerequisite recheck](docs/progress/P11_0916/experiment3/REPORT.md)

### 3.3 Other interfaces cannot inherit EWK's passing results

| Interface | Fact chain | Excluded interpretation | Evidence needed to refute or extend the result |
| --- | --- | --- | --- |
| `SetSyncBinaryReply` | [Static] A plugin calls back into the engine through an interface table. A valid instance receives and copies the vector; the return path later retrieves it. [Representation sample] Both x86_64 samples are 24 bytes with three pointers. The real callback has not run. | Equal sizeof is not ABI safety; a Node/service no-op is not a positive control; an unregistered dummy instance is not the real chain. | Real registration, instance creation/removal, both directions, every byte and length, reply/no-callback sequences, borrowing period and destruction. |
| binder to Dali | [Static] C# uses C-named entries; the binder internally calls C++. [Measured] Property subsets passed exactly in both directions. Replacing only core left missing GNU-vector-mangled symbols on both architectures. After supplying the board GLES path, offscreen construction failed to complete in 5 attempts of 30 seconds each. | Property values do not establish full object behavior. No direct C++ ABI exposure in C# does not remove the internal boundary. Timeout does not prove that the board lacks graphics support. | Complete dependencies/initialization, Control, virtual dispatch, identity, reference counting, destruction, threads and callbacks. Handle proxies are not ruled out. |
| Selected Native wrappers from four families | [Static plus compile/link] 8 ELFs across both libraries; selected platform imports are C entries, with string conversion local to the application. | Not eight fully executed ELFs or safety of every member in four families. A C export name alone is insufficient. | Reclassify if a selected instantiated path actually passes C++ objects/exceptions across the platform entry; inspect other members separately. |
| Six conditional Dali Get overloads | [Static] bool/float/int32/String/Array/Map overloads return false without writing output on a type mismatch. | Copying back a zero-initialized temporary even on failure changes the old value. These are not six new runtime experiments. | Real matching/mismatching types, nonempty old values, nested types and ownership controls. |

Evidence: [real chains and initialization](docs/progress/ADAPTOR_0912/W1/REPORT.md), [Native scope](docs/progress/API_0911/W1/REPORT.md), [per-definition state inventory](docs/progress/ADAPTOR_0912/W2/STATE_SCOPE.tsv), [interface criteria](docs/progress/ADAPTOR_0912/W3/REPORT.md). The inventory denominator is 11 named definitions: 7 conditionally retain output, with another 2 callback/cache-sequence candidates. This is not a platform-wide proportion.

## 4. Implementation size, maintenance and generation

The existing EWK conversion code has 56 lines: protocol header 8, backend 26 and front 22, including comments and blank lines; the probe adds 49 lines. This is not a production estimate. The 61 and 38 export-start lines in two inspected Dali wrapper files are not conversion-edge counts. Complete effort remains `NOT_AVAILABLE`: complete contracts, the reverse-direction target and production integration design are absent. [Counting evidence](docs/progress/ADAPTOR_0912/W3/raw/002_workload.stdout)

| Area | Responsibilities and sizing boundary |
| --- | --- |
| EWK | Interface owners define updates/errors; adaptor owners maintain conversion and entry lookup; build owners maintain ABI flavor, exports and versions; test owners maintain bidirectional differential/reuse matrices. This is not simply four field assignments. |
| Binary replies | Beyond a pair of byte conversions, engine/plugin owners must handle interface-table versions, instance lifetime and reply ordering. Data retained beyond a borrowed callback needs the agreed copying policy, not a stored dangling pointer. |
| Dali | Scalars need a success flag and preservation of old values; strings need encoding/length; Array/Map need recursive protocols. Full objects add proxies, virtual dispatch, ownership, threading and callbacks. “A few dozen lines per interface” cannot cover all of this. |
| Selected Native C edges | No additional standard-library conversion is needed for those edges; existing handle, callback and exception constraints remain. |

[Static] EWK currently performs multiple copies: input and old fields into the provider side, returned fields into local strings, then swaps; temporary allocations exist. Production latency, peak memory and complete allocation counts are `NOT_OBSERVED`, not unmeasurable. Existing alternating-order/repeated-timing practice could inform a dedicated benchmark, but lock microbenchmark values are not adaptor overhead measurements.

[Design, not implemented] Generation can have three levels: use AST/DWARF or equivalent information from both real build configurations to generate signatures/conversion skeletons; generate sizeof/offsetof/export-version drift guards; and generate real-provider differential/reuse tests. Layout guards do not prove compatibility. Update, failure, reference and ownership semantics require maintained human-authored rules and cannot generally be derived from headers alone. Version nodes reduce lookup ambiguity; they do not remove revalidation after changes to semantics, compiler, optimization/LTO or ABI flavor.

[Unverified design] EWK could share a parser core that reports which fields were actually written, an update mask, and commit only those fields. Domain/Path presence must come from the real parser result, not guesses based on emptiness or equality. This may avoid reading old expiry and replacing untouched fields. Partial writes, allocation failures, exceptions and cancellation still require proof. This document neither modifies the prototype nor labels this design a completed fix.

## 5. General criteria and deployment contracts

**Fact chain.** Existing samples show that fully encodable data can be converted when lifetimes remain local. Stateful interfaces additionally require protocols for no-write, failure and partial-update cases. Ownership, internal references, virtual calls and exceptions are not automatically solved by pointer/length pairs, but this does not establish that every proxy is impossible.

| Category | Design that may be assessed, not implemented support | Missing transparency evidence |
| --- | --- | --- |
| Known exceptions | Catch locally, transfer type identifier/data, rethrow a local exception on the other side. | Arbitrary user type identity, original catches, partial state and cancellation; catch-all must not swallow forced unwind. |
| Smart pointers/ownership | Keep the object on its owning side; a local proxy control block releases it through a callback. | use_count, weak_ptr, aliasing ownership, exception safety, threads and release timing. |
| Polymorphic objects | An old-ABI facade/proxy translates each method into handle operations. | Direct fields, already-inlined code, arbitrary subclasses, RTTI, identity and callbacks. |
| future/promise | Local objects plus a remote result/error protocol. | Deferred execution, thread-exit readiness, broken_promise, waiting and destruction semantics. |
| Iterators/internal references | Remote-operation proxies or agreed read-only snapshots. | Old inlined dereferences, live aliasing and invalidation rules; a snapshot is not the original reference. |

### 5.1 Two candidate causes of ordinary-exception failure

**Fact chain.** A personality function selects handlers and cleanups during unwinding; exception_class identifies the exception's runtime origin. Two mechanisms must be distinguished: a foreign exception reaches a runtime that does not perform ordinary typed matching for it; or a native exception enters matching but its RTTI (runtime type information) identity or comparison fails. Catch-all alone does not distinguish them.

**Measured.** The existing controlled GNU plugin throwing `BoundaryError{77}` into a libc++ main program was rerun five times each on x86_64 native and armv7l **QEMU user mode**. Both throw and personality actually enter libc++abi; the identifier is LLVM's `0x434c4e47432b2b00`, and typed matching executes. The identically named types have different RTTI/name addresses; ARM directly records a false matching result. **This fixed failure is a type-identity/matching failure, not the foreign path.** The earlier exit 12 on an ARM physical board remains historical evidence; this round did not instrument the physical board. [Traces and limits](docs/progress/P11_0916/experiment1/REPORT.md)

**Measured controls.** GNU same-side catches 77 with guard=1 in 5/5 runs. LLVM same-side with the original export conditions also exits 12 in 5/5. The failure is therefore not exclusive to mixing standard libraries. Adding only `--export-dynamic` to the isolated consumer link makes the mixed case catch 77 with guard=1 in 5/5; five further traces show unified RTTI/name addresses. This is not a linking recommendation for arbitrary applications. A different **forced-unwind** experiment still exited 134 with that option; the subjects must not be conflated. [Controls](docs/progress/P11_0916/experiment1/CONTROLS.json), [unified-identity trace](docs/progress/P11_0916/experiment1/raw/export_trace_r1.stdout), [earlier forced-unwind control](docs/progress/R39/R39_report.md)

**A separate measurement.** In the version-isolated `throw int` fixture, GNU throw produces `0x474e5543432b2b00`, then actually enters libc++abi personality and catch-all. This confirms **foreign exceptions as a separate real path**, not as the cause of the BoundaryError above. [Binding observations](docs/progress/P11_0916/experiment1/R14B_TRACES.json)

**Excluded generalizations.** Exit 12 is not direct evidence of foreign handling, and typed catches do not invariably fail across libraries. The name-deep-comparison macro remains untested; the identity control does not establish its full exception/destruction contract. Two libraries also do not imply independent heaps. Limited new/delete traces establish neither safety nor failure for all loading combinations or all nine standard exception classes. [Allocation bindings](docs/progress/API_0911/W2/raw/008_binding_trace.stderr)

**What would refute this?** Opposite throw/personality/identifier or matching observations under the same binaries, libraries and loading conditions, or evidence that the observed parameters were invalid. Success after changing loading conditions changes scope, not the original observation. Ordinary exceptions and forced unwind still require separate acceptance tests.

### 5.2 Coexisting exception runtimes and a key-path candidate

**Prior measurement and current rerun.** The 96-cell exception matrix is entirely x86_64. **Both configurations use libc++abi; it is not a libcxxabi/libsupc++ comparison.** Version isolation, runtime combinations and loading conditions change together. It directly concerns Option B's third deployment contract, but is not an EWK implementation of Option B. [Original matrix](docs/progress/P11_0916/experiment1/snapshots/R14b_matrix_all.tsv), [original build](docs/progress/P11_0916/experiment1/snapshots/R14b_build_l2.log)

Five repeats per cell give 480 runs: 48 same-side cells yield 240 type/cleanup/order passes; 24 version-isolated cross-side cells yield 120 catch-all results; 16 globally visible cross-side cells in the other configuration yield 80 exact passes; 8 local-scope cross-side cells yield 40 signal terminations, comprising 15 SIGABRT and 25 SIGSEGV. Five runs have a different signal from the corresponding historical single observation; the old signal ratio is not an invariant. Eight selected cells also have five binding observations each. [Per-cell results](docs/progress/P11_0916/experiment1/R14B_REPEATS.json), [interpretation](docs/progress/P11_0916/experiment1/REPORT.md)

**New measurement.** `LIBCXX_CXX_ABI=libsupc++` moves from deferred work to a **key-path candidate**. libc++ was built with this setting for both architectures in tmp, incorporating GNU static libsupc++ ABI support into the experimental libc++.so. It was compared with the original libcxxabi artifacts using default GNU and process-preloaded LLVM unwinders. Actual function bindings were observed; mapping both libraries is not described as having only one unwinder.

| Exception runs, five per cell | libcxxabi + GNU | libcxxabi + LLVM preload | libsupc++ + GNU | libsupc++ + LLVM preload |
| --- | --- | --- | --- | --- |
| x86_64 native | catch-all / 12 | catch-all / 12 | typed=77, guard=1 | typed=77, guard=1 |
| armv7l QEMU user mode | catch-all / 12 | catch-all / 12 | typed=77, guard=1 | 5/5 SIGABRT |

Every corresponding ordinary C-buffer control produces `old:ping`, length 8 and guard=1. The ARM failure reports personality result 5 as unknown; unfinished typed catches/cleanup are not passes. These are 80 program runs across both architectures, plus debugger observations. The ABI builds are not a single-variable experiment excluding every build/configuration confounder. [Configurations, identities and limits](docs/progress/P11_0916/experiment1/REPORT.md)

**Excluded conclusion.** Selecting libsupc++ does not close the entire exception contract, and x86_64 passes cannot be extrapolated to ARM. **Evidence needed to refute or strengthen this:** explain and resolve the ARM failure, establish production runtime bindings, type identities, exception layouts and lifetimes, and cover cancellation and other old binaries. Candidate status is not implementation advice or approval.

### 5.3 Structure of the forwarding layer

**Fact chain, static inspection.** Real `libchromium-ewk.so` artifacts on three architectures define the old `ewk_parse_cookie` name. The function obtains a handle, looks up the implementation by name with `dlsym`, restores the original arguments and jumps indirectly. This is not an ordinary `UND ewk_parse_cookie@version` reference; the current target exports have no named version node. [Samples and coverage limits](docs/progress/P11_0916/experiment2/REPORT.md); disassembly: [x86_64](docs/progress/P11_0916/experiment2/raw/elf_07_forwarder.stdout), [armv7l](docs/progress/P11_0916/experiment2/raw/021_arm_thumb_forwarder.stdout), [aarch64](docs/progress/P11_0916/experiment2/raw/elf_01_forwarder.stdout).

**Unverified design/inference, not a measurement.** The forwarding layer is already a thin shell and is a candidate location for conversion. **This has not been tested; mechanical feasibility depends on which implementation `dlsym` finds.** Handle search scope, default/versioned entries, recursion or bypass, the old argument layout and the converted target must still be verified. Static structure on three architectures is not a running conversion prototype on three architectures.

**Excluded inference.** Missing old version requirements do not automatically rule out every same-DSO compatibility facade; defining two versions does not automatically identify the caller's object layout either. Three independent old-application samples remain unavailable, preventing a conclusion about all shipped ELFs. **Evidence needed to refute or strengthen this:** observe resolved addresses and maps on the real old forwarding chain with a libc++ implementation, and assert bidirectional state, values, references and destruction. If the handle cannot route to the correct side, this candidate fails. The business question must first define the old-ELF population and allowed platform routing changes; unverified deployment alternatives are not ready for an unconditional business choice.

### 5.4 Public-header static_assert sentinels: assessment only

A static_assert rejects code at compile time when a condition is false. The following is **design assessment; no platform header was changed**.

| Location | Possible checks | Protection and limits |
| --- | --- | --- |
| `ewk_cookie_parser.h`: string input and EWKCookieContents | Approved interface-side ABI tag, standard-library/dual-ABI settings; architecture-specific string/structure size and member offsets where applicable | Reject newly compiled callers with unapproved layouts; does not protect old ELFs or prove dlsym routing, state preservation or exceptions |
| `XW_Extension_SyncMessage.h`: vector callback slot | Interface-table version, compilation-side ABI tag and relevant type traits | Reject mismatched rebuilt plugins; sizeof alone cannot distinguish vectors that are both 24 bytes, or verify instance lifetimes |
| Dali C++ and build-adaptation headers used inside the binder | Consistent binder/core/adaptor/toolkit ABI build tags | Internal C++ dependency check, not a requirement for the outer C# application to share a C++ library; no arbitrary polymorphic-object guarantee |
| Selected Native pure-C declarations | Existing C type and version constraints | Do not add a blanket rejection based only on the application's standard library, which could reject paths carrying no C++ objects |

Macros such as `_LIBCPP_VERSION` and `_GLIBCXX_USE_CXX11_ABI` are configuration inputs, not full compatibility proofs. Interface owners must define the allowed set; retaining a GNU facade must not imply rejecting all GNU callers. Use offsetof only where its type requirements hold, and do not hard-code one size for all architectures. Sentinels can be designed independently, but changing source needs separate approval. None is implemented here or claimed to solve old-application compatibility.

Locations: [EWK declaration](docs/progress/P7_0909/stage2/snapshots/evidence/chromium-efl/55f75e1f273b_ewk_cookie_parser.h), [callback slot](docs/progress/P7_0909/stage2/snapshots/evidence/webapi-plugins/353da9cbd3a2_XW_Extension_SyncMessage.h), [Dali Control](docs/progress/P7_0909/stage2/snapshots/evidence/dali2-toolkit/d8297163615c_control.h), [Native C declaration](docs/progress/P7_0909/stage2/snapshots/native/capi-appfw-app-control-devel/usr/include/appfw/app_control.h). Existing SDK-qualification gaps remain; these candidate locations are not all declared to be shipped SDK headers.

**What would refute or extend the conclusion?** Proving a complete proxy contract for a specified interface would rebut an overly broad reading of “the simple byte bridge does not cover this contract.” One proxy sample cannot establish transparent interoperability for arbitrary objects. Deployment must establish:

- Which entry an old ELF resolves, and how versions, SONAME and plugin discovery agree; whether calls misbind, recurse or bypass the adaptor. A common C name cannot identify the argument ABI automatically. A probe-only configure call is not evidence of unchanged old-application routing.
- Correct treatment of implicit members, weak symbols and inline/templates. Existing 30-run COMDAT controls selected the first definition in the specified GNU/ELF/lld setup; this is not a universal linker guarantee. `_LIBCPP_HIDE_FROM_ABI` tagging depends on hardening, assertions, exception configuration and version. [Link controls](docs/progress/R112/REPORT.md)
- Symbol versions protect requirements recorded in the final ELF, not every old `.o/.a`. Four binding cases were tested five times each: relinked old objects may select a new default node, while new-header code linked against the old library may still record the old node. Later implemented compatibility wrappers must be distinguished from early mechanism exploration; a historical report cannot establish that no wrapper exists now. [Binding controls](docs/progress/R113/REPORT.md)
- Parallel artifacts need more than separate directories. In adapted aarch64 Boost candidates, all 30 examined old edges retained at least one same-mangled-name symbol; 15 retained their entire old symbol sets. This is a symbol check, not 30 runtime failures. Matching rules must be explicit; old clients cannot be assumed to reject a new library automatically. [Intersection record](docs/progress/R111/ITEM2_BOOST_TRANSITION_REPORT.md)
- Inline statics and template static state may split identity. This is a risk to inspect, not an observed product failure. Dynamic export graphs also miss some inline/template behavior and runtime weak binding.
- The actual libc++ ABI flavor must be checked in the target artifact. Upstream Chromium's private `__Cr`/ABI version 2 is not the same configuration as the inspected GNU-built Tizen snapshot. The 42 examined historical ELFs used GNU; upstream settings do not prove that a third flavor already exists after migration. The future artifact remains unobserved. [Upstream configuration](docs/progress/R116/REPORT_ZH.md), [Tizen configuration entry](docs/progress/ADAPTOR_0912/W1/REPORT.md), [42-ELF identities and scope](docs/progress/R19b/R19b_report.md)

## 6. Options: preserve all mechanisms; do not call a policy exclusion technical impossibility

### 6.1 All five previously documented options

The table retains all five mechanisms and trade-offs from the existing background. The historical table concerned Boost boundaries; its edge counts are not counts of current EWK interfaces. [Original five-option table](docs/progress/R110/DECISION_BACKGROUND.md)

| Existing option | What it avoids and what it costs | Applicability under the current decision |
| --- | --- | --- |
| Rebuild the provider and all C++ consumers with one runtime | Removes cross-library layout/destruction issues on covered edges. Component owners must close dependencies, build/deployment order, plugins and external old-object coverage. | Coordinated internal migration remains an option; outer C#/JS applications may be unchanged. Rebuilding old Native C++ clients is not zero change to old binaries. |
| Retain the GNU provider; use C ABI/opaque handles and same-side creation/destruction at its boundary | Prevents cross-ABI interpretation/release. Platform owners define data, errors and lifetimes; callers adopt the contract and validation remains necessary. | Retaining a complete GNU Chromium component is excluded by the owner. The C-data/handle mechanism remains applicable to a new libc++ core; the entire mechanism must not be deleted. |
| Supply isolated parallel artifacts for both ABIs | Each caller uses a matching provider, retaining more original C++ API. Release owners maintain symbol/SONAME/plugin-discovery isolation and dual release/update matrices. | A full GNU Chromium copy is excluded now, not technically disproved. Two thin facades are not two complete components; see Option B. |
| Isolate processes and serialize through IPC | Objects, exceptions and allocators remain process-local. Both ends maintain protocols, copying, recovery, deployment and changed latency. | Call paths need redesign. A retained complete GNU component conflicts with the decision. An old-ABI front plus IPC can be considered, but still needs a compatibility facade; old objects do not inherently cross processes. |
| Accept existing mixed boundaries | Avoids interface changes but does not remove observed wrong values, termination, destruction failures or unmeasured risks. | This is not a support guarantee or a restoration of functionality. Restricting supported combinations instead requires explicit withdrawal of mixed-call commitments, for business approval. |

### 6.2 Added deployment and contract variants

| Option | Mechanism and application impact | Status and responsibility |
| --- | --- | --- |
| Option A: selective nonmigration | Keep the complete component and its C++ dependencies on GNU; an old GNU client does not cross libraries. Isolation still needs checking. | **Excluded by the component owner's decision.** The historical GNU build demonstrates that past configuration, not its sustainability for arbitrary future versions. Reassess if conditions change. |
| Separate-DSO adaptor | Two thin facades convert through POD and may preserve the old Native binary entry. | Mechanism samples exist; old-entry deployment, reverse direction and contracts remain open. Platform owners maintain both builds and routing. |
| Option B: one DSO, dual versions and symbol-version routing | GNU/LLVM facades surround one libc++ core; a selected entry converts objects. This is an adaptor deployment variant, not a new mechanism. | Permitted, not a completed target dual-version implementation. Three production contracts remain unclosed, with partial exception evidence in section 5.2. Existing name-based dlsym forwarding must first be resolved; old-ELF compatibility is not established by defining version nodes. |
| C data interface | Explicit bytes/lengths/handles; applications directly adopting it change source. | Concrete designs exist in 6.3 but are not implemented. Platform and application owners define errors, buffers and coexistence. |
| Header-inline marshaling | Retain source-level calls; compile local wrappers in the application and call a new C entry. | A C-contract variant: no source changes may be needed, but **rebuild/relink is required**. It does not protect already shipped ELFs; old semantics still need checking. |
| Migrate applications and dependencies to libc++ | Removes the mixed-library premise when both sides and dependencies match. | Requires source, build and dependency migration conditions. These are unavailable for the entire external application population. |
| Accept and document restrictions | Support only confirmed library/version/interface combinations. | Platform owners state the boundary; business/application owners accept it. This does not repair the original mixed interface. |
| Preload interception | Another loading/routing variant; it does not replace conversion or state contracts. | Launch-environment changes fail this document's strict zero-change definition. Lookup order, handle scope and production loading remain untested; this is not a validated deployment method. |

After Option A is excluded by the component owner's decision, the adaptor is the only remaining zero-application-change option **within this table's goal of preserving the required function and original entry of old Native C++ binaries, counting Option B and old-ABI compatibility facades within the adaptor family**. This is neither a uniqueness theorem over all future mechanisms nor a deployment guarantee. Coordinated platform-internal migration leaving outer C#/JS applications unchanged is a distinct case that must remain visible.

**Prerequisite, not an established property:** this does not establish zero change for all shipped ELFs. Existing name-based forwarding and the missing application samples require the business question to be restated in terms of a defined binary population, allowed routing changes and entry semantics. Separate-DSO and same-DSO facades remain unverified designs under those prerequisites.

### 6.3 Existing C interface designs and change locations

These reconnect existing designs; **they are not implemented APIs**. [Per-interface design](docs/progress/API_0911/W3/OPTIONS.md), [locations and hashes](docs/progress/API_0911/W3/known_points.json)

```c
#include <stddef.h>
#include <stdint.h>
struct ewk_text_out { char *data; size_t capacity; size_t length; };
struct ewk_cookie_text_out {
    int64_t expiry_utc_seconds;
    struct ewk_text_out domain, name, path, value;
};
int ewk_parse_cookie_bytes(const char *input, size_t input_length,
                           struct ewk_cookie_text_out *output);
```

Location: the old declaration at `ewk_cookie_parser.h` line 70. Known starting points are 1 header, 1 existing declaration, 1 new entry and 2 C structures. The real implementation begins at `ewk_cookie_parser.cc` line 30; parser behavior must not be independently guessed or duplicated. Length units, NUL handling, required length on insufficient capacity, failure/partial writes, field aliasing and expiry rules remain to be defined. These counts are not the full production file/function estimate. Retention time for the old entry depends on user coverage and a business decision; no duration is established.

```c
void (*SetSyncBinaryReplyBytes)(XW_Instance instance,
                               const uint8_t *data, size_t length);
```

Locations: the callback slot at `XW_Extension_SyncMessage.h` line 36, and the caller starting at `extension.cc` line 340 with the call at line 348. A new interface-table version or explicit negotiation is required; changing the old slot's function-pointer type is not compatibility. The real engine receiver and registration chain also need corresponding changes. Borrowing duration and receiver copying must be contractual. Either design can support an inline wrapper or an old-ABI facade above it, but the latter retains the adaptor's semantic obligations.

### 6.4 Interface decision summary

| Interface | Direct application exposure | Assessment and evidence strength | Available mechanisms | Business clarification needed |
| --- | --- | --- | --- | --- |
| EWK parser | Native call path exists; formal SDK qualification unconfirmed | Listed one-way field scenarios passed; direct mixing failed; transparent deployment unproven | Adaptor, including B / C data and inline wrappers / application migration / restrictions | Old GNU binaries, errors/reference contracts and support period |
| Binary replies | JS does not directly pass vector; native plugin-to-engine boundary | Byte protocol statically expressible; real callback untested | Coordinated internal migration / adaptor / C byte interface / restrictions | Migratability of third-party native plugins, lifetimes and sequencing |
| binder to Dali | C# crosses an outer C entry; Native direct calls are separate | Property subsets passed; core-only replacement failed to load; complete objects unverified | Coordinated migration / object proxies or C handles / restrictions | Dependency set, old bindings and object operations |
| Selected Native wrappers | Local application-side inline wrappers | Static and compile/link evidence for the selected platform C edges | Retain the limited C boundary; no duplicate conversion needed | Expansion to other members and callbacks |
| Six Dali Get overloads | Depends on Native/binding call path | Output preservation on failure is a static fact; no added runtime tests | Success flags / byte or recursive protocols / coordinated migration | Old-value, type and ownership contracts |

## 7. Uncovered scope and review suggestions for later evaluation

### 7.1 Limits that must not disappear

The real libc++ EWK reverse direction, real browser receiver, complete Dali objects/virtual dispatch, production old-ELF routing, complete exception/cancellation and partial-write contracts, concurrency/reentrancy, internal-reference lifetimes, complete allocation/performance behavior, LTO and other toolchains remain unverified. Section 5 contains measured ordinary-exception subsets and failures, not coverage of the entire contract. There are no aarch64 **adaptor** measurements; other aarch64 Boost controls cannot fill that gap. SDK qualification, the external-application denominator and all cross-library interfaces are unavailable, not nonexistent.

The static inventory also contains bundle `GetString/GetKeys`, Web-engine `EvaluateJavaScript` with string/function, chart-label vectors, cion vector/shared_ptr interfaces, and component-factory map/unique_ptr/virtual interfaces. These are candidates for further investigation, not proven SDK exposure or measured failures. [Candidate inventory](docs/progress/P7_0909/stage2/REPORT.md) Dynamic-symbol graphs have blind spots including inlining, templates, duplicated static state and runtime binding. The four selected groups are not all platform issues.

### 7.2 Suggestions and why they are not implemented in this revision

The following suggestions remain **for later evaluation**; their expected outcomes are not stated as facts. The shared libsupc++ ABI-runtime proposal now has the experiments in section 5.2 and has moved to key-path candidate status, not implementation approval.

| Reviewer | Proposal and question to test | Disposition and reason |
| --- | --- | --- |
| Claude Code | `_LIBCPP_TYPEINFO_COMPARISON_IMPLEMENTATION=2`: can name-based deep comparison change the failed shared-BoundaryError typed catch? | Experiment deferred. The suggestion does not prove a fix for the complete type/destruction contract; build variants and bidirectional testing are outside this round. |
| Claude Code | Empty-message exception release involving `_S_empty_rep_storage` | Experiment deferred. Check empty/nonempty representations, actual bindings and release ownership; a symbol name is not evidence of an incorrect release. |
| Claude Code | A veneer, or thin compatibility facade, prototype | Experiment deferred. Verify real old-ABI entry routing through POD to the new core and its contracts. A thin interface layer is permitted, but is not already deployable. |
| Kimi Code | Same-DSO versions, preload routing, three-level generation, CI differential tests, missing input matrix and other low-level ABI-runtime precedents | Deployment/generation options and gaps are documented. Target implementation, tool integration, applicability of external precedents and performance tests are deferred; a reference mechanism is not Tizen validation. |
| Codex | Update mask/shared parser core, two-configuration generation, exception/smart-pointer/polymorphic/future/iterator proxies, real registration/borrowing checks | Conditional designs are documented; indeterminate reads and pointer lifetime are explicit static limitations. Implementation and validation are deferred without changing experimental files. |
| Kimi Code and Codex | Preferred ordering, coordinated migration/old-component retention and industry isolation/proxy examples | Mechanisms and costs are retained, not the ranking. The manual decision excludes complete GNU Chromium retention. Industry precedents remain leads for later applicability checks, not new platform experiments. |

The opening supplementary review's authorship awaits confirmation; its comments are retained by source paragraph in the disposition table without assigning them to a reviewer. Of the four original Claude suggestions, three remain deferred above and libsupc++ is evaluated in section 5.2. Historical claims about the subject of the typeinfo control and wrapper availability remain limited to their actual evidence and date; the new direct BoundaryError diagnosis is recorded separately in section 5.1.

## 8. Questions for reviewers and business decision-makers

1. Is the finite-sample conclusion for the two-TU mechanism appropriate? What additional falsifiable evidence is needed for a production contract?
2. Beyond the 12 EWK scenarios, are there further gaps involving uninitialized fields, references to untouched fields or error branches?
3. Are the limits of custom layouts, single-TU approaches and same-DSO versions accurate, without confusing compilation and runtime binding?
4. **With Option A excluded, can the old-name routing prerequisite first be established?** Only then can separate-DSO and same-DSO adaptor deployments be compared; unverified production alternatives should not be presented as an unconditional business choice.
5. What repetitive work can generation remove, and which semantics remain the interface owner's responsibility? Are a shared core and update flags needed?
6. Which precise contracts must complex-object handles/proxies preserve, and which old inline calls cannot be intercepted by a new facade?
7. Which target flavors, dependencies and SDK/application scopes need business confirmation, and is relevant existing evidence still missing?
8. Does any wording still turn source rules into measurements or limited passes into general compatibility?
9. How should old-ELF routing, weak symbols, allocation/release, exceptions/cancellation, real callbacks and deployment updates be accepted without treating successful loading as correctness?
10. **How will the adaptor's libstdc++ interface layer track libstdc++ ABI changes?** Who owns `_GLIBCXX_USE_CXX11_ABI`, exported versions, toolchains, old-binary combinations and revalidation records, and which changes require a new facade rather than replacing an old entry?

This document supplies facts, boundaries and testable options. Mechanism feasibility does not replace a business decision, and an unverified case is not declared permanently impossible.
