# W1 资源门禁

## 第一次

命令：

```bash
tools/resource_gate.sh --level medium
```

输出：

```text
level=medium
nproc=20
mem_available_kib=21626524
mem_min_kib=4194304
load1=26.95
load_multiplier=1.0
load_limit=20.000000
disk_path=.
disk_available_kib=68929464
gate_result=EXCESSIVE_LOAD
resource_gate_exit=21
```

退出码：`21`。未启动构建。终止本会话此前超时后遗留的两个只读 `git status` 查询，完整等待 10 分钟。

## 第二次

命令：

```bash
tools/resource_gate.sh --level medium
```

输出：

```text
level=medium
nproc=20
mem_available_kib=21662140
mem_min_kib=4194304
load1=0.64
load_multiplier=1.0
load_limit=20.000000
disk_path=.
disk_available_kib=68913772
gate_result=PASS
resource_gate_retry=2
resource_gate_exit=0
```

退出码：`0`。W1 开始。
