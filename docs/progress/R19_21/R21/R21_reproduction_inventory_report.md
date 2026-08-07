# R21：已执行测试的具体可复现清单

## T1/R1 语料扫描

扫描分母为 372 个源码 RPM：371 个成功扫描、1 个未扫描。`tables/t1_scanned_371.tsv` 给出全部完整 source-RPM 身份（含版本/Release 字面量）、来源仓及其二进制映射；`t1_not_scanned.tsv` 给出 Chromium 唯一缺口及原始失败证据。

`t1_r1_classification_371.tsv` 逐包给出“需源码修改 / 需构建系统修改 / 仅需人工判读 / 无命中”及模式。45/371 个 T1_HARD 包见 `t1_hard_45_packages.tsv`；其 61,039 条完整命中（文件、行号、模式、前/中/后上下文）在本地总表 `t1_hard_45_full_hits.tsv`，策展副本拆成不超过 5 MB 的多个顺序块。

初版规则、修正规则、bits 方法、逐命中 bits 归属、glibc/libstdc++ bits 文件清单均在 `repro/T1_R1/`。bits 输入为 `glibc-devel-2.40-2.8.armv7l`（SHA256 `c838a113...527b`）与 `libstdc++-devel-14.2.0-1.14.armv7l`（SHA256 `aaefcf92...11c`）；完整值见 `bits_ownership_inputs.tsv`。

## 首批五包

`wave1_five_spec_identity.tsv` 给出五包的 spec、Name/Version 原文与 spec SHA256。两处缺陷的 ±4 行代码上下文在 `wave1_confirmed_defect_context.tsv`：

- `libcynara-commons`：`codes/libcynara-commons/src/common/exceptions/TryCatch.h:65`；
- `security-manager`：`codes/security-manager/src/common/include/utils.h:70`。

44/44 条 include 传播链原表复制到 `repro/wave1/forced_unwind_propagation.tsv`；9/9 个注入疑点位于 `wave1_injection_nine.tsv`。

## 动态实验（R14b L2，96 格）

实验件完整源码复制到 `repro/R14b/experiment_sources/`。`01_build_l2.log` 是逐件编译/链接命令原文；`matrix_all.tsv` 给出 96/96 格全部变量与逐格结果；逐格运行命令日志复制到 `repro/R14b/matrix_commands/`；`run_l2_stage.py`、`audit.c` 和命令日志给出 LD_DEBUG/LD_AUDIT 调用方式。

实测同侧 A、B 各 24/24 格正常退出；跨侧 A 为 24 个非零正常退出；跨侧 B 为 16 个正常退出、4 个 SIGABRT、4 个 SIGSEGV。这里仅复述原始结果，不作安全性或风险判断。8 个崩溃格的完整 `program.stdout`、`program.stderr` 和 `ld_debug.*` 原文逐文件复制，身份索引见 `r14b_crash_raw_files.tsv`。

## 三架构 14 项打包门禁

`t4_gate_14_repro.tsv` 逐项给出名称、判据、旧记录出处、新工具映射、执行说明，以及包含实际 shell 命令的脚本/运行日志与 SHA256；相关脚本、日志和三架构结果表复制到 `repro/T4/`。

P01–P08、P14 三架构均 PASS。P09–P13 的 armv7l/x86_64 有实测结果；aarch64 均为 `NOT_AVAILABLE`（缺少 QEMU carrier），与失败严格区分。逐架构原值见 `repro/T4/gate_results.tsv`。

## 展开器普查与闭包清单

口径分母为 R13 的 6,064 个“至少含一个运行时 ELF”的二进制 RPM 实例。清单完整导出：风险集 4,584、干净集 667、不可判定 813；另有 armv7l 风险集全表。五个厂商对象保留三架构 15 行实测，插件宿主保留 121/121 个二进制 RPM 实例。所有表均在 `tables/`。

## 体积与完整性

策展副本单文件上限为 5 MB。超过上限的完整表按表头保持、逐行顺序拆分；原本地大表及拆分映射见 `EXCLUDED.tsv`。所有策展文件与本地输出均由各自 `MANIFEST.sha256` 覆盖。

本清单不含排期、工时、优先级建议或风险评级。
