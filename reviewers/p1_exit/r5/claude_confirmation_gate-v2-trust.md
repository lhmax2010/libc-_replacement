# 第四门 v2 信任模型 + v1.3 重冻单点确认(评审方:Claude,2026-07-28)

输入:`p1_exit_review_v6.tar.gz`,SHA256 实测
`b58e564b594a02446494e5f31b02ac4d0e3795301dc3f68f72ace49605876c37`(吻合);
MANIFEST 全量校验非 OK 行 = 0。

## 裁定:**CONFIRM**

### 1. 信任模型重写符合 R5-4 方法论条款(逐要件对照 + 实测)

R5-4 条款(数据源独立 + digest 认证 + 自报字段仅交叉核对 + 双向完备)
与 v2.0.0 实现逐项对上,且关键面全部本地亲测:

| 要件 | 实现 | 本方实测 |
|---|---|---|
| 双向完备 | 正向缺行 + 反向幽灵行 | `ghost_ledger_row` → RED `LEDGER_ORPHAN_ROW` ✓ |
| 五元身份 + SHA 三方对账 | (batch,arch,pkg,rpm_arch,nevra) 连接;manifest↔ledger↔镜像实选三方 SHA | `payload_identity_mismatch` → RED;`admit_image_drift` → RED ✓ |
| census 输入独立认证 | 自报 `tier1_component_members` 废弃;闭包唯一来源 = 认证成员表,工具内固化 authority SHA,两级 digest 先验后用 | `forged_membership`/`census_digest_mismatch` → RED `CENSUS_INPUT_UNVERIFIED` ✓ |
| 旧断言面回归 | v1.2 五红保持 | `hold_authority_drift` → RED ✓;clean → GREEN ✓ |

**一条计划外活证**:本方以臆造 census-id 跑真实 Base-first 台账,门立即
`RED: membership rows contain an unexpected census_id`——身份校验对
"拿错数据集"这一最隐蔽的假绿路径真实拦截;换用证据中的
`tizen-0723-production-tier1-v1` 后 `CENSUS_VERIFIED=YES / GATE=GREEN`
(与报告三实例声明一致)。另值得记录的诚实点:0723 census 上游本无
覆盖三张成员表的根 MANIFEST,R5 明写了认证链的建立过程(三源只读表 +
出处/SHA/行数落盘 + gate 专用 authority),没有伪称既有证据。

### 2. ADMIT_STDLIB_NEUTRAL 落地与建议语义一致:免闭包,不免身份对账

- D-G4 条款:定义为 **ADMIT 的受限子类**,三要件与本方建议逐条对应
  (`neutral_non_elf_proof` / `neutral_no_cpp_surface_proof` /
  `neutral_dual_source_sha_proof`),仅免 TIER1 分量闭包断言;
- 代码实核:neutral 行走与 ADMIT 相同的身份/SHA 检查块
  (promotion_ledger_check.py:309),镜像漂移检查包含 neutral(:478),
  三证明列任一非 PASS 即 `STDLIB_NEUTRAL_EVIDENCE_INCOMPLETE`(:323-332)
  ——比"逐例注记"多出了证据不全红码,超出建议;
- `stdlib_neutral` fixture 亲测 GREEN;三实例 boost-license 全部改用该
  类型,boost-doc-html 保持 HOLD ✓。

### 3. v1.3 重冻:合规

- **全矩阵非增量**:39 = G6 7 + G7 13 + edge_diff 7 + 第四门 12;
  verdict_matrix 独立计数 5 GREEN + 34 RED 与报告一致;
- **旧三工具字节不变**:edge_diff SHA `3e06d9e5…f74` 三连持平(本方
  v2/v5/v6 三轮独立实测同值);G6/G7 版本号未动,一级 diff 门断言范围内;
- **130 项冻结清单**:实数 130 行吻合;`gates/census_input_manifest.tsv`
  实存于 corpus 并按报告纳入冻结对象——认证 authority 随执行版本一体
  冻结,这一步补齐了"工具冻结但其信任根不冻结"的残留空隙。

## 结论

**CONFIRM**——信任模型重写与 R5-4 方法论条款一致且经八 fixture +
真实台账 + 一次臆造身份的对抗输入全部实测;NEUTRAL 判据落地忠实于
建议语义并加严;v1.3 重冻合规。P1_EXIT_PASS 总裁定继续成立。
