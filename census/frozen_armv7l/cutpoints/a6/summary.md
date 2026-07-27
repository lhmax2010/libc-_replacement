# A6 — CPP_NOSTL layout coupling and static-archive blind spots

## CPP_NOSTL edge scan

- Input edges: **209**.
- Full NEEDED-scoped resolved UND rows: **5030** (`cpp_nostl_resolved_symbols.tsv`).
- Edge/class evidence rows: **1128** (`cpp_nostl_layout_coupling.tsv`).
- Proven hidden layout-coupling edges: **54** total; **37** after the production test regex; **33** have both endpoints in the corrected production TIER2 core of 248 packages.
- Evidence shape: **53** edges have at least one `YES_DIRECT` class; **1** are recursive-only. Class-row statuses are: `NO_CLASS`=122, `NO_OBSERVED`=762, `UNKNOWN`=132, `YES_DIRECT`=95, `YES_RECURSIVE`=17.

Only `YES_DIRECT` and `YES_RECURSIVE` enter the hidden-edge count. `UNKNOWN`, `NO_OBSERVED`, namespace/no-class rows are retained and do not silently become NO. The reusable decision method is documented in `../bundle_audit/a7/impl_members.md`.

### Full hidden-edge list

| consumer | provider | classes containing inline STL | production | in core 248 |
|---|---|---|---:|---:|
| `ambient-viewer` | `libscreen_connector_remote_surface` | `screen_connector::RemoteSurface` | YES | YES |
| `amd` | `parcel` | `tizen_base::Parcel` | YES | YES |
| `angle` | `jsoncpp` | `Json::CharReader;Json::CharReaderBuilder;Json::Value` | YES | YES |
| `app-core-efl` | `app-core-common` | `tizen_cpp::AppCoreBase` | YES | YES |
| `app-core-efl` | `app-core-ui` | `tizen_cpp::AppCoreUiBase` | YES | YES |
| `app-core-efl` | `bundle` | `tizen_base::Bundle` | YES | YES |
| `app-core-multiwindow` | `app-core-common` | `tizen_cpp::AppCoreBase` | YES | YES |
| `app-core-multiwindow` | `bundle` | `tizen_base::Bundle` | YES | YES |
| `app-core-multiwindow-tcore` | `app-core-common` | `tizen_cpp::AppCoreBase` | YES | YES |
| `app-core-multiwindow-tcore` | `bundle` | `tizen_base::Bundle` | YES | YES |
| `app-installers-tests` | `manifest-parser` | `parser::ManifestParser` | NO | NO |
| `askuser-notification-unit-tests` | `boost-test` | `boost::unit_test::decorator::collector_t;boost::unit_test::test_case` | NO | NO |
| `aul` | `parcel` | `tizen_base::Parcel` | YES | YES |
| `boost-log` | `boost-thread` | `boost::detail::thread_data_base` | YES | YES |
| `capi-appfw-alarm` | `bundle` | `tizen_base::Bundle` | YES | YES |
| `capi-appfw-alarm-unittests` | `bundle` | `tizen_base::Bundle` | NO | NO |
| `capi-appfw-application` | `app-core-efl` | `tizen_cpp::AppCoreEflBase` | YES | YES |
| `capi-appfw-application` | `app-core-ui` | `tizen_cpp::AppCoreUiBase` | YES | YES |
| `capi-appfw-application` | `bundle` | `tizen_base::Bundle` | YES | YES |
| `capi-appfw-united-service` | `bundle` | `tizen_base::Bundle` | YES | YES |
| `capi-media-vision-roi_tracker` | `opencv` | `cv::tracking::TrackerCSRT::Params` | YES | YES |
| `chromium-efl` | `manifest-parser` | `parser::ManifestParser` | YES | YES |
| `device-certificate-manager-tests` | `boost-test` | `boost::unit_test::decorator::collector_t;boost::unit_test::test_case` | NO | NO |
| `grpc-devel` | `abseil-cpp` | `absl::lts_20250814::log_internal::LogMessage;absl::lts_20250814::log_internal::LogMessageFatal` | YES | YES |
| `gst-plugins-bad` | `soundtouch` | `soundtouch::BPMDetect` | YES | NO |
| `ise-default-profile_common` | `libscl-ui` | `scl::CSCLUtils` | YES | YES |
| `ise-default-profile_ivi` | `libscl-ui` | `scl::CSCLUtils` | YES | YES |
| `ise-default-profile_mobile` | `libscl-ui` | `scl::CSCLUtils` | YES | YES |
| `ise-default-profile_tv` | `libscl-ui` | `scl::CSCLUtils` | YES | YES |
| `launchpad-unittests` | `parcel` | `tizen_base::Parcel` | NO | NO |
| `libcryptsvc-test` | `boost-test` | `boost::unit_test::decorator::collector_t;boost::unit_test::test_case` | NO | NO |
| `libgiomm` | `libglibmm` | `Glib::SignalProxyDetailed;Glib::Variant<Glib::VariantBase>;Glib::Variant<Glib::ustring>;Glib::Variant<bool>;Glib::Variant<int>;Glib::ustring` | YES | YES |
| `libmedia-service` | `podofo` | `PoDoFo::PdfArray;PoDoFo::PdfContentsTokenizer;PoDoFo::PdfDocument;PoDoFo::PdfFontMetricsBase14;PoDoFo::PdfMemDocument;PoDoFo::PdfName;PoDoFo::PdfString;PoDoFo::PdfTokenizer` | YES | NO |
| `libphonenumber` | `abseil-cpp` | `absl::lts_20250814::log_internal::LogMessage;absl::lts_20250814::log_internal::LogMessageFatal` | YES | YES |
| `libscreen_connector_provider` | `libscreen_connector_input` | `screen_connector::input::KeyEventArgs;screen_connector::input::Stub` | YES | YES |
| `libscreen_connector_provider_tcore` | `libscreen_connector_input` | `screen_connector::input::KeyEventArgs;screen_connector::input::Stub` | YES | YES |
| `libtrackrenderer` | `jsoncpp` | `Json::Value` | YES | YES |
| `libwidget_viewer_evas-unittests` | `libscreen_connector_remote_surface` | `screen_connector::RemoteSurface` | NO | NO |
| `lldb` | `lldb-devel` | `lldb::SBCommandInterpreterRunOptions;lldb::SBCommandInterpreterRunResult;lldb::SBCommandReturnObject;lldb::SBError;lldb::SBFileSpec;lldb::SBStream` | YES | NO |
| `llvm` | `lldb-devel` | `lldb::SBAddress;lldb::SBCommandReturnObject;lldb::SBDeclaration;lldb::SBEnvironment;lldb::SBError;lldb::SBFileSpec;lldb::SBFileSpecList;lldb::SBInstruction;lldb::SBLaunchInfo;lldb::SBLineEntry;lldb::SBMutex;lldb::SBQueue;lldb::SBStream;lldb::SBStringList;lldb::SBSymbolContext;lldb::SBSymbolContextList;lldb::SBValueList;lldb::SBWatchpoint;lldb::SBWatchpointOptions` | YES | NO |
| `nnstreamer-grpc` | `libgrpc` | `grpc::CompletionQueue` | YES | YES |
| `nnstreamer-unittests` | `nnstreamer-core` | `tensor_filter_cpp` | NO | NO |
| `pkgmgr-client` | `parcel` | `tizen_base::Parcel` | YES | YES |
| `pkgmgr-info-unittests` | `parcel` | `tizen_base::Parcel` | NO | NO |
| `rpk-installer-tests` | `manifest-parser` | `parser::ManifestParser` | NO | NO |
| `rpk-installer-unittests` | `manifest-parser` | `parser::ManifestParser` | NO | NO |
| `tpk-installer-tests` | `manifest-parser` | `parser::ManifestParser` | NO | NO |
| `tpk-manifest-handlers-unittests` | `manifest-parser` | `parser::ManifestParser` | NO | NO |
| `watch-holder` | `libscreen_connector_remote_surface` | `screen_connector::RemoteSurface` | YES | YES |
| `wgt-installer` | `tpk-installer` | `tpk::filesystem::StepTpkPreparePackageDirectory;tpk::pkgmgr::StepManifestAdjustment` | YES | YES |
| `wgt-installer-tests` | `manifest-parser` | `parser::ManifestParser` | NO | NO |
| `wgt-installer-tests` | `wgt-manifest-handlers` | `wgt::parse::SettingInfo` | NO | NO |
| `wgt-manifest-handlers-unittests` | `wgt-manifest-handlers` | `wgt::parse::AccountInfo;wgt::parse::AddonInfo;wgt::parse::AppControlInfo;wgt::parse::ApplicationIcon;wgt::parse::ApplicationIconsInfo;wgt::parse::CSPInfo;wgt::parse::ContentInfo;wgt::parse::LaunchScreenInfo;wgt::parse::MetaDataInfo;wgt::parse::ServiceInfo;wgt::parse::SplashScreenInfo;wgt::parse::WarpInfo;wgt::parse::WidgetInfo` | NO | NO |
| `yaca-tests` | `boost-test` | `boost::unit_test::decorator::collector_t;boost::unit_test::test_case` | NO | NO |


The same rows with evidence status/test/core flags are in `cpp_nostl_hidden_edges_annotated.tsv` and `tier1_promotion_candidates.tsv`.

## 125-package TIER2-minus-TIER1 header check

All installed devel-header class/struct tags were enumerated mechanically; same-source binary packages necessarily repeat the same installed header evidence. The scan saw **478228 package/class occurrences**, **75277 unique header definitions**, and found **4372 package/class hit rows / 1239 unique header definitions in 63 of 125 packages** with both an inline STL member (direct or by-value recursive) and at least one public signature not spelling `std::`.

Hit packages (63): `abseil-cpp`, `app-core-efl`, `aurum`, `aurum-bootstrap`, `boost-filesystem`, `boost-locale`, `boost-log`, `boost-program-options`, `boost-thread`, `boost-wave`, `boost_type_erasure`, `capi-machine-learning-training`, `capi-media-vision-3d`, `capi-media-vision-barcode`, `capi-media-vision-common`, `capi-media-vision-face`, `capi-media-vision-image`, `capi-media-vision-machine_learning`, `capi-media-vision-roi_tracker`, `capi-media-vision-surveillance`, `ccapi-machine-learning-training`, `dali2-physics-2d`, `dali2-physics-3d`, `dali2-usd-loader`, `grpc-devel`, `inference-engine-interface-common`, `inference-engine-interface-common-devel`, `ise-engine-sunpinyin`, `libgiomm`, `libglibmm`, `libgrpc`, `libphonenumber`, `libre2`, `libscl-ui`, `libscl-ui-dali`, `libscl-ui-nui`, `libscreen_connector_provider`, `libscreen_connector_provider_tcore`, `libtbb`, `libzypp`, `lottie-player`, `ncnn`, `ncnn-examples`, `ncnn-tools`, `nnstreamer-grpc`, `nnstreamer-grpc-flatbuf`, `nnstreamer-grpc-protobuf`, `nnstreamer-nntrainer`, `nnstreamer-nntrainer-trainer`, `nnstreamer-openvino`, `nnstreamer-protobuf`, `nntrainer-applications`, `nntrainer-core`, `opencv`, `openusd`, `openvino`, `protobuf`, `protobuf-compiler`, `protobuf-lite`, `python3-opencv`, `singleo`, `tizen-core-imf`, `tizen-dlog`.

Every package/class/header/line is in `tier2_diff_layout_classes.tsv`; the condensed all-class-per-package form is `tier2_diff_layout_hits_by_package.tsv`.

## Static archive blind spot

- Devel-package `.a` paths examined: **2213**; archive members listed: **40244**.
- Archives containing `_ZNSt` and/or demangled `std::`: **319**.
- Same-source strong-symbol fingerprints in corrected core-wave ELF exports: **8** archives.
- Cross-source fingerprints (possible, not proof): **5** archives.
- Core source-family archives with no strong fingerprint: **19**.
- Unscannable/error paths: **555**. Breakdown: `arm-rpi4-rt-linux-kernel-devel`=277, `arm-rpi4-linux-kernel-devel`=276, `app-rootstrap-devel`=1, `glibc-devel`=1. The 553 kernel rows are thin archives whose referenced member objects are absent from the binary RPM; the remaining two are non-archive/linker-script-like files.

### Core-wave confirmed STL archives

| owner | archive | strong fingerprint count | matching core packages |
|---|---|---:|---|
| `capi-machine-learning-training-devel-static` | `abi_census/extracted/capi-machine-learning-training-devel-static/usr/lib/libcapi-nntrainer.a` | 42 | `capi-machine-learning-training` |
| `ccapi-machine-learning-training-devel-static` | `abi_census/extracted/ccapi-machine-learning-training-devel-static/usr/lib/libccapi-nntrainer.a` | 12 | `capi-machine-learning-training,ccapi-machine-learning-training,nnstreamer-nntrainer,nnstreamer-nntrainer-trainer` |
| `nnstreamer-devel-static` | `abi_census/extracted/nnstreamer-devel-static/usr/lib/libnnstreamer_filter_openvino.a` | 18 | `nnstreamer-openvino` |
| `nnstreamer-nntrainer-devel-static` | `abi_census/extracted/nnstreamer-nntrainer-devel-static/usr/lib/libnnstreamer_filter_nntrainer.a` | 7 | `nnstreamer-nntrainer` |
| `nnstreamer-nntrainer-trainer-devel-static` | `abi_census/extracted/nnstreamer-nntrainer-trainer-devel-static/usr/lib/libnnstreamer_trainer_nntrainer.a` | 22 | `nnstreamer-nntrainer-trainer` |
| `nntrainer-devel-static` | `abi_census/extracted/nntrainer-devel-static/usr/lib/libnntrainer.a` | 2094 | `nntrainer-applications,nntrainer-core` |
| `openvino-devel` | `abi_census/extracted/openvino-devel/usr/lib/libinference_engine_s.a` | 1256 | `openvino` |
| `pcre-devel-static` | `abi_census/extracted/pcre-devel-static/usr/lib/libpcrecpp.a` | 84 | `libpcrecpp` |


`libnntrainer.a` is present in this confirmed set: it has 119 archive members, 20,356 raw `_ZNSt` lines, 31,214 demangled `std::` lines, and 2,094 same-source strong export fingerprints into `nntrainer-applications`/`nntrainer-core`.

The complete 2,213-row table is `static_archives_stl.tsv`; STL-only, confirmed-core, and possible-cross-source subsets are separate TSVs. `static_archive_members.tsv` records every `ar t` member; all matching `nm` lines are preserved in `static_archive_stl_symbol_lines.tsv.gz`.

## Data-side promotion / shim candidates

1. The **54 proven hidden edges** are candidates to promote from CPP_NOSTL into TIER1; the production/core-priority subset is the **33** rows marked `production_edge=YES` and `both_endpoints_in_tier2_core_248=YES`.
2. **17** in-core production CPP_NOSTL edges have unresolved provider-class layouts and no proven YES class on the same edge; they are conservative forced-shim/manual-DWARF review candidates (`shim_review_candidates.tsv`), not automatic promotions.
3. The **8 confirmed core static archives** require same-standard-library rebuild discipline or a C/shim boundary because their coupling is invisible to the dynamic graph. The 5 cross-source fingerprints remain manual link-map/build-log checks.
4. The 63 hit packages in the 125-package difference have a demonstrable installed-header layout surface. Package presence alone does not identify a particular binary edge, so those classes are review/shim evidence, not automatic edge promotion.

These are evidence-based candidate sets; wave policy and shim choice remain HLD decisions.

## Method boundaries

- Snapshot DWARF is generally absent. Direct header members and recursively embedded by-value/base classes count; pointers/references are not followed. Signatures alone do not prove members.
- `NO_OBSERVED` is not source-level proof of absence; opaque PIMPLs remain conservative.
- Ctags supplies declaration scope/end boundaries; macro-delimited namespaces with ambiguous leaf names are left UNKNOWN rather than guessed.
- Static strong-symbol fingerprints show binary correspondence, not the original linker command. Definitive cross-source archive use requires build logs/link maps.
- Thin archives with missing external members cannot be exhaustively scanned from the binary RPM alone and are explicitly counted above.
