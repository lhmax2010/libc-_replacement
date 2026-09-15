# 定案前核查状态

分支：`codex/runtime-validation`。执行顺序：实验 2 → 停下报告 → 经确认后实验 1 → 实验 3 → 文档修正。

| 项目 | 状态 | 用时 | 产物 | 说明 |
| --- | --- | --- | --- | --- |
| 实验 2：旧 EWK 引用版本 | PARTIAL，已审阅通过 | 核查与初稿约 20 分钟；交付时间另见 delivery | [报告](experiment2/REPORT.md) | 原材料不改；三个独立应用缺口保留；已获准继续 |
| 实验 1：异常归因与配置 | COMPLETE（含确定失败，非兼容性通过） | 闸门 12:17:43 至最终 ARM 校验 12:41:44 UTC，约 24 分钟；撰文另见记录 | [报告](experiment1/REPORT.md) | 两架构原异常为类型身份问题；96 格各五次；两 ABI/两展开器配置对照包含 ARM LLVM 组合 SIGABRT |
| 实验 3：反方向 EWK | NOT_AVAILABLE | 原断点复核，未启动完整构建 | [报告](experiment3/REPORT.md) | 缺真实 libc++ EWK provider；反方向运行 NOT_OBSERVED，不阻塞文档 |
| 中英文方案文档修正 | COMPLETE，待人工审阅 | 12:45:26 UTC 起，终点见 doc_revision/CHECKS.json；未超三小时 | [汇总](REPORT.md)、[自检](doc_revision/SELF_CHECK.md) | 回接异常矩阵、转发层推论；哨兵只评估，不修改平台头文件 |

疑问见 [QUESTIONS.md](QUESTIONS.md)。本目录作为归档产物目录；未在既有 progress/ 或实验材料中覆盖文件。

实际执行日期以记录时间为准：资源检查开始于 2026-09-15 09:40:01 UTC；目录按任务名保留 P11_0916，不伪造次日时间。本阶段无板上工作。
