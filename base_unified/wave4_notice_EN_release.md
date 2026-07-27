# Wave 4 Base + Unified atomic-release coordination notice

**Audience: Tizen Base and Unified release owners | Decision deadline: 31 Oct 2026**

## Why one coordinated window is required

The aligned 0723 binary graph places **118/121/122 packages** in the corrected
Wave 4 TIER1 core on armv7l/aarch64/x86_64. On every architecture, the Base
members are exactly **jsoncpp, libicu, and libpcrecpp**; the remaining
**115/118/119 packages** are Unified packages. Reproducible C++ ABI edges run
from Unified consumers to those Base providers. After the standard-library
switch, exposing opposite sides at different migration states can disagree on
object layout, exceptions, and template instantiations. Base may build first,
but Base providers and Unified consumers must be promoted atomically; no
externally consumable half-migrated snapshot is acceptable.

**Scope note: the execution list follows the execution-time snapshot; the
aligned 0723 values are 118/121/122, while 130 remains the frozen methodology
baseline.**

For context, the bundle transition preserves the approved legacy surface with
the D-B1 **E48 exact version script**, while G4 asserts that the new main
`libbundle.so` exports no `tizen_base*` symbols. This flavor-isolation gate
complements, but does not replace, cross-repository atomicity.

## Release-window requirements

1. Use one three-architecture candidate matrix, frozen build IDs, and a joint
   go/no-go. Base builds may finish first; external promotion is simultaneous.
2. Publish neither “new Unified + old Base” nor a new Base snapshot consumable
   by old Unified. Image assembly must require a signed paired-snapshot token.
3. Treat the paired Base+Unified snapshots as the minimum rollback unit.
   Retain per-architecture last-known-good pairs and rerun G1–G7 plus board
   `LD_BIND_NOW` checks after rollback.

## Decisions required by the end of October

- release owners, joint window, freeze/withdraw authority, and go/no-go signers;
- build cut-offs and the paired snapshot/build-token format;
- policy for architecture exceptions and independently evidenced anchor waivers;
- evidence owners for E48/G4, cross-repo edges, strong-UND/binding checks, and
  image scenarios;
- atomic-promotion mechanism, rollback SLA, and last-known-good retention.

**Fallback:** if the joint window or core gates are not ready, ship a
dual-runtime image: migrated domains use libc++, the core remains on
libstdc++, and the platform default is not flipped.

Data appendix: `core_base_members.tsv`, `core_cross_repo_edges.tsv`,
`cross_repo_edges.tsv`, `affected_base.tsv`, and `affected_unified.tsv`.
