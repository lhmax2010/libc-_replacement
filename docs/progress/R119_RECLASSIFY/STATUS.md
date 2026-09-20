# 重判任务状态

分支 `codex/runtime-validation`；原两批表不覆盖；不用板子。

| 节 | 状态 | 产物 | 说明 |
|---|---|---|---|
| 未定项成因分类 | 已记录并复核 | CAUSES.md / CAUSES_156.tsv | 最终主因 A 89、B 5、C 23、D 39；cv_status 从初始 B 改为 C，原因见 B_TO_C_154.json |
| A/B 重判 | 已完成 | RECLASSIFIED_001_200.tsv / DISTRIBUTION.json | 89 项结构性不适用、5 项限定范围 INTERNAL_ONLY；44 项阳性不动，62 项未定 |
| C 评估、D 缺口 | 已完成 | CD_GAPS_62.tsv / CD_ASSESSMENT.md | 62 项逐项列缺口、可解性与规划时间；未实施 C 函数身份取证 |
| 分布、耗时、跨包口径 | 已完成 | FINAL.md / TIMING_AND_SCALING.md / CROSS_SURFACE.md | 44 跨包、89 结构不适用、5 范围内阴性、62 未定；44 候选映射 16 包对 / 21 符号边 |

资源首次 light 闸门通过。RLIMIT_AS=9,921,875,558 字节，串行 nice 19 / ionice 3；Git 使用普通 I/O。资源闸门任意非零保存断点停止，不等待抢资源。

初始定位和辅助显示命令的原文/退出码已据工具返回补录到 AUX_COMMANDS.md；主要分析、编译、运行与 Git 操作从 raw/001 起逐条记录。不存在失败查询支持的阴性结论。

本轮任务产出已完成，原两批定界仍有 62 项未定，不能称为 200 项全部关闭。A/B 实测与阶段平均见 TIMING.json；C/D 规划见 CD_GAPS_62.tsv。原表与原阳性证据摘要未变，最终自检通过。最后一节提交推送后停止，待人工审阅。
