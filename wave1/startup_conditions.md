# 波 1 启动条件（P1 出口整改 C4）

本清单是 allowlist 之前的硬门。状态取自 2026-07-25 的只读证据；实际执行
必须对构建时 repo、buildroot 和正式 wave 镜像重新取证。任一 `BLOCK` 或
复验失败均禁止开工。

## 六项硬门

### S1：round3 跨库 API 签核 — `BLOCK_SIGNOFF_DEFERRED_TO_EXECUTION`

原 27 项 C API 与 88/88 映射仍为 `SIGNED/HANDLE_CONFINED`；round2 另有
14 项新增候选：

- `capi-privacy-privilege-manager` 4 项；
- `pkgmgr-info` 10 项。

源码技术预判均为 `HANDLE_CONFINED_CANDIDATE`，但现行
`blocker_adjudication/round3/api_adjudication_candidates_round3.tsv`
的 14/14 行仍是 `PROPOSED`。其中 3 项 callback 非空条目强制绑定
`G7-PKG-001/G7-PPM-001/G7-PPM-002`：rider 二值门未通过、P2 镜像组装前
未完成，或 Q3 错误码漂移人工栏未在
`ACCEPT_WITH_RELEASE_NOTE/FIX_WITH_SEAL` 中二选一，均不得签核转绿。

人工决定（`lhmax2025`，2026-07-25）：签核推迟至正式开工，前置条件为
候选制品构建完成且板上实测通过；Q3 裁决同步推迟。签核文书已经就绪，
签核人、日期和 Q3 均保持空白；上述前置条件满足后完成签署，S1 即 PASS。

三方任一方未签或给出反证，S1 均不得转绿。若 S4 板扫要求保留
security-manager 条件分量，还必须额外关闭
`launchpad → libsecurity-manager-client` C tunnel。

### S2：原 27 项 C API 签核 — `PASS`

原 88 条 unknown 映射保持 88/88；27/27 API 均由 `lhmax2025` 于
2026-07-24 签为 `SIGNED/HANDLE_CONFINED`。评审包 SHA256：
`2e437083bfb9490476c6362c409755e9efa3978f51db11460e82fd2c30e43646`。

### S3：libc++/libc++abi 制品 — `PASS_CANDIDATE_21_1_1_2_REVERIFY_REQUIRED`

三架构权威候选为 `libc++/libc++abi 21.1.1-2`，报告见
`packaging/multiarch_packaging_report.md`，RPM/repodata 由
`REF_INDEX.md` 的三个架构仓和 SHA256 树清单锚定。三架构已完成
`@@LLVM_21`、闭包/强 UND、红绿绑定 fixture；当前结论替代早期 armv7l
本地 PoC 口径。

开工日仍必须在**同一个正式 wave buildroot** 对实际解析到的
21.1.1-2 NEVRA/SHA 重跑三组硬门；solver 解析、NEVRA、SHA 或闭包任一
不一致，不得继承本状态。

### S4：条件四包的板扫对账 — `PASS_PRELIMINARY_LLVM_IMAGE`

当前 buildconf/profile 静态证据为 `OFF_PROVEN`。`wave1_expanded_packages.tsv`
中
`security-manager/libsecurity-manager-client/capi-appfw-capmgr/boost-program-options`
原标为 `PENDING_BOARD_SCAN`。

板上 PASS 规则不是“全局扫描为空”，而是：

1. 对 `_ZN6Cynara13UidSandboxing*` UND **逐 ELF 与
   `n2_consumer_constraints.tsv` 对账**；
2. 台账外命中为 0；
3. 若条件四包摘除，security-manager 系 ELF 命中为 0。

2026-07-25 已在 clang 线
`BUILD_ID=tizen-unified-toolchain_20260707.220449_tizen-headed-armv7l`
完成先导扫描：3,272 个持久化 ELF 中确认 5 个目标 UND 消费 ELF，
`IN_LEDGER=5`、`OUT_OF_LEDGER=0`、`SECURITY_MANAGER_FAMILY=0`，
遍历/不可读错误为 0。权威证据见
`blocker_adjudication/board_scan_v1/board_scan_verdict.tsv` 和
`blocker_adjudication/board_scan_v1/summary.md`。

该结果只把 S4 更新为 `PASS_PRELIMINARY_LLVM_IMAGE`：它支持当前镜像的
条件四包摘除预判，但不使正式摘除自动生效。正式 wave 1 候选镜像仍须重跑
同一全镜像扫描；正式结果不符即改判 FAIL。

若 S4 PASS，四包摘除，`ACTIVE_BATCH=19`；若不满足摘除条件，四包保留，
则必须另闭合 launchpad tunnel，并为
`security-manager → boost-filesystem` 选择已签安全/shim，或启用
boost 三包同批分支（`ACTIVE_BATCH=26`；安全/shim 分支为 23）。

### S5：askuser 构建依赖 — `PASS_SNAPSHOT`

当前快照含 `security-privilege-manager-devel-1.4.0-0`，`.pc` 版本为
1.4.0，RPM Provides 为
`pkgconfig(security-privilege-manager) = 1.4.0`。实际 repo/rootstrap
解析 `>=1.4.0` 失败即登记波 1 blocker，不允许通过删除 BuildRequires
或降低版本要求绕过。

### S6：正式构建命令冻结 — `PASS_V1_SUPERSEDED`

正式 wave 1 命令已逐字冻结为
`formal_build_command/command.txt`，SHA256：
`0108f58741daab38f44be60cef6a30265965721efa3615cf3d4a5e76fba41caf`。
命令钉住 round1 取证的 `tizen_unified_standard` project config
（SHA256 `9eac0e23968b9bd7dabd463d400f0281d2464815fc2299431052f7b8cfb736a5`）、
armv7l、统一 buildroot 和五个 source commit。

机械检索结果为 `uid_app_sandboxing=0`、`dev_wos=0`、`--define=0`，
因此命令没有覆盖 round1 的 `OFF_PROVEN` 宏上下文。状态工件见
`formal_build_command/formal_build_command_status.tsv` 和
`blocker_adjudication/round2/formal_build_command_status.tsv`。

v1 冻结对 pre-D5 构建态有效；波 1 开工第一步为 D5 并入 buildconf 后
S6 v2 重冻（命令字节不变，`-D` 指向新 buildconf，OFF_PROVEN 机械
重取证）。在 v2 的 buildconf SHA、repo、宏展开和 OFF_PROVEN 证据落盘
前，v1 不得作为开工 PASS。

执行日必须先运行 `sha256sum -c command.txt.sha256`；命令任一字节变化、
SHA 不符、buildconf/source commit/path 变化或临时追加参数，均使 S6
立即重开。即使命令字节不变，`-D` 所指内容变化也必须产生新的 S6 v2
状态工件。

## 执行期进入序列

以下顺序是 fail-closed 的唯一进入路径，不得并行越过前项：

1. **D5 并入**：完成空 allowlist 全平台 `rpmspec -P` 差分和真实复杂包
   负面对照，接入 21.1.1-2 三架构仓；
2. **S6 v2 重冻**：命令字节不变，`-D` 指向新 buildconf，重新冻结其
   SHA，并机械重取证 `OFF_PROVEN`；
3. **候选构建**：按 19/23/26 的已裁条件生成唯一 active batch，并在
   同 buildroot 复验 S3、D5 注入和 G1–G7；
4. **板测**：正式候选镜像重跑 S4、强 UND/绑定、场景和回退；
5. **签核**：完成 14 项人工签署及 Q3 二选一，三项 rider 必须已执行；
6. **allowlist 正式化**：只写入已签 active batch source package；
7. **五步验收**：双构建、ABI/边差分、镜像组装、板上场景、回退演练
   全部通过后才允许发波。

## 当前总判定

**NOT_READY**。

当前启动红项为两类：

1. 14 项 round3 API 的签署按人工决定推迟至正式开工；候选制品构建、
   板上实测、3 项 callback G7 rider 与 Q3 裁决尚未完成（S1 文书已就绪）。
2. D5 尚未正式并入，S6 v1 已标 superseded；S6 v2 与同 buildroot 的
   S3 21.1.1-2 复验尚未产生。

S2、S5 为 PASS 类状态，S3 是候选制品 PASS 但带执行日复验义务，S4 为
先导 PASS，S6 v1 仅作历史证据。正式 wave 1 镜像上的 S4 复跑若证明
条件四包必须保留，会按规则展开 launchpad/boost 的附加关闭动作，不得
继承先导结果或静默转绿。
