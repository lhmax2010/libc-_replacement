# 运行时验证线状态（2026-09-05）

| 任务 | 状态 | 用时 | 产物路径 | 备注 |
|---|---|---:|---|---|
| W1 `wbuffer_convert` 使用情况 | PARTIAL | 约 40 分钟 | `docs/progress/R108/W1_REPORT.md` | 覆盖内源码与 39,166 个 ELF 均为零命中；因不可得材料不宣称全平台不存在 |
| W2 不随迁移重新构建组件 | PARTIAL | 约 75 分钟 | `docs/progress/R108/W2_REPORT.md` | 三类已量化；外部产品组件材料不可得，全局上界为 `NOT_AVAILABLE` |
| W3 两组件异步取消可达性 | CONFIRMED_ON_X86_64 | 约 20 分钟 | `docs/progress/R108/W3_REPORT.md` | 两组件条件可达；补丁运行库与系统基线各 5 轮、25/25 判定通过；armv7l 为 `NOT_OBSERVED` |
