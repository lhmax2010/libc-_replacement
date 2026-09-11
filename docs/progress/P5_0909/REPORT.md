# P5 三份对外报告修正交接

状态：**17 组必改完成；6 组建议分别判断并采纳，其中日期建议限定采纳。待人工审阅。**

本次只修正 P4 指定内容，不实施运行库改动，不推 Gerrit。资源闸门 light 通过；
计时起点为 raw/001 的 2026-09-11 02:23:33 UTC，完成时间和用时见 `TIMING.json`。
实际交付目录遵循任务末尾的 `docs/progress/P5_0909/`。

## 六份修改稿

| 报告 | 中文 | 英文 |
|---|---|---|
| API 兼容性 | [修改稿](reports/api_compatibility_report.md) | [英文修改稿](reports/api_compatibility_report_EN.md) |
| ABI 差异（初步版） | [修改稿](reports/abi_comparison_preliminary.md) | [英文修改稿](reports/abi_comparison_preliminary_EN.md) |
| Chromium 调研 | [修改稿](reports/chromium_research_zh.md) | [英文修改稿](reports/chromium_research_en.md) |

`reports/` 与 HQ_files 六稿逐字相同。原输入在 `before/`，修改前后哈希在 `REPORT_SHA256.tsv`。
HQ 附表 `abi_layout_comparison.tsv` 未修改，未将它当作新增报告提交。

## 执行与自检

- [M01–M17、S01–S06 逐项执行、判断、疑问及新问题](EXECUTION.md)。每个替换的完整文本见 `diffs/`；最终位置见 `LINE_INVENTORY.tsv`。
- 33 个类型的原始 GNU／LLVM sizeof、alignof 重新比较；四份 API／ABI 表的十行大小和 ABI 差值全部核对，10 不同、23 指标相同，不能读作完整布局等价。见 `LAYOUT_RECHECK.tsv`、raw/015。
- 重新解析原始导出输入得 6,057／1,969／17；重读原始符号表得 198／231／108，含平台 15 ABS 版本节点。见 raw/011、013。重扫固定源树得 GNU 65／16 文件与原始 LLVM libcxx 0，带正向对照；补丁 50 站点／36 handler 回查原审计记录，见 raw/012、017。
- Chromium 30 格×5 次的 150 份原始退出码、实际提供方身份和具体状态／值断言重新核对。40 次退出 86、100 次退出 0（包含损坏对照）、10 次 SIGABRT，未将退出 0 等同无问题。见 `RAW_MEASUREMENT_RECHECK.tsv`。
- deque 原错值／主动退出 42、ICU 单入口与主版本断言、BoundaryError 源码、三方析构字节及八／两处名称，均回读原材料，见 raw/017。版本、日期、行号、枚举数字与实验单位分开；全部数字出现位置在 `NUMBER_OCCURRENCES.tsv`，对照分组说明见 `NUMERIC_AND_BILINGUAL_REVIEW.md`。
- 中英文逐节核对同一结论、条件与否定范围；API 9 组、ABI 1 组代码片段去注释后中英完全一致。共 20 份片段语法检查通过：C 头原样，C++ 明示补入依赖声明；没有链接或运行，不能当成 ABI 安全验证。见 `SYNTAX_RESULTS.tsv`。
- 三份英文稿汉字扫描为 0，检查器有中文正向对照。仍保留合理代码、文件名、URL 和历史版本，不把它们当翻译残留。
- 六稿 162 处 HTTP 引用、71 个唯一 URL 全部 HTTP 200；Chromium 全文唯一 URL 为 63，其中原新增节仍为 13。M16 的 57 处引用和 M17 的 5 个唯一源码地址全部覆盖。`FINAL_LINK_OCCURRENCES.tsv` 逐处对应 `URL_RESULTS.tsv`；五个源码 TEXT 解码与原快照逐字节及 SHA256 相同，见 `CORRECTED_URLS.tsv`。
- HTTP 成功只是当前可访问；未重测 Chromium 产品、旧运行实验或外部用户权限。保留原 Chromium 证据截止时间。

## 范围外新问题与交接

发现 **1 组范围外展示问题**：Chromium 原有表格中的未转义竖线，详见 EXECUTION 的 N01。
按要求未修改；请人工决定是否另行处理。本报告不将“完成本轮限定修改”表述为“无需人工审阅即可发出”。

命令、退出码、脚本快照及 SHA256 随包提供。仅向 GitHub 的 `codex/runtime-validation` 提交并推送本次六稿和材料；
既有 R115 两份修改及其他未跟踪内容均不纳入。本任务完成后停止，交人工审阅。
