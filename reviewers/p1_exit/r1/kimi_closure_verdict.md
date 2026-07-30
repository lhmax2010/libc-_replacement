# P1 出口定点复核(closure)裁决 — reviewer_kimi

日期:2026-07-27
输入:`/home/toolchain/governance/p1_exit_review/closure/p1_exit_review_v2_remediation.tar.gz`
SHA256:`0d924fad75ce503090a6e5d680bd550948a870d7f65bffc2473d59d6cb8466f8`(与声明一致)
MANIFEST:406 项,`sha256sum -c` 全 OK;corpus HEAD `40e99437`(与 `CORPUS_HEAD.txt` 一致)。
范围:仅核验我方 P1 出口评审阻塞项 #1/#2/#3 的真实闭合 + OQ 两项顺带确认;未重开已 AGREE 面。

---

## 阻塞项 #1(D5 未落地)→ C4:CLOSED

依据:

- `wave1/startup_conditions.md:89` S6 状态已改 `PASS_V1_SUPERSEDED`,并明确
  "在 v2 的 buildconf SHA、repo、宏展开和 OFF_PROVEN 证据落盘前,v1 不得作为开工 PASS"
  ——旧 S6 v1 作为开工依据的通路被文书封死。
- 同文"执行期进入序列"(L113–127)为 fail-closed 唯一路径,顺序为 D5 并入 →
  S6 v2 重冻 → 候选构建 → 板测 → 签核 → allowlist 正式化 → 五步验收,且声明
  "不得并行越过前项"。D5 并入为第 1 步,OQ-5 全平台 `rpmspec -P` 空 allowlist
  差分与复杂包负对照写为该步前置子项,未冒充已执行。
- 当前总判定 `NOT_READY`(L131),启动红项列为两类,第 2 类即"D5 尚未正式并入,
  S6 v1 已标 superseded"。状态标记自洽,不再同时声称"S6 PASS"与"D5 待并入"。
- `d5/d5_mechanism.md` 保持诚实:PoC 未改原始 `gbs_llvm.conf`/S6 config(L45、
  L214–219),正式采用需两处外部状态变化;S6 兼容性预判(L118–130)正确指出
  命令字节可不变但 buildconf SHA 变化必须重冻 S6 v2。
- 新增 `d5/wave1_source_admission.tsv`(5 个 wave1 source 逐项静态归档/自定义
  链接/混合 C/C++ 检查,2 个混合包标 `PASS_WITH_MIXED_SUBPACKAGE_REVIEW_REQUIRED`)
  与 `d5/admission_check_template.tsv`(D5-A01/A02/A03 均带 blocking_action),
  与整改报告证据一致。
- 全 corpus 检索无残留的裸 "S6 PASS" 声明(grep 排除 SUPERSEDED/PRELIMINARY 后
  零命中);排期 8 月行已含"D5 正式并入 + S6 v2 重冻(波 1 开工首步)"。

残留见末尾 notes(plan.md 两处旧摘要句),按判定纪律不构成 NOT_CLOSED。

## 阻塞项 #2(D-G1 新边 triage 缺位)→ C1+C6:CLOSED

依据:

- Errata 3(`HLD_v1.1_errata_2026-07-27.md:52–61`):签核效力绑定 provider/
  consumer source-NEVRA,漂移自动降级 `PROPOSED_REVALIDATE`,边差分门增
  `SIGNOFF_NEVRA_DRIFT` 阻塞码。落点 `wave1/signoff_nevra_bindings.tsv`
  实测 384 条绑定行,0 条 DRIFT/MISMATCH。
- Errata 4(L63–68):执行快照新增 CPP_NOSTL 边与消失边必须经"机械 diff 清单 +
  人工 triage(升格候选/登记豁免/裁决安全)"后方可发波;A6/A9 发现扫描在每次快照
  对齐后、核心波开工前各重跑一次。正是我方开出的规则。
- Errata 5(L70–75):A8/A9 仅为冻结证据的条件回放,"不能外推为新快照不存在新增
  隐藏耦合"——"校准门复现即管线等价"的越界宣称已收缩;`REF_INDEX.md:27–31`
  将 5 个 census v2 实现脚本按绝对路径 + SHA256 锚定。
- 机械验证(我方亲跑,非引报告):`gates/tools/edge_diff_check.py` v1.1 对 7 个
  fixture 实测 `clean=GREEN`,`new_cpp_abi_edge`/`new_cpp_nostl_edge`/
  `new_layout_promoted_edge`/`removed_edge_registry_active`/`signoff_nevra_drift`/
  `soname_change_undeclared` 各红且红码正确(NEW_CPP_NOSTL_EDGE /
  NEW_LAYOUT_PROMOTED_EDGE / SIGNOFF_NEVRA_DRIFT 等,非 incidental 红)。
  与 `gates/verdict_matrix.tsv` 27/27 PASS 一致。
- 新快照上新增边因此同时被文书(发波前置 triage)与门禁(红码阻塞)双层覆盖,
  校准快照等价不再能静默外推。

## 阻塞项 #3(排期 12 包)→ C5:CLOSED

依据:

- `docs/schedule/libcxx_migration_schedule.md:38` 波 1 行已改
  "19 MANDATORY(+4 fail-closed/+3 条件,批次 19/23/26)",与 `wave1/plan.md`
  条件分支口径一致;变更记录 L16 注明 C5 修正。
- 全文检索无残留 "12 包" 波 1 口径(排除 2026-12/月份/118/121/122 后零命中)。
- 排期同时吸收 D5+S6 v2(8 月首步)与 G7 rider 封边补丁(8 月,P2 镜像前硬门),
  与 startup_conditions 进入序列同序。

## OQ 顺带确认(不影响裁定)

- **fallback 边差分句:CLOSED**。`wave4_notice_CN_release.md:40–41`"fallback
  镜像仍须过边差分门验证无跨域 C++ ABI 边";EN 版 L47 同义;
  `wave4_notice_alignment.tsv` 8 段对齐 PASS。过度承诺已消除。
- **MIXED 消解规则:CLOSED**。`ledger/README.md:40` 与 `decision_record.md:59`
  落同一规范规则:MIXED_PACKAGE_LABELS 分量在任一成员开工前必须收敛 SINGLE_LABEL
  (伴随改标 `<主标签>_COMPANION` 或经 D1a/A6 证据裁 shim 拆分)。
  `SHARED-T1-0010` 已收敛(`boost-log=BASE_FIRST_COMPANION`,与 9 月 Base 同窗);
  `SHARED-T1-0008` 标 `PENDING_WAVE1_S4_OUTCOME` 且 decision_record L87 明确
  "不得提前按任一分支记为 PASS"。9 月发波阻塞面已有确定消解路径。

## 整改引入的新阻塞级问题

无。整改范围由 CHANGED_FILES.tsv 限定(86 文件),两级 diff 门(86/86、42/42)
与我方抽查一致;未发现整改越界改动已 AGREE 内容或引入新矛盾。

## notes(非阻塞,不影响裁定)

1. `wave1/plan.md` L197 与 L580 仍保留旧摘要句:"当前启动红项收敛为一类:14 项
   API 未三方签核",并仅以"命令逐字 SHA 一致"描述 S6 约束——与
   `startup_conditions.md` 的两类红项(D5 并入 + S6 v2 重冻)不一致。不判
   NOT_CLOSED 的理由:plan.md L569 自身指定 `startup_conditions.md` 为启动硬门
   文档,§8 自述 NOT_READY 且"不表示 wave 已获准",fail-closed 进入序列无旁路。
   但开工前必须把这两句同步为两类红项口径,消除执行者单读 plan.md 误判 READY 的
   残余风险。
2. `d5/wave1_source_admission.tsv` 证据列引用的是评审区绝对路径
   (`/home/toolchain/governance/p1_exit_review/remediation/evidence/...`),
   建议执行期转为仓内或带 SHA256 的稳定引用,避免评审区清理后证据悬空。

---

总裁定:**P1_EXIT_PASS**
