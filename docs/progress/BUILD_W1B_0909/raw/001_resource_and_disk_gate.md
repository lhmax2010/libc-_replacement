# 开工资源与磁盘门禁

命令原文：

```bash
tools/resource_gate.sh --level medium
df -B1 --output=avail,target .
```

退出码：`0`

输出：

```text
level=medium
nproc=20
mem_available_kib=11154180
mem_min_kib=4194304
load1=2.05
load_multiplier=1.0
load_limit=20.000000
disk_path=.
disk_available_kib=75591832
gate_result=PASS
RESOURCE_GATE_EXIT=0
      Avail Mounted on
77406035968 /home
```

结论：资源门禁通过；工作区所在文件系统可用空间 77,406,035,968 字节。
