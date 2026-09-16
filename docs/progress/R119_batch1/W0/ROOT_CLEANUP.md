# 本次探针的系统诊断清理

人工授权只用于本次诊断清理，未运行 root 探针、未改系统配置或库。

唯一删除文件：`/opt/usr/share/crash/dump/callback_gnu_8998_20260916202320.zip`，135,299 字节，SHA256 `e574958dfe2242f5307c97829ad66f08986555533424be60c8c9345fdf0a67b8`。

归属证据：ZIP 的 `proc/8998/cmdline` 是本次 `callback_gnu`、本次隔离 LWE 路径与 `no-render` 参数；`cwd` 是 `/opt/usr/home/owner/share/tmp/r119`，对应本次失败 PID 8998。仅读取归属所需条目，未打印环境内容。见 [归属记录](DIAGNOSTIC_ATTRIBUTION.json)。

备份在工作区 `tmp/R119_batch1/callback_gnu_8998_20260916202320.zip`，已核对 SHA256，故删除可由此备份恢复。备份不进 git。

全部 root 操作如下；每条命令原文、stdout、stderr、退出码在同名 raw 文件中，全部退出 0：

| 原始记录 | 操作 |
| --- | --- |
| ../raw/049_sdb_root_on | 开启 SDB root |
| ../raw/050_root_diagnostic_inventory | 核实身份与诊断文件清单 |
| ../raw/051_root_target_hash | 核实唯一目标的大小、时间、SHA256 |
| ../raw/052_root_pull_target | 下载该唯一诊断备份 |
| ../raw/055_root_delete_verified_diagnostic | 精确删除该路径，并核验不存在 |
| ../raw/056_root_inventory_after | 重新清点诊断文件 |
| ../raw/057_sdb_root_off | 关闭 SDB root |

以上 raw 位于本报告上一级的 `raw/`，编号可直接检索。`058_owner_restored` 确认 owner 身份恢复、本次上传目录不存在；`059_verify_cleanup` 比较两次清单：7,228 → 7,227，仅上述一项删除，无新增，其余路径/大小/mtime 无变化。所有不属于本次或归属不确定的诊断均未动。

**板子已清理并释放。** 这里只核实身份、任务目录和诊断清单，不声称动态系统进程与开工前逐字节相同，也没有重写旧的 W0 原始证据。
