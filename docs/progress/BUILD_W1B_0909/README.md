# LLVM 剩余门禁矩阵补测

本目录记录 LLVM TensorFlow 2.18 / MLGO 候选在 2026-09-11 的剩余门禁
验证。上一轮已闭合格的原始证据位于 `docs/progress/BUILD_W1_0909/`；本目录
不复制或重跑这些格，只在 `MATRIX.tsv` 标明来源。

建议阅读顺序：

1. `REPORT.md`：结论、完整十二格矩阵和未闭合项；
2. `MATRIX.tsv`：机器可读矩阵；
3. `raw/`：命令原文、退出码和技术性失败说明；
4. `evidence/`：产品核查与同环境对照摘要；
5. `build_logs/`：压缩后的完整构建日志及校验值；
6. `code/`：本轮实际使用的核查脚本快照。

判据是十二格全部闭合后才允许推送 LLVM package 分支；任何一格
`NOT_OBSERVED`、`PARTIAL` 或失败都保持不推送。
