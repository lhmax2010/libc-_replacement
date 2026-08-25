# 命令证据局限

- `002_inventory` 与 `003_read_primary_evidence` 的工具承载调用在输出序列化前中断，退出码无法取得；已分别登记 `NOT_AVAILABLE`，并保留当时取得的 stdout/stderr。其遗留只读进程随后被终止，退出码为 0。
- `012_build_deferred_probes` 与 `012b_monitor_detached_build` 同样在调用承载层超时，原始退出码无法取得，登记 `NOT_AVAILABLE`。首个交叉编译子进程随后因共享盘 I/O wait 被本任务终止；实际产物由 `019` 与 `020` 串行重做并完整记录，二者退出 0。
- `018_check_build_after_five_minutes.command.txt` 只在准备阶段创建，未执行，退出状态登记 `NOT_EXECUTED`；不将它作为证据引用。
- 构建等待期间若干即时资源采样在交互工具回传中可见，但未在执行当时重定向到文件；报告只引用已经逐字落盘的 `001`、`012*`、`014` 等采样，不用未落盘采样支撑结论。
