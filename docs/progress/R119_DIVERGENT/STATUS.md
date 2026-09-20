# 实现定义类型差异扫描

第一节完成：对冻结的 31,586 处结果重扫，旧输出未覆盖。

| 结果 | 改前 | 改后 |
| --- | ---: | ---: |
| RESULT_NO_OBJECT | 15,780 | 15,770 |
| RESULT_TYPE_DIVERGENT | 0 | 10 |
| RESULT_STDLIB_OBJECT | 13 | 13 |
| RESULT_UNRESOLVED | 15,793 | 15,793 |

10 处逐条见 `RECLASSIFIED_CHANGES.tsv`，65 候选全量计数见 `RECLASSIFIED_CANDIDATES.tsv`。本节重新核验 6,952 个平台来源路径 SHA；复用原有两库五次测量，不重新选择实参。静态 const bool 定义链沿用受保护的实际两侧声明证据；具体类型必须相同才允许 RESULT_NO_OBJECT。

与人工给出的数量一致：numeric_limits 1、decay 9。未继续推进 65 项关闭。

主任务尚在进行；只测 x86_64，不用板子。
