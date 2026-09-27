# 运行时状态备案：逐节来源索引

更新日期：2026-09-21。对应 [LINE_STATUS.md](LINE_STATUS.md)。只索引已有材料及本次人工维护指令；未重新扫描接口、执行探针或使用开发板。

## 证据版本及取用方式

下表所有仓内证据固定采用材料提交 **`093286888441e9b3f6d817d90eabb48ae434c8df`**。使用 `git show 093286888441e9b3f6d817d90eabb48ae434c8df:docs/<下表相对路径>` 可取当时原文；Markdown 链接用于浏览当前工作树，若以后文件改动，应以固定提交解释本版状态。该提交包含版本 2 汇编及枚举补测，并可读取所有更早证据，不是声称各项在同一天完成。

平台实施 HEAD **`c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13`** 是另一个源码仓在归并时的身份，不能当作本材料仓提交或当前远端实时状态。相关五个实施 SHA、tree 和 Change-Id 在归并报告中逐项列明。

## 各节引用的源文件

| 状态节 | 来源文件（相对 docs/，全部取上述固定提交） | 用途与界限 |
| --- | --- | --- |
| 1 这条线是什么 | [progress/IMPL_0909/W4/DELIVERY_ZH.md](progress/IMPL_0909/W4/DELIVERY_ZH.md) | 运行时两批改动、交付和非发布承诺 |
| 1、2、3、6、8 | [progress/RUNTIME_PHASE_SUMMARY_0921/NEXT_STAGE.tsv](progress/RUNTIME_PHASE_SUMMARY_0921/NEXT_STAGE.tsv) | 逐边提供包、初始化条件及范围；不是产物现货清单 |
| 2、4、7 | [progress/IMPL_0909/W3/SUPPORT_SCOPE_ZH.md](progress/IMPL_0909/W3/SUPPORT_SCOPE_ZH.md) | 取消前提、五处边界、四种绑定、重编要求和未覆盖 |
| 2、3、4、5、6、7 | [progress/RUNTIME_PHASE_SUMMARY_0921/SUMMARY_zh.md](progress/RUNTIME_PHASE_SUMMARY_0921/SUMMARY_zh.md) | 版本 2 总览；44 候选、18/23、架构范围、新发现及挂账 |
| 3、4 | [progress/RUNTIME_PHASE_SUMMARY_0921/EDGES.tsv](progress/RUNTIME_PHASE_SUMMARY_0921/EDGES.tsv) | 23 边完整 ELF 路径/SHA/符号；不代替跨库运行 |
| 4 基础补丁、7 板子 | [progress/R69/REPORT.md](progress/R69/REPORT.md) | 四补丁身份/重放；当时 PARTIAL 保留；历史板子地址 |
| 4 基础对照 | [progress/R81/REPORT.md](progress/R81/REPORT.md) | ARM owner 标准化基线、补丁套件对照；不包含全部精确取消触发 |
| 4 实施 | [progress/IMPL_0908/W3/REPORT.md](progress/IMPL_0908/W3/REPORT.md) | x86 原生/ARM 实板定向矩阵、状态及正常路径 |
| 4 析构、5 接受标准 | [progress/IMPL_0909/W1/REPORT.md](progress/IMPL_0909/W1/REPORT.md) | 新/旧/GNU 指定样本 3/9/0、显式 9；行为对齐不等于字节全同 |
| 4 套件 | [progress/IMPL_0909/W2/TWO_ARCHITECTURES.tsv](progress/IMPL_0909/W2/TWO_ARCHITECTURES.tsv) | 两架构各 11,402 项结果分母与状态，不是全部 PASS |
| 4、5 分支 | [progress/P6B_0909/REPORT.md](progress/P6B_0909/REPORT.md) | 原样快进五提交、tree/消息身份、删除指定旧分支及其他引用核对 |
| 4 adaptor 机制 | [progress/API_0911/W2/REPORT.md](progress/API_0911/W2/REPORT.md) | 真 Boost 双 TU：四格、20 次、指定输入；不替代其他接口 |
| 2、4 adaptor 修复 | [progress/P13_0917/FINAL.md](progress/P13_0917/FINAL.md) | 错误终止修复、回归、当前 PARTIAL 与行为变化 |
| 4、6 adaptor 契约 | [progress/P13_0917/W3/ERROR_CONTRACT.md](progress/P13_0917/W3/ERROR_CONTRACT.md) | bool/errno、失败状态与不保证的范围 |
| 4、5、6 adaptor 归类 | [progress/P14_0917/FINAL.md](progress/P14_0917/FINAL.md) | GNU 双异常对照、provider 外部缺陷、候选未完成修后验证 |
| 4、6 provider | [progress/P14_0917/W2/REPORT.md](progress/P14_0917/W2/REPORT.md) | 834 字节独立复现、源码定位、候选 patch 与缺构建前置 |
| 4 真实 ARM 边 | [progress/R119_ARM_RETEST/ACTUAL_ARM_EDGES.json](progress/R119_ARM_RETEST/ACTUAL_ARM_EDGES.json) | ARM 消费 UND/提供定义及路径/SHA，不沿用 x86 实证 |
| 4 时钟 | [progress/RUNTIME_PHASE_SUMMARY_0921/EVIDENCE.md](progress/RUNTIME_PHASE_SUMMARY_0921/EVIDENCE.md) | clock、clock-gnu、clock-cxx 等原定义与探针摘要回链 |
| 4 整数 | [progress/RUNTIME_PHASE_SUMMARY_0921/BUILTIN_TYPES.tsv](progress/RUNTIME_PHASE_SUMMARY_0921/BUILTIN_TYPES.tsv) | 13 表达式逐架构结果与来源；不是 13 独立故障 |
| 4 整数、5 原不测范围 | [progress/R119_ARM_RETEST/FINAL.md](progress/R119_ARM_RETEST/FINAL.md) | ARM/aarch64 配置、结果、WaitUntil/Reader 两机制、旧 572 范围 |
| 4 枚举 x86 | [progress/R119_DIVERGENT/TYPE_RESULTS.tsv](progress/R119_DIVERGENT/TYPE_RESULTS.tsv) | 只读取 fmtflags/iostate/openmode/seekdir 四行作既有 x86 证据 |
| 4、6 枚举补测 | [progress/R119_ENUM_RETEST/FINAL.md](progress/R119_ENUM_RETEST/FINAL.md) | ARM/aarch64、五轮、size/align、Seek 名称与槽位、四身份五声明、零新增边 |
| 4、5 版本 | [progress/RUNTIME_PHASE_SUMMARY_0921/VERSION_20260921_2.md](progress/RUNTIME_PHASE_SUMMARY_0921/VERSION_20260921_2.md) | 旧版/差异保留、五份文档增补、572→568；旧生成器/自检属于版本 1 |
| 5 组件迁移/SDK | [progress/P10_0915/adaptor_proposal_for_review.md](progress/P10_0915/adaptor_proposal_for_review.md) | 已裁决前提、选择性不迁移排除但接口层允许 |
| 5 日期辅助 | [progress/P10_0915/DELIVERY.md](progress/P10_0915/DELIVERY.md) | 2026-09-15 交付时间，仅辅助核对，不冒充独立裁决时间戳 |
| 5 SDK 前置 | [progress/ADAPTOR_0912/W1/REPORT.md](progress/ADAPTOR_0912/W1/REPORT.md) | 真实构建/初始化缺口，未要求先取得 SDK 清单 |
| 5 SDK 范围 | [progress/ADAPTOR_0912/W4/adaptor_analysis_zh.md](progress/ADAPTOR_0912/W4/adaptor_analysis_zh.md) | SDK 清单不是机制分析前置，也不宣称 SDK 全覆盖 |
| 5 结构/阴性 | [progress/R119_RECLASSIFY/SCOPE.md](progress/R119_RECLASSIFY/SCOPE.md) | S_R119_20260918、S_A；结构口径与限定阴性不能越过的洞 |
| 5 重判 | [progress/R119_RECLASSIFY/FINAL.md](progress/R119_RECLASSIFY/FINAL.md) | 结构性不适用与限定范围分类，非平台级无使用 |
| 6 C/D | [progress/R119_RECLASSIFY/CD_GAPS_62.tsv](progress/R119_RECLASSIFY/CD_GAPS_62.tsv) | 每项缺口、重启条件、本线可解性原文 |
| 6 C/D 方法 | [progress/R119_RECLASSIFY/CD_ASSESSMENT.md](progress/R119_RECLASSIFY/CD_ASSESSMENT.md) | C 23/D 39，函数身份与真实材料要求，不把估计当实测 |
| 6 挂账 | [progress/RUNTIME_PHASE_SUMMARY_0921/BACKLOG_zh.md](progress/RUNTIME_PHASE_SUMMARY_0921/BACKLOG_zh.md)、[BACKLOG.tsv](progress/RUNTIME_PHASE_SUMMARY_0921/BACKLOG.tsv) | 原 62 项与新增四项枚举分别登记，不重复计成包边 |
| 6 外部材料 | [progress/R110/W1_REPORT.md](progress/R110/W1_REPORT.md) | 外部组件分母与重建策略的材料需求和范围限制 |
| 7 资源 | [progress/R119_ENUM_RETEST/STATUS.md](progress/R119_ENUM_RETEST/STATUS.md)、[progress/R119_RECLASSIFY/STATUS.md](progress/R119_RECLASSIFY/STATUS.md) | 最近采用的 light、串行、nice/ionice、30% 内存与 Git 普通 I/O |
| 7 板上收尾 | [progress/IMPL_0909/W2/REPORT.md](progress/IMPL_0909/W2/REPORT.md) | 历史清理和释放，仅为操作依据，不宣称设备当前已释放 |
| 7 自检 | [progress/R119_ENUM_RETEST/AUDIT.json](progress/R119_ENUM_RETEST/AUDIT.json) | 原材料摘要/正向对照/限定改动验证，不在本轮重跑实验 |
| 9 开场核对 | 第 1–8 节上述所有相关来源 | 提炼既有事实，不增加计数或产品结论 |

## 人工原文与日期缺口

本线程的明确裁决是状态文件第 5、7 节的授权来源：其他包使用 libcxx-migration、不新建 sandbox、运行库调用单列、结构与限定阴性、停止投影、原不测集合及枚举例外、串行/内存/Git 规则、有限 root 清理和每任务收尾维护。已有文件能核其执行或结论，但没有统一保存全部原消息的独立时间戳；因此未知日期明确记 NOT_OBSERVED，不推断日期。

任务包上的日期范围只是任务标签；部分目录名与实际交付日期不同。归并文件明确标注的 2026-09-11 是实施/记录日期，不自动成为人工发出裁决的日期。本次 2026-09-21 的固定备案指令按当前会话日期记录。

## 本次文档自检范围

自检结果：固定九节齐全；两份新文档的本地链接全部存在；去重后 38 份引用源文件逐字节匹配上述固定材料提交。这里只校验文档来源和结构，没有重跑任何实验。未留存独立裁决时间戳的条目仍如实标为 NOT_OBSERVED。

- 只新增 LINE_STATUS.md 与本索引；既有证据不改、不新测，不用板子。
- 计数分开：候选/包对/符号边、类型表达式/结果位置、目标类型测量/真实 ELF 引用/产品运行；ARM 两条不是完整分母。
- 原四补丁历史 PARTIAL 与后续验证分开；五处站点、四类实施、五提交不混为同一计数。
- 572 的旧范围与 4 项例外、568 的当前不测范围分别记录；整数仅“已测两种 LP64 配置”不同，不宣称所有 LP64。
- 时钟错值是推断；真实 provider 修后验证及 adaptor 生产验收未完成，均进入挂账而不写成闭合成果。
- 维护时同步更新来源和证据版本；仅记录现存可复核提交，不预填将来 SHA。
