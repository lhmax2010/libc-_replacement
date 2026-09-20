# ARM 写包原命令复现与归档层取证

先读STATUS.md、STEP1_RESULT.md，再读PRECHECK.md及各轮JSON/原始日志。实际脚本与执行快照均落盘，运行事件记录脚本SHA256。

大文件使用xz无损压缩、每片最多32MiB。DELIVERY_MANIFEST.json列原文件名、原字节数、SHA256与顺序分片名；按清单顺序拼接后用xz解压即可恢复。发布时已实际复原并核对每份SHA256。小文件原样交付，无静默删减。

RPM和完整失败树较大，仅保留在任务报告所列本地tmp路径；交付其文件清单、SHA256、目录状态和完整构建/跟踪日志，不将大RPM当作文档入库。

只推项目仓codex/r5-ehabi-diagnosis，不推包仓/Gerrit。阶段成功发布不代表RPM构建成功，具体退出码与判定以各轮报告为准。
