# Target-interface continuation: findings and limits

2026-09-11. **PARTIAL; for human review, not compatibility acceptance.** The product dependency was explicitly confirmed as `elementary-tizen`. This supplement supersedes the earlier draft's statements that the product choice was unresolved and no target values had been measured. The sealed earlier reports are not rewritten.

## Results

| Target | x86_64 native and armv7l physical-board evidence | Remaining limit |
| --- | --- | --- |
| `ewk_parse_cookie` | On each architecture: GNU caller/GNU real Chromium provider returns all four expected strings in 5/5 runs; libc++ caller/GNU provider encounters SIGSEGV during the call in 5/5 | The measured mixed direction is incompatible. No real libc++ Chromium provider was available for the reverse direction or its same-library control |
| `SetSyncBinaryReply` | Real browser callback and assignment implementation identified. Separate vector representation measurements completed | Actual callback on a valid instance remains `NOT_OBSERVED`; no mock receiver or service-side no-op substituted |
| binder/Dali | On each architecture, unchanged property-wrapper source subset against a real, fully built core: both directions and both controls pass 5/5. Full GNU binder with GNU dependencies also passes 5/5 | Replacing **only core** with libc++ makes the full GNU dependency chain fail to load in 5/5 on each architecture. This is not a test of an entirely migrated Dali stack |

The [primary matrix](MATRIX.md) contains 16 executed cells and 80 attempts: 60 exact-value results, 10 EWK fault-signal results and 10 dependency-loading failures. **80 is not the denominator for complete requested coverage.** Auxiliary probes, earlier reruns, initialization diagnostics and shim runs are excluded from this count.

EWK input was `sid=ABC123; Domain=example.test; Path=/probe`, with initially empty output. Both controls check domain, name, path, value and zero expiry, followed by consumer-scope destruction. String/output-structure sizes are 32/136 versus 24/104 bytes on x86_64, and 24/100 versus 12/52 on armv7l. These numbers describe these builds, not every ABI mode. ARM probes record the actual fault signal and exit unsuccessfully without invoking the board's crash-dump handler; this differs from measuring default core-dump handling.

Dali property tests check three integers, one exact float and three strings, with deletion through the actual wrapper. The string type on this path is **Dali::String**, not std::string. The rebuilt wrapper is a **source subset**, not the complete binder package. The libc++ core provider is a complete build of unchanged real core source, not a mock. The full-stack loading failure identifies the old adaptor's GNU-vector form of `Scene::GetFrameRenderedCallback`; the new core exports the libc++ namespace form. It does not establish that every pair of Dali components must migrate simultaneously.

Full GNU `Control::New` also failed during an initialization-precondition probe on x86_64. No working same-library graphics/virtual-call control was established, so that failure is **not attributed to cross-library ABI mixing**.

## Target-specific shim assessment

**EWK:** a real, signature-preserving two-translation-unit prototype was tested against the actual GNU Chromium provider. The libc++ front end and GNU back end exchange pointers, lengths and scalar state. The complete-cookie/empty-output sample passes 5/5 on x86_64, with real-provider/shim/runtime maps checked.

However, an additional valid-input test found a **silent semantic defect in the prototype**. With pre-existing domain/path values and input `sid=ABC123`, direct GNU calls retain those values in 5/5 runs. The prototype clears them in 5/5 while still returning success and completing destruction. Its back end starts from empty output and fails to preserve the caller's previous state. This is recorded, not hidden or treated as acceptance. Pointer/length transport is demonstrably possible for the fixed sample, but **deployment remains `SHIM_UNCERTAIN`**. The prototype has two translation units, three data-path functions and two configuration/lookup helpers; these are not a complete deployment cost estimate.

**Binary reply:** bytes are expressible as pointer plus length, with a receiving-library vector reconstructed on the engine side. A pair of conversion entry points is not enough to establish deployment: the actual callback table, valid instance registration, context lifetime, reentrancy and failure behavior must be connected and tested. The reply vector travels **from native plugin to engine**, irrespective of the initial request direction. `SHIM_UNCERTAIN`.

**Dali objects:** value data may be converted, and object pointers may remain opaque handles. A pointer does not translate an object's C++ layout, virtual dispatch or destruction contract. Full coverage would require a defined operation, callback and ownership protocol, not one universal pair of byte converters. Its method/lifecycle scope has not been quantified or tested. `SHIM_UNCERTAIN`.

## Vector representation versus actual interface behavior

On x86_64, frozen GNU 14.2 headers and libc++ headers each received five runs with four reserved-capacity states: lengths 0, 1, 4 and 257, capacity 512. Both 24-byte, 8-aligned objects matched three observed machine words to data-begin, used-end and allocation-end. Every payload byte was asserted; copying and destruction within the object's own library were exercised. Earlier GNU 13.3-header observations are retained separately.

These measurements go beyond sizeof, but **do not call SetSyncBinaryReply or prove foreign-library assignment/destruction safe**. Debug modes, custom allocators, default-unallocated empty objects, exceptions and the real instance lifecycle are outside this measurement. Loading the actual provider in each process does not turn a representation probe into a target-call test.

## Evidence and remaining prerequisites

The [Chinese report](REPORT.md), [source excerpts](SOURCE_FACTS.md), per-run matrix links and manifests identify the real components, build commands and concrete observations. The source archive comparison covers 1,100 core files and 477 binder files with no content differences and a positive checksum control. ARM execution used a physical rpi4; native Clang generated ARM code after the old QEMU-hosted compiler wrapper failed. No platform source, system runtime or platform configuration was changed.

Remaining prerequisites include a real libc++ Chromium provider and matching configuration; a valid browser/plugin instance setup; the complete migrated Dali dependency set and graphics initialization; production SDK and application inventories. The real browser initialization probe waited for a Wayland server and timed out; the service implementation merely reports unsupported and was not used as a data-transfer control.

Uploaded board files were hash-verified, removed individually from the task-exclusive directory, and directory absence was confirmed. The board is released. Final canonical probes/scripts have snapshots and SHA256 manifests; intermediate script versions and some display-only log-reading commands were not all independently archived. The [self-check](SELF_CHECK.md) explicitly retains these audit and coverage gaps. No deployment recommendation or migration ordering decision is made.
