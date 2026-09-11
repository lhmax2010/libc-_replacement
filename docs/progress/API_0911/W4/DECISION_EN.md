# Application/platform boundaries across C++ standard libraries

**For human review. Target-interface testing is incomplete; this is not compatibility acceptance.**

The platform is moving to libc++, while some applications or plugins may retain libstdc++. The same C++ source-level type name does not guarantee the same in-memory representation, exception behavior or destruction contract. The application binary interface (ABI) comprises such agreements after compilation. Application-facing and internal component boundaries must be assessed separately; neither all C++ interfaces nor all applications are automatically incompatible.

All four target groups remain **PARTIAL**. Evidence includes eight linked Native-wrapper executables and a separate, measured two-library conversion bridge. EWK, the engine/plugin callback and binder/Dali have no completed bidirectional target-value matrix. The product's `elementary` versus `elementary-tizen` dependency choice and matching runtime environment require confirmation. No product variant was selected merely to finish, and no mock provider was substituted.

## Decision table

“Not guaranteed” means the evidence does not establish cross-library compatibility; it does **not** mean inevitable failure has been measured.

| Interface | Direct application exposure | Finding | Evidence strength | Options | Business decision needed |
| --- | --- | --- | --- | --- | --- |
| ewk_parse_cookie | Candidate application header exists; final product SDK exposure and actual users remain unconfirmed | PARTIAL; no cross-library guarantee for the std-bearing layer | Static declarations/ELF inspection; target behavior unmeasured | C byte interface, dedicated shim, caller migration, declared restrictions | Legacy users/support period, product environment, adaptation and verification costs |
| SetSyncBinaryReply | Native plugin-to-engine boundary; ordinary JavaScript applications do not directly pass vector | PARTIAL; native boundary not guaranteed | Static callback table and real call direction; target behavior unmeasured | Versioned C data table, callback shim, native plugin migration, build restrictions | Native plugin inventory, table-version coexistence and release coordination |
| dali2-csharp-binder → Dali | NUI C# enters through C names/handles; this target is the internal native boundary | PARTIAL; simultaneous migration has not been shown necessary | Static wrappers and real ELF imports; target behavior unmeasured | Internal C handle/operation layer, object adaptation, coordinated native migration, supported-combination declaration | Actual component/Native C++ consumers, transitional combinations and maintenance commitments |
| Selected inline wrappers in four Native families | Application compiles the wrapper; selected platform calls carry C data | Selected edges have C data shape; whole group PARTIAL | Eight actual compile/link results plus header inspection; no valid application-context execution | No demonstrated need to redesign these selected C edges; retain their prerequisites | Final SDK scope and callback/lifetime verification coverage |

## What the conversion-bridge experiment establishes

A shim is intermediate adaptation code. Two translation units—source files compiled separately—use different standard libraries. Their internal bridge carries C-expressible pointers, lengths and status values, not standard-library objects.

The experiment calls the original `to_internal(const std::wstring&) -> std::string` function in **real platform-built Boost.Program_options**, rather than a fabricated provider (the actual called component). Each direction and each same-side positive control ran five times: **four cells, 20 runs**. Five exact-content checks per run give **100 checks**. Empty, ASCII, Chinese/supplementary-character, embedded-NUL and long inputs produced **0, 9, 11, 3 and 257 bytes**, respectively, with byte-for-byte equality. Every run checked both runtimes and the precise provider in process maps; real callee paths and post-scope checkpoints were retained.

The conclusion is only **SHIM_VIABLE for these ordinary inputs on native x86_64**. It does not establish a completed adaptation of the three target interfaces, exception/cancellation behavior, transferred ownership, leak freedom or arbitrary deployment compatibility. Each side destroys its own standard-library objects after copying. This is **not** a successful test of directly deleting another standard library's object in the consumer. [Run matrix](../W2/matrix.json), [code and detailed scope](../W2/REPORT.md), [artifact/runtime identities](../W2/identity.json).

## ewk_parse_cookie

### Evidence chain

1. **Static inspection:** the real candidate header takes `const std::string&` and returns a structure containing four string fields: domain, name, path and value. The declaration is inside `extern "C"`. [Original header](../../P7_0909/stage2/snapshots/evidence/chromium-efl/55f75e1f273b_ewk_cookie_parser.h).
2. **Static ELF inspection:** real libchromium-ewk.so exports `ewk_parse_cookie` and imports GNU string symbols. Its C linkage name does not encode these strings' concrete representation. [Symbols](../W1/raw/009_ewk_symbols.stdout).
3. **Independent sample measurement, not this provider's layout measurement:** application-side string sizeof was 32 bytes for GNU and 24 for libc++. Thus the common type name does not establish identical representation for these builds. These sizes must not be applied to every product or compilation mode. [GNU sample](../W2/raw/gnu_to_cxx_shim_1.stdout), [libc++ sample](../W2/raw/cxx_to_gnu_shim_1.stdout).
4. **Inference:** resolving a C name does not check which object bytes mean pointer, length or internal buffer. If the actual endpoints interpret one object using different layouts, wrong values or memory damage are possible. Successful linking alone would not exclude that risk.
5. **NOT_OBSERVED:** actual cross-library consumer linking/calling, four-field values, destruction, both directions and same-side controls are incomplete. The real implementation was extracted, but loading stopped at a product dependency choice. [Diagnostics](../W1/load_results.json). The chain cannot be extended to “wrong values measured in 5/5 runs.”

### Excluded and still-open approaches

Reinterpreting a custom structure as the original signature's std::string does not establish the real object's invariants or lifetime. Adding C linkage or symbol versions alone does not translate its four string fields. A single translation unit cannot use one selected std definition to directly interpret both representations; this does not mean a process cannot link both standard libraries.

A two-unit bridge remains possible in principle, but this interface needs verified routing from the same C name to the real provider without recursion or unintended interposition. The Boost sample's different C++ mangled names do not solve that automatically. Separate project origins also do not prove that every dual-export design is impossible. [Mechanism and primary sources](../W2/REPORT.md).

### Evidence that could change the finding

Demonstrating no actual cross-library boundary, or establishing compatible representation, lifetimes and calls for the exact product builds with real bidirectional/control/destruction tests, could change the present lack of a guarantee. **Equal sizeof alone is insufficient.** Evidence that this API is absent from the product's application SDK would narrow direct application impact without changing the declaration's type facts.

## SetSyncBinaryReply

### Evidence chain

1. **Static inspection:** the real table slot is `void (*)(XW_Instance, const std::vector<uint8_t>&)`, not a raw-byte interface. [Declaration](../../P7_0909/stage2/snapshots/evidence/webapi-plugins/353da9cbd3a2_XW_Extension_SyncMessage.h).
2. **Static inspection:** plugin `Instance::SendSyncBinaryReply` invokes `g_sync_messaging->SetSyncBinaryReply(xw_instance_, reply)`. The **plugin calls an engine-provided function pointer**. Reversing this direction would test a different boundary. [Real call site](../../P7_0909/stage2/snapshots/evidence/webapi-plugins/e4597e6f32ca_extension.cc).
3. **Independent sample measurement:** vector<uint8_t> sizeof was 24 bytes on both sides. This neither proves complete invariant/build-mode compatibility nor supports a claim of necessarily different sizes. [Measured sizes](../W2/raw/cxx_to_gnu_shim_1.stdout).
4. **Inference:** a function-pointer slot does not automatically convert differing std definitions. Actual misinterpretation must be tested with the real registration direction and instance context.
5. **NOT_OBSERVED:** real engine callback context, bidirectional bytes/lengths/destruction and same-side controls are not established. No hand-written receiver was substituted, so there is no target wrong-value or correct-value result. [Scope record](../W1/REPORT.md).

### Excluded and still-open approaches

Wrapping the table in C linkage does not remove vector from the parameter. Casting its old slot to a data/length function pointer does not preserve the old ABI. A simulated receiver or reversed callback is not the requested experiment. A new C table version or a two-unit bridge installed in the real registration path remains open, with jointly defined versions, data lifetime and error contracts.

### Evidence that could change the finding

The exact real endpoints passing bidirectional content, length, lifetime and control tests in the correct callback direction could strengthen the fixed-combination result. Proving that production does not use this slot, or already uses the same std implementation on both sides, would narrow the impact. One crash-free run or equal vector size does not establish safety for every plugin.

## binder and Dali

### Evidence chain

1. **Static inspection:** C# applications enter the binder through C names/handles. This is not direct transfer of a Dali C++ class from C#. [Managed/native evidence index](../../P7_0909/stage2/DALI_NUI.md).
2. **Static inspection:** `CSharp_Dali_View_New` internally calls `Control::New`, copies and allocates Control; `CSharp_Dali_delete_View` deletes it. The binder imports core/adaptor/toolkit and receives objects through native C++ operations. [Complete wrapper](../../P7_0909/stage2/snapshots/evidence/dali2-csharp-binder/b0487e765ad5_control-wrap.cpp), [ELF dependencies](../../P7_0909/stage2/raw/elf_dali2-csharp-binder_libdali2-csharp-binder.so.2.0.0_dynamic.stdout).
3. **Inference:** internal operations depend on compatible class, virtual-call, reference-count and lifetime agreements. The outer C wrapper does not establish internal consistency. Conversely, being C++ does not itself prove standard-library objects are passed or mixed libraries fail.
4. **NOT_OBSERVED:** dependency diagnostics did not reach a valid Control::New control; mixed-library values and destruction remain unmeasured. **Neither mandatory simultaneous migration nor safe staggered migration has been established.**

### Excluded and still-open approaches

Changing/testing only the C# entry does not test binder/Dali. Replacing creation with a different object while other wrappers continue interpreting it as the original Control* is not complete adaptation. A string/vector copying bridge does not cover this object graph.

An internal C handle-and-operation protocol, coordinated native migration and object-level adaptation remain open. Costs include every wrapper that interprets handles, not merely New. Native applications directly linking Dali C++ use a separate path and do not automatically receive the NUI C binding's isolation.

### Evidence that could change the finding

Real binder/Dali tests in both directions, with valid initialization, exact object-state assertions and destruction, or evidence that production combinations are not heterogeneous, could change the current limit. A complete class/object-contract analysis and broader operation coverage would strengthen it. C# P/Invoke or one export name alone cannot establish internal safety.

## Why the selected Native-wrapper edges differ

**Static inspection followed by actual compilation/linking:** Set/GetOperation in app_control.hpp converts string and C bytes inside the header; AppBase::GetId constructs a caller-side string from C char**; UiAppBase/ServiceAppBase adapt C callback tables and void*. Four representative cases were built with each standard library, producing eight final executables. Their selected platform imports are C functions, while positive controls show local C++ dependencies. [Individual ELF results](../W1/native_final_results.json), [full limitations](../W1/REPORT.md).

The selected platform edges have C data shape. **All family members, arbitrary user callbacks and real application execution have not been proved safe.** Linking allowed transitive unresolved provider symbols; no valid application context was established. Eight successful builds are not eight passing runtime samples. C ABI, allocation/release pairing and preventing unhandled C++ exceptions from escaping incompatible callback boundaries remain prerequisites to verify.

## Costs and responsibilities

| Option | Platform/component maintainers | Application/plugin maintainers | What is quantified |
| --- | --- | --- | --- |
| C data/handle interface | Define bytes, lengths, capacities, status, create/release and table versions; maintain legacy paths | Direct Native users change calls; JavaScript/C# source may remain unchanged if upper-level contracts are preserved | EWK: one located header/declaration; callback: two files, one method and one slot; Dali: one representative file/three functions. These are lower bounds, not full effort estimates |
| Two-unit/object shim | Control routing, side-local lifetimes, conversions, exceptions and version regression; bear copy costs | Verify deployed loading combinations; source transparency depends on complete contract preservation | Fixed Boost sample viable; target implementation size, copy timing and peak memory unmeasured |
| Native consumer migration | Supply matching builds and deployment combinations | Rebuild/test where source and dependencies can migrate; unavailable source prevents this route | No business application/plugin inventory; no percentage or total person-hour estimate |
| Declare unsupported combinations | Specify matching std/ABI settings, legacy scope and duration | Determine compliance and feature impact | Declarations do not repair incompatible binaries; duration and feature trade-offs require business decisions |

[Per-interface options, proposed C signatures and counting evidence](../W3/OPTIONS.md). Retaining an old name does not protect an old std layout; loading both standard libraries does not establish arbitrary C++ ABI-runtime/exception compatibility. [Clang's official toolchain documentation](https://clang.llvm.org/docs/Toolchain.html#c-standard-library).

## Uncovered scope and required confirmations

- Real target bidirectional behavior matrices with five runs per cell; valid engine, Dali and Native contexts; target libc++ providers not obtained. Load failure is not incompatibility evidence.
- Product dependency choice between elementary variants; sessiond was not obtained in Native diagnostics; the binder diagnostic batch limit does not exhaust dependency investigation.
- armv7l/aarch64, other build modes/runtime versions/loading orders. Measurements here are native x86_64 with explicitly recorded identities only.
- Cancellation, typed exception catching, iterators, aliases, cross-side ownership, concurrency, stress, leaks, copying performance and upgrades.
- Final SDK allowlists, third-party application/native-plugin BOMs, complete modification counts and legacy-support periods. Frozen recipes are not a final platform-wide exposure guarantee.

This material supports discussion of evidence prerequisites and option costs; it does not claim all target tests are complete. [Open questions](../../API_QUESTIONS_0911.md), [stage status](../../API_STATUS_0911.md), [self-check](SELF_CHECK.md). No solution recommendation or priority order is assigned; business owners and human reviewers decide.
