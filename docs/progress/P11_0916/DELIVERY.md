# 实验 2 单独交付回执

证据提交：`1ddbc7637e68dc32bb537f9b82caf0707098977b`。

已对 `origin` 的 `codex/runtime-validation` 作普通推送，退出码 0。推送后 `git ls-remote` 与本地 `git rev-parse HEAD` 核对一致；完整记录分别为 `delivery/008_push.*`、`009_remote_check.*`、`010_local_check.*`。本文件及这些回执再用一个普通后续提交归档；最终远端 SHA 在交接消息中报告。不使用 force，不推 Gerrit，不接触实施分支。

状态为 `PARTIAL`，原因见 [报告](experiment2/REPORT.md)：三架构真实组件的无版本导出与按名查找已经查清；至少三个独立已发布应用样本仍缺。不能把此缺口改写成对所有旧应用或所有方案的二值结论。

## 校验

- `delivery/002_hash_check.*`：1234 个封存文件的 SHA256 校验，退出码 0。
- `experiment2/SEAL.json`：输入/样本计数、内部链接、242 条已完成原始命令的配套记录完整；两项非零退出均在报告说明。封存器自身与交付记录在该计数之外。
- `delivery/006_scope_guard.*`：已暂存文件全部在 `docs/progress/P11_0916/`，退出码 0。扫描全量 JSONL 用无损 gzip 归档，不重复提交未压缩文件。
- `delivery/004_format.*`：退出码 2，唯一提示是生成的 `experiment2/SAMPLES.md` 文件末尾多一个空行；不是语法、链接或事实错误。保留生成文件及已封存摘要，不把此项写为通过。
- 原有 R115 两个脏文件未纳入提交，其身份记录见 `experiment2/raw/029_final_environment.stdout`；未覆盖用户既有变更。

实际资源检查起于 2026-09-15 09:40:01 UTC；目录保留用户任务标签中的 0916。本阶段未用开发板、未执行 EWK、未进行后续异常/反向矩阵实验，也未修改中英文方案正文。完成此阶段后停下，等待人工审阅及方案 B 口径确认。
