# neutral 登记册信任树 + v1.4 重冻单点确认(评审方:Claude,2026-07-28)

输入:`p1_exit_review_v7.tar.gz`,SHA256 实测
`52413f783f5de9ee6bc4848da6265bf328372f44e6fc0ea0928debc3b5aa98e2`(吻合);
MANIFEST 全量校验非 OK 行 = 0。

## 裁定:**CONFIRM**

### 1. 登记册方案对原建议语义的忠实度:忠实,且是比 R5 更正确的实现

本方原建议的核心是"三要件**机械证明**"。R5 把三要件做成台账自报列,
GPT r5 指出的自报旁路正是"证明"与"声明"的差距——R6 的修复方向对:

- 三要件证明改为**离线机械证据生成**并逐项 SHA 上锚:`rpm -qpl` 全清单
  (仅目录 + LICENSE_1_0.txt)、逐普通文件四字节 magic 扫描
  `ELF_MAGIC_COUNT=0`(而非信任文件名)、RPM 实算/repodata/冻结表三方
  SHA 同值;三份原始证据 SHA 写入登记册行;
- 台账自报三列**物理删除**(schema/实例/fixture/模板四处),资格唯一
  来源 = 认证登记册,门只验五元身份 + rpm_sha256 精确命中——
  "免闭包不免身份对账"语义保持;
- 亲测分层防御:`neutral_self_declared_boost_program_options` 同时命中
  `NEUTRAL_NOT_IN_TRUSTED_REGISTRY` **和** `ADMIT_CLOSURE_BROKEN`——
  即使 neutral 旁路被尝试,闭包断言仍独立在场。

### 2. 双叶 authority 与 R5 信任树:结构一致,同构扩展

单一固化根(census_input_manifest,SHA `06bd30a0…` 与工具内固定值、
corpus 实算三方一致)→ 每 census 两叶(TIER1 成员表 + neutral 登记册,
登记册 SHA `af06a317…` 实算吻合)→ 叶锚数据。与 R5 的"先验根、再验叶、
漂移即 `CENSUS_INPUT_UNVERIFIED`"完全同构,未引入第二信任入口。

### 3. v1.4 重冻:合规(含链式反应)

- 旧三工具字节不变:edge_diff SHA `3e06d9e5…f74` **四轮独立实测同值**
  (v2/v5/v6/v7);第四门升 v2.1.0,SHA 与报告一致;
- 固化根更新的链式反应闭合:新根 SHA 固化进 v2.1.0 工具 → 工具与两份
  登记册(生产 + fixture)全部纳入冻结清单,142 项实数吻合;
- 全矩阵非增量:42 = 7+13+7+15,5 绿 + 37 红(按 catalog 与报告;本方
  亲测其中第四门关键 6 项)。

### 4. 亲跑结果(含一次自摆乌龙的价值)

| fixture / 实例 | 实测 | 码 |
|---|---|---|
| clean / stdlib_neutral | GREEN/0 | — |
| neutral_self_declared_pkg_a | RED/2 | `NEUTRAL_NOT_IN_TRUSTED_REGISTRY` |
| neutral_self_declared_boost_program_options | RED/2 | 上码 + `ADMIT_CLOSURE_BROKEN` |
| neutral_registry_tampered | RED/2 | `CENSUS_INPUT_UNVERIFIED`(附 expected/actual 摘要) |
| 真实 Base-first 台账 | GREEN/0 | `NEUTRAL_REGISTRY_VERIFIED=YES` |

透明记录:本方首跑 tampered 例得 GREEN,系自身参数回退错拿了共享目录
的**未篡改**登记册——换 fixture 自带篡改文件后按预期红。该乌龙本身
反证了正确行为:门信任的是 authority 链而非文件路径,合法登记册从任何
路径喂入都应绿、篡改件从任何路径喂入都应红,实测两个方向均成立。

### R6-4 方法论条款

"豁免/减免通道与主通道适用同一信任纪律"逐字入 decision_record——
这条把本次教训升为一般规则,今后任何放松型 disposition 天然带资格
认证义务。核准。

## 结论

**CONFIRM**——登记册方案忠实并强化了原建议的机械证明语义;双叶
authority 与 R5 信任树同构;v1.4 重冻合规且链式反应闭合。
P1_EXIT_PASS 总裁定继续成立。
