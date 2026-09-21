# 编译适配线状态

## 线的定位与分界

本线负责 Tizen-Base-Toolchain 的 libc++ 编译适配、输入资产与 RPM 核验。运行时边界决策由运行时线负责。本轮只在独立临时目录产候选，不改平台源码或 spec，不推包仓，不覆盖 Source1002，不执行板上验证。

## 当前位置

Base 对账：73 个含 C++ 源码包中，11 个已适配推送、56 个有依据无需适配、6 个按设计排除、0 个未评估；适配推送不代表新版 static 资产已获发布批准。对账材料原样发布于 `c7aa2c5e72f78516e40b7a5423a4b83673608bad`。

已完成 bpftrace 两架构、三路径的完整 RPM，共 18 份（每格主包/common/static 三份）。逐格状态以本轮 `docs/progress/BPF_W1_0921/STATUS.md` 为准；构建成功、RPM 摘要通过、候选验收通过分别记录，不合并成一个“通过”。

## 本任务结论与证据

截至 2026-09-22 02:44 +08:00，六格整轮退出 0，18 份 RPM 已校验。两架构 libc++ static 均有 `libgcc_s.so.1`、无动态 libc++/libLLVM，标准库字符串检查符合本轮要求；但均额外依赖 `libbpf.so.1`，替换验收仍未闭合。两架构 GCC 与未定义工具链四格均通过，static 与 Source1002 逐字节一致；实际命令及 BuildRequires 中 libc++ 选项为 0。收尾 74 项输入身份校验通过。本轮结论为“写包完成，static 替换 PARTIAL”，见 `docs/progress/BPF_W1_0921/FINAL_RESULT.md`；完整命令、退出码、RPM/ELF 摘要与检查结果均在同目录。

## 挂账

- ARM/aarch64 static 的 libbpf 动态依赖尚未消除；本轮不自行修改候选配方。
- ARM 新 22 份 LLVM 输入已正常接入从原始根复制的新私有副本；普通预检与安装退出 0，未使用文件冲突豁免。旧冲突副本与记录保留，见本轮 INPUT_STATUS.md。
- ARM 第五份 `libclang.a` 与旧份 SHA256 不同，登记身份，不归因。
- 功能等价、取消行为、物理板安装验证不在本轮执行，不能以本轮写包替代。
- static 生成的链接文件与 ELF 依赖已核对，完整 driver 命令回显未观测；不冒充现场命令或取消实测。

## 下一步

停止并交人工审阅；W3 板上输入见本轮 `W3_INPUTS.md`，实际 RPM 依赖声明见 `RPM_REQUIRES.tsv`，板端依赖是否满足仍须正常预检。QuickBuild 按人工裁决，待 bpftrace static 问题解决后再起。后续每项任务收尾继续更新本文件，不自行进入下一任务。
