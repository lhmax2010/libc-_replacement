# R87 证据目录

- `REPORT.md`：结论、口径、逐层结果、盲区、自行判断与疑问。
- `tables/`：源码候选/分类、ELF 与静态归档候选和汇总、ABI 形态表。
- `raw/`：命令原文、stdout、stderr 与退出码；失败和更正尝试均保留。
- `code/`：只用于 R87 的只读扫描器与最小 ABI 探针源码。
- `evidence/abi_probe_corrected/`：最终 ABI 探针的 raw/demangled `nm` 结果。

临时 `.o` 与 Python `__pycache__` 未纳入策展目录；它们不是结论证据，均可由已收录源码和命令重建。平台源码、补丁与配置未复制或修改。
