# 运行时验证线：计划与进度备案

更新日期：2026-09-21。材料分支：`codex/runtime-validation`。

本文件是本线交接的单一状态入口；详细实验事实仍以链接的原始材料为准，不复制替代原始记录。历史状态通过 `git log -- docs/LINE_STATUS.md` 查看。逐节来源见 [来源索引](LINE_STATUS_INDEX.md)。本次只汇编，不增加测量、不复核当前产品产物或板子在线状态。

**证据版本约定**：本次所有证据链接均固定到材料提交 **`093286888441e9b3f6d817d90eabb48ae434c8df`** 的文件内容；下文写作“证据提交 `093286888`”。这是可用 `git show <提交>:<路径>` 复核的归档版本，不声称每份材料首次写于此提交，也不是平台实施提交。索引区分材料版本与实施源码身份。

## 1. 这条线是什么

负责 libc++ 迁移的运行时行为、线程取消、跨标准库对象/异常/所有权边界验证，以及支持范围、adaptor 和对外证据材料；在授权下实施隔离探针或运行时补丁验证。不负责替编译线完成逐包构建适配、RPM 写包/压缩与构建故障诊断，也不代替组件方、业务方决定 SDK 开放范围或发布兼容政策。编译线提供匹配的 libc++ 构建包、头文件、依赖与构建身份，本线据此验证真实调用；共享板子与主机资源须协调。编译线的 GDB/写包工作不属于本线当前位置，不因代为交付其材料而并入本计划。[职责与交付边界](progress/IMPL_0909/W4/DELIVERY_ZH.md)、[逐边输入](progress/RUNTIME_PHASE_SUMMARY_0921/NEXT_STAGE.tsv)；证据提交 `093286888`。

## 2. 总计划

| 阶段 | 目标 → 前置 → 当前状态 |
| --- | --- |
| 取消基础修复 | 识别并继续传播强制展开 → 配套 libc++/libc++abi、GNU 展开器和确定补丁身份 → 四补丁及限定配置基线对照已交付；不等于任意取消安全 |
| 不抛边界及状态修复 | 处理等待边界、写者状态、析构同步及旧新入口 → 人工裁决、基础补丁、相关消费方重编 → 已实施、两架构定向及官方套件材料齐备；发布验收另行处理 |
| 接口盘点与 adaptor | 定界应用/组件边界并验证转换和错误契约 → 真实 provider、接口语义、调用路由 → 已有机制和限定场景实测，整体 PARTIAL；生产方向等见挂账 |
| 跨包面定界与类型复审 | 候选归类、结构链、按架构引用和新发现 → 冻结头/ELF、正向对照 → 已汇编 18 包对/23 符号边及三类发现；未定项暂停，不代表全平台清查完成 |
| 逐边混合运行验证 | 断言实际值、状态、生命周期和错误 → 每条匹配的 libc++ provider、GNU 控制及初始化条件 → 下一阶段输入已列，尚未获本次启动授权 |
| 产品支持与发布 | 合并验证结果、限定承诺、部署/回退验收 → 逐边结果、外部材料、业务与组件方裁决 → 待前置，不以材料推送替代验收 |

以上阶段是既有工作和下一阶段清单的组织，不是新授权或新排期。依据：[支持范围](progress/IMPL_0909/W3/SUPPORT_SCOPE_ZH.md)、[adaptor 当前结论](progress/P13_0917/FINAL.md)、[阶段汇编版本 2](progress/RUNTIME_PHASE_SUMMARY_0921/SUMMARY_zh.md)；证据提交 `093286888`。

## 3. 当前位置

**处于“定界与发现汇编完成，等待逐边验证启动”的交接点。** 版本 2 汇编与枚举补充已提交；本次用户要求建立固定备案，不继续候选关闭或跨包运行。

- 已有下一阶段输入是 **x86_64 的 18 包对/23 符号边**；其中 **2 条**另有真实 armv7l ELF 引用实证。不是“23 条已测跨库失败”。[完整边表](progress/RUNTIME_PHASE_SUMMARY_0921/EDGES.tsv)
- 卡在每条边的匹配 **libc++ 构建提供包**、依赖/头/来源 SHA 和可触发真实调用的夹具；本轮未重新盘点哪些新版包已经生成，不能写成全部不可得或全部就绪。[逐边前置](progress/RUNTIME_PHASE_SUMMARY_0921/NEXT_STAGE.tsv)
- 等人工明确启动批次与范围；等编译线/组件方提供或确认产物及初始化条件；正式 SDK/外部应用分母仍由业务与材料持有方确认，不把它升级为所有机制实验的前置。
- 本次不用板子。板子现在是否空闲/在线为 `NOT_OBSERVED`，不能沿用历史“已释放”判断实时状态。

本节数字的证据提交为 `093286888`。本文件建立后停止，待人工下一任务。

## 4. 已闭合的结论

这里只登记已成立的**有限事实**，不把挂账或推断写成已验证产品结论。每条均给证据路径与证据提交。

### 取消基础与五处站点的决定

- **【静态核查＋实测】基础四补丁已有可追溯交付。** 分别处理外来强制展开状态、GNU 强制展开标记、消费方头内处理器及库内处理器；源码快照和重放有一致性证据。ARM 官方基线与补丁侧按相同 owner 身份逐项对照，11,321 项 libc++、81 项 libc++abi 的状态一致，方案新增失败为 0；这不表示全部 PASS，也不直接证明官方套件未包含的精确取消路径。不得把早期报告的 PARTIAL 隐去或把后续实施当作四补丁原本已解决。证据：[基础补丁与身份](progress/R69/REPORT.md)、[ARM 完整基线对照](progress/R81/REPORT.md)、[后续交付中的两批关系](progress/IMPL_0909/W4/DELIVERY_ZH.md)；证据提交 `093286888`。
- **【静态核查＋实测】五处站点及后续实施已区分。** 五处是普通等待、system-clock 定时等待、steady-clock 定时等待、自定义 Clock 定时等待、现代 `wbuffer_convert` 析构；不是五个相同的库内入口。等待的相应不抛规格已解除，普通/定时共享锁写者增加回滚，现代转换缓冲析构去掉自动同步，两个库内等待入口保留旧节点并增加新默认节点。四类实施由五个源码提交完成，源码归并时 HEAD 为 `c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13`，在既有四补丁之上。头内实现、定时回滚须重编；C++03 转换缓冲析构不在现代析构改动内。证据：[五提交及验证汇总](progress/IMPL_0909/W4/DELIVERY_ZH.md)、[支持边界](progress/IMPL_0909/W3/SUPPORT_SCOPE_ZH.md)、[等树分支归并](progress/P6B_0909/REPORT.md)；证据提交 `093286888`。
- **【实测】两架构的限定等待、回滚和绑定行为已有记录。** x86_64 原生与 armv7l 物理板分别验证等待取消穿过、被断言清理、写者状态恢复及正常路径。旧 ELF 记录旧节点则保留旧行为；旧对象仅重链接不能补清理；新头重编且链接新节点的被测调用链通过；新头对象先链接旧库再换库仍绑定旧节点。普通异常无人捕获仍可终止，不声称解除规格普遍改善错误处理。证据：[两架构实施实测](progress/IMPL_0908/W3/REPORT.md)、[四种绑定及限定](progress/IMPL_0909/W3/SUPPORT_SCOPE_ZH.md)；证据提交 `093286888`。
- **【实测＋静态核查】析构变化及不覆盖范围已明确。** 新库与 GNU 在“不自动刷新”上对齐，不是字节数完全一致：指定三方样本新/旧/GNU 为 3/9/0 字节，显式同步均为 9。两架构官方套件各 11,402 项有结果，新增 overflow 失败按人工标准接受并保留 FAIL。异步取消与目标站点正交；`call_once` 等待不经过本次改动入口，不在写者回滚名单。证据：[三方样本](progress/IMPL_0909/W1/REPORT.md)、[两架构完整统计](progress/IMPL_0909/W2/TWO_ARCHITECTURES.tsv)、[支持范围和排除条件](progress/IMPL_0909/W3/SUPPORT_SCOPE_ZH.md)；证据提交 `093286888`。

### adaptor 的已验边界

- **【实测】双编译单元转换机制在真实 Boost.Program_options 固定样本中成立。** 两侧各自构造/销毁对象，中间传指针、长度等数据；x86_64 四格各五次，共 20 次运行。不是目标接口全部可部署的替代证据。证据：[机制及原始矩阵入口](progress/API_0911/W2/REPORT.md)；证据提交 `093286888`。
- **【实测】EWK 反向组合的正常场景和同步错误隔离已有有限结果。** 真实 GNU provider 的 libc++ 应用方向，两种放法的 12 场景及 17 边界回归未见退化；原后端异常 5/5 SIGABRT 的用例修后 5/5 返回 -ENOMEM、存活并保持输出。公开接口仍为 bool，bad_alloc 映射 false/ENOMEM，length_error→EOVERFLOW，invalid_argument→EINVAL，其余同步异常→EIO；原 GNU 可抛异常，故这是已记录行为变化，不是完全透明替代。证据：[错误修复和回归结论](progress/P13_0917/FINAL.md)、[错误契约](progress/P13_0917/W3/ERROR_CONTRACT.md)；证据提交 `093286888`。
- **【实测＋静态核查】provider 泄漏与双异常已完成性质区分。** 原始 GNU EWK 无 adaptor 也在指定分配失败点遗留 834 字节；展开中第二异常逸出在 GNU-only 与既测 adaptor 路径均终止。候选 patch 不是已验证修复，修后验证见挂账。不能把 `extern "C"` 说成语言自动禁止异常。证据：[性质核查](progress/P14_0917/FINAL.md)、[独立复现与定位](progress/P14_0917/W2/REPORT.md)；证据提交 `093286888`。

### 按架构的跨包面与三类新发现

- **【静态核查：头、存储链与真实 ELF】跨包登记为 18 有向包对/23 符号边。** 原 44 个候选关联 16/21，后补不同消费方的 WaitUntil 与 Reader 得到 18/23；候选不是互不重复边。x86_64 冻结 3,085 个 ELF，占该架构 10,946 个运行 ELF 约 28%。ARM 定点确认 ui-foundation→dali2 与 zypper→libzypp 两条 UND/定义对应，其他边不能继承 ARM 实证。Reader 是 InputStream 对象边，内部 streamoff 不在其构造函数名中。证据：[边及逐 ELF/SHA](progress/RUNTIME_PHASE_SUMMARY_0921/EDGES.tsv)、[计数与归类](progress/RUNTIME_PHASE_SUMMARY_0921/SUMMARY_zh.md)、[真实 ARM 引用](progress/R119_ARM_RETEST/ACTUAL_ARM_EDGES.json)；证据提交 `093286888`。
- **【实测别名/空类型＋静态语义】高分辨率时钟的时间含义不同。** 已测 x86_64 配置的 GNU `high_resolution_clock` 是 system_clock，libc++ 是 steady_clock（依赖单调时钟配置）；两侧时钟标签均空，不存在本标签非静态数据布局差异，不代表 time_point 没有状态。WaitUntil 明确使用 steady_clock，不能说它已测得高分辨率时钟静默错值。跨时钟域把纳秒整数重解释的错值场景仅为**条件性推断**，产品验证未闭合。证据：[事实链及推断边界](progress/RUNTIME_PHASE_SUMMARY_0921/SUMMARY_zh.md)、[定义与探针回链](progress/RUNTIME_PHASE_SUMMARY_0921/EVIDENCE.md#clock)；证据提交 `093286888`。
- **【三架构类型实测】13 项内建整数选择差异仅在已测 LP64 两架构出现。** x86_64/aarch64 为 GNU long(`l`)/libc++ long long(`x`)，同为 8 字节；armv7l 两侧 long long(`x`)，该类别消失。不是外推所有 LP64 配置，也不是宽度冲突。ARM WaitUntil 仍受 chrono 类型名称差异影响；Reader 名称相同但对象边仍在。证据：[逐项三架构表](progress/RUNTIME_PHASE_SUMMARY_0921/BUILTIN_TYPES.tsv)、[ARM 与 aarch64 实测/函数符号](progress/R119_ARM_RETEST/FINAL.md)；证据提交 `093286888`。
- **【三架构类型实测＋ARM 调用方编译观察】四项流状态类型身份仍分歧。** fmtflags/iostate/openmode 为 GNU 枚举对 libc++ unsigned int；seekdir 为两套不同枚举，已测三架构均不同、大小均 4。ARM/aarch64 补测两侧 size/align 均 4/4；真实 ARM podofo 头生成的 Seek 名称不同，普通虚调用却同取索引 6，不能由名称推槽位/产品结论。复用的四个函数身份、五条公开签名在限定 x86_64 集合内未找到异包 UND，有 bundle::Add 正向对照；不排除虚调用/内联，新增确认边为 0，原成员边保留。证据：[x86_64 类型表](progress/R119_DIVERGENT/TYPE_RESULTS.tsv)、[两目标补测、编码和暴露](progress/R119_ENUM_RETEST/FINAL.md)、[汇编版本说明](progress/RUNTIME_PHASE_SUMMARY_0921/VERSION_20260921_2.md)；证据提交 `093286888`。

## 5. 人工已裁决的前提

日期是裁决时间而非实验时间；无独立日期记录的写 `NOT_OBSERVED`，不拿文件名或 git 提交日补成裁决日。以下来源均可在证据提交 `093286888` 复核；本次任务书本身另作为 2026-09-21 的新维护指令。

| 前提 | 裁决日期及可核范围 | 依据 |
| --- | --- | --- |
| chromium-efl 整体迁至 libc++，不保留 GNU 完整组件；少量 GNU adaptor 接口层与同 DSO 部署变体允许 | 2026-09-15（任务书日期；当日交付回执） | [已裁决前提](progress/P10_0915/adaptor_proposal_for_review.md)、[当日回执](progress/P10_0915/DELIVERY.md) |
| SDK 清单推迟，不阻塞现有接口的机制分析；不得由此声称 SDK 全覆盖 | 任务包标注 2026-09-12～09-14；裁决消息独立时间戳 NOT_OBSERVED（落盘可能早于任务包标签） | [适用范围](progress/ADAPTOR_0912/W4/adaptor_analysis_zh.md)、[前置探查](progress/ADAPTOR_0912/W1/REPORT.md) |
| LLVM 仅用 sandbox/lhmax2025/libcxx-ehabi-backport，其他包用 sandbox/lhmax2025/libcxx-migration；不再新建 sandbox；旧 noexcept-relief 不再是现行依赖 | 裁决消息日期 NOT_OBSERVED；已于 2026-09-11 完成归并并记入现行交付说明 | [归并/删除核验](progress/P6B_0909/REPORT.md)、[现行实施取件说明](progress/IMPL_0909/W4/DELIVERY_ZH.md)；其他包分支策略来自本线程人工原文 |
| 应用调用标准库自身单列 RUNTIME_CALL，不计两个平台包之间的 CROSS_PACKAGE | 裁决日期 NOT_OBSERVED；不可按分批目录日期补全 | [计数规则与最终表](progress/RUNTIME_PHASE_SUMMARY_0921/SUMMARY_zh.md) |
| NO_STDLIB_OBJECT 依结构；限定范围阴性获准，带具名范围与洞，不提升为平台级“不存在” | 裁决日期 NOT_OBSERVED；冻结范围名中的日期不是裁决时间 | [具名范围](progress/R119_RECLASSIFY/SCOPE.md)、[重判口径](progress/R119_RECLASSIFY/FINAL.md) |
| 65 项结果投影的完整关闭停止；不继续为 0/65 的低闭合率投入 | 裁决日期 NOT_OBSERVED；当前汇编已记录停项 | [挂账与投影计数](progress/RUNTIME_PHASE_SUMMARY_0921/SUMMARY_zh.md) |
| 原 572 项不测；随后只重开 4 项枚举，其余 568 项仍不测 | 初次不测裁决日期 NOT_OBSERVED；本次四项补充及收账为 2026-09-21 | [原复测范围](progress/R119_ARM_RETEST/FINAL.md)、[版本 2 更新](progress/RUNTIME_PHASE_SUMMARY_0921/VERSION_20260921_2.md) |
| 五处站点的实施按既定四类执行；析构接受标准是“不自动刷新”与 GNU 对齐，不是输出字节数相同 | 实施任务包标注 2026-09-08 夜；三方表述澄清消息独立日期 NOT_OBSERVED | [实施交付](progress/IMPL_0909/W4/DELIVERY_ZH.md)、[三方对照](progress/IMPL_0909/W1/REPORT.md) |
| adaptor 的原三项阻断改归行为变化、provider 外部缺陷、语言固有限制；整体 PARTIAL，不是发布通过 | 任务书标注 2026-09-17；独立消息时间戳 NOT_OBSERVED | [当前分类](progress/P14_0917/FINAL.md) |
| 固定状态文件作为本线交接入口，每个任务收尾同步更新 | 2026-09-21，本次人工指令 | 本文件第 9 节后的维护惯例 |

待人工补充的只是上表未留存的精确裁决日期，不重新打开已决定的技术口径。

## 6. 挂账

| 项 | 状态与范围 | 重启条件/待谁提供 | 证据 |
| --- | --- | --- | --- |
| 23 条边的混合运行 | 未完成逐边真实双向运行；现有表是引用证据，不能改称兼容/失败矩阵 | 人工选定批次；编译线/组件方提供逐行 libc++ provider、匹配头/依赖/构建身份；本线建真实夹具 | [逐边输入](progress/RUNTIME_PHASE_SUMMARY_0921/NEXT_STAGE.tsv) |
| 62 项 C/D 未定 | 主因 C 23、D 39；原条目保留，不按缺材料补阴性 | 逐项取得函数身份、实例化/调用链/绑定/异常等证据；本线可解性逐项见表，不一概要求人工提供 | [原 62 项缺口表](progress/R119_RECLASSIFY/CD_GAPS_62.tsv)、[方法评估](progress/R119_RECLASSIFY/CD_ASSESSMENT.md) |
| 65 项编译期设施投影 | 0/65 完整关闭、15,793 处未定；已停；无对象位置须两侧类型相同 | 人工重新授权，再补真实实参、实例化上下文与结果结构；不以手选样本代替平台实例 | [当前计数与范围](progress/RUNTIME_PHASE_SUMMARY_0921/SUMMARY_zh.md) |
| 第 201–495 个候选 | 295 项未定界；不自动续做 | 人工启动并确认方法、范围与资源 | [挂账](progress/RUNTIME_PHASE_SUMMARY_0921/BACKLOG_zh.md)、[阶段范围](progress/RUNTIME_PHASE_SUMMARY_0921/SUMMARY_zh.md) |
| 四项枚举的产品暴露与影响 | 类型身份已确认；直接公开签名的异包消费未闭合；不是新增四条边 | 真实消费者、虚调用/注册链、provider 表项及参数/生命周期材料 | [独立枚举条目](progress/RUNTIME_PHASE_SUMMARY_0921/BACKLOG.tsv)、[限定查询](progress/R119_ENUM_RETEST/FINAL.md) |
| 其余 568 项 ARM 复测 | 不测，仍未测；既有 x86 构成是相同 283、命名空间/标签 114、实现类/组合 111、不可得 60；不能称全部已知实现差异 | 仅人工改变范围并给出产品问题时重启 | [不测集合说明](progress/RUNTIME_PHASE_SUMMARY_0921/SUMMARY_zh.md) |
| 高分辨率时钟的产品错值 | 条件性推断；没有已测的真实产品静默错值 | 真实时间跨界路径、时钟域/起点/单位契约和结果断言 | [事实与推断分界](progress/RUNTIME_PHASE_SUMMARY_0921/SUMMARY_zh.md) |
| adaptor 生产方向/部署 | GNU 应用→libc++ EWK 未取得真实 provider；产品打包、升级/回退、armv7l、并发/取消、真实 vector 回调未验 | 组件迁移产物、注册/宿主与产品环境；板子可用并获协调后再实测 | [当前缺口](progress/P14_0917/FINAL.md)、[错误契约](progress/P13_0917/W3/ERROR_CONTRACT.md) |
| provider 泄漏修复 | 已定位并有候选 patch；修后编译/运行、泄漏归零 NOT_OBSERVED | 组件方确认局部 GN 草案，补产品生成头/参数/依赖闭包；重建真实 provider 后复验 | [复现/候选及阻塞](progress/P14_0917/W2/REPORT.md) |
| 取消支持发布及外部组件 | aarch64 实施动态矩阵、任意 Clock/回调、异步取消等未覆盖；外部组件分母/重建策略不可得 | 人工单独启动；外部材料持有方提供清单和重建政策；产品运行库/包身份验收 | [支持范围](progress/IMPL_0909/W3/SUPPORT_SCOPE_ZH.md)、[外部材料需求](progress/R110/W1_REPORT.md) |

本节证据提交 `093286888`。这里没有把“本线可研究”写成“必能关闭”，也没有把已停止的工作排成自动待办。

## 7. 常设约束

- **仓库和授权**：材料分支 `codex/runtime-validation`；`codes/` 只读，不修改平台源码、配置或既有实验结果；新实现/重测必须有任务授权。禁止擅推 Gerrit、正式分支和 LLVM 上游；历史一次性 force 授权不可复用，默认不 force。现行源码分支策略见第 5 节。
- **资源**：沿用最近任务的保守配置：先 `tools/resource_gate.sh --level light`，串行、`nice -n 19`、`ionice -c 3`，RLIMIT_AS 为实际内存 30%；闸门失败即保存断点停止，不抢编译线资源。新任务若要求 medium 或给出重试规则，按新任务执行，不把旧的等待重试政策混用。`git add`、`git commit`、`git push` 使用普通 I/O 优先级。依据：[当前资源记录](progress/R119_ENUM_RETEST/STATUS.md)、[具名范围阶段资源规则](progress/R119_RECLASSIFY/STATUS.md)。
- **板子**：历史设备 IP `192.168.108.26`，SDB 端口 `26101`；IP 不是本轮在线确认。需要时先查编译线状态和当前占用、确认无遗留，再运行；清理仅限明确属于本任务的路径/诊断，归属不明列出不动，记录并恢复身份；释放必须核验后写明。有限 root 清理授权不扩为安装或改配置。历史来源：[板子地址与连接记录](progress/R69/REPORT.md)、[后续清理交接](progress/IMPL_0909/W2/REPORT.md)。
- **证据**：中文；命令原文、stdout/stderr、退出码落盘，脚本快照及 SHA256；零命中须有正向对照；实测/静态/推断分开；未知用 NOT_OBSERVED，材料不可得用 NOT_AVAILABLE 并记已试方法。不得由“无崩溃”推“无问题”。
- **实测纪律**：相关运行任务用真实 provider、同侧控制和明确加载身份（通常 `/proc/self/maps`）；断言内容、状态和适用的销毁行为，每格五轮；QEMU/物理板/原生分别标注，不跨架构外推。配置/实参失败不得静默替换。依据：[逐边模板与范围](progress/RUNTIME_PHASE_SUMMARY_0921/SUMMARY_zh.md)、[复测自检](progress/R119_ENUM_RETEST/AUDIT.json)。
- **材料保存**：不覆盖冻结原始表；重判给前后映射；现行文档改动保留可审 diff/历史。每次只提交授权材料，保护其他 Session 的脏文件；不以 GitHub 推送成功证明产品验收。当前运行时工作树原有两份其他任务日志修改不纳入本备案。

本节操作惯例来自本线程明确授权；仓内可核来源的证据提交为 `093286888`。新任务有更严格限制时服从新任务；不因备案扩大授权。

## 8. 下一步

**等待人工选定 23 条已登记边中的启动批次，在逐行 libc++ provider、GNU 对照、头/依赖/构建身份及初始化条件就绪后开展逐边验证；本次不启动。** 输入为 [NEXT_STAGE.tsv](progress/RUNTIME_PHASE_SUMMARY_0921/NEXT_STAGE.tsv)，证据提交 `093286888`；不给总工时，不默认产物已就绪。

## 9. 给新 Session 的开场核对清单

开始前复述以下事实，任一项与任务或证据不符，先纠正再工作：

1. 我在运行时材料分支，不把编译线的写包/GDB 状态当成本线进度；先检查分支、HEAD、暂存区和脏文件。
2. 当前停止于汇编/备案交接，下一阶段未自动授权；读第 3、6、8 节和最新人工指令。
3. 四基础补丁、五处站点、四类实施、五个实施提交是不同计数；产品验收未由材料交付完成。
4. 18 包对/23 符号边仅及登记的 x86_64 范围；ARM 只另证实两条真实引用，不是完整 ARM 跨包面，更不是混合运行通过。
5. 13 项整数选择分歧在已测 ARM 消失，不删除对象边；WaitUntil 名称仍异，Reader 名称相同不代表内部对象安全。
6. 四项流状态类型三架构均分歧；Seek 的名称差异不能直接推出虚表槽位差异或产品失败。
7. 高分辨率时钟别名及时间含义不同已证；真实产品静默错值仍是有条件推断，不能嫁接给 steady_clock 的 WaitUntil。
8. adaptor 是 PARTIAL：反向机制与错误契约有证据，生产方向真实 libc++ EWK、真实回调和产品集成仍缺；provider patch 未修后验证。
9. 65 项关闭已停、62 项 C/D 有缺口、201–495 未定界；原 572 减去单列枚举 4 项后其余 568 不测，不能自动扩范围。
10. 先资源闸门、串行和 30% 内存限制；板子地址不是实时空闲证明；保留原始证据、独立架构和限定范围，不抢另一线资源。

### 维护惯例（此后遵守）

**“每个任务的收尾步骤增加一条：更新 `docs/LINE_STATUS.md` 的第 3、4、6、8 节，与任务产物同一个 commit。”**

无新增闭合结论时明确保持第 4 节不变；变更状态必须有证据和架构范围。同步维护 [来源索引](LINE_STATUS_INDEX.md)，新增证据固定到可取用的提交；与当前产物同提交的引用可先用路径，下一次维护再补该产物已知 SHA，禁止编造自引用提交哈希。不得借状态维护回写历史实验结果。
