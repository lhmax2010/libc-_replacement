# R108：tensorflow2 的 GCC 基线对照实验

## 1. 结论

状态：`COMPLETE`。

未应用本次候选改动的精确基线，在相同 armv7l 真实 GCC 条件下同样失败。
R105 候选版与 R108 基线版均使用 GCC 14.2.0，首个失败命令均为
`737/4213` 的同一 XNNPACK AArch32 dot-product/FP16 汇编文件；两边各 48 条
assembler error、737 步完整命令及构建终止块在只去掉耗时前缀和临时脚本名后
逐字节一致。

因此该失败与 libc++ 条件注入及 `elementwise.cc` 的 `std::abs` 修正无关。
六格“全部完整构建成功”的原门禁不适用于这个本来就无法完成的 armv7l GCC
路径。调整后的门禁已通过，候选改动提交并普通推送至既有
`sandbox/lhmax2025/libcxx-migration`：

`6d76139029ca4162249667f0db65a73387a94cfe`

没有使用 force；原基线提交仍是新提交的直接父提交，tensorflow2 远端其余 50
个分支逐项 SHA 未变。

## 2. 对照对象与可比性

### 2.1 源码身份

- 基线 revision：`1cdba73549f741720f11dd53da1bb516b03763a2`。
- 新建 detached worktree：`tmp/R108-tensorflow2-gcc-baseline`。
- `tensorflow/lite/kernels/elementwise.cc` 与该 revision 中的内容 SHA256 均为
  `f852cf60023460453fe531645a439a6fa1120840fa57e6b8eeca503942677e33`。
- 基线 worktree 唯一差异是验证夹具：让 spec 现有的 `USE_CLANG=OFF` 分支在
  armv7l 可达；没有候选的 libc++ BuildRequires/flags，也没有 `std::abs` 修正。

验证夹具与 R105 使用的块逐字节一致，SHA256 都是
`37811ec7681588541122b1a9859ebcd3bd5de07f3a1501591883492f85c7f522`。
该块只存在于 detached 验证 worktree，没有进入提交。

### 2.2 构建条件

| 项目 | R105 候选版 | R108 未改基线 | 可比性 |
|---|---|---|---|
| 架构 | `armv7l` | `armv7l` | 相同 |
| GBS | 2.0.8 | 2.0.8 | 相同 |
| 仓配置 | 根目录 `gbs_llvm.conf` | `progress/T3R/config/gbs_llvm.conf` | 路径不同；内容 SHA256 同为 `8fab16c1...d49cf` |
| 工具链覆盖 | `_toolchain_override gcc` | 同左 | 相同 |
| GCC 夹具 | `r105_validation_force_gcc 1` | 同左 | 相同 |
| release | `105.20.3` | `105.20.3` | 相同 |
| GBS worker | `--threads 1` | 同左 | 相同 |
| 包内并行 | `_smp_mflags -j2` | 同左 | 相同 |
| 调度 | `nice -n 15`、`ionice -c 3` | 同左 | 相同 |
| 清理 | `-C --fail-fast` | 同左 | 相同 |
| build root | `R105-tensorflow2-armv7l-gccactual` | `R108-tensorflow2-armv7l-gccbaseline` | 仅输出隔离路径不同 |
| 待比较变量 | 含候选 spec 与源码修正 | 不含候选改动 | 实验变量 |

R105 使用的根目录配置在本任务开始时已不存在。没有推断或重建它，而是使用
此前归档的逐字节相同副本；R94/R96 已记录两份文件 SHA256 相同。实际 737 步
完整命令也在两轮中逐字节一致，进一步确认最终 compiler flags 没有漂移。

构建前 medium 资源门禁返回 0；磁盘可用约 189 GiB，无其他 `gbs`、`ninja` 或
`rpmbuild` 进程。构建使用单 worker 和 `-j2`，未扩大并发。

尝试从两个 scratch root 的 RPM 数据库枚举包时，两边均返回空清单；因此该
查询不能作为“包集合一致”的证据。可比性依据是固定配置身份、构建参数、实际
编译器身份和失败命令本身，不使用这份空清单。

## 3. 基线构建结果

完整命令见 `raw/015_build_tensorflow2_armv7l_gcc_baseline.command.txt`，完整
stdout/stderr 与退出码同目录保存。结果：

- GBS 退出码：1；
- C 与 C++ compiler identification：GNU 14.2.0；
- 首个失败 Ninja 命令：`737/4213`；因 `-j2`，失败报告之后还能看到已经调度的
  `738/4213`，这不改变首个失败点；
- 失败输入：
  `qd8-f16-qc8w-gemm-4x8c4-minmax-asm-aarch32-neondotfp16arith-cortex-a55.S`；
- 两轮均未执行
  `CMakeFiles/tensorflow-lite.dir/kernels/elementwise.cc.o` 的编译命令。

代表性原始错误如下；两次文本相同：

```text
...cortex-a55.S:119: Error: selected processor does not support `vsdot.s8 q8,q2,d0[0]' in ARM mode
...cortex-a55.S:121: Error: selected processor does not support `vsdot.s8 q9,q3,d0[0]' in ARM mode
...cortex-a55.S:123: Error: selected processor does not support `vsdot.s8 q10,q2,d1[0]' in ARM mode
...cortex-a55.S:288: Error: selected processor does not support `vsdot.s8 q15,q3,d3[0]' in ARM mode
ninja: build stopped: subcommand failed.
error: Bad exit status from /var/tmp/rpm-tmp.NORMALIZED (%build)
```

完整 48 条错误分别在
`artifacts/failure_compare/r105_assembler_errors.normalized.txt` 与
`r108_assembler_errors.normalized.txt`。

## 4. 两次失败的机械比对

规范化只做两项与原因无关的替换：移除 GBS elapsed-time 前缀；把随机
`rpm-tmp.*` 名称替换为固定文本。没有删除 compiler flags、输入文件、行号、
指令或错误正文。

| 对象 | R105 SHA256 | R108 SHA256 | 结果 |
|---|---|---|---|
| 48 条 assembler error | `eabc4738d3fc...e8088ea6` | `eabc4738d3fc...e8088ea6` | 完全一致 |
| 737 步完整命令 | `c53ca77b0191...899e979` | `c53ca77b0191...899e979` | 完全一致 |
| 终止块 | `6a21a90a1877...ff07be3` | `6a21a90a1877...ff07be3` | 完全一致 |

`raw/016_compare_failures.*` 中的三个 `cmp` 均返回 0，最终输出
`NORMALIZED_FAILURE_BLOCKS_IDENTICAL=PASS`。两边 compiler identification
和未到达 `elementwise.cc.o` 的机械门禁见 `raw/018_*`。

## 5. 裁决与调整后的门禁

### 5.1 因果裁决

该失败与候选改动无关，依据是：

1. 未改源码的精确基线可以独立复现；
2. 候选版与基线版的实际编译器、首个失败命令和全部 48 条错误相同；
3. 失败前没有执行候选改动所在的 `elementwise.cc.o`；
4. 失败命令是 XNNPACK C/汇编微内核，与 libc++ 标准库选择无关。

### 5.2 新判据

对这个包采用如下非回归判据：

1. armv7l、aarch64、x86_64 的 libc++ 完整构建和产物/归档核验均通过；
2. aarch64、x86_64 的真实 GCC 完整构建和产物/归档核验均通过；
3. armv7l 真实 GCC 候选版允许停在已独立证明的原有失败，但必须与未改基线在
   编译器、首个失败命令和完整错误序列上相同，并且不得先到达候选源码站点。

这不是把失败改写成成功，而是把该格由“绝对成功门禁”改为“相对基线非回归
门禁”。armv7l GCC 对候选 `elementwise.cc` 的实际编译仍是
`NOT_OBSERVED`，不能写成该文件已由 armv7l GCC 验证。

### 5.3 调整后矩阵

| 架构 | 模式 | 构建/对照 | 标准库/归档证据 | 新判据 |
|---|---|---|---|---|
| armv7l | libc++ | 完整 PASS | `std::__1`；最终 ELF 依赖 `libc++.so.1`；五头文件与 R103 成员核验 PASS | PASS |
| aarch64 | libc++ | 完整 PASS | 同上 | PASS |
| x86_64 | libc++ | 完整 PASS | 同上 | PASS |
| armv7l | GCC | 候选与未改基线同在 737/4213 失败 | 两边 GCC 14.2.0；48 条错误及命令相同；修改源文件未到达 | PASS（基线非回归） |
| aarch64 | GCC | 完整 PASS | `std::__cxx11`；最终 ELF 依赖 `libstdc++.so.6`；R103 成员核验 PASS | PASS |
| x86_64 | GCC | 完整 PASS | 同上 | PASS |

其余五格复用 R105 已归档的完整构建与验证，不重做实验；对应 15 个 build、产物
和归档成员核验退出码均为 0，机械复核见 `raw/022_adjusted_matrix_gate.*`。

## 6. XNNPACK 问题的性质与影响边界

基线 spec 默认 `USE_CLANG="ON"`、`USE_XNNPACK="ON"`；只有验证宏才让
armv7l 进入现有 GCC 分支。该 GCC 分支显式关闭 ARM BF16 和 I8MM，却没有关闭
ARM dot-product/FP16 微内核。最终 XNNPACK 仍生成带 `vsdot.s8` 等指令的
AArch32 汇编，而当前 armv7l GCC/assembler 组合拒绝这些指令。

因此：

- 当前正常 armv7l 包路径由 spec 选择 Clang，R105 的 armv7l libc++ 完整构建
  已通过；本次失败不影响这条当前产品构建路径。
- 如果要求该 revision 支持 armv7l 的完整 GCC 构建，则问题会直接阻止产包；
  没有 armv7l GCC 产物可供判断运行时行为，所以运行时影响是
  `NOT_OBSERVED`。
- 该问题应作为独立的“tensorflow2/XNNPACK armv7l GCC 构建路径”待处理项
  登记。它不属于 libc++/`std::abs` 候选改动，也不应为通过本任务而顺手修改
  XNNPACK 配置。

## 7. 提交、推送与分支核验

- 目标仓：tensorflow2；
- 目标分支：`sandbox/lhmax2025/libcxx-migration`；
- 推送前远端 HEAD：`1cdba73549f741720f11dd53da1bb516b03763a2`；
- 新提交：`6d76139029ca4162249667f0db65a73387a94cfe`；
- 新提交父节点：上述原 HEAD；
- 提交只修改 `packaging/tensorflow2.spec` 与
  `tensorflow/lite/kernels/elementwise.cc`；
- 普通 push 返回 0，没有 force；
- 推送后远端 HEAD 与本地一致，既有提交未丢；
- 远端分支总数仍为 51，去掉目标分支后，推送前后其余 50 条逐项相同。

完整 candidate diff 为 `tensorflow2_candidate.diff`；远端前后快照和机械比较
在 `tensorflow_remote_*.tsv`、`verify_tensorflow_remote.sh` 与 `raw/027_*`。

## 8. 技术性错误、自行判断与尚存疑问

技术性错误均发生在判据执行之外或已修正重跑：

- `raw/009_*`：R105 根目录 `gbs_llvm.conf` 已不存在；改用 SHA256 相同的归档
  副本，`raw/012_*` 对配置身份确认成功。
- `raw/012_*`：进程检测表达式匹配了门禁命令自身；改用精确进程名，
  `raw/013_*` 成功。
- 首次启动 logger 因文件无执行位在 GBS 前返回 126；`raw/014_*` 补记，实际
  构建通过显式 `bash` 启动并记录为 `raw/015_*`。
- `raw/017_*`：第一次“是否到达源码”检索把打包阶段列出的工作树文件也算成
  编译命中；改用 Ninja 对象路径后 `raw/018_*` 成功。
- `raw/020_*`：只读源码检索经 `head` 提前关闭管道，包装器未写退出码；
  `raw/021_*` 用有界 `sed` 重新取得所需 spec 原文并记录该恢复。

自行判断：

1. 编译适配任务继续落在项目仓 `codex/r5-ehabi-diagnosis`，没有污染
   `codex/runtime-validation`。
2. 将“与基线相同的原有失败”定义为 armv7l GCC 格的非回归通过条件，同时明确
   保留 `elementwise.cc` 在该编译器下未观测的覆盖缺口。
3. 将 XNNPACK GCC 失败登记为独立问题，不在本任务修改它。

尚存疑问：产品是否长期要求 tensorflow2 支持 armv7l GCC 完整构建，需要由
后续工作确定；这个产品支持范围问题不影响本次候选改动的因果裁决。

未推送正式分支，未使用 force，未修改 XNNPACK 或其他平台源码。
