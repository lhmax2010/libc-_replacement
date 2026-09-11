# aarch64 同环境对照

候选与基线均在 2026-09-11 使用同一 `gbs.conf`、release `111.1`、
目标架构、编译器、构建参数及滚动仓时间窗口完成干净构建。

比较命令：

```bash
docs/progress/BUILD_W1B_0909/code/compare_same_environment_rpms.sh \
  tmp/BUILD_W1B_0909/rpms/aarch64_candidate \
  tmp/BUILD_W1B_0909/rpms/aarch64_baseline \
  aarch64 tmp/BUILD_W1B_0909/checks/aarch64_same_environment
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

逐项规模：12 个 RPM、5691 条包内文件、158 条 RPM 标准库依赖、
731 条 ELF `DT_NEEDED`、653085 条动态符号。候选与基线每类的行数和
SHA256 见 `evidence/aarch64_same_environment/SUMMARY.txt`；五份空 diff
亦已落盘。

资产来源另由候选日志直接确认：未启用 libc++ 时解包的是
`mlgo_aarch_model.tar.gz`，候选和基线该归档 SHA256 均为
`76f896dfdce3fa19d0ed467554ed3a643f062f52b4a4db81f576d04c469014a1`，
`cmp` 退出码为 0。
