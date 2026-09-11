# 平台 API 跨标准库验证：周末状态

分支 `codex/runtime-validation`；仅隔离验证与材料，不修改平台源码或配置、不推 Gerrit。每格须有真实 provider、同侧对照、具体值断言和加载映射；缺项不替代为模拟结果。

| 任务 | 状态 | 用时 | 产物路径 | 备注 |
| --- | --- | --- | --- | --- |
| W1 四处接口实测 | PARTIAL（续测材料完成，待审） | 原轮约 12 分钟；续测约 65 分钟 | [续测报告](API_0911/W1_CONT/REPORT.md) / [主矩阵](API_0911/W1_CONT/MATRIX.md) | 两架构 16 个已执行格、80 次尝试；EWK 反方向、真实 vector 回调、完整 Dali 图形/虚调用仍缺 |
| W2 shim 评估 | SHIM_VIABLE（固定样本） | 约 7 分钟 | [报告](API_0911/W2/REPORT.md) | 4 格 × 5 次通过；三个目标接口仍 SHIM_UNCERTAIN |
| W3 四方案 | PARTIAL（条件性评估完成） | 约 3 分钟 | [四方案](API_0911/W3/OPTIONS.md) | 完整改动量/旧接口期限待材料与业务确认 |
| W4 中英决策材料 | 原轮待审稿；续测补充另列 | 原轮约 8 分钟 | [原中文](API_0911/W4/DECISION_ZH.md) / [原 English](API_0911/W4/DECISION_EN.md)；[续测中文](API_0911/W1_CONT/REPORT.md) / [English supplement](API_0911/W1_CONT/SUMMARY_EN.md) | 原轮封存正文未重写；当前事实以续测补充为准，不能继续引用“目标均未实测”或“产品变体待确认”作为现状 |

用时为资源检查起至材料封存的近似墙钟时间，不是人时。原轮四次资源 gate 均返回 0，未使用开发板；续测 medium gate 也返回 0。原轮因产品依赖选择暂停；本轮获明确 elementary-tizen 确认后继续，并使用 armv7l 物理板。没有修改平台源码/配置，没有以时间上限未耗尽冒充材料完整。

## 交接摘要

- W1：两架构 EWK GNU 同侧各 5/5 正确，libc++→GNU 各 5/5 触发 SIGSEGV；反方向未测。Dali 真实属性包装子集两架构各 4 格×5 次通过；完整 GNU binder 同侧通过，仅 core 换为 libc++ 的依赖链各 5/5 缺符号。真实 SetSyncBinaryReply 仍 NOT_OBSERVED。原 Native 8 个 ELF 不改写为运行通过。
- W2：原真实 Boost 4 格×5 次/100 项比较不变。续测另有真实 EWK 专用 shim 固定样本 5/5 正确，但输出复用时 5/5 清空了直接调用会保留的字段；目标部署仍 SHIM_UNCERTAIN，其他目标也未完成部署验证。
- W3：四方案与责任/已定位改动点；完整平台和应用改造量、旧接口期限未取得。
- W4：原中英稿保留封存，续测补充明确新增事实和反例；[全部疑问](API_QUESTIONS_0911.md)。待人工审阅，不替业务方作决定。
- 构建物仅在 `tmp/API_0911/`，未进 git；平台源码/配置未改，不推 Gerrit、不 force。既有两份 R115 脏文件原样保留。
- **本轮板上上传件已按清单清理，独占目录已核验不存在，板子已释放**；清理记录见 `API_0911/W1_CONT/arm_board_cleanup.json`。
