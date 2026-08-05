# R9-B：统一三架构源基线（平台标准导出）执行报告

日期：2026-08-05  
最终状态：`RED_STOP_TAG_ARCHIVE_SHA_MISMATCH`  
停止位置：共同输入前置（任务第 1c）；未进入任何架构构建、T4 门禁或旧制品隔离。

## 1. 结论摘要

本地 `codes/llvm` 身份核对通过：分支为 `tizen_base`，HEAD 为 `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b`，工作树零输出，`upstream/22.1.8` peeled commit 为 `ca7933e47d3a3451d81e72ac174dcb5aa28b59d1`。

标准非 native GBS 导出已实测成功，完整命令未含 `--no-patch-export`。它从 `upstream/22.1.8` 生成六个源码 patch，逐个与本地六个源码提交的稳定 patch-id 相同；EHABI backport 是 `0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal.patch`，spec 注入项为 `Patch5` 与 `%patch5 -p1`。

共同身份断言未通过：

| 项目 | 冻结期望 | 本机标准 GBS 实测 | 结果 |
|---|---|---|---|
| Source0 SHA256 | `16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e` | `1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2` | `MISMATCH_RED_STOP` |

任务第 1c 明定“不等即停报”，故没有采用导出后替换 tarball、修改配置或其他未获裁决的规避方式。

## 2. 导出前身份冻结

| 项目 | 实测 | 状态 |
|---|---|---|
| 当前分支 | `tizen_base` | PASS |
| HEAD | `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b` | PASS |
| `git status --porcelain` | 0 字节 | PASS |
| backport 位于 HEAD | `3d3e3da…` | PASS |
| 上游 tag peeled commit | `ca7933e47d3a3451d81e72ac174dcb5aa28b59d1` | PASS |
| 相对 tag 的全部提交 | 8 个，其中 2 个仅改 `packaging/` | RECORDED |
| 相对 tag 的源码提交 | 期望 5，实测 6 | `MISMATCH_DECLARED_CONTINUE` |

六个源码提交依次为：

1. `53accdc47cd72c0daf61bf70e52946b2d8d80f2e` — Add support for Tizen toolchain vendor
2. `f31417256f953cc5d26cd188704c89ad41572b50` — Link libclang_rt.asan.so against libatomic when necessary
3. `9576175b59fa577c940eff1712a59b129ae03f03` — Fix armv7l build when compiler is clang
4. `9ad40bd946a75b44d8e05de8ba3263a5ce2ed885` — clang: add support for LLVM gold plugin in case of accelerated environment
5. `fd7a4929c0e424d564cdd1f0c55af9153dcea321` — Merge MLGO AOT compilation support and optimization improvements
6. `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b` — libcxxabi EHABI backport

原 `identity_freeze.tsv` 中 `platform_tag_archive_sha256=PASS` 实际只核对了平台 SRPM 内 Source0，并未实测本机 GBS 重新生成的 tag archive；该标签已由 `identity/tag_archive_adjudication.tsv` 明确标为 `SUPERSEDED`，最终身份状态改为红停。

## 3. 标准导出实测

最终有效的 export-only 调用上下文：

- 工作目录：`tmp/R9B/gbs-source-std-r2`
- packaging 目录：`packaging`
- spec：`libcxx-runtimes.spec`
- GBS 配置：`progress/T3R/config/gbs_llvm.conf`
- 选项：`--style git --include-all --packaging-dir packaging --spec libcxx-runtimes.spec --export-only`
- 明确未使用：`--no-patch-export`
- GBS 退出码：0

完整命令和退出码见 `commands/02b_standard_export_preflight_corrected.log`，全量输出见 `build/standard_export_preflight_corrected.full.log`。日志原文确认：

- upstream tag 格式为 `upstream/%(upstreamversion)s`；
- patch 范围为 `upstream/22.1.8..HEAD` 的导出虚拟提交；
- spec 从 `Patch0` 依次注入到 `Patch5`；
- GBS/GBP 强制重新生成 `llvm-22.1.8.tar.gz`，而不是保留已放入 packaging 的冻结 tarball。

### 3.1 导出 patch 清单

六个 patch 的文件名、SHA256、对应提交、patch-id 与 MATCH 状态见 `export_patch_inventory.tsv`。重点断言：

- 数量：6；
- EHABI：`0006-…patch`；
- spec：`Patch5`、`%patch5 -p1`；
- 六项 exported patch-id 与对应 commit patch-id 全部 `MATCH`。

### 3.2 首次预检调用错误

第一次 export-only 预检使用了自定义目录 `r9b-packaging`。GBS 只忽略所选 packaging 路径，因此仓内原 `packaging/` 历史被错误纳入 patch，产出 8 个 patch。该次分类为 `SPEC_ERROR_IN_PREFLIGHT_PACKAGING_DIR`；未做依赖排序、未出现 `circle found`、未运行 `init_buildsystem`、未调用编译器。完整失败现场保留在第一个预检根并标记 `RETIRED_SINGLE_USE`。

纠正为标准目录 `packaging` 后，patch 数量、编号与内容全部符合 R9 机制结论，但 Source0 仍触发本报告的共同身份红项。

## 4. Source0 差异实测

对冻结 tarball 与本机标准 GBS 生成 tarball 做了完整内容级比对：

- 两边 tar 路径列表均为 184,826 条，列表 SHA256 同为 `a9163aac6748e47470d6233fb849e7d0a86c0f2bf83ea75e131e303a1e3044a9`；
- 两个压缩包完整解包后，`diff -qr` 仅报告一个不同文件：`llvm-22.1.8/clang/bindings/python/.git_archival.txt`；
- 冻结 tarball 中 `describe-name` 保留字面量 `%(describe:tags=true,match=llvmorg-*[0-9]*)`；
- 本机 GBS/Git 导出将其展开为 `llvmorg-14-init-170484-gca7933e47d3a`；
- 因该内容差异，未压缩 tar SHA256 也不同：冻结值 `40759572…`，本机值 `8f9473e3…`。

取证命令、预期非零的 `cmp`/`diff` 退出码和完整原文见 `commands/03b_common_input_red_stop_forensics.log`。其中两个收集器断言因 `pipefail` 与转义写法误报，原失败日志保留；`commands/03c_forensics_collector_retry.log` 只重跑相应只读断言并全部退出 0，没有重复 export 或解包。

## 5. 停止后的状态

| 范围 | 状态 |
|---|---|
| armv7l 构建 | `NOT_RUN_COMMON_INPUT_RED_STOP` |
| aarch64 构建 | `NOT_RUN_COMMON_INPUT_RED_STOP` |
| x86_64 构建 | `NOT_RUN_COMMON_INPUT_RED_STOP` |
| 新 SRPM/RPM | 未产生 |
| T4 门禁重跑 | 未执行 |
| `artifacts/rebuild_2218_std/` | 无文件 |
| 旧制品隔离 | 未执行；`rebuild_2218/` 保持原位 |
| `SOURCE_PROVENANCE` | 预备修订已回滚，工作树与任务前 HEAD 一致 |
| `codes/llvm` | `tizen_base`/`3d3e3da…`，最终仍为干净工作树 |
| 外部仓 push | 未执行 |

本轮没有产生可供身份核验的新制品，因此没有把预备的 provenance 字段修订提交到 packaging，也没有移动旧制品。各阶段精确状态见 `execution_status.tsv` 和 `provenance_revision_disposition.tsv`。

## 6. 覆盖边界

由于共同身份前置红停，本轮未形成构建期、静态或加载期的新门禁结论；此前登记的 ARM EHABI 与 Itanium/DWARF 运行时异常展开缺口仍为 `PENDING_BOARD_VERIFICATION`，本轮未触及也未声称覆盖。

## 7. 证据索引

- 身份冻结与修订：`identity/identity_freeze.tsv`、`identity/tag_archive_adjudication.tsv`
- 调用与首次预检事故：`invocation_delta.tsv`、`preflight_incident.tsv`
- 标准导出全日志：`build/standard_export_preflight_corrected.full.log`
- patch 身份：`export_patch_inventory.tsv`、`raw/retry_exported_patch_commit_mapping.stdout.txt`
- Source0 差异：`raw/red_stop_frozen_and_generated_sha256.stdout.txt`、`raw/red_stop_extracted_tree_diff.stdout.txt`、`raw/red_stop_archival_substitution_contents.stdout.txt`
- 最终阶段状态：`execution_status.tsv`
- 所有命令原文与退出码：`commands/*.log`
