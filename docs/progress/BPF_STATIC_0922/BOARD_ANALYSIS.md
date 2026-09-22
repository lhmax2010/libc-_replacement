# ARM物理板：正常安装预检阻断

## 结论

W3实际止于 **static RPM正常安装预检失败**。未执行正式安装，未运行候选/原件功能对照，也未运行取消探针；这些项全部 NOT_OBSERVED，实际执行次数为0，不是“0/20失败”，更不是“双方同样失败故等价”。不满足候选可替换发布资产的完整门禁。

## 实际输入与环境

- 物理板：192.168.108.26，`uname -a` 为 `Linux localhost 6.12.80-arm-rpi4-v7l #1 SMP Tue Jul 28 02:41:25 UTC 2026 armv7l GNU/Linux`。
- `/proc/version` 实际含 `clang version 22.1.8, LLD 22.1.8`；原文 board/009.transport.out、010.transport.out。
- root模式下读取 `/proc/config.gz` 或 `/boot/config-*` 未获得配置，退出1，BPF内核配置 NOT_OBSERVED（board/kernel_config.json），不按版本猜测。
- static RPM传输前后 SHA相同：`672279ea1e93f896c6b82cd1d1b5d557ef6c8a8dcc9079793b2a2efe11f9e756`，22,338,628字节。
- 原ARM资产也已传输，SHA `ade7530b3824dd83eebba837928d3919dcb4cd7708212a8607c3629cb5c55f5b`；没有覆盖系统文件。全部传输见 board/transfers.json。

## 正常预检的实际结果

主包/common预检：

```sh
rpm -ivh --test /var/tmp/bpf_static_0922/bpftrace-0.24.2-1.armv7l.rpm /var/tmp/bpf_static_0922/bpftrace-common-0.24.2-1.armv7l.rpm
```

远端退出2，报告缺少 libbcc_bpf.so.0、libc++.so.1、libc++abi.so.1、libclang-cpp.so.22.1、libclang.so.22.1 及相应版本依赖。这是主包预检结果，不能记成static ELF失败。未安装主包或common。

static预检：

```sh
rpm -ivh --test /var/tmp/bpf_static_0922/bpftrace-static-0.24.2-1.armv7l.rpm
```

远端退出1，实际输出末尾为：

```text
error: Unable to write device security policy to /etc/device-sec-policy
warning: Plugin msm: hook tsm_post failed
        installing package bpftrace-static-0.24.2-1.armv7l needs 92MB on the / filesystem
```

完整原始输出（含RPM进度控制字符）在 board/INSTALL_PREFLIGHT.json 及 board/commands.json 对应stdout文件。没有关闭MSM插件、没有 `--nodeps`、没有修改安全策略/权限/挂载、没有重试预检。

只读收尾观测：`df -Pk / /etc /usr/bin /var/tmp` 报 `/` 可用 1,393,876 KiB；`/etc/device-sec-policy` 为 root:root、0644、65,379字节。预检时 `id` 是 uid0，安全上下文原文为 `User::Shell`。这些观测与“需92MB”的错误一并保留，**不足以断言根因是磁盘不足、只读挂载或某种MAC权限问题**；本轮未继续归因，根因记 NOT_OBSERVED。

## 对照与取消：未执行清单

| 项 | 候选 | 原件/对照 | 原因 |
|---|---|---|---|
| --version | NOT_OBSERVED | NOT_OBSERVED | 正常安装预检未通过 |
| --info逐行对照 | NOT_OBSERVED | NOT_OBSERVED | 同上 |
| -l前50行与实际探针选择 | NOT_OBSERVED | NOT_OBSERVED | 同上，不按包名或旧结果选探针 |
| BEGIN脚本，各3次 | NOT_OBSERVED | NOT_OBSERVED | 同上 |
| 实际kprobe/tracepoint脚本，各3次 | NOT_OBSERVED | NOT_OBSERVED | 同上 |
| interval脚本，各3次 | NOT_OBSERVED | NOT_OBSERVED | 同上 |
| 共享libgcc取消，计划20次 | NOT_OBSERVED（0次执行） | — | 未进入探针阶段 |
| 静态libgcc取消，计划20次 | — | NOT_OBSERVED（0次执行） | 未进入探针阶段 |

两份探针的本地SHA仍与既有W3_REFERENCE_INPUTS一致；本地身份匹配不是本轮板测结果。

## 板端收尾与保留现场

- 未安装任何bpftrace相关包，无包需要卸载。失败后rpm包名检查仍无bpftrace/main/common/static。
- 预检前后五个受影响路径均为 ABSENT：`/usr/bin/bpftrace`、`/usr/bin/bpftrace-static`、`/usr/bin/bpftrace-aotrt`、`/usr/share/licenses/bpftrace-static`及其LICENSE。board/before.json与after.json逐项相同。
- 已把本轮开启的SDB root模式关闭，恢复装前非root模式；PC sudo调用为0。root操作、命令与退出码完整保存在board/commands.json及board/failure-state/commands.json。
- 按“失败现场保留”要求，**没有删除** `/var/tmp/bpf_static_0922/`：保留主包/common/static三个RPM、run-bounded、original，共五个文件；各文件SHA收尾再次核对一致。完整路径与SHA在board/CLEANUP.json。
- 因目录保留，清理状态是 `PRESERVED_FAILURE_SCENE`，不能写成“全部清理完成”。没有修改原Source1002，没有推包仓。
- 包预检本身曾调用MSM插件；本轮未保存预检前全系统文件快照，不宣称全系统所有文件均未改变。已核实的是上述包和五个bpftrace相关路径未变、SDB root模式恢复。

下一步需人工先裁决如何查清正常安装预检阻断；不得借本轮W2R成功直接跳过W3或启动QuickBuild。后续仍为补齐W3 → W4审阅配方/签字 → 经授权推包仓与QuickBuild。
