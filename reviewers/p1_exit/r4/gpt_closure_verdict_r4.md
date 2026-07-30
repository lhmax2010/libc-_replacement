# P1 出口定点复核 r4

评审日期：2026-07-28  
输入：`p1_exit_review_v5.tar.gz`  
输入 SHA256：`d379012baf8206d976a6f5e121caf72709fd2914f5d6ae34f392cbdbb97e541c`  
corpus HEAD：`a0727098b1e6d38de53d3601e798dfe3ccca6f0f`

总裁定：`P1_EXIT_STILL_FAIL`。关闭条件 1、2、4 CLOSED；关闭条件 3
NOT_CLOSED；B-02 NOT_CLOSED；D-G4 DISAGREE。

## 完整性前置

`p1_exit_review_v5.tar.gz` 实算 SHA256 与给定值一致。归档共有 614 个文件：
顶层 `MANIFEST.sha256` 的 611 行覆盖除三个自校验 MANIFEST 外的全部
611 个 payload，实跑 611/611 OK；`corpus/MANIFEST.sha256` 254/254 OK；
`remediation_r4/MANIFEST.sha256` 346/346 OK。完整性前置通过。

## 关闭条件 1：CLOSED

结论：共享 noarch 输出已纳入 source 输出域，R3 的 31/28/31
架构目录口径已被 33/30/33 全 source 口径替换，未再复现原漏项。

依据：

- `remediation_r4/evidence/noarch_independent_verification.tsv:2-3` 的
  `boost-license`、`boost-doc-html` NEVRA 均为 `1.83.0-5.1.noarch`，
  SOURCERPM 均为 `boost-1.83.0-5.1.src.rpm`；实算 RPM SHA256 分别为
  `39484d87349fe00b7dbc9382518cb258b694d3126d0c64358da3e3abf3871cf0`
  和
  `434a09ae5959c89d3fcf9014c02748786e10268148db0be72e58ddfc7b61fe6a`，
  与 repodata 及 R3 取证值一致。`boost-license` 文件清单只有许可证文本。
- 对包内 Base/Unified 完整 primary metadata 独立遍历，四个相关 source
  的六个精确 source-NEVRA 均无 noarch 输出；结果与
  `remediation_r4/evidence/related_source_noarch_check.tsv:2-7` 一致。
- `corpus/ledger/boost_source_unit_census.tsv:9,14,41,47,74,80` 已为每个
  目标架构登记两项共享 noarch 输出；独立重算得到 armv7l 31+2=33、
  aarch64 28+2=30、x86_64 31+2=33，且每架构包名唯一、全部归属同一
  Boost SOURCERPM。结果与
  `remediation_r4/evidence/source_output_domain_counts.tsv:2-4` 一致。
- `remediation_r4/evidence/validator_r3_to_r4.diff` 与 R3 原验证器到 R4
  修正版的实算 diff 一致；修正版从 census 重算 arch/noarch/all 三个
  计数，不再使用旧 31/28/31 和 `EXTERNAL_PROVIDER` 断言，实跑 exit 0。

## 关闭条件 2：CLOSED

结论：R3 指出的特定假绿——source 命中、candidate manifest 已含 noarch
输出、但晋级台账漏该行——现在会被 `LEDGER_MISSING_OUTPUT` 拦截。

重放结果：

| 输入 | 实际结果 |
|---|---|
| clean | GREEN，exit 0 |
| missing_output | `LEDGER_MISSING_OUTPUT`，RED，exit 2 |
| duplicate_row | `LEDGER_DUPLICATE_ROW`，RED，exit 2 |
| hold_authority_drift | `HOLD_AUTHORITY_DRIFT`，RED，exit 2 |
| admit_closure_broken | `ADMIT_CLOSURE_BROKEN`，RED，exit 2 |
| exact_lock_split | `EXACT_LOCK_SPLIT`，RED，exit 2 |
| header-only manifest | `INPUT_ERROR`，exit 3 |
| 非空 authority 表缺少某个 HOLD key | `HOLD_AUTHORITY_DRIFT`，RED，exit 2 |

五红一绿的门色、红码和 exit 0/2/3 语义均与
`corpus/gates/verdict_matrix.tsv:29-34` 一致。本项只确认上述既定失效路径；
工具整体 fail-closed 性见关闭条件 3。

## 关闭条件 3：NOT_CLOSED

结论：`promotion_ledger_check.py` 仍不是 fail-closed 门，且其 ADMIT
闭包断言没有认证 census 分量数据。以下三个构造均从 clean fixture
做单点变更并实跑得到 `GATE=GREEN`、exit 0。

### 3.1 manifest 外的台账行被静默忽略

`corpus/gates/tools/promotion_ledger_check.py:67-68` 只以
`(batch_id, arch, binary_package)` 为键；`:132-134` 对 manifest 中不存在的
ledger key 直接 `continue`。在 clean ledger 追加一个 manifest 外
`ghost` ENTRY，即使其 disposition/闭包状态为非法值，实际仍为
GREEN/exit 0。

失效路径：`wave1/plan.md:455-461` 要求镜像装入台账全部 ADMIT candidate；
因此 manifest 未登记的 ADMIT 行可进入镜像，而第四门不判红。其 source
输出全集、分量归属和 payload 身份均未经门验证。

### 3.2 noarch/架构身份和 candidate payload 未绑定

工具的连接键不含 `rpm_arch`、binary NEVRA 或 RPM SHA；LEDGER 必需字段
甚至不含 `rpm_arch`（`promotion_ledger_check.py:27-40`）。工具也没有比较
manifest `rpm_sha256` 与 ledger `candidate_rpm_sha256`，ADMIT 的
`image_selected_sha256` 同样未与 candidate SHA 比较。

可复现反例：clean manifest 的 `pkg-doc` 是 `rpm_arch=noarch`、SHA=`c…c`；
把唯一 ledger 行改为 ADMIT 且 candidate/image SHA=`e…e`，原 manifest
不变，实际仍为 GREEN/exit 0。等价地，同一 target arch 下的同名
arch-specific/noarch 输出会折叠到同一个键。故“noarch 有一条台账行”
可以由错误架构或错误 payload 的行冒充。

### 3.3 闭包只信任 manifest 自报，不消费或认证 census

闭包检查在 `promotion_ledger_check.py:143-186` 直接读取 manifest 的
`tier1_component_members`；工具没有 census 输入、census digest 或受信
component-membership 表。一次性实例生成脚本
`remediation_r4/evidence/build_promotion_ledgers.py:157-170` 确实读取 census，
但该数据来源没有被执行门认证。

可复现反例：在 clean manifest 中把 T1-A 的 ADMIT `pkg-a`
`tier1_component_members` 从 `pkg-a,pkg-b` 改为 `pkg-a`，同时把真实同分量
`pkg-b` 改为 HOLD 并提供合法 authority；实际仍为 GREEN/exit 0。部署时即可
形成 `pkg-a=libc++`、`pkg-b=libstdc++` 的同一真实 TIER1 分量拆分。

当前三组实例本身没有数据错配：manifest/ledger 分别为 108/108、129/129、
438/438 行；Base-first、T1-0008、Wave 1 的每架构 ADMIT 数分别为 7、5、
24/27，三组均实跑 GREEN，且与当前 census 分量表抽验一致。但当前实例正确
不能证明执行门会拒绝上述被篡改或错误生成的执行期输入。

## 关闭条件 4：CLOSED

结论：v1.2 是 33 项完整重跑，旧三工具及旧 fixture payload 未被触碰。

依据：

- `corpus/gates/fixture_catalog.tsv:2-34` 共 33 项；独立重跑四工具全矩阵
  得到 4 GREEN、29 RED、0 mismatch，门色、exit 与 required marker
  33/33 符合预期。
- v1.1 的 27 个既有 tool/fixture key 与 v1.2
  `corpus/gates/verdict_matrix.tsv:2-28` 逐项结果一致。唯一 raw stdout
  字节差异是 `registry_expired` 的 `as_of=2026-07-27` 变为
  `2026-07-28`；其 `REGISTRY_EXPIRED / RED / exit 2` 不变。
- 旧三工具与 v1.1 字节一致：
  `tunnel_registry_check.py=3feca96a…373ccc`、
  `g7_exception_gate.py=eeb0f387…d4db98`、
  `edge_diff_check.py=3e06d9e5…22c2f74`；原 82 个旧 fixture 文件也全部
  字节一致。
- `corpus/gates/FREEZE_MANIFEST.sha256` 108 行、108 个唯一路径，
  实跑 108/108 OK；冻结集合无漏项或多项。v1.1 的 89 项中无删除：
  85 项 SHA 不变，4 项矩阵/版本派生元数据随全量重冻更新，新增第四工具
  及其 18 个 fixture 输入。

“本轮 gates 变化仅为第四工具新增与清单重签”若按路径数理解并非仅两个
文件，实际还包含第四工具 fixtures 和全矩阵派生元数据；但旧三工具及旧
fixture payload 确实零变化，不构成关闭条件 4 的反例。

## B-02 终判：NOT_CLOSED

R4 已关闭 Boost noarch 漏枚举这一条具体路径，但 B-02 要求执行期不能因
source/promotion 分层而错误拆分真实 TIER1 分量。关闭条件 3.3 的反例仍可让
一个真实分量的一部分 ADMIT、另一部分 HOLD，并让强制门返回 GREEN；3.1/3.2
还允许未登记输出或错误 payload 被 ADMIT。该结果会直接造成错误镜像决策，
所以 B-02 尚未闭合。

## D-G4 终判：DISAGREE

D-G4 的文本规则和本轮三个实例自身一致，但它宣称的两个机械不变量——
“每个 source 输出恰一行”和“ADMIT 集合满足 census TIER1 分量闭包”——
尚未由第四门可靠执行。存在上述 GREEN 假绿时，规则不能作为执行期强制门，
故本轮不能 AGREE。

## 新引入的阻塞级问题

`R4-NB-01`：第四门使用未认证、欠约束的三表连接。manifest 外 ledger 行、
noarch/架构及 payload 身份错配、伪造 component-membership 三类输入均可
GREEN。它们属于同一根因，不重复拆项；该问题会在执行期造成错误 ADMIT/HOLD
或混合 stdlib 分量，属于阻塞。

## Notes（不影响上述裁定）

- R4 修正版验证器的计数已从 census 重算；生成汇总表时仍带有
  33/30/33 预期常量。由于本轮已对完整 repodata、RPM 身份和 census
  独立重算，这一点不改变关闭条件 1 的 CLOSED。

P1_EXIT_STILL_FAIL
