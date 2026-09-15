# 本轮交接入口

当前结论：**PARTIAL**，10包已完成并推送，bpftrace的预编译static子包门禁待裁决。

建议顺序：

1. [最终报告](FINAL_REPORT.md)：结论、资源实测、PATH失误与重编统计、待裁决项。
2. [矩阵](MATRIX.md) / [机器表](MATRIX.tsv)：bcc-tools六格、bpftrace四格的精确范围。
3. [11包状态](PACKAGE_STATUS.md) / [机器表](PACKAGE_STATUS.tsv)。
4. [兼容性核对](COMPATIBILITY_REVIEW.md)：逐包条件与历史实测边界。
5. `checks/`、`raw/`、构建日志与JSONL：命令、输出、退出码、最终RPM哈希与readelf。
6. `SCRIPT_SHA256.tsv`：本轮归档脚本的哈希；执行前快照/哈希还在各raw记录中。

REPORT.md、DELIVERY.md保留首次停下的历史状态，不是当前结论。
CONTINUATION_REPORT.md保留人工确认后的恢复与测量过程。
没有提交RPM、构建根、凭据或其他无关文件。
