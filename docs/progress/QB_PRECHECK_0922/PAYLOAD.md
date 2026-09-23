# QuickBuild payload 核查

## 结论

**QuickBuild 无需 payload 对策（按本次人工裁决与源码/包头核查）。OBS 当前实际宏值仍为 NOT_OBSERVED。** 不能用本地配置或历史 RPM 代替服务端最终生效宏；但不能因此把已明确不触发缺陷的 `w5T.xzdio` 列为阻断。未修改任何 payload、spec、prjconf 或构建根。

`docs/progress/ARM_RPM_DIAG/upstream-rpmio.c:765–777`：T 后无数字时走 else，`threads = -1`，分支内不前移 mode；外层 for 移到终止符后即结束，不越界。只有 T 后有数字时进入 if，`while (isdigit(*++mode));` 停在终止符上，再由第758行外层 `mode++` 越过终止符造成越界读。因此本地项目配置与两个保留根的 `w5T.xzdio` **不触发该缺陷**。

09-17 armv7l LLVM 失败由本地为限制 xz 线程数而覆盖 `w5T1.xzdio` 引入，与 libc++ 无关；该覆盖不在任何 spec 或 sandbox 提交中。后续本地构建继续用 `w5.xzdio`（单线程、不触发），**禁止使用 T<数字> 形式**。本轮只核查，不执行构建或改配置。

## 现有 RPM 包头补查（2026-09-23）

取自已下载的固定 Base-Toolchain 快照 `tizen-base-toolchain_20260828.101647`，不是本地重构产物、不是 GCC Base、也不是当前 reference 的同义词。逐个重算 SHA256，与原下载清单匹配后执行用户指定的 rpm -qp 查询，均退出0。

| armv7l RPM | 大小（字节） | PAYLOADCOMPRESSOR | PAYLOADFLAGS |
| --- | ---: | --- | --- |
| libllvm-22.1.8-2.3 | 12518937 | xz | 5T |
| libicu-78.1-1.2 | 9697277 | xz | 5T |
| boost-devel-1.83.0-1.1 | 12833849 | xz | 5T |

原命令、完整URL、RPM SHA、原清单SHA、stdout/stderr/退出码见 `RPM_PAYLOAD_HEADERS.json` 和 raw/031。三个实际包头支持历史发布采用 `5T`、非 `5T1`；不据此声称取得当前OBS配置或所有包均同值。源码摘录见 raw/025。

| 证据层 | 实际观测 | 能证明 / 不能证明 |
| --- | --- | --- |
| osc | raw/007：命令退出 1，缺少 `/home/toolchain/.oscrc`，在本地配置提示阶段遇 EOF；raw/009 确认未创建配置 | 未取得 prjconf；连所尝试 API 地址及项目标识是否正确也未由服务端确认。不是一次已认证的 OBS 查询失败 |
| 本地项目配置 | `codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config:44–62`，Macros 中第 61 行 `%_binary_payload w5T.xzdio` | 本地保存值不是以 T<数字> 结束；不代表当前服务端值 |
| GBS 配置 | `config/gbs_llvm.conf:8–12` 指向 Base-Toolchain 与 Unified-Toolchain reference 的软件包仓；本身没有 payload 宏 | 此次只读取 Base-Toolchain 公开索引，不把 Unified 或 GCC Base 混作目标 |
| armv7l W1 保留根 | `/usr/lib/rpm/macros:410` 为 `w19.zstdio`；`/home/abuild/.rpmmacros:17` 为 `w5T.xzdio`；按原 rcfile、宏源及 target、无 CLI 覆盖实测展开 `w5T.xzdio` | 当前保留根的有效宏，不是历史每次 CLI 的值，也不是 OBS 当前值 |
| aarch64 W1 保留根 | 同上，实际展开 `w5T.xzdio` | 不能把既有 aarch64 RPM 的 zstd payload 与本次宏查询混为一个观测 |
| 公开 reference | build.xml 标识 `tizen-base-toolchain_20260914.073422`；builddata 目录仅列 buildlogs/、manifest/ | 已读索引没有 prjconf；未取得服务端继承配置或命令行覆盖 |

命令、退出码与全文分别见 raw/007、009、013、017、018、020；根内只读命令见 `roots/commands.json`，各宏文件命中及 SHA 见 `roots/*-payload-files.out`。`/etc/rpm/macros*` 与 `/usr/lib/rpm/tizen/macros` 也逐个查过，未命中，不能将 grep 的退出 1 写成工具不可执行。第 409 行的 gzdio 是注释，不是生效值。

本地项目配置 SHA256：`aa7e0ad85c9decbb5b53f822b2414c1ed4569a2b2b39930c08c3f69e011fa209`；GBS 配置 SHA256：`8fab16c1ca9db28d45f6b269881af47a71ef3d111d93527b602f57e1e37d49cf`。公开 URL 为 `https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/reference/`，其滚动性由本次 build.xml 身份限定。

## 09-17 的变化来自哪里

**已确认是本地 rpmbuild 命令行覆盖，不是项目配置同步。** `docs/progress/BUILD_STATIC_0917B/resume_0917/resume_stop_1909/STATUS.md:65–72` 原文：

> armv7l 首轮编译确为 -j1，但 RPM 使用原 .rpmmacros:17 的 `%_binary_payload w5T.xzdio`。
>
> 命令行覆盖为 `_binary_payload w5T1.xzdio`，保留原 XZ 压缩级别 5，明确线程 1；不修改任何 spec 或平台宏文件。

同轮 `raw/202_arm_serial_rpm_build.stdout.txt:5` 的实际 argv 含该 `--define`。本轮 raw/009、016 保存摘录。历史触发背景是 XZ 请求 20 线程、按预算降到 15；因此当时为了资源控制加了 T1。不能据本地 T1 推导 OBS 也为 T1。

## 服务端观测边界与已撤销的条件性对策

若需直接确认当前服务端宏，仍需正确 API/项目标识及只读 prjconf 导出（含继承配置、最终覆盖），或实际 QuickBuild 宏展开记录。本轮不尝试凭据、不创建 osc 配置。这是观测边界，**不再作为启动QuickBuild前必须选择payload对策的阻断**。

前版列出的以下条件性对策均未实施；本次已裁决无需采用，保留仅为说明前版讨论范围，不是待执行任务：

| 条件性选择 | 改动位置 / 分支 | 范围与副作用 |
| --- | --- | --- |
| llvm spec 中 `%define _binary_payload w5.xzdio` | llvm 的 `sandbox/lhmax2025/libcxx-ehabi-backport`，须另行批准 | 包级覆盖，改变 LLVM RPM 压缩线程接口/表示；其他包不受保护；不是修复 RPM |
| 项目 Macros 改成无末尾 T<数字> 的值，如 w5.xzdio | OBS 项目 prjconf；它不是某个包的 sandbox，配置归属分支 NOT_OBSERVED | 项目范围改变压缩并行度、耗时与 RPM 身份；需核对继承/CLI 覆盖；不是修复 RPM |

`w5.xzdio` 已有本地完整写包通过证据；不能把这条规避写成 RPM 根修复已合入。
