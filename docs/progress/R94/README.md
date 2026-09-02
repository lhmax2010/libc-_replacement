# R94 材料索引

- `REPORT.md`：完整结论、机制分析、A/B 实构建、替代选项和误判复盘。
- `tables/build_matrix.tsv`：构建矩阵。
- `evidence/KEY_EVIDENCE.md`：关键日志摘录及其原始文件位置。
- `raw/`：命令原文、stdout、stderr、退出码；GBS 完整输出未裁剪。
- `code/run_logged.sh`：本任务日志包装器。

测试用 detached worktree 与 GBS 构建根体积较大，保留在工作区
`progress/R94/work/` 与 `tmp/GBS-ROOT/R94-*`，不纳入项目仓提交。
