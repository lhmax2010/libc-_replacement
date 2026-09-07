定位命令对整个历史开发目录执行 `find`，30 秒内未结束；为避免无界扫描，按 PID 精确发送 SIGTERM。runner 在写退出码前被终止，因此 `023_locate_qemu_and_aarch64_root.exitcode` 按 shell 的 SIGTERM 退出约定补记为 143。随后改用冻结仓 package record 的 SHA256 直接定位解包目录。
