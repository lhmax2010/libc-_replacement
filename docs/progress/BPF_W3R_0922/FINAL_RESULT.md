# W3R最终结果

**需先申报操作失误：** 本轮llvm-objcopy提取.text时漏指定独立输出ELF，实际改写了四份既有tmp主包解包副本，违反第二部分只读要求。已停止使用、未擅自恢复。原RPM校验未变，随后内存只读解析复核原ELF/.text数据相符；本轮板测static ELF不在受影响四份之内。详见[INCIDENT.md](INCIDENT.md)。任务不能标为无违规完成。

可写区物理板验证通过；此前正常安装预检的只读根分区条件已确认并与固定RPM源码对上。构建时长完成核对，但ARM提速的唯一原因与主包.text差异仍未归因；不得把原生加速线索写成已完成的历史性能归因。**rpm安装验证留待镜像阶段**。本轮不构建、不改spec、不推包仓、不起QuickBuild。

## 一、预检归因

| 项 | 结论 | 证据 |
|---|---|---|
| 上传目录 | /var/tmp→/opt/var/tmp，/dev/mmcblk0p3 rw ext4 | MOUNTS.tsv |
| 安装目标 | /usr、/etc属/dev/mmcblk0p2，ro ext4 | board-root-read/mounts.out |
| df与92MB | 物理空闲不等于RPM可写可用空间；只读时RPM按0块计算 | transaction.c:164、269–272；rpmprob.c:143–151，完整原文见INSTALL_CAUSE.md |
| Size旁证 | 90,882,270字节≠92MiB；5%余量与取整不能省略 | INSTALL_CAUSE.md |
| 源码版本边界 | 实为固定上游4.14.1提交；不冒称部署4.14.1.1源码身份证明 | INSTALL_CAUSE.md |

## 二、构建时长

| 项 | 结论 | 证据 |
|---|---|---|
| ARM两轮 | 119.17min→10.02min，840进度公告相同 | history/TIMINGS.json |
| aarch64两轮 | 13.37min→10.00min，并非两小时→十分钟 | 同上 |
| ccache | 指定目录与PATH未找到，命中数NOT_OBSERVED | build-audit/ |
| 当前原生加速 | 只读--version确实execve至/emul原生Clang；非历史逐目标证明 | compiler-read/ |
| 主包身份 | ARM ELF/.text均不同；aarch64均相同 | BUILD_TIMING.md |
| 完整提速归因/同期其它负载 | NOT_OBSERVED；缺历史编译进程映像、同期连续负载/缓存证据 | BUILD_TIMING.md |

## 三、物理板成对结果与取消

| 项 | 候选/共享 | 原件/静态对照 | 结论 |
|---|---|---|---|
| RPM解包ELF身份 | 匹配指定6e110b9c… | 原件ade7530b…匹配 | PASS |
| --version / --info / -l | 各退出0 | 各退出0 | PASS；info仅LLVM版本不同 |
| BEGIN / tracepoint / interval | 各3/3退出0 | 各3/3退出0 | 已覆盖用例等价 |
| 取消探针 | 共享20/20通过 | 静态20/20通过 | 实际数据；不外推x86_64结果 |
| 清理/系统状态 | 工作目录已删、无安装、/usr/bin/bpftrace*不存在 | 起止挂载一致、root已关闭 | PASS |
| RPM安装 | NOT_OBSERVED（本轮禁止） | — | 留待镜像阶段 |

逐项命令、退出码、stdout/stderr、耗时与前50行见 [BOARD_RESULT.md](BOARD_RESULT.md)。推送前只提交本轮证据和LINE_STATUS，不提交大二进制。方法选择与疑问见 [DECISIONS.md](DECISIONS.md)。W4输入沿用上一轮六份RPM及配方，不重新产出或替换；本轮板测是其中ARM static RPM的实际解包载荷。
