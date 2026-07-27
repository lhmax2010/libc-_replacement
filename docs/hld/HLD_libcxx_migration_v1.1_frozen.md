# Tizen 平台级 libc++ 迁移 HLD — v1.1(冻结)

> 状态:**已冻结**。三轮异构评审全部闭合——第一轮 5 FAIL、第二轮 7 FAIL、
> 第三轮 3 PASS + R1(上界口径),R1 经 A13 分量传递闭包实算修正后由三方
> 复核确认闭合。核心波区间 **[130, ≤513]**(UB_direct 402 / UB_closure
> 513,四方独立重算同数;封顶范围脚注见 §2)。冻结基线数据锚定
> abi_census/ 与 abi_poc/native_rerun/。
> 修改任何决策需附新证据并走变更记录(§13)。

---

## 1. 背景与目标

- 驱动:总部问询 Chromium 不再支持 libstdc++ 的应对 + GCC→LLVM 迁移的 ABI 验证。
- 交付:2027-10,平台默认 C++ 标准库 libstdc++ → libc++。
- 范围:标准库与 abi 库(libc++ + libc++abi);unwinder 保持 libgcc_s;
  compiler-rt 不在范围(D3)。
- 三线关系:线 1(本文档)/ 线 2(Chromium bundled libc++,先锋项目)/
  线 3(clang 默认化,本项目前置,D7)。接口见 §11。

## 2. 现状与证据基线(P0 census 定稿)

- 方法版本:v5(修正链 v1→v5 全程留档,superseded 目录可复现)。
  校验:15/15 + addendum 4/4 PASS。
- 数据源:gbs.conf 不可变快照,armv7l,3707 RPM;有效 census
  6727 ARM ELF / 2005 平台节点(剔除 x86 基建、SYSROOT_COPY、STDLIB 集)。
- 边分层:STDLIB 4508 / CPP_ABI 890 / CPP_NOSTL 209 / 纯 C 7980。
- 迁移单元(双口径):
  - TIER1(确证,mangled 签名含 std::):最大分量 412/2005;去 test 272。
  - TIER2(+成员级耦合上界,CPP_NOSTL):最大分量 490/2005;去 test 350。
  - 生产口径(测试正则 unittests|gtest|haltests|-tests?$,剔除 201 测试包,
    余 1804 生产包):TIER1 最大 239,TIER2 最大 312。
  - 方法学发现:测试包曾充当桥接节点虚增生产耦合(去 test 由 272→239 /
    350→312,降幅超成员数)——生产域真实耦合低于中间各版估计。
- **归因修正(A3,7/7 PASS)**:生产 VAGUE 边符号 886 条中 882 条为 BOTH
  (消费者 UND 同时可由 libstdc++.so.6 满足;readelf 抽样 10/10 直接
  NEEDED libstdc++)——判归因 artifact 剔除;HUB_ONLY 仅 4 条(2 边,
  项目自有模板显式实例化接口,合法边保留,非 bug)。isf 型缺失-inline
  存量 bug 在全平台 VAGUE 边中零命中。
- **修正后生产核心波:TIER1 ≥130 包(下界),真实值区间 [130, ≤513]**
  (A9 量化 + A13 闭包实算;四方独立重算同数)。**上界语义(评审纠错)**:
  此前 ≤330 = 130 + 200 端点包数,属**一跳口径错误**已作废——升格是
  分量级并查(一条边坐实则其端点所在 TIER1 分量整体并入,不可拆),
  最坏面取 unknown 边传递闭包。两档权威读数(A13):
  **UB_direct = 402**(130 + 189 外部分量/272 包,跨界边端点分量整体
  并入)、**UB_closure = 513**(130 + 289 外部分量/383 包,全 692 unknown
  边完整闭包,最远 5 跳)。冻结承诺用 UB_closure=513 封顶,402 为直接
  闭包读数。下界 130 扎实(A9:762 NO_OBSERVED 全量复扫 + 7980 C 边 +
  严格证据门);不确定面 692 unknown 边;隧道 API 2479 条人工登记册 +
  同波约束;区间随逐波登记册收敛。**精确值超动态符号图能力,需
  CodeGraph 级源码分析。** 分量域对齐:核心 ≥130、vision/ML+OpenCV 44、
  输入法 20、mm 2、cynara 7+5 等。核心波是关键路径。
  **封顶范围脚注(第三轮复核披露)**:513 是"692 条 unknown 边全部坐实
  + TIER1 分量不可拆"声明范围内的严格封顶(闭包外逃边 0)。范围外另有
  两条受纪律控制的残留路径,均已在案、不构成算法错误:
  (a)A8 波次同批约束台账 32 包中 6 包落在 513 闭包外(capi-media-
  vision-3d / dali2-extension-icu-plugin / libgiomm / libglibmm /
  libsigc++ / libzypp)——若其布局未决边人工复核判"同波"而非"加 shim",
  **执行波批次**最坏为 513+6(同批约束不进分量,只约束批次划分);
  (b)17 条 shim 候选中 12 条不在 692 集内,若 shim 不可行走 fallback
  升格,理论可再及核心 +3(严格极值 516,幅度 0.6%)。
  两条均由"shim 优先"处置纪律控制,并纳入 §9 复审对象。
- **方法论条款(D2 前提,板上实证后修订表述)**:BOTH 边**当下在运行时
  真实存在**(RPI4 实测 6/6 样本 vague 符号绑定到枢纽库,BFS 序使然),
  剔除依据不是"边不存在"而是"迁移下 ABI 安全自愈":枢纽迁 libc++ 后
  停止导出 _ZNSt*,消费者回落绑定 libstdc++ 同一实例化(ODR)。自愈两
  要件已板上实证:libstdc++.so.6 导出被测符号(REFUTES=0)+ 消费者直接
  NEEDED libstdc++(10/10)。前提仍是 D2(libstdc++ 永驻);证据效力:
  对 Toolchain 线镜像为直接证据,对 census 快照为机理级支持(上游
  14.2.0/2.40 一致,RPM release 漂移)。证据:onboard_verification/
  (raw log 71+144 个,含绑定行原文)。
- 结论:准 big-bang 风险由"必须切割"降级为"按域波次可排期";核心波
  (下界 130,封顶 513)仍为关键路径,切割为优化项而非必需项(§5)。
- 锚点终局(addendum v2,5/5 PASS):5 ANCHOR / 0 EXONERATED。
  pen-wave/pen-wave-devel 为实证 blob(.comment GCC 9.2.0);drm 三包
  .comment 缺失,保守 ANCHOR。**生产口径下 TIER1/TIER2 最大分量均无锚**;
  5 锚点在生产域全为单点分量或测试域包。
- 公开 API 泄漏:a 类 15 包 / 500 条(D6 台账,§6);b 类 96 包(原生 C++
  devel,by design);c 类 76 包(第三方,预期)。
- **独立抽验(SPOTCHECK v2,PASS)**:绕开 census 流水线,readelf/c++filt
  直怼原始二进制复核:三条预注册边四要素全成立(bundle INTERFACE 边 /
  isf CPP_ABI 边(scim 层)/ zypper→libzypp 含版本化符号 @ZYPP_plain
  匹配)+ pen-wave 6/6 路径 .comment GCC 9.2.0 与 _ZNSt UND 实证
  (blob 为 libstdc++ 消费者,D2 语义精确成立)。v1 曾因抽样规则规格
  缺陷(评审方错误,A2 缺 edge_type 约束)FAIL,fail-stop 正确触发,
  修正后重跑通过;全程留档 independent_spotcheck/(v1+v2)。
  P0 证据结构三层收口:Codex 采集 + Codex 板上动态验证 + 预注册规则
  独立抽验。
- 证据索引:/home/toolchain/plan_evaluation/abi_census/(§14)。

## 3. 架构决策记录(D1–D7)

每条固定格式:决策 / 理由 / 证据锚 / 状态。

- **D1** 迁移单元 = CPP_ABI 传播图连通分量。
  证据:components_tier1/tier2.tsv。补充决策槽位:
  ✅ **D1a 治理口径(A4 后定案建议,P1 冻结确认)**:TIER1(INTERFACE +
  EXPLICIT_INSTANTIATION)定义迁移单元;CPP_NOSTL 边不定义单元,降级为
  波次边界人工核查清单(每波启动前逐条审查跨界 CPP_NOSTL 边,确认无
  成员级 STL 传递或加 shim)。依据:A4 模拟中 TIER2 巨块(248)在任何
  切割下均 ≥192,按 TIER2 编排波次不可操作;125 条差集包台账已落盘
  (tier2_core_minus_tier1_core.tsv)。
- **D2** 双运行时长期共存,libstdc++ 降级为兼容运行时永不删除。
  锚点分量不可迁。证据:anchors.tsv + closure_anchor_disposition_v2.tsv。
  豁免清单终版:5 包(pen-wave 系实证 blob;drm 系保守判定),生产域全为
  单点分量,不约束波次;新增 blob 准入规则(§12)承接后续增量。
- **D3** libc++ + libc++abi + libgcc_s unwinder;无 compiler-rt。
- **D4** 符号可见性纪律平台化(-fvisibility-inlines-hidden + 导出白名单
  + 门禁,§7)。
- **D5** stdlib 注入复用 aggressive-flags GBS 注入基建(§8)。
- **D6** 公开 API C 纯度审计(§6);census 已证 a 类非空,审计由假设转事实。
- **D7** 依赖线 3 clang 默认化先行或同 profile 可用;不走 GCC+libc++。

## 4. 传播图方法论(定稿,附录级)

固化为方法规范,后续快照复扫直接复用:

1. 解析限定:UND 仅在 DT_NEEDED 提供者集内解析(禁全局符号池)。
2. canonical SONAME:路径优先(/lib*,/usr/lib*)→ 非 devel → 非基建;
   同内容 SHA256 判重;变体族(-profile_*/-extension-*)折叠取基础包;
   真歧义宁缺毋假弃边并留证。
3. 排除集:非 EM_ARM;SYSROOT_COPY(/opt/data/*rootstrap*/);
   STDLIB 集(glibc/libgcc/libstdc++)不参与分量。
4. 符号分类:_Z → C++;Rust 三特征(17h 哈希后缀 / 转义残迹 / .rustc
   section)排除;STL-carrying 分 INTERFACE/VAGUE;边三层
   CPP_ABI / CPP_NOSTL / C。
5. 测试口径:unittests|gtest|haltests|-tests?$(⏳ v2 确认)双口径报告。
6. **隐藏布局耦合边(评审后补强,诚实性条款)**:签名无 std:: 但类
   数据成员含 STL 的跨界边(如 Glib::ustring、tizen_base::Bundle),
   在 mangled 签名层不可见,曾被误分 C 边。A6 头文件/DWARF 分析确认
   生产核心内 33 条,已升 TIER1(核心 123→130)。另 17 条布局证据不足
   边(ICU/sigc++/bullet 等)标强制 shim 候选,8 个含 STL 静态归档
   (libnntrainer.a 等)标同 stdlib 重建约束——两类不进连通分量,但进
   波次同批约束清单。这是 dynsym census 的能力边界,评审逼出的补强。
7. **抽验协议纪律(SPOTCHECK v1 教训)**:预登记预期与抽样规则必须联合
   审查后再发——预期对样本类型有前提时(如"demangle 含 std::"仅对
   CPP_ABI 边有效),抽样规则必须显式携带该前提;fail-stop 对评审方
   自身的规格缺陷同样生效,不设豁免。
7. **已知盲区(诚实性条款,汇报材料必须引用)**:
   a) dlopen 插件边不在图内,orphans.tsv(857)为候选清单,人工确认;
   b) CPP_NOSTL 是成员级耦合的二进制近似,精判需 DWARF/头文件级;
   c) repo 无 debuginfo → 存量产物只能 dynsym 级比对,ABI 基线比对
      依赖本地双构建(与线 3 共享此约束);
   d) **静态归档边(评审 §7 补强)**:census 仅动态符号,.a 被链入时图上
      无边;A6 实测 319 个含 STL 归档、8 个核心波同源(libnntrainer.a
      2094 指纹等)。闭合:含 STL 归档并入 D5 同 stdlib 重建纪律;
      555 个内核 thin archive 成员缺失,明确记为不可扫盲区;
   e) **快照外消费者(评审 §7 补强)**:census 节点=3707 RPM 快照,板上
      真实镜像另含已装 .tpk app 与 partner blob;A7 证实 bundle_cpp.h
      经 app-rootstrap-devel 发布,快照外 app 可直接绑定 libbundle C++
      导出。闭合:G1 删除导出前加"外部绑定者探测"前置(出厂 app 池
      UND 反扫 a 类 500 符号 + libbundle 35 导出);
   f) **单架构外推**:结论出自 armv7l 单快照,aarch64/x86 profile 的
      std 类型尺寸不同,布局断裂形态随架构变化,分量数未在其他架构核查
      ——列为已知外推边界,P2 前补 aarch64 sanity census。

## 5. 波次规划

- 波 0(先锋,线 2):chromium-efl bundled libc++ —— 平台最大 C++ 包
  退出传播图,经验回灌 P1。
- 波 1(试点,2027 Q1):无锚小分量整体切换。候选:cynara 双分量
  (7 包 + 5 包,安全栈,自闭合);⏳ 板上验证通过判据。
- 波 1.5(域波,A3 后新增):按功能域推进——输入法栈 19 包(含 isf:
  线 3 试点 + D6 a 类包同分量,三线协同)→ vision/ML 35 包。域内自闭合,
  域间无 CPP_ABI 边,可并行排期。
- 波 2..n(长尾):生产单点分量(TIER1 1521 个)按入度升序分批 mass rebuild。
- **核心波(A8 终局:TIER1 130,评审后含隐藏耦合边升格)**:
  真实枢纽序列(核心内入度):bundle 44 >> dali2 13 / jsoncpp 12 /
  dali2-adaptor 10 / app-core-common 9 / parcel 9。aul/rpc-port 经 A3
  证伪为归因假象,非切割点。
  **bundle 处置(乙案出局,compat DSO,A10 数据后定案)**:
  - SONAME 路由(定案):主体保留 libbundle.so.0(125 纯 C 消费者零改动)
    + 主体显式 NEEDED libbundle-compat.so + compat(libstdc++ 编译)承接
    35 个 C++ 导出;81 个 C++ 消费者经作用域传递解析到 compat。依据
    A10:302 ELF/215 包 NEEDED libbundle.so.0,可判定包纯 C 60.7%——
    主体保号使两类消费者均不加载期硬失败。G3 为主体→compat 依赖显式
    开洞并登记(已迁移包依赖 libstdc++ 系 DSO 是 compat 方案的设计豁免)。
  - 桥接不变量(A10,14 项):任何 std::string/vector/KeyInfo::Impl 及
    其引用不得跨 flavor;输入按 bytes/length 深拷贝,按值输出接收侧重建,
    GetName() 的 const string& 须锚定原 KeyInfo 生命周期或改 caller-owned
    copy,GetKeys() 逐元素重建保序。规格见 bridge_invariants.tsv,
    错误码/embedded-NUL/null 语义 P1 实现评审。
  - flavor 混流(A10):13 条 Bundle 跨界边全部在 130 核心同分量
    (a8_tier1_0001),跨分量混流边 0——同波原子性天然覆盖。约束:130
    核心波部署原子性列为显式发布约束。
  范式复用线 2 libwrt-c++wrapper.so bridge。切割模拟 123→43 基于旧图,
  A8/A9 升格后需在新图重跑(**A12 为行使 bundle 切割的前置**,不切割则
  非阻塞)。
  **桥接不变量补全(评审 R2 + A13 确认)**:A10 的 14 项未覆盖 BundleRaw
  = pair<unique_ptr<bundle_raw, decltype(std::free)*>, int> 及边界
  Bundle(BundleRaw,bool) / ToRaw(bool)(A13 核验:14 项命中 0)。补 2 项
  (输入/输出深拷贝 + unique_ptr 所有权跨 stdlib 转换点,pair/unique_ptr/
  deleter 不得跨 flavor),桥接规格总计 **16 项**。BundleRaw 遗漏同存于
  bundle-devel 与 app-rootstrap-devel 两份发布头(同 SHA)。
  chromium-efl 在图上近似叶节点(移除仅 -2),线 2 对线 1 的价值在
  范式与门禁经验,不在图收缩,§11 措辞以此为准。
- 波次模板(每波固定五步):双构建 → 门禁(§7)→ abidiff → 镜像组装 →
  板上场景集。白名单逐波收缩,收缩速度 = 健康度指标(借线 3 阶段 C 口径)。
- 排期骨架:P0 完成(2026 Q3,提前)→ P1 HLD 冻结(Q4)→ P2 试点
  (2027 Q1)→ P3 波次(Q2–Q3)→ P4 默认翻转(Q3 末)→ 交付 2027-10。

## 6. D6 公开 API 纯度治理

- 台账(a 类 15 包 / 500 条,census 定稿):capi-appfw-app-common /
  app-control / widget-application、capi-media-vision-3d /
  machine_learning、capi-ui-inputmethod(-core)、tizen-core-imf /
  core-util / database / dlog / libopener / shared-queue / watcher、
  united-service(均 -devel)。
- 处置模板(线 2 EWK 先例):无消费者 → 摘除;有消费者 → C API 过渡层;
  最忌消费者跟切。⏳ 每包逐条定处置,需消费者反查(graph 数据可支持)。
- 备注:capi-ui-inputmethod 与 isf 同族,线 3 试点顺带产出其双构建数据。

## 7. 门禁规范(D4)

- 门禁三件套(镜像线 2 check_libcxx_isolation 设计,方向反转):
  G1 迁移包导出无 _ZNSt(libstdc++ 泄漏);G2 未迁移包无 __1 符号混入;
  G3 NEEDED 与波次归属一致;G4 导出白名单比对。
- **G5 回绑验证(板上验证派生,每波必查)**:枢纽迁移后,对其未迁移
  消费者跑 `LD_BIND_NOW=1 LD_DEBUG=bindings ldd -r`,确认 _ZNSt* 绑定
  已从枢纽实际转移到 libstdc++.so.6——自愈从推断变为物理断言。
  标准探针即此命令(ld.so --list 不做重定位,不可用);须在无
  LD_PRELOAD/审计注入的干净环境执行(评审 §1 补强)。
- **G6 对象流类型比对(二次评审后缩定)**:适用范围**仅限双侧均有 DWARF
  的显式 C++ 签名跨界边**——对这类边做参数/返回类型闭包比对。明确**不
  覆盖** void*/opaque pointer 隧道(cast 契约不在签名内,DWARF 各自
  自洽,签名闭包方法学上不可见)与存量二进制侧(无 DWARF,退化为头文件
  法并继承其指针盲区)。上线前置:负面对照(已知类型不匹配样本必须红)。
- **隧道 API 登记册(替代 G6 对 void* 的虚假覆盖,人工纪律 + 设计消除)**:
  void*/opaque handle 承载 C++ 对象的 API 逐条人工登记(A9 产候选种子
  清单:C 头含 void*/不透明 typedef 且提供者 ELF 内部用 STL);登记册内
  API 的提供者与全部消费者**强制同 stdlib 同波**(设计上消除,不依赖
  检测);登记册纳入 §9 复审对象。
- **G7 异常处置(二次评审后改逐接口盘点制)**:先产存量"跨界抛出"接口
  盘点(含函数指针回调、exception_ptr 重抛),逐接口定处置——封边点位
  在符号图上不可枚举,禁一刀切规则。noexcept **仅限共存窗口内的混链
  边界新增接口**,存量接口不得追加 noexcept(会把 catch(...) 可恢复
  路径改写为 terminate,含 bad_alloc 场景的守护进程行为回归);
  catch(...) 封边的吞/转换语义逐接口定,不设统一规则。执法方式:源审
  清单制(noexcept 不入 mangling,无二进制执法,如实声明)。
- **执行纪律(教训固化,与 systemd-oomd 同级)**:绑定验证一律
  loader-only 重定位(ldd -r),永不真跑业务可执行文件(pkg_initdb
  --help 进入 ManifestDirectInstall 操作路径事故,2026-07-20);
  必须真跑时用牺牲镜像。
- 负面对照实验为门禁上线前置(真实红→绿,禁"预期会失败")。
- 插入点:spec %check;⏳ 与 QuickBuild CI 的挂接方式(tizen-ci-triage
  基建可复用)。

## 8. 注入与构建基建(D5)

- stdlib 切换注入:复用 aggressive-flags 的 Optflags/hook +
  per-package allowlist;新增 stdlib 维度宏(命名对齐线 2 的
  __use_bundled_libcxx 风格,待定名)。
- libc++ 打包:基于产线 LLVM 21/22 源出 libc++/libc++abi RPM 进 GBS repo;
  ⏳ 版本策略与 Toolchain 快照线的关系。
- 构建隔离:大规模 mass rebuild 沿用 systemd service + MemoryMax 教训。
- **静态归档同 stdlib 重建纪律(评审 §7/A6 派生)**:链入含 STL 静态
  归档(.a)的包,必须与该归档同 stdlib 重建——归档耦合对动态图不可见,
  混 stdlib 链接会在单 DSO 内产生 ABI 撕裂。8 个核心波同源归档
  (libnntrainer.a/libinference_engine_s.a/libpcrecpp.a 等)进准入检查。

## 9. 回退协议

- 包级:allowlist 摘除即回 libstdc++(D5 机制天然支持);
  **CPP_NOSTL 复审触发器(二次评审后机械化)**,双触发条件,均为机器
  检测非人工申报:
  a) 归属集合变更:allowlist/波次台账以机器可读文件为准(NEVRA 级),
     VCS hook 挂 diff,任何提交触发受影响包的跨界边复审;波次**顺序**
     属于集合身份(重排即变更);
  b) 跨界边集变更:每次构建产物做 NEEDED/UND 差分(对上一版基线),
     新增跨界 CPP_ABI/CPP_NOSTL 边即触发——覆盖版本 bump 引入新耦合
     而归属不变的窗口(快照漂移);census 台账设 edge invalidation
     register,基线快照过期边逐条登记失效,静默作废是违规。
  **紧急回退时序**:波启动时预计算每包"单包回退安全表"(该包独退后
  新增的跨界边及逐边判定);紧急回退查表——表内绿则放行、红则拦截,
  消除"回退部署先于复审"的竞态。安全表生成是波启动门禁的一部分。
- 波次级:波内任一 P0 缺陷 → 整波回滚,白名单回退上一版本;
- 平台级:默认翻转(P4)前保留双 profile 可构建;翻转后 libstdc++
  运行时仍在镜像(D2),回退无 ABI 断崖;
- **D7 状态(2026-07-22 翻转:阻塞已消解)**:D7(clang 默认化)前置
  **已满足**——平台 Toolchain 线于 2026-07-22 发布更新 buildconf
  (SHA 9eac0e23),原生默认 %_toolchain=clang、装 clang/LLVM、导出
  CC/CXX、flags 分层;gbs_llvm.conf 无 overlay 无 -B 即以 clang 21.1.1
  clean 构建 isf 通过(retest_no_B 身份门 PASS)。platform_gap_report
  四项缺口全 CLOSED,仅 flags 分层细节留作迁移治理清单(非阻塞)。
  原书面日期索取动作**取消**(契约已在手);决策门(滑移映射/最迟
  可行日/descope)降为历史预案保留,不再是活动风险。P2 试点关键路径
  头部解锁。
- ⏳ 回退演练纳入 P2 试点验收项。

## 10. 风险登记簿

| 风险 | 数据 | 缓解 | 状态 |
|---|---|---|---|
| 核心波规模 | 下界 130,上界 402(直接闭包)/513(全闭包封顶);旧 ≤330 作废 | 逐波登记册收敛 + CodeGraph 精确化 | 已量化(区间口径) |
| 隐藏布局耦合 | 33 边升格 + 692 unknown 边(2479 隧道 API)+ 指针/C 边 | G6 缩定 + 隧道登记册 + 同波约束 + A9 扫描 | 已量化,登记册承接 |
| BOTH 剔除依赖 D2 | 882 边自愈以 libstdc++ 永驻为前提 | D2 写死为不可撤销决策 | 已记录 |
| 生产包内测试 ELF 漏网 | nntrainer 2 条实证 | 包级过滤盲区,ELF 级路径可补 | 已定界(量级极小) |
| CPP_NOSTL 上界不确定 | 209 边 | D1a 双口径 + 抽样 DWARF 精判(⏳) | 开放 |
| dlopen 盲区 | orphans 857 | 配置侧清单人工确认(⏳ 排期) | 开放 |
| 锚点触及核心波 | 生产最大分量无锚(addendum v2) | 5 单点豁免台账 | 已闭合 |
| a 类 API 泄漏 | 15 包 500 条 | §6 台账治理 | 已定界 |
| repo 无 debuginfo | — | 本地双构建供 ABI 比对 | 已接受 |

## 11. 与线 2 / 线 3 接口

- 线 2 → 线 1:bridge(C ABI wrapper)架构范式;__use_bundled_libcxx
  宏形态;EWK 泄漏处置模板;隔离门禁脚本设计。⏳ 线 2 构建到底 +
  门禁首跑结论。
- 线 3 → 线 1:D7 前置(clang 默认化);isf PoC 已收官——pipeline.sh
  直接复用为波次五步中的 abidiff 环节;偏差编目首条
  ABSTRACT_VTABLE_DTOR_SLOT(标注型 suppression,不隐匿计数)进波次
  门禁预分类;P1-INLINE 与 census 的交叉互证为两套方法首次相互确证。
  %_toolchain 契约缺口已实测确认并上报(platform_gap_report:原子契约
  缺失 / RPM 4.14 宏基线 / flags 分层),为 D7 与阶段 C 的显式 blocker。
- 线 1 → 线 2/3:census 图数据(graph_edges.tsv)开放反查接口。

## 12. 交付物清单

HLD v1.0(P1 冻结)/ 波次台账 / 门禁脚本套件 / 白名单治理手册 /
新增 blob 准入规则(D2 配套)/ 交付报告(中英)。
- **compat DSO 准入规则(R7h,防范式增殖)**:compat DSO(平台自有
  libstdc++ 编译的 C++ 兼容层永驻)仅限"快照外无界消费 + 值语义 C 化
  成本可控"的枢纽包(当前仅 bundle 合格);b 类 96 个原生 C++ devel 包
  若提同类诉求,逐个走准入评审,非默认许可;compat DSO 总数设上限并
  纳入豁免台账审计(见下)。
- **豁免台账元审计(R7i 派生)**:门禁例外集合(ABSTRACT_VTABLE_DTOR_
  SLOT suppression / 5 锚点豁免 / blob 准入 / compat DSO 的 G3 开洞 /
  不可扫归档)由专人持有并逐波审计增长速率与收敛性——门禁可信度 =
  规则强度 × 例外收敛性。

## 13. 变更记录

| 版本 | 日期 | 变更 | 证据 |
|---|---|---|---|
| v0.1 | 2026-07-17 | 骨架建立,P0 数据槽位填充 | census v5 + addendum |
| v0.2 | 2026-07-17 | 锚点终局、生产口径数字、测试桥接发现填充 | closure addendum v2(5/5)|
| v0.3 | 2026-07-20 | A3 归因修正:核心波 123、域对齐分量、D2 前提条款、HUB_ONLY 改判 | vague_attribution.md(7/7)+ 本 session 明细审读 |
| v0.4 | 2026-07-20 | A4 终局:D1a 定案、bundle 唯一切割点(123→43/35)、TIER2 248 降为核查上界、Prompt A 收官 | final_summary.md(8/8)+ a4_cut_simulation 明细审读 |
| v0.5 | 2026-07-20 | 板上验证:BOTH 剔除锚定(REFUTES=0)、表述修正为"存在但迁移安全"、新增 G5 回绑门禁与执行纪律条款 | onboard_verification/(RPI4 raw log)|
| v0.6 | 2026-07-20 | SPOTCHECK v2 PASS:P0 三层证据收口、抽验协议纪律条款、pen-wave D2 语义实证 | independent_spotcheck/v2/ |
| v0.7 | 2026-07-20 | A5 bundle 核查:EASY 44/HARD 0、切割双方案并陈(倾向 header-only inline 化)、乙案前置验证项 | bundle_audit/(明细审读)|
| v0.8 | 2026-07-20 | 乙案前置验证闭合(1 例外边,两案均不可消,同波次处理);线 3 联动:isf 双构建身份门 FAILED(LLVM 线静默回落 GCC),诊断中——D7 前置条件(clang 默认化基建可用)当前不满足,证据同时支撑 D7 的必要性 | inter_consumer_check.md + abi_poc/checkpoint1.md |
| v0.9 | 2026-07-20 | 冻结前提证伪并改判:%_toolchain 契约在 Toolchain 快照中不存在(仅 rpm 包孤立 clang 宏,与 buildconf GCC 装包集自相矛盾);线 3 阶段 C 就绪判据改写,platform gap 上报;PoC 走最小覆盖层(申报制) | toolchain_diagnosis.md(H1)|
| v0.10 | 2026-07-20 | Prompt D 收官:isf 偏差全量定性(ABSTRACT_VTABLE_DTOR_SLOT 编目 / P1-INLINE census 互证 / 阻断 0),§11 接口填实,pipeline.sh 交付,进 P1 冻结评审 | p0_final_verdict.md + d2b/ |
| v0.11 | 2026-07-22 | 首轮异构评审 PASS 2/FAIL 5;§2 经 D-2c 实测维持 PASS(规范演进触发器登记);§1 维持 PASS | HLD_review_response + d2c_verdict.md |
| v1.0-rc | 2026-07-22 | 首轮 5 FAIL 整改并入正文(核心波 A8→≥130、bundle→compat DSO、G6/G7、§9、盲区 3 类、D5 重建纪律、D7 决策门) | A6/A7/A8 + 评审整改 |
| — | 2026-07-22 | **二次异构评审 7/7 FAIL**,不冻结。元教训:整改产出设计意图≠闭合证据,"技术闭合"提前核销已纠正;核心波改述为 ≥130 下界;四阻塞项待 A9/A10 + G6 重定义 + D7 日期闭合后升 v1.1-rc 进第三轮短评审 | 三模型评审 + HLD_review_response 第二轮 |
| v1.1-rc | 2026-07-22 | 二次 7 FAIL 技术闭合:核心波定为 ≥130 + 区间[130,≤330](A9)、compat DSO SONAME 定案+14 桥接不变量+flavor 0 跨分量(A10)、G6 缩定+隧道登记册(2479 种子)、G7 盘点制、§9 双触发机械化、D7 决策门四要素、compat 准入规则+豁免元审计。唯一剩余硬门=D7 书面日期(外部) | A9/A10 + 机制重写 + 评审第二轮响应 |
| — | 2026-07-22 | **D7 硬门消解**:平台发布原生 clang buildconf(SHA 9eac0e23),gbs_llvm.conf 无 overlay 构建通过;isf ABI 原生重验坐实(7 抽象类/P0-REAL 0/非INLINE P1 0/强未解析 0;P1-INLINE 1→6、P2 1234→212 均良性可归因;profile release 差异使根因限"原生环境"非单一编译器)。无剩余硬门,可进第三轮短评审 | retest_no_B + native_rerun |
| — | 2026-07-22 | **第三轮短评审 3 PASS(R2/R3/D7)/1 FAIL(R1)**:R1 上界 ≤330 系一跳口径错误,经 A13 分量传递闭包实算修正为区间 [130,≤513](UB_direct 402/UB_closure 513,四方同数);BundleRaw 桥接不变量补 14→16。R1 为纯文档机械修正,核对该处即视同通过 | 三模型短评审 + A13 |
| **v1.1** | 2026-07-22 | **冻结**。R1 修正经三方复核确认闭合(判据 1–4 全 YES);正文残留旧读数(核心波 123)清除;补封顶范围脚注(同批约束 +6 批次面 / shim fallback +3 严格极值 516)。三轮评审全部闭合,HLD 冻结生效 | 三模型 R1 复核 + A13 + 正文同步 |

## 14. 证据文件索引

/home/toolchain/plan_evaluation/abi_census/:summary.md(v5+addendum)、
final_validation_v5.tsv、components_tier1/2.tsv、graph_edges.tsv、
anchors.tsv、variant_families.tsv、public_api_cpp_leak_class*.tsv、
orphans.tsv、sysroot_copies.tsv、rust_cpp_edges.tsv、各 *_superseded/。
