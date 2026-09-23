# 编译适配线状态

## 线的定位与分界

本线负责 Tizen-Base-Toolchain 的 libc++ 编译适配、输入资产与 RPM 核验；运行时边界决策由运行时线负责。本轮LLVM_W4_0923补本地libclang.a配方提交并核对11包已验证配方与sandbox。codes、Source1002不动；不构建（仅%prep）、不上板、不推sandbox/包仓、不起QuickBuild；只推项目材料。

## 当前位置

Base 对账：73 个含 C++ 源码包中，11 个已适配推送、56 个有依据无需适配、6 个按设计排除、0 个未评估；适配推送不代表新版 static 资产已获发布批准。对账材料原样发布于 `c7aa2c5e72f78516e40b7a5423a4b83673608bad`。

此前六格18份RPM与只重链接ELF材料保留。`BPF_STATIC_0922` 新配方已完成两架构6份RPM及static ELF门禁。本轮 `BPF_W3R_0922` 已完成ARM物理板可写区对照与取消探针；无需重新写包。正常RPM安装仍未验证，按本轮授权留待镜像阶段。

## 本任务结论与证据

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

- **QuickBuild前置（R3修订）：人工裁决本地spec清单中的额外历史/测试配方差异并闭合所需配方清单 → 签字推LLVM两笔并核远端SHA → 人工批准QuickBuild。** 两笔已备好，包仓未推；不再要求继续R2全根逐包溯源，旧表仅保留。

- 四份主包解包副本：**已恢复，事故文件保留于 *.objcopy-modified-0922**，本项恢复挂账关闭；事故历史保留。
- W4 人工推送与远端登记已关闭；条件组合两种未覆盖形态、实际QuickBuild最终宏集仍未观测，不能由本地六格外推。payload按源码/包头与本次人工裁决无需对策，不再挂为启动阻断。

- 正常static安装预检的ro挂载条件已与指定RPM源码对上；**rpm安装验证留待镜像阶段**，包括%post覆盖逻辑；可写区通过不冒充安装通过。主包依赖缺口仍见前轮原始记录。
- 新RPM及人工已推配方包含修正后的static，不再挂“未写包/未推送”。`-lstdc++fs` 仍挂账：两架构实测解析GCC14.2目录下的静态归档，本轮不改该项。
- ARM 新 22 份 LLVM 输入已正常接入从原始根复制的新私有副本；普通预检与安装退出 0，未使用文件冲突豁免。旧冲突副本与记录保留，见 `docs/progress/BPF_W1_0921/INPUT_STATUS.md`。
- ARM 第五份 `libclang.a` 与旧份 SHA256 不同，登记身份，不归因。
- 本轮物理板测试通过后，先保存全部工作目录文件SHA，再按授权删除`/var/tmp/bpf_static_0922/`，独立验证不存在；本地RPM/探针仍可重新部署。无bpftrace安装、/usr/bin/bpftrace*仍ABSENT、起止挂载一致、root已恢复。
- 仍须审阅ARM主包.text差异及构建提速的历史证据缺口；ARM静态libgcc对照通过与x86_64既有崩溃的适用范围必须分开。GCC/未定义路径本轮不重构。

## 下一步

停止交人工审阅额外历史spec差异和未判定参照范围。**闭合本地所需配方清单 → 人工按W4_SIGNOFF签字推LLVM两笔、核远端SHA → 人工起Tizen-Base-Toolchain QuickBuild → 与预期失败清单对账。** 本轮不推包仓、不自动排除历史测试项；payload无需对策，正常RPM安装/%post仍留待镜像阶段。R2全根表保留，不继续。
