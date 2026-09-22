# 编译适配线状态

## 线的定位与分界

本线负责 Tizen-Base-Toolchain 的 libc++ 编译适配、输入资产与 RPM 核验；运行时边界决策由运行时线负责。本轮W3R仅核查安装预检、既有构建记录，并在ARM板可写区解包测试。原spec、codes、Source1002不动；不构建、不rpm安装、不remount、不推sandbox/包仓，不起QuickBuild。

## 当前位置

Base 对账：73 个含 C++ 源码包中，11 个已适配推送、56 个有依据无需适配、6 个按设计排除、0 个未评估；适配推送不代表新版 static 资产已获发布批准。对账材料原样发布于 `c7aa2c5e72f78516e40b7a5423a4b83673608bad`。

此前六格18份RPM与只重链接ELF材料保留。`BPF_STATIC_0922` 新配方已完成两架构6份RPM及static ELF门禁。本轮 `BPF_W3R_0922` 已完成ARM物理板可写区对照与取消探针；无需重新写包。正常RPM安装仍未验证，按本轮授权留待镜像阶段。

## 本任务结论与证据

**本轮只读违规需先处理**：W3R的llvm-objcopy提取.text时漏指定独立输出ELF，改写了四份既有tmp主包解包副本；未擅自恢复。原RPM校验未变，内存只读解析复核原ELF/.text比较仍成立，板测static不在受影响四份之内。路径与前后SHA见`docs/progress/BPF_W3R_0922/INCIDENT.md`。不把任务标为无违规完成。

新spec仅新增Patch0及prep无条件应用；Patch只在STATIC_LINKING且LIBCLANG_STATIC_PATH分支以`${LIBBPF_LIBRARIES}`替代裸bpf。两架构RPM内static均精确依赖libm/libgcc_s/libc/架构loader，不依赖动态libbpf/libc++/LLVM；命名空间及libbpf已定义符号交集通过。ARM新RPM ELF与上轮重链接ELF SHA不同，aarch64相同，只登记不归因。完整证据：`docs/progress/BPF_STATIC_0922/FINAL_RESULT.md`。

前轮正常main/common预检退出2、static预检退出1的原始记录保留。本轮确认：/var/tmp实际位于/opt的rw ext4分区，/usr与/etc位于ro根分区；固定RPM源码transaction.c:164在ST_RDONLY时按0可用块计算，再由269–272产生DISKSPACE、rpmprob.c:143–151显示92MB，故与df物理空闲并不矛盾。固定源码为上游4.14.1提交，不冒充部署4.14.1.1的源码身份证明。

新RPM直接解包到可写区，ELF SHA匹配；版本/info/list和三种脚本各3次，共12对调用均退出0；info只差LLVM21.1.1→22.1.8。共享libgcc取消20/20通过，静态libgcc对照也20/20通过，不外推此前x86_64崩溃结论。完整证据：`docs/progress/BPF_W3R_0922/FINAL_RESULT.md`、`BOARD_RESULT.md`。

构建时长核实：ARM119.17min→10.02min；aarch64实际13.37min→10.00min，并非两小时→十分钟。指定ccache位置/launcher未观测，当前--version trace确认/emul原生Clang分派；缺历史逐编译进程及同期连续负载，提速唯一原因仍NOT_OBSERVED。ARM主包ELF/.text均不同、aarch64均相同，差异留人工审阅，未自行归因。见`BUILD_TIMING.md`。

## 挂账

- 四份主包解包副本已被objcopy改写，不应继续作为原始输入；需人工批准从原RPM恢复。本轮只保存事实，不恢复、不重跑。

- 正常static安装预检的ro挂载条件已与指定RPM源码对上；**rpm安装验证留待镜像阶段**，包括%post覆盖逻辑；可写区通过不冒充安装通过。主包依赖缺口仍见前轮原始记录。
- 新RPM已包含修正后的static，不再挂“未写包”。W4仍需审阅 `SPEC_DIFF.patch`、Patch和 `-lstdc++fs`：两架构实测解析GCC14.2目录下的静态归档，本轮不改该项。
- ARM 新 22 份 LLVM 输入已正常接入从原始根复制的新私有副本；普通预检与安装退出 0，未使用文件冲突豁免。旧冲突副本与记录保留，见 `docs/progress/BPF_W1_0921/INPUT_STATUS.md`。
- ARM 第五份 `libclang.a` 与旧份 SHA256 不同，登记身份，不归因。
- 本轮物理板测试通过后，先保存全部工作目录文件SHA，再按授权删除`/var/tmp/bpf_static_0922/`，独立验证不存在；本地RPM/探针仍可重新部署。无bpftrace安装、/usr/bin/bpftrace*仍ABSENT、起止挂载一致、root已恢复。
- 仍须审阅ARM主包.text差异及构建提速的历史证据缺口；ARM静态libgcc对照通过与x86_64既有崩溃的适用范围必须分开。GCC/未定义路径本轮不重构。

## 下一步

停止交人工审阅。W4审阅/签字配方、-lstdc++fs与新挂账后，按人工授权进入QuickBuild及镜像阶段RPM安装验证。最新交接为 `docs/progress/BPF_W3R_0922/W4_INPUTS.md`；RPM与配方仍取 `BPF_STATIC_0922` 原六份，不用旧重链接ELF替代。本轮不推包仓、不起QuickBuild。
