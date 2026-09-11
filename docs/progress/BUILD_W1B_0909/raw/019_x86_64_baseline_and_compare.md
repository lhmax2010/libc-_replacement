# x86_64 基线构建与同环境对照

首次启动因日志目录按错误相对路径创建，在进入 GBS 前失败：

```text
script: cannot open .../tmp/BUILD_W1BC_0911/logs/x86_64_baseline_build.typescript: No such file or directory
EXIT_CODE=1
```

修正为绝对日志路径后执行：

```bash
nice -n 15 ionice -c 3 gbs \
  -c /home/toolchain/development/libc++_replacement/config/gbs.conf build \
  -A x86_64 \
  -B /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/BUILD-W1BC-llvm-x86_64-baseline \
  -C --fail-fast --threads 1 --define '_smp_mflags -j2' \
  --define 'mlgo_build_jobs 2' --release 111.1 --style git \
  --include-all --packaging-dir packaging --spec llvm.spec
```

完整输出：`../build_logs/x86_64_baseline_0911.typescript.gz`。终态：

```text
[10270s] toolchain-2026 finished "build llvm.spec"
=== Total succeeded built packages: (1) ===
Script done ... [COMMAND_EXIT_CODE="0"]
```

比较命令：

```bash
nice -n 15 ionice -c 3 \
  docs/progress/BUILD_W1B_0909/code/compare_same_environment_rpms.sh \
  tmp/BUILD_W1B_0909/rpms/x86_64_candidate \
  tmp/GBS-ROOT/BUILD-W1BC-llvm-x86_64-baseline/local/repos/tizen_unified_standard/x86_64/RPMS \
  x86_64 tmp/BUILD_W1BC_0911/compare/x86_64
```

完整输出与退出码：

```text
packages_diff_lines=0
file_lists_diff_lines=0
stdlib_requires_diff_lines=0
elf_needed_diff_lines=0
dynamic_symbols_diff_lines=0
SAME_ENVIRONMENT_RPM_COMPARE=PASS
COMPARE_EXIT=0
```
