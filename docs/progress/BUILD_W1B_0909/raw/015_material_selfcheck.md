# 材料自检

执行内容：两份脚本 `bash -n`、压缩日志 `gzip -t`、`SHA256SUMS` 全量
校验、占位词检查、矩阵计数、`git diff --check`。

退出码：`0`。输出：

```text
aarch64_baseline_clean.log.gz: OK
aarch64_candidate_clean.log.gz: OK
aarch64_candidate_verify.log: OK
aarch64_same_environment_compare.log: OK
x86_64_candidate_clean.log.gz: OK
x86_64_candidate_verify.log: OK
../code/compare_same_environment_rpms.sh: OK
../code/verify_gcc_product.sh: OK
PASS_CELLS=10
NOT_OBSERVED_CELLS=2
SELFCHECK_EXIT=0
```

检查时 `REPORT.md` 与 `MATRIX.tsv` 已无 `TO_BE_FINALIZED` 或 `RUNNING`
占位值。
