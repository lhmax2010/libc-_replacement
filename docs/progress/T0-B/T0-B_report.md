# T0-B 板级基线打捞报告

状态：`RED_STOP_BOARD_UNREACHABLE`。

任务开始时磁盘守卫通过（`progress/T0-B/00_disk_guard.log:1`）。使用非交互 SSH、8 秒连接超时、禁用 known_hosts 写入的只读 `true` 探针连接两台板；`192.168.108.26` 与 `192.168.108.25` 均在 TCP/22 立即返回 `Connection refused`，SSH 退出码均为 255（`progress/T0-B/192.168.108.26/00_ssh_root_probe.log:1`；`progress/T0-B/192.168.108.25/00_ssh_root_probe.log:1`）。

连接拒绝发生在认证前，改换用户名不会改变该结果。按规格，两台均记 `NOT_AVAILABLE`；两台都不可达，因此本任务红停。未改走 SDB、端口转发或其他替代路径，也未在板上执行任何项目制品或写命令。

板可用性见 `progress/T0-B/board_availability.tsv:1`。由于没有板端原文，包集差异、C++ 运行时差异、版本差异及代表性二进制 `ldd -r` 全部明确记为 `NOT_RUN_BOARD_UNREACHABLE`（`progress/T0-B/board_comparison.tsv:1`），不作推断、不判良恶。
