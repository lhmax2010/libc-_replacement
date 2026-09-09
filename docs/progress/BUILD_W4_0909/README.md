# BUILD W4 0909 材料索引

- `REPORT.md`：17 包失败原因、分类、证据边界与结论。
- `OPTION_PROBE.md`：全局注入选项的三架构 driver 探针。
- `tables/package_failure_classification.tsv`：17 包机器可读分类。
- `tables/trace_option_probe.tsv`：三架构、两类 driver、逐选项结果。
- `evidence/log_inventory.tsv`：本任务使用的 QuickBuild 日志大小与 SHA256。
- `evidence/log_excerpts.md`：受控的关键失败摘录；完整日志未提交。
- `code/`：日志证据提取、选项探针、汇总脚本快照。
- `raw/`：命令原文、输出、退出码及技术性重跑说明。

最终结论为 `PARTIAL`：build 1165447 的指定 17 包均已分类，但原 armv7l
build 1165448 日志不可得，且 `hdf5` 根因与 aarch64 GCC 的部分逐项探针
仍未闭合。
