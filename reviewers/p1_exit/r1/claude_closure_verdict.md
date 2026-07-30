# P1 出口定点复核裁定(评审方:Claude,2026-07-27)

输入:`p1_exit_review_v2_remediation.tar.gz`,SHA256 实测
`0d924fad75ce503090a6e5d680bd550948a870d7f65bffc2473d59d6cb8466f8`(吻合);
`MANIFEST.sha256` 406 项全量校验非 OK 行 = 0;corpus HEAD
`40e99437e7e34e57e7da2fa543a2eb0f1a2913b6` 与声明一致。
仅核验本方 B1/B2/B3;不重开已 AGREE 面。关键门禁做了**本地实测执行**,
不止于读 diff。

## B1(波次归属治理矛盾)— **CLOSED**

- 消解规则逐字入 [decision_record](corpus/docs/hld/decision_record.md):59
  ("执行单元 = 名单包所在 TIER1 分量闭包;MIXED 分量任一成员开工前必须
  收敛为 SINGLE_LABEL;波 1 三条件裁决结果必须回写台账与 D-G3");
- `SHARED-T1-0010` 机械收敛:boost-log = `BASE_FIRST_COMPANION`,分量
  `SINGLE_LABEL`(台账第 11 行实核)——A6 YES_DIRECT 布局边不再有被拆
  窗口;
- `SHARED-T1-0008` → `PENDING_WAVE1_S4_OUTCOME`(第 9 行实核),两个
  分支的归属结果与回写义务在 decision_record 61-73 行**双向定义**:
  摘除分支的拆分合法性建立在"per-ELF UidSandboxing 对账证明边在生产
  产物中不存在"之上,与消解规则的"证据裁决后拆分"语义一致;S4 前
  fail-closed(任一成员不得开工)保持;
- 排期修正实核:波 1 行改为 **19 MANDATORY / +4 fail-closed=23 / +3
  条件=26**;Base 先行行加 boost-log 同窗伴随;8 月新增 G7 rider 封边
  与 D5 正式并入 + S6 v2 重冻两行(顺带闭合本方 OQ-10)。

执行期不再存在两读空间:名义名单、台账标签、波 1 条件分支、排期四处
同源,冲突态有唯一收敛路径。

## B2(门禁假绿)— **CLOSED(经本地实测)**

对冻结 v1.1 工具的关键 fixture 逐一实际执行(非复读证据):

| fixture | 实测 exit | 实测结果/红码 |
|---|---|---|
| edge_diff clean | 0 | GREEN |
| edge_diff **new_cpp_nostl_edge** | 2 | `NEW_CPP_NOSTL_EDGE` |
| edge_diff **new_layout_promoted_edge** | 2 | `NEW_LAYOUT_PROMOTED_EDGE` |
| edge_diff **signoff_nevra_drift** | 2 | `SIGNOFF_NEVRA_DRIFT`(附 `PROPOSED_REVALIDATE` 处置指令) |
| edge_diff new_cpp_abi_edge(回归) | 2 | 原红仍红 |
| g7 clean | 0 | GREEN |
| g7 **closure_leak** | 2 | `CLOSURE_LEAK_REMAINS` |
| g7 **partial_result** | 2 | `PARTIAL_RESULT_ACCEPTED` |

工具源码实核:edge_diff v1.1 红条件映射含
CPP_NOSTL/LAYOUT_PROMOTED/A8_LAYOUT_PROMOTED/A9_POINTEE_PROMOTED
(edge_diff_check.py:32-35)——原 :90 只认 CPP_ABI 的假绿路径消除。
`verdict_matrix.tsv` 27 项 = 3 GREEN + 24 RED(独立计数吻合);
FROZEN_EXECUTION_VERSION 升 v1.1.0、FREEZE_MANIFEST 重签、
"SHA 变更即解冻全矩阵重跑"纪律保留。

## B3(签核继承未定义区)— **CLOSED**

- 规范条款逐字入 [errata](corpus/docs/hld/HLD_v1.1_errata_2026-07-27.md):52-61
  (Errata 3:签核效力绑定 source-NEVRA;漂移 → 自动降级
  `PROPOSED_REVALIDATE`,重跑源码证据面 diff 后重签;§9 增漂移检测,
  未处置即红);
- `signoff_nevra_bindings.tsv` 384 条绑定(行数实核 385 含表头);
  27 项注册表 + 14 项候选表均增 `bound_nevra` 列(表头实核);
- 检测非纸面:`SIGNOFF_NEVRA_DRIFT` 在冻结工具中实现并经上表实测跑红,
  drift fixture 输出 bound/observed NEVRA 与处置指令。

腐蚀路径(快照滚动 → 边集不变 → 签核静默失效)已被机械门拦截。

## OQ 同批项闭合确认(不影响裁定)

| OQ | 状态 | 依据 |
|---|---|---|
| OQ-1 D-G3 证据包外 | 闭合 | REF_INDEX 增两行(base_first_8_verification / component_diffs,含 SHA 与字节数) |
| OQ-3 新架构锚点豁免 | 闭合 | `ledger/anchor_exemptions_per_arch.tsv`(6 数据行) |
| OQ-4 摘要域规格 | 闭合 | `member_edge_sha256_spec.md`:四元组 `(src,dst,edge_class,stl_kind)` 显式入摘 + 跨类反例 fixture + validation 断言 |
| OQ-8 BASE_FIRST=5 注记 | 闭合 | base_unified/summary.md:32-35 显式解释 boost-iostreams 口径 |
| OQ-9 双语对齐 | 闭合 | `wave4_notice_alignment.tsv` 落盘 |
| OQ-10 rider/D5 排期行 | 闭合 | 排期 8 月两新行(已在 B1 项下实核) |

## 整改引入的新阻塞级问题

**未发现。**C4(startup_conditions 现势化 + D5 准入模板/波 1 source
审计)为增强性变更:S4 升级为 `PASS_PRELIMINARY_LLVM_IMAGE` 时保留
"正式候选镜像必须重跑"的复验义务与 ACTIVE_BATCH 19/23 分支逻辑,未
弱化任何门;D5 准入工件同步落地(兼收本方 OQ-6/7 的一部分)。
CHANGED_FILES 86 文件范围门与装配校验日志自洽。

## notes(残留改进,不影响裁定)

1. G6 三个新 fixture(ambiguous_registry/registry_expired/
   registry_not_signed)本方未逐一实测,依据 verdict_matrix 与
   gate_v1_1 证据目录采信;下轮任一解冻重跑时顺带留原始输出即可。
2. T1-0008 的 S4 回写发生后,记得同步 `base_first_6.tsv` 的名义名单
   注记(decision_record 已定义义务,执行时勿漏)。
3. OQ-2(A6/A9 发现扫描重跑节律)按 C6 入 METHOD/errata,节律执行的
   首个到期点在核心波开工前——列入 10 月检查单。

---

## 总裁定:**P1_EXIT_PASS**

三项阻塞全部真实闭合(其中门禁修复经本地执行验证,非文档采信);
无新增阻塞。P1 可关账,执行期可开工;三项 G7 rider、S1 签核、S4 正式
镜像复验等执行期硬门按卷宗继续持有,不因本裁定转绿。
