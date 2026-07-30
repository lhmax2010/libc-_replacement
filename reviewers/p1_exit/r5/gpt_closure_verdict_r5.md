# P1 出口定点复核 r5

评审日期：2026-07-28  
输入：`p1_exit_review_v6.tar.gz`  
输入 SHA256：`b58e564b594a02446494e5f31b02ac4d0e3795301dc3f68f72ace49605876c37`  
corpus HEAD：`72f4ff613b006214073e712b85230b9443f78155`

总裁定：`P1_EXIT_STILL_FAIL`。R4-NB-01 原三类失效路径 CLOSED；但 R5
新增 `ADMIT_STDLIB_NEUTRAL` 证明自报旁路，导致关闭条件 3 与 B-02 仍为
NOT_CLOSED，D-G4 仍为 DISAGREE。

## 完整性与 Level 1：CLOSED

- 归档 SHA256 与给定值一致。归档共 653 个文件；顶层
  `MANIFEST.sha256` 的 650 行覆盖除其自身和两个校验输出外的全部
  650 个 payload，实跑 650/650 OK；corpus 279/279、remediation
  364/364 均通过。
- 仓实际 `a0727098..72f4ff61` diff 为 66 个路径，与
  `remediation_r5/diffs/level1_changed_paths.txt:1-66` 逐项相等，无漏项
  或越界路径。
- 旧三工具与基线逐字节一致：
  `tunnel_registry_check.py=3feca96a…373ccc`、
  `g7_exception_gate.py=eeb0f387…d4db98`、
  `edge_diff_check.py=3e06d9e5…22c2f74`。

## R4-NB-01：CLOSED

R4 的三个反例均已在 frozen fixture 和三组真实台账上重放；全部变为
`GATE=RED / exit 2`。

### 1. 幽灵 ledger 行

`promotion_ledger_check.py:111-118` 使用
`(batch,target_arch,package,rpm_arch,nevra)` 五元键；
`:229-275` 做 manifest↔ledger 双向唯一性检查。fixture
`ghost_ledger_row` 以及 Base-first、T1-0008、Wave 1 三组真实台账追加
manifest 外同型行后，均命中 `LEDGER_ORPHAN_ROW`。

### 2. noarch 冒充与 payload/image 身份错配

`promotion_ledger_check.py:286-322` 对匹配行执行：

- manifest `rpm_sha256 == ledger candidate_sha256`；
- ADMIT/neutral `image_selected_sha256 == candidate_sha256`。

fixture 及三组真实台账的 noarch identity、candidate SHA 或 image SHA
单点篡改分别命中 `PAYLOAD_IDENTITY_MISMATCH`、
`ADMIT_IMAGE_DRIFT`，或因 `rpm_arch` 改变同时命中
`LEDGER_MISSING_OUTPUT + LEDGER_ORPHAN_ROW`。R4 的欠约束连接键已消除。

### 3. 伪造分量成员

普通 ADMIT 闭包只读取经认证 census：
`promotion_ledger_check.py:370-464`；manifest 不再含
`tier1_component_members`。fixture 和三组真实台账对成员表
`component_id/component_pkg_count` 做单点篡改后，均在进入闭包计算前命中
`CENSUS_INPUT_UNVERIFIED`。

因此，R4-NB-01 所定义的幽灵行、身份不绑定、自报闭包三类失效路径均已真实
闭合。

## census 认证链：CLOSED

### 合并保真与来源真实性

- `ledger/census_membership_source_manifest.tsv:2-4` 登记的三张源表实算为：
  armv7l 1,795 行、SHA `e14b008c…a1e6`；aarch64 1,801 行、SHA
  `368fc213…0512`；x86_64 1,795 行、SHA `3b7f5843…c30`，与登记值完全
  一致。
- 按 armv7l→aarch64→x86_64 顺序给三源表逐行增加
  `census_id,target_arch` 后，与
  `ledger/census_membership_production_tier1.tsv` 全文件比较为零差异：
  共 5,391 个数据行，无丢行、加行、改值或重复 `(arch,package)` 键；
  每个 `component_pkg_count` 与实际成员数一致。
- 将三源表按 component 聚合并与
  `census/multiarch_v2/comparison/component_isomorphism.tsv` 的 TIER1
  记录逐组件展开比对：4,690 个分量全部对应，component ID、成员数和
  5,391 个成员均零差异。其 1,795/1,801/1,795 包数也与
  `arch_census_summary.tsv`、`isomorphism_summary.tsv` 一致。三源表可确认
  为 corpus 所引用的 0723-aligned census v2 产物。

### authority 根不可替换

- 工具源码 `promotion_ledger_check.py:16-18` 固化
  `TRUSTED_CENSUS_MANIFEST_SHA256=1ff1387e…7192`，没有 CLI 或环境覆盖项。
- `gates/census_input_manifest.tsv` 实算 SHA 正是 `1ff1387e…7192`；
  其中生产 membership 登记 SHA 为 `449b3fb1…0882`，合并成员表实算值
  与之相等。
- 校验次序为源码固化 SHA→census manifest 实算 SHA
  （`:179-192`）→指定 census ID 唯一行→membership 实算 SHA
  （`:194-218`）。自行构造一个 manifest/membership 内部自洽的替代组合，
  因 manifest SHA 不等于固化根，实跑仍得到
  `CENSUS_INPUT_UNVERIFIED / RED / exit 2`。`--census-manifest` 只能替换
  路径，不能替换受信内容。

## schema v2 实例迁移：CLOSED

- Base-first、T1-0008、Wave 1 的 manifest/ledger 数据行仍分别为
  108/108、129/129、438/438；五元业务键、NEVRA、candidate SHA 及最终
  PROMOTE/HOLD 选择与 R4 逐项一致。
- `boost-license` 从普通 ADMIT 改为 neutral 后，每架构有效晋级分布仍为：
  Base-first 7、T1-0008 5、Wave 1 23/26 分支 24/27；三组原始实例均
  `CENSUS_VERIFIED=YES / GREEN / exit 0`。
- neutral 行仍经过五元身份、manifest/ledger SHA 和 image SHA 对账；
  对当前 neutral 行篡改 SHA/rpm_arch 会报红。此项只确认身份绑定，不代表
  neutral 资格证明已闭合；后者见“新引入阻塞”。

## v1.3 重冻：CLOSED

- `fixture_catalog.tsv` 共 39 项，执行脚本遍历整个 catalog；全量结果为
  5 GREEN、34 RED、0 mismatch，非只跑新增 fixture。
- v1.2 的旧 33 个 tool/fixture key 在 expected/actual gate、exit、
  required marker 和 PASS 状态上逐项一致；旧三工具的 27 项 raw
  stdout/stderr/exit 字节一致。
- `gates/FREEZE_MANIFEST.sha256` 为 130 个唯一路径，实跑 130/130 OK，
  且第 2 行纳入 `census_input_manifest.tsv=1ff1387e…7192`；清单本身 SHA
  为 `9605fe4f…30f5`。
- `ref_manifests/gate_v1_3_raw_outputs.sha256` 的 156 项 raw 输出实跑
  156/156 OK。

重冻过程合规，但矩阵没有覆盖下述 neutral 自报反例，故 39/39 不能改变
总裁定。

## 新引入的阻塞级问题：R5-NB-01

判定：`FAIL`。`ADMIT_STDLIB_NEUTRAL` 的三项“机械证明”只是 ledger 中三个
未认证字符串，任意普通 C++ 包都可自报 `PASS` 后跳过闭包。

失效机理：

- `promotion_ledger_check.py:323-335` 只判断
  `neutral_non_elf_proof`、`neutral_no_cpp_surface_proof`、
  `neutral_dual_source_sha_proof` 的字面值是否为 `PASS`；工具不读取文件
  清单、ELF/符号扫描结果、第二 SHA 来源或任何证据 digest。
- 闭包循环 `promotion_ledger_check.py:402-404` 只处理 disposition
  `ADMIT`，所有 `ADMIT_STDLIB_NEUTRAL` 均无条件跳过认证 census 闭包。
- 实例生成器 `remediation_r5/evidence/build_promotion_ledgers_v2.py:296-300`
  仅按包名 `boost-license` 选择 neutral，并在 `:373-380` 无条件写入三个
  `PASS`；`:382-387` 的 `decision_evidence` 路径不属于门的输入或校验字段。
  因此未来同名候选即使新增 ELF/C++ 面，生成器与门仍可给出 GREEN。

可复现 fixture 反例：

1. 以 `admit_closure_broken` 为输入；认证 census 明确 `pkg-a/pkg-b`
   同属 T1-A，`pkg-b=HOLD_SIBLING`。
2. 只把 `pkg-a` 改为 `ADMIT_STDLIB_NEUTRAL`，将三 proof 字段填 `PASS`；
   manifest、candidate SHA、image SHA 和受信 census 均不改。
3. v2.0.0 实跑结果为
   `CENSUS_VERIFIED=YES / GATE=GREEN / blocking=0 / exit 0`。

真实台账反例：

1. Base-first 的 `boost-program-options` 在认证 census 中属于
   `tier1_production_0008`，分量共四个成员；原台账将该包 HOLD，其他三个
   Unified 成员也未在 Base-first 晋级。
2. 只把 armv7l 的该行从 HOLD 改为 `ADMIT_STDLIB_NEUTRAL`，保持候选身份和
   image SHA 相等，并把三 proof 字段自报为 `PASS`。
3. 工具仍返回
   `CENSUS_VERIFIED=YES / GATE=GREEN / blocking=0 / exit 0`。
   镜像据此会晋级 libc++ `boost-program-options`，而同一真实 T1-0008
   分量的三个 Unified 成员仍为 libstdc++，形成 D-G4 明令禁止的分量拆分。

这不是措辞或证据完美性问题，而是可使执行期错误镜像获得 GREEN 的新增旁路。

## 关闭条件 3 终判：NOT_CLOSED

R4 的三类原始假绿已关闭，但 R5 新增 neutral disposition 后，闭包门仍能被
未认证的三字符串旁路。关闭条件 3 的目标是第四门不再产生执行期假绿，因此
终判仍为 NOT_CLOSED。

## B-02 终判：NOT_CLOSED

真实 Base-first 反例可将 `tier1_production_0008` 的一个 libc++ 成员晋级、
三个 libstdc++ 成员保留，并获得 GREEN。B-02 所要求的分量级晋级不变量仍
可被破坏，故不闭合。

## D-G4 终判：DISAGREE

D-G4 `decision_record.md:92-97` 宣称 neutral 资格由三项“机械证据”建立；
实际强制门只验证三个自报 `PASS`，与宣称不一致。当前实例数据正确不能替代
执行期资格认证，故仍 DISAGREE。

P1_EXIT_STILL_FAIL
