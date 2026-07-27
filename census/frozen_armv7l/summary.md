# Tizen C++ ABI census — final report (v5 + stage 3)

## Status and immutable inputs

- Generated (UTC): `2026-07-17T09:51:14+00:00`.
- Prompt A is complete. No dynamic symbols were recollected in v5; platform binaries and repository state were not modified.
- `repo.base-standard`: `https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base/tizen-base_20260709.234055/repos/standard/packages/armv7l/` (observed `2026-07-17T06:47:46+00:00`).
- `repo.unified-standard`: `https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Unified/tizen-unified_20260716.042355/repos/standard/packages/armv7l/` (observed `2026-07-17T06:47:46+00:00`).
- Repository inventory: **2853 binary RPMs**, **854 devel RPMs**, **0 debug RPMs**.

## v5 graph corrections

- Variant canonicalization folded **17 SONAMEs** (EXTENSION_VARIANTS=3, PROFILE_VARIANTS=12, SAME_BASENAME_VARIANTS=2); canonical choice and all SHA256s are in `variant_families.tsv`. Remaining true ambiguity: **9 SONAMEs**, only **2 consumer/SONAME pairs**.
- Focus outcome: `libgiomm`, `elementary_test`, and every LAPACK focus ELF are **0 unresolved**. One related residual remains in `libglibmm_generate_extra_defs`: `_ZNSt7__cxx117collateIcE2idE@GLIBCXX_3.4.21` (`std::__cxx11::collate<char>::id`), absent from every canonical DT_NEEDED provider.
- Non-WEAK unresolved fell from v4 **74637** to **21254 / 697598**; legal unresolved WEAK **22745 / 32644**.
- Edge layers: STDLIB **4508**, CPP_ABI **890**, CPP_NOSTL **209**, corrected pure C **7980**; total **13587**.
- Rust exclusion remains effective across both C++ layers: **0** CPP_ABI/CPP_NOSTL edges involving `rust-*` packages.

## TIER1/TIER2 migration-unit bounds

- TIER1 (CPP_ABI only, lower bound): **1570 components**, distribution **1→1555, 2→10, 3→2, 5→1, 7→1, 412→1**, largest **412**. Without `unittests|gtest`: **1621**, distribution **1→1604, 2→11, 3→3, 5→1, 7→1, 272→1**, largest **272**.
- TIER2 (CPP_ABI + CPP_NOSTL, upper bound): **1495 components**, distribution **1→1480, 2→11, 3→2, 7→1, 490→1**, largest **490**. Without `unittests|gtest`: **1546**, distribution **1→1529, 2→12, 3→3, 7→1, 350→1**, largest **350**.
- TIER1 or TIER2 as the governance-wave definition is an HLD decision; this census does not select one.

TIER2-new components with at least three packages:

- `tier2_component_0001` — **490** 包 — 测试配套 + 应用框架/包管理 + UI/图形/窗口系统（仅命名推断） — `abseil-cpp,alarm-manager-unittests,alarm-server,ambient-viewer,amd,amd-mod-component-manager,amd-mod-cynara-core,amd-mod-input,amd-mod-light-user,amd-mod-proc-group,amd-mod-rpc-port,amd-mod-screen-resolution,angle,app-core-common,app-core-efl,app-core-multiwindow,app-core-multiwindow-tcore,app-core-tcore,app-core-ui,app-core-unittests,app-defined-loader,app-installers,app-installers-tests,appcore-agent,appcore-agent-unittests,appcore-ui-app-ambient,appcore-watch-unittests,appcore-widget,appcore-widget-base,appcore-widget-base-tcore,appcore-widget-unittests,askuser-notification,askuser-notification-test,askuser-notification-unit-tests,aul,aul-unittests,aurum,aurum-bootstrap,auth-fw,auth-fw-test,badge,badge-unittests,bluetooth-haltests,boost-filesystem,boost-iostreams,boost-locale,boost-log,boost-program-options,boost-test,boost-thread；另 440 包，完整清单见 tier2_new_components_ge3.tsv`.
- `tier2_component_0002` — **7** 包 — 机器学习/视觉/媒体 + 系统服务/HAL/IPC（仅命名推断） — `nnstreamer-lua,nnstreamer-onnxruntime,nnstreamer-python3,nnstreamer-single,nnstreamer-tizen-hal,nnstreamer-tvm,tvm-runtime`.
- `tier2_component_0004` — **3** 包 — 混合 C++ 依赖簇（仅命名推断） — `lldb,lldb-devel,llvm`.

- mm result: `libgiomm`, `libglibmm`, and `libsigc++` are TIER1 singletons but all enter TIER2 `tier2_component_0001` (490 packages) through CPP_NOSTL edges.

Filtered indegree top 20:

- TIER1: `gtest`=156, `bundle`=113, `aul`=108, `app-core-common`=66, `libsecurity-manager-client`=32, `rpc-port`=31, `isf`=17, `app-installers`=14, `dali2`=13, `opencv`=12, `abseil-cpp`=11, `parcel`=11, `dali2-adaptor`=10, `jsoncpp`=10, `nntrainer-core`=10, `tizen-core`=10, `capi-appfw-event`=9, `manifest-parser`=9, `app-core-multiwindow`=8, `klay`=8.
- TIER2: `gtest`=157, `bundle`=120, `aul`=108, `app-core-common`=69, `dali2`=32, `libsecurity-manager-client`=32, `rpc-port`=31, `dali2-adaptor`=25, `manifest-parser`=21, `isf`=18, `opencv`=18, `libicu`=16, `parcel`=16, `app-installers`=14, `libcynara-commons`=14, `abseil-cpp`=13, `dali2-toolkit`=12, `jsoncpp`=12, `nntrainer-core`=10, `tizen-core`=10.

### Unresolved non-WEAK top 20

1. `PyErr_SetString` — 130 — `PyErr_SetString`
2. `_Py_Dealloc` — 129 — `_Py_Dealloc`
3. `PyExc_TypeError` — 116 — `PyExc_TypeError`
4. `PyErr_Occurred` — 114 — `PyErr_Occurred`
5. `_Py_NoneStruct` — 113 — `_Py_NoneStruct`
6. `PyModuleDef_Init` — 109 — `PyModuleDef_Init`
7. `PyExc_ValueError` — 107 — `PyExc_ValueError`
8. `PyErr_Format` — 105 — `PyErr_Format`
9. `PyLong_FromLong` — 99 — `PyLong_FromLong`
10. `PyErr_NoMemory` — 93 — `PyErr_NoMemory`
11. `PyMem_Free` — 85 — `PyMem_Free`
12. `PyList_New` — 82 — `PyList_New`
13. `PyMem_Malloc` — 82 — `PyMem_Malloc`
14. `PyUnicode_FromString` — 82 — `PyUnicode_FromString`
15. `PyErr_Clear` — 81 — `PyErr_Clear`
16. `PyObject_IsTrue` — 81 — `PyObject_IsTrue`
17. `PyExc_RuntimeError` — 79 — `PyExc_RuntimeError`
18. `PyObject_GetAttrString` — 79 — `PyObject_GetAttrString`
19. `PyTuple_New` — 75 — `PyTuple_New`
20. `PyBytes_FromStringAndSize` — 74 — `PyBytes_FromStringAndSize`

Top 10 consuming ELF files:

1. `python3-testsuite:/usr/lib/python3.14/lib-dynload/_testcapi.cpython-314-arm-linux-gnueabi.so` — 518
2. `capi-base-utils:/usr/lib/libbase-utils-i18n.so.3.1.5` — 460
3. `python3-base:/usr/lib/python3.14/lib-dynload/_testlimitedcapi.cpython-314-arm-linux-gnueabi.so` — 323
4. `python3-numpy:/usr/lib/python3.14/site-packages/numpy/_core/_multiarray_umath.cpython-314-arm-linux-gnueabi.so` — 316
5. `bind:/usr/lib/libns.so.1307.1.2` — 301
6. `python3-lxml:/usr/lib/python3.14/site-packages/lxml/etree.cpython-314-arm-linux-gnueabi.so` — 283
7. `dali2-extension-rive-animation-view:/usr/lib/libdali2-extension-rive-animation-view.so.0.0.0` — 233
8. `python3-cython:/usr/lib/python3.14/site-packages/Cython/Compiler/Code.cpython-314-arm-linux-gnueabi.so` — 230
9. `python3-lxml:/usr/lib/python3.14/site-packages/lxml/objectify.cpython-314-arm-linux-gnueabi.so` — 221
10. `perl:/usr/lib/perl5/5.42.0/armv7l-linux-thread-multi/auto/re/re.so` — 220

## Compiler baseline and anchors

- `.comment` gate: **3569** active shared libraries; **219** contain recognized compiler strings. Modal baseline **GCC 14.2.0**, **197 ELF**. Signal c is exactly `family=GCC AND major<14`; observed old versions: GCC 8.5.0=34 ELF, GCC 9.2.0=2 ELF.
- Candidate signals: a=20 packages, c=19 packages; `_ZNSt*` gate leaves **5 anchors**. Signal b is disabled because both repositories have debug_count=0.

Anchors:

- `drm-service-core-tizen` — signal `a` — `_ZNSt*` EXPORT/UND `3/3` — TIER1 `tier1_component_0385`, TIER2 `tier2_component_0354`.
- `hal-api-drm-haltests` — signal `a` — `_ZNSt*` EXPORT/UND `63/26` — TIER1 `tier1_component_0001`, TIER2 `tier2_component_0001`.
- `hal-backend-drm-generic` — signal `a` — `_ZNSt*` EXPORT/UND `2/5` — TIER1 `tier1_component_0564`, TIER2 `tier2_component_0530`.
- `pen-wave` — signal `c` — `_ZNSt*` EXPORT/UND `534/54` — TIER1 `tier1_component_1147`, TIER2 `tier2_component_1086`.
- `pen-wave-devel` — signal `c` — `_ZNSt*` EXPORT/UND `534/54` — TIER1 `tier1_component_1148`, TIER2 `tier2_component_1087`.

Anchored components: TIER1 **5**, TIER2 **5**.

- TIER1 `tier1_component_0001` — 412 包 — anchors `hal-api-drm-haltests` — members `abseil-cpp,alarm-manager-unittests,alarm-server,ambient-viewer,amd,amd-mod-component-manager,amd-mod-cynara-core,amd-mod-input,amd-mod-light-user,amd-mod-proc-group,amd-mod-rpc-port,amd-mod-screen-resolution,app-core-common,app-core-multiwindow,app-core-multiwindow-tcore,app-core-tcore,app-core-ui,app-core-unittests,app-defined-loader,app-installers,app-installers-tests,appcore-agent,appcore-agent-unittests,appcore-ui-app-ambient,appcore-watch-unittests,appcore-widget,appcore-widget-base,appcore-widget-base-tcore,appcore-widget-unittests,askuser-notification`；另 382 包.
- TIER1 `tier1_component_0385` — 1 包 — anchors `drm-service-core-tizen` — members `drm-service-core-tizen`.
- TIER1 `tier1_component_0564` — 1 包 — anchors `hal-backend-drm-generic` — members `hal-backend-drm-generic`.
- TIER1 `tier1_component_1147` — 1 包 — anchors `pen-wave` — members `pen-wave`.
- TIER1 `tier1_component_1148` — 1 包 — anchors `pen-wave-devel` — members `pen-wave-devel`.
- TIER2 `tier2_component_0001` — 490 包 — anchors `hal-api-drm-haltests` — members `abseil-cpp,alarm-manager-unittests,alarm-server,ambient-viewer,amd,amd-mod-component-manager,amd-mod-cynara-core,amd-mod-input,amd-mod-light-user,amd-mod-proc-group,amd-mod-rpc-port,amd-mod-screen-resolution,angle,app-core-common,app-core-efl,app-core-multiwindow,app-core-multiwindow-tcore,app-core-tcore,app-core-ui,app-core-unittests,app-defined-loader,app-installers,app-installers-tests,appcore-agent,appcore-agent-unittests,appcore-ui-app-ambient,appcore-watch-unittests,appcore-widget,appcore-widget-base,appcore-widget-base-tcore`；另 460 包.
- TIER2 `tier2_component_0354` — 1 包 — anchors `drm-service-core-tizen` — members `drm-service-core-tizen`.
- TIER2 `tier2_component_0530` — 1 包 — anchors `hal-backend-drm-generic` — members `hal-backend-drm-generic`.
- TIER2 `tier2_component_1086` — 1 包 — anchors `pen-wave` — members `pen-wave`.
- TIER2 `tier2_component_1087` — 1 包 — anchors `pen-wave-devel` — members `pen-wave-devel`.

## Public API header scan

- Scanned **39171 files** under `usr/include/` from **781 devel packages**; read errors **0**.
- Result is **non-empty**: **193291 matching lines in 187 packages**; `std::` occurrences **172485**, STL-header include occurrences **21167**. `public_api_cpp_leak.txt` retains the first 50 lines.
- Highest counts: `boost-devel`=88162, `llvm-devel`=19453, `clang-devel`=13277, `tensorflow2-lite-devel`=10226, `openusd-devel`=7295, `abseil-cpp-devel`=5589, `protobuf-devel`=4029, `lldb-devel`=3312, `libzypp-devel`=3122, `openfst-devel`=3095.

## Final risk judgment

- **Yes: the largest component is a data-supported quasi big-bang risk under either bound.** TIER1 is 412/2005 packages (20.5%); TIER2 is 490/2005 (24.4%). Even after removing `unittests|gtest`, the largest units are 272/1919 (14.2%) and 350/1919 (18.2%). Both largest components are anchored by `hal-api-drm-haltests`.

## Method blind spots

- `dlopen`/plugin edges are absent from DT_NEEDED; `orphans.tsv` is a candidate list requiring configuration/manual confirmation.
- CPP_NOSTL is a binary-side upper-bound proxy for member/layout coupling. It can over-connect ordinary C++ calls and cannot prove hidden STL members; exact classification requires DWARF or authoritative headers.
- The repositories contain zero debug RPMs. Signal b is unusable, DWARF type/layout confirmation is unavailable, and stripped binaries cannot be upgraded beyond symbol/header inference.
- `.comment` coverage is sparse (219/3569 active shared libraries); absence of signal c is not evidence of a modern compiler.
- Variant-family folding is a package-name normalization. Profile/extension implementations may differ at runtime; `variant_families.tsv` records representative selection, including fallbacks where no base provider owns the SONAME.
- Header-only, inline, template-only, and visibility-hidden ABI relationships may leave no dynamic-symbol edge. Conversely, the public-header scan includes third-party C++ development APIs and does not by itself define which headers are platform-stable API policy surface.
- Unresolved symbols remain excluded from graph edges; their counts and rankings bound the missing-edge uncertainty.

## Reproducibility

- `phase2_graph_v5.py` recomputes variant families, edge layers, both component tiers, and unresolved evidence from `elf_analysis.sqlite3` without symbol recollection.
- `phase3_finalize.py` derives `.comment`, anchors, and public-header evidence; `final_validation_v5.tsv` records the terminal consistency checks.
- v1–v4 graph results remain under `v1_superseded/` through `v4_superseded/`.

## Closure addendum

### Anchor/component cross-reference

- `drm-service-core-tizen` — TIER1 `tier1_component_0385` (1 packages; max-412=NO); TIER2 `tier2_component_0354` (1 packages; max-490=NO).
- `hal-api-drm-haltests` — TIER1 `tier1_component_0001` (412 packages; max-412=YES); TIER2 `tier2_component_0001` (490 packages; max-490=YES).
- `hal-backend-drm-generic` — TIER1 `tier1_component_0564` (1 packages; max-412=NO); TIER2 `tier2_component_0530` (1 packages; max-490=NO).
- `pen-wave` — TIER1 `tier1_component_1147` (1 packages; max-412=NO); TIER2 `tier2_component_1086` (1 packages; max-490=NO).
- `pen-wave-devel` — TIER1 `tier1_component_1148` (1 packages; max-412=NO); TIER2 `tier2_component_1087` (1 packages; max-490=NO).

**Answer:** YES. `hal-api-drm-haltests` is in the largest component under both TIER1 (412 packages) and TIER2 (490 packages); the other four anchors are singleton components.

### Public API C++ leak stratification

Classification is exhaustive and mutually exclusive, with A taking precedence. A uses the requested `capi-`/`tizen-` prefix or ownership of `/usr/include/tizen*`; C comprises Base upstream packages plus an explicit Unified upstream/vendor allowlist; the remaining Unified Tizen/platform projects are B. Per-package decisions are recorded in `public_api_cpp_leak_classes.tsv`.

- A — capi-/Tizen public API: **15 packages**, **500 matching lines**.
- B — platform-native C++ devel: **96 packages**, **13063 matching lines**.
- C — third-party/upstream C++ devel: **76 packages**, **179728 matching lines**.

**Class-A conclusion:** NON-EMPTY — **15 packages / 500 matching lines**.

Class A full package list and samples (maximum three per package):

- `capi-appfw-app-common-devel` — 31 hits — `/usr/include/appfw/app_common.hpp:116` [STD_NAMESPACE] `std::string GetLanguage() const {`; `/usr/include/appfw/app_common.hpp:123` [STD_NAMESPACE] `auto p = std::unique_ptr<char, decltype(std::free)*>(lang, std::free);`; `/usr/include/appfw/app_common.hpp:125` [STD_NAMESPACE] `return std::string(lang);`
- `capi-appfw-app-control-devel` — 33 hits — `/usr/include/appfw/app_control.hpp:23` [STL_INCLUDE] `#include <string>`; `/usr/include/appfw/app_control.hpp:24` [STL_INCLUDE] `#include <memory>`; `/usr/include/appfw/app_control.hpp:25` [STL_INCLUDE] `#include <vector>`
- `capi-appfw-widget-application-devel` — 11 hits — `/usr/include/appfw/widget_app.hpp:24` [STL_INCLUDE] `#include <list>`; `/usr/include/appfw/widget_app.hpp:25` [STL_INCLUDE] `#include <memory>`; `/usr/include/appfw/widget_app.hpp:50` [STD_NAMESPACE] `virtual std::unique_ptr<InstanceBase> Create(widget_context_h h) = 0;`
- `capi-media-vision-3d-devel` — 1 hits — `/usr/include/media/mv_3d_private.h:26` [STD_NAMESPACE] `using PointCloudPtr = std::shared_ptr<open3d::geometry::PointCloud>;`
- `capi-media-vision-machine_learning-devel` — 61 hits — `/usr/include/media/IImageSegmentation.h:34` [STD_NAMESPACE] `virtual void setUserModel(std::string model_file, std::string meta_file, std::string label_file) = 0;`; `/usr/include/media/IImageSegmentation.h:35` [STD_NAMESPACE] `virtual void setEngineInfo(std::string engine_type, std::string device_type) = 0;`; `/usr/include/media/IImageSegmentation.h:37` [STD_NAMESPACE] `virtual const std::string &getEngineType(unsigned int engine_index) = 0;`
- `capi-ui-inputmethod-core-devel` — 1 hits — `/usr/include/inputmethod_core_internal.h:208` [STD_NAMESPACE] `typedef void (*inputmethod_candidate_item_layout_set_cb)(std::vector<uint32_t> item, void *user_data);`
- `capi-ui-inputmethod-devel` — 2 hits — `/usr/include/inputmethod_core_internal.h:208` [STD_NAMESPACE] `typedef void (*inputmethod_candidate_item_layout_set_cb)(std::vector<uint32_t> item, void *user_data);`; `/usr/include/inputmethod_internal.h:246` [STD_NAMESPACE] `typedef void (*ime_candidate_item_layout_set_cb)(std::vector<uint32_t> item, void *user_data);`
- `tizen-core-imf-devel` — 42 hits — `/usr/include/tizen-core-imf/tizen_core_imf_internal.h:31` [STL_INCLUDE] `#include <functional>`; `/usr/include/tizen-core-imf/tizen_core_imf_internal.h:32` [STL_INCLUDE] `#include <list>`; `/usr/include/tizen-core-imf/tizen_core_imf_internal.h:33` [STL_INCLUDE] `#include <memory>`
- `tizen-core-util-devel` — 42 hits — `/usr/include/tizen-core-util/tizen_core_thread_callback.hpp:23` [STL_INCLUDE] `#include <functional>`; `/usr/include/tizen-core-util/tizen_core_thread_callback.hpp:24` [STL_INCLUDE] `#include <map>`; `/usr/include/tizen-core-util/tizen_core_thread_callback.hpp:25` [STL_INCLUDE] `#include <memory>`
- `tizen-database-devel` — 176 hits — `/usr/include/tizen-database/database.hpp:22` [STL_INCLUDE] `#include <functional>`; `/usr/include/tizen-database/database.hpp:23` [STL_INCLUDE] `#include <future>`; `/usr/include/tizen-database/database.hpp:24` [STL_INCLUDE] `#include <list>`
- `tizen-dlog-devel` — 49 hits — `/usr/include/tizen-dlog/tizen-dlog.hh:22` [STL_INCLUDE] `#include <format>`; `/usr/include/tizen-dlog/tizen-dlog.hh:23` [STL_INCLUDE] `#include <queue>`; `/usr/include/tizen-dlog/tizen-dlog.hh:24` [STL_INCLUDE] `#include <ranges>`
- `tizen-libopener-devel` — 7 hits — `/usr/include/tizen-libopener/libopener.hpp:22` [STL_INCLUDE] `#include <string_view>`; `/usr/include/tizen-libopener/libopener.hpp:23` [STL_INCLUDE] `#include <filesystem>`; `/usr/include/tizen-libopener/libopener.hpp:29` [STD_NAMESPACE] `LibOpener(std::string_view lib_path, int flag = RTLD_LAZY | RTLD_GLOBAL) {`
- `tizen-shared-queue-devel` — 18 hits — `/usr/include/tizen-shared-queue/shared-queue.hpp:20` [STL_INCLUDE] `#include <condition_variable>`; `/usr/include/tizen-shared-queue/shared-queue.hpp:21` [STL_INCLUDE] `#include <memory>`; `/usr/include/tizen-shared-queue/shared-queue.hpp:22` [STL_INCLUDE] `#include <mutex>`
- `tizen-watcher-devel` — 21 hits — `/usr/include/tizen-watcher/tizen_watcher_proxy.h:11` [STL_INCLUDE] `#include <string>`; `/usr/include/tizen-watcher/tizen_watcher_proxy.h:12` [STL_INCLUDE] `#include <vector>`; `/usr/include/tizen-watcher/tizen_watcher_proxy.h:13` [STL_INCLUDE] `#include <memory>`
- `united-service-devel` — 5 hits — `/usr/include/tizen-base/service.h:327` [STL_INCLUDE] `#include <string>`; `/usr/include/tizen-base/service.h:336` [STD_NAMESPACE] `std::string GetServiceName(void) {`; `/usr/include/tizen-base/service.h:345` [STD_NAMESPACE] `std::string result(name);`

The class-A samples are a targeted read-only replay over the already-extracted headers because the original capped artifact retained only the first 50 global details. Every replayed class-A package count exactly matches `public_api_cpp_leak_by_package.tsv`; no RPM, ELF, symbol, or full header census was recollected.

## Closure addendum v2

### Final anchor verification

Rule: an anchor candidate with recognized `.comment` equal to the platform baseline GCC 14.2.0 is treated as built from source in this snapshot and marked **EXONERATED**; an old or missing `.comment` remains **ANCHOR**. The `_ZNSt*` gate remains unchanged.

- `drm-service-core-tizen` — signal `a:regex_substring=drm`; `.comment` `<MISSING:/usr/lib/libdrm-service-core-tizen.so.0.3.9> || <NOT_COVERED_NON_SHARED:/usr/bin/drm_tizen_initialize>`; assessment `COMMENT_EVIDENCE_MISSING_OR_NOT_COVERED`; final **ANCHOR** — baseline proof incomplete: missing_active_shared=1, non_shared_not_covered=1; cannot establish current-snapshot source build.
- `hal-api-drm-haltests` — signal `a:regex_substring=drm`; `.comment` `<NOT_COVERED_NON_SHARED:/usr/bin/hal/drm-haltests>`; assessment `COMMENT_EVIDENCE_MISSING_OR_NOT_COVERED`; final **ANCHOR** — baseline proof incomplete: missing_active_shared=0, non_shared_not_covered=1; cannot establish current-snapshot source build.
- `hal-backend-drm-generic` — signal `a:regex_substring=drm`; `.comment` `<MISSING:/hal/lib/libhal-backend-drm.so>`; assessment `COMMENT_EVIDENCE_MISSING_OR_NOT_COVERED`; final **ANCHOR** — baseline proof incomplete: missing_active_shared=1, non_shared_not_covered=0; cannot establish current-snapshot source build.
- `pen-wave` — signal `c:compiler_major_below_baseline`; `.comment` `/usr/lib/libpen-wave.so.1.4.0:GCC: (Tizen GCC 9.2.0 20190812 1.44) 9.2.0 || /usr/lib/libpen-wave.so.1.4.0:GCC: (Tizen GCC 9.2.0 20190812 1.45) 9.2.0`; assessment `OLD_GCC_CROSS_MAJOR`; final **ANCHOR** — recognized .comment GCC major<14; old/prebuilt risk retained.
- `pen-wave-devel` — signal `c:compiler_major_below_baseline`; `.comment` `/usr/lib/libpen-wave.so.1.4.0:GCC: (Tizen GCC 9.2.0 20190812 1.44) 9.2.0 || /usr/lib/libpen-wave.so.1.4.0:GCC: (Tizen GCC 9.2.0 20190812 1.45) 9.2.0`; assessment `OLD_GCC_CROSS_MAJOR`; final **ANCHOR** — recognized .comment GCC major<14; old/prebuilt risk retained.

Final disposition: **5 ANCHOR**, **0 EXONERATED**.

### Expanded production-only test exclusion

The test predicate is case-insensitive `unittests|gtest|haltests|-tests?$`. It matches **201 packages**, leaving **1804 production packages**.

- TIER1 production-only: **1537 components**; distribution **1→1521, 2→9, 3→3, 5→2, 7→1, 239→1**; largest **239 packages**.
- TIER2 production-only: **1467 components**; distribution **1→1451, 2→10, 3→3, 5→1, 7→1, 312→1**; largest **312 packages**.

**Final production-anchor conclusion:** TIER1 largest is **NOT ANCHORED** and TIER2 largest is **NOT ANCHORED** after expanded test removal. `hal-api-drm-haltests`, the sole anchor in both unfiltered maximum components, is excluded by the `haltests` term.
