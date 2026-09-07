# W1：MLGO / XLA AOT 资产再生成（含最终续跑记录）

## 结论

`PARTIAL`。人工随后授权同步重新生成三架构 XLA runtime，并确认采用
“有效载荷、接口与 ABI 一致”的等价口径。隔离副本中的双标准库开关和
三包兼容组装工具已经准备；AOT 工具构建在修正若干命令自身的环境问题后推进到
`31,594 / 35,555`，但中途资源门禁返回 21（负载过高），已立即中断并清理
本轮进程。尚未生成可用资产，因此符号、接口、载荷及 LLVM 构建验证均为
`NOT_OBSERVED`，W2 仍为 `NOT_AVAILABLE`。

第 1–12 节保留第一阶段与两轮人工确认的历史记录；第 13–17 节是最终续跑
结果，若状态或未观测项与前文不同，以后者为准。

两个指定分支和两个模型输入均已取得并核对，但现有指南、脚本、目标 LLVM spec 与“生成基于 libc++ 的资产”这一目标之间存在多处实质不一致。按任务书“指南与实际材料不符或缺少必要输入即停”的要求，本阶段没有执行 `setup-chroot.sh`、`mlgo-pack`、GBS 构建或任何资产导入。

## 1. 输入身份

仓库：`ssh://lhmax2025@review.tizen.org:29418/platform/upstream/tensorflow2`

| 分支 | 远端与本地工作树 HEAD | 本地只读工作树 |
|---|---|---|
| `sandbox/lhmax2025/mlgo_aot_v3` | `c5c4230a988bec124e83a89d8ffedb8ce07e01ea` | `codes/tensorflow2-mlgo-aot-v3` |
| `sandbox/lhmax2025/mlgo_aot_temp` | `e611ebb4e4dd6d47023f3b06bccc528022aa098f` | `codes/tensorflow2-mlgo-aot-temp` |

两个工作树均以 detached HEAD 检出，状态干净；这样不会改动远端分支或现有 `codes/R104/tensorflow2` 的工作分支。身份记录见 `raw/018_remote_target_refs.*`、`raw/024_v3_identity.*`、`raw/025_temp_identity.*`。

指南：

- 文件：`codes/tensorflow2-mlgo-aot-temp/README.md`
- SHA256：`de6fd78190a7b66f84b1b97209623ea4f2ba00589f6b72d5d7764cfe61edd936`
- 全文归档：`raw/034_guide_full.stdout.txt`

模型：

| 文件 | SHA256 | ZIP 完整性 | SavedModel 入口 |
|---|---|---|---|
| `model_source/inline_model.zip` | `0fdcae0fb686149f72ad2438885978413e876e9b73c10f4f2c01bc26f97a3199` | 通过 | `saved_model/saved_model.pb` |
| `model_source/regalloc_model.zip` | `d7f8e76afed514c42c203a7b614daf80d0059ccb495ac6349fc1c045fc327b12` | 通过 | `model/saved_model.pb` |

证据见 `raw/055_models_sha256.*`、`raw/061_inline_model_test.*`、`raw/062_regalloc_model_test.*`、`raw/074_models_saved_model_entries.*`。

## 2. 对指南所述流程的理解

### 2.1 前置输入

指南要求：

1. 宿主机有 Python 3、`sudo`、`tar`、`nm`、`readelf`；
2. 一个带 Clang、Clang++、`ar` 的 x86_64 GBS buildroot；
3. 在该 buildroot 中安装“支持交叉目标”的 `tensorflow2-aot` x86_64 RPM，其 `/opt/tensorflow-aot/bin/saved_model_cli` 可用；
4. armv7l 与 aarch64 的目标头文件 sysroot；
5. inliner 与 regalloc 两个 SavedModel；
6. 供最终 GBS 验证使用的 LLVM 源码树及 `gbs.conf`。

### 2.2 生成流程

1. 用 `setup-chroot.sh` 安装 AOT RPM、检查工具链、布置两个交叉 sysroot、挂载 `/proc`，并确认 Clang 含 ARM/AArch64 后端；
2. 分别对 inliner、regalloc 模型调用 `mlgo-pack --model ... --type ...`；一个 x86_64 buildroot 一次可生成 armv7l、aarch64、x86_64 三架构结果；
3. TensorFlow 升级时另用 `mlgo-pack --make-runtime` 交叉编译 34 个 XLA runtime 源文件；
4. 工具自动检查 ELF 架构、armv7l softfp ABI、模型入口符号和 5 个必须嵌入的 runtime 对象；
5. 默认随后运行 `verify-gbs.sh`，按架构串行构建 LLVM。也可在生成时用 `--no-verify`，之后单独运行验证脚本。

### 2.3 指南声明的输出

共 9 个 tarball：

- `mlgo_runtime_{armv7l,aarch64,x86_64}.tar.gz`；
- `mlgo_inliner_{armv7l,aarch64,x86_64}.tar.gz`；
- `mlgo_regalloc_{armv7l,aarch64,x86_64}.tar.gz`。

模型包应含对应的 `.h`、`.o` 和 manifest；runtime 包应含 5 个嵌入对象、TF include tree、`libtf_xla_runtime_prebuilt.a`、CMake stub、许可证和 manifest。

### 2.4 指南可确认的时间和资源要求

- 三架构由一个 x86_64 buildroot生成，不要求三套本机构建环境；
- `verify-gbs.sh` 按架构串行；低于 6 GiB 可用内存拒绝启动；脚本按约 3 GiB/任务计算并行度，范围为 2 至 6；
- AOT spec 注释说明：Bazel 输出缓存若被清理，完整 TensorFlow 重建约 4 小时；
- aarch64 spec 的分析阶段单次超时为 90 分钟，最多尝试 3 次；
- 指南没有给出两个模型、runtime 和三架构 LLVM 验证的总耗时，因此总耗时记为 `NOT_AVAILABLE`，不能自行补值。

本机门禁时有约 21 GiB 可用内存、20 个逻辑 CPU、约 185 GiB 可用磁盘；两个工作树检出后 `df -h` 显示约剩 182 GiB。资源门禁通过，但这不消除下述材料/机制阻断。

## 3. 实际材料核对与阻断点

### 3.1 当前工具链仍明确生成 libstdc++ ABI 的 runtime 对象

`fetch_sysroot.py` 的目标包清单明确下载 `libstdc++-devel` 与 `libstdc++`；`mlgo_pack.py` 从 GCC 安装目录寻找 libstdc++ 头文件，并把这些目录用 `-isystem` 传给 Clang。runtime 编译参数没有 `-stdlib=libc++`，也没有等价的 libc++ include/sysroot 选择机制。

证据：`raw/047_mlgo_pack_py_full.stdout.txt`、`raw/049_fetch_sysroot_full.stdout.txt`、`raw/088_temp_stdlib_controls.stdout.txt`。

这与 W1 的目标“生成基于 libc++、符号为 `std::__1` 的资产”直接矛盾。按现状执行，不能据此期待产出 `std::__1`；因此未执行。

### 3.2 AOT RPM spec 固定使用 GCC

`mlgo_aot_v3` 的两个 AOT spec 均含：

```spec
BuildRequires:  gcc
BuildRequires:  gcc-c++
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
export BAZEL_COMPILER=gcc
```

证据：`raw/056_v3_aot_spec_full.stdout.txt`、`raw/057_v3_aot_aarch64_spec_full.stdout.txt`、`raw/089_v3_compiler_controls.stdout.txt`。

因此它也没有提供“用 libc++ 构建 AOT 工具”的明确路径。

### 3.3 指南要求的交叉支持开关没有在 AOT 构建命令中启用

指南要求 AOT RPM 的 spec 带 `--define=with_cross_compiler_support=true`，并引用相邻文件 `../mlgo-tf-aot-cross.diff`。实际情况：

- 两个 spec 的 Bazel 命令均未出现该 `--define`；精确检索退出码为 1（零匹配），见 `raw/076_cross_define_enable_search.*`；
- 源码中存在名为 `with_cross_compiler_support` 的 Bazel `config_setting`，但“存在条件定义”不等于“构建命令启用了该条件”，见 `raw/071_cross_define_exact_search.*` 与 `raw/077_cross_setting_context.*`；
- 指南引用的 `mlgo-tf-aot-cross.diff` 在当前两个工作树及其相邻候选位置均不存在，temp 分支历史中也没有该文件，见 `raw/044_parent_reference_presence.*`、`raw/059_temp_cross_diff_git_search.*`。

当前不能确认由 v3 spec 构建的 RPM满足指南所要求的三目标 AOT 前提。

### 3.4 指南声明的预置 sysroot 缓存不存在

指南称该目录约 75 MiB，主要是 `build/sysroot-cache/`；实际 temp 工作树约 800 KiB，`build/sysroot-cache/` 不存在，且 `/build/` 被 `.gitignore` 排除。`setup-chroot.sh` 可在缓存缺失时从网络下载，但“改为在线下载”属于输入来源和可复现性变化，本阶段未自行采用。

证据：`raw/028_worktree_sizes.*`、`raw/036_temp_file_inventory.*`、`raw/039_temp_gitignore.*`、`raw/045_sysroot_cache_presence.*`。

### 3.5 指南的 9 包接口与当前 LLVM spec 的 3 包接口不一致

指南称 9 个输出“名称与 llvm.spec 的 Source1002–1010 完全一致”。当前可得两份 LLVM spec 均只有：

```spec
Source1002: mlgo_arm_model.tar.gz
Source1003: mlgo_aarch_model.tar.gz
Source1004: mlgo_x86_model.tar.gz
```

现有三个旧包确实存在，且其中 runtime 符号为 `std::__cxx11`。没有 Source1005–1010，也没有指南所述的 9 个文件名。

证据：`raw/087_expected_output_existing.*`、`raw/090_target_llvm_mlgo_sources.*`、`raw/091_old_asset_hashes.*`、`raw/092_old_assets_symbol_samples.*`、`raw/095_target_spec_source_count.*`。

### 3.6 默认目录布局不能直接执行验证

`verify-gbs.sh` 固定取工具目录的相邻 `../llvm` 和 `../gbs.conf`。当前布局中：

- 相邻 `codes/llvm` 存在，但在 `tizen_base` 上 ahead 2 / behind 3，不是 R104 的迁移工作树；
- 实际迁移工作树是 `codes/R104/llvm`，其 `packaging/llvm.spec` 还有本地未提交改动；
- `codes/gbs.conf` 不存在。

证据：`raw/050_verify_gbs_full.stdout.txt`、`raw/069_llvm_candidates_identity.*`、`raw/073_expected_sibling_inputs.*`。

### 3.7 验证脚本的自动并行度与本任务硬上限冲突

以门禁时约 21 GiB 可用内存计算，`verify-gbs.sh` 会为 x86_64 选择 6 个任务、为两种交叉架构选择 5 个任务；本任务总则要求并行度 2，并要求 `nice -n 15` + `ionice -c 3`。现有脚本使用 `nice -n 10`，没有 ionice，也没有可直接指定 job=2 的命令行参数。因此不能原样运行且同时满足本任务资源纪律。

## 4. 已核对可用的部分

- 两个远端分支存在，本地 HEAD 与远端精确一致；
- 两个模型 ZIP 结构完整，均能定位唯一的浅层 `saved_model.pb`；
- 本机有至少两个现成 x86_64 GBS buildroot，均含 `clang`、`clang++`、`ar`，但均不含 `/opt/tensorflow-aot/bin/saved_model_cli`；
- 旧三架构资产可读取，且可独立复现其 `std::__cxx11` 符号问题；
- 磁盘和内存门禁本身满足开始轻量准备工作的条件。

## 5. 待确认后的拟执行计划（未执行）

以下计划只有在第 6 节问题得到确认、并提供一致的 libc++ 生成路径后才执行：

1. 在 `tmp/` 建立隔离的生成目录，保留两个 `codes/` 工作树只读；
2. 用经确认的 spec/revision 构建并校验交叉 AOT RPM；
3. 在选定的干净 x86_64 buildroot 中安装 RPM并布置经确认的 libc++ 目标 sysroot；
4. 先用单架构小样确认生成对象出现 `std::__1` 且不出现 `std::__cxx11`，以此作为方法阳性门禁；
5. 再生成 inliner、regalloc、runtime 的三架构完整集合；
6. 逐文件记录 SHA256、ELF machine、armv7l softfp、入口符号、5 个 runtime 对象和新旧接口集合差异；
7. 按经确认的 LLVM spec 文件布局导入隔离副本，并以并行度 2、`nice -n 15`、`ionice -c 3` 完成三架构 GBS 验证；
8. 只有全部核验符合目标，才进入 W2。

## 6. 第一轮停止时的问题（历史记录；处理状态见第 10–11 节）

问题全文同时记录在 `../BUILD_QUESTIONS_0905.md`：

1. 哪个 revision 或补丁提供真正的 libc++ 生成路径？现有脚本与 AOT spec 明确使用 libstdc++/GCC，不能满足 W1 目标。
2. 指南引用的 `mlgo-tf-aot-cross.diff` 是否漏交？若不是，v3 spec 应通过什么已存在机制启用 `with_cross_compiler_support=true`？
3. 缺失的预置 `build/sysroot-cache/` 应由人工补齐，还是允许脚本从所写的滚动 `reference` URL在线重取？
4. 应以哪份 LLVM revision/spec 为导入与验证目标？指南要求 9 个 Source1002–1010，但当前迁移分支仍消费 3 个旧的合并包。
5. 应提供哪份 `gbs.conf`，以及如何在不修改归档脚本判定逻辑的前提下满足 job=2、nice=15、ionice idle 这三个资源硬要求？

## 7. 自行判断与未观测项

### 自行判断

- 为避免两次下载约 180 万个 Git 对象，复用已有 `codes/R104/tensorflow2` 对象库，以 detached worktree 取得两个指定远端 SHA；没有改动源码内容。
- 将指南所称“spec 必须带 `--define`”按 Bazel 的实际条件机制解释为“构建命令必须显式启用该 define”；仅存在 `config_setting` 不足以证明已启用。
- 将当前脚本明确选择 libstdc++ 头文件且无 libc++ 开关，判定为目标机制不成立，而非可在执行后解释的普通警告，因此按门禁停止。

### 未观测

- AOT RPM构建：`NOT_OBSERVED`；
- `setup-chroot.sh` 执行：`NOT_OBSERVED`；
- 三架构资产生成：`NOT_OBSERVED`；
- 新资产 `std::__1` / `std::__cxx11` 符号：`NOT_OBSERVED`；
- LLVM 三架构 GBS 验证：`NOT_OBSERVED`。

## 8. 命令技术性异常

- 最初按不存在的 `codes/tensorflow2` 查询，命令以 128 退出；随后仅修正为实际已有的 `codes/R104/tensorflow2`，未改变任何判据。见 `raw/008_*` 至 `raw/017_*`。
- 三个范围过大的只读 `find`（`raw/066_*` 至 `raw/068_*`）耗时异常，已终止其具体子进程并改用明确路径/`rg` 检查；其 shell 管道最终写下 0，但结果不作为结论依据。终止命令见 `raw/070_stop_broad_searches.*`。
- `raw/076_cross_define_enable_search.*` 的退出码 1 是 `rg` 的“零匹配”判据结果；`raw/086_temp_v3_relation.*` 的退出码 1 表示两个分支无共同 merge-base，均非脚本故障。

## 9. 边界确认

- 未生成或覆盖任何 MLGO/XLA 资产；
- 未修改任何平台源码、spec、配置或补丁；
- 未向 Gerrit 或任何外部源码仓推送；
- 未切换或修改 `codex/runtime-validation`；
- W2 因 W1 没有可用资产而记 `NOT_AVAILABLE`；W3 已独立执行，见 `docs/progress/BUILD_W3_0905/`。

## 10. 人工答复后的续查（最终状态更新）

人工已授权：由本项目增加默认兼容的标准库开关；缺失 sysroot 缓存时从脚本中的 Tizen `reference` 地址下载；使用 `config/gbs.conf`；把验证脚本资源限制改为 job=2、`nice -n 15`、`ionice -c 3`；模型只选择 inliner。续查没有改动源码或生成资产。

### 10.1 交叉支持机制的更正

早先“没有等价交叉支持内容”的表述不完整。继续检查隐藏文件后确认，`.bazelrc` 已定义 `build:cpu_cross --define=with_cross_compiler_support=true`，`release_base` 也引用 `--config=cpu_cross`；嵌套 XLA 配置有同类内容。但两个 AOT spec 的实际 Bazel 命令既没有 `--config=cpu_cross`，也没有直接传入该 define。因此代码和等价配置存在，未解决的是 spec 没有消费它。证据见 `raw/117_*`、`raw/119_*`、`raw/120_*`。

### 10.2 inliner 范围不能解除当前阻断

逐对象核对现有三架构合并资产得到：

| 架构 | `InlinerSizeModel.o` 中 `std::__cxx11` / `std::__1` | `RegAllocEvictModel.o` 中 `std::__cxx11` / `std::__1` | 整包 `std::__cxx11` | 整包 `std::__1` |
|---|---|---|---:|---:|
| aarch64 | 0 / 0 | 0 / 0 | 44 | 0 |
| armv7l | 0 / 0 | 0 / 0 | 26 | 0 |
| x86_64 | 0 / 0 | 0 / 0 | 26 | 0 |

整包的 ABI 敏感符号全部位于 XLA runtime 归档/对象；新指南的 `mlgo_inliner_<arch>.tar.gz` 只含模型头、模型对象和 manifest，不含 runtime。证据见 `raw/123_*`、`raw/124_*`、`raw/125_*`。

因此，只生成三个 inliner 包不能替换携带 `std::__cxx11` 的 runtime，无法解除当前 LLVM 链接阻断。继续生成会得到一个可检查的部分产物，但不会满足 W1 的目标；这属于新的目标矛盾，故停止 W1，没有用不完整资产进入 W2。

### 10.3 默认产物“完全相同”的口径问题

生成脚本会写入 manifest 时间戳，tar 封装也带元数据；在没有可复现构建约束时，即使有效载荷相同，压缩包逐字节哈希也可能不同。因此需要人工明确“完全相同”是指字节级一致，还是载荷、接口、ABI 和有效内容一致（排除时间戳与封装元数据）。本轮未自行选择口径。

## 11. 需与开发人员确认的事项

| 项目 | 指南或材料的说法 | 实际情况 | 本轮临时处理 | 需确认内容 |
|---|---|---|---|---|
| 交叉支持 | 指南引用 `mlgo-tf-aot-cross.diff` 并要求启用交叉 define | diff 不存在；`.bazelrc` 有等价 `cpu_cross`，AOT spec 未引用 | 记录现状，未改 spec | 正式 spec 应加 `--config=cpu_cross` 还是使用其他入口 |
| sysroot 缓存 | 指南称目录内有约 75 MiB 缓存 | 当前分支没有且 `build/` 被忽略 | 已获准未来从 `reference` 地址下载；本轮未下载 | 指南是否应固定快照与校验值以保证复现 |
| 验证资源 | 脚本自动使用 5/6 jobs、`nice -n 10`、无 ionice | 与 job=2、`nice -n 15`、`ionice -c 3` 冲突 | 已获准未来只改资源参数；本轮未运行 | 是否为脚本增加正式资源参数接口 |
| 资产拆分 | 指南产出 9 包，LLVM spec 消费 3 个合并包；本轮只选 inliner | ABI 阻断在 runtime，不在 inliner/regalloc 模型对象 | 未修改 LLVM spec，未拼装不完整资产 | 三包消费接口如何适配，以及是否允许同步重生 runtime |

## 12. 最终未观测项

- 兼容标准库开关的实现与默认路径基线对照：`NOT_OBSERVED`；
- sysroot 在线下载：`NOT_OBSERVED`；
- 三架构 inliner/runtime 资产生成：`NOT_OBSERVED`；
- LLVM 三架构 GBS 验证：`NOT_OBSERVED`。

## 13. 最终授权后的实施范围

人工最终确认：

- 同步重新生成 XLA runtime 的 armv7l、aarch64、x86_64 三架构资产；
- 模型采用 inliner 三架构；regalloc 本轮不重新生成；
- 默认 libstdc++ 路径必须保持现状，显式开关才选择 libc++；
- 新旧比较采用“有效载荷、接口与 ABI 一致”，不要求含时间戳和 tar 元数据的
  压缩包逐字节一致；
- 接口符号缺失或新增必须逐项申报，新对象必须以 `std::__1` 为 ABI 证据；
- 缺失的 sysroot 缓存可从脚本所列 Tizen `reference` 地址在线获取；
- 缺失的交叉支持 diff、指南的 sysroot 缓存遗漏和验证脚本资源参数冲突均继续
  记录，但不再作为开始实施的阻断。

为兼容当前 LLVM spec 的三个合并包接口，新增了
`code/assemble_combined_assets.py`：每个架构以新 runtime、新 inliner 和旧
regalloc 组装 `mlgo_{arm,aarch,x86}_model.tar.gz`。该工具仅准备并通过语法
检查（`raw/150_helper_syntax_and_help.*`）；因没有生成输入，未实际运行，
组装结果为 `NOT_OBSERVED`。

## 14. 隔离副本中的实现（未修改 `codes/`）

所有实验性修改均位于 `tmp/BUILD_W1_0905/`，没有写入只读的 `codes/`：

1. `fetch_sysroot.py` 增加 `--stdlib {libstdc++,libc++}`，默认
   `libstdc++`；按选择下载并核验相应开发包和头文件。
2. `setup-chroot.sh` 增加同一开关，并为两种标准库使用相互隔离的 sysroot
   缓存与 buildroot 路径。
3. `mlgo_pack.py` 增加同一开关；libc++ 路径显式使用
   `-stdlib=libc++ -nostdinc++` 与 libc++/libc++abi 头文件，默认路径保留
   原有 libstdc++ 查找逻辑。
4. 隔离的 `tensorflow2-aot.spec` 在 Bazel 调用中加入现有
   `--config=cpu_cross`，没有自行补写缺失的 diff。

完整 diff 与静态检查见 `raw/136_switch_diff_static_checks.*`。静态检查只能证明
开关存在、默认值与代码路径按设计保留，不能替代“默认输出与现状等价”的产物
实测；该实测因 AOT 工具未构建完成而为 `NOT_OBSERVED`。

## 15. AOT 工具构建过程与停止点

### 15.1 命令自身的技术性问题

这些非零均发生在生成资产所需的 AOT 工具构建阶段，未产出可供 LLVM 使用的
资产；按“命令自身技术性错误可修正并申报”的纪律处理：

| 记录 | 结果 | 现象与修正 |
|---|---:|---|
| `raw/138_aot_build.*` | 1 | GBS/Bazel 执行 LLVM overlay 时超时；保留已取得的外部仓数据后作一次增量重试。 |
| `raw/139_aot_build_retry.*` | 1 | Bazel 报 `@ruy: download is disabled`；`raw/140_*` 证明所需 SHA256 为 `a22c42e80c7bb450db8492728e4742ee66f46d5458c45fe67ce2c9b61240630c` 的缓存文件实际存在且校验一致，判定为 Bazel 外部仓状态问题，而非输入缺失。 |
| `raw/142_aot_build_clean_bazel_state.*` | 1 | 隔离执行继承宿主 `HOME`，RPM 将 `%{_topdir}` 解析到无权限路径；改为 buildroot 内的 `/home/abuild`。 |
| `raw/143_aot_build_clean_bazel_state_home.*` | 1 | 继承宿主 Snap 的 `LOCPATH`，导致 Go SDK 解包过程的 UTF-8 locale 转换错误；移除该宿主变量，并在 buildroot 中明确设置 UTF-8 locale。 |
| `raw/145_aot_build_clean_state_utf8.*` | 130（恢复记录） | 修正后的命令成功推进到 `31,594 / 35,555`。资源门禁返回 21 后人工中断；退出码文件为依据统一执行会话返回值补记，非包装器自动写入，详见同名 `RECOVERY_NOTE.md`。 |

`raw/145` 的命令明确限制 `_smp_mflags -j2` 和 `aot_build_jobs 2`，并由
`nice -n 15`、`ionice -c 3` 的日志包装器启动。该次构建从约 11:48 运行至
17:13，标准错误完整保留（约 1.24 MiB）。

### 15.2 资源门禁与收尾

开工门禁与构建前门禁均通过：可用内存约 20–21 GiB、负载约 5、磁盘可用
约 182 GiB，见 `raw/131_*` 与 `raw/137_*`。17:13 的中途门禁结果为：

```text
mem_available_kib=20027912
load1=32.14
load_limit=20.000000
gate_result=EXCESSIVE_LOAD
```

退出码为 21，见 `raw/146_midbuild_resource_gate.*`。因此没有等待构建自然结束，
立即发送中断。残留 Bazel server 对 `TERM` 未及时退出；只对已精确核对属于
本轮的 PID 发送了强制终止信号。`raw/147_*`、`raw/148_*` 的即时轮询仍返回 1，
随后 `raw/149_build_process_cleanup_verification.*` 确认本轮 AOT 的 Bazel、
rpmbuild、编译器或 Ninja 进程均已不存在。未删除 buildroot、缓存或中间结果，
以便人工决定是否续跑。

## 16. 产物与验证结论

| 要求 | 结果 | 说明 |
|---|---|---|
| AOT 工具构建完成 | `NOT_OBSERVED` | 在 31,594 / 35,555 时因资源门禁中止 |
| 默认 libstdc++ 输出与现状等价 | `NOT_OBSERVED` | 仅完成开关静态检查，未生成对照资产 |
| libc++ XLA runtime（三架构） | `NOT_OBSERVED` | 未进入生成阶段 |
| libc++ inliner（三架构） | `NOT_OBSERVED` | 未进入生成阶段 |
| 新对象为 `std::__1` 且无错误的 `std::__cxx11` | `NOT_OBSERVED` | 无新对象可查 |
| 接口符号逐项对照 | `NOT_OBSERVED` | 无新资产可比 |
| 模型有效载荷一致 | `NOT_OBSERVED` | 无新资产可比 |
| 文件数量与结构一致 | `NOT_OBSERVED` | 无新资产可比 |
| 三个 LLVM 合并资产 | `NOT_OBSERVED` | 组装工具未执行 |
| W2 LLVM / bcc-tools / bpftrace | `NOT_AVAILABLE` | W1 未提供经核验可用资产 |

因此不能给出 `ASSETS_REGENERATED`，也不能把已缓存的旧资产或未完成构建当作
可用结果。最终结论为 `PARTIAL`。

## 17. 自行判断、疑问与范围边界

### 自行判断

- 将 GBS 第一次 overlay 超时和随后 Bazel 外部仓缓存状态异常视为命令/构建
  环境的技术性问题，而不是资产内容判据失败；保留缓存后修正执行环境继续。
- 为避免继续触发宿主资源问题，在中途门禁出现明确的 21 后立即停止，即使
  AOT 工具已完成约 88.9% 的动作。
- 使用旧 regalloc 参与未来三包兼容组装，因为人工明确本轮不重生 regalloc，
  且此前逐对象检查未在两个模型对象中发现 `std::__cxx11`/`std::__1`；最终
  组装仍须在生成成功后做完整接口对照，当前不据此推断产物可用。

### 尚存疑问与需开发人员确认

- 正式 AOT spec 应以何种方式消费已有 `cpu_cross` 配置；本轮只在隔离副本
  临时加入该配置。
- 指南是否应固定 sysroot 快照与包校验值，而非仅指向滚动 `reference`。
- 验证脚本是否应提供正式的 job、nice、ionice 参数接口。
- 9 个生成包到 LLVM 现有 3 个合并包的组装方式是否应成为正式工具；本轮的
  helper 尚未用真实新资产验证。
- AOT 打包所带离线仓缓存是否应补全/清理 Bazel repository 状态，以免同一
  完整缓存被 `--experimental_repository_disable_download` 拒绝使用。

### 时间与边界申报

本次最终续跑从 09:53 门禁到 17:16 清理核验约 7 小时 23 分；加上前文已
记录的第一阶段约 2 小时，W1 跨多轮的累计主动工作时间估算超过原任务的
8 小时上限。最终停止的直接原因是资源门禁 21，而不是计时器。本报告如实
保留该偏差，不把它解释为完成。全过程没有修改 `codes/`、平台源码、LLVM
消费 spec 或 Gerrit 分支，也未推送任何外部源码仓。

提交前自检的 `raw/151_material_selfcheck.*` 还发生一次命令自身的引号错误：
Markdown 反引号被 shell 当作命令替换，因而该轮状态文本检查无效；原始输出未
修改，原因记录在同名 `RECOVERY_NOTE.md`，并由
`raw/152_material_selfcheck_corrected.*` 以修正后的命令重跑通过。
`raw/153_material_sha256.*` 是报告定稿前的初步清单；
`raw/154_final_material_sha256.*` 记录报告定稿后的材料 SHA256。
