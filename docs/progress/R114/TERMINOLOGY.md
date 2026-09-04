# 英文清单术语选择

任务书已指定的术语全部原样采用：

| 中文 | 英文 |
|---|---|
| 需要适配 | requires adaptation |
| 不需要适配 | does not require adaptation |
| 当前不能适配 | currently cannot be adapted |
| 跨包 C++ 运行时边 | cross-package C++ runtime edge |
| 布局敏感 | layout-sensitive |
| 静态归档 | static archive |
| 消费方 | consumer |
| 提供方 | provider |
| 符号交集 | symbol intersection |
| 设备镜像 | device image |
| 迁移集合 | the migrating set |
| 头文件载体 | header carrier |

表中还出现了以下术语。为保证 254 行一致，统一采用下列译法：

| 中文或原内部表达 | 统一英文 | 说明 |
|---|---|---|
| C++ 候选证据并集 | C++ candidate evidence union | 明确它是四类机械证据的并集，不称为绝对完整全集 |
| 实际成功构建日志中的 C++ 动作 | a successful build log contains C++ compilation | `actual` 的含义由 successful build log 保留 |
| C++ runtime 依赖 | C++ runtime dependency | 与方法说明的 RPM dependency 口径一致 |
| 可解码的全局 C++ 符号 | globally visible demangleable C++ symbols | 同时保留可见性和可 demangle 两个条件 |
| 精确运行时图 | exact runtime graph | 不把“精确”扩大为全架构；理由中继续明确 armv7l |
| 架构份次 | architecture-specific instance | 表示同一载体在不同架构上的记录，不译成 package count |
| C++ ABI 耦合但布局未证明 | verified C++ ABI coupling without proven concrete standard-library layout transfer | 保留“已核实耦合”和“不能证明具体布局传递”的区别 |
| 仅 C 接口 | C interfaces only | 不译成 ABI-safe；安全性结论仍由理由中的限定范围承担 |
| 无可见符号交集 | no visible symbol intersection | 不扩大为没有任何关系 |
| 当前实施状态 | current implementation status | 与判定列分开，避免把“暂停”误读为判定变化 |
| 已完成并推送 | completed and pushed | SHA 原样保留 |
| 未观测 | not observed / `NOT_OBSERVED` | 正文自然语言用 not observed；既有状态值保持 `NOT_OBSERVED` |
| 在已核查的消费范围内 | within the verified consumer scope | 用于 `gmp`，不扩大到未知 consumer |
| 至少涉及 546 个归档成员 | involving at least 546 archive members | `at least` 保留原文下界语义 |

内部规则码没有音译或保留，而是按 R113 给出的公开语义展开；这只改变对外表达，不改变规则命中或判定。
