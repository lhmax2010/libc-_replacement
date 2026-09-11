# Cross-standard-library adaptors: demonstrated scope and deployment conditions

For human review. This document neither selects nor ranks solutions. It does not promise compatibility for untested interfaces.

The new result is specific: **transferring the output object's initial state fixes the observed EWK cookie adaptor field loss when that output is reused**. Evidence covers normally returned fields for a libc++ caller and a real GNU provider, on native x86_64 and a physical armv7l board. **The migration direction, an existing GNU caller using a libc++ EWK provider, remains untested.** This is not approval for fully transparent deployment. [Run matrix](../W2/MATRIX.md), [prototype and limitations](../W2/REPORT.md)

## Mechanism: each side understands and destroys its own objects

Two C++ standard libraries may represent a string differently. Successful linking establishes that an entry point was found, not that the callee interprets the object's memory correctly. `extern "C"` can give the entry point a common name; it does not turn C++ string arguments into C byte arrays.

An adaptor reads a caller-side string and constructs a provider-side string using the provider's library. Two translation units are compiled against their respective libraries. Between them, only simple data such as pointers, byte lengths, integers and status codes crosses the boundary (POD). Results are copied back, and each side destroys only the objects it constructed. Pointers here are borrowed within **one process for an agreed lifetime**; this is not an inter-process serialization protocol.

The mechanism already has a real Boost.Program_options fixed-sample demonstration in both directions: 4 cells, 5 runs per cell, 20 runs. It is not a substitute for the EWK, browser callback or Dali experiments. [Mechanism experiment](../../API_0911/W2/REPORT.md)

Two translation units are only a starting point. Correct routing, initial output state, ownership and prevention of conflicting same-name helper definitions are also required. This experiment found and corrected an instance of that last problem.

## EWK cookies: field protocol demonstrated, full deployment still conditional

**Fact chain.** [Static inspection] The real `ewk_parse_cookie` overwrites name/value after accepting input. Domain and Path are updated only when the attributes are present; otherwise their existing output values survive. Rejection of empty, HttpOnly or Secure input occurs before field writes. [Measurement] A protocol that omitted initial state lost fields on reuse. The revised protocol transfers all four existing fields and expiry before invoking the real GNU function. On both architectures, same-library controls and the revised adaptor produced the expected fields and completed destruction in all 5 repetitions of every scenario. [Numbered implementation](../W1/raw/008_targeted.stdout), [scenario details](../W2/REPORT.md)

The normative experiment has 12 scenarios: complete input, missing attributes, a single attribute, an explicitly empty attribute, rejected input, long strings, fixed Expires, Max-Age checked against the current call's clock interval, and sequential reuse of the same output. Each process records maps to establish both runtimes, entry point and real provider. String lengths and hexadecimal bytes are recorded. Max-Age is not expected to produce identical absolute timestamps in different processes.

| Environment | GNU same-library control | Revised adaptor: libc++ to GNU | Negative mode deliberately dropping initial state |
| --- | --- | --- | --- |
| Native x86_64 | 5 runs, 60/60 scenarios correct | 5 runs, 60/60 correct | 5 runs, 20/60 correct, 40/60 wrong values |
| Physical armv7l board | 5 runs, 60/60 scenarios correct | 5 runs, 60/60 correct | 5 runs, 20/60 correct, 40/60 wrong values |

Each environment/path combination has 5 processes with 12 scenarios each: 6 cells, 30 runs and 360 scenario observations in total, not 360 independent runs. All three paths completed consumer-side scope destruction for 60/60 scenarios on each architecture, **including the negative mode that still returned wrong values**. Completed destruction is not proof of correct state. [Native raw index](../W2/v2native_matrix.json), [physical-board raw index](../W2/board_matrix.json)

**Excluded approaches.** The negative control rules out the tested input/final-output-only protocol that omits initial state. Another initial design instantiated a temporary `EWKCookieContents` in both translation units. Their implicit destructors had the same mangled name; WEAK HIDDEN definitions were coalesced, and destruction of a GNU object reached the wrong implementation and SIGSEGV. The revised front uses four local strings, avoiding generation of that external class's destructor on both sides. External wrapper special members need checking in addition to standard-library namespace separation. [Object symbols](../W2/raw/007_object_symbols.stdout), [failure backtrace](../W2/raw/006_debug_state.stdout)

**What would refute the conclusion?** A normally returned field mismatch against the same-library control, with matching provider/runtime identity, input and initial state, would refute the limited state-fix claim. Full deployment additionally requires the real libc++ EWK reverse matrix, existing-caller routing, product build/packaging, exceptions and cancellation, and internal-reference/address-lifetime evidence. The experimental catch-all/local-runtime_error policy does not preserve the original exception types, partial writes or cancellation semantics. **It is not a production error contract.**

## Browser binary replies: an expressible data protocol, no real callback validation yet

**Fact chain.** [Static inspection] The plugin invokes `SetSyncBinaryReply(instance, const std::vector<uint8_t>&)` through an interface table. Direction is plugin callback into the engine. The real engine looks up a valid instance, assigns the vector to its stored reply, and the synchronous return path later retrieves that reply. [Earlier measurement] Selected x86_64 representation samples from both libraries had a 24-byte, three-pointer vector. This establishes those representation observations, not the real callback, allocator, exception or destruction contract. [Registration, instance and receiver evidence](../W1/REPORT.md), [state-function inventory](../W2/STATE_SCOPE.tsv)

**Excluded approaches.** Equal sizeof is not a callback test. The unsupported no-op in the Node/service implementation is not a positive control. Copying a vector before a call does not establish real interface-table or instance registration. [Inference] Byte elements can be reconstructed from pointer and length on the receiving side, so the data conversion is expressible; deployment through a real host still needs validation. Compilation of the real component lacks base headers and generated dependencies. No working receiver was started. Neither "the entire browser is mandatory" nor "a browser-free host already works" is established.

**What would change the assessment?** Real components on both sides, valid registration and instances, and repeated two-direction assertions of length, every byte, reply order and destruction, with loaded identities checked, would support measured feasibility. If copying cannot preserve an agreed reference, exception or ownership contract, the simple byte adaptor is unsuitable for that contract.

## Dali: property values are not complete object behavior

**Fact chain.** [Static inspection] C# applications use C-named binding exports, but the binding implementation calls Dali C++ APIs. `Control::New` returns an object; the binder constructs a Control handle and a paired entry deletes it. Objects, virtual dispatch and lifetimes are not merely byte sequences. [Earlier measurement] The real property-wrapper subset passed exact-value/destruction tests in both directions. Replacing only core with libc++ in the complete GNU binder chain produced a missing GNU-vector-mangled symbol on both architectures. [New measurement] The physical board has an offscreen API. Initial startup lacked the GLES search path. After locating the installed `/hal/lib/driver` libraries, New still did not finish within the 30-second probe deadline, identically in 5 revised-path repetitions. Control/virtual-dispatch behavior therefore remains unobserved. [Dependencies and offscreen results](../W1/REPORT.md), [actual Control construction/destruction](../W3/raw/002_workload.stdout)

**Excluded approaches.** Replacing only core does not make the existing complete GNU dependency chain load. Passing the property subset does not validate all of Dali. Copying raw memory of an object with a vtable or internal references is not a POD conversion protocol. That is a structural assessment of the **raw-copy approach**, not a rejection of keeping the object on its original side and using handle/operation proxies. Such a proxy remains untested. The offscreen timeout does not prove that the board lacks graphics capability.

**What would change the assessment?** Correctly initialized complete components, with explicit object identity, property values, virtual calls, reference counting/destruction, threads and callbacks validated, could extend the supported scope. A real edge shown to carry only stable scalars or borrowed data should not be classified as a whole-object problem. Absence of direct C++ ABI exposure to C# does not remove the internal binder-to-Dali boundary.

## Native wrappers and additional conditional outputs

**Fact chain.** [Static inspection and compile/link checks] Selected operations in app_control.hpp, GetId in app_common.hpp and selected Run paths in service_app.hpp/app.hpp produced 8 ELF files across the two libraries with C platform imports. String conversion stays on the application side. Those edges need no duplicate standard-library adaptor; this is not a runtime-safety finding for all four families. [Static inspection] Six additional Dali Get output overloads were checked: bool, float, int32, Dali::String, Array and Map. A type mismatch returns false without writing the output. [Native scope](../../API_0911/W1/REPORT.md), [numbered Get implementations](../W2/raw/005_get_state.stdout)

**Excluded approaches.** Local C++ symbols do not establish that the platform edge passes a C++ object; C linkage names alone do not establish safety either. Unconditionally copying back a zero-initialized temporary after a failed Get would violate the unchanged-output rule. This follows from source, not from new runtime tests of those six Get overloads. Nested Array/Map values and ownership cannot be inferred completely from the outer signature.

**What would refute or extend this classification?** A selected Native instantiation found to pass C++ objects or exceptions through the platform entry must be reclassified. Get needs matching/mismatching types and nonempty initial outputs tested for state, type encoding and lifetime. In the bounded inventory of 11 definitions, 7 conditionally preserve output, while 2 others are callback/cache-sequence candidates. **This is not a platform-wide count**; the total outside that inventory is unavailable.

## How to assess another interface

Do not decide from a string in the signature alone. Check complete data encoding; initial state/failure/partial writes; creation and destruction ownership; exception and cancellation contracts; internal references/iterator lifetimes; virtual dispatch/object identity; callback registration/thread order; same-name helpers; real components in both directions and existing-caller deployment. Missing evidence for one condition is not supplied by success on another. [Criteria and per-interface table](../W3/REPORT.md)

## Size and alternatives

The implemented EWK conversion experiment is **56 lines**: protocol header 8, backend 26, front 22, including comments and blank lines. Its scenario probe is another 49 lines. Build/evidence orchestration and production integration are excluded. This observed source size is not a production estimate. The opposite direction needs its corresponding builds and independent validation. With error policy unresolved, total production effort is unavailable. [Raw counts](../W3/raw/002_workload.stdout)

| Scope | Work and responsible parties |
| --- | --- |
| EWK | Platform adaptor maintainers own both conversions, initial-state protocol, routing and symbol isolation; API owners define the error contract; test owners maintain reverse-direction and reuse coverage |
| Binary replies | Besides a pair of byte-conversion operations, engine/plugin maintainers need interface-table adaptation, instance mapping/unregistration and synchronous reply-sequence validation |
| Dali scalar Get | At least three success-tagged operations, potentially sharing object-identity handling; strings additionally need encoding/length, Array/Map need recursive type protocols |
| Complete Dali objects | API/binding maintainers need proxies, operation sets and ownership/callback/thread contracts; total size is unavailable. The two inspected wrapper files have 61 and 38 export-start lines, not that many conversion edges |
| Selected Native C edges | No additional standard-library conversion entry is needed; application/platform owners still validate existing handle, callback and exception constraints |

EWK copies input and initial fields to the provider side and returned fields to the caller side; final commit uses swap. A vector data protocol copies input bytes at least once. Production latency, peak memory and complete allocation counts were not measured. Upstream changes to fields, types, default rules, interface tables, library versions or compiler/LTO settings require renewed checks. Headers can help generate signature/copy skeletons, not infer state preservation, lifetime or failure behavior automatically.

| Solution | What it addresses / remaining cost | Responsible parties |
| --- | --- | --- |
| Adaptor | May preserve the caller-facing interface; adds conversion, state/lifetime, symbol and version maintenance | Platform API, build and test owners |
| C data/handle API | Avoids direct standard-library objects across the boundary; still needs fields, errors, create/release and coexistence with old entries specified | Platform adds interfaces; application developers adopt the protocol |
| Migrate application and dependencies to libc++ | Removes this mixed-standard-library premise when both sides and dependencies are coordinated; requires source and migration conditions | Application/component owners; external readiness was not inferred |
| Declare and accept restrictions | Does not promise unvalidated mixing; restricts library/version/interface and deployment combinations | Platform maintains support scope; business and application owners assess acceptability |

This document does not select an optimal solution for the business.

## Decision summary

| Interface | Direct application exposure | Assessment and evidence strength | Options | Business contract to clarify |
| --- | --- | --- | --- | --- |
| EWK parser | Callable from native code; formal SDK membership not assessed here | One-direction field protocol measured; full deployment unverified | Adaptor / C data API / migration / restrictions | Existing GNU direction, transparent exceptions and old-binary support |
| Binary reply | Internal engine/plugin edge, not a JS application passing C++ vector directly | Data protocol statically expressible; real callback untested | Adaptor / C byte API / component migration / restrictions | Plugin coverage, lifetime and reply contract |
| Binder to Dali | C# uses outer C entries; internal C++ edge remains | Property subset measured; complete objects unverified | Coordinated component migration / proxies or C handles / restrictions | Object operations and old bindings to retain |
| Selected Native wrappers | Application-side inline wrappers | Selected C edges supported by static/link evidence; no duplicate conversion needed | Keep the bounded C edges and their existing constraints | Whether to extend to additional members/callbacks |
| Six conditional Dali Get overloads | Depends on the native/binding path | State rules inspected statically; no new runtime tests | Scalar/byte/type protocols / migration / restrictions | State, type and ownership contracts |

## Uncovered scope and review boundary

Uncovered: real libc++ EWK in the reverse direction; the real browser receiver; complete mixed-library Dali graphics/virtual calls; production integration and old-application loading; exceptions/cancellation; concurrency/reentrancy; internal-reference lifetimes; LTO/other toolchains; all external interfaces; and a formal SDK inventory. An SDK inventory was not a prerequisite for this mechanism study, nor does this study establish SDK-wide coverage. Full Chromium build duration and whole-object proxy cost are unavailable; no invented estimate fills those gaps. Failed prototype forms do not prove an interface can never be adapted.

Board uploads were removed by manifest, the private directory was verified absent, and no new diagnostic files appeared. The board is released. [Cleanup evidence](../W2/board_cleanup.json) Sources, commands, exit codes, script snapshots and SHA256 hashes accompany the material. Original failures are retained. Work stops for human review.
