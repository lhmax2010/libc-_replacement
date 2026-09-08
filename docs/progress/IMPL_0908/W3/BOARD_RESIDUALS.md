# 实板失联后的残留与断点

状态：`CLEANUP_BLOCKED`。本轮未完成板端收尾清理，不能宣称无残留。
最后一次恢复尝试见 raw/102、103：既有目标查询返回 `target not found`；显式连接返回
`error: failed to connect to remote target '192.168.108.26'`。后者主机退出码为 0，
但文本明确失败，未据退出 0 继续板上操作。已使用三次恢复尝试，不再重启测试。

## 已创建、最后可达时仍存在的本批路径

| 路径 | 内容与核验边界 |
|---|---|
| `/var/tmp/impl_0908` | 两套隔离运行库、定向探针、SHA256SUMS；第二次恢复时原 16 文件摘要通过 |
| `/opt/usr/impl_0908_lit` | 新／旧运行库、官方测试夹具及逐项工作目录；失联时进程与最终文件状态 `NOT_OBSERVED` |
| `/opt/usr/home/owner/share/tmp/impl_0908_payload.tar` | 主矩阵部署包 |
| `/opt/usr/home/owner/share/tmp/impl_0908_old_timed` | 补充定时探针；上传后主机与板端 SHA256 一致 |
| `/opt/usr/home/owner/share/tmp/impl_0908_run-7610e781ded34a4487806c1f82e90d08.tar` | 首次缺失返回码标记前已成功上传的包，执行器因故障未执行 finally 清理 |
| `/opt/usr/home/owner/share/tmp/impl_0908_run-827fe9828f724efa880fdae3fdc519ac.tar` | 第二次缺失返回码标记前已成功上传的包，同上 |

发现阶段对 `impl_0908_run-966adc7e416d4154a9a536a4878eb104.tar` 的上传返回主机 1、
`target not found`；上传成功未观测到，不能把它列为已确认存在的文件。
已启动的全部主机 lit 控制器都已结束；失联后的板上进程状态无法核验。

## 系统崩溃诊断

板端 `/proc/sys/kernel/core_pattern` 为 crash-manager 管道，测试设置 core 大小为 0
仍可能留下诊断 ZIP。已只读查见以下四个当日候选（raw/093–096）：

- `/opt/usr/share/crash/dump/t.tmp.exe_12031_20260908112709.zip`
- `/opt/usr/share/crash/dump/t.tmp.exe_28667_20260908114329.zip`
- `/opt/usr/share/crash/dump/t.tmp.exe_28747_20260908114332.zip`
- `/opt/usr/share/crash/dump/t.tmp.exe_16996_20260908131255.zip`

最后一个 ZIP 内的进程 cwd 已实查为本批官方工作目录，作为归属方法的正向对照。
其余三个尚未逐个读取 cwd；不能仅按日期和同名程序认领。
后续的析构同步测试又出现断言失败，最终诊断包总量 `NOT_OBSERVED`。
**没有执行诊断包下载或删除，主机备份也未建立。**

## 保存的清理步骤（未执行）

`code/board_cleanup.py` 与 `code/cleanup_crash_reports.py` 是待恢复后的核查/清理脚本快照，
不是已执行凭证；当前前置检查会拒绝未核实的失联状态，不能直接当作成功命令重放。
重新获得通道后，须先核实身份、活动进程、上述精确路径及文件摘要，再删除本批目录/上传件。
诊断包须逐个读取 cwd 确认本批归属，先保存并验证主机副本，再逐个删除；保留其他任务历史诊断。
本记录不要求删除整个 `/opt/usr/share/crash/dump`，也不授权修改系统库或配置。

正式套件断点见 [未观测项](ARM_UNOBSERVED.tsv)：3,554 项，其中 39 项只有基础设施失败、
3,515 项尚未执行。不可用 QEMU 结果替补物理板缺口。
