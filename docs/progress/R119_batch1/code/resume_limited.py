#!/usr/bin/env python3
"""续跑资源包装：串行子进程，30% 地址空间上限，低 CPU/I/O 优先级。

RLIMIT_AS 比物理驻留内存上限更保守，限制由子进程继承；不启动并行工作。
按主机 MemTotal 与当前 cgroup/祖先有效 memory.max 的最小值计算。
"""
import os
import pathlib
import resource
import sys

total = int(next(line.split()[1] for line in pathlib.Path('/proc/meminfo').read_text().splitlines()
                 if line.startswith('MemTotal:'))) * 1024
limits = [('MemTotal', total)]
for line in pathlib.Path('/proc/self/cgroup').read_text().splitlines():
    if line.startswith('0::'):
        path = pathlib.Path('/sys/fs/cgroup') / line[3:].lstrip('/')
        root = pathlib.Path('/sys/fs/cgroup')
        if '..' not in path.parts:
            while path == root or root in path.parents:
                f = path / 'memory.max'
                if f.is_file():
                    value = f.read_text().strip()
                    if value.isdigit():
                        limits.append((str(f), int(value)))
                if path == root:
                    break
                path = path.parent
cap = min(v for _, v in limits) * 30 // 100
resource.setrlimit(resource.RLIMIT_AS, (cap, cap))
print('memory_basis=' + repr(limits), flush=True)
print('RLIMIT_AS_bytes=' + str(cap) + '; parallelism=1; nice=19; ionice=3', flush=True)
os.execvp('nice', ['nice', '-n', '19', 'ionice', '-c', '3', *sys.argv[1:]])
