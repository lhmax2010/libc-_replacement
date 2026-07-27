# Multi-architecture census v2 method

This directory is a clean-room, aligned-snapshot rerun. It does not mutate
`census_multiarch/` or the frozen `abi_census/` evidence.

## Inputs

- Base: `tizen-base_20260709.234055`
- Unified: `tizen-unified_20260723.231947`
- Architectures: `armv7l`, `aarch64`, `x86_64`

## Frozen semantics ported

1. v5 standard-library hub exclusion, unique SONAME-provider resolution,
   target-ELF and rootstrap-copy filtering, Rust-symbol filtering, and
   CPP_NOSTL tiering.
2. D2 drops a CPP_ABI edge only when its entire VAGUE contribution is also
   exported by canonical `libstdc++.so.6`; INTERFACE and HUB_ONLY explicit
   instantiation contributions remain.
3. A8 promotes the frozen 33 evidence-confirmed layout edges only when that
   architecture currently has the same CPP_NOSTL edge.
4. A9 applies the five frozen YES_POINTEE evidence edges under the same
   current-edge condition and does not widen the core in the calibrated or
   aligned snapshots.
5. Production scope excludes package names matching
   `unittests|gtest|haltests|-tests?$`, case-insensitively.

The rule-by-rule frozen-input reproduction verification is
`methodology_equivalence.tsv`. The hard ARM calibration reproduces not only
the 123/130 sizes but every member and the complete D2/A8/A9/TIER2 component
partitions (`calibration_arm_0716/`).

## Discovery boundary

A8 and A9 in this aligned rerun replay the frozen evidence-confirmed edge
conditions against each architecture's current graph. They do not rerun the
A6 layout-member discovery or A9 pointee/tunnel discovery on the aligned
snapshot. Calibration therefore verifies reproduction of frozen inputs; it
does not claim that a newer snapshot has no newly hidden couplings.

After every snapshot alignment, and again before the core wave starts, the
A6/A9 discovery scans must run on that snapshot. Newly observed CPP_NOSTL
edges, promoted layout edges and disappeared edges then enter the mechanical
edge-diff and human triage required by HLD errata v2.

The implementation scripts remain outside this conclusion-only repository
and are content-addressed in `REF_INDEX.md`.

`components_production_tier1.tsv` is D2+A8+A9 and is authoritative.
`components_production_tier2.tsv` is D2+all CPP_NOSTL. Raw phase outputs are
retained with `raw_v5_superseded` suffixes.
