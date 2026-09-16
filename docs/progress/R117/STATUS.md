# 范围候审状态

总状态：PARTIAL_SCOPE；仅范围调查，未进入差异查证。

| 阶段 | 状态 | 产物 | 缺口 |
|---|---|---|---|
| light 资源闸门 | PASS / exit 0 | raw/001_gate.* | 无 |
| 头文件与实体索引 | 完成有限配置索引，全集未证 | HEADER_COVERAGE.tsv / SKELETON.tsv | 条件分支、规范归属、34 个物理入口候选未完整解析 |
| 平台扫描 | 371/371 身份完成所选文件规则 | platform/STATUS.json | 112 个文件未作文本解析；真实语义使用数未取得 |
| 场景/优先级 | 候审 | HIGH_PRIORITY_SCENARIOS.tsv | 全部高优先级类型总数 NOT_OBSERVED |
| 旧材料对照 | 10 + 25 + 5 + 7 逐项映射 | EXISTING_MATERIAL_CROSSWALK.tsv | 原评审的顺序/hash 逐字出处未定位 |
| 自检/提交推送 | 见 SELF_CHECK.json 及交付记录 | SCOPE.md / QUESTIONS.md | 完成后停止等待审阅 |

开始：2026-09-16T04:59:30.956402+00:00；本状态生成：2026-09-16T05:50:10.557160+00:00；已用约 0.84 小时。任务时限六小时。
