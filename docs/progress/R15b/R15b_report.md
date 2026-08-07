# R15b：开发板 192.168.108.25 能力探测续跑

> 状态更新（R23，依据 R22）：R15b 尚未实际执行未签名探针而留下的
> “可执行路径未观测”已由 R22 闭合。R22 在 `/root`、`/opt/usr`、
> `/opt/home`、`/home/owner`、`/var/tmp`、`/opt/var/tmp` 六个路径逐一
> 完成 ARM 静态探针执行（退出 0）和删除核验；`/tmp` 仍为 `noexec`，
> 但不再阻塞板上验证。证据见 `docs/progress/R22/R22_board_path_report.md`
> 与 `docs/progress/R22/board/path_probe_results_v2.tsv`。R15b 以下内容仍按
> 当次任务边界保留。

状态：`LIMITED`。网络问题修复后，SDB `192.168.108.25:26101` 已成为可用执行载体；板端实测为 Tizen 11.0、armv7l、32 位。SSH 22 端口仍为 `Connection refused`。探测没有安装包、没有启停服务、没有运行本项目制品；唯一板端写入是任务授权的 `/opt`、`/tmp`、`/home` 临时探针，均在同一命令中完整删除。

## 1. 连通与身份

ping 2/2 成功、退出 0；显式 `ssh -p 22` 退出 255；`sdb connect 192.168.108.25:26101` 与 `sdb devices` 均退出 0，设备标识 `rpi4`。逐次命令与结果见 `tables/connectivity.tsv` 和 `commands/01_ping.log`、`commands/32_ssh_explicit_port22.log`、`commands/03_sdb_26101.log`、`commands/04_sdb_devices.log`。

板端 `uname -a` 为 `Linux localhost 6.12.80-arm-rpi4-v7l ... armv7l`；`/etc/tizen-release` 为 `Tizen11.0/Unified 11.0.0 (arm)`，build ID 为 `tizen-unified-dev_20260727.074529_tizen-headed-armv7l`，`getconf LONG_BIT` 为 32。SDB shell 身份为 root、Smack 标签 `User::Shell`。身份表见 `tables/identity.tsv`。

`clang`、`clang++`、`gcc`、`g++` 均不可得；`readelf`、`eu-readelf`、`llvm-readelf`、`objdump`、`nm` 也均不可得，逐工具 `command -v` 退出 1。板端无法直接进行编译或 ELF 符号检查。

## 2. C++ 运行时现状

`/lib` 是 `/usr/lib` 的符号链接。实测存在：

- `libstdc++.so.6.0.33`，SHA256 `7f2953bef4475029215a9e15d7cb32ed234a6529eb9e435c40500d751e96e2e9`；
- `libgcc_s.so.1`，SHA256 `53f1c12ec20a3daa7c63d4e0631e5a749f42ddd53c5d0919cc90213653f690c7`；
- nongnu `libunwind.so.8.0.1`，SHA256 `0e035b1af7627c5ec19b85a7d68620b05e4dc133ac8705d70f74f1ad89c40ba5`。

在 `/usr/lib*` 与 `/lib*` 中均未发现 `libc++.so*` 或 `libc++abi.so*`。完整路径、符号链接和 SHA 原文见 `commands/07_runtime_ls.log`、`commands/08_runtime_sha256.log`、`commands/09_runtime_presence.log` 与 `commands/31_lib_alias_runtime_inventory.log`。

因板端无 readelf，本次只读 `sdb pull` 了 `libgcc_s.so.1` 和 `libunwind.so.8.0.1` 到宿主 `tmp/R15b/pulled/`；拉取后 SHA 与板端逐字相同。宿主 readelf 实测 libgcc_s 定义 16 个 `_Unwind*`/`__gnu_unwind_frame` 符号，包括 `_Unwind_RaiseException@@GCC_3.0`、`_Unwind_Resume@@GCC_3.0`、`_Unwind_DeleteException@@GCC_3.0`、`_Unwind_ForcedUnwind@@GCC_3.0`、`_Unwind_VRS_Get@@GCC_3.5`、`_Unwind_VRS_Set@@GCC_3.5` 与 `__gnu_unwind_frame@@GCC_3.5`。nongnu libunwind 对该命名集合的定义计数为 0。原文见 `commands/11_pull_libgcc.log` 至 `commands/17_pulled_hashes.log`、`commands/26_libgcc_unwind_exports.log`、`commands/33_libgcc_unwind_defined_count.log` 和 `commands/34_libunwind_defined_count.log`。

通过实际 `ldd` 扫描选出 `/usr/bin/ablc`、`/usr/bin/account-svcd`、`/usr/bin/action-db-recovery` 三个含 libstdc++ 的代表性二进制；三次 `ldd -r` 均退出 0，均解析到 libstdc++.so.6 与 libgcc_s.so.1。运行时现状汇总见 `tables/runtime_status.tsv`。

## 3. 部署能力边界

根文件系统为只读；`/opt` 为 rw ext4，`/tmp` 为 rw/noexec tmpfs；`/home -> /opt/usr/home`，其后端 `/opt/usr` 为 rw f2fs。三个指定路径的 mkdir、touch、删除文件与 rmdir 均逐项退出 0，没有遗留探针。`/opt` 与 `/home` 所在挂载未见 `noexec`，`/lib/ld-linux.so.3` 存在；这些是实测能力边界，不等同于已运行未签名程序。

RPM 版本为 4.14.1，`rpm -qa` 计数 1270。Smackfs 已挂载，当前标签为 `User::Shell`；securityfs 路径不可得。未执行未签名二进制，因此其 Smack 接受性为 `NOT_OBSERVED`。磁盘空间及全部能力项见 `tables/capability_boundary.tsv`。

## 4. 可行性判定

三选一结论为 `LIMITED`：SDB、armv7l 动态加载器、可写且未标 noexec 的 `/opt`/`/home` 已实测存在；但根据本轮只探测不实验的边界，没有运行交叉编译程序，也没有运行带自有 `LD_LIBRARY_PATH` 的程序，未签名二进制的 Smack 行为同样未观测。`/tmp` 明确 noexec，板上也没有编译器。因此两个具体问题均保持 `NOT_OBSERVED`，而不是由静态条件补成可行。详见 `tables/feasibility.tsv`。

## 5. 纪律偏差申报

`commands/06_toolchain_versions.log` 在探测编译器时误执行了只读的 `rpm -q libstdc++` 与 `rpm -q libgcc`，超出了任务限定的 `rpm --version` 与 `rpm -qa` 计数。两条命令只查询、不改变板上状态；原文未删除，另在 `commands/28_runtime_inventory_rpm_scope_deviation.log` 登记，交人工裁决。除此之外未安装、未部署、未启停服务、未运行本项目制品。

首次运行时清单的 07–09 号命令因宿主 shell 提前展开远端变量而生成空结果，原文保留为 `07_runtime_ls_invalid_attempt.log`、`08_runtime_sha256_invalid_attempt.log`、`09_runtime_presence_invalid_attempt.log`；随后只修正参数引用方式，判据与扫描路径不变，并以同号正式日志重新执行。三次无效尝试和正式结果均为只读。

本报告只陈述探测事实，不提供后续方案建议。

## 6. 后续状态下仍存在的能力限制

当前项目只有一块已识别的 armv7l 板卡。SDB 在 R15 当次会话不可达、
在 R15b 与 R22 会话实测可达；未做跨会话连续性监测，因此连接稳定性
没有更强的持续可用结论。板端 `clang`、`clang++`、`gcc`、`g++` 均
不可得，后续板上程序仍须由宿主交叉编译。这些限制不改变 R22 已实测的
六个可写可执行路径结论。
