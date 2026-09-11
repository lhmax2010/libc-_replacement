# aarch64 候选产品核查

命令原文见下；执行时构建根仍是候选构建，随后才被基线构建清理重用。

```bash
docs/progress/BUILD_W1B_0909/code/verify_gcc_product.sh \
  tmp/GBS-ROOT/BUILD-W1B-llvm-aarch64/local/BUILD-ROOTS/scratch.aarch64.0 \
  tmp/GBS-ROOT/BUILD-W1B-llvm-aarch64/local/repos/tizen_unified_standard/aarch64/RPMS \
  tmp/GBS-ROOT/BUILD-W1B-llvm-aarch64/local/repos/tizen_unified_standard/aarch64/SRPMS/llvm-22.1.8-111.1.src.rpm \
  aarch64 tmp/BUILD_W1B_0909/checks/aarch64_candidate
```

输出与退出码：

```text
FORBIDDEN_OPTION_MATCHES=0
FORBIDDEN_BUILDREQUIRES_MATCHES=0
GCC_PRODUCT_VERIFY=PASS arch=aarch64
VERIFY_EXIT=0
```

完整核查文件已复制到 `evidence/aarch64_candidate/`。
