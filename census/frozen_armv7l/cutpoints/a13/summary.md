# A13 核心波传递闭包上界与 BundleRaw 核验

## 权威数字

| 口径 | 核心基线 | 并入 TIER1 分量数 | 并入包数 | 核心包数 |
|---|---:|---:|---:|---:|
| `UB_direct` | 130 | 189 | 272 | **402** |
| `UB_closure` | 130 | 289 | 383 | **513** |

因此 HLD 应使用：确证下界 **130**，分量级一跳上界 **402**，全部 unknown
视真的传递闭包最坏上界 **513**。此前 `130+200=330` 只统计一跳外部端点包，
不是不可拆 TIER1 分量的包数，现标为 superseded。

## UB_direct

692 条生产 unknown 中有 376 条直接跨越 130 包核心。它们涉及 200 个唯一
核心外端点包，但这些端点分布在 189 个 TIER1 分量；把分量整体并入后实际
带入 272 包，比端点集合多 72 包：

```text
UB_direct = 130 + 272 = 402
```

189 个分量逐项的规模、触发边数、直接端点包和全成员见
`ub_direct_components.tsv`。

## UB_closure

将全部 692 条 unknown 边都视为真，在 TIER1 分量图上做完整并查/可达闭包。
边按分量映射后为：376 条核心跨界边、203 条核心外分量间边、113 条已有
TIER1 分量内部边。含核心的最终团块共 290 个 TIER1 分量，其中核心外 289
个分量、383 包：

```text
UB_closure = 130 + 383 = 513
```

相对 `UB_direct`，传递闭包额外吸收 100 个分量、111 包。最远传播距离为
5 个 unknown 分量跳；逐跳分布为：

| 最小 unknown 跳数 | 分量数 | 包数 |
|---:|---:|---:|
| 0 | 1 | 130 |
| 1 | 189 | 272 |
| 2 | 72 | 82 |
| 3 | 13 | 14 |
| 4 | 13 | 13 |
| 5 | 2 | 2 |

290 个分量的逐项成员、最小跳数和连接边计数见
`ub_closure_components.tsv`；289 个并入分量及其中 100 个闭包追加分量分别见
`ub_closure_added_components.tsv`、`ub_closure_only_components.tsv`；全部新增
包见 `ub_added_packages.tsv`；692 条边到分量的映射见
`unknown_edge_component_map.tsv`。输入包唯一性、端点完整映射、
376 条跨界标记复算以及固定输入计数均在 `validation.tsv` 中 PASS。

## BundleRaw

**覆盖结论：未覆盖。** `BundleRaw` 是
`std::pair<std::unique_ptr<bundle_raw, decltype(std::free)*>, int>`；公开的
`Bundle(BundleRaw, bool)` 输入和 `ToRaw(bool)` 输出均不在 A10 的 14 项中，
必须新增两条“字节深拷贝 + 两侧本地重建 unique_ptr 所有权”的桥接不变量。
完整判定见 `bundleraw_bridge_gap.md`，机器可读补充见
`bundleraw_bridge_addendum.tsv`。

## 方法边界

`UB_direct` 和 `UB_closure` 都是假定 unknown 为真的保守模拟，不把 unknown
改判为已证实耦合。`UB_closure=513` 是当前 692 条已落盘 unknown 边在分量级
传递下的最坏可达团块，不覆盖尚未被 A9 枚举的静态盲区或运行时配置边。
