# 编译适配线状态

## QuickBuild #1194606 修复 R4（当前：cereal 已推，另两包门禁待裁决）

cereal 保留 d62ccf97，追加批准的两处 performance int 分布转 char 源码补丁 a681c106a5866b6113b90a25307bf3abb3e76334；已推 sandbox/lhmax2025/libcxx-migration 并核对远端。四组宏、x86_64 完整构建与 performance 链接通过，原 basic_binary_oprimitive / basic_binary_iprimitive 未定义引用已实测消失；NEEDED 有 libc++、无 libstdc++；cereal-devel 清单不变、90 个头文件逐字节相同。tizen 分支空树占位的提醒仍适用，实际源码来自 manifest 28aedc95。

ICU b7ab6868 构建退出 0、tz2icu -iquote 修复实测，但 libicudata 与 libicui18n 没有直接 libc++.so.1 NEEDED，不满足 R4“各 .so”门禁，未推。qemu 候选 3b8d6274 未改；armv7l/aarch64 变体均在 x86_64 写包成功，无原 unpackaged 错误；aarch64 对前版同变体清单新增 8 项而非仅 2 项，门禁 FAIL，不推；依“失败停止该包”riscv64 未启动。armv7l 对应前版 RPM 未取得，不能用跨变体清单证明一致。

证据：docs/progress/QB_1194606_FIX/FINAL_RESULT_R4.md、verify-*_R4/、两个 FIRST_ERROR_R4.md。R5 当前 13 行远端均 MATCH，ICU 保持旧已推 dcbb，qemu 未加入；**不可作为第二轮 QuickBuild 开工清单**。R101 cereal 更新已推，判定计数 12/55/6/0、需改集合 12 个已有已推分支，不代表本轮后续修复都通过。BASE_LEDGER 的历史记录保留，另加 R4 补记。

下一步：人工裁决 ICU 两份库的直接依赖门禁，以及 qemu 额外清单项/缺失变体基线 → 继续未闭合验收并仅推通过者 → 完整 R5 → 人工决定第二轮 QuickBuild。本轮不上板、不起 QuickBuild、不改 codes/；所有构建与验收现场保留。

## QuickBuild #1194606 修复 R3（历史，验收失败后停止）

cereal 真实源码 ref 已确认：tizen_base/manifest 为 `28aedc95fdc600568c57babf28ff717e0b2bca7d`；tizen 的 `896a21f0` 是 Initial empty repository 空树占位，不能作为 QuickBuild 源码。ICU 使用已推 sandbox `dcbb3fb4`，qemu-accel 使用 manifest `2737f3bf` 而非较新的 tizen_base HEAD。三份候选仅在本地提交：cereal `d62ccf97`、ICU `b7ab6868`、qemu-accel `3b8d6274`；均未推送。

RBS `20261012.081855` 的实际 x86_64 根里，五份 spec × 四种宏组合差异门禁通过，payload 实测 w5.xzdio。cereal GBS 在约 450.5 秒后退出 1：performance.cpp:261/280 的 `std::uniform_int_distribution<char>` 被 libc++ 类型断言拒绝。CLANG_USE_LIBCPP=ON 已实际生效，但未到 performance 链接，不能宣称原 basic_binary_oprimitive 问题已实测消失。按原 FIX 第 8 条停止；ICU/qemu 的本地构建与验收 NOT_OBSERVED，未自行新增源码修法、未跳过 performance。现场和首错见 `docs/progress/QB_1194606_FIX/FINAL_RESULT_R3.md`、`failure-cereal_R3/`，构建树保留。

R101 cereal 按人工裁决登记为需要改、**未推送**。最新判定计数 12/55/6/0，已推仍 11，不能混写为 12 已推。BASE_LEDGER 后续说明见 `CEREAL_CORRECTION_1194606_R3.md`。`QUICKBUILD_BRANCHES_R5.tsv` 暂保留原 12 行，全部远端核对 MATCH，不纳入失败/未验收的本地 SHA；它不是三项修复完成后的开工清单。

下一步：人工裁决 cereal performance 新编译错误的修法，以及是否先继续另两包验证 → 通过对应验收后才推 sandbox、更新完整 R5 → 人工决定第二轮 QuickBuild。本轮没有上板、没有启动 QuickBuild、没有修改 codes/。

## QuickBuild #1194606 修复任务（R1/R2 基线预检历史）

**最新：BLOCKED_PATCH_NOT_APPLICABLE，按要求停止。** 人工允许从cereal当前tizen `896a21f0cbcb34835bba8d39f2a4254d691ea98f`起建后，已在tmp完整克隆并建立指定本地sandbox分支。但该提交说明为Initial empty repository、ls-tree为空；与28aedc95比较显示211文件/52728行删除（仅树比较，非本轮操作），CMakeLists.txt与packaging/cereal.spec均不存在。`git apply --check`退出1：`error: packaging/cereal.spec: No such file or directory`。完整diff和原始证据在 `docs/progress/QB_1194606_FIX/FINAL_RESULT_R2.md`、raw/019–028。没有退回旧SHA、导入旧源码、实施补丁或构建，未推包仓。R5及已推送台账未改。

人工已裁决的cereal/ICU/qemu修法保持；下一步须明确含真实源码的cereal基线或空仓如何引入源码，再恢复其余门禁、验证与sandbox推送。icu/qemu实时核查尚未继续。初始及续跑记录均保留。当前不能登记“修复已推、可起第二轮QuickBuild”。

## QuickBuild #1194606（2026-10-13，只读分析历史）

**全量构建未通过，只读分析完成，未实施修复。** 258个构建包×3架构：752 succeeded、9 failed、13 excluded，无unresolvable/blocked。cereal三格为构建期performance消费libstdc++风格Boost ABI、提供方已为libc++；ICU三格为tzdata的version经-I.遮蔽libc++标准头；qemu-accel三个变体的x86_64格为两条/emul/usr/lib64/libc++*.so.1未列RPM清单。九格均为EXPECTED_FAILURES基线外新失败，不套用tensorflow2 GCC例外。

12个指定构建包实际checkout/export SHA均匹配QUICKBUILD_BRANCHES_R4.tsv（实际branch名未观测）；LLVM/runtime三格succeeded，bpftrace两适用架构succeeded。**发布manifest的ICU为e4a4d741，实际执行dcbb3fb4，元数据不一致挂账。** 日志PROJECT_CONFIG含三行libc++ Support/Keep，build_ldflags无fuse-ld=lld；不以内容一致推定Gerrit356690已合入/被使用。

证据及候选diff：docs/progress/QB_1194606/FINAL_RESULT.md、BUILD_STATUS.md。原始17份日志合计13,267,844字节留tmp/QB_1194606/downloads/logs/，URL/SHA在DOWNLOADS.tsv。人工登录profile保留tmp/QB_1194606/browser-profile/，不入库、不导出cookie。成功构建不冒充本次RPM的安装/板测/全部ELF门禁。

挂账：ICU manifest身份；cereal“头文件交付物无需运行时迁移”不覆盖构建期Boost基准，本次不改旧11包判定；qemu清单漏项确定，具体ldd路径过滤机制未观测；候选修法均未经构建。下一步：**人工审阅三类候选修法与cereal构建策略 → 授权后定点验证/修复 → 人工决定新QuickBuild并对账。** 下文“人工起首次QuickBuild”等为历史阶段记录。

## Base project_config 评审上传（2026-10-12）

**当前 R3：已压为单笔评审 356690 patchset 3，356689 已作废。** [356690](https://review.tizen.org/gerrit/c/scm/meta/qb/+/356690/3) 新 SHA `e13ce91df74fe79237a3b6afe2e1443b8a8e8d00`，保留 Change-Id I0403eb61…，作者/提交人/Signed-off-by 均 Hao Lin。新 tree 与 R2 最终 tree `04f46636709cd42b7967e3c7bd40cb6098d61cfa` 相同，直接以原 master 为父节点；配置内容未改变。Gerrit 返回 updated: 1，远端 /3 引用一致，356689 查询确认为 ABANDONED；master 仍为 0bb6ff…。证据 `docs/progress/PRJCONF_REVIEW_1012/FINAL_RESULT_R3.md`。以下 R1/R2 段落仅为历史，不再表示当前需合入两项。

**UPLOADED_FOR_REVIEW：两个叠加评审已上传，未合入。** 人工确认 SSH `ssh://lhmax2025@review.tizen.org:29418/scm/meta/qb` 后，SSH 与只读镜像 master 均为 `0bb6ff66506574606cf4d636aaa8974ff4c779b8`。在 tmp 新克隆基于此 HEAD 完整撤销 bae4ebdd 的唯一 `-fuse-ld=lld`，再加 R84 三行 libc++ Support/Keep。评审：[356689 撤 lld](https://review.tizen.org/gerrit/c/scm/meta/qb/+/356689)、[356690 加 libc++](https://review.tizen.org/gerrit/c/scm/meta/qb/+/356690)；提交分别 db671bec…、0e9fb65c…，作者/提交人/Signed-off-by 均 Hao Lin <hao.lin@samsung.com>。推前 fetch 未变，唯一 qb push 为 refs/for/master，退出 0；推后 master 仍未变。只改 Base-Toolchain 一份配置，lldb 保留。完整 diff、身份与回执见 `docs/progress/PRJCONF_REVIEW_1012/FINAL_RESULT.md`；初次地址阻断已解除，原记录保留于 INITIAL_BLOCKER.md。

**R2：上述两笔现均已更新到 patchset 2。** 356689 新 SHA `2c5b5af89ce1a001f7acea9d8355fe5bc1e91137`，只修正正文为实际撤销 `-fuse-ld=lld`；356690 新 SHA `f385421f72c3daf28ddb60b5b04d051cbd2fdbad`，仅随父提交改变，完整正文不变。两笔 tree 与 patchset 1 分别完全相同，作者/提交人/签字仍均 Hao Lin，Change-Id 不变。Gerrit 返回 updated: 2，两条远端 /2 引用已核对；未新建 change，master 仍为 0bb6ff…。证据 `docs/progress/PRJCONF_REVIEW_1012/FINAL_RESULT_R2.md`。

下一步：人工审阅唯一保留的 **356690 patchset 3**，其中同时撤 lld 并加 libc++；不再合入已作废的 356689。**libcxx-runtimes 已构建并发布后，libc++ Support 才能生效**，否则新增 Support 包不可解析。lld 验收后重提；Unified 全局 libc++ 等 Base 出包后另提。R3 未改任何配置内容，codes/ 只读，未构建/上板/推 sandbox 或包仓分支/起 QuickBuild。实际撤销仍只是一处 fuse-ld 标志。

## Unified 默认 libc++ 试编（2026-09-27）

**PARTIAL：三架构均因本地 Base libc++ RPM 输入缺失而未启动试编。** 本轮固定运行线边表提交 `72c0ad91858db03f84a653d029767efebde9dfa7`，Unified 快照 `20260917.132101`（Base `20260914.073422`）。23 条边中 12 个 Unified 提供方已列源码 VCS；直接 C++ BuildRequires 另有 39 个，发现集合 51 个，超过 30 个的取舍已提问，未自行裁剪或开工。完整证据见 `docs/progress/UNIFIED_NIGHT_0927/FINAL_RESULT.md`。

本地输入缺口：armv7l 缺 boost/icu/jsoncpp/libsigc++/pcre/taglib 的 libc++ RPM；aarch64 另缺 abseil-cpp；x86_64 再缺与 5c169afc 配方一致的 llvm/runtime RPM。已查输出仓与原 scratch 写包目录；许多目录虽含 libcxx 字样，实际保留的是 GCC 轮 RPM。历史通过/配方已推送不代表 RPM 仍齐备，旧解包 payload 也不当 RPM 用。完整路径、SHA、包头及搜索范围见 RPM_INVENTORY.tsv、BASE_INPUT_SUMMARY.tsv。ARM/aarch64 新 LLVM/runtime、部分其他包仍保留，未混用 GCC 输入。

R84 两份 project_config 补丁已在 tmp 副本成功应用，原件 SHA 未变；本轮没有改任何 spec/codes，没有构建/上板/包仓推送/QuickBuild。按任务“缺输入架构今晚不跑”，全部 36 个提供方×架构格记 NOT_OBSERVED，非编译失败；纯 C ELF libc++abi 副作用数量同为 NOT_OBSERVED。下一步：人工补齐已验证 libc++ RPM（若要补建 Base，另行授权）并确定不超过 30 个的范围 → 在授权时间窗口内续跑。原截止为 2026-09-28 08:30 +08:00，过期不自行延长。Base QuickBuild 仍由人工发起，下述已完成的配方审计结论不变。

## 线的定位与分界

本线负责 Tizen-Base-Toolchain 的 libc++ 编译适配、输入资产与 RPM 核验；运行时边界决策由运行时线负责。本轮LLVM_W4_0923 R4登记人工推送、依裁决关闭配方审计并交付QuickBuild分支清单。codes、Source1002不动；不构建、不上板、不推sandbox/包仓、不起QuickBuild；只推项目材料。

## 当前位置

Base 对账：73 个含 C++ 源码包中，11 个已适配推送、56 个有依据无需适配、6 个按设计排除、0 个未评估；适配推送不代表新版 static 资产已获发布批准。对账材料原样发布于 `c7aa2c5e72f78516e40b7a5423a4b83673608bad`。

此前六格18份RPM与只重链接ELF材料保留。`BPF_STATIC_0922` 新配方已完成两架构6份RPM及static ELF门禁。本轮 `BPF_W3R_0922` 已完成ARM物理板可写区对照与取消探针；无需重新写包。正常RPM安装仍未验证，按本轮授权留待镜像阶段。

## 本任务结论与证据

**2026-09-23 LLVM W4 R4：关闭。配方审计关闭，QuickBuild配方与分支前置已满足。** 人工已推`5c169afc97945bfb88786d9f38477f6973fe5285`（父`da14498e07fa4c841b5a6bc7f88766d993982a16`，再上级f203923a）；两笔作者、提交人、Signed-off-by均Hao Lin <hao.lin@samsung.com>，与本地db5b49af候选完整树一致。26项历史差异按本轮人工裁决逐项关闭，27项测试/Unified实验排除当前Base-Toolchain范围；旧R2/R3记录不改。**QuickBuild所需的全部本地配方改动均已进入sandbox。**

分支清单`docs/progress/LLVM_W4_0923/QUICKBUILD_BRANCHES_R4.tsv`共12个构建包/11仓，逐行远端MATCH。固定20260828.101647快照SOURCERPM证明libcxx-runtimes是独立源码构建单元，因此与llvm分别列行、同指5c169afc与libcxx-ehabi-backport，须选择同仓各自spec。实时OBS采用_link/_multibuild/_service哪种机制仍NOT_OBSERVED；当前reference为20260914.073422，公开metadata未列runtime及四类二进制，原因未观测。历史证据不冒充当前服务端配置；人工起QuickBuild应覆盖两份spec。本轮不执行任何包仓push或QuickBuild。

以下R1–R3为保留的历史过程，原停报/未推送表述不再作为当前结论；最终详见`FINAL_RESULT_R4.md`与`LOCAL_SPECS_R4.md`。

**2026-09-23 LLVM W4 R3：两笔本地提交与ARM prep完成，本地spec历史差异门禁停报。** 人工允许纯空行忽略后，runtime四组严格比较全PASS，三种非默认组合全部静态OFF且无.a项。第二笔`db5b49afa7836db0414140208b2d3ab4f4aa7293`已追加在`617a210064c4559fe1152728f7c9cf5e1ab99a9f`之后；干净、ahead2/behind0、无signoff、包仓未推；runtime与指定c5358237…候选逐字节一致。ARM `%prep`退出0，未build。

按新裁决停止全根逐包溯源，R2表及NOT_OBSERVED原样保留。改扫实际命令记录的本地spec：112个路径，38与sandbox一致、15与原仓一致、32差异（其中6个路径对应获准两项），27个缺唯一原仓参照。额外26条含旧配方与GCC验证开关，已列完整diff并按指令停报，不能自动说都是当前QuickBuild漏推，也不能自行豁免。详见`docs/progress/LLVM_W4_0923/FINAL_RESULT_R3.md`及`spec-audit_R3/`。目标“每处所需本地改动都已在sandbox”尚不能宣告成立。

**2026-09-23 LLVM W4 R2：PARTIAL。** 人工撤销旧GCC override门禁、授权同仓runtime静态配方。LLVM新四组展开全PASS；第一笔仅改正文为`617a210064c4559fe1152728f7c9cf5e1ab99a9f`，tree不变。runtime默认组除获准四个开关和两条归档记录外多12个开头空行，严格比较未通过；已询问是否可单列非功能差异，未自行豁免。第二笔与runtime prep未做，当前干净、ahead1/behind0、包仓未推。下述R1门禁记录为历史，不再将旧GCC override条件作为阻断。

BPF实际两输入根每架构19份追加RPM已重算SHA并核对包头：各6 A / 1 B / 12获准C（待推送后转B）。全根为133/134项，LLVM为126/127项；仍有未归类历史输入，不能将19份当全量、不能宣称无C类。证据：`docs/progress/LLVM_W4_0923/FINAL_RESULT_R2.md`、`inputs_R2/ALL_INPUTS_R2.tsv`。

**2026-09-23 LLVM配方W4：PARTIAL，当前不得推包仓。** ARM22份与aarch6412份完整RPM的llvm.spec同SHA `cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68`；相对f203923a只有条件内 `-DLIBCLANG_BUILD_STATIC=ON`，没有VCS/Release差异。已在原可写克隆目标sandbox分支创建本地提交 `2cba97e503d506cc184c34b6a4301087b6fa4bb7`，仅一行、工作树干净、ahead1/behind0、未推。libclang.a由两架构llvm-static-devel提供，bpftrace直接BR对应；ARM独立%prep退出0。

**新门禁问题：指定 `_toolchain_override gcc` 宏展开仍选Clang，新增static开关出现1次。** 原spec:12强制override=clang，平台_toolchain宏动态引用它；远端基线同条件也有LLVM_ENABLE_LIBCXX，只是没有静态开关。本轮严格不换为字面_toolchain gcc、不修旧逻辑，故该格FAIL判据（命令本身退出0）；Clang与未定义两格符合。提交正文的GCC保持原状不能外推到此override组合，签字前须人工裁决，不能称可推候选。

11包主spec/patch比对：9个只有GBS的VCS字段差异，bpftrace一致，LLVM有已授权但未推的一行。patch集9份均匹配；完整表、原文与哈希见 `docs/progress/LLVM_W4_0923/RECIPE_AUDIT.tsv`。另发现本地验证输入的 `libcxx-runtimes.spec` 开启静态库及安装文件，而同一LLVM sandbox仍关闭libc++.a/libc++abi.a；这是主llvm.spec之外的供给缺口，未自行纳入提交。QuickBuild是否另有提供这两份归档的runtime来源NOT_OBSERVED，不能只补libclang.a便宣布输入齐备。详见 `FINAL_RESULT.md`、`UNPUSHED_LIBCXX_RUNTIME.diff`、`W4_SIGNOFF.md`。

**W3R 事故副本已按本轮批准恢复**：4/4 原 RPM SHA 与原清单一致，重新独立解包得到的主包 ELF SHA 与 INCIDENT 改前值一致，复制回原路径后再次一致；四份事故文件保留于原路径 `*.objcopy-modified-0922`。修正脚本明确新输出 ELF 并核对输入不变，一次性副本验证通过。证据 `docs/progress/BPF_W4_0922/RESTORE_RESULT.md`。旧 INCIDENT 和原始事故记录保留，不抹去违规事实；不得再运行 W3R 的旧提取脚本。

相对**前轮 tmp static 候选 spec**，BPF_STATIC 新 spec 仅新增 Patch0 及 prep 无条件应用；这不是相对包仓 f895f8c 的全部差异。Patch 只在 STATIC_LINKING 且 LIBCLANG_STATIC_PATH 分支以 `${LIBBPF_LIBRARIES}` 替代裸 bpf。两架构 RPM 内 static 均精确依赖 libm/libgcc_s/libc/架构 loader，不依赖动态 libbpf/libc++/LLVM；命名空间及 libbpf 已定义符号交集通过。ARM 新 RPM ELF 与上轮重链接 ELF SHA 不同，aarch64 相同，只登记不归因。完整证据：`docs/progress/BPF_STATIC_0922/FINAL_RESULT.md`。

**W4 关闭：人工已推送，远端登记完成。** bpftrace 远端 `7831fb34c6881b5cff0b8bdd0dac27a1829290fa` 与本地已验证候选 `72fda9941031fc35d8825e73446ca43153c4b69b` 的 tree 均为 `0cd67fd05efc0a16d7e2463695f1f2b040bbf2fe`，内容零差异。Author 为 Hao Lin <hao.lin@samsung.com>；Committer、Signed-off-by 为 He Fangyu <fangyu.he@samsung.com>，签字人与作者不同，仅登记不修改。历史 ARM prep 退出0、setup/Patch0/Source1002/test与sed通过的证据仍沿用。见 `docs/progress/QB_PRECHECK_0922/FINAL_RESULT.md`。本轮仅fetch/查询，没有包仓推送；W4关闭不等于安装或QuickBuild已通过。

**QuickBuild 无需 payload 对策；OBS 当前实际宏值仍为 NOT_OBSERVED。** `ARM_RPM_DIAG/upstream-rpmio.c:765–777`：T后无数字走else（threads=-1），分支内指针不前移，不越界；只有T<数字>进入if，while停在终止符，再由外层for的mode++越过终止符。本地Base-Toolchain project_config:61与W1两个保留根的 `w5T.xzdio` 因而不触发。09-17 armv7l LLVM失败是为限制xz线程而本地CLI覆盖 `w5T1.xzdio` 引入（BUILD_STATIC_0917B/resume_0917/STATUS.md:67–70），与libc++无关，不在任何spec或sandbox提交中。后续本地构建继续用单线程 `w5.xzdio`，禁止T<数字>形式。补查固定Base-Toolchain快照20260828.101647的armv7l libllvm/libicu/boost-devel，三份RPM SHA匹配原下载清单，包头均xz/5T；不把历史包头冒充当前服务端宏。11个已适配包远端SHA均MATCH；4个静态BR源码均在R101范围内（bcc-tools已适配、libbpf/xz/zlib无需改），OBS当前静态输入可用性未观测。详见 `docs/progress/QB_PRECHECK_0922/PAYLOAD.md`、`RPM_PAYLOAD_HEADERS.json`、`STATIC_DEPENDENCIES.tsv`。

条件组合：Base-Toolchain 项目宏默认 `_toolchain=clang`、支持 override；`build_with_libcxx` 不是项目/GBS 全局宏，而是本包 spec:1–5 在 Clang 路径派生为 1。W1 两架构各覆盖 clang/1、gcc/未定义、未定义/未定义；构建阶段 clang/未定义、gcc/1 两种组合均 NOT_OBSERVED。已核配置默认路径两条件同步，但实际 QuickBuild 服务端最终宏集未取得，外部强制宏不能由正常六格外推通过。见 `CONDITION_COMBINATIONS.md`。

前轮正常main/common预检退出2、static预检退出1的原始记录保留。本轮确认：/var/tmp实际位于/opt的rw ext4分区，/usr与/etc位于ro根分区；固定RPM源码transaction.c:164在ST_RDONLY时按0可用块计算，再由269–272产生DISKSPACE、rpmprob.c:143–151显示92MB，故与df物理空闲并不矛盾。固定源码为上游4.14.1提交，不冒充部署4.14.1.1的源码身份证明。

新RPM直接解包到可写区，ELF SHA匹配；版本/info/list和三种脚本各3次，共12对调用均退出0；info只差LLVM21.1.1→22.1.8。共享libgcc取消20/20通过，静态libgcc对照也20/20通过，不外推此前x86_64崩溃结论。完整证据：`docs/progress/BPF_W3R_0922/FINAL_RESULT.md`、`BOARD_RESULT.md`。

构建时长核实：ARM119.17min→10.02min；aarch64实际13.37min→10.00min，并非两小时→十分钟。指定ccache位置/launcher未观测，当前--version trace确认/emul原生Clang分派；缺历史逐编译进程及同期连续负载，提速唯一原因仍NOT_OBSERVED。ARM主包ELF/.text均不同、aarch64均相同，差异留人工审阅，未自行归因。见`BUILD_TIMING.md`。

W4 追加只读核查：两轮启动脚本均未显式用 QEMU 包裹 make/cmake；当前两轮根的 QEMU SHA 相同且均有 /emul 原生工具。当前宿主同时存在 arm（qemu-arm-binfmt，flags P）与 qemu-arm（qemu-arm-static，空 flags）注册，不据当前注册推断历史分派。原 GBS 保留根具备平台 accel 组件，但缺正式 OBS 执行记录；历史 W1 编译器分派差异及本轮与正式构建同形性仍 NOT_OBSERVED。见 `docs/progress/BPF_W4_0922/BUILD_DISPATCH.md`。

## 挂账

- **QuickBuild配方与分支前置（R4）已满足：LLVM两笔已由人工签字推送并核实，历史配方审计依裁决关闭。** 清单见QUICKBUILD_BRANCHES_R4.tsv；不再继续R2全根溯源。实时OBS接入形式未观测，runtime须作为独立构建单元覆盖；当前reference缺runtime的观测保留，不推测原因。

- 四份主包解包副本：**已恢复，事故文件保留于 *.objcopy-modified-0922**，本项恢复挂账关闭；事故历史保留。
- W4 人工推送与远端登记已关闭；条件组合两种未覆盖形态、实际QuickBuild最终宏集仍未观测，不能由本地六格外推。payload按源码/包头与本次人工裁决无需对策，不再挂为启动阻断。

- 正常static安装预检的ro挂载条件已与指定RPM源码对上；**rpm安装验证留待镜像阶段**，包括%post覆盖逻辑；可写区通过不冒充安装通过。主包依赖缺口仍见前轮原始记录。
- 新RPM及人工已推配方包含修正后的static，不再挂“未写包/未推送”。`-lstdc++fs` 仍挂账：两架构实测解析GCC14.2目录下的静态归档，本轮不改该项。
- ARM 新 22 份 LLVM 输入已正常接入从原始根复制的新私有副本；普通预检与安装退出 0，未使用文件冲突豁免。旧冲突副本与记录保留，见 `docs/progress/BPF_W1_0921/INPUT_STATUS.md`。
- ARM 第五份 `libclang.a` 与旧份 SHA256 不同，登记身份，不归因。
- 本轮物理板测试通过后，先保存全部工作目录文件SHA，再按授权删除`/var/tmp/bpf_static_0922/`，独立验证不存在；本地RPM/探针仍可重新部署。无bpftrace安装、/usr/bin/bpftrace*仍ABSENT、起止挂载一致、root已恢复。
- 仍须审阅ARM主包.text差异及构建提速的历史证据缺口；ARM静态libgcc对照通过与x86_64既有崩溃的适用范围必须分开。GCC/未定义路径本轮不重构。

## 下一步

**人工起 Tizen-Base-Toolchain QuickBuild → 结果与 QB_PRECHECK_0922/EXPECTED_FAILURES.md 对账。** 按QUICKBUILD_BRANCHES_R4.tsv覆盖12个构建包（11仓，llvm/runtime共用同一sandbox）。本轮不代起QuickBuild、不推包仓；payload无需对策，正常RPM安装/%post仍留待镜像阶段。R2全根表及R3审计历史保留，不继续。
