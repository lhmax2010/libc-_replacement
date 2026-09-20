# 进度对齐核对交付

建议先读 `ALIGNMENT_REPORT.md`（逐条裁定及误读纠正），再读 `CURRENT_STATUS.md`；`CURRENT_STATUS.tsv` 为同一状态表的机器可读版。

- `comparison/`：两轮完整命令、原始 JSON 字节副本、argv/scope/guard/deadline 全部差异，以及本次只读 rpm --eval 的完整输出。
- `rpm-order-comparison.tsv`：三轮 Wrote 顺序逐包对照。
- `static-identities/`：四份相关 RPM 的单包摘要核验、libclang.a 成员提取命令及 SHA256。
- `trace-counts.json`、`statx-enosys-locations.tsv`：严格 guest trace 分布统计及全部位置，不用粗文本匹配替代调用归属。
- `evidence/`、`source-provenance-identities.json`、`current-rpm-inventory.json`：所查当前文件副本、身份与 RPM 盘点。
- `raw/`：命令原文、完整输出、退出码和时间；包括第一步 ARM_RPM_DIAG 的提交与推送回执。
- `*.py`：实际执行的核对/交付脚本；`*.executed_v1.py` 保存技术性纠正前的执行版本。
- `DELIVERY_MANIFEST.json`：从 progress/ALIGN_0920 到本交付目录逐文件原样复制的 SHA256 与校验结果。没有删减大型位置表。

原 ARM_RPM_DIAG 全部材料已先行发布到相邻目录；其大 trace 无损分片，恢复办法见该目录 README_DELIVERY.md。原始证据不修改；本次纠正说明只写在新的对齐报告。

核对不包含新构建、修复、板上测试、包仓推送；下一步由人工审阅后决定。
