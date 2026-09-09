# 254 包状态材料机械更新

命令原文：

```bash
awk -v language=zh -f docs/progress/BUILD_W3_0909/code/update_current_status.awk docs/progress/BUILD_W3_0909/source/base_packages_zh.tsv > /tmp/build_w3_base_packages_zh.tsv
awk -v language=en -f docs/progress/BUILD_W3_0909/code/update_current_status.awk docs/progress/BUILD_W3_0909/source/base_packages_en.tsv > /tmp/build_w3_base_packages_en.tsv
mv /tmp/build_w3_base_packages_zh.tsv docs/progress/BUILD_W3_0909/updated/base_packages_zh.tsv
mv /tmp/build_w3_base_packages_en.tsv docs/progress/BUILD_W3_0909/updated/base_packages_en.tsv
awk -v title='Tizen Base 层 libc++ 适配完整清单（状态更新）' -f docs/progress/BUILD_W3_0909/code/tsv_to_markdown.awk docs/progress/BUILD_W3_0909/updated/base_packages_zh.tsv > /tmp/build_w3_base_packages_zh.md
awk -v title='Complete Tizen Base libc++ adaptation inventory (status update)' -f docs/progress/BUILD_W3_0909/code/tsv_to_markdown.awk docs/progress/BUILD_W3_0909/updated/base_packages_en.tsv > /tmp/build_w3_base_packages_en.md
mv /tmp/build_w3_base_packages_zh.md docs/progress/BUILD_W3_0909/updated/base_packages_zh.md
mv /tmp/build_w3_base_packages_en.md docs/progress/BUILD_W3_0909/updated/base_packages_en.md
printf 'status_update_exit=0\n'
```

输出：

```text
status_update_exit=0
```

退出码：`0`。更新脚本只为 `llvm` 替换第九列；Markdown 从更新后的 TSV
机械生成。
