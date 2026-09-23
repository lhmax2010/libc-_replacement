# QuickBuild 结果对账基线：保留触发条件，不制造失败豁免

范围仅为指定的 Base-Toolchain 历史登记。本轮未构建，未新增故障判定。未找到可直接宣布“默认 Clang 全量 QuickBuild 必败”的已确认包清单；这不等于保证全量成功。唯一可复用的已知原因完整构建失败条目如下，但有明确 GCC 条件。

| 包 | 已知原因和条件 | 证据路径及行号 | 登记轮次 | 当前用途 |
| --- | --- | --- | --- | --- |
| tensorflow2 | armv7l 真实 GCC 14.2 路径的 XNNPACK dot-product/FP16 汇编 `vsdot.s8` 被拒；候选与未改基线同点同因，737/4213，48 条错误 | `docs/progress/R108/REPORT.md:7–10,149–165`；`docs/progress/BUILD_WEEKEND_0918/w4/PACKAGE_STATUS.md:14`；`docs/progress/BASE_LEDGER_0921/FINAL_RESULT.md:91` | R105 发现、R108 基线确认，周末及 Base 对账沿用 | 仅在重现该 GCC 条件时作对账依据；默认 USE_CLANG=ON，不预报默认 Clang 失败 |

## 已登记的条件风险，不是逐包已测失败

| 包 | 条件 | 原因 / 证据 | 证据等级 |
| --- | --- | --- | --- |
| gcc | 全局 libc++ flag 实际进入 GCC 命令 | GCC 拒绝 `-stdlib=libc++` | driver 探针，不是该包完整构建 |
| gcc-aarch64 | 同上 | 同上 | 同上；aarch64 driver 当轮未观测 |
| gcc-armv7hl | 同上 | 同上 | driver 探针，不是该包完整构建 |
| gcc-armv7l | 同上 | 同上 | driver 探针，不是该包完整构建 |
| gcc-riscv64 | 同上 | 同上 | driver 探针，不是该包完整构建 |
| glibc | 同上 | 同上 | driver 探针，不是该包完整构建 |

来源：`docs/progress/R100/REPORT.md:118–125,159–188`、`docs/progress/R101/SUMMARY_ONE_PAGE.md:23–29`。实测 driver 拒绝来自 armv7l/x86_64；当轮 aarch64 driver 启动即 139，NOT_OBSERVED。这六个包按设计排除，不应在当前逐包适配方案下自动登记为必败；更不能写成“六包完整实测失败”。

## 不纳入上述已知原因失败清单的历史条目

| 条目 | 排除原因 / 来源 |
| --- | --- |
| LLVM armv7l 整轮 RPM 失败 | 周末 W2 报告当时根因 NOT_OBSERVED，不能回写为当时已知原因；后续 payload 根因与绕过另见本轮 PAYLOAD.md 和 ARM_W5_0921，不因旧失败自动豁免新 QB |
| bpftrace ARM 超时、aarch64 static 多 libbpf 依赖 | 前者不是已确认编译/写包错误，后者是 ELF 验收不符而非构建失败；周末报告保留历史，新版配方已修正并由人工推送 |
| 旧 MLGO / XLA ABI 阻断 | 已发布 LLVM 适配，不能把已解决的历史阻断当成当前失败基线 |
| Unified 的 lapack/openblas/onnxruntime/yaca | 不属于本次锁定的 Base-Toolchain 范围，不补入本表 |

没有用“未找到清单”推导“没有其他失败”。QuickBuild 的新失败仍需逐项审查，不自动套用这张条件表。
