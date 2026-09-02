# R100 材料索引

- `REPORT.md`：完整调查、逐包分类汇总、两种配置方案对照与盲区。
- `STAGE1_RULES.md`：人工确认后的机械判定规则；冗余的“广泛链接”规则已删除。
- `tables/base_cpp_package_decisions.tsv`：73 个源码包逐包结论与证据。
- `tables/r95_r100_denominator_crosswalk.tsv`：与 R95 38 包口径的逐包交叉表。
- `tables/configuration_strategy_comparison.tsv`：按包修改与全局注入的量化对照。
- `tables/global_injection_package_effects.tsv`：全局注入对每个候选包的影响分类。
- `tables/driver_option_probe.tsv`：GCC/Clang 实际驱动器探针摘要。
- `tables/armv7l_cross_edges/`：Toolchain 快照中 Unified→Base 的符号级边界证据。
- `tables/armv7l_internal_edges/`：Base 内部向关键 provider 的符号级边界证据。
- `tables/static_archive_symbols_refined.tsv`：静态归档的全局、可解码 C++ 符号检查。
- `code/`：本轮脚本快照；`code/reused/` 是原样调用的 R95 扫描器。
- `raw/`：命令原文、标准输出、标准错误、退出码及技术性恢复记录。

大体积下载包和提取目录未入库；其 SHA256、下载状态、RPM 提取状态及逐条
检查命令均在表格与 `raw/` 中。
