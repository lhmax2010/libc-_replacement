# 预编译static资产核查

先读[报告](REPORT.md)。本次不推送bpftrace，不改变其余10包状态。

- `summary.json`：源码范围、tar及ELF哈希、六镜像精确归属、字符串计数。
- `image_membership.tsv`：六清单的原始URL、哈希及计数。
- `qemu_source_files.tsv`：提供仓库6个文件的身份。
- `additional_string_evidence.json`：补充字符串证据，不能直接推导生产编译器。
- `raw/`、`commands.jsonl`：命令原文、实际输出、退出码。
- `final_check.json`：核查、正反样本校准及脚本哈希。

未发现可配对的qemu-accel bpftrace二进制；来源为NOT_OBSERVED，
跨来源SHA256比对为NOT_AVAILABLE。不要把它误写为哈希比对不一致。
镜像结论只限定到已申报的固定历史快照。
