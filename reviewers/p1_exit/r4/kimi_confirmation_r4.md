# 单点确认:R4 台账实例与四处同源保持 — reviewer_kimi

日期:2026-07-28
输入:`p1_exit_review_v5.tar.gz`,SHA256 `d379012baf8206d976a6f5e121caf72709fd2914f5d6ae34f392cbdbb97e541c`(一致);包级 MANIFEST 0 失败;corpus HEAD `a0727098` 与报告一致。

## 1. ADMIT 7 项口径四处同步:CONFIRM

Base-first 每架构 ADMIT = 6 个生产/C++ 输出 + 同源 noarch
`boost-license` = **7 项**,四处一致:

- `decision_record.md`:Boost 全输出口径修正为含 noarch 的 33/30/33;
  "共四个 C++ 输出;两种晋级场景均另 ADMIT … `boost-license`,并 HOLD
  `boost-doc-html`";Base-first 第 4 条 ADMIT 清单含 `boost-license`;
- 排期:Base 先行波行改 "6 个生产 ADMIT 输出 + `boost-license`",变更
  记录补 noarch 口径;
- 台账:`promotion_ledger_basefirst.tsv` 实测每架构 ADMIT 恰 7 行
  (filesystem/iostreams/log/thread + libsigc++ + taglib + license);
- `plan.md`(23/26 分支 ADMIT 含 `boost-license`)与 `summary.md`
  (six production ADMIT + noarch `boost-license` ADMIT、doc-html HOLD)。

`boost-license` 由"外部 provider"改判同源 ADMIT 有独立证据
(NEVRA/SOURCERPM/SHA 复验,文件清单仅 License 文本、非 ELF),且
T1-0008(5 项=4 成员+license)与波 1 23/26(24/27 项)口径同源推导,
未发现某处仍按旧"外部 provider"记账。

## 2. 三组实例与 D-G4 规则抽验:CONFIRM(亲跑,非引报告)

- 行数:108/129/438 数据行(basefirst/t1_0008/wave1_cond),与报告一致;
  ADMIT 分布 7/5/24/27 每架构,HOLD 87/114/285 与 authority 行数一致;
  全部 HOLD 行 `legacy_authority_sha256` 非空(0 漏绑)。
- 亲跑 `promotion_ledger_check.py`:三组实例 exit=0 **GREEN**;
  六 fixture 中 clean GREEN,missing_output/duplicate_row/
  hold_authority_drift/admit_closure_broken/exact_lock_split 各红且红码
  正确(LEDGER_MISSING_OUTPUT / LEDGER_DUPLICATE_ROW /
  HOLD_AUTHORITY_DRIFT / ADMIT_CLOSURE_BROKEN / EXACT_LOCK_SPLIT),
  与 v1.2 矩阵断言逐条相符;空输入 exit=3 fail-closed 路径存在。

## 3. r3 note(26 分支回写义务)落账:CONFIRM,完整

- `decision_record.md` D-G4 新增跨批 legacy authority 交接二选一规则
  (重验闭包再 ADMIT / 以前批 NEVRA+SHA 为新 authority 并 HOLD),并禁止
  两批对同一输出各持不同 authority;
- `plan.md:472-475` 明文:26 分支生效时必须回写 Base-first 晋级台账,
  逐输出显式二选一,"不得留空";
- 变更记录(plan L27)登记该义务来源。落账完整,无只点名未机制化。

## 残余(非阻塞)

无。R3→R4 唯一结构性变化(noarch 纳入输出域、license 重分类)已同步到
全部治理面;census 验证器"禁止硬编码预期"的教训落账方式正确(由采集表
推导输出域)。

## 总裁定

**CONFIRM**。ADMIT 7 项口径四处同步一致;三组台账实例与 D-G4 规则
抽验相符(门禁亲跑复现);26 分支回写义务已完整落入 D-G4 与 plan.md。
