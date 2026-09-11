# aarch64 候选 GCC / 非 libc++ 干净构建命令

命令原文：

```bash
nice -n 15 ionice -c 3 gbs -c /home/toolchain/development/libc++_replacement/config/gbs.conf build -A aarch64 -B /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/BUILD-W1B-llvm-aarch64 -C --fail-fast --threads 1 --define '_smp_mflags -j2' --define 'mlgo_build_jobs 2' --release 111.1 --style git --include-all --packaging-dir packaging --spec llvm.spec
```

构建开始时间：`2026-09-11T10:00`（Asia/Shanghai）；构建根内完成时间为
12:26:22。完成 `7546/7546` 并写包，耗时 8664 秒，`GBS_EXIT=0`。
产品核查见 `006_aarch64_candidate_product_check.md`。

配置阶段已观测：

```text
-DCMAKE_C_COMPILER=aarch64-tizen-linux-gnu-gcc
-DCMAKE_CXX_COMPILER=aarch64-tizen-linux-gnu-g++
-- The C compiler identification is GNU 14.2.0
-- The CXX compiler identification is GNU 14.2.0
-- Performing Test LLVM_USES_LIBSTDCXX - Success
```
