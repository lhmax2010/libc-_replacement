# 材料索引

- `REPORT.md`：完整事实查证、两版本差异、双向代价与未覆盖项；不作版本推荐。
- `COMPARISON.tsv`：便于程序处理的 2.15.1 / 2.18.0 逐项对比表。
- `EXTERNAL_SOURCES.md`：可访问的上游出处与“未找到”结论的检索边界。
- `raw/`：本地查证、自检命令的原文、标准输出、标准错误和退出码。
- `upstream/`：从 TensorFlow 官方 tag 下载的十个固定版本源码文件，以及每次下载命令、输出、错误与退出码。

建议阅读顺序：`REPORT.md` → `COMPARISON.tsv` → `EXTERNAL_SOURCES.md`；需要独立复核时再按报告中的证据索引查看 `raw/` 与 `upstream/`。
