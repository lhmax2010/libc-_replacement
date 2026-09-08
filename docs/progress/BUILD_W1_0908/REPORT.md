# W1：TensorFlow 2.18 MLGO 资产接入与 LLVM 适配

## 当前结论

结论为 `PARTIAL`，不得据此推送。

三架构资产的静态门禁和三架构 CMake 配置门禁已通过；armv7l、x86_64
的 libc++ 全量构建及产物检查通过，x86_64 原生的 release-mode 模型
实际执行也已通过。aarch64 全量构建在 7,376/7,545 时因时间上限中止；
三架构 GCC/非 libc++ 兼容格、armv7l 物理板及 aarch64 QEMU 的模型实际
运行均未完成。因此硬门禁没有闭合，LLVM 候选改动未提交、未推送。

## 输入身份

基线仓库为 LLVM 包既有 `sandbox/lhmax2025/libcxx-migration`，基线提交：
`2d23367d74afbf2bb1e9e4013fce072b3a154109`。实施使用独立 detached
worktree：
`tmp/BUILD_W1_0908/llvm-worktree`；`codes/` 未作修改。

新增 TensorFlow 2.18/libc++ 资产：

| 架构 | 文件 | SHA256 |
| --- | --- | --- |
| armv7l | `mlgo_arm_model_libcxx_tf218.tar.gz` | `3c0a04d193eb4b3a601036a6ee7217303fddb44bca5adcd13c67066c37a1c87c` |
| aarch64 | `mlgo_aarch_model_libcxx_tf218.tar.gz` | `d50d874bab843ea96c3a282d34f172b87a16aefe5aa8309871e4d328c8b2f17c` |
| x86_64 | `mlgo_x86_model_libcxx_tf218.tar.gz` | `858031bbfddf385ac859320f40f91dd0cf51bd1d4414b5cd55e36308ab616636` |

## 接入改动

候选改动只涉及 `packaging/llvm.spec` 和三个新资产文件；没有修改上游
C++ 源码。

1. 仅当 RPM 已定义 `_toolchain` 且 `toolchain_is clang` 时定义
   `build_with_libcxx`；该分支选用 TensorFlow 2.18/libc++ 资产，并启用
   `LLVM_ENABLE_LIBCXX=ON`。
2. GCC 覆盖路径和未定义 `_toolchain` 的非迁移配置仍选择原来的
   TensorFlow 2.15/libstdc++ 资产。
3. libc++ 分支声明 `libc++-devel` 与 `libc++abi-devel` 构建依赖，并在
   链接器参数中显式保留 `libc++abi`。
4. 没有改动补丁内的两个 CMake 扩展。原因是新归档仍提供 CMake 当前
   消费的 `InlinerSizeModel.h`、`InlinerSizeModel.o` 和五个相同文件名
   的 XLA runtime 对象；三架构配置探针均验证了这些现有入口可以原样
   解析、选中并生成 `tf_xla_runtime` 目标。这是本任务中的自行判断；
   后续完整链接与模型运行是其行为门禁，不能由文件名相同直接替代。

## 资产静态核查

归档核查脚本快照：
`code/verify_mlgo_asset_archive.sh`，SHA256
`bf2fc7cb91cf922015f667d10b9fdc4c3b1dafb5a14aba37161b2c2c595c3bef`。

| 架构 | 目标格式 | `std::__1` 命中 | `std::__cxx11` 命中 | 结果 |
| --- | --- | ---: | ---: | --- |
| armv7l | ARM EABI5 | 120 | 0 | PASS |
| aarch64 | AArch64 | 122 | 0 | PASS |
| x86_64 | x86-64 | 122 | 0 | PASS |

三份归档均含模型头/对象、五个约定的 runtime 对象、sysroot 与许可文件。
逐对象证据见 `asset_checks/`。

## 2.15 与 2.18 接口差异

x86_64 的外部定义符号对照为：旧 76、新 175、共同 44、旧独有 32、
新独有 131。模型入口由旧的长路径派生名字改为
`_xla_InlinerSizeModel_llvm_InlinerSizeModel`；LookupArg/Result 的字符串
ABI 从 `std::__cxx11` 变为 `std::__1`。完整集合和头文件 diff 见
`interface/` 与 `INTERFACE_DELTA.md`。

新增/缺失符号不能单凭数量判定兼容；本轮采用同一 2.18 生成物中的
头文件与对象成对接入，最终以 LLVM 完整链接以及
`ReleaseModeModelRunner` 实际构造和执行作为契约门禁。

## 构建与产物核查

三架构配置探针均确认：使用 Clang 22.1.8，选择正确架构的 2.18 资产，
模型对象及五个 runtime 对象的机器类型正确，现有 CMake 消费入口成功
建立。探针通过后按设计显式退出，因此外层 GBS 的退出码 1 是探针的
预期终止，不是配置失败；解释和完整日志见 `raw/010`—`012` 及
`build_logs/configure_probe_*.log`。

armv7l libc++ 全量构建：

- GBS/abuild 最终退出码 0，7,147 个 Ninja 步骤及 RPM 打包完成；
- `libLLVM.so.22.1` 在此前阻断点成功链接并入包；
- 动态依赖含 `libc++.so.1`、`libc++abi.so.1`、`libgcc_s.so.1`，不含
  `libstdc++.so`；
- 库内存在新模型入口以及
  `LookupArgIndex/LookupResultIndex(std::__1::basic_string...)`，
  `std::__cxx11` 命中为 0；
- 五处 libc++ 头文件检查全部命中。

完整构建日志为 `build_logs/full_build_armv7l.log`，SHA256：
`ede9ee72579b4aacd99e8ca2027e1be4677e6188a0468f0defc6178f724516ea`。
产物检查见 `product_checks/armv7l/`。

产物检查脚本首次将 `cxxabi.h` 错拼到 `libcxx/include`，在前四项成功后
以退出码 2 停止。实际文件位于 `libcxxabi/include`；仅修正该取证路径、
不改变匹配和判据后重跑通过。原始失败与修正说明保留在 `raw/023`，
重跑记录在 `raw/024`。

构建日志还持续报告 GNU Build ID 缺失；这是既有发布注意事项，本任务
没有修复或隐藏。HAL rootstrap checker 对 SOURCES 中额外 spec 的查找也
产生告警，但最终 RPM 成功生成，退出码为 0。

x86_64 libc++ 全量构建同样以退出码 0 完成 7,633 个 Ninja 步骤和
RPM 打包。产物为 x86-64 ELF，依赖 `libc++.so.1`、`libc++abi.so.1`、
`libgcc_s.so.1` 而不依赖 libstdc++；新模型入口、LookupArg/Result 的
`std::__1` 符号和五处头文件检查全部通过，`std::__cxx11` 为 0。
完整日志 `build_logs/full_build_x86_64.log` 的 SHA256 为
`51b78b3c34be30ba66f48a395f7f8110594e9d54d421ce92ccd4e8a6296182f3`。

x86_64 产物脚本第一次因 arm 使用 `build/lib`、x86_64 使用
`build/lib64` 而在文件存在性断言处退出 1；增加仅限这两个标准目录的
解析后，原判据重跑通过。跟踪与解释见 `raw/031`—`033`。

最终产物检查脚本 `code/verify_llvm_libcxx_build.sh` 的 SHA256 为
`172c50ef03044b159bcd14e9b9520f9c6c256297e8db14d8d5349ad596806f83`。
为避免向项目仓提交约 68 MiB 的重复文本，armv7l/x86_64 的完整动态符号
输出以 gzip 无损压缩保存为 `libllvm_symbols.txt.gz`；检查摘要仍为纯文本。

## MLGO 实际运行

脚本快照 `code/verify_mlgo_release_mode.sh`（SHA256
`839604338a798455f227623219e9e1d2227453e16b4aea049c8a9bb0ff8eac48`）使用
`-enable-ml-inliner=release`，并读取 LLVM 原生 `inline-ml` YAML
optimization remarks；其中同时记录输入特征和 `ShouldInline` 决策。
x86_64 原生执行中默认与 release 子进程均以 0 退出，YAML 有 6 条
`Pass: inline-ml` 和 6 个 `ShouldInline`，输出 IR 再次通过 verifier。
这证明实际构造并执行了 release-mode AOT advisor，而不是默认启发式。

最初使用的附加 `print<inline-advisor>` pass 在模型已经完成多轮决策后，
于 `MLInlineAdvisor::print` 访问已删除函数并段错误（release 子进程
退出 139）。不修改 LLVM；改用上述已有 optimization remark 机制后
完整执行通过。原始崩溃栈、替代探针和最终运行结果均保留在
`runtime_checks/x86_64/`，方法说明见 `MLGO_RUNTIME_OBSERVATION_NOTE.md`。

armv7l 物理板三次连接尝试均不可达，`devices` 列表为空，最终只读
`shell true` 明确退出 1。没有部署、创建或删除任何板端文件；按纪律
不再重试，该运行格为 `NOT_OBSERVED`。

aarch64 libc++ 全量构建实际使用 Clang 22.1.8，并已进行到 Ninja
7,376/7,545；确认已超过 W1 时间边界后主动中止。GBS 顶层退出码为 143，构建
日志随后明确记录 `ninja: build stopped: interrupted by user`，所以这不是
候选代码触发的编译错误，也不能记为构建通过。完整部分日志为
`build_logs/full_build_aarch64_partial.log`，SHA256 为
`02a142c545b529276c7e36c90ac9de070c897d8f4e706670aeef392ce61a653b`；
构建根保留在 `tmp/GBS-ROOT/BUILD-W1-0908-llvm-aarch64/`，可作断点参考。
中止后曾短暂残留由 sudo 启动的休眠包装进程，最终核验三个 PID 均已自行
退出，没有遗留编译负载。

## 未覆盖范围

- aarch64 全量构建只到 7,376/7,545，最终链接、打包和产物核查未观测；
- armv7l 物理板与 aarch64 QEMU 用户态的 AOT 模型实际执行尚未观测；
- 三架构 GCC 路径和不启用 libc++ 时的完整兼容构建尚未完成；
- 因上述硬门禁未闭合，尚未提交或推送 LLVM 包改动；
- W2 依赖 W1 推送，在此前不能启动。

## 门禁矩阵（截至报告生成时）

| 架构 | libc++ 完整构建与产物 | MLGO 实际运行 | GCC 路径完整构建 | 不启用 libc++ 的完整构建 |
| --- | --- | --- | --- | --- |
| armv7l | PASS | `NOT_OBSERVED`（物理板三次不可达） | `NOT_OBSERVED` | `NOT_OBSERVED` |
| aarch64 | `PARTIAL`（7,376/7,545 后按时限中止） | `NOT_OBSERVED` | `NOT_OBSERVED` | `NOT_OBSERVED` |
| x86_64 | PASS | PASS（原生） | `NOT_OBSERVED` | `NOT_OBSERVED` |

表中的 `PASS` 只覆盖该格明确列出的判据，不能外推到未观测格。三架构
静态资产与配置探针均已通过，但它们不替代完整构建或实际运行。

## 自行判断与疑问

1. 自行判断：在全量验证顺序上先完成 armv7l，再做 x86_64。理由是
   armv7l 为已知旧阻断架构；x86_64 完成后还能在本机直接核验 AOT 模型
   运行。aarch64 已完成配置探针，但这不替代其全量格。
2. 自行判断：新资产保持既有模型/对象文件名，因此不先修改 CMake
   扩展；以完整链接和运行时契约作为是否需要进一步改 CMake 的判据。
3. 当前没有需要人工裁决才可继续的设计岔路。物理板可用性和六小时
   上限造成 `NOT_OBSERVED`/`PARTIAL`，但没有放宽门禁或据此推送。
4. 执行偏差：W1 首份工件时间为 11:02，按六小时上限应在约 17:02
   中止；实际在 17:15 发出顶层终止信号，并在约 17:16 完成子进程停止，
   超出约 13–14 分钟。原因是轮询过程中对当前时刻估算错误。该偏差没有
   被用于扩大验证结论，aarch64 仍严格记为未完成，但属于本次时间纪律
   未完全遵守，需明确申报。

## 最终状态

`PARTIAL`：已完成 TF 2.18 资产接入、三架构静态/配置核查、armv7l 与
x86_64 libc++ 全量构建和产物核查、x86_64 原生 MLGO 执行；未完成
aarch64 全量构建、armv7l/aarch64 模型运行和三架构兼容构建。没有提交或
推送 LLVM 包分支。W1 材料提交为
`b038c73bae8b0a2666d86fbad613341f652d3976`，已推送并核验项目仓远端一致。
