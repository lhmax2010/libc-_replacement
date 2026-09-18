# 复用的历史失败原始记录

来源目录：`progress/BUILD_STATIC_0917B/resume_0917/`。这些不是本轮新实验，不更改原文件。

- `copy_rsync.strace`、`copy_cp.strace`、`copy_read_diagnostic.json`：此前同一 `llvm-opt-report` 文件的 rsync/cp 对照。rsync 的 read 返回 EPERM、退出 23；cp 退出 0、来源与副本哈希一致。底层环境原因仍未查明。
- `llvm-arm-prior-build.log`：来源 `cells/llvm-arm-rpm-serial/build.log`，包含旧整轮 RPM 的 cpio 错误。该命令旧退出码为 1，不因本轮保存或后续重试而改变。

本轮复制命令和 SHA256 在 raw 日志中。若阶段归档将大日志压为 `.gz`，解压后的哈希由阶段 MANIFEST.json 记录。
