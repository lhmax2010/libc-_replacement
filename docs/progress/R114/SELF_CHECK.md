# R114 自检记录

## 输入身份

- 原压缩包：`hq_deliverables_20260904.tar.gz`
- SHA256：`8026cb05dd9e1d7ec80dded6528bf9cda92d179a69bfadaa5eca45b769d6f62e`
- 实际解压路径：`/tmp/r114_input_20260904/hq_20260904/`
- 中文 TSV 与 R109 版本逐字节一致：是
- 中文 TSV SHA256：`ef20021a789d4d3032fccb216251bee1effb6b19e349e0db973f37a3c42f33cf`

完整输入哈希见 `INPUT_SHA256.tsv`。

## 英文清单

| 检查 | 结果 |
|---|---|
| TSV 行数 | 255（表头 1 + 数据 254） |
| Markdown 数据行 | 254 |
| 列数 | 9 |
| 包名逐行一致 | PASS |
| 判定逐行映射一致 | PASS |
| 判定计数 | requires adaptation=11；does not require adaptation=237；currently cannot be adapted=6 |
| 实施状态 | completed and pushed=8；paused=3；not applicable=243 |
| SHA 逐行一致 | PASS |
| `source=`、`VCS=`、`Summary=` 等受保护证据串 | PASS |
| 数字 token 逐行一致 | PASS |
| 路径/符号等反引号代码片段逐行一致 | PASS |
| 中文字符残留 | 0 |
| 内部任务编号和规则码残留 | 0 |

随机 10 行回译结果为 10/10 一致，见 `BACK_TRANSLATION_AUDIT.md`；另做 12 个不同理由模板的定向复核，原文记录见 `raw/027_translation_sample_review.stdout.txt`。

## R113 修正

- M1–M8 的新文本存在：PASS
- 被替换的旧表述不存在：PASS
- S1–S7 均已按记录采纳：PASS
- 三项“当前倾向”均保持为待裁决/可行选项，没有补造裁决记录：PASS
- 完整英文清单内部标记：0
- 50 行附件：50 数据行，其中真实 catch 46、非子句 4；状态列改为描述所在 handler
- 172 行附件：172 数据行；`source_kind` 已删除；`LITERAL_THROW=46`、宏传播 126
- 公开逻辑键 `(package_name, file, line, clause_text, rethrow_kind)` 去重：104

## 数字与文件间一致性

对三份正文和两份附件分别提取数字 token，与原包逐项比较，五份文件的数字多重集均无增删；英文清单逐行数字多重集与中文输入一致。对应原始核算见 `raw/039_numeric_change_audit.*`。

关键闭合关系再次核对：

- `11 + 237 + 6 = 254`
- `8 + 3 = 11`
- `46 + 4 = 50`
- `46 + 126 = 172`
- 172 个 occurrence 按公开逻辑键得到 104 个逻辑站点

## 新压缩包

- 文件：`hq_deliverables_20260904_corrected.tar.gz`
- SHA256：`8b2ddb0de1b6e8e9ab78f1dcaab4a902fb9cdf6c2bb8f6a407ad805c85f66de7`
- 内容：README + 七份交付文件，共 8 个文件
- 解压回读路径：`/tmp/r114_verify_20260904/hq_20260904/`
- 解压后与 `deliverable/hq_20260904/` 逐文件一致：PASS

完整输出哈希见 `OUTPUT_SHA256.tsv`。
