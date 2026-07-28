# 波 1：cynara + 插件/N2 扩展试点执行方案

状态：P1 评审终裁修订稿；不代表已获得迁移准入。数据基线固定为
`freeze_v1.1/evidence/`，生产包过滤沿用
`unittests|gtest|haltests|-tests?$`。

门禁与启动状态以 startup_conditions.md 为唯一权威,本文冲突处以彼为准

## 变更记录

| 版本 | 评审出处 | 修订 |
|---|---|---|
| review-final-1 | HLD v1.1.3 D3 | 重写 libc++/libc++abi 打包门：版本节点、依赖闭包、逐符号绑定和符号族归属 |
| review-final-1 | wave1 88↔27 终裁 | 新增 88 条 unknown→27 API 的启动门映射工件；无映射/未裁决均 fail-closed |
| review-final-1 | 回退终裁 | 增加策略库状态连续性、legacy NEVRA 恢复口径和镜像级原子回滚；明确回到 clang+libstdc++ |
| review-final-1 | Toolchain 仓只读核验 | 记录当前 armv7l 仓及 clang RPM 均不含 libc++abi，故版本节点实检为无产物阻塞而非 PASS |
| review-final-2 | 打包任务评审复核轮 | 版本节点收紧为 `SYMBOL@@LLVM_21` 默认版本；增加无版本 UND 对默认版本的运行期绑定实测 |
| review-final-3 | Claude/GPT/Kimi 三方 AI 初评收敛 + `lhmax2025` 人类签核 | 27 项 C API 全部回填 `SIGNED/HANDLE_CONFINED`；签核人、日期、评审包 SHA 固化到登记册 |
| review-final-3 | 三方收敛 N1/N2/N4 | 将 dlopen 插件提供包、UidSandboxing C++ API 消费面纳入 fail-closed 同批集合；增加板端三项镜像门和 C/C++ 发布头分流方法 |
| review-final-3 | 插件 spec / repo 只读核验 | 固化插件 RPM 拆分事实；核验 `security-privilege-manager-devel 1.4.0-0` 在当前快照可解析 |
| review-final-4 | blocker round2 消费者/provider 取证 | 以 askuser 17 个调用点和精确同 NEVRA `pkgmgr-info-0.37.2-1` 源码补齐 10 项 pkgmgr-info 候选证据；与 4 项 capi 候选合并为 14 项待三方签核包 |
| review-final-4 | UidSandboxing 门终裁 | 将“扫描为空”修正为逐 ELF 台账对账；增加正式构建命令冻结 S6，四包摘除改为 `PENDING_BOARD_SCAN`，并登记 boost 三包条件同批分支 |
| review-round3 | 三方评审缺口闭合 | 穷尽 capi callback invoke 根并确认 `UNCOVERED_ROOT=0`；补齐 pkgmgr-info Label view 与 user_data 旁证；callback 非空项强制并入 G7 异常轴 |
| review-round3 | sibling 依赖核验 | 确认 `security-license-manager` 未版本锁定主包，RPM resolver 可接受新插件/旧主包混装；镜像组装门新增 sibling NEVRA/stdlib 对账 |
| p1-exit-remediation-r2 | GPT B-04/B-07 | 启动红项与 `startup_conditions.md` 同步为两类；21.1.1-1 PoC 标记 SUPERSEDED，改以三架构 21.1.1-2 报告为权威 |
| p1-exit-remediation-r3 | D-G4 source/promotion 分层 | 将 D5 构建粒度固定为 source RPM，将镜像晋级粒度固定为 TIER1 分量闭包；19/23/26 分支补齐 Boost source 条件构建和逐输出 ADMIT/HOLD 台账 |
| p1-exit-remediation-r4 | GPT r3 关闭条件 + Claude/Kimi notes | source 输出域纳入 noarch；boost-license 改为同源 ADMIT；增加 promotion-ledger 第四门、三组实例、跨批 authority 交接和 26 分支回写义务 |

## 1. 数据支撑的结论

### 1.1 原始图内 cynara 集合

冻结 A9 TIER1 图给出两个独立分量，共 12 包：

| 分量 | 包数 | 成员 |
|---|---:|---|
| `a9_tier1_0004` | 7 | `cyad`, `cynara`, `libcynara-admin`, `libcynara-agent`, `libcynara-client`, `libcynara-commons`, `libcynara-monitor` |
| `a9_tier1_0006` | 5 | `libcynara-creds-commons`, `libcynara-creds-pid`, `libcynara-creds-self`, `libcynara-creds-socket`, `libcynara-uid-creds` |

逐包清单见 `components.tsv`。原 12 包内有 19 条边：
`CPP_ABI=10`、`CPP_NOSTL=8`、`C=1`；其中 5 条 `CPP_NOSTL`
连接两个分量。因此这 12 包是不可拆的内部核心，但经 review-final-3 发现的
dlopen 插件与 N2 消费面表明，**12 包不再是完整执行集合**。原始切片明细见
`internal_edges.tsv`，执行集合改为下一节的条件化批次台账。

### 1.2 review-final-3 扩展执行集合

三方 AI 初评一致指出，cynara 插件通过 `dlopen` 装载后以 C++ 虚接口传递
`PluginData=std::string`、`std::vector<PolicyDescription>`、
`PolicyResult` 和 RTTI 对象；这是真实 C++ ABI 边界，不在 27 项 C API
签核范围内。冻结解包树确认四只插件：

- `security-license-manager`：
  `liblicense-manager-plugin-{client,service}.so`；
- `askuser-notification`：
  `libaskuser-plugin-{client,service}.so`。

spec 与 A9 分量核验结果：

| 提供包 | A9 分量 | 核心 130 | RPM 拆分结论 |
|---|---|---|---|
| `security-license-manager` | `a9_tier1_1398`，1 包 | NO | 两只插件和 agent 独占 `%files -n security-license-manager`（`security-manager.spec:429-436`）；RPM payload 可独立切换，但同一 source build 生成的 sibling candidate RPM 禁止未经 allowlist 进入镜像 |
| `askuser-notification` | `a9_tier1_0010`，3 包 | NO | 插件位于主包 `%files -n %{name}`（`askuser-notification.spec:177-187`），不能只切插件文件；整个 3 包分量同批 |

`security-manager` 自身位于 `a9_tier1_0007`（4 包），**不在 130 核心波**；
因此本快照没有“波 1 与核心波耦合”。N2 源码审计同时确认
`libcynara-uid-creds` 的公开 C++ API 使用
`std::string_view/std::optional/std::variant`。冻结动态符号观察到 6 个
UidSandboxing 消费包，其中 3 个已在原 12 包，另 3 个
`libcynara-creds-{dbus,gdbus,sd-bus}` 必须加入。同一源码审计还发现
`security-manager` / `libsecurity-manager-client` 在
`SUPPORT_UID_SANDBOXING` profile 下调用新版 std-bearing API；按
fail-closed 口径将其 4 包 TIER1 分量一并加入。只有目标 buildconf 明确证明
`SUPPORT_UID_SANDBOXING=OFF`，正式 wave 构建命令又证明没有
`uid_app_sandboxing`、`dev_wos` 或其他宏覆盖，并且全镜像
UidSandboxing UND **逐 ELF 与台账对账通过**，才允许经评审摘除该条件
分量。扫描预期包含已登记的 cynara creds consumers，不能再以“全局为空”
作为 PASS。

round3 对当前 `security-manager.spec` 反向核验发现：
`security-license-manager` 子包只有 `Requires: cynara`
（spec:133-138），没有
`Requires: security-manager = %{version}-%{release}`；主包对它也只是
无版本 `Requires: security-license-manager`（spec:22-23）。因此“新插件
子包 + 存量 libstdc++ 主包”在 RPM 依赖层面**可以解析**。这不是安全结论：
镜像组装必须对同 source build sibling 的 NEVRA/stdlib 另做 fail-closed
对账，不能依赖包管理器自动阻止混装。原文与判读见
`blocker_adjudication/round3/sibling_dependency_verdict.md`。

review-final-4 将批次固化为一份 **26 行条件化预台账**：

- 19 包无条件必选；
- `security-manager`、`libsecurity-manager-client`、
  `capi-appfw-capmgr`、`boost-program-options` 四包标
  `PENDING_BOARD_SCAN`，在板扫通过前仍视为候选集合内；
- 若四包保留且 `security-manager → boost-filesystem` 选择“同批”处置，
  则条件加入 `boost-filesystem/boost-log/boost-thread` 三包。

分支规模为：摘除条件四包时 `ACTIVE_BATCH=19`；保留四包并以 shim/已签安全
裁决关闭 boost 边时 `ACTIVE_BATCH=23`；保留四包且选择 boost 同批时
`ACTIVE_BATCH=26`。当前尚未执行板扫，故不能提前选择 19 包分支。
这里的 19/23/26 是镜像 **ADMIT 生产包**口径，不是 D5 source 构建单元数，
也不计同源 noarch 依赖输出。19 分支仍用既定五个 source 构建单元；
23/26 分支因 `boost-program-options` 与 `capi-appfw-capmgr` 均须额外构建
整个 `boost-1.83.0-5.1` 与 `capi-appfw-capmgr-0.0.4-1` source。
Boost 的全输出口径含共享 noarch 层，为 armv7l/aarch64/x86_64
**33/30/33**：23 分支 ADMIT `boost-program-options` 与无 ELF/C++ 面的同源
`boost-license`，26 分支再 ADMIT
`boost-filesystem/boost-log/boost-thread`；`boost-doc-html` 和其余 Boost
candidate 输出均为 `HOLD_SIBLING`。`security-manager` source 的八个输出也
必须逐项登记；
其中 `security-license-manager` 已属于波 1，其他非 ADMIT sibling 保持
存量制品权威。完整输出事实见 `../ledger/boost_source_unit_census.tsv`
和 `../ledger/related_source_unit_summary.tsv`。

原 23 包候选构成为：

| 扩展来源 | 包数 | 成员 |
|---|---:|---|
| 原 cynara 双分量 | 12 | 见 `components.tsv` |
| security-manager 插件子包 | 1 | `security-license-manager` |
| askuser 插件所在完整分量 | 3 | `askuser-notification`, `capi-privacy-privilege-manager`, `libaskuser-notification-common` |
| N2 冻结 UND 外部消费者 | 3 | `libcynara-creds-dbus`, `libcynara-creds-gdbus`, `libcynara-creds-sd-bus` |
| N2 source/profile 消费者完整分量 | 4 | `security-manager`, `libsecurity-manager-client`, `capi-appfw-capmgr`, `boost-program-options` |

逐包原因、分量和条件见 `wave1_expanded_packages.tsv`；该文件现为
26 行条件化预台账，不等于 26 包已经获准。插件证据见
`plugin_provider_components.tsv`、`review_checks/plugin_spec_evidence.log`。

### 1.3 扩展集合的生产边界

按测试正则剔除 consumer，并沿用 D2 的 BOTH 归因修正后，原 23 包切片内有
**40** 条边（CPP_ABI 15、CPP_NOSTL 13、C 12）；生产边界有
**317** 条、涉及 115 个波外包：

| 边类 | 数量 | 方向与含义 |
|---|---:|---|
| `CPP_ABI` | **0** | 无显式 STL 签名跨波边 |
| `CPP_NOSTL` | **1** | `security-manager → boost-filesystem`，仍须裁决、同批或 shim |
| `C` | 248 | C/opaque 边，继续受隧道登记册约束 |
| `STDLIB` | 68 | 指向 canonical 运行时，不参加组件计算 |

全量见 `expanded_internal_edges.tsv` 与 `expanded_boundary_edges.tsv`。
此前 5 条 cynara CPP_NOSTL 边均已因批次扩展内化；不得据此把剩余
`security-manager → boost-filesystem` 静默转为安全。该边在 19 包摘除
分支随 consumer 一并消失；若四包保留，则必须选择已签安全/shim 或条件
加入 boost 三包，不得把条件分支误记为现状 PASS。

### 1.4 27 项签核与扩展后残余 unknown

原 12 包触达的 88 条生产 unknown 已完成闭包：

- `unknown_to_api_mapping.tsv` 为每条生产 unknown 边列出候选 API；
- `api_adjudication_registry_27.tsv` 固定 27 项的裁决栏；
- 27 项原始 `resolved_edges` 的并集与生产 unknown 集合相交后覆盖
  **88/88**，未映射 **0**；并集另含 11 条测试包边，按生产正则排除；
- `lhmax2025` 于 2026-07-24 对 27/27 项签署“安全/同意”，均回填为
  `SIGNED/HANDLE_CONFINED`；评审包 SHA256 为
  `2e437083bfb9490476c6362c409755e9efa3978f51db11460e82fd2c30e43646`。

三方 AI 初评原文及限制条件归档在 `reviewers/`；机械对账见
`mapping_closure_check.tsv`、`signoff_validation.tsv`。签核关闭的是这
88 条映射，不自动覆盖批次扩展新增的边。

扩展 23 包切片后，unknown 触达边为 **93**：其中 **88** 条由已签 27 API
覆盖，新增 **5** 条：

1. `launchpad → libsecurity-manager-client`（C tunnel）；
2. `libaskuser-notification-common → pkgmgr-info`（C tunnel）；
3. `security-manager → boost-filesystem`（UNKNOWN_POINTEE）；
4. `webapi-plugins-profile_mobile → capi-privacy-privilege-manager`（C tunnel）；
5. `webapi-plugins-profile_wearable → capi-privacy-privilege-manager`（C tunnel）。

review-final-4 的 round2 取证把其中三条非条件边归并到 **14 项待签 API**：

- `libaskuser-notification-common → pkgmgr-info` 对应 10 项；
- 两条 `webapi-plugins-profile_* → capi-privacy-privilege-manager`
  共用 4 项。

round3 已补齐 invoke 根、`pc::Label` non-owning view、pkgmgr-info
`user_data` 原样透传以及 callback 异常轴证据。14 项仍有
`HANDLE_CONFINED_CANDIDATE` 技术预判，状态仍为 `PROPOSED`，未完成三方
签核；签核权威输入改为
`blocker_adjudication/round3/api_adjudication_candidates_round3.tsv`
与 v3 评审包。任何 callback 非空条目必须同时关闭
`G7-PKG-001/G7-PPM-001/G7-PPM-002` 对应异常 rider，不能仅凭布局安全签字。
`launchpad → libsecurity-manager-client` 与
`security-manager → boost-filesystem` 随四包条件分量：19 包分支中消失；
若板扫不允许摘除，则前者必须另行签核/shim，后者必须按上一节的 boost
条件分支关闭。全量旧切片见 `expanded_unknown_edges.tsv`，新候选和源码证据
见 `blocker_adjudication/round2/`。

### 1.5 锚点、依赖与运行时前置

- 条件化预台账 26 包均未命中冻结锚点表，也无 130 核心成员。
- askuser spec 要求
  `pkgconfig(security-privilege-manager) >= 1.4.0`。冻结 manifest 含
  `security-privilege-manager-devel-1.4.0-0`，其 `.pc` 为 1.4.0，RPM
  Provides 精确给出 `pkgconfig(security-privilege-manager) = 1.4.0`，
  当前快照判定 `PASS_SNAPSHOT`；实际 wave buildroot 必须重跑解析，失败即
  blocker。证据见 `review_checks/security_privilege_manager_dependency.*`。
- 冻结 RPM 目录清单中 `libc++/libc++abi` 包名命中数为 **0**。
- HLD D3 固定运行时组合为 `libc++ + libc++abi + libgcc_s`，不使用
  compiler-rt/libunwind。
- 2026-07-23 对当前 `gbs_llvm.conf` 的 Base/Unified Toolchain armv7l
  目录复核：无 `libc++abi`/`libcxxabi` RPM；`clang-21.1.1-2.5.armv7l.rpm`
  文件表和 Requires 也无 libc++abi。因不存在候选 ELF，版本节点
  `readelf` 核验在当时是 **BLOCKED_NO_ARTIFACT**，不是“无版本问题”。
- 三架构权威候选现为 `libc++/libc++abi 21.1.1-2`，报告见
  `packaging/multiarch_packaging_report.md`；早期 armv7l 21.1.1-1 PoC
  仅保留为历史证据，不再提供启动状态。

当前启动红项为两类：

1. 14 项 round3 API 的签署按人工决定推迟至正式开工；候选制品构建、
   板上实测、3 项 callback G7 rider 与 Q3 裁决尚未完成（S1 文书已就绪）。
2. D5 尚未正式并入，S6 v1 已标 superseded；S6 v2 与同 buildroot 的
   S3 21.1.1-2 复验尚未产生。

当前状态与进入顺序只引用 `startup_conditions.md` 的“执行期进入序列”：
D5 并入后重冻 S6 v2，再进行候选构建、板测、14 项+Q3 签核、allowlist
正式化与五步验收；不得以历史 PoC repo 或 S6 v1 越过该序列。

### 1.6 D6 方法论补注（N4）

组件公开面审查必须先按语言和 ABI 分类发布头：C 头按 opaque/C 数据契约
审查，C++ 头继续扫描 `std::`、模板、公开类布局与 inline 实现。判断依据是
“头中暴露什么 ABI”，**不是头是否被安装**；`cynara-devel` 同时安装 C 头
与 `types/plugin/attributes/log` 等 C++ 头，不能因同属一个 devel RPM
整体判安全或整体判泄漏。

## 2. 设计决策待评审

1. `ACTIVE_BATCH` 是一个 TIER1 分量闭包的晋级/镜像事务；D5 allowlist
   采用 source RPM 构建单元。条件四包只有在
   `OFF_PROVEN + S6 命令冻结 + UidSandboxing 逐 ELF 对账`均通过后才能
   摘除，须由 Security owner 与 Toolchain ABI owner 决定。
2. 14 项新增 C API 的三方签核人及截止时间；若 S4 保留条件四包，还须决定
   launchpad C tunnel 处置和 boost 三包同批/shim 分支。
3. 跨 stdlib 的 `abipkgdiff` 必然观察到波内 STL 签名变化。建议验收口径为：
   原始差异全部保留；仅预登记的“`ACTIVE_BATCH` 内部 stdlib 命名空间迁移”可标
   `EXPECTED_TRANSITION`，波外可见 C/C++ 面仍要求 P0=0、非 INLINE P1=0。
   是否接受该口径须在开工前评审确认，禁止用 suppression 隐藏计数。
4. 板上真实鉴权场景使用哪个受保护服务/测试应用，由 Security owner 定案；
   本文给出二值化协议，不代替业务选择。

## 3. 前置依赖与 libc++ RPM 打包草案

### 3.1 Blocker 判据

以下任一不满足，禁止把 `ACTIVE_BATCH` 加入正式 allowlist：

- GBS 仓存在 armv7l 的 `libc++`, `libc++abi` runtime 与 devel RPM；
- 版本与产线 clang 21/22 快照策略一致；
- rootstrap 能安装头文件、链接脚本和真实 SONAME 文件；
- libc++abi 的**全部对外导出**均定义为批准节点的**默认版本**：
  `readelf --dyn-syms -W` 中逐项必须显示 `SYMBOL@@LLVM_21`；仅显示
  `SYMBOL@LLVM_21` 的非默认版本、匿名/unversioned 导出或混合 `@/@@`
  均为 blocker。`readelf --version-info -W` 必须同时证明
  `LLVM_21` 是相应 version definition；
- 按每个最终 ELF 的实际 UND 建立 NEEDED 依赖闭包，并逐符号验证运行时绑定
  归属；不得要求每个 ELF 直接同时出现 `libc++.so.1`、
  `libc++abi.so.1`、`libgcc_s.so.1`，因为 `--as-needed` 可合法裁掉无实际
  引用的直接 NEEDED；
- 逐符号归属至少满足：
  `__cxa_finalize → glibc`；
  `*Unwind*` 与 ARM personality → `libgcc_s`；
  `__cxa_throw/__cxa_allocate_exception/__cxa_guard_*` 等 C++ ABI 族
  → `libc++abi`；
- 混合进程 fixture 以 `LD_BIND_NOW=1 LD_DEBUG=bindings` 启动，逐条断言
  上述 `__cxa_*` 家族的实际 provider；
- 另建“旧式无版本引用”fixture：其目标 libc++abi 符号在 consumer
  `readelf --dyn-syms -W` 中为无 `@` 的 UND，且
  `readelf --version-info -W` 不含该符号的 version need；先将候选
  libc++abi 加入同一全局 lookup scope，再用 `LD_BIND_NOW=1
  LD_DEBUG=bindings` 装载 fixture。绑定原文必须证明该无版本 UND 解析到
  provider 的 `SYMBOL@@LLVM_21` 默认版本；
- 全闭包无 `libstdc++.so.6`、`libunwind.so`、compiler-rt runtime；
- debuginfo 可按 build-id 配对，供 G6/abidiff 使用。

负面对照：

- 无版本脚本的 libc++abi：G5 扩展探针必须报告 unversioned exports 并红；
- 只提供 `SYMBOL@LLVM_21`、不提供 `SYMBOL@@LLVM_21` 的构建：默认版本门
  必须红，旧式无版本 UND fixture 不得被误记为 PASS；
- fixture 若被正常链接器意外写入 version need，输入门必须先红，不能用它
  伪装“无版本引用”实测。

### 3.2 打包草案

建议从与产线 clang 同源的 LLVM monorepo tag 生成：

- `libc++`：`/usr/lib/libc++.so.1` 及版本化实文件；
- `libc++abi`：`/usr/lib/libc++abi.so.1`；
- `libc++-devel`：`<c++/v1>` 头、无版本链接名和 CMake/pkg-config 元数据；
- `libc++abi-devel`：ABI 头与开发链接名；
- 对应 debuginfo/debugsource 包。

构建固定 armv7l 目标、与平台一致的 `-mfloat-abi`，unwinder 选
`libgcc_s`。包不得覆盖 libstdc++ 文件或全局 C++ include 路径；stdlib
选择只由 D5 per-package 注入触发。打包验收含一个 clang
`-stdlib=libc++` 编译/链接/运行探针、`@@LLVM_21` 默认版本全导出检查、
无版本 UND→默认版本运行期绑定实测、依赖闭包与逐符号绑定检查、RPM 文件
冲突检查和两次可重复构建 SHA 比较。

### 3.3 当前 Toolchain 仓核验

核验输入为 `gbs_llvm.conf` 当前两个 `reference` URL 的 armv7l 目录，以及
其中 `clang-21.1.1-2.5.armv7l.rpm`：

| 检查 | 结果 | 证据 |
|---|---|---|
| Base/Unified 目录的 libc++abi/libcxxabi 包名 | 0 | `review_checks/toolchain_*_armv7l_index.html`, `repo_libcxxabi_name_matches.log` |
| clang RPM 文件表中的 libc++abi | 0 | `clang_rpm_filelist.log`, `clang_rpm_libcxxabi_file_matches.log` |
| clang RPM Requires 中的 libc++abi | 0 | `clang_rpm_requires.log`, `clang_rpm_libcxxabi_requires_matches.log` |
| libc++abi 导出均为 `@@LLVM_21` 默认版本 | **不可执行：无候选 ELF** | `BLOCKED_NO_ARTIFACT` |
| 无版本 UND 运行时绑定到 `@@LLVM_21` | **不可执行：无 provider/fixture** | `BLOCKED_NO_ARTIFACT` |

这只是当前仓现状；一旦 RPM 发布，必须对实际 `libc++abi.so.*` 执行
`readelf --version-info -W` 和 `readelf --dyn-syms -W` 的全导出对账，
再执行无版本 UND fixture。原文固定落盘：

```text
packaging_evidence/libcxxabi_dynsyms.log
packaging_evidence/libcxxabi_version_info.log
packaging_evidence/unversioned_fixture_dynsyms.log
packaging_evidence/unversioned_fixture_version_info.log
packaging_evidence/unversioned_fixture_ld_debug_bindings.log
packaging_evidence/unversioned_fixture_verdict.tsv
```

`unversioned_fixture_verdict.tsv` 至少记录：
`symbol, consumer_und_is_unversioned, provider_default_symbol,
actual_provider, raw_binding_log_line, verdict`；只有前三项证据成立且实际绑定行
指向 `SYMBOL@@LLVM_21` 对应 provider 时才 PASS。

### 3.4 [SUPERSEDED] 波 1 armv7l 21.1.1-1 本地 PoC repo 状态

以下内容仅记录历史 PoC。`p1/libcxx_packaging/packaging_report.md` 曾将 §3.3 的
`BLOCKED_NO_ARTIFACT` 对波 1 本地 repo 转为实测 PASS：

- `libc++/libc++-devel/libc++abi/libc++abi-devel` 均为 armv7l
  `21.1.1-1`；
- libc++ 1965/1965、libc++abi 371/371 定义导出均为
  `@@LLVM_21`，异常 0；
- 依赖链为 `libc++.so.1 → libc++abi.so.1 → libgcc_s.so.1`，无
  libstdc++/libunwind；
- 红绿混合进程、无 version-need fixture、异常/dynamic_cast/string
  冒烟和 GBS 本地 repo 可见性均 PASS。

本节已由 `packaging/multiarch_packaging_report.md` 的三架构
**21.1.1-2** 权威报告取代，不得再用于启动判定。Step 2 仍以实际装入
wave buildroot 的 21.1.1-2 RPM 为输入重跑全部判据，NEVRA 或 SHA
不一致即不得继承 PASS。

## 4. 执行步骤

### Step 0：冻结输入与人工闭合

1. 冻结正式 wave 的**完整构建命令原文**并计算 SHA256；命令行不得含
   `uid_app_sandboxing`、`dev_wos` 或任何其他会覆盖 profile 宏值的参数。
   命中任一覆盖项，round1 的 `OFF_PROVEN` 立即失效并重判。该永久门编号
   S6，工件写入 `blocker_adjudication/round2/formal_build_command.*`。
2. 恢复板连接后执行 UidSandboxing 全镜像逐 ELF 扫描：所有命中必须属于
   `n2_consumer_constraints.tsv` 台账，台账外命中必须为 0；若选择摘除条件
   四包，`security-manager` 系 ELF 命中还必须为 0。板扫与 S6 都通过后才
   可将四包从 `PENDING_BOARD_SCAN` 转为摘除生效。
3. 由 `wave1_expanded_packages.tsv` 选择并冻结 19/23/26 的唯一
   `ACTIVE_BATCH` 分支及 NEVRA，生成只含 ADMIT 包的 `active_batch.tsv`，
   再按 source 反查生成 `source_build_units.tsv` 和覆盖每个候选二进制输出的
   `promotion_ledger.tsv`，三者均计算 SHA256；确认插件目录预期清单和 N2
   consumer 集合。若四包保留，则对
   `security-manager → boost-filesystem` 选择已签安全/shim，或启用
   boost 三包同批分支；未关闭即停。
   生成物必须以
   `promotion_ledger_wave1_cond_{manifest,authority}.tsv` 与
   `promotion_ledger_wave1_cond.tsv` 的冻结实例为结构基线，并通过
   `gates/tools/promotion_ledger_check.py`；该冻结实例使用现有 RPM 仅校准门
   行为，执行日必须替换为本次候选构建 manifest。
4. 验证 `api_adjudication_registry_27.tsv` 仍为 27/27 `SIGNED` 且
   `resolved_edges` 并集覆盖原 88/88；再逐条关闭
   `blocker_adjudication/round3/api_adjudication_candidates_round3.tsv`
   的 14 项 `PROPOSED`，并关闭 callback 非空项引用的 3 条 G7 rider。
   任一未完成三方签核、Q3 人工裁决留空或 rider 未达到二值 PASS 即停；
   若 S4 保留条件四包，再关闭 `launchpad → libsecurity-manager-client`。
5. 在实际 repo/rootstrap 重跑
   `pkgconfig(security-privilege-manager) >= 1.4.0` 解析；失败登记为波 1
   blocker，不允许临时降版本或删 BuildRequires。
6. 生成每包单包回退安全表；预期至少 `libcynara-commons` 单退为红。
7. 完成 libc++/libc++abi RPM blocker。

### Step 1：allowlist 加入

一次提交加入冻结的全部 **source 构建单元**；台账记录 source commit、
source NEVRA、目标 stdlib、波次 ID、审批人及过期时间。19 分支沿用
`wave1_source_admission.tsv` 的五源；23/26 分支必须先补入
`boost-1.83.0-5.1` 与 `capi-appfw-capmgr-0.0.4-1` 两个 source 行，
缺任一即阻塞。每个 source 的全部候选二进制
输出必须在 `promotion_ledger.tsv` 恰好出现一次：`ADMIT` 表示其 TIER1
分量闭包已在批内，`HOLD_SIBLING` 表示候选镜像禁入且存量 NEVRA/SHA
继续为权威。提交本身触发 §9 归属集合 diff 和跨界边复审。
`security-manager` 和 `boost` source build 都会生成多个 sibling RPM；
未列入 `ACTIVE_BATCH` 的 candidate sibling 不得由 repo 优先级意外进入
镜像。

### Step 2：双构建

对同一 source tarball、同一 clang profile 做两套 clean build：

- `legacy`：clang + libstdc++ + libgcc_s；
- `candidate`：clang + libc++ + libc++abi + libgcc_s。

两线 source tarball SHA256 必须一致；buildroot 分离；保留主包、devel、
debuginfo、完整构建日志、BUILDINFO 和编译命令。source 的全部输出都必须
保留并进入晋级台账，不能通过丢弃未登记 sibling 制造表面闭包。任何 source
构建失败即整波失败，不做源码绕过。

本步骤在开始 `ACTIVE_BATCH` candidate 编译前先执行 libc++abi 打包硬门：对将进入
rootstrap/镜像的同一 RPM 解包，逐项确认导出为 `SYMBOL@@LLVM_21`，并运行
无版本 UND fixture 证明默认版本绑定；§3.3 规定的 readelf、LD_DEBUG 与
verdict 原文全部写入本次构建的 `packaging_evidence/`。缺候选 RPM、仅有
`@LLVM_21`、fixture 带 version need 或运行时未绑定到默认定义，Step 2
立即阻塞，不进入整批双构建。

### Step 3：G1–G5

| 门 | 二值 PASS 条件 | 负面对照：如何证明会红 |
|---|---|---|
| G1 迁移侧纯度 | `ACTIVE_BATCH` 无 `DT_NEEDED libstdc++.so.6`；导出面无未登记 `_ZNSt*`/GLIBCXX/CXXABI 泄漏；标准库模板默认隐藏 | 构建一个导出 `std::string`/显式模板实例的 candidate fixture，门必须报导出符号与所属 ELF |
| G2 未迁移侧隔离 | 波外包无 `std::__1` 符号且无新增 `libc++.so.1/libc++abi.so.1` NEEDED；每个 `HOLD_SIBLING` 的候选 payload 均未进入镜像，存量 NEVRA/SHA 与台账相等 | 构建一个 legacy fixture，故意链接 libc++ 并引用 `std::__1::string`，门必须红；再把一个 HOLD candidate 放入 repo 优先路径，镜像门必须红 |
| G3 NEEDED/归属 | 每个 ELF 的 NEEDED 闭包与台账一致；逐 UND 绑定到批准 provider；不强制无引用库成为直接 NEEDED；cynara 波无 G3 豁免 | 给一个波外 fixture 加 `--no-as-needed -lc++`，即使无 UND 也必须由 NEEDED 检查报红 |
| G4 导出白名单 | candidate 相对冻结/双构建基线无未登记新增或消失导出；所有差异逐符号归档 | 给一个函数加 default visibility 的负面对照构建，新增导出必须红 |
| G5 回绑与强解析 | 干净板环境对波内 ELF及全部波外入边消费者执行 `LD_BIND_NOW=1 ldd -r`，强未解析为0；`LD_DEBUG=bindings` 证明符号族 provider；libc++abi 全导出为 `@@LLVM_21`；无版本 UND fixture 实际绑定默认版本 | 强 UND必红；无版本化或仅单`@LLVM_21`构建必红；带version need的伪无版本fixture输入门必红；`__cxa_throw`绑错provider必红 |

G1/G2 的精确符号正则须随 libc++ 包产物做一次基线生成，避免把合法
`std::__1` 与 libstdc++ 泄漏混为一谈；无论正则如何，`DT_NEEDED` 是独立
硬判据。

### Step 4：ABI 对比

复用冻结的 `docs/pipeline.sh` 的产物定位、debuginfo 配对、原始 diff、
分级和 UND 闭包步骤。每个二进制包均保存 raw `abipkgdiff` 输出。

二值验收：

- P0=0；
- 波外可见面非 INLINE P1=0；
- P1-INLINE 仅在全平台/镜像消费者 UND 反查为 0 时可接受；
- 波内 stdlib namespace 变化只能进入逐符号
  `EXPECTED_TRANSITION` 台账，不能通过 suppression 消失；
- candidate 强未解析=0。

### Step 5：镜像组装

从同一 candidate repo 原子组装恰好包含
`promotion_ledger.tsv` 中全部 `ADMIT` candidate RPM、且不含任何
`HOLD_SIBLING` candidate payload 的试点镜像，同时保留 libstdc++ 兼容
运行时。镜像 manifest 必须逐项证明 ADMIT 无 legacy NEVRA、HOLD 仍为
登记的存量 NEVRA/SHA，并保存 libc++/libc++abi/libgcc_s 包版本。门禁必须
机械断言 ADMIT 集合覆盖其全部 TIER1 分量成员，且每个 source 构建输出在
晋级台账中恰好出现一次。

候选仓求解必须使用实际三架构候选 RPM 重跑完整依赖闭包和 Provides/Requires
匹配。冻结 Boost 取证未发现 Boost sibling 间精确 EVR 强锁，内部 Boost
名称/SONAME 依赖均能由同 source 输出集合解析；`boost-license` 已由冻结
Base `noarch/` RPM 的 `SOURCERPM=boost-1.83.0-5.1.src.rpm` 和 SHA 对账证明
为同源输出，23/26 均 ADMIT。候选仓缺失、版本漂移或候选元数据新增精确锁仍
直接阻塞；solver 检查是辅助验证，不能代替逐输出晋级门。镜像组装后、业务
冒烟前必须先通过下一节三项 fail-closed 枚举门和
`promotion_ledger_check.py` 第四门。

若 26 分支生效，本批对 `boost-filesystem/boost-log/boost-thread` 的处理
必须回写 Base-first 晋级台账：逐输出显式选择重验闭包后再次 ADMIT，或把
Base-first 已晋级 NEVRA/SHA 设为新的 legacy authority 并 HOLD；不得留空，
也不得让两个批次对同一输出各持不同 authority。

`security-license-manager` 与 `security-manager` 之间没有 sibling 版本锁，
所以组装门还必须逐 RPM 比较 source NEVRA/build-id/stdlib 身份：若装入新
`security-license-manager`，同一 source build 的主包或经批准的“插件独立
切换声明”必须与 manifest 一致。resolver 能解出新插件+旧主包不构成 PASS；
未登记的这种混装直接红。

### Step 6：板上场景集

1. **插件目录批次门**：枚举 `/usr/lib/cynara/plugin/` 下全部 `.so`，
   逐文件用 RPM ownership 映射提供包，并与冻结的 `active_batch.tsv`
   插件批次清单比较。发现未登记、无 owner、legacy NEVRA 或树外 vendor
   插件即红；不得只检查已知四只插件。原始命令至少包含
   `find /usr/lib/cynara/plugin -type f -name '*.so' -print` 和逐文件
   `rpm -qf`，排序后的路径、owner、NEVRA 全量落盘。
2. **agent 消费者门**：全 rootfs 用 `scanelf` 扫描
   `cynara_agent_*` UND，结果与已知 agent 清单比较。除登记 consumer 外
   任何新增 ELF 即红并进入载荷路径复核；命令
   `scanelf -qRs +cynara_agent_ /` 的 stdout/stderr/退出码逐字落盘。
   扫描工具缺失或目录不可读也按红。
3. **UidSandboxing 门**：全镜像扫描
   `_ZN6Cynara13UidSandboxing*` UND，逐 ELF 与
   `n2_consumer_constraints.tsv` 对账。PASS 不是“扫描为空”，而是**逐 ELF
   对账台账内、台账外命中 0；条件四包摘除时 security-manager 系命中
   0**。遗漏 consumer、出现台账外 ELF、条件分量被摘除但仍有
   security-manager 系命中，或输出不可完整复现均为红；命令
   `scanelf -qRs +_ZN6Cynara13UidSandboxing /` 的原始输出和逐 ELF
   `readelf --dyn-syms -W` 复核一并落盘。
4. **启动期全解析**：对所有波内 ELF 和按最终 `ACTIVE_BATCH` 重算出的
   全部边界外消费 ELF 跑 `LD_BIND_NOW=1 ldd -r`；review-final-3 的原
   23 包切片是 115 个波外包，只是历史基线，不得对 19/26 分支硬编码该
   数字。强未解析总数必须为 0。仅 loader 重定位，不用业务可执行文件的
   `--help` 触发验证。
5. **cyad 增删查**：用唯一临时 bucket：
   `--set-bucket` → `--set-policy` → `--check` →
   `--list-policies=<bucket>` → `--erase=<bucket>` →
   `--delete-bucket=<bucket>`。每步退出码、stdout/stderr 和最终不存在
   断言均落盘；失败清理由预置恢复命令执行。
6. **真实鉴权路径**：在牺牲镜像运行一个由 Security owner 预登记的受保护
   操作。先以 deny policy 证明拒绝，再切 allow policy 证明同一 client/user/
   privilege 被允许，并以服务端审计记录确认实际经过 cynara，而不是测试
   桩短路。两个方向结果与审计记录均须匹配。

## 5. 验收清单

全部为二值项，任一红即整波不通过：

- [ ] 正式构建命令已逐字冻结，且不含 `uid_app_sandboxing`、`dev_wos`
      或其他宏覆盖；否则 `OFF_PROVEN` 已失效并重判；
- [ ] `ACTIVE_BATCH` allowlist/镜像原子一致；条件四包若摘除，
      UidSandboxing 扫描满足“逐 ELF 对账台账内、台账外命中 0；
      security-manager 系命中 0”并已签核；
- [ ] source 构建单元清单覆盖每个 source 的全部二进制输出；
      `promotion_ledger.tsv` 对每个输出恰好一行，ADMIT 集合满足 TIER1
      分量闭包，HOLD candidate 未入镜像且存量 NEVRA/SHA 保持权威；
- [ ] 23/26 分支已在 `wave1_source_admission.tsv` 补入 Boost 与
      capi-appfw-capmgr source；
      19 分支未无条件引入 Boost，分支 ADMIT/HOLD 集合与 D-G4 一致；
- [ ] `promotion_ledger_check.py` 对候选 manifest、晋级台账与存量
      authority 表为 GREEN；noarch 输出无漏行，HOLD SHA 无漂移，精确锁未
      跨 ADMIT/HOLD；
- [ ] 插件目录枚举与批次清单逐文件一致，无未登记/legacy/vendor 插件；
- [ ] `security-license-manager` sibling NEVRA/stdlib 与镜像声明一致；
      包依赖虽可解析的新插件+旧主包组合若未登记，镜像门为红；
- [ ] 保留条件四包时，生产边界 boost CPP_NOSTL 已取得三包同批、shim
      或安全裁决；摘除分支中已机械证明该边消失；
- [ ] 原 88/88 unknown→API 由 27 个 `SIGNED/HANDLE_CONFINED` 条目覆盖；
      round2 新增 14 项 API 已完成三方签核；保留条件四包时
      launchpad tunnel 也已关闭；
- [ ] `security-privilege-manager >= 1.4.0` 在实际 buildroot 解析成功；
- [ ] 全 rootfs `cynara_agent_*` 和 UidSandboxing UND 清单与台账一致；
- [ ] libc++abi 全导出均为 `@@LLVM_21` 默认版本；无版本 UND fixture 确认
      运行期绑定默认版本；依赖闭包/逐符号归属与混合进程
      `LD_DEBUG=bindings` 全绿，六份 `packaging_evidence/` 工件齐全；
- [ ] G1–G5 正例全绿、每门负面对照均真实跑红；
- [ ] P0=0，波外可见面非 INLINE P1=0，P1-INLINE 消费者 UND=0；
- [ ] 板上 `LD_BIND_NOW=1 ldd -r` 强未解析=0；
- [ ] cyad 增、查、删和最终清理全部通过；
- [ ] deny→allow 的真实鉴权路径及服务审计通过；
- [ ] 下节整波回退演练通过。

## 6. 回退演练设计

回退是波 1 验收项，不是文档演习。本文选择**镜像级原子回滚**：不支持
`ACTIVE_BATCH` 逐 RPM 顺序降级，也不把任何“降到一半后重启”的混装态定义为可运行
状态；部署系统必须在新旧镜像槽之间原子切换。

1. 从 allowlist 一次摘除全部 `ACTIVE_BATCH` 包，生成台账 diff；
2. 恢复安全表登记的 last-known legacy NEVRA；若制品保留策略无法保证该
   NEVRA，平台回退策略必须显式选择“同一 source tarball 全新 clean
   rebuild”，不可临时混用；
3. `readelf -d/-Ws` 逐 ELF 证明重新 `NEEDED libstdc++.so.6`，不再
   NEEDED `libc++.so.1/libc++abi.so.1`，且无 `std::__1` 残留；
4. 在 candidate 镜像先完成 cyad 增删查及真实鉴权，并写入预登记的持久
   policy 数据；保留数据分区，原子刷/切 rollback 镜像；
5. rollback 镜像用 libstdc++ 构建执行同一 policy 的 check/list，结果必须
   与 candidate 写入内容一致，随后再做增删查和真实鉴权；
6. 重跑 G2/G3/G4、全量 `LD_BIND_NOW=1 ldd -r`；
7. 输出迁移前、candidate、rollback 三份镜像 manifest、数据连续性对账和
   last-known legacy NEVRA 恢复证据。

“legacy/rollback”在本线含义固定为 **clang + libstdc++ + libgcc_s**，
不是波前 GCC 产物；编译器回退变量由线3管辖，不得混入本波 stdlib 回退验收。

负面对照：尝试生成“只回退 `libcynara-commons`”的安全表。它与多个波内包
存在 `CPP_ABI` 边，算法必须给出 RED；若给绿，回退门本身未上线。

脚本接口设计（本任务不实现）：

```text
wave-rollback-rehearse \
  --wave-id wave1-cynara \
  --packages <active_batch.tsv> \
  --allowlist-before <candidate_allowlist.tsv> \
  --allowlist-after <rollback_allowlist.tsv> \
  --source-manifest <source_sha256.tsv> \
  --legacy-buildconf <clang-libstdcxx.conf> \
  --candidate-repo <url-or-path> \
  --image-profile <profile> \
  --board <sdb-serial> \
  --scenario <cyad-and-auth.yaml> \
  --output <evidence-dir>
```

阶段固定为 `VALIDATE_INPUT → DIFF_ALLOWLIST → CLEAN_BUILD →
VERIFY_STDLIB_IDENTITY → ASSEMBLE_IMAGE → LDD_R →
CYAD_SMOKE → REAL_AUTH → VERDICT`，每阶段写独立 raw log 和
`phase_results.tsv`。退出码建议 0=全部通过，2=验收失败，3=输入/环境错误；
失败不得跳过后续清理，但最终状态保留第一个失败阶段。脚本只允许使用预登记
的临时 bucket/test identity，cleanup 结果单独成为二值验收项。

## 7. 证据索引

- `evidence_summary.tsv`：本方案计数。
- `components.tsv`、`internal_edges.tsv`、`boundary_edges.tsv`：原 12 包
  冻结图切片，仅作 review-final-3 前基线。
- `wave1_expanded_packages.tsv`：26 行条件化预台账；启动时选择
  19/23/26 的单一 `ACTIVE_BATCH` 分支并生成 `active_batch.tsv`。
- `../ledger/boost_source_unit_census.tsv`：Boost 三架构全部输出、分量、
  生产边、Requires、devel/静态归档与分支 ADMIT/HOLD 事实。
- `../ledger/related_source_unit_summary.tsv`：T1-0008 三个 Unified
  成员、`libsigc++`、`taglib` 的 source 输出闭包审计。
- `../ledger/promotion_ledger_template.tsv`：执行期逐 RPM 晋级台账模板。
- `../ledger/promotion_ledger_{basefirst,t1_0008_s4pass,wave1_cond}.tsv`
  及配套 manifest/authority：冻结 RPM 输入上的三组可执行实例；执行日只继承
  schema 与门行为，不继承候选 SHA。
- `expanded_internal_edges.tsv`、`expanded_boundary_edges.tsv`：
  review-final-3 原 23 包的生产图切片；最终 `ACTIVE_BATCH` 冻结时必须
  按相同规则重算，不能把 23 包边界计数套用到 19/26 分支。
- `plugin_provider_components.tsv`、`n2_consumer_constraints.tsv`：N1/N2
  批次边界；spec 原文见 `review_checks/plugin_spec_evidence.log`。
- `unknown_hits.tsv`、`tunnel_api_hits.tsv`：A9 不可判定面。
- `unknown_to_api_mapping.tsv`、`api_adjudication_registry_27.tsv`、
  `mapping_closure_check.tsv`、`signoff_validation.tsv`：88↔27 签核闭包。
- `expanded_unknown_edges.tsv`：review-final-3 的 23 包切片；round2 对
  其中三条非条件边形成 14 项候选。
- `blocker_adjudication/round2/`：17 个消费者调用点、精确同 NEVRA
  provider 旁证、14 项候选三方评审包、S4/S6 状态和 review-final-4 diff。
- `blocker_adjudication/round3/`：invoke 根闭包、Label/user_data 补证、
  3 项 G7 rider、4 项非波门存量缺陷、sibling Requires 判读与 v3 评审包；
  `api_adjudication_candidates_round3.tsv` 是 14 项签核的现行候选表。
- `reviewers/`：Claude/GPT/Kimi 三方 AI 初评、限制条件及
  `lhmax2025` 人类签核源。
- `startup_conditions.md` / `.tsv`：review-final-4 的六项启动硬门。
- `review_checks/`：当前 Toolchain 仓/clang RPM 的 libc++abi 只读核验原文。
- 冻结权威源：
  `../../freeze_v1.1/evidence/abi_census/cutpoints/a9/`、
  `../../freeze_v1.1/evidence/abi_census/graph_edges.tsv`、
  `../../freeze_v1.1/docs/HLD_libcxx_migration_v1.1_frozen.md`。

## 8. 修订自查

外置变更记录为 `change_log.tsv`；终裁自查为 `self_check.tsv`。注意：自查
通过表示文档与证据一致，不表示 wave 已获准。27 项原 C API 已关闭；当前
仍为 `NOT_READY`，启动红项为两类：

1. 14 项 round3 API 的签署按人工决定推迟至正式开工；候选制品构建、
   板上实测、3 项 callback G7 rider 与 Q3 裁决尚未完成（S1 文书已就绪）。
2. D5 尚未正式并入，S6 v1 已标 superseded；S6 v2 与同 buildroot 的
   S3 21.1.1-2 复验尚未产生。

S4 只是先导 PASS，正式 wave 镜像仍须复跑。D5 并入后的 S6 v2 和同
buildroot S3 复验必须按 `startup_conditions.md` 的“执行期进入序列”产生；
历史 21.1.1-1 PoC 与 S6 v1 均不能替代。
