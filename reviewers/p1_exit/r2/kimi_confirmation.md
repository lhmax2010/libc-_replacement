# 单点确认:T1-0008 处置重写 + plan.md 同步 — reviewer_kimi

日期:2026-07-27
输入:`p1_exit_review_v3.tar.gz`,SHA256 `4cc5e814c94c34943403b3c3eff891616ca7837862ef8965ff53537f18450f55`(一致);包级 MANIFEST 全 OK(0 失败);HEAD `2e93c633`。

## 1. T1-0008 处置重写(R1):CONFIRM

- 技术前提成立:`evidence/cross_repo_edges_7_51.tsv` 坐实两条 CPP_ABI
  INTERFACE 边(`capi-appfw-capmgr→boost-program-options`、
  `security-manager→boost-program-options`,样本含 `std::vector<std::string>`/
  `boost::any` 签名,D2_RETAINED,三架构均在 `tier1_production_0008` 内)。
  S4 只裁 UidSandboxing,对这两条 STL 布局边无消解力——旧"S4 PASS →
  boost-program-options 单独 Base-first、三包长尾"分支会把 CPP_ABI 边切成
  跨批边界,GPT 指出的缺陷真实存在。
- 重写后两分支均整分量单元:S4 摘除生效 → 四包退出波 1,作独立跨 repo
  原子批次(Base+Unified 成对晋级);S4 不满足 → 整分量入波 1
  `ACTIVE_BATCH=23/26`。`t1_0008_branch_closure.tsv` 四行 `same_batch=YES`
  与之一致;`decision_record.md` 明禁"任一分支中拆分四个成员"。
- 传播链自洽:`decision_record.md`、`ledger/README.md`、`base_first_6.tsv`
  (5 行)、`package_wave_preassignment.tsv`(四成员均标 "never split from
  T1-0008")、排期(5 包 + companion + T1-0008 独立批次行)口径一致;
  `startup_conditions.md` 的 19/23/26 分支与新处置兼容(23/26 本已含四包),
  无需改动。`shared_main_tier1.tsv` 状态输入未动,符合声明。

## 2. plan.md 同步 + notes 闭合(R2):CONFIRM

- note 1(旧摘要句)CLOSED:头部新增"门禁与启动状态以
  `startup_conditions.md` 为唯一权威,本文冲突处以彼为准";§1.5 与 §8
  均改写为两类红项(14 项签核推迟 + D5 未并入/S6 v1 superseded),并显式
  禁止以历史 PoC repo 或 S6 v1 越过进入序列。§3.4 标 SUPERSEDED、
  21.1.1-2 三架构报告为权威,消除了我上轮未点名的同类隐患。
- note 2(证据路径)CLOSED:`d5/wave1_source_admission.tsv` 证据列已改
  `REF_INDEX.md|sha256=db9adf1f…` 稳定引用,该 SHA256 在
  `REF_INDEX.md:20`(Raw D5 evidence)核实存在。

## 残余(非阻塞,不构成本轮 OBJECT)

- `base_unified/summary.md:34` 仍称 "D-G3's six-package execution list",
  未随 R1 同步为 five-package(该文 September 节已正确标注 "v1-era
  selected list",仅 L34 现在时措辞过期)。R1 九文件范围未含 summary.md,
  属传播遗漏而非决策矛盾;执行链(ledger/schedule/decision_record/
  startup_conditions)均已为 5 包,不会据此产生错误派工。建议下轮随手
  修正,与我上轮 note 1 同类同级。

## 总裁定

**CONFIRM**。两处修订自洽,两条 notes 均闭合;唯一残余为 summary.md:34
的过期数字措辞(非阻塞)。
