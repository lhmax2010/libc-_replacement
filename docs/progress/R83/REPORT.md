# R83：sandbox libc++ 打包完整性与全量编译材料

## 结论摘要

**构建路径结论：可以直接从 sandbox 分支构建 libc++ 与 libc++abi 包。**
`libcxx-runtimes.spec` 的构建选项、BuildRequires、四个主子包和安装清单完整；
GBS standard export 实测把 sandbox HEAD 上四个补丁生成为 `Patch6`–`Patch9`
并在 `%prep` 依次应用。层 C 修改的四个消费者头均由 CMake 安装到
`/usr/include/c++/v1`，整个目录归 `libc++-devel`，所以后续软件包编译会
取得这批防护。

**但打包审计元数据有一项明确缺失：** 随包安装的 `SOURCE_PROVENANCE`
仍记录旧 HEAD、6 个补丁和 “plus six patches”；当前实际是 HEAD
`08a64b6e...`、10 个导出补丁。它不阻止构建和安装，但不能作为准确发布
provenance。R83 按纪律未修改，发布前需人工决定后续打包修正。

R83 没有执行全量编译。当前主机已有 armv7l 内核 GBS 构建占用多核，负载
约 9–10；为避免近期崩溃重现，本机 libc++ 单包试构建也记
`NOT_OBSERVED`。完成的是低优先级 GBS export、静态安装链核对、既有成功
构建证据复核和另一台服务器的完整运行材料。

## 1. 输入和纪律

- 工作区：`/home/toolchain/development/libc++_replacement`
- sandbox ref：`refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport`
- 远端/本地复核 HEAD：`08a64b6eb95ed74339f1d862a6c22a2f93fa78f9`
- 只读 worktree：`tmp/R83/sandbox`
- Base project_config SHA256：
  `aa7e0ad85c9decbb5b53f822b2414c1ed4569a2b2b39930c08c3f69e011fa209`
- Unified project_config SHA256：
  `a5abe9c7a6dcf2909799e6bbc6cca6349c2fe94d9cf74132db74eebc30f34086`
- 资源门禁：`tools/resource_gate.sh --level light` 退出 0。

未修改平台源码、四补丁、sandbox worktree 或两份 project_config；未向
Gerrit/LLVM 上游或其他外部源码仓推送；未执行平台全量编译。

## 2. 分支与打包内容盘点

从 `upstream/22.1.8` 到 sandbox HEAD 的关键提交链是：

1. 五个既有 Tizen downstream 提交，止于 `fd7a4929...`；
2. `3d3e3da4...`：既有 ARM EHABI backport；
3. `5ed6c772...`：`packaging: add libc++ runtimes spec`；
4. `77e0f73b...`：层 A；
5. `82afebab...`：层 B；
6. `9039afa4...`：层 C 头文件批；
7. `08a64b6e...`：层 C 库内批。

打包提交新增四文件：`SOURCE_PROVENANCE`、`libcxx-llvm22.map`、
`libcxx-runtimes.manifest`、`libcxx-runtimes.spec`，无其他改动。完整现状见
`tables/packaging_files.tsv` 和 `PACKAGING_INVENTORY.md`。

spec 生成四个主包：

- `libc++`：shared runtime；
- `libc++-devel`：头、linker script、modules 和 experimental archive；
- `libc++abi`：shared ABI runtime；
- `libc++abi-devel`：ABI linker name，公共 ABI 头放在共同 c++/v1 目录。

BuildRequires 与 runtime/devel 精确 Requires 形成闭合。R9C 在四补丁进入前
已用同一 spec 在 x86_64、armv7l、aarch64 三架构各退出 0 并生成全部 RPM，
证明 spec 基本形态可构建；当前 sandbox QuickBuild 已通过是任务背景给定
事实，但 R83 本机拿不到该平台日志，日志本身记 `NOT_AVAILABLE`。

## 3. 四补丁实际应用证据

对 sandbox HEAD 执行：

```text
nice -n 15 ionice -c 3 gbs export --spec libcxx-runtimes.spec \
  --packaging-dir packaging -o /home/toolchain/development/libc++_replacement/tmp/R83/export .
```

退出 0。GBS 明确输出从 `upstream/22.1.8..HEAD` 生成补丁。导出 spec 中
`Patch0`–`Patch9` 与 `%patch0`–`%patch9` 全部存在，四层对应：

| 导出项 | 提交 | 层 | 应用指令 |
|---|---|---|---|
| `Patch6` / `0007-...` | `77e0f73b...` | A | `%patch6 -p1` |
| `Patch7` / `0008-...` | `82afebab...` | B | `%patch7 -p1` |
| `Patch8` / `0009-...` | `9039afa4...` | C 头文件批 | `%patch8 -p1` |
| `Patch9` / `0010-...` | `08a64b6e...` | C 库内批 | `%patch9 -p1` |

完整十补丁 SHA256 在 `tables/exported_patch_inventory.tsv`；原始导出日志、
spec 行和文件 SHA256 在 `raw/015_*`、`raw/017_*`、`raw/019_*`。因此
checked-in spec 没有手写 Patch 行不是漏包，而是 GBS non-native export 的
预期形态。

## 4. 层 C 头文件安装链

层 C 头文件批修改的安装头为 `__ostream/basic_ostream.h`、`future`、
`istream`、`string`；另有实现辅助代码 `src/future.cpp`。库内批再次修改
`istream`，并修改 `src/ios.cpp`。

证据链：

1. 四个头都出现在 `libcxx/include/CMakeLists.txt` 的 `files` 清单；
2. `LIBCXX_INSTALL_HEADERS` 默认 ON，spec 未关闭；
3. CMake 第 1726–1734 行逐文件安装到
   `${LIBCXX_INSTALL_INCLUDE_DIR}`，默认是 `${includedir}/c++/v1`；
4. spec `%install` 执行完整 `cmake --install .`；
5. `libc++-devel` 第 130 行拥有 `%{_includedir}/c++/v1`。

结论：20 处消费者实例化防护随 `libc++-devel` 安装；在新 provider 进入
buildroot 后才编译其余包，就不会漏掉这批头文件。逐路径见
`tables/layer_c_install_paths.tsv`。

当前四补丁 RPM 的包内逐文件提取检查为 `NOT_OBSERVED`，原因见第 6 节；
这不改变上述由实际导出 spec + CMake 安装清单组成的静态结论。

## 5. project_config 分析和精确建议

Base 提供工具链、flags、buildroot package 集与架构基础；Unified 是产品
overlay，并在第 151–157 行最终定义 `%build`。默认 clang 已由 Base 第
133 行选择。

两文件中没有源码 URL、Git branch 或 sandbox selector。因此：

- 为“选择 sandbox 源码”修改的 project_config 行：**无**；
- sandbox 源码由 checkout/ref/SHA 固定；
- sandbox 二进制由优先本地 RPM repo、较高验证 Release 和
  `--use-higher-deps` 固定。

为了让全部消费者实际使用新头和 libc++，建议只在人工服务器的副本上：

1. Base 原第 290 行后新增
   `Support: libc++-devel libc++abi-devel`；
2. Base 原第 298 行后新增
   `Keep: libc++-devel libc++abi-devel`；
3. Base 原第 299 行后新增 `Keep: libc++ libc++abi`；
4. Unified 原第 155 行后加入 `CXXFLAGS += -stdlib=libc++`；
5. 同处加入
   `LDFLAGS += -stdlib=libc++ -Wl,--no-as-needed -lc++abi -Wl,--as-needed`。

精确 diff 为 `artifacts/PROJECT_CONFIG_CHANGES.diff`，已针对输入 SHA 以
`patch --fuzz=0 --dry-run` 和临时副本实际应用双重验证；原文件 SHA 保持
不变。详细取舍和全局 LDFLAGS 风险见 `PROJECT_CONFIG_ANALYSIS.md`。

## 6. 本地试构建

处置：`NOT_OBSERVED`，未启动。

资源门禁初始退出 0，但准备构建时发现已有 armv7l 内核 GBS 任务运行；20
核主机 load1 约 9，多条 QEMU/clang 各占 100% CPU，swap 已使用约 3.2–3.4
GiB。用户此前明确要求控制资源避免再崩溃。叠加 libc++ GBS build 会与
该任务竞争 CPU/I/O，故只完成单进程、nice 15、idle-I/O 的 export。

这是“资源/并发环境不适合”而非 spec 构建失败，未触发任务 3(d) 的失败
停止条件。原始快照在 `raw/021_*`、`raw/022_*`、`raw/036_*`。

## 7. 人工全量编译材料

`GBS_RUNBOOK.md` 给出可复制的完整流程：

1. 固定 sandbox ref 与 HEAD；
2. 核验并复制两份 project_config；
3. 生成未注入的 bootstrap buildconf 与 libc++ consumer buildconf；
4. 每架构先用 sandbox 的 `llvm.spec` 构建 LLVM/Clang；
5. 将其放进优先本地仓，再构建同 HEAD 的 `libcxx-runtimes.spec`；
6. 将四个 provider 包放进优先本地仓；
7. 用修改副本的 buildconf 执行 `gbs --full-build`；
8. 收集每条命令、退出码、完整日志、RPM SHA256 与 diagnostics。

验证 RPM 用 `--release 83.1` 区分可能存在的旧 `22.1.8-1`，不改源码或
补丁。provider 必须先于消费者；所谓安装是由 GBS 从本地仓装入隔离
buildroot，禁止装到宿主。

依赖与容量预算见 `DEPENDENCIES_AND_TRIAGE.md`。全量源码服务/认证不在两份
project_config 或本机 gbs.conf 中，记 `NOT_AVAILABLE`；执行服务器必须沿用
其正常 Tizen source-enabled profile。若缺失，应作为环境失败停止，不能
自行猜 URL。

## 8. 结果判读

完整 45 个已知 source RPM 身份与 13 个硬编码 `-lstdc++` 身份分别在：

- `tables/known45_source_rpms.txt`
- `tables/hardcoded_lstdcxx_13.tsv`

45 项可能以内部头/扩展类型/ABI namespace/保留成员/configure probe 的
编译或链接错误出现；名单含同包不同 Release，不能改写成 45 个唯一包名。

13 项只证明源码存在硬编码引用，生产构建可达性此前均为
`NOT_OBSERVED`。它们可能不触发、可能链接失败，也可能成功但产物同时
`DT_NEEDED` libc++ 与 libstdc++。后一种仍是必须处理的污染，不能只看
构建退出码。

判读先核对 buildroot 安装的是 sandbox `83.1` RPM；再与同 snapshot
baseline 逐包比对。R83 输入没有 matched baseline 完整结果，故无法对照的
新增失败只能标 `INDETERMINATE`。具体分流规则见
`DEPENDENCIES_AND_TRIAGE.md`。

## 9. 自行判断与尚存疑问

自行判断：

1. 因已有高负载构建，跳过本地单包构建；
2. 用验证 Release `83.1` 避免同 NEVRA 内容歧义；
3. 任务未列架构，运行手册按 spec 全部三架构编排；
4. 建议 Support/Keep + CXXFLAGS/LDFLAGS 作为验证注入，不把 R29 三个样本的
   私有 pkg-config/CMake overlay 路径带入平台配置。

尚存疑问/不可得：

1. 人工服务器的全量 source service、内部镜像与认证配置 `NOT_AVAILABLE`；
2. 最终批准的架构矩阵 `NOT_SPECIFIED`；
3. 当前 sandbox QuickBuild 完整日志 `NOT_AVAILABLE`（任务背景声明已通过）；
4. 同 snapshot matched baseline 逐包结果 `NOT_AVAILABLE`；
5. stale `SOURCE_PROVENANCE` 的修复提交/Release 策略待人工决定。

逐项表见 `tables/self_decisions_and_questions.tsv`。

## 10. 技术性无效尝试

- 010a：记录器没有执行位，直接调用退出 126；未改文件，改用 `bash`。
- 010b：把编号与标签合成一个参数，记录器尝试执行 `-lc`，退出 127；010c
  按接口拆开参数后退出 0。
- 018：前三个补丁文件 glob 写窄，三个 `sed` 报不存在；019 用完整文件名
  补录。外层脚本因未启用 `set -e` 最终退出 0，stderr 原样保留。
- 024：一次只读 `find .` 范围过宽并进入磁盘等待；025 对明确 PID 发送
  TERM，记 143；随后 026 改为明确目录。
- 030：宿主 `clang` 不在 PATH；这不影响由 GBS buildroot 提供 clang，031
  改为显式 `NOT_AVAILABLE_ON_HOST`。
- 032：首版建议 diff 的 hunk 计数错误，patch 拒绝；035 的最终版本已用
  `--fuzz=0` 通过。

这些均为检查/记录脚本问题，不改变 sandbox、patch、project_config 或结论
判据。记录器外恢复命令补记于 `raw/UNLOGGED_RECOVERY_COMMANDS.md`。

## 11. 最终边界

- sandbox 构建路径：`READY_WITH_STALE_PROVENANCE_METADATA`；
- 四补丁应用：`CONFIRMED_BY_GBS_EXPORT`；
- 层 C 头安装：`CONFIRMED_BY_CMAKE_AND_SPEC`；
- 当前四补丁本地 RPM 提取：`NOT_OBSERVED_RESOURCE_CONCURRENCY`；
- 平台全量编译：`NOT_EXECUTED`（按任务要求）；
- project_config 原件修改：无；
- 平台源码/补丁修改：无；
- Gerrit/LLVM 上游推送：无。
