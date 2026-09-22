# 编译适配线状态

## 线的定位与分界

本线负责 Tizen-Base-Toolchain 的 libc++ 编译适配、输入资产与 RPM 核验；运行时边界决策由运行时线负责。本轮 W4 仅按批准恢复四份误改 tmp 副本、修正审计脚本、只读核查构建提速线索及包仓基线。因配方范围冲突，未修改包仓。原 spec、codes、Source1002 不动；不构建、不上板、不推 sandbox/包仓、不起 QuickBuild；只交付项目材料。

## 当前位置

Base 对账：73 个含 C++ 源码包中，11 个已适配推送、56 个有依据无需适配、6 个按设计排除、0 个未评估；适配推送不代表新版 static 资产已获发布批准。对账材料原样发布于 `c7aa2c5e72f78516e40b7a5423a4b83673608bad`。

此前六格18份RPM与只重链接ELF材料保留。`BPF_STATIC_0922` 新配方已完成两架构6份RPM及static ELF门禁。本轮 `BPF_W3R_0922` 已完成ARM物理板可写区对照与取消探针；无需重新写包。正常RPM安装仍未验证，按本轮授权留待镜像阶段。

## 本任务结论与证据

**W3R 事故副本已按本轮批准恢复**：4/4 原 RPM SHA 与原清单一致，重新独立解包得到的主包 ELF SHA 与 INCIDENT 改前值一致，复制回原路径后再次一致；四份事故文件保留于原路径 `*.objcopy-modified-0922`。修正脚本明确新输出 ELF 并核对输入不变，一次性副本验证通过。证据 `docs/progress/BPF_W4_0922/RESTORE_RESULT.md`。旧 INCIDENT 和原始事故记录保留，不抹去违规事实；不得再运行 W3R 的旧提取脚本。

相对**前轮 tmp static 候选 spec**，BPF_STATIC 新 spec 仅新增 Patch0 及 prep 无条件应用；这不是相对包仓 f895f8c 的全部差异。Patch 只在 STATIC_LINKING 且 LIBCLANG_STATIC_PATH 分支以 `${LIBBPF_LIBRARIES}` 替代裸 bpf。两架构 RPM 内 static 均精确依赖 libm/libgcc_s/libc/架构 loader，不依赖动态 libbpf/libc++/LLVM；命名空间及 libbpf 已定义符号交集通过。ARM 新 RPM ELF 与上轮重链接 ELF SHA 不同，aarch64 相同，只登记不归因。完整证据：`docs/progress/BPF_STATIC_0922/FINAL_RESULT.md`。

**W4 包提交阻断：待人工确认范围。** 远端基线仍为 `f895f8c0373d224847fc7d3ecbeaac3bf926a1a1`，但缺完整已验证 recipe 的 static BuildRequires、共享 libgcc 切换、独立 static 构建/安装段等。不能同时满足“仅两行及 patch、不改其他行”和“完整等于已验证配方”。未创建包提交，工作树干净、相对 FETCH_HEAD 为 0/0；不能标“提交已备好待签字推送”。完整差异和待裁决项见 `docs/progress/BPF_W4_0922/DECISIONS.md`。

前轮正常main/common预检退出2、static预检退出1的原始记录保留。本轮确认：/var/tmp实际位于/opt的rw ext4分区，/usr与/etc位于ro根分区；固定RPM源码transaction.c:164在ST_RDONLY时按0可用块计算，再由269–272产生DISKSPACE、rpmprob.c:143–151显示92MB，故与df物理空闲并不矛盾。固定源码为上游4.14.1提交，不冒充部署4.14.1.1的源码身份证明。

新RPM直接解包到可写区，ELF SHA匹配；版本/info/list和三种脚本各3次，共12对调用均退出0；info只差LLVM21.1.1→22.1.8。共享libgcc取消20/20通过，静态libgcc对照也20/20通过，不外推此前x86_64崩溃结论。完整证据：`docs/progress/BPF_W3R_0922/FINAL_RESULT.md`、`BOARD_RESULT.md`。

构建时长核实：ARM119.17min→10.02min；aarch64实际13.37min→10.00min，并非两小时→十分钟。指定ccache位置/launcher未观测，当前--version trace确认/emul原生Clang分派；缺历史逐编译进程及同期连续负载，提速唯一原因仍NOT_OBSERVED。ARM主包ELF/.text均不同、aarch64均相同，差异留人工审阅，未自行归因。见`BUILD_TIMING.md`。

W4 追加只读核查：两轮启动脚本均未显式用 QEMU 包裹 make/cmake；当前两轮根的 QEMU SHA 相同且均有 /emul 原生工具。当前宿主同时存在 arm（qemu-arm-binfmt，flags P）与 qemu-arm（qemu-arm-static，空 flags）注册，不据当前注册推断历史分派。原 GBS 保留根具备平台 accel 组件，但缺正式 OBS 执行记录；历史 W1 编译器分派差异及本轮与正式构建同形性仍 NOT_OBSERVED。见 `docs/progress/BPF_W4_0922/BUILD_DISPATCH.md`。

## 挂账

- 四份主包解包副本：**已恢复，事故文件保留于 *.objcopy-modified-0922**，本项恢复挂账关闭；事故历史保留。
- W4 完整 recipe 与基线差异超出“只加两行”的限制，待人工批准范围；包提交及 prep 核验尚未做。

- 正常static安装预检的ro挂载条件已与指定RPM源码对上；**rpm安装验证留待镜像阶段**，包括%post覆盖逻辑；可写区通过不冒充安装通过。主包依赖缺口仍见前轮原始记录。
- 新RPM已包含修正后的static，不再挂“未写包”。W4仍需审阅 `SPEC_DIFF.patch`、Patch和 `-lstdc++fs`：两架构实测解析GCC14.2目录下的静态归档，本轮不改该项。
- ARM 新 22 份 LLVM 输入已正常接入从原始根复制的新私有副本；普通预检与安装退出 0，未使用文件冲突豁免。旧冲突副本与记录保留，见 `docs/progress/BPF_W1_0921/INPUT_STATUS.md`。
- ARM 第五份 `libclang.a` 与旧份 SHA256 不同，登记身份，不归因。
- 本轮物理板测试通过后，先保存全部工作目录文件SHA，再按授权删除`/var/tmp/bpf_static_0922/`，独立验证不存在；本地RPM/探针仍可重新部署。无bpftrace安装、/usr/bin/bpftrace*仍ABSENT、起止挂载一致、root已恢复。
- 仍须审阅ARM主包.text差异及构建提速的历史证据缺口；ARM静态libgcc对照通过与x86_64既有崩溃的适用范围必须分开。GCC/未定义路径本轮不重构。

## 下一步

停止交人工审阅。先裁决 W4 包提交范围，再准备实际本地提交与 prep 核验；人工审阅 spec 完整差异、patch、提交信息、-lstdc++fs 与 Source1002 回退后，另行授权推送包仓及 QuickBuild；RPM 安装验证留待镜像阶段。最新审阅单为 `docs/progress/BPF_W4_0922/W4_SIGNOFF.md`。RPM 与配方仍取 `BPF_STATIC_0922` 原六份，不用旧重链接 ELF 替代。本轮未推包仓、不起 QuickBuild。
