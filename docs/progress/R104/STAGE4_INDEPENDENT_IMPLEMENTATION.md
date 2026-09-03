# 独立八包实施与验证记录

状态：`PARTIAL`

本阶段按人工确认继续处理与 LLVM 链无依赖的八个包；`llvm`、
`bcc-tools`、`bpftrace` 继续作为同一组暂停。所有构建均使用
`nice -n 15`、`ionice -c 3`、GBS 单任务和 `_smp_mflags -j2`，没有并行执行
两个构建。只有满足六格（三架构 × libc++/GCC 环境）判据的包才提交并普通
推送到既有 `sandbox/lhmax2025/libcxx-migration`，没有 force 或正式分支推送。

## 结果矩阵

| 包 | armv7l | aarch64 | x86_64 | 提交/推送 | 结论 |
| --- | --- | --- | --- | --- | --- |
| abseil-cpp | 两格通过 | 两格通过 | 两格通过 | `7a4f04dad6d30ba09c406837cdd23ad779a69210` | 通过 |
| icu | 两格通过 | 两格通过 | 两格通过 | `dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330` | 通过 |
| jsoncpp | 两格通过 | 两格通过 | 两格通过 | `9cbab147f44097d5b8da2b3ef38f75376a282d01` | 通过 |
| pcre | 两格通过 | 两格通过 | 两格通过 | `09a5bfc9a96c4235628ce800db3b95bf28ef2f23` | 通过 |
| taglib | 两格通过 | 两格通过 | 两格通过 | `8b552bdf27c913a93bca664c41c3fafb214962a1` | 通过 |
| libsigc++ | libc++ 构建成功但产物判据失败；GCC 通过 | 同左 | 同左 | 未提交、未推送 | 六格构建完成，严格判据未通过 |
| boost | libc++ 产物仍链接 libstdc++；GCC 通过 | libc++ 格在决定性失败后中止 | 未运行 | 未提交、未推送 | 改法未生效 |
| tensorflow2 | libc++ 编译失败 | 未运行 | 未运行 | 未提交、未推送 | 源码兼容阻断 |

“两格通过”表示：libc++ 格实际使用 Clang 22.1.8、命令含
`-stdlib=libc++`/`-lc++abi`、产物至少一个 ELF 依赖 `libc++.so.1` 且没有
`libstdc++.so.6`，构建根五处补丁头文件检查全过；GCC 格实际使用 GCC
14.2.0、不含上述 libc++ 参数、产物依赖 `libstdc++.so.6` 且没有
`libc++.so.1`。每格均为完整 GBS 构建，不是配置探针。

## 已通过五包

五包的六格均符合预先申报的判据，提交前审计确认只改各自的 spec；普通推送
后本地/远端 SHA 一致，既有其他远端分支按建分支前快照逐项核对未变。完整
命令、构建日志、验证输出和分支对照位于 `raw/`，矩阵摘要位于 `work/`。

## libsigc++：严格判据无法由该产物直接满足

三架构 libc++ 完整构建均实际使用对应目标的 Clang 22.1.8，编译和链接命令
均含 `-stdlib=libc++`，链接命令也含 `-lc++abi`。最终库的 RPM 依赖包含
`libc++abi.so.1`，但每格唯一 ELF 对
libc++ 和 libstdc++ 都没有 `DT_NEEDED`。
原因是库内没有留下需要 libc++ 动态定义的引用，`--as-needed` 合理地移除了
`libc++.so.1`。这不能满足人工已确认的字面判据“产物依赖
`libc++.so.1`”，因此没有把“命令已生效”替代为“产物判据已通过”，也没有
提交或推送。作为对照，三架构 GCC 格也均完整构建成功，实际使用 GCC
14.2.0、没有 libc++ 参数，并且最终 ELF 依赖 `libstdc++.so.6`；所以“GCC
路径不受影响”已完整实证。

另有一个 GBS 技术问题：显式传 `--spec libsigc++.spec` 时，GBS 在 export
前报告找不到 source package；不改变源码、只省略该参数后，GBS 能正常识别
唯一 spec 并完成构建。失败与恢复过程均保留在原始记录中。

## boost：Boost.Build 的安装阶段丢失标准库参数

armv7l/libc++ 格的首次 `b2` 调用确实收到含 `-stdlib=libc++` 的
`cxxflags`，以及含 `-stdlib=libc++ -lc++abi` 的 `linkflags`；构建器也显示
使用 Clang 22。但 `%install` 再次调用 `b2 install` 时没有传入这两组参数，
并重新构建最终安装物。最终 39 个 ELF 中 36 个依赖 `libstdc++.so.6`，没有
任何 ELF 依赖 `libc++.so.1`。因此已确认的改法没有覆盖真正进入 RPM 的构建
路径，验证失败；armv7l/GCC 对照完整通过。aarch64/libc++ 格已开始，随后因
armv7l 的失败机制已经确定且与架构无关，为控制资源而终止；外层进程状态
143 已标为恢复记录，不能冒充包装器自动写入的退出码。未擅自补改第二次
调用，未提交、未推送。

## tensorflow2：libc++ 源码兼容阻断及 GCC 路径语义

armv7l/libc++ 完整 GBS 构建实际进入 4,230 个 Ninja 步骤，所有已观察 C++
命令使用 Clang 并含 `-stdlib=libc++`。在第 1,504 项编译
`tensorflow/lite/kernels/elementwise.cc` 时失败：源码把
`std::abs<float>` 和 `std::abs<int32_t>` 传给 `std::function<T(T)>`，libc++
提供的重载集合不能匹配，Clang 报 `no matching function for call to
'EvalImpl'`。这需要源码兼容修正，超出本轮“只改 spec 与必要构建配置”的
授权，所以按失败门禁停止，没有执行其余五格、归档成员或外部 consumer
链接验证，也没有提交、推送。

该 spec 在 armv7l、aarch64、x86_64 上把 `USE_CLANG` 定义为 ON，并在两个
CMake 阶段显式指定 `clang`/`clang++`。`_toolchain_override gcc` 会使本轮
新增的 libc++ 条件关闭，但不会让此包实际改由 GCC 编译。后续材料必须区分：

- “GCC 工具链环境下保持原有行为”：可运行，预期仍由 spec 强制 Clang，
  但不注入 libc++；
- “实际由 GCC 编译”：这三个架构按现有 spec 不具备该路径。

不能把前者写成已完成后者。若该事实与人工要求“每个包在 GCC 下实际跑”
冲突，应在构建前作为裁决点报告，不擅自改变 `USE_CLANG`。

## 未触碰的 LLVM 链

`llvm` 的本地 spec 改动仍未提交、未推送；`bcc-tools` 与 `bpftrace` 没有进入
实施。三者继续一起暂停，MLGO/XLA 资产核查另见
`MLGO_ASSET_ASSESSMENT.md`。

## 自行判断与未决问题

- 自行判断：五包的“产物依赖标准库”按所有架构化 ELF 的 `DT_NEEDED` 验证，
  同时排除反向标准库；头文件五项从实际构建根核对。
- 未决：libsigc++ 已证明编译器与参数生效，但因无动态引用不能产生
  `libc++.so.1` 的 `DT_NEEDED`；是否允许用真实 consumer 的模板实例化/链接
  证明替代该字面判据，需要人工裁决。
- 未决：boost 要使第二次 `b2 install` 沿用相同 flags，需要补充设计并重新
  确认，当前没有自行修正。
- 未决：tensorflow2 首先需要人工决定是否另开源码兼容修正任务；在此之后，
  三个目标架构仍没有实际 GCC 编译路径，需确认“GCC 环境保持现状”是否满足
  该包的 GCC 验证要求。
