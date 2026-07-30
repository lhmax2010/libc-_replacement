# 单点确认:neutral 登记册信任树 + v1.4 重冻 + 实例分布保持 — reviewer_kimi

日期:2026-07-28
输入:`p1_exit_review_v7.tar.gz`,SHA256 `52413f783f5de9ee6bc4848da6265bf328372f44e6fc0ea0928debc3b5aa98e2`(一致);包级 MANIFEST 0 失败;corpus HEAD `c61abe4d` 与报告一致。

## 1. 实例分布保持:CONFIRM

三组实例 108/129/438 行不变;ADMIT/NEUTRAL/HOLD 计数与 r5 逐行相等
(18+3/87、12+3/114、147+6/285);ADMIT∪NEUTRAL 集合按
`batch|arch|package|nevra` 与 r5 diff 结果三组均 IDENTICAL。三个自报
proof 列已从全部实例表头物理删除(0 命中),合法 neutral 行仍为
`boost-license` 3/3/6,`decision_evidence` 指向登记册。

## 2. 登记册忠实于"三要件机械证明"语义:CONFIRM

- 资格证据离线机械生成且真实:`rpm -qpl` 仅 2 条路径(目录 +
  `LICENSE_1_0.txt`)、`FILE_COUNT=1`、`ELF_MAGIC_COUNT=0`、RPM 实算
  SHA/repodata `<checksum>`/冻结表三方同为 `39484d87…`;
- 每份证据 artifact 的 SHA 锚入登记册行(qpl/elf_magic/dual_source 三列),
  抽查证据文件实测 SHA 与登记值逐一致;登记册文件 SHA `af06a317…` 与
  authority 叶一致,仓内登记册与证据目录生成件字节相同;
- 门只做精确命中:`(census_id,target_arch,package,rpm_arch,nevra)
  +rpm_sha256` 不全等即 `NEUTRAL_NOT_IN_TRUSTED_REGISTRY`——台账行
  不再能自封 neutral,语义正是"证明离线生成 + SHA 链上锚,门只验命中"。

## 3. 双叶 authority 与 R5 信任树结构一致:CONFIRM

`census_input_manifest.tsv` 现每 census_id 两片叶子(membership +
neutral_registry),生产与 fixture 对称;authority 自身 SHA
`06bd30a0…` 与工具 v2.1.0 内固化根一致(工具 SHA `245d30e2…` 与报告
一致)。链式反应完整:根变 → 工具重冻 → 登记册双叶入
`FREEZE_MANIFEST.sha256`(142 项,亲跑 `sha256sum -c` 0 失败);旧三
工具与 v6 字节相同(cmp 验证),未搭车修改。

## 4. 亲跑复现:CONFIRM

- 三实例过 v2.1 门:均 `CENSUS_VERIFIED=YES` +
  `NEUTRAL_REGISTRY_VERIFIED=YES` + `GATE=GREEN blocking=0`;
- fixture:stdlib_neutral GREEN;neutral_self_declared_pkg_a 红
  `NEUTRAL_NOT_IN_TRUSTED_REGISTRY`;neutral_self_declared_
  boost_program_options 红(NEUTRAL 未命中 + `ADMIT_CLOSURE_BROKEN`
  闭包门保持红);neutral_registry_tampered 红
  `CENSUS_INPUT_UNVERIFIED` 且 `NEUTRAL_REGISTRY_VERIFIED=NO`
  (须以 fixture 自带篡改登记册为输入;我首次误传共享登记册得 GREEN,
  系我方输入错误,非工具缺陷,纠正后与官方 raw log 逐字一致)。

## 5. 治理文档口径:CONFIRM

`decision_record.md` 逐字落"豁免/减免通道与主通道同一信任纪律"条款
及双叶结构说明;`ledger/README.md`、`d5/d5_mechanism.md` 已同步
登记册制;排期/plan 的 ADMIT 集合表述不变(集合未变)。

## 残余(非阻塞)

无。

## 总裁定

**CONFIRM**。登记册方案忠实于三要件机械证明语义;双叶 authority 与
R5 信任树结构一致且固化根更新的链式反应完整;三组实例分布与 r5 逐行
相等;v1.4 重冻合规(142/142,旧工具零改动,42/42 矩阵关键项亲跑
复现)。
