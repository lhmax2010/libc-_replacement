# W1 写包重启最终结果

截至 2026-09-22 02:44 +08:00，六格整轮构建均退出 0，全部写包完成；18 份 RPM（每格 main/common/static 三份）已逐份核验。**写包任务完成，static 替换验收仍为 PARTIAL**：两架构 libc++ static 均额外依赖 `libbpf.so.1`。本轮没有修改配方来消除该依赖，没有推包仓、覆盖 Source1002 或上板。

## 六格结果

| 架构 | 路径 | 整轮退出码 / RPM 数 | 载荷核验 |
|---|---|---|---|
| armv7l | libc++ | 0 / 3 | 本轮要求的标准库依赖与字符串检查通过；额外 libbpf 动态依赖未闭合 |
| armv7l | GCC | 0 / 3 | PASS；static 与本格 Source1002 原件逐字节一致 |
| armv7l | 未定义 `_toolchain` | 0 / 3 | PASS；static 与本格 Source1002 原件逐字节一致 |
| aarch64 | libc++ | 0 / 3 | 本轮要求的标准库依赖与字符串检查通过；额外 libbpf 动态依赖未闭合 |
| aarch64 | GCC | 0 / 3 | PASS；static 与本格 Source1002 原件逐字节一致 |
| aarch64 | 未定义 `_toolchain` | 0 / 3 | PASS；static 与本格 Source1002 原件逐字节一致 |

证据：`MATRIX.json`、`RESULT.md`、各 `cells/*/build.log` 与 `exitcode`、各 `verify-bpf-*/result.json`、`audit-bpf-*/result.json`。四个回退格均实跑 GNU 14.2.0；每格记录 120 条编译/链接命令，libc++ 选项计数为 0，展开 spec 的 libc++ BuildRequires 为 0。该结论限于本轮配置及已记录的命令，不外推到任意外部配置。

## 候选与原资产

| 架构 | 候选 ELF 字节数 | 原 ELF 字节数 | 候选 `_ZNSt3__1` 行数 | 候选窄/宽 `__cxx11` 行数 |
|---|---:|---:|---:|---:|
| armv7l | 90,847,040 | 98,974,296 | 22,952 | 0 / 0 |
| aarch64 | 136,127,672 | 129,681,256 | 16,480 | 0 / 0 |

两个候选均动态依赖 `libbpf.so.1`、`libm.so.6`、`libgcc_s.so.1`、`libc.so.6` 和本架构加载器；均不依赖动态 libc++、libc++abi、libstdc++ 或 libLLVM。原资产均只有 libm、libc 与加载器这三项动态依赖。

每架构 Source1002 对应目录内有 1 个普通文件（原二进制）；static RPM 内有 2 个普通文件（`usr/bin/bpftrace-static` 与 11,358 字节 LICENSE），不能将这两种计数混为二进制数量差异。完整文件记录、SHA256、大小及依赖见 `RPM_INVENTORY.tsv`、`ELF_COMPARISON.tsv` 与各核验 JSON。六份 static RPM 不是本轮 RPM 总数；总数为 18。

两架构原资产的精确 `_ZNSt7__cxx11` 检索均为 0，但扩展 `St7__cxx11` / `std::__cxx11` 检索均有 257 行；所以精确前缀零命中不能证明原资产没有 libstdc++ ABI 痕迹。这里只报告字符串观察，不还原其未知编译配方，不改变既有判定。

## 输入、参数与资源

- ARM 22 份完整 LLVM RPM 与 aarch64 12 份完整 LLVM RPM 均复制并逐文件核验；实际构建安装的是其中各 8 个所需 LLVM 包，另加已固定的依赖输入。ARM 使用第五份 `libclang.a`：`5deb3c6c662c73865e8a38f4f8df8d6e672af2ae8a6a8faae7700b5909c1524a`，与旧份差异只登记，不归因。
- 收尾对 34 份 LLVM RPM 的来源/副本、两份已安装归档、两份 spec 的来源/副本重新校验，共 74 项全通过：`INPUT_IDENTITY_FINAL.json`。原 spec 与候选副本未改。
- 六格隔离环境的 payload 预检均为 `w5.xzdio`；aarch64 LLVM 输入 RPM 原有 zstd 未改。本轮六格均未出现写包失败，没有触发 GDB 失败取证分支。核验脚本因额外 libbpf 返回 1 的两格不是 rpmbuild 失败。
- 单任务并行度 1，nice 19、ionice idle，cgroup `memory.max=16536457216` 字节（机器总内存 50%）；实际构建调度进程的资源记录、每 500 目标 I/O 监控与脚本快照见各 cell。没有触发 I/O 暂停；最后可用空间 158 GiB。
- ARM GCC 00:56 启动、01:49 前整轮完成；ARM 未定义路径 01:50 启动、02:43 前完成。截止仍是 05:01；界面中断没有重启构建或延长时限。

## 发布与后续输入

对账原样推送远端 SHA：`c7aa2c5e72f78516e40b7a5423a4b83673608bad`。aarch64 阶段材料已推送 `e44ad19ff271af0b7761b186ec49a6c0bb4b98cd`。本次 ARM 阶段与最终材料的提交/远端核对记录见后续 `PUSH_RECEIPT.md` 及 raw；仅推项目分支 `codex/r5-ehabi-diagnosis`。

W3 输入清单见 `W3_INPUTS.md`；18 份 RPM 的实际 Requires 共 607 行，见 `RPM_REQUIRES.tsv`。这是包的依赖声明，不证明板端已具备；旧板上记录缺 BCC/Clang 运行库，本轮未连接板子，当前板端状态 `NOT_OBSERVED`。本轮不替代功能等价、取消探针与安装测试。QuickBuild 仍遵循人工裁决，等待 static 子包问题解决。

## 自行判断与未闭合项

1. ARM 旧私有副本出现同 NEVRA 文件冲突后，另复制原始根并正常安装固定输入；普通预检和安装均退出 0，未用 `--replacefiles`、`--nodeps` 或 sudo。该实施判断与过程详见 `INPUT_STATUS.md`、`DECISIONS.md`。
2. 新 ARM LLVM 输入与旧输入不同，使用新私有 topdir 完整构建，不将旧 88% 对象作为新输入的验证；旧断点保留。
3. static 构建没有 verbose 驱动命令回显。CMake `link.txt` 声明 `-shared-libgcc`、无 `-static-libgcc`，实际 ELF 含 `libgcc_s.so.1`；但完整驱动命令现场回显 `NOT_OBSERVED`，不能把生成文件冒充已捕获命令，也不能据此补称做了本轮取消实验。
4. 本轮不擅自修复 libbpf 动态依赖；候选尚不能据此批准替换发布资产。改法、上板和发布均交人工审阅后另定。

全部构建已结束，不继续下一任务。
