# 编译适配线状态

## 线的定位与分界

本线负责 Tizen-Base-Toolchain 的 libc++ 编译适配、输入资产与 RPM 核验。运行时边界决策由运行时线负责。本轮只在独立临时目录产候选，不改平台源码或 spec，不推包仓，不覆盖 Source1002，不执行板上验证。

## 当前位置

Base 对账：73 个含 C++ 源码包中，11 个已适配推送、56 个有依据无需适配、6 个按设计排除、0 个未评估；适配推送不代表新版 static 资产已获发布批准。对账材料原样发布于 `c7aa2c5e72f78516e40b7a5423a4b83673608bad`。

已完成 bpftrace 两架构、三路径的完整 RPM，共 18 份（每格主包/common/static 三份），逐格状态见 `docs/progress/BPF_W1_0921/STATUS.md`。随后本轮在临时 CMake 副本中将 libbpf 静态链接，armv7l/aarch64 新 static ELF 均通过依赖与符号验收；旧 18 份 RPM 未被改写，不包含本轮新 ELF。构建成功、RPM 摘要通过、候选验收通过分别记录。

## 本任务结论与证据

本轮 `BPF_LIBBPF_0922` 已消除两架构 static ELF 的动态 libbpf 依赖：DT_NEEDED 恰为 libm、libgcc_s、libc 和架构 loader；`_ZNSt3__1` 有、`_ZNSt7__cxx11` 无。两架构已有 libbpf.a；原因是本地 static libclang 扩展添加裸 bpf 项，将其改为既有 `${LIBBPF_LIBRARIES}` 后，仅重链接 static。实际前后 driver 命令、CMake diff、SHA 与保护清单见 `docs/progress/BPF_LIBBPF_0922/FINAL_RESULT.md`。x86_64 同套静态运行库加实际链入的 libbpf、共享 libgcc 取消探针 20/20 通过；不是 ARM 板测。

原 spec、主包与 Source1002 未改。GCC/未定义四格沿用前轮实测，本轮未重跑、不冒充重测。本轮新 ELF 尚未纳入新 RPM；发布替换仍为 PARTIAL，不能以链接验收替代安装、功能与物理板验证。

## 挂账

- ARM/aarch64 static 的 libbpf 动态依赖已在新 ELF 消除；CMake 仅 tmp 副本一行修改，待 W4 人工审阅是否纳入包配方。旧 RPM 仍含旧 ELF，新 static RPM 为 NOT_OBSERVED。
- ARM 新 22 份 LLVM 输入已正常接入从原始根复制的新私有副本；普通预检与安装退出 0，未使用文件冲突豁免。旧冲突副本与记录保留，见 `docs/progress/BPF_W1_0921/INPUT_STATUS.md`。
- ARM 第五份 `libclang.a` 与旧份 SHA256 不同，登记身份，不归因。
- 功能等价、ARM 取消与物理板安装验证不在本轮执行，不能用 ELF 链接验收替代。最新输入与新 RPM 缺口见 `docs/progress/BPF_LIBBPF_0922/W3_INPUTS.md`，优先于上一轮的旧 RPM 输入清单。
- 本轮完整 driver 命令已在两架构前后实际执行并记录；x86_64 取消探针已实测。ARM 取消与功能仍待 W3，不作跨架构推广。

## 下一步

停止并交人工审阅；W3 板上输入见本轮 `W3_INPUTS.md`，实际 RPM 依赖声明见 `RPM_REQUIRES.tsv`，板端依赖是否满足仍须正常预检。QuickBuild 按人工裁决，待 bpftrace static 问题解决后再起。后续每项任务收尾继续更新本文件，不自行进入下一任务。
