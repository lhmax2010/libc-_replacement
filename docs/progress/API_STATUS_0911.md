# 平台 API 跨标准库验证：周末状态

分支 `codex/runtime-validation`；仅隔离验证与材料，不修改平台源码或配置、不推 Gerrit。每格须有真实 provider、同侧对照、具体值断言和加载映射；缺项不替代为模拟结果。

| 任务 | 状态 | 用时 | 产物路径 | 备注 |
| --- | --- | --- | --- | --- |
| W1 四处接口实测 | PARTIAL | 约 12 分钟 | [报告](API_0911/W1/REPORT.md) | 8 个 Native ELF；行为运行格未完成，产品依赖选择待确认 |
| W2 shim 评估 | SHIM_VIABLE（固定样本） | 约 7 分钟 | [报告](API_0911/W2/REPORT.md) | 4 格 × 5 次通过；三个目标接口仍 SHIM_UNCERTAIN |
| W3 四方案 | PARTIAL（条件性评估完成） | 约 3 分钟 | [四方案](API_0911/W3/OPTIONS.md) | 完整改动量/旧接口期限待材料与业务确认 |
| W4 中英决策材料 | NOT_STARTED | — | API_0911/W4/ | 区分实测、静态核查、推断 |
