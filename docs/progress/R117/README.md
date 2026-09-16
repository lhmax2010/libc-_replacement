# 标准库差异清单的范围候审

先读 [范围说明](SCOPE.md)，再审 [骨架 TSV](SKELETON.tsv) 或 [Markdown 全表](SKELETON.md)。状态为 **PARTIAL_SCOPE**，不是完整性认证，也不是差异验证报告。

主要索引：

- [逐头覆盖](HEADER_COVERAGE.tsv) / [入口名交差集](HEADER_SETS.tsv) / [解析诊断](HEADER_ERRORS.tsv)
- [高关注类型的场景](HIGH_PRIORITY_SCENARIOS.tsv) / [接口候选队列](platform/INTERFACE_CANDIDATES.tsv)
- [既有材料映射](EXISTING_MATERIAL_CROSSWALK.md) / [不能放入单一实体的项目](UNMATCHED_OR_NONENTITY.tsv)
- [覆盖统计](platform/SUMMARY.json) / [未读文件](platform/unread_files.tsv) / [疑问及自行判断](QUESTIONS.md)
- [自检](SELF_CHECK.json) / [状态](STATUS.md) / `SHA256SUMS.tsv`

## 证据读取与复查

TSV/JSONL 的 `.gz` 文件是普通 gzip，可解压流式读取；各行保留源 RPM、路径、行号或实体源码位置。主表逐项来源在 `entity_sources.tsv.gz`，头文件到实体关系在 `header_entities.tsv.gz`，成员操作在 `public_members.tsv.gz`。

`raw/` 保存实际外部命令、stdout/stderr、退出码与时间。大体积 AST stdout 已无损压缩，其原文哈希及还原说明在 `headers/AST_ARCHIVES.json`。`scripts/` 保存脚本各阶段快照，`scripts/config/` 保存解析使用的生成配置；代码树和平台原件没有被修改。

复查时使用独立工作树，恢复快照到 `tmp/R117/` 后按 raw 中原命令及各批快照执行；不要在原证据目录上直接覆盖运行。脚本依赖现有 Tizen Clang22 sysroot、GCC14 SDK、冻结的 371 包抽取目录，具体路径/选项在 `common.py`、解析状态和命令记录中；缺少这些输入时，不能声称在另一台机器上已重现。

主表的 0 仅是明确覆盖内的限定名零命中；不支持扫描的命名空间、C 全局 API 和操作符使用为 NOT_OBSERVED。所有可能差异点均留待范围审阅后查证。
