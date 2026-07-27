# BASE/UNIFIED affected RPM split — aligned census v2

## Scope

“Affected” means a package is an endpoint of at least one corrected production
TIER1 or TIER2 edge on armv7l, aarch64, or x86_64. Pure-C and edge-free
singleton nodes retained by Union-Find are not silently counted as affected.
Base is `tizen-base_20260709.234055`; Unified is `tizen-unified_20260723.231947`.

## Counts

- Union across three architectures: **332** packages: Base
  **25**, Unified **307**,
  unknown/conflict **0**.
- Per architecture affected endpoints: armv7l **323**,
  aarch64 **325**, x86_64
  **326**.
- Cross-repo effective edges per architecture:
  armv7l **58**
  (CPP_ABI 32, CPP_NOSTL
  26), aarch64
  **58**
  (32/26),
  x86_64 **62**
  (33/29).
  Every observed direction is Unified consumer → Base provider.

Wave preassignment counts are mechanical and may overlap in the combined
`WAVE_2+BASE_FIRST_PROVIDER` label:
`BASE_FIRST`=5, `LONG_TAIL`=117, `WAVE_1`=19, `WAVE_1_5`=20, `WAVE_2`=43, `WAVE_2+BASE_FIRST_PROVIDER`=1, `WAVE_4_CORE`=127.

`BASE_FIRST=5` is the affected-dynamic-graph table count: `boost-iostreams`
has no production incoming edge and therefore is absent from that mechanical
label. D-G3's six-package execution list nevertheless includes
`boost-iostreams` as an explicit Base provider rebuild; `boost-log` is a
same-window companion rather than a seventh Base package.

## Wave 4 cross-repository surface

The aligned-v2 TIER1 core is 118/121/122 packages on
armv7l/aarch64/x86_64. **All three cores contain exactly the same three Base
members: `jsoncpp`, `libicu`, `libpcrecpp`; no Base member was added or
removed from the frozen three-package expectation.** The corresponding Unified
members are 115/118/119.

Core-internal cross-repo effective edges are:
armv7l **27**,
aarch64 **28**,
x86_64 **30**.
Every one is Unified → Base. Full symbol samples and edge layers are in
`core_cross_repo_edges.tsv`.

This is the binary-graph evidence for a coordinated Base+Unified Wave 4
release: component connectivity cannot be preserved if the Base providers and
Unified consumers are promoted in separate externally visible states.

## September Base-first eight

The v1-era selected list is:
`abseil-cpp, boost-filesystem, boost-iostreams, boost-program-options, boost-test, boost-thread, libsigc++, taglib`.

- All eight RPMs exist in Base on all three architectures.
- Seven are comparable in the production graph; all seven have an exact
  armv7l-v2 vs frozen TIER1 member set and exact cross-architecture TIER1
  member sets.
- `boost-test` is excluded from the production graph by the frozen
  `-tests?$` package-name rule. Its artifact exists, but production component
  stability is therefore **not determinable under this method**.
- Six of the eight currently have production Unified→Base incoming edges.
  `boost-iostreams` has no production incoming edge (its prior consumer was
  test-only); `boost-test` is production-filtered.
- `abseil-cpp` remains in the exact 44-package vision/ML TIER1 component, so
  Base-first execution must be scheduled together with Wave 2 consumers.
- TIER2 sets for abseil/boost-filesystem/boost-program-options/boost-thread/
  libsigc++ are per-architecture because they join the architecture-specific
  large upper-bound component. This does not invalidate their exact TIER1
  stability; it requires the per-arch overlay already prescribed by census v2.

## Files

- `affected_base.tsv`, `affected_unified.tsv`: package split, per-arch
  component IDs/sizes, edge counts, and wave preassignment.
- `cross_repo_edges.tsv`: all corrected cross-repo dependencies.
- `core_base_members.tsv`, `core_cross_repo_edges.tsv`: Wave 4 evidence.
- `base_first_8_verification.tsv`, `base_first_component_diffs.tsv`: September
  provider audit.
- `wave4_notice_CN.md`, `wave4_notice_EN.md`: owner-facing notices.
