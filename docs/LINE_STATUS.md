# 编译适配线状态

## 线的定位与分界

本线负责 Tizen-Base-Toolchain 的 libc++ 编译适配、输入资产与 RPM 核验；运行时边界决策由运行时线负责。本轮 W4 续已获完整配方授权，在 tmp 包克隆创建一个本地提交并完成 ARM %prep 检查、只读条件组合核查。codes、Source1002 不动；不运行 %build、不上板、不推 sandbox/包仓、不起 QuickBuild；只推项目材料。

## 当前位置

Base 对账：73 个含 C++ 源码包中，11 个已适配推送、56 个有依据无需适配、6 个按设计排除、0 个未评估；适配推送不代表新版 static 资产已获发布批准。对账材料原样发布于 `c7aa2c5e72f78516e40b7a5423a4b83673608bad`。

此前六格18份RPM与只重链接ELF材料保留。`BPF_STATIC_0922` 新配方已完成两架构6份RPM及static ELF门禁。本轮 `BPF_W3R_0922` 已完成ARM物理板可写区对照与取消探针；无需重新写包。正常RPM安装仍未验证，按本轮授权留待镜像阶段。

## 本任务结论与证据

**W3R 事故副本已按本轮批准恢复**：4/4 原 RPM SHA 与原清单一致，重新独立解包得到的主包 ELF SHA 与 INCIDENT 改前值一致，复制回原路径后再次一致；四份事故文件保留于原路径 `*.objcopy-modified-0922`。修正脚本明确新输出 ELF 并核对输入不变，一次性副本验证通过。证据 `docs/progress/BPF_W4_0922/RESTORE_RESULT.md`。旧 INCIDENT 和原始事故记录保留，不抹去违规事实；不得再运行 W3R 的旧提取脚本。

相对**前轮 tmp static 候选 spec**，BPF_STATIC 新 spec 仅新增 Patch0 及 prep 无条件应用；这不是相对包仓 f895f8c 的全部差异。Patch 只在 STATIC_LINKING 且 LIBCLANG_STATIC_PATH 分支以 `${LIBBPF_LIBRARIES}` 替代裸 bpf。两架构 RPM 内 static 均精确依赖 libm/libgcc_s/libc/架构 loader，不依赖动态 libbpf/libc++/LLVM；命名空间及 libbpf 已定义符号交集通过。ARM 新 RPM ELF 与上轮重链接 ELF SHA 不同，aarch64 相同，只登记不归因。完整证据：`docs/progress/BPF_STATIC_0922/FINAL_RESULT.md`。

**W4：本地提交已备好，待人工签字后推送。** 人工已取消“两行”限制，允许完整已验证 recipe。再次核远端基线 `f895f8c0373d224847fc7d3ecbeaac3bf926a1a1` 后，本地创建 `72fda9941031fc35d8825e73446ca43153c4b69b`；仅 spec 和新 patch，两文件与 recipe 逐字节一致，工作树干净、ahead 1 / behind 0；远端仍旧基线，未推包仓。ARM 新独立 prep 树退出 0，setup、Patch0、Source1002 解包、test 1=1 和 sed 均通过，未运行 %build。审阅见 `docs/progress/BPF_W4_0922/W4_SIGNOFF.md`。

条件组合：Base-Toolchain 项目宏默认 `_toolchain=clang`、支持 override；`build_with_libcxx` 不是项目/GBS 全局宏，而是本包 spec:1–5 在 Clang 路径派生为 1。W1 两架构各覆盖 clang/1、gcc/未定义、未定义/未定义；构建阶段 clang/未定义、gcc/1 两种组合均 NOT_OBSERVED。已核配置默认路径两条件同步，但实际 QuickBuild 服务端最终宏集未取得，外部强制宏不能由正常六格外推通过。见 `CONDITION_COMBINATIONS.md`。

前轮正常main/common预检退出2、static预检退出1的原始记录保留。本轮确认：/var/tmp实际位于/opt的rw ext4分区，/usr与/etc位于ro根分区；固定RPM源码transaction.c:164在ST_RDONLY时按0可用块计算，再由269–272产生DISKSPACE、rpmprob.c:143–151显示92MB，故与df物理空闲并不矛盾。固定源码为上游4.14.1提交，不冒充部署4.14.1.1的源码身份证明。

新RPM直接解包到可写区，ELF SHA匹配；版本/info/list和三种脚本各3次，共12对调用均退出0；info只差LLVM21.1.1→22.1.8。共享libgcc取消20/20通过，静态libgcc对照也20/20通过，不外推此前x86_64崩溃结论。完整证据：`docs/progress/BPF_W3R_0922/FINAL_RESULT.md`、`BOARD_RESULT.md`。

构建时长核实：ARM119.17min→10.02min；aarch64实际13.37min→10.00min，并非两小时→十分钟。指定ccache位置/launcher未观测，当前--version trace确认/emul原生Clang分派；缺历史逐编译进程及同期连续负载，提速唯一原因仍NOT_OBSERVED。ARM主包ELF/.text均不同、aarch64均相同，差异留人工审阅，未自行归因。见`BUILD_TIMING.md`。

W4 追加只读核查：两轮启动脚本均未显式用 QEMU 包裹 make/cmake；当前两轮根的 QEMU SHA 相同且均有 /emul 原生工具。当前宿主同时存在 arm（qemu-arm-binfmt，flags P）与 qemu-arm（qemu-arm-static，空 flags）注册，不据当前注册推断历史分派。原 GBS 保留根具备平台 accel 组件，但缺正式 OBS 执行记录；历史 W1 编译器分派差异及本轮与正式构建同形性仍 NOT_OBSERVED。见 `docs/progress/BPF_W4_0922/BUILD_DISPATCH.md`。

## 挂账

- 四份主包解包副本：**已恢复，事故文件保留于 *.objcopy-modified-0922**，本项恢复挂账关闭；事故历史保留。
- W4 完整 recipe 范围已获批准、本地提交与 prep 完成；待人工签字推包仓。条件组合的两种未覆盖形态及实际 QuickBuild 最终宏集仍需签字时审阅。

- 正常static安装预检的ro挂载条件已与指定RPM源码对上；**rpm安装验证留待镜像阶段**，包括%post覆盖逻辑；可写区通过不冒充安装通过。主包依赖缺口仍见前轮原始记录。
- 新RPM已包含修正后的static，不再挂“未写包”。W4仍需审阅 `SPEC_DIFF.patch`、Patch和 `-lstdc++fs`：两架构实测解析GCC14.2目录下的静态归档，本轮不改该项。
- ARM 新 22 份 LLVM 输入已正常接入从原始根复制的新私有副本；普通预检与安装退出 0，未使用文件冲突豁免。旧冲突副本与记录保留，见 `docs/progress/BPF_W1_0921/INPUT_STATUS.md`。
- ARM 第五份 `libclang.a` 与旧份 SHA256 不同，登记身份，不归因。
- 本轮物理板测试通过后，先保存全部工作目录文件SHA，再按授权删除`/var/tmp/bpf_static_0922/`，独立验证不存在；本地RPM/探针仍可重新部署。无bpftrace安装、/usr/bin/bpftrace*仍ABSENT、起止挂载一致、root已恢复。
- 仍须审阅ARM主包.text差异及构建提速的历史证据缺口；ARM静态libgcc对照通过与x86_64既有崩溃的适用范围必须分开。GCC/未定义路径本轮不重构。

## 下一步

停止交人工审阅。**人工签字、人工推送并核对包仓远端 SHA → 人工批准 QuickBuild**；未 amend 时远端预期为 `72fda9941031fc35d8825e73446ca43153c4b69b`。需审完整 diff、format-patch、英文提交信息、条件组合、-lstdc++fs 与 Source1002 回退。RPM 安装/%post 验证留待镜像阶段。最新审阅单为 `docs/progress/BPF_W4_0922/W4_SIGNOFF.md`。RPM 仍取 `BPF_STATIC_0922` 原六份；本轮未重构产物、未推包仓、不起 QuickBuild。
