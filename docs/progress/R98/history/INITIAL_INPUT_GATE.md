# R98 输入门禁：等待人工指定迁移后 LLVM 镜像

状态：`BLOCKED_INPUT_IDENTITY`

## 已找到的候选

1. `Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647` +
   `Tizen-Unified-Toolchain/tizen-unified-toolchain_20260829.015247`
   - 是可验证的成对候选；后者有六个设备镜像清单。
   - 但实际构建日志显示抽查的 `bcc-tools`、`bpftrace`、`binutils`、
     `gdb`、`llvm` 等均由 Clang 22.1.8 构建。
   - 仓库元数据显示 Unified 中 311 个产出 C++ runtime 依赖的源码包仍直接
     依赖 `libstdc++.so.6`，没有一个直接依赖 `libc++.so`/`libc++abi.so`。
   - 因而它不符合本任务所描述的“迁移后大多数包使用 libc++、少数 GCC 包
     形成孤岛”的目标状态，不能作为 R98 的最终输入。

2. `Tizen-Base-Toolchain/tizen-base-toolchain_20260901.103155`
   - 这是线上目前比上述 Base 更新的候选，基于 20260828 快照重建；包含
     `libcxx-runtimes` 的三架构构建记录。
   - 该项目没有 `images/`，线上也未找到以它为 base 的更新版
     `Tizen-Unified-Toolchain` 快照；当前最新 Unified 仍是
     `tizen-unified-toolchain_20260829.015247`。
   - 因而它本身不足以回答镜像归属、双向依赖和迁移后运行时边界。

3. 两项目的 `reference/`
   - 当前在线目录时间分别对应旧的 Base/Unified 状态，且不是上述 9 月 1 日
     Base 与一个迁移后 Unified 镜像的成对输入。

## 需要人工提供或确认

请指定迁移后 LLVM Unified 镜像的精确快照名/URL（以及与之配套的 Base
快照），或给出其本地挂载路径。若目标尚未发布为 download.tizen.org 快照，
请提供 QuickBuild 产物/仓库/镜像清单的可读位置。

在获得该身份前，不继续把旧 GCC-world/过渡态数据外推到迁移后镜像。

## 证据位置

- 候选与在线目录：`raw/020_list_current_toolchain_snapshots.*`
- 本地/在线引用关系：`raw/021_inspect_current_reference_and_candidate.*`
- 新 Base 状态：`raw/022_inspect_new_base_snapshot.*`
- 旧候选元数据汇总：`raw/015_summarize_metadata_signals.*`
- 旧候选实际编译器证据：`raw/016_fetch_target_buildlogs.*`、
  `raw/017_inspect_compiler_markers.*`、`raw/018_verify_runtime_flags_in_logs.*`

说明：R95 表格仅作为 GCC 世界的比较基准和分类方法复用，未作为本任务的
迁移后事实证据。
