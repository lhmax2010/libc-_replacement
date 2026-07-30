# 单点确认:三组台账 v2 迁移与四处同源保持 — reviewer_kimi

日期:2026-07-28
输入:`p1_exit_review_v6.tar.gz`,SHA256 `b58e564b594a02446494e5f31b02ac4d0e3795301dc3f68f72ace49605876c37`(一致);包级 MANIFEST 0 失败;corpus HEAD `72f4ff61` 与报告一致。

## 1. 迁移前后分布不变:CONFIRM

逐行实测三组 v2 实例,与 r4 的差仅在 schema 与 license 类型:

| 实例 | r4 ADMIT | v2 ADMIT + ADMIT_STDLIB_NEUTRAL | HOLD |
|---|---|---|---|
| Base-first | 7/arch(21) | 6+1 /arch(18+3=21) | 87(不变) |
| T1-0008 S4-PASS | 5/arch(15) | 4+1 /arch(12+3=15) | 114(不变) |
| wave1 23/26 | 24/27 per arch(153) | 同(147+6=153) | 285(不变) |

NEUTRAL 行恰为 `boost-license`(basefirst/t1_0008 各 3 行、wave1 两条
分支共 6 行),无任何其他输出的 disposition 发生变化;镜像准入集合
(ADMIT∪NEUTRAL)与 r4 逐行相等。

## 2. ADMIT 口径四处同源:CONFIRM

- `decision_record.md`:NEUTRAL 三要件(非 ELF/无 C++ 面/双源 SHA 证明)
  、五元身份键、SHA 三方绑定、双向完备与"断言数据源必须独立且经 digest
  认证"方法论条款均已落;T1-0008 与 Base-first 分支表述同步改用
  `ADMIT_STDLIB_NEUTRAL`;
- `ledger/README.md`、`d5/d5_mechanism.md`、`admission_check_template.tsv`
  与 v2 schema 同步;
- 排期与 `plan.md` 沿用 umbrella 表述 "ADMIT 同源 `boost-license`"——
  NEUTRAL 是 ADMIT 的受限子类(decision_record:92),集合与规则无歧义,
  口径同源。

## 3. 亲跑复现:CONFIRM

- 三实例过 v2.0.0 门(含 census 认证输入):均
  `CENSUS_VERIFIED=YES` + `GATE=GREEN blocking=0`;
- census 信任链抽查:合并表 SHA `449b3fb1…` 与
  `census_input_manifest.tsv` 登记一致;源清单中 armv7l 成员表 SHA
  `e14b008c…` 与工作区只读权威文件实测一致(认证链非伪称);
- 六项关键 fixture 亲跑:forged_membership / census_digest_mismatch 均
  红 `CENSUS_INPUT_UNVERIFIED`(伪造成员表与伪造 authority 均不可消费);
  ghost_ledger_row 红 `LEDGER_ORPHAN_ROW`;payload_identity_mismatch /
  admit_image_drift 各红对应码;stdlib_neutral 合法绿。与 v1.3 矩阵
  (第四门 2 GREEN + 10 RED)逐条相符。

## 残余(非阻塞)

无。

## 总裁定

**CONFIRM**。v2 迁移仅改变 schema 与 license disposition 类型,
ADMIT/HOLD 分布与 r4 逐行相等;ADMIT 口径在四处治理文档保持同源;
三实例过门 GREEN + CENSUS_VERIFIED=YES 已亲跑复现,伪造 census 输入
的两个负例均正确报红。
