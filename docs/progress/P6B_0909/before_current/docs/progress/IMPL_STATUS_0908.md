# 四项决策实施总览

> 2026-09-11 元数据同步：提交引用已换为补齐 Change-Id 的等树新提交；历史阶段的用时、推送动作和验证结果保持原意。[新旧映射与树核验](P6_0909/resume/SHA_MAPPING.tsv)。

实施源分支：`sandbox/lhmax2025/libcxx-noexcept-relief`；基线：`c3f8578a4db871d9d6de96d751f4c2ea7b1638fa`（已核实远端）。材料分支：`codex/runtime-validation`。

| 任务 | 状态 | 用时 | 产物路径 | 备注 |
|---|---|---|---|---|
| 等待规格、写者回滚、析构补丁 | PATCHES_READY | 约 18 分钟 | `IMPL_0908/W1/` | 三提交已推 sandbox，严格原生匹配构建通过、25 次轻量值断言通过 |
| 双版本符号 | DUAL_VERSION_READY | 约 9 分钟 | `IMPL_0908/W2/` | 第四提交 4c5ba1fa 已推 sandbox；真实库六格各 5 次通过，远端一致 |
| 两架构完整验证 | REGRESSION_FOUND；ARM 全量 PARTIAL | 约 3 小时 16 分（证据封存） | `IMPL_0908/W3/` | 最终定向 470 次符合预期；原生全套 11,402 项，ARM 有效 7,848 项；唯一新增有效失败为析构同步用例；实板失联，3,554 项缺口及板端清理阻塞已记录 |
| 支持范围声明 | DRAFT_READY_FOR_REVIEW | 约 11 分钟（证据封存） | `IMPL_0908/W4/` | 中英文草稿及审阅清单；76 个链接、16 个不同证据文件、关键分母与 SHA 检查通过；不代替人工批准 |

全部疑问与口径见 [IMPL_QUESTIONS_0908.md](IMPL_QUESTIONS_0908.md)。未修改既有四补丁；不推正式平台分支或 LLVM 上游，不使用 force。

实施分支最终 SHA：`c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13`（原四提交之后追加 ARM 定时符号编码修正；远端一致）。
板端残留不能忽略，见 [失联断点与未清理范围](IMPL_0908/W3/BOARD_RESIDUALS.md)。

提交审阅：[中文支持范围](IMPL_0908/W4/SUPPORT_SCOPE_ZH.md)、[English draft](IMPL_0908/W4/SUPPORT_SCOPE_EN.md)、
[事实核对清单](IMPL_0908/W4/REVIEW_CHECKLIST.md)。材料完成不表示官方新增失败、ARM 全量缺口或板端清理已解决。
