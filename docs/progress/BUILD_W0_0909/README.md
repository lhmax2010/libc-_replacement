# BUILD_W0_0909

本目录保存 QuickBuild 与相关站点匿名访问验证材料。

- `REPORT.md`：中文结论、访问结果、登录形式与后续影响。
- `ACCESS_RESULTS.tsv`：六个 URL 的机器可读结果。
- `raw/COMMANDS_AND_OUTPUT.md`：命令原文、逐次输出与退出码。
- `raw/SUBMISSION.md`：主材料提交、推送与远端 SHA 核对记录。
- `samples/`：一份登录表单摘录、两个公开目录索引以及六条请求的响应头链；响应头副本已删除服务器下发的 `Set-Cookie` 行，所有文本样本已规范化为 LF 并删除行尾空白。

建议先读 `REPORT.md`，再用 `ACCESS_RESULTS.tsv` 核对逐 URL 结果，最后按需查看原始命令和样本。
