# Resource gate

`resource_gate.sh` 是仓内固定的单次资源门禁。调用方选择档位，脚本输出实测资源与阈值并返回专用退出码；脚本本身不重试。

```sh
tools/resource_gate.sh --level light
tools/resource_gate.sh --level medium
tools/resource_gate.sh --level heavy
```

| 档位 | 适用范围 | 可用内存下限 | 1 分钟负载上限 |
|---|---|---:|---:|
| `light` | 只编测试件、只读分析、板上运行 | 2 GiB | 1.5 × CPU 核数 |
| `medium` | 重链、少量目标重编 | 4 GiB | 1.0 × CPU 核数 |
| `heavy` | 重建 runtimes、完整构建 | 8 GiB | 0.8 × CPU 核数 |

| 退出码 | 含义 |
|---:|---|
| 0 | 门禁通过 |
| 10 | 参数、依赖命令或脚本自身错误 |
| 20 | 可用内存不足 |
| 21 | 1 分钟负载过高 |

调用方必须显式检查退出码。任何非零退出码都必须使整个任务退出，不得继续执行任何后续步骤，包括只读命令。重试策略由调用方实现，不得依赖门禁脚本自行重试。

以下环境变量仅用于受控自验或指定磁盘测量路径；常规任务不应覆盖默认阈值：

- `RESOURCE_GATE_MEM_MIN_KIB`
- `RESOURCE_GATE_LOAD_MULTIPLIER`
- `RESOURCE_GATE_DISK_PATH`
