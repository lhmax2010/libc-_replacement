# 波 1 启动条件（review-round3）

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

### S3：libc++/libc++abi 制品 — `PASS_LOCAL_POC_ARTIFACT`

`p1/libcxx_packaging/packaging_report.md` 已对四个 armv7l RPM 完成
`@@LLVM_21`、NEEDED/逐符号归属、红绿 fixture 和运行冒烟。实际 wave
buildroot 必须对同一 NEVRA/SHA 重跑；不同制品不得继承 PASS。

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

### S6：正式构建命令冻结 — `PASS`

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

执行日必须先运行 `sha256sum -c command.txt.sha256`；命令任一字节变化、
SHA 不符、buildconf/source commit/path 变化或临时追加参数，均使 S6
立即重开。

## 当前总判定

**NOT_READY**。

当前启动红项为一类：

1. 14 项 round3 API 的签署按人工决定推迟至正式开工；候选制品构建、
   板上实测、3 项 callback G7 rider 与 Q3 裁决尚未完成（S1 文书已就绪）。

S2、S3、S5、S6 为 PASS 类状态，S4 为先导 PASS。正式 wave 1 镜像上的 S4
复跑若证明条件四包必须保留，会按规则展开 launchpad/boost 的附加关闭
动作，不得继承先导结果或静默转绿。
