# 波 4 Base + Unified 原子发布协同知会

**受众：Tizen Base / Unified 发布线 owner｜目标：2026-10-31 前冻结协同机制**

## 为什么必须同窗发布

0723 对齐二进制图显示，波 4 的修正 TIER1 核心在
armv7l/aarch64/x86_64 分别为 **118/121/122 包**，其中 Base 成员在三架构
均精确为 **jsoncpp、libicu、libpcrecpp**，其余 **115/118/119 包**来自
Unified；核心内部存在可复现的 Unified→Base C++ ABI 边。标准库切换后，
边界两侧若在外部可见状态中分属新旧标准库，会出现对象布局、异常与模板实例
不一致，因此 Base 可先构建，但 Base provider 与 Unified consumer 必须在
同一发布窗口原子晋级，不能形成可消费的半迁移快照。

**口径说明：执行清单以执行时点快照为准；0723 现值为
118/121/122，冻结方法论基线为 130。**

bundle 侧同时以 D-B1 的 **E48 精确版本脚本**承接旧 C++ 导出，并由 G4
反向断言确保新主体 `libbundle.so` 不再导出 `tizen_base*`；该门是波 4
跨 flavor 隔离的配套背景，不替代双 repo 原子性。

## 发布窗口要求

1. Base 与 Unified 使用同一三架构候选矩阵、冻结 build IDs 和联合
   go/no-go；Base build 可先完成，但对外 promotion 必须同窗。
2. 窗口内不得出现“新 Unified + 旧 Base”或“新 Base 被旧 Unified
   消费”的公开快照；镜像组装只接受已签名的成对 snapshot token。
3. 失败或回退以 Base+Unified 成对快照为最小单元，三架构分别留存
   last-known-good 组合并复跑 G1–G7/板上 `LD_BIND_NOW`。

## 10 月底前需对齐的决策

- 两线 release owner、联合变更窗口、冻结/撤回权限和 go/no-go 签字人；
- Base-first build 与 Unified rebuild 的 cut-off、成对 snapshot/token 格式；
- 三架构例外与锚点豁免是否允许进入同窗，以及未齐架构的阻塞规则；
- E48/G4、跨 repo 边清单、强 UND/绑定归属和镜像场景集的证据 owner；
- 原子 promotion 技术手段、失败回滚 SLA、last-known-good 保留周期。

**兜底：**若联合窗口或核心门禁未按期就绪，交付双运行时镜像——已迁域使用
libc++，核心波继续使用 libstdc++，不做默认翻转。

数据附录：`core_base_members.tsv`、`core_cross_repo_edges.tsv`、
`cross_repo_edges.tsv`、`affected_base.tsv`、`affected_unified.tsv`。
