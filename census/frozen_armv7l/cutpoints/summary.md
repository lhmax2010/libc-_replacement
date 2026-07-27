# Core-wave C++ ABI cutpoint analysis

## Scope and method

- Production predicate: case-insensitive `unittests|gtest|haltests|-tests?$`; 201 test packages removed, 1804 production packages retained.
- Baseline core: TIER1 239 packages (1→1521, 2→9, 3→3, 5→2, 7→1, 239→1); TIER2 312 packages (1→1451, 2→10, 3→3, 5→1, 7→1, 312→1).
- Symbol rows are rebuilt from landed UND/export/version/demangle tables using v5 canonical SONAME providers and the v4_active gate. No ELF or symbols were recollected.
- Simulation changes every production CPP_ABI/CPP_NOSTL edge incident to the selected hub(s) to C (implemented as removing those edges from the corresponding undirected component layer).
- Cost rule (data-side only): LOW requires at most 10 visible STL clusters and at least 60% of resolved occurrences in the top three clusters; otherwise HIGH. CPP_NOSTL clusters are reported but excluded from the visible-STL threshold.

## Hub comparison

| hub | inbound ABI/NOSTL | core inbound T1/T2 | clusters I/V/N | unique STL symbols | top-3 cluster share | main std types (edge-symbol rows) | cost | cut max T1/T2 |
|---|---:|---:|---:|---:|---:|---|---|---:|
| `bundle` | 84/6 | 84/90 | 7/2/4 | 12 | 69.9% | string:200, vector:12, initializer_list:11, pair:11 | **LOW** | 230/304 |
| `aul` | 76/0 | 76/76 | 0/4/0 | 4 | 95.3% | string:140 | **LOW** | 226/297 |
| `app-core-common` | 53/3 | 53/56 | 3/4/25 | 7 | 87.8% | string:100, shared_ptr:15 | **LOW** | 196/305 |
| `rpc-port` | 20/0 | 20/20 | 0/3/0 | 3 | 100.0% | string:38 | **LOW** | 237/311 |

## Data-side C API cost judgments

- `bundle` — **LOW**: 9 visible STL clusters (7 INTERFACE, 2 VAGUE), 12 unique STL symbols, 276 resolved occurrences; top three clusters account for 69.9%. The visible surface is concentrated in Bundle construction/Add/Get families.
- `aul` — **LOW**: 4 visible STL clusters (0 INTERFACE, 4 VAGUE), 4 unique STL symbols, 215 resolved occurrences; top three clusters account for 95.3%. All visible STL rows are VAGUE and none are INTERFACE; symbol visibility/export cleanup may precede API redesign.
- `app-core-common` — **LOW**: 7 visible STL clusters (3 INTERFACE, 4 VAGUE), 7 unique STL symbols, 164 resolved occurrences; top three clusters account for 87.8%. Only three INTERFACE clusters carry STL, but 25 CPP_NOSTL families make the TIER2 proxy materially broader.
- `rpc-port` — **LOW**: 3 visible STL clusters (0 INTERFACE, 3 VAGUE), 3 unique STL symbols, 56 resolved occurrences; top three clusters account for 100.0%. All visible STL rows are VAGUE and none are INTERFACE; the measured signature-conversion surface is minimal.

## Cut simulation

Baseline production maximum is TIER1 **239**, TIER2 **312**.

| scenario | removed edges T1/T2 | largest T1 | reduction | largest T2 | reduction |
|---|---:|---:|---:|---:|---:|
| `bundle` | 84/90 | 230 | 9 | 304 | 8 |
| `aul` | 77/78 | 226 | 13 | 297 | 15 |
| `app-core-common` | 54/57 | 196 | 43 | 305 | 7 |
| `rpc-port` | 21/21 | 237 | 2 | 311 | 1 |
| `all_four` | 234/244 | 114 | 125 | 250 | 62 |

## Data-only priority

Priority sorts individual cuts by the sum of TIER1+TIER2 maximum-component reduction, then by fewer visible STL clusters. It is a graph/symbol recommendation, not an implementation estimate.

1. `app-core-common` — combined reduction 50; 7 visible STL clusters; cost LOW
2. `aul` — combined reduction 28; 4 visible STL clusters; cost LOW
3. `bundle` — combined reduction 17; 9 visible STL clusters; cost LOW
4. `rpc-port` — combined reduction 3; 3 visible STL clusters; cost LOW

All-four result: TIER1 **114**, TIER2 **250**.

## Evidence index

- `cutpoint_metrics.tsv`: four-hub comparison metrics used by this summary.
- `cutpoint_<hub>_inbound_edges.tsv`: full production inbound edge lists and core membership.
- `cutpoint_<hub>_symbols.tsv`: full contributing resolved symbol surface with demangles.
- `cutpoint_<hub>_clusters.tsv`: function-family aggregation, INTERFACE/VAGUE/NOSTL separated.
- `cutpoint_<hub>_std_types.tsv`: standard-library type distributions.
- `cut_simulation.tsv`: all five requested scenarios and largest-component members.
