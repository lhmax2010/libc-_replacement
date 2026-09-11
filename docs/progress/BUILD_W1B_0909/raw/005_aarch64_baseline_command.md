# aarch64 同环境基线构建命令

命令原文：

```bash
nice -n 15 ionice -c 3 gbs -c /home/toolchain/development/libc++_replacement/config/gbs.conf build -A aarch64 -B /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/BUILD-W1B-llvm-aarch64 -C --fail-fast --threads 1 --define '_smp_mflags -j2' --define 'mlgo_build_jobs 2' --release 111.1 --style git --include-all --packaging-dir packaging --spec llvm.spec
```

执行目录：
`/home/toolchain/development/libc++_replacement/tmp/BUILD_W1B_0909/llvm-baseline`

构建开始时间：`2026-09-11T12:27`（Asia/Shanghai）；构建根内开始时间为
12:29:55，完成时间为 14:55:51。完成 `7546/7546` 并写包，耗时
8756 秒，`GBS_EXIT=0`。同环境对照见
`008_aarch64_same_environment_compare.md`。

候选与基线使用同一份 `config/gbs.conf`、同一构建根路径（候选完成并复制
RPM 后以 `-C` 清理重建）、同一 release `111.1`、相同并行度与资源优先级。
基线源码与候选的父树均为
`2d23367d74afbf2bb1e9e4013fce072b3a154109`；二者只相差候选工作树中的
打包层改动和三份新资产。
