# 对外材料输入复制与哈希

本轮不回写上一轮材料，而是以 `docs/progress/BUILD_W3_0908/updated/` 为
已审计输入，复制到本轮 `source/`，再把本轮输出放在 `updated/`。

复制命令原文：

```bash
mkdir -p docs/progress/BUILD_W3_0909/source docs/progress/BUILD_W3_0909/updated docs/progress/BUILD_W3_0909/raw docs/progress/BUILD_W3_0909/code
cp docs/progress/BUILD_W3_0908/updated/base_adaptation_report_zh.md docs/progress/BUILD_W3_0909/source/base_adaptation_report_zh.md
cp docs/progress/BUILD_W3_0908/updated/base_adaptation_status_en.md docs/progress/BUILD_W3_0909/source/base_adaptation_status_en.md
cp docs/progress/BUILD_W3_0908/updated/base_packages_zh.tsv docs/progress/BUILD_W3_0909/source/base_packages_zh.tsv
cp docs/progress/BUILD_W3_0908/updated/base_packages_zh.md docs/progress/BUILD_W3_0909/source/base_packages_zh.md
cp docs/progress/BUILD_W3_0908/updated/base_packages_en.tsv docs/progress/BUILD_W3_0909/source/base_packages_en.tsv
cp docs/progress/BUILD_W3_0908/updated/base_packages_en.md docs/progress/BUILD_W3_0909/source/base_packages_en.md
cp docs/progress/BUILD_W3_0909/source/* docs/progress/BUILD_W3_0909/updated/
sha256sum docs/progress/BUILD_W3_0909/source/*
printf 'copy_exit=0\n'
```

输出：

```text
d2312e18e6872b5a527992b5214888a0928287735851774eebc08932ba1e567a  docs/progress/BUILD_W3_0909/source/base_adaptation_report_zh.md
4f29f1d14f426c68e9b2d51bb19c16a88cf8f1d9f62ca3a135a04650b98c22bb  docs/progress/BUILD_W3_0909/source/base_adaptation_status_en.md
52d7b7493b05f746dff39cd3cdac75af6ca22ca652d9bb305e6f0b8a9ecd3620  docs/progress/BUILD_W3_0909/source/base_packages_en.md
a7be2cb8715d0e89fd40ab88b11f75552dea99e1b2e311ed8db58b4c737c1c5f  docs/progress/BUILD_W3_0909/source/base_packages_en.tsv
e55996f04e4ee37cd39a370d8d9043ca7f1e293079bbc9fb6f406027ffca3ec8  docs/progress/BUILD_W3_0909/source/base_packages_zh.md
b2e198af2e25fc368349715fa446c69f6e3bcb460e4568cdd6901c92ae9041ee  docs/progress/BUILD_W3_0909/source/base_packages_zh.tsv
copy_exit=0
```

退出码：`0`。
