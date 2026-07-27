# P0/P1 冻结基线 v1.1

- 冻结版本：HLD v1.1（冻结）
- 冻结日期：2026-07-23（Asia/Shanghai）
- HLD 原文件：`HLD_libcxx_migration_v0.1_skeleton.md`；按标题和末条变更记录确认内容为 v1.1 冻结版，归档名规范化为 `docs/HLD_libcxx_migration_v1.1_frozen.md`。
- 一致性硬门：10/10 PASS，逐项证据见 `PRECHECK.tsv`。

## Census 快照

| 仓库 | 快照 URL | 目录观测时间 |
|---|---|---|
| Tizen-Base | `https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base/tizen-base_20260709.234055/repos/standard/packages/armv7l/` | `2026-07-17T06:47:46+00:00` |
| Tizen-Unified | `https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Unified/tizen-unified_20260716.042355/repos/standard/packages/armv7l/` | `2026-07-17T06:47:46+00:00` |

Census 汇总生成时间为 `2026-07-17T09:51:14+00:00`。URL、观测时间与生成时间均摘自 `evidence/abi_census/summary.md`。

## 三轮评审结论

首轮 2 PASS/5 FAIL、第二轮 7/7 FAIL 均完成证据化整改；第三轮短评审的 R1 经 A13 传递闭包重算、正文机械修正及三方复核后闭合，最终三轮评审全部闭合并冻结 v1.1。

## 冻结核心数字

| 指标 | 冻结值 | 权威证据 |
|---|---:|---|
| 生产核心波确证下界 | 130 包 | `evidence/abi_census/cutpoints/a13/summary.md` |
| UB_direct | 402 包 | 同上 |
| UB_closure | 513 包 | 同上 |
| bundle compat 桥接不变量 | 16 项 | `evidence/abi_census/cutpoints/a13/bundleraw_bridge_gap.md` |
| 隧道 API 机检种子 | 2479 条 | `evidence/abi_census/cutpoints/a9/` 与冻结 HLD §2 |
| 最终保守锚点 | 5 个 | `evidence/abi_census/anchors.tsv` 与冻结 HLD §2 |

`513` 是当前 692 条已落盘 unknown 边在 TIER1 分量级传递闭包中的严格封顶；同批约束和 shim fallback 的额外批次面按冻结 HLD 脚注另行披露，不混入该图上界。

## 证据目录索引

- `docs/`：冻结 HLD、评审响应、Line 3 中英双语一页纸、平台缺口报告和可复跑 ABI pipeline。
- `evidence/abi_census/`：完整 census 结果、`v1_superseded/` 至 `v4_superseded/`、全部 `cutpoints/`、板上动态绑定实证 `onboard_verification/`、独立原始二进制抽验 `independent_spotcheck/`。
- `evidence/abi_poc/native_rerun/`：当前原生 GCC/clang 双构建 ABI 对比的权威产物。
- `evidence/abi_poc/retest_no_B/`：无 `-B` 的原生 clang 身份门复验。
- `evidence/review_probe/abstract_dtor/`：抽象类 vtable 析构槽的标准语义与实测证据。
- `manifest.tsv`：未复制的大型原始解包树和 RPM 缓存的绝对路径、字节数与文件数；原目录保持不动。
- `SYMLINKS.tsv`：归档内符号链接及其原始目标清单（含解包产物中按目标根文件系统解释的绝对链接）。
- `PRECHECK.tsv`：冻结一致性硬门。
- `MANIFEST.sha256`：本目录除该清单自身外全部常规文件的 SHA256；符号链接元数据由已纳入哈希的 `SYMLINKS.tsv` 固化，自排除用于避免自引用。

本包仅归档和校验既有只读证据，不修改 HLD、源码、配置、RPM、解包树或采集结果。
