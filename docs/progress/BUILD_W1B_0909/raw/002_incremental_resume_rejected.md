# 增量断点尝试及技术性失败

## 命令原文

```bash
nice -n 15 ionice -c 3 gbs -c /home/toolchain/development/libc++_replacement/config/gbs.conf build -A aarch64 -B /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/BUILD-W1-0908-llvm-aarch64 --incremental --noinit --fail-fast --threads 1 --define '_smp_mflags -j2' --define 'mlgo_build_jobs 2' --release 109.3 --style git --include-all --packaging-dir packaging --spec llvm.spec
```

退出码：`1`

## 现象

GBS 使用 `rpmbuild --short-circuit -bc` 进入原构建根；CMake 实际寻找：

```text
/bin/aarch64-tizen-linux-gnu-clang
/bin/aarch64-tizen-linux-gnu-clang++
```

二者在当前构建根中不存在，CMake 配置阶段退出。该状态与上一轮记录的
GCC 断点不一致，不能作为 GCC 路径或非 libc++ 路径证据。

## 修正

不修改源码、spec 或判据；弃用被污染的增量状态，改用新的独立构建根和
`-C` 干净构建。该非零属于构建根残留导致的技术性失败，不是产品判据失败。
