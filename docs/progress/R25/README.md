# R25 产物索引

- `REPORT.md`：迁移切面、原子单元、两笔账和清单核对的中文报告。
- `METHODOLOGY.md`：输入口径、边判据、切面算法、属性分档及盲区。
- `inputs/input_identity.tsv`：本轮复用输入的路径、SHA256、大小。
- `tables/cut_summary.tsv`：六个切面的正向/反向跨界统计。
- `tables/cut_cross_boundary_edges.tsv`：逐切面正向跨界边全量清单。
- `tables/cut_reverse_edges.tsv`：反向边表；本次只有表头，表示实测零项。
- `tables/cut_top20_providers.tsv`：逐切面跨界边最多的前 20 个 provider。
- `tables/cross_boundary_edge_properties.tsv`、`cut_property_summary.tsv`：逐边客观属性和逐切面汇总。
- `tables/atomic_components.tsv`、`atomic_component_summary.tsv`、`atomic_component_size_distribution.tsv`：无向连通分量全量成员与分布。
- `tables/strategy_scale.tsv`：三种处置口径的机械规模数据。
- `tables/gtest_consumers_153.tsv`、`gtest_consumers_summary.tsv`：153 个 gtest/gmock BuildRequires 消费者的完整去向。
- `tables/vendor_five_to_four_accounting.tsv`：5 个厂商二进制 Name 到 4 个源码 RPM 身份的映射。
- `tables/migration_list_audit.tsv`、`migration_list_field_distribution.tsv`：R24 交付清单的行数、字段及取值分布。
- `tables/data_sources.tsv`：仓快照和对象形态申报。
- `validation.tsv`：机械断言。
- `commands/`：命令原文、工作目录、标准输出/错误与退出码。
- `MANIFEST.sha256`：本地 R25 产物清单；策展副本有独立清单。

所有表的源码包单位均为精确 `source RPM identity`，除非表内明确标为二进制包 Name、边或连通分量。
