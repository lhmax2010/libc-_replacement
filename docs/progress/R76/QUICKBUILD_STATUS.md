# QuickBuild 状态

- 触发方式：沿用 R7，对 `platform/upstream/llvm` 直推既有 sandbox ref；
- ref：`refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport`；
- 触发提交：`08a64b6eb95ed74339f1d862a6c22a2f93fa78f9`；
- 推送命令/退出码：command 127，退出 0；
- 平台 profile、架构矩阵：`NOT_OBSERVED`；
- 完整构建日志：`NOT_OBSERVED`，当前环境无读取入口，需人工平台查看；
- 构建结果、产物、与本地产物比对、其他包结果、新增编译告警：均
  `NOT_OBSERVED`；
- 重试：0 次；
- 当前结论：`SUBMITTED_BUILD_PENDING`。

Gerrit 在 push 时返回的 subject/行宽 warning 仅是 commit-message 风格提示，
不属于 QuickBuild 编译日志，不能据此判断平台构建结果。
