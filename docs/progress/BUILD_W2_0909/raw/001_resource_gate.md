# 资源门禁记录

命令原文：

```bash
set +e
tools/resource_gate.sh --level medium > /tmp/build_w2_gate.out 2>&1
rc=$?
cat /tmp/build_w2_gate.out
printf 'resource_gate_exit=%s\n' "$rc"
exit 0
```

输出：

```text
level=medium
nproc=20
mem_available_kib=10526208
mem_min_kib=4194304
load1=2.05
load_multiplier=1.0
load_limit=20.000000
disk_path=.
disk_available_kib=78193840
gate_result=PASS
resource_gate_exit=0
```

门禁退出码：`0`。
