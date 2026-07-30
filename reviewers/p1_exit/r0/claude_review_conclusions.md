# P1 出口评审结论(评审方:Claude,2026-07-27)

输入:`p1_exit_review_v1.tar.gz`,SHA256 实测
`1ca767dfb826291e7e9b118521b59d3595c99db6aeb865a2512a5d83b57712ec`(吻合);
`MANIFEST.sha256` 170 成员全量校验非 OK 行 = 0。
基于包内证据评审;PRIOR_UNANIMOUS_PASS 未重审,仅在与新增内容矛盾处如实报。

## 总判定:**FAIL(3 项阻塞;均为文档/工具级修复,估计单日量级,修复后仅需定点复核)**

三项阻塞的共性:每一项都会在执行期(9 月 Base 先行波 / 首次快照滚动 /
首次波间差分)造成错误决策,且修复路径唯一明确。P1 主体工作(census v2、
台账机械层、D5 机制、wave1 卷宗、通告与排期骨架)质量高,不在阻塞面内。

---

## 阻塞项清单

### B1【波次归属治理矛盾:D-G3 六包 × 台账 MIXED × wave1 条件分支 × 排期,四文档无消解规则】

可复现路径(全部为包内数据):

1. `ledger/shared_main_tier1.tsv` 第 9 行:`SHARED-T1-0008` 分量 =
   {boost-program-options(**BASE_FIRST**), capi-appfw-capmgr,
   libsecurity-manager-client, security-manager(均 **LONG_TAIL**)},
   4 包 3 条 TIER1 内部边,`MIXED_PACKAGE_LABELS` 无消解;
2. 同文件第 11 行:`SHARED-T1-0010` =
   {boost-filesystem(**BASE_FIRST**), boost-log(**LONG_TAIL**),
   boost-thread(**BASE_FIRST**)}——其中 boost-log→boost-thread 是冻结
   A6 台账的 **YES_DIRECT 布局耦合边**(`boost::detail::thread_data_base`);
3. `docs/hld/decision_record.md` D-G3 把六包定为 Base 先行执行名单,
   未提分量闭包;`docs/schedule` 9 月行承诺"Base 先行波(6 包)",
   "Unified 消费者按台账跟进重编"未区分"重编"与"同窗迁移";
4. `wave1/wave1_expanded_packages.tsv` 又把 boost-program-options/
   security-manager 分量按三条件**放进波 1**(fail-closed),boost 三包为
   CONDITIONAL_BRANCH——同一分量在三份 NEW_SINCE_FREEZE 文档中有三种
   归属,条件裁决结果与 D-G3/台账如何回写无规则。

失效场景:执行 owner 按 D-G3+排期在 9 月单独迁移 boost-filesystem/
boost-thread 而 boost-log 留 libstdc++ → 拆开确证布局耦合分量 → 运行期
ABI 断裂,这正是全项目要防的第一类事故。

修复(文本级):D-G2/D-G3 增补一条消解规则——"执行单元 = 名单包所在
TIER1 分量闭包;MIXED 分量在其任一成员开工前必须收敛为 SINGLE_LABEL
(伴随成员改标 `*_COMPANION` 或经 D1a/A6 证据把耦合边处置为 shim 后
拆分);wave1 三条件裁决结果必须回写台账与 D-G3"。同批修复排期同步错误:
9 月 Base 先行行加"含 boost-log 同窗迁移(或 shim 裁决)";8 月波 1 行
"12 包"改为分层口径 **19 MANDATORY / +4 fail-closed = 23 / +3 条件 = 26**
(与 `wave1_expanded_packages.tsv`、`startup_conditions.md`、census v2
cynara19 回归的 19 口径对齐;12 已是三轮前的旧数)。

### B2【冻结执行版门禁漏检:edge_diff 放行新增 CPP_NOSTL 边;G7 两个断言面无红 fixture】

可复现路径:

1. `gates/tools/edge_diff_check.py:90`——新增边仅当
   `edge_class == "CPP_ABI"` 才产生 `NEW_CPP_ABI_EDGE` 红;构造 after
   边表加一条 `CPP_NOSTL` 新边(拿 `new_cpp_abi_edge` fixture 改
   edge_class 一词即可复现),门 **GREEN**。冻结 §9 设计明文"新增…未审
   `CPP_NOSTL`…均 FAIL";CPP_NOSTL 恰是 A6/A8/A9 全部隐藏布局故事的
   载体类,版本升级引入新隐藏耦合边(§9 双触发机制的存在理由)将被
   FROZEN_WAVE_EXECUTION_V1 静默放行;
2. fixture 矩阵(13 项)没有 new_cpp_nostl_edge 红例——"该红但没有
   fixture"的缺口使 13/13 全过给出虚假完备感;
3. `g7_exception_gate.py` 实现了 `CLOSURE_LEAK_REMAINS` 与
   partial_result 断言(:90-99),但矩阵中无 closure_leak / partial_result
   红 fixture——两个断言面从未被负面对照证明会红,违反本项目自设的
   "真实红→绿"门禁上线纪律。

修复:工具一处条件扩展(CPP_NOSTL/已升格布局边入红)+ 新增 3 个红
fixture + 按冻结纪律(SHA 变更自动解冻)重跑全矩阵 16/16。

### B3【D-G1 未定义区:已签核裁决与快照 NEVRA 漂移之间无继承规则】

D-G1 确立"方法论冻结、数据现势化",校准门治理**图管线复现**;但已签核
的裁决语料(S2 的 27 项 SIGNED、G7 台账、锚点豁免)全部以精确 NEVRA 为
证据硬门(askuser 0.8.4-1 / pkgmgr-info 0.37.2-1 / webapi 2.92-0)。
快照滚动后的腐蚀路径:pkgmgr-info 0.37.2-1 → 0.38.x,UND/边集不变 →
edge_diff GREEN、校准门无涉、S2 注册表仍 SIGNED——而签核所依据的源码级
confinement 证据(borrowed 指针、user_data 透传、Label view)对新版本
**未被任何门重验**。新版实现改一个 getter 的所有权语义即无声失效。
`startup_conditions.md` 对制品有"不同制品不得继承 PASS"(S3),对签核
裁决没有等价规则;校准门作为唯一前置**不充分**。

修复(规则一条):签核记录增加 provider/consumer source-NEVRA 绑定列;
§9 差分增加 NEVRA 漂移检测,签核相关包漂移 → 对应签核行自动降级
`PROPOSED_REVALIDATE`(轻量:对新版本重跑该 API 的源码证据面 diff),
写入 D-G1 勘误页。

---

## 第一层逐项判定(NEW_SINCE_FREEZE)

| 面 | 判定 | 摘要 |
|---|---|---|
| a. D-G1 快照治理 | **DISAGREE** | 核心成员的"冻结基线+现势化+校准门"框架本身成立且诚实;未定义区在裁决继承(B3)。另:方法论移植是"证据回放"而非"发现重跑",见 c 与 OQ-2 |
| b. D-G2 台账分层 | **DISAGREE(仅消解规则缺失)** | member_edge_sha256 机械层质量高:互斥/完备/计数 8 断言全 PASS,1520+43/46/41 与 census 总数精确对账;MIXED 仅 2 个分量但恰好命中 9 月 Base 先行名单(B1)。digest 是否含 edge_class 未在包内证明(OQ-4) |
| c. census v2 | **AGREE(附边界声明要求)** | 校准门为成员级+四分区全复现,方法等价证明逐规则落盘,诚实度高。所求反例存在但已被承接:A8/A9 是**冻结证据边的条件回放**,不重跑 A6/A9 发现扫描——校准快照上与原方法等价(证据=该快照的发现输出),新快照上新生隐藏布局边系统性不入 TIER1。补偿网(TIER2 上界 + D1a 波界复审 + 隧道登记册)是冻结强制项,故不构成阻塞;但 METHOD.md 必须明写此边界(OQ-2),否则"校准门复现即管线等价"会被误读为发现完备 |
| d. D5 机制 | **AGREE(采用前置见 OQ-5/6/7)** | 机制选型论证充分(判定单位=source Name 正确;不走 --define 保 S6 字节;shell token 对合法 RPM 名字符集安全);三组对照+G1–G5+三架构仓验证成立;文档自设 ADOPTION_BLOCKED 停止点纪律好。阴性对照仅 1 个合成包,正式写入 buildconf 前必须补全平台 spec 解析级负对照(OQ-5)与 -nostdlib/自定义 ld 包准入检查(OQ-6) |
| e. 门禁工具 | **DISAGREE** | B2。除此之外 13 项矩阵设计(每断言一红例、冻结即 SHA 锁、解冻全重跑)方向正确 |
| f. wave4_notice | **AGREE** | 数字全对(118/121/122;Base 3 包三架构精确一致;115/118/119 算术自洽);130 与现值的双口径披露诚实;"Base 可先构建、promotion 同窗"技术准确;兜底方案(双运行时不翻转)如实。未见过度承诺。EN 版仅抽验(OQ-9) |
| g. 排期+packaging | **DISAGREE(仅 B1 的同步错误)** | 排期骨架与 D-G1/D-G3 衔接正确(校准门前置、0723 参考值、兜底);wave1 行 12 包为陈旧口径(并入 B1 修复)。packaging 报告与 S3/D5 完全一致(21.1.1-2、@@LLVM_21、8/8 RPM) |

## 第二层一致性发现

1. **数字口径**:130/118/121/122 在 decision_record、errata、census v2、
   wave4 双语通告、排期中全部一致 ✓;台账 1520+43/46/41 与 census 分量
   总数 1563/1566/1561 精确对账 ✓;批次 19/23/26 在 wave1 卷宗、
   startup_conditions、base_unified 标签(WAVE_1=19)间一致 ✓——唯一
   失同步是排期的"12 包"(B1);
2. **同一分量三处归属**(wave1 条件分支 / D-G3 / 台账 MIXED)无回写
   规则(B1);
3. **D-G3 证据链在包外**:decision_record 与 base_unified/summary 引用的
   `base_first_8_verification.tsv`、`base_first_component_diffs.tsv` 不在
   corpus,也不在 SOURCE_MAP/REF_INDEX 中(SOURCE_MAP 仅含
   base_first_6.tsv)——D-G3 摘除 boost-test/移出 abseil-cpp 的依据不可
   在包内复核,EVIDENCE_GAP(OQ-1);
4. errata 与冻结正文的引用关系(§2/§5/§9/§10)抽验自洽;errata"不回写
   正文"的治理形态与冻结纪律一致 ✓;
5. 锚点从 armv7l 5 个扩到 aarch64 9 / x86_64 11(rust/jdk/libedgetpu/
   dotnet 新增)——census v2 证据新鲜度绑定 ✓,但新锚点的 D2 豁免台账
   处置行未见(OQ-3)。

## 复核路径

三项阻塞修复后,无需第四轮全量:B1 核对四文档的消解规则与回写文本;
B2 核对工具 diff + 16/16 矩阵输出;B3 核对勘误页新规则一段。定点复核
半日内可完成。
