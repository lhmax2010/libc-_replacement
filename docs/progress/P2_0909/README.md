# QuickBuild 日志拉取工具交付

建议阅读顺序：

1. `REPORT.md`：完成情况、凭据安全与验证结论。
2. `USAGE.md`：可直接复制的使用命令。
3. `VALIDATION.md`：在线拉取、跳过与断点恢复测试记录。
4. `samples/lapack_failure_excerpt.txt`：从下载日志摘出的最小验证片段。
5. `raw/COMMANDS_AND_OUTPUT.md`：不含凭据与会话数据的命令及输出记录。

工具本体位于仓库根目录的 `tools/quickbuild_logs.py`，离线测试位于 `tools/tests/test_quickbuild_logs.py`。
