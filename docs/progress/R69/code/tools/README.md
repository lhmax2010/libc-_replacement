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

# Source-tree difference gate

`source_tree_diff_gate.py` 固化“上一版已验证源码树”与“本次重放源码树”的逐文件、逐 hunk 比对。它不判断改动是否合理，只要求每个实际差异都有人工填写的任务来源和理由。

先生成差异清单及稳定的 hunk ID：

```sh
tools/source_tree_diff_gate.py \
  --old /path/to/previous-verified \
  --new /path/to/current-replay \
  --include libcxx --include libcxxabi \
  --diff-output progress/tree.diff \
  --hunks-output progress/hunks.tsv \
  --inventory-only
```

说明文件须为 TSV，表头和字段固定为：

```text
hunk_id\ttask\treason
H-0123456789abcdef\tR69\t该 hunk 的变更理由
```

填写后执行正式门禁：

```sh
tools/source_tree_diff_gate.py \
  --old /path/to/previous-verified \
  --new /path/to/current-replay \
  --include libcxx --include libcxxabi \
  --diff-output progress/tree.diff \
  --hunks-output progress/hunks.tsv \
  --rationale progress/rationale.tsv
```

退出码为 `0` 表示所有 hunk 均有且只有一条有效说明；`2` 表示存在无说明差异或已失效的多余说明；`10` 表示参数、输入或脚本自身错误。默认忽略 `.git`、`__pycache__` 和 `*.pyc`，可用重复的 `--exclude NAME` 追加生成物目录名。`--include` 可重复指定要完整比较的源码子树；不指定时比较输入树的全部内容。调用方不得用已知文件清单代替完整的受影响源码子树。

调用方必须保存完整 diff、hunk 清单、说明文件和运行输出，并逐条人工审阅说明内容。只有正式门禁返回 `0` 才能继续交付；`--inventory-only` 仅用于准备说明文件，不代表门禁通过。
