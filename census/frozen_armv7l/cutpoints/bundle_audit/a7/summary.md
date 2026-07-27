# A7 — bundle cut-plan evidence completion

## 1. Full-symbol rescan of the 84 intra-consumer edges

The 44-package set contains exactly 84 graph edges (`CPP_ABI=46`, `C=38`). Replaying NEEDED-scoped canonical-provider resolution from `elf_analysis.sqlite3` produced 919 complete resolved UND rows in `intra_consumer_all_resolved_symbols.tsv`; no graph edge failed reconstruction.

The complete scan finds **13 edges / 31 symbol rows / 13 consumer packages** carrying `tizen_base::Bundle`. It finds no `KeyInfo`-carrying edge. This replaces the earlier sample-column observation of 1 edge: the five-symbol cap missed 12 Bundle-bearing edges.

Consumers: `app-core-tcore`, `app-core-ui`, `app-defined-loader`, `appcore-agent`, `appcore-widget`, `appcore-widget-base`, `appcore-widget-base-tcore`, `component-based`, `component-based-tcore`, `component-based-widget`, `liblaunchpad`, `libwatchface-complication-provider`, `team-application`.

The full edge/symbol evidence is `bundle_keyinfo_cross_edges.tsv`; edge-level totals are `intra_consumer_edges.tsv` and `a7_stats.tsv`.

## 2. Impl members

- `Bundle::Impl`: **NO_OBSERVED**, binary-layout inference. The complete constructor initializes only vptr/scalar/pointer-shaped fields and its complete destructor is empty.
- `KeyInfo::Impl`: **YES, inline `std::__cxx11::basic_string`**, direct binary-layout evidence. Constructor/destructor offsets and the GCC string SSO test are recorded verbatim.
- Both public wrapper objects themselves directly declare `std::unique_ptr<Impl>` in `bundle_cpp.h`.

There is no usable DWARF in the unpacked snapshot. Evidence, confidence boundaries, and the reusable A6 decision procedure are in `impl_members.md`.

## 3. EXPORT_API visibility

`readelf --dyn-syms -W` confirms that `libbundle.so.0.18.15` default-exports **49** `_ZN10tizen_base6Bundle*` rows: **25 public Bundle member symbols**, **10 public KeyInfo member symbols**, and **14 Impl constructor/destructor symbols**. Thus the public Bundle/KeyInfo export surface is 35 dynamic symbols; the opaque Impl surface is also unintentionally visible. Full mangled/demangled rows and raw readelf lines are in `bundle_keyinfo_exports.tsv` and `libbundle_dynsym_readelf.log`.

## 4. Snapshot-external publication surface

YES: `bundle_cpp.h` is shipped both by `bundle-devel` at `/usr/include/bundle_cpp.h` and by `app-rootstrap-devel` at `/opt/data/app-rootstrap/usr/include/bundle_cpp.h`. The two files have the same SHA256 (`7f818af27923d404f40a55558f8966146c80b274a321b26fa65fa1eeb9007104`). Ownership/RPM evidence is in `bundle_cpp_header_owners.tsv`.

## Data-side assessment of the header-only-inline option

- Layout freeze: not standard-library-neutral for the full wrapper. `KeyInfo::Impl` embeds the GCC C++11 string representation, and both public classes embed `std::unique_ptr`; only `Bundle::Impl` has no observed inline STL member.
- Coexistence risk: real and wider than the earlier sample suggested. Bundle objects occur on 13 distinct intra-S edges, not 1; KeyInfo objects do not occur on these edges.
- Old-export retirement: the snapshot export surface is explicitly enumerable (35 public symbols), but consumer retirement is not bounded to the 44 packages because the identical C++ header is published through `app-rootstrap-devel` as well as `bundle-devel`.

These facts constrain the option; they do not choose between HLD alternatives.
