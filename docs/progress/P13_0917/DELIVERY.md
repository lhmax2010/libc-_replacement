# 提交与推送交接

仅普通推送 `origin` 的 `codex/runtime-validation`，没有 force、Gerrit、平台分支或开发板操作。

| 阶段 | 完整提交 SHA | 推送回执 |
| --- | --- | --- |
| 后端异常边界修复 | df1e90ffbc0843a6ea7523e62a1363b435d92b48 | delivery/005_w1_push.* |
| 回归与扩展异常隔离 | 8f8eebac37ada767b49c9cd76ec39699a9d07b2d | delivery/008_w2_push.* |
| 错误契约与资源清理限制 | e4f6277ec4ad717d689e7fa056461e74b82ff15c | delivery/014_w3_push.* |

第三阶段推送后，`delivery/015_w3_verify.stdout` 记录远端与本地均为上述第三阶段 SHA，命令退出 0。本文件及闭合回执随后单独提交；最终回执提交的远端核对在交接答复报告，原始记录保存在 `tmp/P12/P13_delivery/`，避免自引用。

原始 stdout 含 `/proc/self/maps` 和反汇编工具自带尾空格，完整 `git diff --check` 因此退出 2，见 011 回执；为保留逐字证据没有清理它们。针对作者编写的 Markdown/C/C++/Python 文件复查通过，暂存路径范围复查通过，见 012 回执。

实验代码留在 `tmp/P12/`，最终代码快照、差异、SHA256 和原始实测记录已归档；没有把构建二进制提交。P12 仅更新 REPORT.md 与 DEPLOYMENT.md 的当前状态说明，历史代码和实验记录未改。工作区原有不相关改动保留。

结论仍为 **RELEASE_BLOCKED（原因更新）**。原普通分配异常终止已修复；provider 内部清理保证与析构双异常要求未满足，详细事实和待裁决项见 FINAL.md、W3/ERROR_CONTRACT.md、QUESTIONS.md。完成后停，交人工审阅。
