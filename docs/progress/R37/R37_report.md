# R37 开发板健康检查与 R36 代码补充上传

执行日期：2026-08-14（Asia/Shanghai）
执行边界：板上与既有源码/证据全程只读；未安装、未部署、未删除、未终止进程、未修改配置、未构建。

## 1. 结论摘要

| 项目 | 结论 | 直接依据 |
|---|---|---|
| 板卡物理身份 | `UNDETERMINED` | 当前硬件序列号为 `100000004694d889`、machine-id 为 `08df89168e0a4d68845fa7e84c2ac480`，但旧 `.25` 证据没有记录任一值，无法比较 |
| 运行镜像身份 | `DIFFERENT_IMAGE_CONFIRMED` | `.25` 为 `tizen-unified-dev_20260727.074529_tizen-headed-armv7l`；`.26` 为 `tizen-unified-toolchain_20260728.012216_tizen-headed-armv7l` |
| 本项目残留 | `有` | 板上仍有 R36 遗留的 PID 9448 与子进程 PID 9450；R36 专用部署目录本身已不存在 |
| 系统完整性 | `存在异常` | `rpm -Va` 退出 1；可见 3 个 `/usr/lib` 路径存在 size/digest/mtime 漂移，另有大量缺失、权限不可读和其他属性差异；关键 C/C++ 运行时文件本身未被定点 `rpm -V` 报告为漂移 |
| R36 材料上传 | `AVAILABLE_WITH_DECLARED_GAPS` | 代码、完整当前文件、实际用例、CMake 配置、工具链、部署/运行/清理证据均已整理；未曾生成或未执行的材料明确登记 |

以上“存在异常”只陈述实测状态，不把 RPM 漂移或残留归因到任何未取证来源。本任务未实施清理。

## 2. 第一部分：开发板健康检查

### 2.1 连接与身份

SDB 目标为 `192.168.108.26:26101`，第一次连接即成功，未发生重试。当前实测：

- `uname -a`：`Linux localhost 6.12.80-arm-rpi4-v7l #1 SMP Tue Jul 28 02:41:25 UTC 2026 armv7l GNU/Linux`；
- 架构：`armv7l`；系统：Tizen 11.0 Unified；
- build ID：`tizen-unified-toolchain_20260728.012216_tizen-headed-armv7l`；
- Raspberry Pi 型号：Raspberry Pi 4 Model B Rev 1.5；硬件序列号：`100000004694d889`；
- machine-id：`08df89168e0a4d68845fa7e84c2ac480`；
- 已安装 RPM：1,272 个，排序后的完整 NEVRA 集 SHA256 为 `c2c79cdd94ad26a06fbc4fc3c81bb956a275361c5e54ee487467534250bb86e8`。

旧 `.25` 证据只保留了 Tizen/架构/build ID，未保留 machine-id、硬件序列号或可重建的完整 NEVRA 集。因此：

- 物理上是“同一块板改 IP”还是“另一块板”，结论为 `UNDETERMINED`；
- 当前镜像与旧镜像不同则为确定事实；
- R22 的六路径结论和 R16 的旧板运行时现状不能直接视为当前镜像事实，须按当前镜像分别实测。本任务只复核健康状态，不重新执行写入/运行探针。

逐字段对照见 `tables/identity_comparison.tsv`；原文见 `commands/001_workspace_and_prior_identity.log`、`003_board_identity.log`、`015_board_package_identity.log`。

### 2.2 文件与进程残留

对 `/root`、`/opt/usr`、`/opt/home`、`/home/owner`、`/var/tmp`、`/opt/var/tmp`、`/tmp` 以 `r36`、`r22`、`s1_`、`libc++abi`、`probe` 名称模式只读检索：

- `/var/tmp/r36_s1_20260814`：`NOT_FOUND`；
- 可读范围内：未命中文件或目录；
- 未覆盖范围：`/root`、`/opt/usr/home/guest`、owner 的部分 Chromium 私有目录因当前 SDB owner 权限被拒，不能把结果扩展为全盘零残留；
- 可读 environment/profile/profile.d 中未命中项目路径或变量；root/owner 私有 profile 不可读；
- `/etc/ld.so.conf*` 可读项只见系统路径 `/usr/local/lib`、`/usr/lib/hal`、`/usr/lib/lwe`、`/usr/lib/rustlib/dylib`，未见项目路径；板上 `ldconfig` 命令不可用。

进程核查发现明确残留：

| PID | PPID | 命令 | 状态 |
|---:|---:|---|---|
| 9448 | 1 | `/bin/sh -c base64 -d > /var/tmp/r36_stdin_transfer_test/probe && chmod 700 ...` | sleeping |
| 9450 | 9448 | `base64 -d` | sleeping |

PID 9448 的 fd 0/1/2 均指向 `/dev/pts/0`，环境中未见自定义 `LD_LIBRARY_PATH`；目标目录当前已不存在。依纪律未终止进程、未删除任何对象。完整 `/proc` 原文见 `commands/012_board_residual_process.log`，汇总见 `tables/residue_findings.tsv`。

### 2.3 RPM 与关键运行时完整性

`rpm -Va` 完整执行并返回 `RPM_VA_RC=1`。原始 9,449 行输出保存在 `commands/008_board_rpm_verify_retry.log`；机械分类口径为 RPM 原始记录，不等同于文件去重数：

| 分类 | 记录数 | 分母/口径 |
|---|---:|---|
| missing | 8,268 | `rpm -Va` 输出记录 |
| 其中带 Permission denied | 1,994 | missing 记录 |
| 属性标志记录 | 1,168 | `rpm -Va` 输出记录 |
| 其中 config marker | 11 | 属性标志记录 |
| `/bin`、`/sbin`、`/lib`、`/usr/bin`、`/usr/sbin`、`/usr/lib` 路径 | 112 | 属性标志记录 |

二进制/库路径的 112 条属性记录中，只有 3 条带 size/digest 漂移标志 `S.5....T.`：

- `/usr/lib/gconv/gconv-modules`；
- `/usr/lib/rpm/macros`；
- `/usr/lib/systemd/user/default.target`。

其余该类记录主要是 `P`、`?`、`L` 等属性。配置文件类 11 条与全部逐行记录见 `tables/rpm_verify_flagged.tsv`；标志计数和上述三条分别见 `tables/rpm_binary_flag_counts.tsv`、`tables/rpm_binary_content_mismatches.tsv`。这些差异未被本任务判定来源。

关键运行时实测：

| 文件 | 所属 RPM | SHA256 |
|---|---|---|
| `/usr/lib/libstdc++.so.6.0.33` | `libstdc++-14.2.0-1.14.armv7l` | `0f259ca14a701c7fd1122c74b16aa5a224e6e3446855a3649700739959e4d26c` |
| `/usr/lib/libgcc_s.so.1` | `libgcc-14.2.0-1.14.armv7l` | `0eba387a1e9bbed6ecc903e426a84ed3aa4b7f848f547d19b08d151072c19335` |
| `/usr/lib/libc.so.6` | `glibc-2.40-2.8.armv7l` | `d5e36dd6339e95adedcbb01b655bc3df46d233fbba5d98f24105192eb8935015` |
| `/usr/lib/libpthread.so.0` | `glibc-2.40-2.8.armv7l` | `c57f5ccf3cd02dc0008f6c4e669ab4754378ce1b56cf57d08a690c6f1516a9cd` |

对 `libstdc++`、`libgcc`、`glibc` 的定点 `rpm -V` 未报告上述四个关键文件的 size/digest 漂移；输出中的差异为许可证缺失、locale 缺失/不可读及 `glibc_post_upgrade` 不可读等。安装 RPM 名中无 libc++/libc++abi；可读文件系统范围也零命中，但由于 `/opt` 私有目录不可读，全文件系统结论为 `UNDETERMINED`。磁盘原文见 `commands/007_board_runtime_integrity.log`。

### 2.4 第一部分最终申报

- 板卡身份：`UNDETERMINED`（物理板）；`DIFFERENT_IMAGE_CONFIRMED`（运行镜像）。
- 本项目残留：`有`，为 PID 9448/9450；可读范围未见项目文件，权限盲区已列明。
- 系统完整性：`存在异常`，详见 RPM 全量清单；关键运行时文件未被定点校验报为内容漂移。
- 本任务未清理、未修改、未启停任何板上对象。

## 3. 第二部分：R36 代码与材料补充上传

### 3.1 实际用例与 R34-B 对照

板上实际部署的用例源码原件为：

- `materials/R36/src/s1_rethrow_probe_nostdlib.cpp`；
- 参照原件：`materials/R34B/src/s1_rethrow_probe.cpp`；
- R37 由上述两份既有原件生成的完整 diff：`materials/R36/src/r34b_to_r36_actual_board_source.diff`。

实际变化与同构标签全部列于 `tables/testcase_equivalence.tsv`。最关键的非同构点是 R34-B 的 `std::runtime_error` 被替换为自定义 `RealException`；原因记录为板上没有 libc++.so，R36 实际测试件使用 `-nostdinc++/-nostdlib++` 只链接实验版 libc++abi。cancel、exit 与两项 no-catch 的控制流和判据保持同构，但源码并非逐字同构。foreign 格为 EHABI 字段适配。

R36 只在板上实际运行了第一格 `libcxxabi/cancel`；该格退出 134 后 fail-closed，其他格均为 `NOT_OBSERVED`。本任务没有重新生成输出冒充原件。

### 3.2 配置、工具链与运行身份

- CMake 完整命令和输出：`materials/R36/commands/025_configure_armv7l_libcxxabi.log`；
- CMakeCache 原件副本：`materials/R36/build/CMakeCache.txt`；
- 关键选项：`LLVM_ENABLE_RUNTIMES=libcxx;libcxxabi`、`LIBCXXABI_USE_LLVM_UNWINDER=OFF`、shared ON、static OFF；
- 交叉编译器：Clang 22.1.8，target `armv7l-tizen-linux-gnueabi`；
- sysroot：R30 armv7l 单用途 GBS scratch root，完整路径见 `tables/toolchain_identity.tsv` 与 wrapper 原件；
- 测试件编译/链接、DT_NEEDED、SONAME/readelf：`materials/R36/commands/028_compile_armv7l_probes.log`、`027_build_output_identity_and_s1_diff.log`；
- 板上实际加载身份：`materials/R36/commands/036_board_runtime_identity.log`，包含 `LD_LIBRARY_PATH`、`ldd` 与 `LD_DEBUG=libs` 原文；
- 部署与 SHA：`materials/R36/commands/035_deploy_board_artifacts_gzip_chunked.log`；
- 实际运行输出：`materials/R36/commands/038_board_native_matrix_retry.log`；
- 清理命令与当时的自核验：`materials/R36/commands/039_board_post_failure_and_cleanup.log`。

### 3.3 资源门禁与脚本原件

R36 所有留存脚本均逐字复制到 `materials/R36/tools/`。事实链为：

1. `commands/024_resource_gate.log` 中的资源门禁是内联 bash 命令，不是独立脚本；
2. 该命令因 awk 引用展开导致 `bash: line 11: $2: unbound variable`，退出 1；
3. 之后 `commands/025`、`026` 为另外的独立命令动作并继续配置/构建；
4. 构建监控脚本 `build_with_resource_monitor.sh` 本身另有完整原件，其调用见 `commands/026`；
5. 没有留存“资源门禁的上层调用脚本”，故登记 `NOT_AVAILABLE`，未事后生成；
6. 清理同样是 `commands/039` 中的内联命令，没有独立清理脚本。

部署、矩阵、运行时盘点、路径探针、结果日志包装器、交叉编译 wrapper 均在 `materials/R36/tools/`；R36 所有 015–040 号相关命令原文均在 `materials/R36/commands/`。

### 3.4 libc++abi 改动完整内容

R36 留存的三处 S1 改动 diff 与三个被改文件完整当前内容位于：

- `materials/R36/libcxxabi/r34b_s1_complete.diff`；
- `materials/R36/libcxxabi/include/cxxabi.h`；
- `materials/R36/libcxxabi/src/cxa_personality.cpp`；
- `materials/R36/libcxxabi/src/cxa_exception.cpp`。

上游基线身份原件位于 `materials/R36/evidence/baseline_identity.txt`。R36 原有代码交付目录中的测试文件与 diff 另按原样复制至 `materials/R36/code_tests/`，便于区分“R36 当时已上传的差异”与“R37 本次按实际板上源码补生成的完整差异”。

### 3.5 索引与缺失申报

- `tables/material_index.tsv`：每份材料的来源任务、原路径、R37 上传路径、SHA256 与状态；
- `tables/material_gaps.tsv`：所有不存在或未观测材料的 `NOT_AVAILABLE`/`NOT_OBSERVED` 申报；
- 所有复制均为逐字复制；唯一新生成的代码差异明确标为 R37 派生文件，不冒充 R36 原件。

## 4. 证据导航

- 板上命令原文：`commands/002`–`009`、`012`、`015`；
- 旧/新身份原文：`commands/001`、`003`；
- R36 原件收集与 diff 生成过程：`commands/010`、`011`、`016`；
- 表格生成过程：`commands/013`、`014`、`017`、`018`；
- 全量 SHA256：`MANIFEST.sha256`。

本报告不分析或重判 R36 的 S1 技术结论，只交付健康检查事实与可供人工审阅的原始代码/证据。
