# S1 签核准备（签署前）

本目录是新增 14 项 API 的待签材料，不是已生效登记册。

人工决定（`lhmax2025`，2026-07-25）：签署与 Q3 裁决均推迟至正式开工，
候选制品构建和板上实测通过后再签。三个 callback rider
`G7-PPM-001`、`G7-PPM-002`、`G7-PKG-001` 的文书已就绪，但对应二值门
仍须在 P2 镜像组装前执行并通过；本次没有把未来测试记为 PASS。

`signoff_sheet_proposed.tsv` 严格采用既有 27 项登记册的 16 列格式。表中
`SIGNED` 是供人工确认的目标值；`signer`、`signoff_date`、`q3_decision`
均保持空白。在这些列和最终签署来源回填前，不得合并进权威登记册。每行
均包含：

- v3 评审包 SHA256：
  `c4a9e30ea220340131fa2053c3cc97241e1d1e96921de5046144cc7a43033966`；
- GPT、Claude、Kimi 三份 round3 `closure_verdict` 的包内相对路径及
  SHA256；
- callback 条目的 G7 ID 与逐字
  `二值门 P2 镜像组装前 BLOCKING`。

待签表 SHA256：
`4be2c861a5f4d8674a7594488bbf0a9e5d308e83efe946bc37d0b1e016a70cd2`。

机械对账见 `mechanical_crosscheck.tsv`：14/14 PASS、差异 0。
`reviewers/` 同时保留三方 round2 初评与 round3 闭合确认，两轮共六份
评审文件；逐文件校验见 `reviewers/SHA256SUMS`。

## 本轮变更

- `adjudication_criteria.md` 新增 OQ-3b callback 再入判据；
- `startup_conditions.md` 的 S1 状态改为
  `BLOCK_SIGNOFF_DEFERRED_TO_EXECUTION`；
- 三项 Q3 决策保持空白，G7 状态保持 `TO_REVIEW`；
- 11 项无回调拟签 `SIGNED/HANDLE_CONFINED`；
- 3 项回调拟签 `SIGNED/HANDLE_CONFINED_WITH_G7_RIDER`。

出处和差分见 `change_record.tsv`、
`adjudication_criteria_signoff.diff` 与
`startup_conditions_signoff_deferred.diff`。候选制品与板测满足后完成
签署，S1 即 PASS。
