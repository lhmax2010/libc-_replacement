# R110 材料索引

- `REPORT.md`：原始出处、核对结果、当前复查和结论。
- `FOR_HQ_SUMMARY.md`：可直接给总部的口径说明。
- `REPORTED_50_AUDIT.md`：历史 50 行逐条核对，按包分组。
- `STRICT_RETHROW_SITE_LIST.md`：历史冻结源码中严格识别并传播的站点清单。
- `CURRENT_RETHROW_SITE_LIST.md`：历史阳性包当前开发分支的站点清单。
- `METHOD_AND_LIMITS.md`：匹配规则、覆盖面和已知盲区。
- `tables/reported_50_audit.tsv`：历史 50 行的机器可读审计表。
- `tables/strict_clause_sites_original.tsv`：严格语法定义下的全部 175 个 catch 子句。
- `tables/strict_rethrow_sites_original.tsv`：其中继续传播的 172 个子句。
- `tables/current_rethrow_sites.tsv`：当前八个历史阳性包中的 140 个传播站点。
- `tables/package_summary.tsv`：按包汇总。
- `tables/source_instance_duplicates.tsv`：两个重复源码 RPM 变体带来的重复位置。
- `tables/current_source_identity.tsv`：当前复核所用仓库、分支和 HEAD。
- `tables/audit_metrics.tsv`：所有关键计数。
- `code/`：生成与校验脚本；`raw/`：命令、输出、错误和退出码。
- `ARTIFACT_SHA256.txt`：提交前非原始日志产物的 SHA256 清单。
- `RECOVERED_TECHNICAL_ERRORS.md`：已恢复的命令/脚本技术错误及处置。
