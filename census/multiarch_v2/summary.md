# Multi-architecture census v2 summary

## Status

All hard gates passed. Frozen ARM calibration reproduced D2 core 123 and
D2+A8+A9 core 130 with exact member sets and reproduced every component in all
four calibrated partitions. The aligned 0723 rerun used fresh RPM downloads
and extraction for every architecture; the three symbol databases pass
`PRAGMA integrity_check`.

## Aligned 0723 results

| Architecture | Production packages | TIER1 core | TIER2 core | Anchors |
|---|---:|---:|---:|---:|
| armv7l | 1795 | 118 | 236 | 5 |
| aarch64 | 1801 | 121 | 237 | 9 |
| x86_64 | 1795 | 122 | 237 | 11 |

TIER1 is the corrected lower-bound graph: D2-retained CPP_ABI plus A8/A9
evidence promotions. TIER2 adds all current CPP_NOSTL edges.

## Component isomorphism

| Tier | Architecture | Exact shared components / total | Packages in exact shared components |
|---|---|---:|---:|
| TIER1 | armv7l | 1520 / 1563 (97.25%) | 88.41% |
| TIER1 | aarch64 | 1520 / 1566 (97.06%) | 88.12% |
| TIER1 | x86_64 | 1520 / 1561 (97.37%) | 88.41% |
| TIER2 | armv7l | 1455 / 1495 (97.32%) | 84.18% |
| TIER2 | aarch64 | 1455 / 1499 (97.06%) | 83.90% |
| TIER2 | x86_64 | 1455 / 1493 (97.45%) | 84.18% |

Package presence union is 3847: 3531 occur on all three
architectures, 316 do not, and 186 occur on only one architecture.
Full members and attribution are in `comparison/package_presence.tsv` and
`comparison/single_arch_packages.tsv`.

## Corrected core counterpart

| Architecture | Component | Size | ARM-v2 member overlap | Status |
|---|---|---:|---:|---|
| armv7l | tier1_production_0001 | 118 | 118 | EXACT_SELF_REFERENCE |
| aarch64 | tier1_production_0001 | 121 | 118 | PER_ARCH_DIFFERENCE |
| x86_64 | tier1_production_0001 | 122 | 116 | PER_ARCH_DIFFERENCE |

The armv7l self-check is exact. The historical frozen-0716 130-package set
projects onto two components on the aligned snapshot because four packages are
absent and the screen-connector subgroup no longer joins the largest
component; this is reported separately in
`comparison/core130_counterparts.tsv` and is not a calibration failure.

## cynara19 regression

| Architecture | Packages present | TIER1 boundary | TIER2-only boundary | Status |
|---|---:|---:|---:|---|
| armv7l | 19 | 0 | 0 | PASS |
| aarch64 | 19 | 0 | 0 | PASS |
| x86_64 | 19 | 0 | 0 | PASS |

All three architectures have zero corrected production boundary edges.

## Anchors

| Architecture | Count | Packages |
|---|---:|---|
| armv7l | 5 | drm-service-core-tizen,hal-api-drm-haltests,hal-backend-drm-generic,pen-wave,pen-wave-devel |
| aarch64 | 9 | drm-service-core-tizen,hal-api-drm-haltests,hal-backend-drm-generic,libedgetpu1-max,libedgetpu1-std,pen-wave,pen-wave-devel,rust,rust-devel |
| x86_64 | 11 | dotnet-build-tools,drm-service-core-tizen,hal-api-drm-haltests,hal-backend-drm-generic,jdk,libedgetpu1-max,libedgetpu1-std,pen-wave,pen-wave-devel,rust,rust-devel |

Anchor evidence was rebuilt from each architecture's aligned artifacts.
`anchor_evidence_freshness.tsv` binds every verdict to current NEVRA and the
per-architecture evidence hash.

## Ledger recommendation

Data supports a shared main ledger only for exact member+edge digests, with a
per-architecture override layer for every differing component. TIER1 has
shared main ledger for exact member+edge digests (tier1_shared_component_count =
1520); the minimum exact-shared package ratio is
0.88117712.
The policy decision remains human.

## Supersession

`census_multiarch/` was not modified. `V1_SUPERSEDED.md` marks its conclusions
as superseded from v2, and `supersede_mapping.tsv` maps every retained v1
conclusion/evidence file to a corrected v2 counterpart or explicitly marks it
`RETIRED`.
