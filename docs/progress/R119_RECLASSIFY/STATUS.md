# 重判任务状态

分支 `codex/runtime-validation`；原两批表不覆盖；不用板子。

| 节 | 状态 | 产物 | 说明 |
|---|---|---|---|
| 未定项成因分类 | 已记录并复核 | CAUSES.md / CAUSES_156.tsv | 最终主因 A 89、B 5、C 23、D 39；cv_status 从初始 B 改为 C，原因见 B_TO_C_154.json |
| A/B 重判 | 已完成 | RECLASSIFIED_001_200.tsv / DISTRIBUTION.json | 89 项结构性不适用、5 项限定范围 INTERNAL_ONLY；44 项阳性不动，62 项未定 |
| C 评估、D 缺口 | 已完成 | CD_GAPS_62.tsv / CD_ASSESSMENT.md | 62 项逐项列缺口、可解性与规划时间；未实施 C 函数身份取证 |
| 分布、耗时、跨包口径 | 待汇总 | 待生成 | 候选数与包边数分开 |

资源首次 light 闸门通过。RLIMIT_AS=9,921,875,558 字节，串行 nice 19 / ionice 3；Git 使用普通 I/O。资源闸门任意非零保存断点停止，不等待抢资源。

初始定位的 pwd / ls / rg / sed 只读辅助命令在会话中有输出与退出码，尚未逐条另存 recorder 文件；主要分析、编译、运行与 Git 操作从 raw/001 起逐条记录。不存在这些失败查询支持的阴性结论。
