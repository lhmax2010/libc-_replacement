# armv7l 基线构建与同环境对照

构建命令：

```bash
nice -n 15 ionice -c 3 gbs \
  -c /home/toolchain/development/libc++_replacement/config/gbs.conf build \
  -A armv7l \
  -B /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/BUILD-W1-0908-llvm-armv7l \
  -C --fail-fast --threads 1 --define '_smp_mflags -j2' \
  --define 'mlgo_build_jobs 2' --release 109.3 --style git \
  --include-all --packaging-dir packaging --spec llvm.spec
```

完整输出：`../build_logs/armv7l_baseline_0911.typescript.gz`。终态：

```text
[7188s] toolchain-2026 finished "build llvm.spec"
=== Total succeeded built packages: (1) ===
Script done ... [COMMAND_EXIT_CODE="0"]
```

第一次比较命令把构建仓目录直接作为基线输入，旧 release 108.2 的 10 个
debuginfo RPM 被一并读取：

```text
packages_diff_lines=24
file_lists_diff_lines=11654
stdlib_requires_diff_lines=0
elf_needed_diff_lines=0
dynamic_symbols_diff_lines=0
SAME_ENVIRONMENT_RPM_COMPARE=DIFFERENT
COMPARE_EXIT=20
```

完整原输出保存在
`../build_logs/armv7l_same_environment_compare_unfiltered_0911.log`。

隔离本轮 release 109.3 的 12 个基线 RPM 后，保持比较脚本不变重跑：

```bash
nice -n 15 ionice -c 3 \
  docs/progress/BUILD_W1B_0909/code/compare_same_environment_rpms.sh \
  tmp/BUILD_W1BC_0911/rpms/armv7l_candidate \
  tmp/BUILD_W1BC_0911/rpms/armv7l_baseline armv7l \
  tmp/BUILD_W1BC_0911/compare/armv7l_release109_3
```

输出与退出码：

```text
packages_diff_lines=0
file_lists_diff_lines=0
stdlib_requires_diff_lines=0
elf_needed_diff_lines=0
dynamic_symbols_diff_lines=0
SAME_ENVIRONMENT_RPM_COMPARE=PASS
COMPARE_EXIT=0
```
