# census 对账三项复核

## 1. core130 对应异常

**裁决：`METHODOLOGY_DIVERGENCE`。** 生产包宇宙四套结果均为 1804 包，
生产剔除正则一致；SONAME canonical map 与 `graph_edges.tsv` 也由 A9
manifest 的 SHA256 证明为同一输入。唯一造成 239 膨胀的关键维度是边语义：
`tier1_prod_v2_0001` 直接接纳全部原始 `CPP_ABI`，其中 VAGUE-only 的
libstdc++ BOTH 归因假边尚未应用 D2；冻结 A9 则从 D2 修正后的 123 开始，
再并入 A8 的 33 条确证布局边（核心增至 130），A9 的 5 条
`YES_POINTEE` 没有继续扩大该核心。

集合对账为：raw=239，A9=130，交集=126，raw-only=113，
A9-only=4。**预登记写作“239 中不属 130 的 109 包”不成立：集合交集为 126，故 239\\130=113、130\\239=4；109 只是净规模差 239-130。** 全部 113 个
raw-only 包见 `raw239_extra_packages.tsv`，每包均附从冻结 123 核心到其
D2 `DROP_BOTH_ARTIFACT` 桥路径；4 个反向缺席包见
`a9_core_missing_from_raw239.tsv`。因此此前 multiarch 的
“arm 239 对应 core130”分析必须标 superseded；应先把 D2+A8/A9 口径移植
到多架构管线，再重跑对应物分析，不能据 239 判 `GRAPH_DRIFT`。

维度明细见 `core130_method_dimensions.tsv`；raw 图跨修正分量的假桥
130 条，合并了 41 个 core 外修正
分量，见 `raw239_artifact_bridge_edges.tsv` 与
`raw239_component_merge_lineage.tsv`。

## 2. cynara19 九条边对账

指定四类波 1 卷宗的机械结果：
`ADJUDICATED=0`、
`COVERED_BY_LEDGER=0`、
`UNMATCHED=9`。按任务纪律，9
条均为**红项**，全量见 `cynara19_edge_reconciliation.tsv`。

红项的非卷宗根因不改变红色簿记：其中 7 条为 D2 已判
`DROP_BOTH_ARTIFACT` 的 VAGUE 假边，2 条消费者包命中生产测试剔除正则。
也就是说，原“9 条生产边界边”输出本身使用了未修正 raw-v5 边和未过滤测试
消费者；它们不应靠补填 API 台账来“覆盖”。应用冻结 D2 与生产过滤后，
现有落盘图得出的 cynara19 生产边界为 0 条，
见 `cynara19_corrected_production_boundaries.tsv`。dlopen 插件与 N2 同批约束
仍由波 1 卷宗管理，不因动态图边为零而取消。

## 3. 0723 快照对齐成本

已完成的 aarch64+x86_64 并行新 census，从首个环境证据到最终 summary 的
mtime 包络为 75.83 分钟；这是主机墙钟，
不是 CPU profiler 数据。最低成本方案是保留两个已钉死 0723 的架构，只对
ARM 做 0723 增量抓取/抽取，并**全局重算** SONAME、边、D2、A8/A9 分量与
三架构对账，估算 45–70 分钟。严格三架构 clean-room 方案预算并行
90–120 分钟（顺序约 228 分钟，另为 IO 竞争预留余量）。详见
`snapshot_alignment_cost.tsv`。

ARM 增量步骤见 `armv7l_incremental_rerun_plan.tsv`。当前没有列取 ARM
0723 目录，故 changed/new/removed RPM 的精确数量属于待执行项；不能从
aarch64/x86_64 NEVRA 猜 ARM 清单。本任务未执行任何下载、解包或重算。

## 复核结论

- core 异常：**`METHODOLOGY_DIVERGENCE`**，多架构对应物分析须修口径后重跑。
- cynara19 九边：**9/9 UNMATCHED，门禁红**；
  同时证实九边输入清单本身均不属于冻结生产边语义。
- 快照对齐：建议人工在“ARM 增量 45–70 分钟”与“三架构 clean
  90–120 分钟”间裁决；本报告不执行。
