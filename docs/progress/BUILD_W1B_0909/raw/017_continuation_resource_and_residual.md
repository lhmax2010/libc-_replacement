# 续跑资源门禁与残留构建根

命令原文：

```bash
date -Ins
./tools/resource_gate.sh --level medium
echo RESOURCE_GATE_EXIT=$?
df -h /home/toolchain/development/libc++_replacement
find tmp/BUILD_W1B_0909/rpms/x86_64_candidate -maxdepth 1 -type f -name '*.rpm' | wc -l
test -d tmp/GBS-ROOT/BUILD-W1B-llvm-aarch64
```

输出：

```text
2026-09-11T18:12:55,637097615+08:00
level=medium
nproc=20
mem_available_kib=5934388
mem_min_kib=4194304
load1=2.84
load_multiplier=1.0
load_limit=20.000000
disk_path=.
disk_available_kib=44379972
gate_result=PASS
RESOURCE_GATE_EXIT=0
/dev/sda1       1.8T  1.7T   43G  98% /home
X86_CANDIDATE_RPM_COUNT=12
AARCH64_RESIDUAL=present
```

总命令退出码：`0`。

`ps` 未发现引用 aarch64、x86_64 或本次 armv7l 构建根的活动 abuild/GBS
进程。aarch64 残留位于独立路径
`tmp/GBS-ROOT/BUILD-W1B-llvm-aarch64`；本次使用的构建根分别为
`BUILD-W1BC-llvm-x86_64-baseline` 与 `BUILD-W1-0908-llvm-armv7l`，无路径
重叠。未尝试提权。
