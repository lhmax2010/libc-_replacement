# x86_64 增量断点尝试及拒用理由

命令原文：

```bash
nice -n 15 ionice -c 3 gbs -c /home/toolchain/development/libc++_replacement/config/gbs.conf build -A x86_64 -B /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/BUILD-W1-0908-llvm-x86_64 --incremental --noinit --fail-fast --threads 1 --define '_smp_mflags -j2' --define 'mlgo_build_jobs 2' --release 109.3 --style git --include-all --packaging-dir packaging --spec llvm.spec
```

退出码：`1`。构建在 51 秒的 CMake 编译器探针处失败。

实际状态同时包含：

```text
C compiler: /bin/x86_64-tizen-linux-gnu-gcc
link flags: -Wl,--no-as-needed -lc++abi -Wl,--as-needed
CMAKE_ASM_COMPILER: /bin/aarch64-tizen-linux-gnu-clang (不存在)
collect2: fatal error: cannot find 'ld'
```

这不是自洽的 x86_64 GCC/非 libc++ 状态，说明旧断点混有先前配置缓存。
因此该非零只记为技术性构建根失败，不用于产品判定；随后改用新的独立
构建根和 `-C` 干净构建。
