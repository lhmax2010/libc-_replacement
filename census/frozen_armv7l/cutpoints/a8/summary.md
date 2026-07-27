# A8 — 隐藏布局耦合边升格后的生产 TIER1

## 口径与机械校验

- 基线为 A4 修正生产 TIER1：`INTERFACE + EXPLICIT_INSTANTIATION_EDGE`，测试包正则已经剔除；BOTH VAGUE 的剔除仍以既有 D2 条款为前提。
- 从 A6 `tier1_promotion_candidates.tsv` 精确选入 `production_edge=YES`、`both_endpoints_in_tier2_core_248=YES` 且证据为 `YES_DIRECT/YES_RECURSIVE` 的 **33** 条边。
- 17 条布局证据不足边与 8 个静态归档均不加入 Union-Find；因此不会机械改变分量数。

## 修正后的核心波终值

- 旧核心：**123** 包；升格后核心：**130** 包；差值：**+7**。
- 生产分量总数：**1582 → 1570**。
- 新分布：**1→1544, 2→10, 3→6, 4→3, 5→2, 6→1, 7→1, 20→1, 37→1, 130→1**。
- 新核心成员前 20：`ambient-viewer, amd, amd-mod-component-manager, amd-mod-cynara-core, amd-mod-input, amd-mod-light-user, amd-mod-proc-group, amd-mod-rpc-port, amd-mod-screen-resolution, angle, app-core-common, app-core-efl, app-core-multiwindow, app-core-multiwindow-tcore, app-core-tcore, app-core-ui, app-defined-loader, app-installers, appcore-agent, appcore-ui-app-ambient`。
- 全量新核心成员见 `components_production_tier1_promoted.tsv`；新增成员见 `core_new_members.tsv`。

### 并入旧 123 核心的原独立分量

| 旧分量 | 原规模 | 原成员 | 升格桥边 |
|---|---:|---|---|
| `interface_only_0059` | 1 | `angle` | `angle→jsoncpp` |
| `interface_only_0061` | 1 | `app-core-efl` | `app-core-efl→app-core-common;app-core-efl→app-core-ui;app-core-efl→bundle;capi-appfw-application→app-core-efl` |
| `interface_only_0167` | 1 | `capi-appfw-alarm` | `capi-appfw-alarm→bundle` |
| `interface_only_0174` | 1 | `capi-appfw-united-service` | `capi-appfw-united-service→bundle` |
| `interface_only_0902` | 1 | `libscreen_connector_provider` | `libscreen_connector_provider→libscreen_connector_input` |
| `interface_only_0903` | 1 | `libscreen_connector_provider_tcore` | `libscreen_connector_provider_tcore→libscreen_connector_input` |
| `interface_only_0949` | 1 | `libtrackrenderer` | `libtrackrenderer→jsoncpp` |


共 **7** 个原独立分量、**7** 个包并入核心；其余升格边或为旧分量内部边，或只合并核心之外的独立域。所有非核心合并关系见 `component_merge_lineage.tsv`。

## 图外耦合的同批约束

这两组记录进入波次同批约束清单，但不作为连通分量边：布局未决记录尚未达到 `YES_DIRECT/YES_RECURSIVE` 证据门槛；静态归档指纹证明同标准库重建纪律需求，却不能证明一个运行时 DSO 依赖方向。

### 17 条布局证据不足的核心-248 CPP_NOSTL 边

| consumer | provider | 未决类 | A8 关系 |
|---|---|---|---|
| `boost-locale` | `libicu` | `icu::BreakIterator;icu::Formattable` | a8_tier1_0139 ↔ a8_tier1_0001 |
| `capi-base-utils` | `libicu` | `icu::Formattable` | a8_tier1_0172 ↔ a8_tier1_0001 |
| `capi-media-vision-3d` | `capi-media-vision-common` | `MediaVision::Common::MediaSource` | a8_tier1_0018 ↔ a8_tier1_0002 |
| `capi-media-vision-image` | `capi-media-vision-common` | `MediaVision::Common` | 同一 A8 分量 |
| `capi-media-vision-surveillance` | `capi-media-vision-common` | `MediaVision::Common` | 同一 A8 分量 |
| `com.samsung.dali-demo` | `dali2-physics-3d` | `btBoxShape;btCollisionObject;btConvexInternalShape;btPoint2PointConstraint;btSphereShape` | a8_tier1_0001 ↔ a8_tier1_0342 |
| `cordova-api` | `libicu` | `icu::Formattable` | 同一 A8 分量 |
| `dali2-csharp-binder-physics3d` | `dali2-physics-3d` | `btBU_Simplex1to4;btBox2dShape;btBoxShape;btBvhTriangleMeshShape;btCapsuleShape;btCollisionObject;btCollisionShape;btCompoundShape;btConcaveShape;btConeShape;btConeTwistConstraint;btConvex2dShape;btConvexConcaveCollisionAlgorithm;btConvexHullShape;btConvexInternalShape;btConvexPointCloudShape;btConvexPolyhedron;btConvexShape;btConvexTriangleCallback;btConvexTriangleMeshShape;btCylinderShape;btDeformableMultiBodyConstraintSolver;btDiscreteDynamicsWorld;btDiscreteDynamicsWorldMt;btEmptyShape;btFixedConstraint;btGeneric6DofConstraint;btGeneric6DofSpring2Constraint;btGeneric6DofSpringConstraint;btGhostObject;btHashedOverlappingPairCache;btHeightfieldTerrainShape;btHinge2Constraint;btHingeAccumulatedAngleConstraint;btHingeConstraint;btMinkowskiSumShape;btMultiBody;btMultiBodyConstraint;btMultiBodyConstraintSolver;btMultiBodyPoint2Point;btMultiSphereShape;btMultimaterialTriangleMeshShape;btNNCGConstraintSolver;btOptimizedBvh;btPersistentManifold;btPoint2PointConstraint;btPolyhedralConvexShape;btQuantizedBvh;btScaledBvhTriangleMeshShape;btSequentialImpulseConstraintSolver;btSequentialImpulseConstraintSolverMt;btShapeHull;btSliderConstraint;btSortedOverlappingPairCache;btSphereShape;btStaticPlaneShape;btStridingMeshInterface;btTriangleIndexVertexArray;btTriangleIndexVertexMaterialArray;btTriangleMeshShape;btTypedConstraint;btUniformScalingShape;btUniversalConstraint;btVoronoiSimplexSolver` | a8_tier1_0334 ↔ a8_tier1_0342 |
| `dali2-extension-icu-plugin` | `libicu` | `icu::BreakIterator` | a8_tier1_0337 ↔ a8_tier1_0001 |
| `libgiomm` | `libsigc++` | `sigc::connection;sigc::internal::slot_rep;sigc::slot_base;sigc::trackable` | a8_tier1_0021 ↔ a8_tier1_0898 |
| `libglibmm` | `libsigc++` | `sigc::connection;sigc::internal::signal_impl;sigc::internal::slot_rep;sigc::signal_base;sigc::slot_base;sigc::trackable` | a8_tier1_0021 ↔ a8_tier1_0898 |
| `libzypp` | `libsigc++` | `sigc::connection;sigc::internal::signal_impl;sigc::internal::slot_rep;sigc::signal_base;sigc::slot_base;sigc::trackable` | a8_tier1_0016 ↔ a8_tier1_0898 |
| `nnstreamer-grpc-flatbuf` | `nnstreamer-grpc` | `grpc::NNStreamerRPC` | 同一 A8 分量 |
| `nnstreamer-grpc-protobuf` | `nnstreamer-grpc` | `grpc::NNStreamerRPC` | 同一 A8 分量 |
| `nnstreamer-grpc-protobuf` | `nnstreamer-protobuf` | `nnstreamer::protobuf::Tensor;nnstreamer::protobuf::Tensors;nnstreamer::protobuf::Tensors_frame_rate` | 同一 A8 分量 |
| `pkgmgr-client` | `pkgmgr` | `pkgmgr::common::PkgmgrSignal` | a8_tier1_0001 ↔ a8_tier1_1181 |
| `pkgmgr-installer` | `pkgmgr` | `pkgmgr::common::PkgmgrSignal` | a8_tier1_0001 ↔ a8_tier1_1181 |


全量机器可读字段及核心标志见 `layout_unresolved_same_wave_constraints.tsv`。

### 8 个核心-248 同源 STL 静态归档

| 归档所属包 | 归档 | 受影响包 | 新核心内受影响包 |
|---|---|---|---|
| `capi-machine-learning-training-devel-static` | `abi_census/extracted/capi-machine-learning-training-devel-static/usr/lib/libcapi-nntrainer.a` | `capi-machine-learning-training` | `无` |
| `ccapi-machine-learning-training-devel-static` | `abi_census/extracted/ccapi-machine-learning-training-devel-static/usr/lib/libccapi-nntrainer.a` | `capi-machine-learning-training,ccapi-machine-learning-training,nnstreamer-nntrainer,nnstreamer-nntrainer-trainer` | `无` |
| `nnstreamer-devel-static` | `abi_census/extracted/nnstreamer-devel-static/usr/lib/libnnstreamer_filter_openvino.a` | `nnstreamer-openvino` | `无` |
| `nnstreamer-nntrainer-devel-static` | `abi_census/extracted/nnstreamer-nntrainer-devel-static/usr/lib/libnnstreamer_filter_nntrainer.a` | `nnstreamer-nntrainer` | `无` |
| `nnstreamer-nntrainer-trainer-devel-static` | `abi_census/extracted/nnstreamer-nntrainer-trainer-devel-static/usr/lib/libnnstreamer_trainer_nntrainer.a` | `nnstreamer-nntrainer-trainer` | `无` |
| `nntrainer-devel-static` | `abi_census/extracted/nntrainer-devel-static/usr/lib/libnntrainer.a` | `nntrainer-applications,nntrainer-core` | `无` |
| `openvino-devel` | `abi_census/extracted/openvino-devel/usr/lib/libinference_engine_s.a` | `openvino` | `无` |
| `pcre-devel-static` | `abi_census/extracted/pcre-devel-static/usr/lib/libpcrecpp.a` | `libpcrecpp` | `libpcrecpp` |


这 8 个归档影响 **9** 个去重后的核心-248 包；其中落在修正后 130 包核心内的是：`libpcrecpp`。完整分量映射见 `static_archive_same_wave_constraints.tsv`。

## mm 族专项

- `libgiomm→libglibmm` 的 `Glib::ustring`/相关 `YES_DIRECT` 隐藏边升格后，二者组成独立 **2 包**分量 `a8_tier1_0021`：`libgiomm,libglibmm`；**未并入 130 包核心**。
- `libsigc++` 仍为独立单包分量 `a8_tier1_0898`。`libgiomm→libsigc++` 与 `libglibmm→libsigc++` 均属于上述布局未决强制-shim候选，按本轮证据门槛不能升格。因此 mm 三包未形成同一图分量。

## HLD 冻结读数

P0 生产核心波的修正 TIER1 最终数字为 **130 包**。另将 17 条布局未决边涉及包与 8 个静态归档影响包纳入“同批约束”审查，但不得据此改写图分量数量；机器可读并集见 `same_wave_constraint_packages.tsv`。
