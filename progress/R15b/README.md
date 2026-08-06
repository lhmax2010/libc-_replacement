# R15b 证据入口

结论入口为 `R15b_report.md`。`commands/` 保存 ping、SSH、SDB 与全部板端命令原文、输出和退出码；`tables/` 保存连通、身份、运行时、能力边界和可行性表。板端拉取的两个运行时文件仅落在被忽略的 `tmp/R15b/`，其 SHA 与 readelf 结果已进入命令证据。

状态为 `LIMITED`；另有一次额外只读 `rpm -q` 查询的纪律偏差，已原样申报。
