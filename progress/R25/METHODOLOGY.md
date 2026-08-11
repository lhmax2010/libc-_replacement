# R25 方法与覆盖边界

## 输入和单位

本轮没有扫描 RPM 仓或读取新的 ELF。节点直接取 R24 的 483 行源码 RPM 清单；边直接取 R22 已冻结的 1,234 条 `TRUE_CPP_ABI_COUPLING` 源码级有向边。边 `A -> B` 表示 A 的运行时 ELF 通过 `DT_NEEDED` 唯一解析到 B 的 provider ELF，且消费者 UND 与 provider DEF 动态符号交集至少含一个 `_Z` 开头的 Itanium ABI 修饰符号。

R22 原判据调用 `readelf --dyn-syms -W`，去掉首个 `@` 后的符号版本再求交；弱 UND 没有另行过滤。静态库、header-only、`dlopen`、未唯一解析的 DT_NEEDED、没有动态 UND 的布局/内联耦合不在该图中。本轮结论严格限定于这 483 个节点和 1,234 条实测动态边。

## 切面算法

节点迁移秩固定为：阶段一=1；阶段二 provider-first 第 0/1/2 层=2/3/4；阶段三整体=5；4 个厂商源码 RPM 身份永久保持未迁。对每个切面逐边计算：

- 正向跨界：消费者未迁、provider 已迁；
- 反向跨界：消费者已迁、provider 未迁。

切面 0 到 5 的定义与任务书逐字对应。阶段一和阶段三内部没有额外切面，因此同批内部边不会出现在切面表中。一个边可能跨越多个连续切面；`cut_cross_boundary_edges.tsv` 以“边×切面”为行，`cross_boundary_edge_properties.tsv` 则对出现过的边去重。

## 客观属性分档

对 R22 已落盘的动态符号交集使用本机 `c++filt` 解名，不读取新的二进制：

- `HIGH_RISK_OBJECTIVE_MARKERS`：观测到 `std::`、异常/throw/catch/terminate 字样，或 vtable/RTTI/VTT/typeinfo 符号；
- `MANUAL_JUDGEMENT_REQUIRED`：确认有 `_Z` 动态符号交集，但上述请求属性无法由现有符号建立；
- `LOW_RISK_PROVEN`：只有输入能证明“不跨运行时语义”时才允许使用。本次为 0，不能把“未见标记”当作低风险。

这三个值是按任务要求输出的客观属性桶，不是方案评级。R22/R24 没有窗口期重建触发或排期数据，所以每条跨界边的“消费者是否在窗口期重建”均为 `NOT_AVAILABLE`。provider 形态只能证明当前边由动态共享库承载；是否能提供双运行时、静态库和 header-only 形态均不能从 DT_NEEDED 图补全。

## 原子迁移单元

把 1,234 条有向边无向化后求连通分量。每条边的两个端点必须同批时，连通分量是不可再拆的最小原子单元。99 是原子单元数；若每个单元单独一批，可形成 99 批，但多个互不相连单元仍可合并，故不能推断“批次数只能是 99”。

## 两笔账

gtest/gmock 表以 `source_package_nevra` 去重。原表 157 行中有 4 个源码包各自命中两个 BuildRequires 名，去重后分母为 153。对 17 个不在 R22 483 范围内的包，只复用 R11 `binary_package_records.tsv` 和 `elf_inventory.tsv`：逐包给出运行时 ELF 数与 `cpp_indicator=YES` 数，不扫描新对象。

厂商“5”以二进制包 Name 为单位；R24 清单以源码 RPM 身份为单位。`pen-wave` 与 `pen-wave-devel` 同属 `pen-wave-1.4.0-2.src.rpm`，因此 5 个 Name 精确映射为 4 个源码身份。
