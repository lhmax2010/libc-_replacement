# R44a：资源门禁退出码 1 的性质核查与工具固化

## 结论

R44 的退出码 1 是门禁脚本自身的运行时错误，不是内存或负载阈值触发。本次 R44 红停定性为 `FALSE_RED_STOP`。

固定工具已建立为 `tools/resource_gate.sh`，支持 `light`、`medium`、`heavy` 三档；退出码 0、10、20、21 的受控自验全部符合定义。`shellcheck` 在当前主机不可得，已登记 `NOT_AVAILABLE`。

本任务没有重跑 R44，没有构建、测试或连接开发板。

## 1. R44 原始记录

原始文件未经修改复制到：

- `progress/R44a/evidence/R44_raw/resource_gate_dry_run.raw.log`
- `progress/R44a/evidence/R44_raw/gate_dry_run/preflight.raw.log`
- `progress/R44a/evidence/R44_raw/resource_gate_syntax.raw.log`
- `progress/R44a/evidence/R44_resource_gate.sh`

完整逐行呈现及命令退出码另见：

- `progress/R44a/raw/evidence_inspection.raw.log`
- `progress/R44a/raw/r43_r44_source_numbered.raw.log`

关键原文是：

```text
progress/R44/tools/resource_gate.sh: line 24: PIPESTATUS[1]: unbound variable
```

干跑调用记录为退出 1；此前的 `bash -n` 为退出 0。因此这是运行时错误，不是语法错误。

## 2. 出错位置与原因

R44 脚本相关代码：

```text
22  top -bn1 | head -20
23  top_rc=${PIPESTATUS[0]}
24  head_rc=${PIPESTATUS[1]}
```

第 22 行产生两个元素的 `PIPESTATUS`。第 23 行的单独赋值命令执行后，Bash 将 `PIPESTATUS` 更新为该赋值命令自身的单元素结果；第 24 行再读取索引 1 时，该元素已经不存在。脚本设置了 `set -u`，所以 Bash 报 `unbound variable` 并退出 1。

该行为的最小复现已落盘于 `progress/R44a/raw/pipestatus_reproduction.raw.log`：赋值前数组长度为 2，赋值后为 1、索引 1 为 `UNSET`。

错误发生在阈值数据计算和判定之前，因此 `gate_values.tsv` 与 `gate_result.tsv` 当时没有生成。

## 3. 阈值是否触发

R44 当时原始观测：

| 项目 | 实测 | 轻档阈值 | 判定 |
|---|---:|---:|---|
| CPU 核数 | 20 | — | — |
| 可用内存 | `free -h` 为 15 GiB；`top` 为 15622.7 MiB | 2 GiB | 未触发 |
| 1 分钟负载 | 4.39 | 30.0（1.5 × 20） | 未触发 |
| 磁盘可用 | 469 GiB | 本任务未设磁盘红线 | 仅记录 |

所以退出 1 与资源阈值无关；R44 是假红停。

## 4. R43 与 R44 回溯

| 轮次 | 脚本 SHA256 | 仓内可核验退出码 |
|---|---|---:|
| R43 | `841e1c28b2a30bd5c48b6e8ab2110c1b5afc60915e20a8ace4840aeb11d2ee32` | 20 |
| R44 | `bd927f50c64fb67369e381ef518d00cd5341ccf0a0ab3fdb88f8cf3a91405e99` | 1 |

两份脚本 SHA 不同，完整 diff 位于 `progress/R44a/raw/evidence_inspection.raw.log`，确认 R44 重新编写了门禁脚本。

任务背景将 R43 记为退出 21，但当前仓内 R43 原始记录 `progress/R43/commands/002_resource_gate_driver.log` 明确记载 `EXIT_CODE=20`，对应 `MemAvailable=7934588 KiB` 低于当时 8 GiB 阈值；在已搜索的 R43/R43b 范围内没有找到退出 21 的运行记录。因此本报告不以背景值替代原始记录，登记为 `NOT_CONFIRMED`。

R43 脚本在 `printf` 的同一次参数展开中读取 `${PIPESTATUS[0]}`，没有 R44 的两次连续赋值问题；R44 改为分别赋值后引入了此次错误。

## 5. 固定工具

新增：

- `tools/resource_gate.sh`
- `tools/README.md`

档位：

| 档位 | 内存下限 | 负载上限 |
|---|---:|---:|
| light | 2 GiB | 1.5 × 核数 |
| medium | 4 GiB | 1.0 × 核数 |
| heavy | 8 GiB | 0.8 × 核数 |

退出码：

| 退出码 | 含义 |
|---:|---|
| 0 | 通过 |
| 10 | 参数、依赖或脚本自身错误 |
| 20 | 内存不足 |
| 21 | 负载过高 |

工具设置 `set -u` 与 `set -o pipefail`，校验参数和 `nproc`、`awk`、`df` 依赖；所有结果均输出档位、CPU 数、可用内存、负载、磁盘空间和阈值。依赖缺失时，对不可取得的实测字段明确输出 `NOT_AVAILABLE`，并返回 10。脚本不自行重试。

调用方义务已写入 `tools/README.md`：必须显式检查退出码；任何非零值均须退出整个任务，不得继续任何后续步骤，包括只读命令。

## 6. 自验

自验汇总见 `progress/R44a/selftest_results.tsv`，完整输出位于 `progress/R44a/selftest/`。

- `bash -n`：退出 0。
- `shellcheck`：当前主机命令不存在，`NOT_AVAILABLE`。
- light / medium / heavy：均退出 0。
- 非法档位：退出 10。
- 子 shell 临时清空可搜索依赖命令的 PATH：退出 10。
- 子 shell 设置 `RESOURCE_GATE_MEM_MIN_KIB=999999999999`：退出 20。
- 子 shell 设置 `RESOURCE_GATE_LOAD_MULTIPLIER=0`：退出 21。

阈值覆盖只作用于相应子进程，没有修改脚本默认值、系统环境或持久配置。
