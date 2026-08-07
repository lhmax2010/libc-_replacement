# R15 策展证据

状态更新（R23，依据 R22）：下述状态仅适用于 R15 当次会话。R22 后续
已实测 SDB 可达，且 `/root`、`/opt/usr`、`/opt/home`、`/home/owner`、
`/var/tmp`、`/opt/var/tmp` 六个路径可写可执行；“无板上执行载体/路径”
不再是当前阻塞。R15 原证据不改写。

本目录是 `progress/R15/` 的可审阅策展副本。结论入口为 `R15_report.md`；连通性、身份、运行时现状、能力边界与可行性判定均保留为 TSV。

`progress/R15/` 是完整证据树，包含 SSH/SDB/ping/路由命令原文与退出码。命令日志不在策展目录重复，见 `EXCLUDED.tsv`；这不表示它们未提交。

本次状态为 `NOT_FEASIBLE_CURRENT_SESSION_NO_EXECUTION_CARRIER`：板 IP 在线，但 SSH/SDB 均不可用，未执行任何板上命令。
