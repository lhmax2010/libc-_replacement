# Cross-package boundaries by architecture and new standard-library findings

For human review. This document consolidates existing records only. It adds no source investigation, probes, board runs or compatibility commitments. Read every confirmation together with its evidence class and architecture.

## Scope totals and counting units

The original **44 candidate entries** map to **16 directed source-package pairs and 21 symbol edges**. Adding WaitUntil from a different consumer and the Reader constructor brings the x86_64 register to **18 pairs and 23 symbol edges**. Candidates, pairs and symbols are different units: several candidates can share one edge, and several edges can share one pair. The complete original mapping is available as [Markdown](CANDIDATE_MAP.md) and [TSV](CANDIDATE_MAP.tsv). {{base-map}} {{edges}}

The additional records use expression labels such as `steady_clock::rep` and `streamoff`; they do not constitute another completed candidate-classification batch. “44 → 18/23” describes the progression of the register, not 44 disjoint types producing 23 independent defects. Different WaitUntil consumers are separate edges. Reader remains an object boundary, not a direct builtin-type symbol collision. {{extra-proofs}} {{arm-final}}

| Architecture | Confirmed package references | Evidence coverage | What is not established |
| --- | --- | --- | --- |
| x86_64 | 18 pairs / 23 symbol edges | Frozen selection of 3,085 ELF files out of 10,946 runtime ELF files on this architecture, approximately 28% | Not the whole platform; not 23 measured mixed-library failures |
| armv7l | 2 targeted pairs / 2 symbol edges: ui-foundation → dali2 and zypper → libzypp | Actual ARM consumer UND entries, provider definitions and NEEDED/SONAME correspondence | Remaining edges not rechecked; the complete ARM denominator is NOT_OBSERVED, not 16/21 or 18/23 |
| aarch64 | Package-reference total and each listed edge: NOT_OBSERVED | Only the specified type measurements below | Type measurements do not establish package references or successful execution |

Sources for these denominators and architecture distinctions: {{scope}} {{arm-elf}} {{arm-count}} {{arm-final}}. The ARM records were obtained from actual ARM ELF files, not inherited from x86_64. Their raw symbols, file paths and full hashes are in the [per-edge appendix](EDGES_en.md).

## Complete boundary register

This register records **static findings** from public declarations, object-storage chains and actual released GNU ELF references. It is not a mixed-standard-library execution matrix. Calls directly into the standard-library runtime itself are excluded. Edges found in test artifacts remain registered, but do not establish direct third-party application usage. Transfer forms and ownership descriptions retain the original evidence and its limits. {{base-map}} {{edges}}

{{edge_table}}

The complete machine-readable table is [EDGES.tsv](EDGES.tsv). Each row retains x86_64 consumer/provider paths, SHA256 values and the original symbol. Rows with ARM evidence additionally preserve ARM paths, hashes and raw UND/DEF lines. `NOT_OBSERVED` does not mean absent.

### Correct classification of Reader

**Fact chain — static evidence:** the actual call is `Reader(InputStream const&, Validate const&)`. `InputStream::_size` is `DefaultIntegral<std::streamoff,-1>`, whose implementation stores a value member. The containing class also has a `_name` string, `_stream` and other members. The constructor symbol encodes the names `InputStream` and `Validate`, not the internal type of `_size`. The saved InputStream/DefaultIntegral header excerpts and ELF records support this chain. {{structure}} {{extra-proofs}}

**Excluded explanation:** this Reader symbol is not evidence of a direct `streamoff` parameter causing an `l/x` name collision. Cross-compiling the real ARM headers with each library produced the same Reader reference name; the actual GNU package reference also exists. Neither fact removes the object boundary or proves that its internals can be interpreted or destroyed across libraries. {{arm-signatures}} {{arm-elf}}

**What would overturn this finding:** disproving the association between the frozen headers and ELF artifacts, or showing that the transferred object is not the recorded InputStream, would require revising the edge. Establishing mixed-library compatibility still requires content, state and destruction tests against the matching libc++ provider. Equal function names are insufficient. The retained classification is “cross-package C++ object, by reference and through member state”; the edge count is unchanged.

## New findings suitable for an external report

### The same high-resolution clock name can mean different time

**Fact chain:** in the measured x86_64 configuration, libstdc++ expands `high_resolution_clock` to `system_clock`, whereas libc++ expands it to `steady_clock`. Both clock types were measured as empty and 1 byte in size. Saved definitions locate the GNU alias in `bits/chrono.h` and the conditional libc++ alias in `__chrono/high_resolution_clock.h`. libc++ selects steady_clock when monotonic-clock support is enabled and can select system_clock otherwise; this is not an unconditional statement about every libc++ build. {{clock}} {{clock-gnu}} {{clock-cxx}}

**Static semantic evidence:** the saved GNU definition describes system_clock as system-wide wall time with `is_steady=false`, and steady_clock as increasing uniformly with `is_steady=true`. Empty clock tags have no non-static data members. That excludes different stored clock-tag layouts as the explanation for this finding; **it does not establish equal time semantics or cover the state carried by a time_point**. Installed-header samples include the Boost alias import, LLDB StatsClock/StatsTimepoint and LLD startTime member. These samples alone do not add confirmed consumer edges. {{clock-gnu}} {{clock-cxx}} {{clock}}

**Connection to confirmed boundaries:** WaitUntil's transfer of a time_point is supported by its public signature, duration/rep storage chain and ELF references. However, its header explicitly uses **steady_clock, not high_resolution_clock**. The established time_point boundary and the separate high-resolution alias difference cannot be combined into a claim that WaitUntil has already produced a measured silent wrong value. {{structure}} {{edges}}

**Concrete wrong-value scenario — inference, not measured:** suppose a GNU component exports a high-resolution timestamp as an integer number of nanoseconds since its clock's epoch. A libc++ component then reconstructs its own high_resolution_clock deadline without converting the clock domain and epoch. The receiver would interpret a wall-time number as a monotonic-clock timestamp. Integer transfer can succeed while representing the wrong deadline, potentially causing an early timeout or an incorrectly long wait. This is a conditional scenario with explicit assumptions: an integer boundary, normalized units, and no epoch conversion. It is not an observed failure on any listed released call path; no wrong value or time offset for this scenario was measured here.

**Excluded explanations:** this semantic risk is not attributed to a size difference, does not treat all chrono types as one clock, and does not assume all mixed C++ symbols will link. Direct transfer of differently named types may fail at link time first.

**What would overturn the scenario:** identical clock selection in the actual builds, explicit validation/conversion of clock domain and epoch, or absence of this reinterpretation would invalidate it. A product-defect claim requires an actual consumer path and a testable deadline result. The current distinction is “aliases and emptiness measured; wrong-time scenario inferred.”

### Equal-width integers can still produce different link names

**Fact chain — measured:** the specified **13 type expressions** resolve to GNU `long` versus libc++ `long long` on x86_64 and aarch64, with actual encodings `l` versus `x`; both occupy 8 bytes. On armv7l they all resolve to `long long`, encoding `x`, occupying 8 bytes on both sides. These are counts of expressions, not separate package edges or failures. Both libraries were measured, with five consistent repetitions per recorded cell and positive controls for type encodings. {{types-x86}} {{types-arm}} {{types-aarch64}} {{arm-final}}

{{type_table}}

The complete source/hash columns are in [BUILTIN_TYPES.tsv](BUILTIN_TYPES.tsv). x86_64 used native execution; armv7l and aarch64 used cross-compilation followed by QEMU user-mode printing. Target headers, macros and compiler configuration determine the types; QEMU does not choose them again. This does not validate product calls on a physical board. The recorded configurations use GNU 14 and libc++ 22.1.8. {{arm-config}} {{aarch64-config}} {{arm-final}}

**Known public exposure — static, not automatically a consumer edge:**

| Provider package and header | Public declaration / structure | Relation to the finding and consumer evidence |
| --- | --- | --- |
| dali2, conditional-wait.h | `using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;`; `void WaitUntil(const ScopedLock& scope, TimePoint timePoint);` | time_point → duration → rep; toolkit and ui-foundation each have an x86_64 edge; only ui-foundation was targeted on ARM |
| podofo, PdfInputDevice.h | `virtual std::streamoff Tell() const;`; `virtual void Seek(std::streamoff off, std::ios_base::seekdir dir = std::ios_base::beg);`; `virtual std::streamoff Read(char* pBuffer, std::streamsize lLen);` | Seek directly takes the integer type. Tell/Read return types are generally absent from ordinary function mangling, so universal link failure cannot be claimed. Existing Tell/Seek identity queries found definitions but no external UND; Read's query was not completed; virtual dispatch is not excluded |
| abseil-cpp, absl/time/time.h | `FromChrono(const std::chrono::nanoseconds& d)` and microseconds / milliseconds / seconds overloads, plus a system_clock::time_point overload | Corresponding duration/clock reps are involved. Existing identity queries did not establish external UND references; inline/template routes remain open. No new edge is added |
| libzypp, InputStream.h / Reader.h | `std::streamoff size() const`; `void setSize(std::streamoff val_r)`; Reader borrows InputStream | size/setSize are saved inline members. Reader's symbol encodes the containing class name, not its internals; the member-type difference is not a direct Reader symbol collision. Its object boundary remains |

Exact declaration locations and source SHA256 values: {{declarations}} {{structure}}. Actual references: {{extra-proofs}} {{arm-elf}}. Scoped negative findings retain the existing function-identity method with a `bundle::Add` positive control; they do not assert platform-wide absence of consumers. {{boundary-control}} No further closed, independent consumer edge is added here for char_traits/off_type, istreambuf_iterator::difference_type or the other listed expressions.

**The two mechanisms affecting WaitUntil:** on x86_64, different duration representations add the newly identified builtin `l/x` factor. Separately, the libraries have different chrono inline namespaces and clock-type identities, an already known factor. On armv7l the first factor disappears, but actual reference symbols compiled from the real headers still differ: the second factor remains. aarch64 measurements cover the types, not WaitUntil's function symbol or business references; LP64 cannot supply the missing evidence. {{extra-proofs}} {{arm-signatures}} {{arm-final}}

**Excluded explanations:** occupying 8 bytes does not make long and long long the same C++ type. This finding is not a 4-versus-8-byte width change or an artifact of different pretty-printed spelling. The comparison uses actual TypeToken mangling, with direct type encodings retained. Return types and containing-object members need not appear in a function name, so not every occurrence implies link failure.

**What would overturn the finding:** equal actual type encodings in a specified target configuration remove the builtin divergence for that configuration; the armv7l measurement is precisely such a counterexample. A particular interface's link consequence also requires the type to enter its symbol and an actual consumer. No relevant parameter/template argument, an explicit compatible entry point, or no mixed-library boundary would defeat that failure inference. Real libc++-provider execution for each edge is not yet established by this work.

**A paragraph ready for external use:** Some standard-library names resolve to different integer types in the two implementations. In the measured x86_64 and aarch64 configurations, those integers occupy the same number of bytes, but the compiler distinguishes long from long long in function names. Parameters or template types involving them may therefore prevent a caller from finding the provider's function. This differs from an object being misread at runtime because its memory layout changed. Return types and internal members do not necessarily change the function name. This integer-selection difference is absent in the measured armv7l configuration; another architecture's finding must not be transferred to it unchanged.

## Paused work and restart conditions

| Work item | State | Restart condition |
| --- | --- | --- |
| 62 identity/material gaps | Paused; 23 primarily C and 39 primarily D; not platform-wide negatives | See [individual gaps](BACKLOG_en.md) and [full TSV](BACKLOG.tsv), retaining local-resolvability and dependency records |
| Result projections of 65 compile-time facilities | 0/65 fully closed; 15,793 RESULT_UNRESOLVED occurrences; stopped | Renewed human authorization, then actual template arguments, instantiation/name-lookup context or result-structure evidence; hand-selected int examples cannot close candidates |
| Candidates 201–495 | 295 not yet bounded | Authorization for the next batch and confirmation of scope/method; no continuation here |
| Remaining 572 expressions on armv7l | Do not test; remain unmeasured | Restart only after an explicit scope change and a product-relevant question; equality of the selected 13 does not establish safety of the others |

Count sources: {{pending}} {{projection}} {{projection-count}} {{base-count}} {{measurement}} {{mechanisms}}. Candidate counts, source occurrences and type expressions are different units and must not be summed.

**Latest projection counts:** 15,770 no-object result occurrences, 10 type-divergent occurrences, 13 standard-library-object occurrences and 15,793 unresolved occurrences. The 13 object-result occurrences are not the 13 builtin-type expressions above. An object result does not itself establish another consumer edge, and full candidate closure remains 0/65. The missing same-type assertion was corrected in new artifacts while retaining the original records. {{projection-count}} {{projection}}

**Correction to the rationale for not testing:** the remaining 572 expressions break down, by their existing x86_64 results, into 283 identical, 114 namespace/tag-only differences, 111 implementation/composite-type differences, 4 enum-related differences and 60 unavailable results. Avoiding repeated work on known naming mechanisms supports the scope decision, but does not justify describing all remaining expressions as known implementation differences. Identical x86_64 results do not establish ARM equality, and unavailable results remain unavailable. The human decision not to test is retained without assigning missing conclusions. {{measurement}} {{mechanisms}}

## Inputs and prerequisites for the next validation stage

The following are **planning estimates, not new measurements or priorities**. Every row needs the named **libc++-built provider package**, matching public headers, dependencies, build options and provenance hashes, plus GNU providers for same-library controls and the opposite direction. Current availability of newly built packages was not re-inventoried; this is a required-input list, not a list of available artifacts.

Use the existing three-type template: pass same-library controls on both sides before mixed directions; use real providers; record the libraries actually loaded; assert concrete values, failure state and applicable destruction responsibilities; repeat every cell five times. Link failure is a result, not a reason to manufacture natural linkability with forced renaming or function-pointer casts. Ownership checks must not collapse into exit-code checks. {{template}}

**Architecture boundary:** every listed edge can start on x86_64 once its provider and initialization fixture are ready; this does not promise that every current service runs headlessly. Each edge requires its own ARM-version validation for an armv7l product claim, planned on a physical board following the template. For edges not yet checked there, first establish the actual ARM references. Existing evidence cannot identify a definite subset that is technically impossible to test without physical hardware; that requirement is NOT_OBSERVED. Rendering, WebView, inference and service initialization are recorded as prerequisites. A board is needed if those requirements can only be met there, rather than assumed now. The two confirmed ARM references are not exempt from execution tests.

**Estimate boundaries:** h / working days below refer to one edge on one architecture after providers and the environment are ready, covering fixture work, validation and records. Provider rebuilding, dependency repair, environment setup waits and board scheduling are excluded. Simple data calls, state/resource calls and heavier service/callback initialization receive different planning bands. These are editorial estimates, not historical per-edge measurements. All historical three-type timing records have `complete_scope=false`, so they cannot calibrate complete per-edge costs. No total effort is given. {{timings}} {{template}}

{{next_table}}

Detailed inputs are in [NEXT_STAGE.tsv](NEXT_STAGE.tsv), including provider ELF targets and evidence hashes. This plan does not choose a compatibility strategy or imply that one symbol can stand in for every other symbol in its provider package.

## Uncovered scope and editorial judgments

Uncovered areas include artifacts outside the frozen ELF set, static linking, macro/conditional generation, generated headers, template instantiation, unqualified names after aliases, dlopen/indirect calls, unscanned architectures, and per-edge mixed-library content/destruction/exception/concurrency validation. Public headers and test ELF files do not by themselves establish third-party SDK availability. The original named frozen header/ELF scope and x86_64 structural-measurement scope remain in force; no technical measurement scope was expanded here. Existing targeted ARM evidence was brought into the same register. {{scope}} {{arm-final}}

Editorial judgments are limited to separating counting units and architectures, restating Reader's reviewed category, presenting the wrong-time scenario as conditional inference, estimating per-edge work bands, and correcting the composition behind the no-test decision. The 62 gaps were not reclassified, closure of the 65 facilities was not advanced, and builtin-alias/empty-type tests were not rerun. Human review is requested for these formulations and the next-stage entry conditions, not replaced by a compatibility-policy decision.

See [SELF_CHECK.md](SELF_CHECK.md), the repository paths and full SHA256 values in [EVIDENCE.md](EVIDENCE.md), and all input hashes in [INPUT_SHA256.json](INPUT_SHA256.json). Fact chains, excluded explanations and falsification conditions are separated. Absence of a crash is not treated as absence of a problem.
