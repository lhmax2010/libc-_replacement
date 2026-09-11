# x86_64 候选 GCC / 非 libc++ 干净构建

命令原文：

```bash
nice -n 15 ionice -c 3 gbs -c /home/toolchain/development/libc++_replacement/config/gbs.conf build -A x86_64 -B /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/BUILD-W1B-llvm-x86_64 -C --fail-fast --threads 1 --define '_smp_mflags -j2' --define 'mlgo_build_jobs 2' --release 111.1 --style git --include-all --packaging-dir packaging --spec llvm.spec
```

执行目录：
`/home/toolchain/development/libc++_replacement/tmp/BUILD_W1_0908/llvm-worktree`

开始时间：`2026-09-11T15:01`（Asia/Shanghai）；构建根内开始时间为
15:03，完成时间为 17:35:46。完成 `7634/7634` 并写包，耗时 9149 秒，
`GBS_EXIT=0`。产品核查见 `012_x86_64_candidate_product_check.md`。
