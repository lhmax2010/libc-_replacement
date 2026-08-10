# R24 方法与覆盖边界

## 图与单位

- R22 图节点为 479 个待迁移源码 RPM 身份；另有 4 个厂商源码 RPM 身份单列，交付清单总分母为 483。
- 有向边 `A -> B` 表示源码包 A 的某个运行时 ELF 通过 `DT_NEEDED` 唯一解析到源码包 B 提供的 ELF，并且消费者 UND 与 provider DEF 动态符号交集至少含一个 `_Z` 开头的 Itanium ABI 修饰符号。
- R22 在 1,705 条 R20 候选源码边中保留 1,234 条真 C++ ABI 边；剔除 445 条纯 C 接口边和 26 条无动态符号交集边。剔除不是推断：逐边符号依据见 `tables/deleted_candidate_edges.tsv`。
- 厂商单列边界涉及 12 条真边，故 479 节点迁移图的边分母为 1,222。

## 符号提取原口径

R22 使用 `readelf --dyn-syms -W <ELF>`；解析后以第一个 `@` 为界去掉符号版本后缀再求交。该实现见 `progress/R22/tools/classify_abi_edges.py:49-83`。

- 版本化符号：归一为基础符号名后参与相交。
- 弱引用：原脚本没有按 Bind 过滤，所有 `Ndx=UND` 条目同口径参与；因此弱 UND 未被排除。
- 静态库边：未纳入。输入只覆盖运行时 ET_EXEC/ET_DYN 与动态符号。
- header-only 接口：未单列；只有其影响落到动态符号或 DT_NEEDED 才可被本图观察。
- `dlopen` 边：未纳入；R22 从 R13 的 `RESOLVED_UNIQUE` DT_NEEDED 解析边取证。
- 未唯一解析的 DT_NEEDED：13,167/151,403 条未补全，因此不进入本图。
- 无动态 UND 的布局、内联、pointee/tunnel 耦合：本图不覆盖；旧 aligned census 的 A8/A9 是另一套冻结证据提升规则。

## SCC 与分层

SCC 使用 Tarjan 算法在 479 节点、1,222 边图上重算。对每一阶段的诱导子图，以“当前不再依赖该阶段内其他未分层 provider 的节点”为第 0 层，逐层移除，得到 provider-first 层。若存在非平凡 SCC，应把整个 SCC 作为同批节点；本次实测非平凡 SCC 为 0。

## 118/121/122 的原始口径

该数字来自 aligned-0723 `tier1_production_0001`：单位是生产二进制包 Name，不是 ELF，也不是源码包。其图使用 D2 保留 CPP_ABI 边并加入 A8/A9 证据提升；快照为 Base `tizen-base_20260709.234055` + Unified `tizen-unified_20260723.231947`。R22 第三阶段使用较后的 R11 composite snapshot、源码 RPM 节点、动态 UND/DEF 实交边和拓扑深度分档，二者不是同一对象。

## 只读边界

本任务没有重新扫描仓库、没有读取新 RPM payload、没有构建或安装。所有数字由已落盘 TSV/Markdown 输入重新聚合；既有文档和输入没有修改。
