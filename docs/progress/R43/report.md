# R43：libc++ 标准库自身缺失 forced-unwind 防护的实测

## 执行状态

`RED_STOP_RESOURCE`

本轮在进入静态复核、编译和运行矩阵前触发资源硬门禁，任务主体未执行。门禁实测 `MemAvailable=7934588 KiB`，低于任务规定的 8 GiB（`8388608 KiB`）下限，脚本返回 20，原文见 `resources/preflight_attempt1.log`。

同时测得：`nproc=20`，一分钟负载 `7.45`，负载上限 `16.00`；构建分区可用空间 `466G`。本次红停原因仅为可用内存不足。

## 已执行范围

1. 建立并校验独立资源门禁脚本；`bash -n` 退出 0。
2. 调用门禁并显式保留退出码；门禁返回 20，驱动记录 `RED_STOP_RESOURCE`。
3. 未检索 libstdc++/libc++ 源码，未生成静态计数。
4. 未编译或链接任何测试件，未重建 libc++abi。
5. 未运行 x86_64 或 armv7l 验证矩阵，未得出 `CONFIRMED`、`NOT_CONFIRMED` 或 `PARTIAL` 结论。
6. 未估算 371 包消费面。

## 门禁后的越界只读调用申报

读取门禁结果后，同一轮工具调用中误执行了一次板端只读 preflight。其命令只读取 `uname`、发行版、进程表和 `/var/tmp` 所在文件系统空间；未部署、未写入、未删除、未安装、未修改配置、未重启或启停服务。完整命令、输出与退出码见 `commands/003_board_preflight.log`。

该只读调用不构成 R43 板端验证，也不据此形成任何任务结论。

## 缺失项

| 项目 | 状态 | 原因 |
|---|---|---|
| libstdc++ 防护子句计数与分布 | NOT_OBSERVED | 资源门禁红停前未开始 |
| libc++ 对应位置逐处核查 | NOT_OBSERVED | 同上 |
| x86_64 运行矩阵 | NOT_OBSERVED | 同上 |
| armv7l 板上运行矩阵 | NOT_OBSERVED | 同上 |
| R42 补丁前后对照 | NOT_OBSERVED | 同上 |
| 371 包消费面估算 | NOT_OBSERVED | 同上 |
| R43 三选一结论 | NOT_AVAILABLE | 没有运行时实测证据 |

## 证据索引

- `commands/001_bash_n_tools.log`：门禁及辅助脚本语法检查。
- `commands/002_resource_gate_driver.log`：门禁驱动退出码。
- `resources/preflight_attempt1.log`：资源原始采样与红停判定。
- `commands/003_board_preflight.log`：误执行的只读板端 preflight 完整原文。
- `tools/`：本轮实际使用的脚本源码。

本报告不实现修法、不做方案推荐、不修改任何平台源码、libc++ 或 libc++abi。
