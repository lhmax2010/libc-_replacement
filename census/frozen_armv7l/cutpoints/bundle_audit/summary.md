# Bundle C++ wrapper cut audit

## Result

- Consumer scope: 44 packages with at least one `INTERFACE` row in `cutpoint_bundle_symbols.tsv`.
- Actual cost surface: 129 unique `(package, bundle C++ symbol)` uses across 10 public C++ overloads.
- EASY: 44
- MEDIUM: 0
- HARD: 0

`c_mappable_ratio` uses `有=1`, `部分=0.5`, `无=0` over unique actually used mangled symbols. Grade rule: any `无` is HARD; otherwise any `部分` is MEDIUM; all `有` is EASY.

## Interface inventory

`bundle_cpp.h` contains 37 public C++ API/type rows: 21 with public-C semantic equivalents, 11 partial/internal/composite equivalents, and 5 with no C API ownership-transfer equivalent. Full declarations and evidence are in `cpp_to_c_mapping.tsv`.

Headers are classified in `header_inventory.tsv`: `bundle.h` is the public C API, `bundle_internal.h` is internal/legacy C API, `bundle_cpp.h` is the public C++ wrapper (`tizen_base::Bundle` and `KeyInfo`), and `SLP_bundle_PG.h` is documentation only.

### Interfaces with no C equivalent

- `tizen_base::Bundle::KeyInfo::KeyInfo(KeyInfo&&)` — `bundle_cpp.h:98`
- `tizen_base::Bundle::KeyInfo::operator=(KeyInfo&&)` — `bundle_cpp.h:105`
- `tizen_base::Bundle::Bundle(Bundle&&)` — `bundle_cpp.h:196`
- `tizen_base::Bundle::operator=(Bundle&&)` — `bundle_cpp.h:203`
- `tizen_base::Bundle::Detach()` — `bundle_cpp.h:344`

## HARD consumers

- None. `hard_interfaces.tsv` contains only its header.


## Binary structure

`readelf --dyn-syms -W` finds the C++ `tizen_base::Bundle`/`KeyInfo` exports in `bundle/usr/lib/libbundle.so.0.18.15` itself (`libbundle_cpp_exports_mangled_readelf.txt`, with demangled evidence in `libbundle_cpp_exports_readelf.txt`). Its SONAME is `libbundle.so.0` (`libbundle_soname_readelf.txt`). The package layout contains no separate C++ wrapper library: the runtime package has the real `libbundle.so.0.18.15` plus SONAME symlink, while `bundle-devel` has only the link-time `libbundle.so -> libbundle.so.0` symlink (`bundle_library_layout.txt`).

Therefore the cut shape is: **header-level deprecation/removal of `bundle_cpp.h` plus rewrites in consumers; there is no independent wrapper `.so` to detach.**

## One-line workload judgment

The measured work is broad but mechanically shallow: all 44 consumers are EASY and their observed surface is limited to 10 fully public-C-mappable overloads, so the dominant cost is coordinated source rewrites across packages rather than designing replacement semantics.
