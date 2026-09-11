# 平台 API 跨标准库验证：周末状态

分支 `codex/runtime-validation`；仅隔离验证与材料，不修改平台源码或配置、不推 Gerrit。每格须有真实 provider、同侧对照、具体值断言和加载映射；缺项不替代为模拟结果。

| 任务 | 状态 | 用时 | 产物路径 | 备注 |
| --- | --- | --- | --- | --- |
| W1 四处接口实测 | PARTIAL | 约 12 分钟 | [报告](API_0911/W1/REPORT.md) | 8 个 Native ELF；行为运行格未完成，产品依赖选择待确认 |
| W2 shim 评估 | NOT_STARTED | — | API_0911/W2/ | 前提本身也需核查 |
| W3 四方案 | NOT_STARTED | — | API_0911/W3/ | 不推荐、不排序 |
| W4 中英决策材料 | NOT_STARTED | — | API_0911/W4/ | 区分实测、静态核查、推断 |
