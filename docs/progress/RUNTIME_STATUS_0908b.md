# 运行时验证 R114/R115 进度

| 任务 | 状态 | 用时 | 产物路径 | 备注 |
|---|---|---|---|---|
| W1 libc++ 等待与状态排查 | PARTIAL，待范围确认；已提交推送 | 约 19 分钟墙钟 | R114/W1/REPORT.md | 8a25994f；124 行归属；两处有害等待，6×5 实测；call_once 范围待确认 |
| W2 两种后端设计对照 | PARTIAL，事实汇编已提交推送 | 约 20 分钟墙钟（与其他项交叠） | R114/W2/REPORT.md | d10c3fa9；明确 GCC 优化动机；精确取消缺陷史及 libc++ 排他性动机缺证据 |
| W3 armv7l 物理板 | MEASURED_WITH_LIMITATIONS，交接中 | 约 30 分钟墙钟 | R115/W3/REPORT.md | 两架构各 21×5；另 GCC/Clang 各 5 次物理板；angle 原快照 SHA 一致；已清板 |
| W4 实验副本回滚 | WAITING_FOR_CLARIFICATION | 未开工 | tmp/R115-W4/ | W1 完整名单及 W4 推送范围待澄清 |

原始启动时分支为 codex/runtime-validation；既有 R110 未跟踪记录保留。

疑问：W4 推送冲突、call_once 是否扩围、设计史材料缺口，完整内容见
[RUNTIME_QUESTIONS_0908b.md](RUNTIME_QUESTIONS_0908b.md)。
W4 未创建实验改动、未运行、未推送；不以假想结果宣告回滚可行。
各时限均未触发；上述是墙钟跨度，不是纯 CPU 用时，原始 UTC 起止在每条命令 `.time.json`。
