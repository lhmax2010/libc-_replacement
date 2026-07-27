# Prompt A final corrected summary (A4)

## Corrected semantics

- HUB_ONLY is relabeled `EXPLICIT_INSTANTIATION_EDGE`: a legal ABI interface edge retained in both tiers.
- The two `nntrainer-applications→nntrainer-core` rows originate from test binaries inside a production package; package-level test filtering therefore does not remove their node or edge.
- Corrected TIER1 = INTERFACE + EXPLICIT_INSTANTIATION_EDGE. Corrected TIER2 = corrected TIER1 + CPP_NOSTL.

## Corrected component panorama

- TIER1: **1582 components**; distribution **1→1557, 2→10, 3→5, 4→3, 5→2, 6→1, 7→1, 19→1, 35→1, 123→1**; largest **123** (`interface_only_0001`).
- TIER2: **1500 components**; distribution **1→1478, 2→12, 3→4, 5→2, 6→1, 7→1, 19→1, 248→1**; largest **248** (`corrected_tier2_0001`).
- TIER2 largest minus TIER1 123-member core: **125 packages**; TIER1-only difference **0**. Added-domain counts: OTHER=53, ML_MEDIA_VISION=26, UI_GRAPHICS_WINDOW=22, SYSTEM_HAL_IPC=18, APP_FRAMEWORK_PACKAGE=13, WEB_ENGINE=5, SECURITY_AUTH=3, CONNECTIVITY=2.
- Added members: abseil-cpp,angle,app-core-efl,aurum,aurum-bootstrap,boost-filesystem,boost-locale,boost-log,boost-program-options,boost-thread,boost-wave,boost_type_erasure,boot-animation-dali,calendar-service,calendar-service-profile_tv,capi-appfw-alarm,capi-appfw-capmgr,capi-appfw-united-service,capi-base-utils,capi-machine-learning-training,capi-media-vision-3d,capi-media-vision-barcode,capi-media-vision-common,capi-media-vision-face,capi-media-vision-image,capi-media-vision-machine_learning,capi-media-vision-roi_tracker,capi-media-vision-surveillance,capi-system-media-key,capi-system-media-key-tcore,capi-ui-inputmethod,ccapi-machine-learning-training,context-app-history-dummy,context-job-scheduler-dummy,context-sensor-recorder-dummy,context-service,dali2-csharp-binder-physics2d,dali2-csharp-binder-physics3d,dali2-csharp-binder-toolkitdemo,dali2-extension-camera-player-plugin,dali2-extension-icu-plugin,dali2-extension-vector-animation-renderer-plugin,dali2-extension-video-player-plugin,dali2-extension-web-engine-chromium-plugin,dali2-extension-web-engine-lwe-plugin,dali2-physics-2d,dali2-physics-3d,dali2-usd-loader,device-certificate-manager,dfs-adaptation; +75 (full TSV).

Domain-aligned nontrivial components (all memberships and domain counts are in `domain_aligned_components.tsv`):

- TIER1 `interface_only_0001` — **123** packages — `OTHER,APP_FRAMEWORK_PACKAGE,UI_GRAPHICS_WINDOW` (name inference only) — ambient-viewer,amd,amd-mod-component-manager,amd-mod-cynara-core,amd-mod-input,amd-mod-light-user,amd-mod-proc-group,amd-mod-rpc-port,amd-mod-screen-resolution,app-core-common,app-core-multiwindow,app-core-multiwindow-tcore,app-core-tcore,app-core-ui,app-defined-loader,app-installers,appcore-agent,appcore-ui-app-ambient,appcore-widget,appcore-widget-base; +103 (full TSV).
- TIER1 `interface_only_0002` — **35** packages — `OTHER,ML_MEDIA_VISION,SYSTEM_HAL_IPC` (name inference only) — abseil-cpp,aurum,aurum-bootstrap,capi-media-vision-barcode,capi-media-vision-common,capi-media-vision-face,capi-media-vision-image,capi-media-vision-machine_learning,capi-media-vision-surveillance,device-certificate-manager,dfs-opencv,grpc-devel,inference-engine-interface-common,inference-engine-interface-common-devel,inference-engine-opencv,libgrpc,libphonenumber,libre2,mmi-manager,ncnn; +15 (full TSV).
- TIER1 `interface_only_0003` — **19** packages — `OTHER,APP_FRAMEWORK_PACKAGE,UI_GRAPHICS_WINDOW` (name inference only) — capi-ui-inputmethod,ise-default-dali,ise-default-profile_common,ise-default-profile_ivi,ise-default-profile_mobile,ise-default-profile_tv,ise-engine-anthy,ise-engine-default,ise-engine-glm,ise-engine-hangul,ise-engine-loader,ise-engine-sunpinyin,ise-engine-tables,ise-engine-thai,ise-engine-unikey,isf,libscl-core,libscl-ui-nui,tizen-core-imf.
- TIER1 `interface_only_0004` — **7** packages — `SECURITY_AUTH,OTHER` (name inference only) — cyad,cynara,libcynara-admin,libcynara-agent,libcynara-client,libcynara-commons,libcynara-monitor.
- TIER1 `interface_only_0005` — **6** packages — `ML_MEDIA_VISION,OTHER,APP_FRAMEWORK_PACKAGE` (name inference only) — capi-machine-learning-training,ccapi-machine-learning-training,nnstreamer-nntrainer,nnstreamer-nntrainer-trainer,nntrainer-applications,nntrainer-core.
- TIER1 `interface_only_0006` — **5** packages — `OTHER,SECURITY_AUTH` (name inference only) — klay,libode,ode,trust-anchor,trust-anchor-runtime.
- TIER1 `interface_only_0007` — **5** packages — `SECURITY_AUTH,UI_GRAPHICS_WINDOW` (name inference only) — libcynara-creds-commons,libcynara-creds-pid,libcynara-creds-self,libcynara-creds-socket,libcynara-uid-creds.
- TIER2 `corrected_tier2_0001` — **248** packages — `OTHER,APP_FRAMEWORK_PACKAGE,UI_GRAPHICS_WINDOW` (name inference only) — abseil-cpp,ambient-viewer,amd,amd-mod-component-manager,amd-mod-cynara-core,amd-mod-input,amd-mod-light-user,amd-mod-proc-group,amd-mod-rpc-port,amd-mod-screen-resolution,angle,app-core-common,app-core-efl,app-core-multiwindow,app-core-multiwindow-tcore,app-core-tcore,app-core-ui,app-defined-loader,app-installers,appcore-agent; +228 (full TSV).
- TIER2 `corrected_tier2_0002` — **19** packages — `SECURITY_AUTH,OTHER,UI_GRAPHICS_WINDOW` (name inference only) — askuser-notification,capi-privacy-privilege-manager,cyad,cynara,libaskuser-notification-common,libcynara-admin,libcynara-agent,libcynara-client,libcynara-commons,libcynara-creds-commons,libcynara-creds-dbus,libcynara-creds-gdbus,libcynara-creds-pid,libcynara-creds-sd-bus,libcynara-creds-self,libcynara-creds-socket,libcynara-monitor,libcynara-uid-creds,security-license-manager.
- TIER2 `corrected_tier2_0003` — **7** packages — `ML_MEDIA_VISION,OTHER,SYSTEM_HAL_IPC` (name inference only) — nnstreamer-lua,nnstreamer-onnxruntime,nnstreamer-python3,nnstreamer-single,nnstreamer-tizen-hal,nnstreamer-tvm,tvm-runtime.
- TIER2 `corrected_tier2_0004` — **6** packages — `CONNECTIVITY,SYSTEM_HAL_IPC,OTHER` (name inference only) — capi-network-smartcard,smartcard-plugin-nfc,smartcard-plugin-uicc,smartcard-plugin-usb,smartcard-service,smartcard-service-common.
- TIER2 `corrected_tier2_0005` — **5** packages — `SECURITY_AUTH` (name inference only) — key-manager,key-manager-misc,key-manager-pam-plugin,libkey-manager-client,libkey-manager-common.
- TIER2 `corrected_tier2_0006` — **5** packages — `OTHER,SECURITY_AUTH` (name inference only) — klay,libode,ode,trust-anchor,trust-anchor-runtime.

## Corrected TIER1 core indegree top 10

| rank | package | indegree | INTERFACE | EXPLICIT_INSTANTIATION |
|---:|---|---:|---:|---:|
| 1 | `bundle` | 44 | 44 | 0 |
| 2 | `dali2` | 13 | 13 | 0 |
| 3 | `jsoncpp` | 12 | 12 | 0 |
| 4 | `dali2-adaptor` | 10 | 10 | 0 |
| 5 | `app-core-common` | 9 | 9 | 0 |
| 6 | `parcel` | 9 | 9 | 0 |
| 7 | `app-installers` | 7 | 7 | 0 |
| 8 | `manifest-parser` | 7 | 7 | 0 |
| 9 | `dali2-toolkit` | 6 | 6 | 0 |
| 10 | `libpcrecpp` | 6 | 6 | 0 |

Beyond `bundle`, the measured core hubs are: `dali2`=13, `jsoncpp`=12, `dali2-adaptor`=10, `app-core-common`=9, `parcel`=9, `app-installers`=7, `manifest-parser`=7, `dali2-toolkit`=6, `libpcrecpp`=6.

## Three requested simulations

Baseline: TIER1 **123**, TIER2 **248**. Bundle cutting removes every corrected C++ edge incident to `bundle`; chromium simulation removes the node and all incident edges.

| scenario | TIER1 largest | change | TIER2 largest | change |
|---|---:|---:|---:|---:|
| `cut_bundle` | 43 | -80 | 193 | -55 |
| `remove_chromium_efl` | 121 | -2 | 247 | -1 |
| `cut_bundle_plus_remove_chromium_efl` | 35 | -88 | 192 | -56 |

## Methodological condition: D2

**The removal of BOTH VAGUE edges is conditional on D2.** A3/A4 sizes are valid only if D2 establishes that loader lookup/order permits those symbols to bind to `libstdc++.so.6` as assumed. If D2 is not satisfied, BOTH edges must be restored and these corrected component sizes must not be used as unconditional migration bounds.

## Evidence index

- `components_production_corrected_tier1.tsv`, `components_production_corrected_tier2.tsv`.
- `tier2_core_minus_tier1_core.tsv`, `domain_aligned_components.tsv`.
- `corrected_core_indegree_top10.tsv`, `a4_cut_simulation.tsv`.
- `explicit_instantiation_edges.tsv` for the corrected legal edge labels and binary roles.
