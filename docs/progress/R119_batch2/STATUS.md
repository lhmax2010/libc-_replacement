# 第二批候选定界状态

当前执行边界：已复核至第二批第 100 项（总序号 200），没有进入第三批。两项最终分类口径待确认，见 [疑问](../R119_batch1/RESUME_QUESTIONS.md)。脚本当前摘要见 `CURRENT_SCRIPT_SHA256.json`；阶段汇总见 [报告](W1/SUMMARY.md)。

已写第 101–200 项的逐项复核记录；尚有 0 项未复核。**PARTIAL，不等于全部定界完成。**

| 分类 | 候选数 |
| --- | ---: |
| INTERNAL_ONLY | 0 |
| PUBLIC_NO_CONSUMER | 0 |
| CROSS_PACKAGE | 10 |
| RUNTIME_CALL | 0 |
| NOT_OBSERVED | 90 |

零值表示尚未作出该分类，不是平台零命中结论；运行库关系列与主分类可重叠。成员/基类及别名计数不等于新的独立包边。

记录：[TSV](W1/REVIEW_101_200.tsv)。逐项依据和缺口见 `W1/reviewed/`。

实际累计墙钟时间 1068.9 秒；完整定界平均时间 NOT_OBSERVED。

资源：light，串行，nice 19 + ionice 3，30% RLIMIT_AS；Git 普通 I/O；不使用板子。

第一批仍有未定项，参见 ../R119_batch1/W1/SUMMARY.md。跨批建索引和复核不抹去第一批缺口。
