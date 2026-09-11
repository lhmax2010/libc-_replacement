# x86_64 候选产品核查与断点保存

产品核查命令：

```bash
docs/progress/BUILD_W1B_0909/code/verify_gcc_product.sh \
  tmp/GBS-ROOT/BUILD-W1B-llvm-x86_64/local/BUILD-ROOTS/scratch.x86_64.0 \
  tmp/GBS-ROOT/BUILD-W1B-llvm-x86_64/local/repos/tizen_unified_standard/x86_64/RPMS \
  tmp/GBS-ROOT/BUILD-W1B-llvm-x86_64/local/repos/tizen_unified_standard/x86_64/SRPMS/llvm-22.1.8-111.1.src.rpm \
  x86_64 tmp/BUILD_W1B_0909/checks/x86_64_candidate
```

输出与退出码：

```text
FORBIDDEN_OPTION_MATCHES=0
FORBIDDEN_BUILDREQUIRES_MATCHES=0
GCC_PRODUCT_VERIFY=PASS arch=x86_64
VERIFY_EXIT=0
```

12 个候选 RPM 的复制与 SHA256 记录退出码为 0：

```text
CANDIDATE_RPM_COUNT=12
COPY_EXIT=0
```

保留位置：`tmp/BUILD_W1B_0909/rpms/x86_64_candidate/`。这是下一轮同环境
基线对照的候选输入，不是提交材料；完整核查文件复制在
`evidence/x86_64_candidate/`。
