# A9 — 成员级耦合扫描域扩展（R1 + R7g）

本目录只读取既有 census、A6、A8 证据。输入摘要及 SHA256 见
`input_manifest.tsv`；机械校验见 `validation.tsv`。生产包判定沿用正则
`unittests|gtest|haltests|-tests?$`（匹配消费者即剔除）。

## 1. 一层指针 / 引用 pointee 复扫

- A6 `NO_OBSERVED` 输入：**762 行**（严格复核）。
- 行级结果：`YES_POINTEE` **6**，`UNKNOWN_POINTEE` **214**，可见且在允许深度内未观察到 STL **27**，无裸指针/引用成员 **515**。
- `YES_POINTEE` 去重为 **6 边**；去测试后 **5 边**。逐字段证据见
  `pointee_evidence.tsv`，全部 762 行的机械判定见 `pointee_rescan.tsv`。

确凿边如下：

| consumer | provider | pointee 证据 | 生产口径 |
|---|---|---|---|
| capi-media-vision-common | opencv | `cv::Mat::u -> cv::UMatData -> std::shared_ptr<void> allocatorContext` | YES |
| capi-media-vision-roi_tracker | opencv | 同上 | YES |
| libmm-utility | opencv | 同上 | YES |
| nntrainer-applications | opencv | 同上 | YES |
| nntrainer-core | opencv | 同上 | YES |
| nnstreamer-unittests | opencv | 同上 | NO（测试消费者） |

判定只穿越本类的**一层**裸指针/引用，并要求 pointee 完整定义与拥有指针的类
位于同一公开 header path；进入后复用 A6 的直接成员、按值成员和基类递归。
pointee 只在同 devel 包的其他头可见时标 `UNKNOWN_POINTEE_VISIBILITY`；pointee
自身仍需第二次非内建指针跳转时标 `UNKNOWN_POINTER_DEPTH`。加上无公开定义、
`void*` 和解析失败，最终合并为 214 个行级 UNKNOWN，未静默转成 NO。本快照
没有为这些对象补充可用 DWARF 证据。LLDB SB wrapper 的 private pointee 即因
仅在其他内部头定义而保持 UNKNOWN，不参与升格。

## 2. 7,980 条 C 边及 C++ 实现泄漏

- 7,980 条 C 边全部从 NEEDED 范围内重新解析成功，共 **238,950** 条实际
  UND→导出符号记录，见 `c_edge_resolved_symbols.tsv.gz`。
- 严格证据门要求同一公开头同时证明：C API 的 opaque/`void*`/结构指针或直接
  C++ 类指针，它与某 C++ 类的类型、显式 cast 或一层可达结构关系，以及该类
  的 STL 成员。扫描覆盖直接 class pointer/reference、完整 signature record
  的一层成员可达性，以及注释剥离后 API 邻近代码的显式 cast。
- 该门下 `C_EDGE_STL_LEAK` 为 **0 边**。因此没有 C 边自动并入 TIER1。
- **这不是 7,980 条 C 边均无耦合的结论**；不满足关系证明的记录进入
  `c_edge_stl_unknown.tsv`，保持不可判定。

## 3. 隧道 API 机检种子

- `tunnel_api_candidates.tsv`：**3,745 条发布位置记录**，对应 **2,479 个唯一
  (provider, API)**。
- 每条均有公开 C 声明中的 `void*`/opaque typedef/结构指针，以及提供者 active、
  非 Rust ELF 中 demangle 含 `std::` 的符号证据（raw 与 demangle 均落盘）；
  它们只是人工隧道登记册种子，**不是自动边**。opaque typedef 识别覆盖
  `typedef struct tag *handle`、`typedef struct tag alias; alias *` 以及两段式
  pointer typedef；无法展开的宏仍属残余 UNKNOWN。
- 仅“同包/同 ELF 存在 STL”不能证明 opaque 指针两侧的实际 cast 契约，故未
  用来升格。

## 4. 升格重算

- A8 基线：P0 生产核心波 **130 包**、生产分量 **1,570**。
- 并入 5 条生产 `YES_POINTEE` 边及 0 条确凿 `C_EDGE_STL_LEAK` 后：核心波
  下界仍为 **130 包**，新增核心成员 **0**；生产分量变为 **1,568**。
- 新升格边在核心外合并 OpenCV/NNTrainer/vision 族：A8 的 3 个分量合成
  **44 包**分量。
  完整谱系见 `component_merge_lineage.tsv`，完整分量见
  `components_production_tier1_a9.tsv`。

因此供 HLD 使用的新数据下界为：**真实生产核心波 ≥ 130 包**。不可判定差距
不能换算成一个可相加的包数；其证据面为 **692 条生产 unknown 边、431 个端点
包**。其中 **376 条**跨越当前 130 包核心边界，涉及核心外 **200 包**。这些数
表示待人工核验的最大候选面，不表示 200 包必然加入核心。

## 方法边界与残余

1. `void*`/opaque handle 两侧的 cast、所有权和 flavor 契约通常不出现在动态
   符号或公开声明中，必须由隧道登记册人工确认，并在确认后施加同波约束。
2. 指针链只跟进一层；pointee 内的第二层指针已显式标成
   `UNKNOWN_POINTER_DEPTH`，经容器保存的间接对象、函数指针回调携带对象均未
   继续推断。
3. 宏生成声明、条件编译后的真实类型和缺少公开定义的 PImpl 无法从当前头文件
   静态确证；均保留为 UNKNOWN，而不是 NO。
4. `C_EDGE_STL_LEAK=0` 仅表示严格的公开头证据门没有命中，不排除实现私有的
   C→C++ 隧道。
