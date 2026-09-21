# Cross-package boundaries by architecture and new standard-library findings

Version 2 (2026-09-21): adds only the four stream-state enum retests and related tracking; other conclusions are unchanged. See [version notes](VERSION_20260921_2.md) for the previous version, diff and current audit.

For human review. This document consolidates existing records only. It adds no source investigation, probes, board runs or compatibility commitments. Read every confirmation together with its evidence class and architecture.

## Scope totals and counting units

The original **44 candidate entries** map to **16 directed source-package pairs and 21 symbol edges**. Adding WaitUntil from a different consumer and the Reader constructor brings the x86_64 register to **18 pairs and 23 symbol edges**. Candidates, pairs and symbols are different units: several candidates can share one edge, and several edges can share one pair. The complete original mapping is available as [Markdown](CANDIDATE_MAP.md) and [TSV](CANDIDATE_MAP.tsv). [Evidence: base-map](EVIDENCE.md#base-map) [Evidence: edges](EVIDENCE.md#edges)

The additional records use expression labels such as `steady_clock::rep` and `streamoff`; they do not constitute another completed candidate-classification batch. “44 → 18/23” describes the progression of the register, not 44 disjoint types producing 23 independent defects. Different WaitUntil consumers are separate edges. Reader remains an object boundary, not a direct builtin-type symbol collision. [Evidence: extra-proofs](EVIDENCE.md#extra-proofs) [Evidence: arm-final](EVIDENCE.md#arm-final)

| Architecture | Confirmed package references | Evidence coverage | What is not established |
| --- | --- | --- | --- |
| x86_64 | 18 pairs / 23 symbol edges | Frozen selection of 3,085 ELF files out of 10,946 runtime ELF files on this architecture, approximately 28% | Not the whole platform; not 23 measured mixed-library failures |
| armv7l | 2 targeted pairs / 2 symbol edges: ui-foundation → dali2 and zypper → libzypp | Actual ARM consumer UND entries, provider definitions and NEEDED/SONAME correspondence | Remaining edges not rechecked; the complete ARM denominator is NOT_OBSERVED, not 16/21 or 18/23 |
| aarch64 | Package-reference total and each listed edge: NOT_OBSERVED | Only the specified type measurements below | Type measurements do not establish package references or successful execution |

Sources for these denominators and architecture distinctions: [Evidence: scope](EVIDENCE.md#scope) [Evidence: arm-elf](EVIDENCE.md#arm-elf) [Evidence: arm-count](EVIDENCE.md#arm-count) [Evidence: arm-final](EVIDENCE.md#arm-final). The ARM records were obtained from actual ARM ELF files, not inherited from x86_64. Their raw symbols, file paths and full hashes are in the [per-edge appendix](EDGES_en.md).

## Complete boundary register

This register records **static findings** from public declarations, object-storage chains and actual released GNU ELF references. It is not a mixed-standard-library execution matrix. Calls directly into the standard-library runtime itself are excluded. Edges found in test artifacts remain registered, but do not establish direct third-party application usage. Transfer forms and ownership descriptions retain the original evidence and its limits. [Evidence: base-map](EVIDENCE.md#base-map) [Evidence: edges](EVIDENCE.md#edges)

| Row | Consumer → provider | Interface | Form; ownership | ELF architecture | Raw evidence |
| --- | --- | --- | --- | --- | --- |
| 1 | appcore-agent → app-core | AppCoreBase::AddEvent | shared_ptr&lt;EventBase&gt; by value; Shared ownership; retention unverified | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-1) |
| 2 | amd → bundle | Bundle::Add | const references: string and vector&lt;string&gt;; Borrowed; cross-library deallocation not assumed | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-2) |
| 3 | amd → bundle | Bundle::Bundle(initializer_list) | View by value; pair&lt;string,string&gt; elements; Passing a view does not transfer its elements | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-3) |
| 4 | ise-engine-anthy → isf | scim::utf8_wcstombs | wstring const reference; Borrowed input; handle result according to actual declaration | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-4) |
| 5 | xwalk-extensions-common → jsoncpp | Json::Reader::parse | Implicit this pointer; Reader deque/stack members; output reference; Consumer constructs Reader; borrowed method call | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-5) |
| 6 | grpc → abseil-cpp | SpinLockWait | atomic&lt;unsigned int&gt;* and transition-table pointer; Borrowed control word; no transfer declared | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-6) |
| 7 | com.samsung.dali-demo → dali2-adaptor | ActorAccessible::GetMatches | tuple by value; array/unordered_map/hash/equal_to subobjects; By value; internal resource contract unverified | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-7) |
| 8 | dali2-adaptor → dali2 | Scene::AddFrameRenderedCallback | unique_ptr&lt;CallbackBase&gt; by value; Explicit transfer of unique ownership and deleter | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-8) |
| 9 | enlightenment → gtest | StringStreamToString | stringstream pointer; streambuf/openmode members; Borrowed; original consumer is a test artifact | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-9) |
| 10 | app-installers → manifest-parser | DeltaParser::ParseManifest | filesystem::path const reference; Borrowed path | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-10) |
| 11 | app-installers → cert-svc | SignatureValidator::checkList | list const reference; SignatureData output reference; Borrowed input; output-release contract to establish | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-11) |
| 12 | com.samsung.dali-demo → dali2-toolkit | WebView::RegisterPageLoadStartedCallback | function by value; callback string const reference; Captured ownership depends on callable | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-12) |
| 13 | dali2-toolkit → dali2 | CalculateHash | string_view const reference; Borrowed view and backing storage | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-13) |
| 14 | dali2-toolkit → dali2 | ConditionalWait::WaitUntil | time_point by value; duration/rep members; ScopedLock reference; Time value transfers no heap resource | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-14) |
| 15 | dali2-toolkit → dali2 | SingletonService::GetSingleton | type_info const reference; Borrowed compiler-generated type information | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-15) |
| 16 | capi-media-vision → inference-engine-interface | GetInputTensorBuffers | Non-const map output reference; less member; Backend fills output; final tensor-buffer release responsibility unresolved | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-16) |
| 17 | app-core → gtest | PrintStringTo | string const reference; ostream pointer and state members; Borrowed; original consumer is a unit-test artifact | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-17) |
| 18 | data-provider-master → notification | SharedFile::SetPrivateSharing | list&lt;shared_ptr&lt;...&gt;&gt; and multimap by value; Value parameters; copy/move and retention unverified | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-18) |
| 19 | libzypp-bindings → libzypp | ResPool::setRequestedLocales | unordered_set&lt;Locale&gt; const reference; Borrowed container | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-19) |
| 20 | libzypp-bindings → libzypp | ZConfig::multiversionSpec | set&lt;string&gt; by value; Declaration alone does not determine copy versus move | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-20) |
| 21 | libzypp-bindings → libzypp | CheckSum::CheckSum | string const reference and istream reference; Borrowed input and stream | x86_64; armv7l NOT_OBSERVED | [ELF / SHA / symbol](EDGES_en.md#edge-21) |
| 22 | dali2-ui-foundation → dali2 | ConditionalWait::WaitUntil | time_point by value; duration/rep members; ScopedLock reference; Time value transfers no heap resource | x86_64; armv7l | [ELF / SHA / symbol](EDGES_en.md#edge-22) |
| 23 | zypper → libzypp | xml::Reader::Reader | InputStream const reference; internal string/shared_ptr/streamoff state; Borrowed parameters; post-construction retention not validated here | x86_64; armv7l | [ELF / SHA / symbol](EDGES_en.md#edge-23) |


The complete machine-readable table is [EDGES.tsv](EDGES.tsv). Each row retains x86_64 consumer/provider paths, SHA256 values and the original symbol. Rows with ARM evidence additionally preserve ARM paths, hashes and raw UND/DEF lines. `NOT_OBSERVED` does not mean absent.

### Correct classification of Reader

**Fact chain — static evidence:** the actual call is `Reader(InputStream const&, Validate const&)`. `InputStream::_size` is `DefaultIntegral<std::streamoff,-1>`, whose implementation stores a value member. The containing class also has a `_name` string, `_stream` and other members. The constructor symbol encodes the names `InputStream` and `Validate`, not the internal type of `_size`. The saved InputStream/DefaultIntegral header excerpts and ELF records support this chain. [Evidence: structure](EVIDENCE.md#structure) [Evidence: extra-proofs](EVIDENCE.md#extra-proofs)

**Excluded explanation:** this Reader symbol is not evidence of a direct `streamoff` parameter causing an `l/x` name collision. Cross-compiling the real ARM headers with each library produced the same Reader reference name; the actual GNU package reference also exists. Neither fact removes the object boundary or proves that its internals can be interpreted or destroyed across libraries. [Evidence: arm-signatures](EVIDENCE.md#arm-signatures) [Evidence: arm-elf](EVIDENCE.md#arm-elf)

**What would overturn this finding:** disproving the association between the frozen headers and ELF artifacts, or showing that the transferred object is not the recorded InputStream, would require revising the edge. Establishing mixed-library compatibility still requires content, state and destruction tests against the matching libc++ provider. Equal function names are insufficient. The retained classification is “cross-package C++ object, by reference and through member state”; the edge count is unchanged.

## New findings suitable for an external report

### The same high-resolution clock name can mean different time

**Fact chain:** in the measured x86_64 configuration, libstdc++ expands `high_resolution_clock` to `system_clock`, whereas libc++ expands it to `steady_clock`. Both clock types were measured as empty and 1 byte in size. Saved definitions locate the GNU alias in `bits/chrono.h` and the conditional libc++ alias in `__chrono/high_resolution_clock.h`. libc++ selects steady_clock when monotonic-clock support is enabled and can select system_clock otherwise; this is not an unconditional statement about every libc++ build. [Evidence: clock](EVIDENCE.md#clock) [Evidence: clock-gnu](EVIDENCE.md#clock-gnu) [Evidence: clock-cxx](EVIDENCE.md#clock-cxx)

**Static semantic evidence:** the saved GNU definition describes system_clock as system-wide wall time with `is_steady=false`, and steady_clock as increasing uniformly with `is_steady=true`. Empty clock tags have no non-static data members. That excludes different stored clock-tag layouts as the explanation for this finding; **it does not establish equal time semantics or cover the state carried by a time_point**. Installed-header samples include the Boost alias import, LLDB StatsClock/StatsTimepoint and LLD startTime member. These samples alone do not add confirmed consumer edges. [Evidence: clock-gnu](EVIDENCE.md#clock-gnu) [Evidence: clock-cxx](EVIDENCE.md#clock-cxx) [Evidence: clock](EVIDENCE.md#clock)

**Connection to confirmed boundaries:** WaitUntil's transfer of a time_point is supported by its public signature, duration/rep storage chain and ELF references. However, its header explicitly uses **steady_clock, not high_resolution_clock**. The established time_point boundary and the separate high-resolution alias difference cannot be combined into a claim that WaitUntil has already produced a measured silent wrong value. [Evidence: structure](EVIDENCE.md#structure) [Evidence: edges](EVIDENCE.md#edges)

**Concrete wrong-value scenario — inference, not measured:** suppose a GNU component exports a high-resolution timestamp as an integer number of nanoseconds since its clock's epoch. A libc++ component then reconstructs its own high_resolution_clock deadline without converting the clock domain and epoch. The receiver would interpret a wall-time number as a monotonic-clock timestamp. Integer transfer can succeed while representing the wrong deadline, potentially causing an early timeout or an incorrectly long wait. This is a conditional scenario with explicit assumptions: an integer boundary, normalized units, and no epoch conversion. It is not an observed failure on any listed released call path; no wrong value or time offset for this scenario was measured here.

**Excluded explanations:** this semantic risk is not attributed to a size difference, does not treat all chrono types as one clock, and does not assume all mixed C++ symbols will link. Direct transfer of differently named types may fail at link time first.

**What would overturn the scenario:** identical clock selection in the actual builds, explicit validation/conversion of clock domain and epoch, or absence of this reinterpretation would invalidate it. A product-defect claim requires an actual consumer path and a testable deadline result. The current distinction is “aliases and emptiness measured; wrong-time scenario inferred.”

### Equal-width integers can still produce different link names

**Fact chain — measured:** the specified **13 type expressions** resolve to GNU `long` versus libc++ `long long` on x86_64 and aarch64, with actual encodings `l` versus `x`; both occupy 8 bytes. On armv7l they all resolve to `long long`, encoding `x`, occupying 8 bytes on both sides. These are counts of expressions, not separate package edges or failures. Both libraries were measured, with five consistent repetitions per recorded cell and positive controls for type encodings. [Evidence: types-x86](EVIDENCE.md#types-x86) [Evidence: types-arm](EVIDENCE.md#types-arm) [Evidence: types-aarch64](EVIDENCE.md#types-aarch64) [Evidence: arm-final](EVIDENCE.md#arm-final)

| Type expression | x86_64 GNU / libc++ | armv7l GNU / libc++ | aarch64 GNU / libc++ |
| --- | --- | --- | --- |
| std::streamoff | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::char_traits&lt;char&gt;::off_type | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::char_traits&lt;wchar_t&gt;::off_type | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::char_traits&lt;char16_t&gt;::off_type | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::char_traits&lt;char32_t&gt;::off_type | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::istreambuf_iterator&lt;char&gt;::difference_type | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::chrono::system_clock::rep | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::chrono::steady_clock::rep | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::chrono::high_resolution_clock::rep | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::chrono::nanoseconds::rep | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::chrono::microseconds::rep | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::chrono::milliseconds::rep | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |
| std::chrono::seconds::rep | long (`l`, 8 B) / long long (`x`, 8 B) | long long (`x`, 8 B) / long long (`x`, 8 B) | long (`l`, 8 B) / long long (`x`, 8 B) |


The complete source/hash columns are in [BUILTIN_TYPES.tsv](BUILTIN_TYPES.tsv). x86_64 used native execution; armv7l and aarch64 used cross-compilation followed by QEMU user-mode printing. Target headers, macros and compiler configuration determine the types; QEMU does not choose them again. This does not validate product calls on a physical board. The recorded configurations use GNU 14 and libc++ 22.1.8. [Evidence: arm-config](EVIDENCE.md#arm-config) [Evidence: aarch64-config](EVIDENCE.md#aarch64-config) [Evidence: arm-final](EVIDENCE.md#arm-final)

**Known public exposure — static, not automatically a consumer edge:**

| Provider package and header | Public declaration / structure | Relation to the finding and consumer evidence |
| --- | --- | --- |
| dali2, conditional-wait.h | `using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;`; `void WaitUntil(const ScopedLock& scope, TimePoint timePoint);` | time_point → duration → rep; toolkit and ui-foundation each have an x86_64 edge; only ui-foundation was targeted on ARM |
| podofo, PdfInputDevice.h | `virtual std::streamoff Tell() const;`; `virtual void Seek(std::streamoff off, std::ios_base::seekdir dir = std::ios_base::beg);`; `virtual std::streamoff Read(char* pBuffer, std::streamsize lLen);` | Seek directly takes the integer type. Tell/Read return types are generally absent from ordinary function mangling, so universal link failure cannot be claimed. Existing Tell/Seek identity queries found definitions but no external UND; Read's query was not completed; virtual dispatch is not excluded |
| abseil-cpp, absl/time/time.h | `FromChrono(const std::chrono::nanoseconds& d)` and microseconds / milliseconds / seconds overloads, plus a system_clock::time_point overload | Corresponding duration/clock reps are involved. Existing identity queries did not establish external UND references; inline/template routes remain open. No new edge is added |
| libzypp, InputStream.h / Reader.h | `std::streamoff size() const`; `void setSize(std::streamoff val_r)`; Reader borrows InputStream | size/setSize are saved inline members. Reader's symbol encodes the containing class name, not its internals; the member-type difference is not a direct Reader symbol collision. Its object boundary remains |

Exact declaration locations and source SHA256 values: [Evidence: declarations](EVIDENCE.md#declarations) [Evidence: structure](EVIDENCE.md#structure). Actual references: [Evidence: extra-proofs](EVIDENCE.md#extra-proofs) [Evidence: arm-elf](EVIDENCE.md#arm-elf). Scoped negative findings retain the existing function-identity method with a `bundle::Add` positive control; they do not assert platform-wide absence of consumers. [Evidence: boundary-control](EVIDENCE.md#boundary-control) No further closed, independent consumer edge is added here for char_traits/off_type, istreambuf_iterator::difference_type or the other listed expressions.

**The two mechanisms affecting WaitUntil:** on x86_64, different duration representations add the newly identified builtin `l/x` factor. Separately, the libraries have different chrono inline namespaces and clock-type identities, an already known factor. On armv7l the first factor disappears, but actual reference symbols compiled from the real headers still differ: the second factor remains. aarch64 measurements cover the types, not WaitUntil's function symbol or business references; LP64 cannot supply the missing evidence. [Evidence: extra-proofs](EVIDENCE.md#extra-proofs) [Evidence: arm-signatures](EVIDENCE.md#arm-signatures) [Evidence: arm-final](EVIDENCE.md#arm-final)

**Excluded explanations:** occupying 8 bytes does not make long and long long the same C++ type. This finding is not a 4-versus-8-byte width change or an artifact of different pretty-printed spelling. The comparison uses actual TypeToken mangling, with direct type encodings retained. Return types and containing-object members need not appear in a function name, so not every occurrence implies link failure.

**What would overturn the finding:** equal actual type encodings in a specified target configuration remove the builtin divergence for that configuration; the armv7l measurement is precisely such a counterexample. A particular interface's link consequence also requires the type to enter its symbol and an actual consumer. No relevant parameter/template argument, an explicit compatible entry point, or no mixed-library boundary would defeat that failure inference. Real libc++-provider execution for each edge is not yet established by this work.

**A paragraph ready for external use:** Some standard-library names resolve to different integer types in the two implementations. In the measured x86_64 and aarch64 configurations, those integers occupy the same number of bytes, but the compiler distinguishes long from long long in function names. Parameters or template types involving them may therefore prevent a caller from finding the provider's function. This differs from an object being misread at runtime because its memory layout changed. Return types and internal members do not necessarily change the function name. This integer-selection difference is absent in the measured armv7l configuration; another architecture's finding must not be transferred to it unchanged.

### Stream-state enums: type identities still diverge on armv7l

**Fact chain — new measurements added in version 2:** GNU fmtflags, iostate and openmode are the named enums `_Ios_Fmtflags`, `_Ios_Iostate` and `_Ios_Openmode`; libc++ uses `unsigned int` for all three. GNU seekdir is `_Ios_Seekdir`, whereas libc++ uses `ios_base::seekdir`. Both armv7l and aarch64 show 4/4 divergences, with five consistent repetitions per library. All four have size/alignment 4/4 on both sides. The earlier target-type probe/configurations and QEMU user mode were reused; no board was used. [Evidence: TYPES.tsv](../R119_ENUM_RETEST/TYPES.tsv) — SHA256 `f1c2e9551a6a379141ab91b462612ae5799dee1dc077e2044e5fd1816705a377`

| Type | GNU encoding | libc++ encoding (same on both architectures) |
| --- | --- | --- |
| fmtflags | `St13_Ios_Fmtflags` | `j` |
| iostate | `St12_Ios_Iostate` | `j` |
| openmode | `St13_Ios_Openmode` | `j` |
| seekdir | `St12_Ios_Seekdir` | `NSt3__18ios_base7seekdirE` |

**Unlike the integer aliases:** the earlier 13 expressions selected the same integer type on the measured armv7l configuration. These four enum/builtin or enum-identity differences remain despite equal widths. They are separately recorded type findings, not four demonstrated product failures.

Compiling the real ARM podofo header produced GNU `_ZN6PoDoFo14PdfInputDevice4SeekExSt12_Ios_Seekdir` and libc++ `_ZN6PoDoFo14PdfInputDevice4SeekExNSt3__18ios_base7seekdirE`. The streamoff encoding is `x` on both sides; seekdir accounts for the remaining difference. A qualified call exposes the UND name. Both ordinary virtual-call IR bodies instead select zero-based index 6 relative to the vtable address point and call indirectly. This observes caller compilation, not provider-vtable or mixed-execution compatibility. [Evidence: SEEK_SYMBOLS.json](../R119_ENUM_RETEST/SEEK_SYMBOLS.json) — SHA256 `32ccb97b8ce8012af090b855466c334cc2eae73aae8c0cacf289cc9e1ef7f0d5` [Evidence: VIRTUAL_CALL.json](../R119_ENUM_RETEST/VIRTUAL_CALL.json) — SHA256 `f928bf2ead58d70a78ad55060a48aed7a3d374172e6c4414b740901001d97e41`

**Excluded explanations:** this is not a 4/8-byte width difference. Different function names do not directly establish different virtual-table slots or inevitable virtual-call failure. Function-identity queries covered the two ChecksumStream::flags overloads, open, PdfInputDevice::Seek and Clear: 5 confirmed declarations under 4 identities. No corresponding external UND was found in the scoped x86_64 set; the bundle::Add positive control passed. The old index's 61/85/87/12 positions are not 245 confirmed public declarations. Virtual calls, inline code and out-of-scope artifacts remain open. Existing fmtflags/openmode stream-member edges are retained. New edges: 0; the total remains 18 pairs / 23 edges. [Evidence: BOUNDARY_QUERIES.json](../R119_ENUM_RETEST/BOUNDARY_QUERIES.json) — SHA256 `ffc287e4232035231ee82c88a28f0aaeb57621d5871b5b9a4021f4096d9fcf06` [Evidence: BOUNDARY_CONTROL.json](../R119_ENUM_RETEST/BOUNDARY_CONTROL.json) — SHA256 `0a8d08bb6b8dddb528f55d176d7126f5c0af127a937ff4efc92c2deb67355092`

**What would overturn the finding:** equal complete type encodings under the actual target configuration would require revising the divergence claim. Product consequences require real cross-package calls, including virtual calls, actual providers, parameter semantics and object-lifetime evidence. Symbol/slot observations do not substitute for those execution tests.

## Paused work and restart conditions

| Work item | State | Restart condition |
| --- | --- | --- |
| 62 identity/material gaps | Paused; 23 primarily C and 39 primarily D; not platform-wide negatives | See [individual gaps](BACKLOG_en.md) and [full TSV](BACKLOG.tsv), retaining local-resolvability and dependency records |
| Result projections of 65 compile-time facilities | 0/65 fully closed; 15,793 RESULT_UNRESOLVED occurrences; stopped | Renewed human authorization, then actual template arguments, instantiation/name-lookup context or result-structure evidence; hand-selected int examples cannot close candidates |
| Four stream-state enums | Type divergence confirmed on armv7l / aarch64; consumers of the confirmed direct declarations remain unclosed; tracked separately | Actual consumers or virtual-call paths; see the enum addition and separate BACKLOG entries |
| Candidates 201–495 | 295 not yet bounded | Authorization for the next batch and confirmation of scope/method; no continuation here |
| Remaining 568 expressions on armv7l | Do not test; remain unmeasured | Restart only after an explicit scope change and a product-relevant question; equality of the selected 13 does not establish safety of the others |

Count sources: [Evidence: pending](EVIDENCE.md#pending) [Evidence: projection](EVIDENCE.md#projection) [Evidence: projection-count](EVIDENCE.md#projection-count) [Evidence: base-count](EVIDENCE.md#base-count) [Evidence: measurement](EVIDENCE.md#measurement) [Evidence: mechanisms](EVIDENCE.md#mechanisms). Candidate counts, source occurrences and type expressions are different units and must not be summed.

**Latest projection counts:** 15,770 no-object result occurrences, 10 type-divergent occurrences, 13 standard-library-object occurrences and 15,793 unresolved occurrences. The 13 object-result occurrences are not the 13 builtin-type expressions above. An object result does not itself establish another consumer edge, and full candidate closure remains 0/65. The missing same-type assertion was corrected in new artifacts while retaining the original records. [Evidence: projection-count](EVIDENCE.md#projection-count) [Evidence: projection](EVIDENCE.md#projection)

**Updated no-test set:** the 4 enum-related differences have been separated from the former 572 expressions for retesting and tracking. The remaining 568 break down, by their existing x86_64 results, into 283 identical, 114 namespace/tag-only differences, 111 implementation/composite-type differences and 60 unavailable results. Avoiding repeated work on known naming mechanisms supports the scope decision, but does not justify describing all remaining expressions as known implementation differences. Identical x86_64 results do not establish ARM equality, and unavailable results remain unavailable. The human decision not to test is retained without assigning missing conclusions. [Evidence: measurement](EVIDENCE.md#measurement) [Evidence: mechanisms](EVIDENCE.md#mechanisms)

## Inputs and prerequisites for the next validation stage

The following are **planning estimates, not new measurements or priorities**. Every row needs the named **libc++-built provider package**, matching public headers, dependencies, build options and provenance hashes, plus GNU providers for same-library controls and the opposite direction. Current availability of newly built packages was not re-inventoried; this is a required-input list, not a list of available artifacts.

Use the existing three-type template: pass same-library controls on both sides before mixed directions; use real providers; record the libraries actually loaded; assert concrete values, failure state and applicable destruction responsibilities; repeat every cell five times. Link failure is a result, not a reason to manufacture natural linkability with forced renaming or function-pointer casts. Ownership checks must not collapse into exit-code checks. [Evidence: template](EVIDENCE.md#template)

**Architecture boundary:** every listed edge can start on x86_64 once its provider and initialization fixture are ready; this does not promise that every current service runs headlessly. Each edge requires its own ARM-version validation for an armv7l product claim, planned on a physical board following the template. For edges not yet checked there, first establish the actual ARM references. Existing evidence cannot identify a definite subset that is technically impossible to test without physical hardware; that requirement is NOT_OBSERVED. Rendering, WebView, inference and service initialization are recorded as prerequisites. A board is needed if those requirements can only be met there, rather than assumed now. The two confirmed ARM references are not exempt from execution tests.

**Estimate boundaries:** h / working days below refer to one edge on one architecture after providers and the environment are ready, covering fixture work, validation and records. Provider rebuilding, dependency repair, environment setup waits and board scheduling are excluded. Simple data calls, state/resource calls and heavier service/callback initialization receive different planning bands. These are editorial estimates, not historical per-edge measurements. All historical three-type timing records have `complete_scope=false`, so they cannot calibrate complete per-edge costs. No total effort is given. [Evidence: timings](EVIDENCE.md#timings) [Evidence: template](EVIDENCE.md#template)

| Row | Required libc++ provider package | Prerequisites | Value / destruction checks | Per edge, per architecture |
| --- | --- | --- | --- | --- |
| 1 | app-core | Event object and usable AppCore instance | Event fields, reference counts, destruction counts | 4–8 h |
| 2 | bundle | Constructible Bundle and multi-value input | Keys, element counts and bytes; destruction on each side | 2–4 h |
| 3 | bundle | Real constructor and element lifetimes | Fields, empty/duplicate inputs; destruction after construction | 2–4 h |
| 4 | isf | Wide-character inputs, actual return type and environment | Non-ASCII characters and output bytes; result destruction | 2–4 h |
| 5 | jsoncpp | Real Reader/Value construction and malformed input | Parsed values, error queues, reuse and destruction | 4–8 h |
| 6 | abseil-cpp | Controlled transitions, thread-exit/timeout fixture | Transition values and thread completion; not merely absence of a hang | 4–8 h |
| 7 | dali2-adaptor | Real accessibility object, registration and initialization | Exact match-set contents; aggregate destruction | 1–2 working days |
| 8 | dali2 | Scene, frame event and actual CallbackBase lifetime | Callback values/counts; exactly-once destruction by new owner | 1–2 working days |
| 9 | gtest | Original test entry point and real stringstream | Stream contents, mode, result and object destruction | 2–4 h |
| 10 | manifest-parser | Controlled manifest file and parser initialization | Path and parsed fields; failure and destruction | 4–8 h |
| 11 | cert-svc | Valid/invalid URI samples and certificate-service environment | List contents, validation results, output state and destruction | 4–8 h |
| 12 | dali2-toolkit | WebView/page-load event and callback-unregistration path | Callback string, count and captured-object destruction | 1–2 working days |
| 13 | dali2 | Real export and stable input lifetime | Hash versus same-library control; empty/embedded-null input | 2–4 h |
| 14 | dali2 | Wait/wake fixture with explicit clock domain and lock preconditions | Deadline/wake result and lock state; no forced mismatched symbol call | 4–8 h |
| 15 | dali2 | Service registration, type identity and same-library control | Returned object identity, fields and lifetime | 4–8 h |
| 16 | inference-engine-interface | Real backend, model/tensor configuration and release contract | Keys, buffer dimensions/contents, failure state and paired release | 1–2 working days |
| 17 | gtest | Original unit-test entry point and actual output stream | Escaped bytes, stream flags and destruction | 2–4 h |
| 18 | notification | Real SharedFile/AbstractItem, permissions and service initialization | Shared items/fields, failure state and object destruction | 1–2 working days |
| 19 | libzypp | Controlled ResPool state and Locale construction | Locale set, duplicate/empty sets and destruction | 4–8 h |
| 20 | libzypp | Isolated configuration instance/state; no system configuration change | Exact rule set, reuse and destruction | 4–8 h |
| 21 | libzypp | Specified algorithm/input stream and actual result state | Checksum, stream position/error state and destruction | 2–4 h |
| 22 | dali2 | Wait/wake fixture for this consumer; do not inherit another consumer's run | Deadline/wake and lock state; match symbols separately per architecture | 4–8 h |
| 23 | libzypp | Actual InputStream/Validate, XML input and failure paths | XML fields, stream position, construction failure and destruction | 4–8 h |


Detailed inputs are in [NEXT_STAGE.tsv](NEXT_STAGE.tsv), including provider ELF targets and evidence hashes. This plan does not choose a compatibility strategy or imply that one symbol can stand in for every other symbol in its provider package.

## Uncovered scope and editorial judgments

Uncovered areas include artifacts outside the frozen ELF set, static linking, macro/conditional generation, generated headers, template instantiation, unqualified names after aliases, dlopen/indirect calls, unscanned architectures, and per-edge mixed-library content/destruction/exception/concurrency validation. Public headers and test ELF files do not by themselves establish third-party SDK availability. The original named frozen header/ELF scope and x86_64 structural-measurement scope remain in force; no technical measurement scope was expanded here. Existing targeted ARM evidence was brought into the same register. [Evidence: scope](EVIDENCE.md#scope) [Evidence: arm-final](EVIDENCE.md#arm-final)

Editorial judgments are limited to separating counting units and architectures, restating Reader's reviewed category, presenting the wrong-time scenario as conditional inference, estimating per-edge work bands, and correcting the composition behind the no-test decision. The 62 gaps were not reclassified, closure of the 65 facilities was not advanced, and builtin-alias/empty-type tests were not rerun. Human review is requested for these formulations and the next-stage entry conditions, not replaced by a compatibility-policy decision.

See [SELF_CHECK.md](SELF_CHECK.md), the repository paths and full SHA256 values in [EVIDENCE.md](EVIDENCE.md), and all input hashes in [INPUT_SHA256.json](INPUT_SHA256.json). Fact chains, excluded explanations and falsification conditions are separated. Absence of a crash is not treated as absence of a problem.
