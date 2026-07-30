# P1 出口评审开放问题(评审方:Claude,2026-07-27;限制条件与 EVIDENCE_GAP,不混入结论)

以下各项不构成 FAIL 依据(阻塞项见 review_conclusions.md B1–B3),
但需在对应节点前闭合或声明。

## OQ-1 D-G3 证据链包外(EVIDENCE_GAP)

`decision_record.md` D-G3 与 `base_unified/summary.md` 引用
`base_first_8_verification.tsv`、`base_first_component_diffs.tsv`,均不在
corpus,SOURCE_MAP/REF_INDEX 也无条目。boost-test 摘除("生产入边为 0")
与 abseil-cpp 移出的依据在包内不可复核。闭合:两文件入 REF_INDEX 附 SHA,
或在下一版评审包补录。

## OQ-2 census v2 的发现边界必须成文

A8/A9 移植为"冻结证据边条件回放",不重跑 A6 布局扫描与 A9 指针/C 边
发现。执行时点快照上 0716 之后新生的隐藏布局耦合边系统性不入 TIER1
现势数字;承接网为 TIER2 上界 + D1a 波界复审 + 隧道登记册(均冻结强制)。
要求:METHOD.md/errata 增加一段边界声明,并定义 A6/A9 发现扫描的重跑
节律(建议:每次 Base/Unified 快照对齐后、核心波开工前各一次)。

## OQ-3 新架构锚点的豁免台账行

aarch64 新增 rust/rust-devel/libedgetpu1-{max,std},x86_64 另增
jdk/dotnet-build-tools。census v2 证据绑定新鲜 ✓,但 D2 豁免清单
(5 包版)未见对应的扩充处置行(是否豁免、约束哪些波)。9 月三架构
波次开工前补。

## OQ-4 member_edge_sha256 的摘要域未在包内证明

台账 README 称摘要覆盖"成员集和有效边集摘要";边的 class
(CPP_ABI/CPP_NOSTL/升格)是否入摘要、边方向/符号层是否入摘要,包内无
规格或生成代码。若 class 不入摘要,存在"同成员同拓扑、不同边类"被判
共享的边角。闭合:一段摘要域规格 + 一个跨类反例自检行加入
`ledger/validation.tsv`。

## OQ-5 D5 正式采用前的平台级阴性对照(采用门,非 P1 门)

现有阴性对照 = 1 个合成 fixture。正式写入 buildconf 前:
(a) 以空 allowlist 对全平台 spec 跑 `rpmspec -P` 差分(或分层抽样),
断言零展开变化、零解析失败;(b) 对至少一个真实复杂包(重定义
Optflags/%build_ldflags 类)做逐字节负对照。D5 文档的
ADOPTION_BLOCKED 停止点已把采用另置门,本项挂入该门即可。

## OQ-6 D5 allowlist 准入检查:-nostdlib / 自定义 ld 脚本 / 静态-only 包

`-lc++ -lc++abi` 经 %build_ldflags 注入后,-nostdlib 链接中显式 -l 仍会
生效,可使刻意 freestanding 的产物意外携带 libc++ 依赖。allowlist 准入
模板增加一项:包内是否存在 -nostdlib/-nodefaultlibs/自定义 ld 脚本/纯
静态归档产物,存在则单独裁决注入面。同时确认平台 LDFLAGS 是否默认
`--as-needed`(决定混 C/C++ 包中纯 C DSO 是否携带无用 libc++ NEEDED)。

## OQ-7 binary→source 映射校验工件

D5 判定单位为 source Name,台账为二进制包。映射的机械生成与校验工件
(binary→source 去重表 + 断言)未在 corpus 中。allowlist 首次生成时落盘。

## OQ-8 `package_wave_preassignment` 的 BASE_FIRST=5 与六包名单

base_unified 统计 BASE_FIRST=5,base_first_6 为 6 包:差额应为
boost-iostreams(仅测试消费者,不构成 affected 端点)。为免执行侧误读
"名单被削",在台账 README 或 D-G3 文本加一行说明。

## OQ-9 wave4_notice EN 版仅抽验

CN 版逐句核过;EN 版本仅抽验数字与兜底段。发送前建议做一次 CN/EN 逐段
对齐核对(受众为发布线 owner,双语不一致的代价高)。

## OQ-10 排期 12 月总门与 G7 rider 时序的显式勾连

排期 11 月核心波、12 月完成;G7 三 rider 的时序门是"P2 镜像组装前"。
波 1 若 8 月开工,rider 封边补丁的开发窗口在 8 月内——排期未列该工作
项的 owner/交付行。建议在 8 月表加一行"G7 rider 封边补丁(bundle/ppm/
pkgmgr)",避免隐含在波次内。
