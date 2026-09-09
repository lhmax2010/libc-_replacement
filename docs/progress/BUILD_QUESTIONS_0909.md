# 2026-09-09 至 2026-09-10 编译适配线疑问

## W1

任务要求“不启用 libc++ 时资产来源与构建产物与现状完全一致”。armv7l
已证明资产树、接口、标准库身份和禁止参数一致，但候选与跨日期固定快照
的依赖及动态符号集合存在差异。本文保守标为 `PARTIAL`。

需确认后续闭合口径：

- 若要求整个产物对照，应在同一固定仓集合和同一环境中分别构建未改
  baseline 与候选，再做逐项比较；
- 若只要求本次条件分支影响范围内的资产、接口和标准库身份等价，则现有
  armv7l 证据已经覆盖，但这不是本轮自行放宽的判据。

## W4

- 原目标是总部 armv7l build 1165448 的 17 包，但该 build 日志不可得；
  可访问的 build 1165447 只有 aarch64/x86_64。需人工确认后续是否还可能
  提供 1165448 的离线日志，以闭合 armv7l 的逐包同因核对。
- `hdf5` 的完整 package log 没有 `config.log` 内容，只显示 Fortran
  `SIZEOF` / `STORAGE_SIZE` 最终判据失败。要确认全局 LDFLAGS 是否是底层
  原因，需要该次构建的 `config.log` 或等价的 configure probe 原始输出。
- aarch64 GCC 的复用本地 driver 连 `--version` 都退出 139；实际
  QuickBuild 日志已证明 `-stdlib=libc++` 会被 aarch64 GCC 明确拒绝，
  但三个其余链接选项的 aarch64 逐项 trace 仍为 `NOT_OBSERVED`。若必须
  闭合这一格，需要可正常运行的 aarch64 GCC buildroot。
