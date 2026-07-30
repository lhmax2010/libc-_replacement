# P1 出口定点复核 r6

- 评审日期：2026-07-30
- 评审范围：仅复核 R5-NB-01 及其对关闭条件 3、B-02、D-G4 终判的影响
- 输入：`p1_exit_review_v7.tar.gz`
- 输入 SHA256：`52413f783f5de9ee6bc4848da6265bf328372f44e6fc0ea0928debc3b5aa98e2`
- corpus HEAD：`c61abe4d02a721ea7b8dbf7c2c596c0be2e93aa8`

## 0. 输入完整性与整改范围

**CLOSED。**

1. 归档 SHA256 实算与给定值一致。顶层 `MANIFEST.sha256` 的 670/670 项、
   corpus 子清单 293/293 项、remediation 子清单 370/370 项均校验通过。
2. `remediation_r6/diffs/level1_changed_paths.txt`、name-status、补丁中的路径
   集合与仓库基线 `72f4ff61` 到 HEAD `c61abe4d` 的实际差分完全相同，共
   45 条路径，无清单外改动。
3. 旧三工具与 v1.3 基线逐字节一致：
   - `edge_diff_check.py`：
     `3e06d9e593533789360cfe10c0db4a924782f223b9febf37fd6328a9e22c2f74`
   - `g7_exception_gate.py`：
     `eeb0f387e1cd9ec26c8e33ec38e4b7415b75cf9678c3b5d9bcedc9bb1fd4db98`
   - `tunnel_registry_check.py`：
     `3feca96a1629ff90c2a0c057b4dd933c7ed43af114d4c82f5dba828984373ccc`

## 1. R5 两个反例重放

**CLOSED。**

### 1.1 fixture 层

独立运行第四门所得结果：

| 反例 | 结果 | 红码 |
|---|---:|---|
| `pkg-a` 自封 neutral | exit 2 / RED | `NEUTRAL_NOT_IN_TRUSTED_REGISTRY` |
| `boost-program-options` 三架构自封 neutral | exit 2 / RED | 3×`NEUTRAL_NOT_IN_TRUSTED_REGISTRY`，且 3×`ADMIT_CLOSURE_BROKEN` |
| neutral 登记册篡改 | exit 2 / RED | `CENSUS_INPUT_UNVERIFIED` |

工具在 `promotion_ledger_check.py:381-410` 以
`(census_id,target_arch,package,rpm_arch,nevra)+rpm_sha256` 精确匹配
受信登记册；neutral 不会在 `:477-539` 被计作普通 `ADMIT` 来满足分量
闭包。因此登记册红与闭包红是两条独立断言，不是同一判断的重复输出。

### 1.2 三组真实台账

Base-first、T1-0008 S4-pass、wave1 conditional 三张原始台账均独立复跑为
exit 0 / GREEN。随后对每个 `boost-program-options` 架构行逐行做单点
neutral 篡改，共 12 次：

- Base-first 三架构：每次均 exit 2，报
  `NEUTRAL_NOT_IN_TRUSTED_REGISTRY`；
- T1-0008 三架构：每次均 exit 2，同时报登记册红和闭包红；
- wave1 的 23/26 两分支各三架构：每次均 exit 2，同时报登记册红和
  闭包红。

Base-first 中该分量原来全部为 HOLD，故没有剩余 ADMIT 驱动闭包红是正确
语义；登记册门本身已经阻断该旁路。结果证明不是“多行同时篡改”才触发。

## 2. neutral 登记册五级证据链

**CLOSED。**

### 2.1 RPM 实物到资格证据

冻结 RPM
`boost-license-1.83.0-5.1.noarch.rpm` 实算 SHA256 为
`39484d87349fe00b7dbc9382518cb258b694d3126d0c64358da3e3abf3871cf0`，
NEVRA 与 SOURCERPM 分别为：

- `boost-license-1.83.0-5.1.noarch`
- `boost-1.83.0-5.1.src.rpm`

`rpm -qpl` 只有许可证目录和 `LICENSE_1_0.txt` 两个路径；解包后仅有一个
普通文件，其首四字节为 `426f6f73`，ELF magic 命中数为 0。现场
`rpm -qpl` 输出 SHA256 为
`ecd78249d01d898c0766ceb6087d6b5e81d98e4465cdff2952dda5438f9b15df`，
与登记册 `filelist_sha256` 一致。

RPM 实物、Base primary metadata、冻结 `noarch_source_outputs.tsv` 三方
均给出同一 payload SHA
`39484d87349fe00b7dbc9382518cb258b694d3126d0c64358da3e3abf3871cf0`；
primary 又由 `repomd.xml` 的摘要指向，相关 R4 证据均受其 MANIFEST
锚定。

### 2.2 资格证据到登记册

三份证据原文的实算摘要为：

- qpl：`b7203d811a7768e42d006f90da92744a35c94d7e178a24aabf5ebed81172d8da`
- ELF magic 零命中：
  `28b4bb2792a5dd003ef187821eaba887dd611a9002f4996eb6db909c3a74b498`
- 双源内容 SHA：
  `6865a2b5e14f613cdeea8d14bf814e29e33365e417cb9d80e71fd736a925b363`

三值均精确写入 `stdlib_neutral_registry.tsv` 的三架构行；登记册仅含
`boost-license`，生成件与 corpus 件逐字节相同。登记册 SHA256 为
`af06a317bd69e4358e4ce3f499310fb086367e84311371bf33195105cdfd1084`。
`neutral_registry_evidence_r6.sha256` 所列 7/7 份证据亦全部校验通过。

### 2.3 登记册到 authority，再到工具固化根

`census_input_manifest.tsv` 同时认证：

- production TIER1 membership：
  `449b3fb1d65543b46703f26301a87a2effb6f911b26262066357d4a887000882`
- production neutral registry：
  `af06a317bd69e4358e4ce3f499310fb086367e84311371bf33195105cdfd1084`

authority manifest 实算 SHA256 为
`06bd30a0c143f290c6cac4f61363a61b142dd192f14ab7a40c6afccda9e384ee`，
并在工具 `promotion_ledger_check.py:15-18` 中作为唯一常量写死。工具先
校验 authority 根，再分别校验 membership 和 neutral 两个叶子。源码中
不存在 CLI 或环境变量覆盖受信根的入口。

### 2.4 “合法认证但语义错误”攻击

在现冻结根下，把含 ELF 的包连同伪造证据加入登记册不能静默通过：

1. 只改登记册，登记册叶摘要不匹配；
2. 同时重算 authority，但不改工具，固化 authority 根不匹配；
3. 再改工具常量，则工具本体、authority 和登记册三个冻结对象均发生
   SHA 漂移，触发解冻与全量矩阵重跑。

`FROZEN_EXECUTION_VERSION.md:30-44` 明确规定任一冻结对象变化须解冻并
完整重跑；三对象分别受 `FREEZE_MANIFEST.sha256:136`、`:2`、`:140`
锚定。故该链不存在不改变冻结根的捷径。

门运行时不会重新解释 `qualification_evidence` 文本的业务语义；其信任
边界是已经冻结并复核的登记册。这不构成本轮缺口：当前唯一登记实体
`boost-license` 已从 RPM 实物独立验证，未来新增登记实体必然改变上述
冻结链并进入解冻复核，不能由台账行自报获得资格。

## 3. 自报 proof 输入面删除

**CLOSED。**

当前 corpus 全量检索不到旧三 proof 字段及旧的 proof 不完整红码；工具
schema、模板、三组实例和全部 fixture 均已物理删除这些列。R6 的一次性
迁移/fixture 生成脚本仅在删除集合中列出旧列名并执行移除，不读取其值
参与裁决，也不重新输出这些列，因而不是残留旁路。

## 4. 固化根更新与 v1.4 重冻

**CLOSED。**

1. 旧 authority 根实算为 `1ff1387e…7192`，新根为
   `06bd30a0…384ee`。逐行差分仅包括：
   - 将旧 `logical_path` 明确命名为 `membership_logical_path`；
   - 为 production 和 fixture 各加入 neutral registry 摘要及路径。

   原有两行的 census ID、membership SHA、membership 路径和 scope
   均未改变。因此根变化恰由双叶结构引起。
2. `run_matrix.py` 遍历完整 fixture catalog；42 项均有各自 command、
   exit、stdout、stderr 四份原始记录，共 168 份，引用清单 168/168
   校验通过。
3. v1.4 为 42/42 PASS，实际分布为 5 GREEN、37 RED。v1.4 矩阵前
   39 项连同表头的 SHA256 为
   `4853fd94cb2256c41aad82798c09b1649083d56e695b8d396cabaf2f4d4f6083`，
   与 v1.3 完整矩阵逐字节相同；本轮只追加三个 neutral 负例。
4. `FREEZE_MANIFEST.sha256` 共 142 项，142/142 自算通过，包含 authority、
   production neutral registry、fixture neutral registry、第四门工具及
   矩阵；v1.3 的旧 39 项结果未被改写。

## 5. 终判

- **R5-NB-01：CLOSED。** neutral 资格已从被验台账自报改为由固化根
  认证的独立登记册；原两个反例在 fixture 和真实台账层均稳定报红。
- **关闭条件 3：CLOSED。** 第四门不再存在幽灵行、身份不绑定、自报
  闭包数据或 neutral proof 自报四类已知假绿路径。
- **B-02：CLOSED。** source 全输出台账、ADMIT/HOLD 分层、分量闭包及
  neutral 受限子类的机械门均已形成 fail-closed 链。
- **D-G4：AGREE。** 当前规则与工具实现、实例台账和冻结证据一致。
- **整改引入的新阻塞级问题：无。**

P1_EXIT_PASS
