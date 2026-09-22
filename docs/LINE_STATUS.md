# 编译适配线状态

## 线的定位与分界

本线负责 Tizen-Base-Toolchain 的 libc++ 编译适配、输入资产与 RPM 核验；运行时边界决策由运行时线负责。本轮获授权在 tmp spec 副本中加入静态 libbpf Patch、完整写包并做 ARM 板测。原 spec、codes、Source1002 不动；不推 sandbox/包仓，不起 QuickBuild。

## 当前位置

Base 对账：73 个含 C++ 源码包中，11 个已适配推送、56 个有依据无需适配、6 个按设计排除、0 个未评估；适配推送不代表新版 static 资产已获发布批准。对账材料原样发布于 `c7aa2c5e72f78516e40b7a5423a4b83673608bad`。

此前六格18份RPM与只重链接ELF材料保留，不冒充本轮结果。本轮 `BPF_STATIC_0922` 用加Patch的新配方串行完成 armv7l/aarch64 两个libc++格，各main/common/static三份，共6份新RPM；构建退出0，三份RPM摘要/Requires/清单核验及RPM内static ELF门禁均通过。ARM正常安装预检失败，W3未闭合。

## 本任务结论与证据

新spec仅新增Patch0及prep无条件应用；Patch只在STATIC_LINKING且LIBCLANG_STATIC_PATH分支以`${LIBBPF_LIBRARIES}`替代裸bpf。两架构RPM内static均精确依赖libm/libgcc_s/libc/架构loader，不依赖动态libbpf/libc++/LLVM；命名空间及libbpf已定义符号交集通过。ARM新RPM ELF与上轮重链接ELF SHA不同，aarch64相同，只登记不归因。完整证据：`docs/progress/BPF_STATIC_0922/FINAL_RESULT.md`。

物理板正常main/common预检退出2，缺BCC/libc++/Clang动态库；static预检退出1，报告无法写`/etc/device-sec-policy`、MSM插件失败及根文件系统需92MB。未正式安装，未运行成对功能和取消探针，全部NOT_OBSERVED，不能称为功能等价或候选执行失败。未绕过、未修复、未重试。详见同目录 `BOARD_ANALYSIS.md`。

## 挂账

- 正常static安装预检阻断未归因：df记录根文件系统可用1,393,876 KiB，却同时出现92MB不足提示；不据此自行判断权限、挂载或容量根因。须人工裁决后才能补W3。
- 新RPM已包含修正后的static，不再挂“未写包”。W4仍需审阅 `SPEC_DIFF.patch`、Patch和 `-lstdc++fs`：两架构实测解析GCC14.2目录下的静态归档，本轮不改该项。
- ARM 新 22 份 LLVM 输入已正常接入从原始根复制的新私有副本；普通预检与安装退出 0，未使用文件冲突豁免。旧冲突副本与记录保留，见 `docs/progress/BPF_W1_0921/INPUT_STATUS.md`。
- ARM 第五份 `libclang.a` 与旧份 SHA256 不同，登记身份，不归因。
- 板端未安装bpftrace相关包，五个受影响路径与装前一致，SDB root模式已恢复。失败现场五个上传文件保留在`/var/tmp/bpf_static_0922/`，清理状态为PRESERVED_FAILURE_SCENE，不写“全部清理完成”。
- 本轮ARM取消与功能未执行；不得复用旧轮20/20或x86_64结果冒充本轮物理板实测。GCC/未定义路径本轮不重构，既有结果仍在前轮。

## 下一步

停止交人工审阅。先裁决正常安装预检阻断并补齐W3，再由W4审阅/签字决定sandbox配方，随后按人工授权进入QuickBuild；不得跳过板测门禁。最新输入为 `docs/progress/BPF_STATIC_0922/W4_INPUTS.md`、`RPM_INVENTORY.tsv`与`RPM_REQUIRES.tsv`，旧ELF或旧RPM不可替代。本轮不推包仓、不起QuickBuild。
