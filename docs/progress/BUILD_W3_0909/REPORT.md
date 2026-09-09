# W3：Base 层 11 包最终状态与 QuickBuild 准备

## 结论

本整理任务为 **`COMPLETE`**；包实施状态仍为 **8/11 已完成并推送、
3/11 暂停**。本轮没有改变包判定、理由或证据，没有构建 package，没有
修改平台源码或配置，也没有触发 QuickBuild。

## 11 包状态

| 状态 | 数量 | 包 |
| --- | ---: | --- |
| 已完成并推送 | 8 | `abseil-cpp`、`boost`、`icu`、`jsoncpp`、`libsigc++`、`pcre`、`taglib`、`tensorflow2` |
| 候选验证未闭合、未推送 | 1 | `llvm` |
| 前置不可得、未实施 | 2 | `bcc-tools`、`bpftrace` |

八个完成包的远端 SHA 与既有验证记录逐项一致。七个常规包的三架构 ×
libc++/GCC 六格完整通过；`tensorflow2` 的 armv7l GCC 格是候选与未改
基线同点同因失败、按预先确认的相对基线无回归判据通过，不能改写成六格
完整构建全成功。精确 SHA、验证描述和本地成功 push 日志写入时间见
`PACKAGE_STATUS.tsv`。Gerrit `ls-remote` 不提供服务端 ref 更新时间，故不把
这些本地时间冒充服务端审计时间。

LLVM 的 libc++ 三架构完整构建与三种执行环境的 MLGO 实测已经通过；兼容
门禁仍未闭合，详情见 W1 报告。LLVM 未提交、未推送，两个下游包按依赖规则
未实施。

## 兼容性交叉核对

对八个已推送提交的实际 spec 做了结构扫描：所有 libc++ BuildRequires、
运行依赖、编译与链接选项均受 `build_with_libcxx` 保护，而该宏仅在
`_toolchain` 已定义且 `toolchain_is clang` 时定义；`tensorflow2` 还要求
`USE_CLANG=ON`。八份均 PASS。既有 GCC 实构建证据没有观察到 libc++ 选项
进入 GCC 路径。

不能把这一结论扩大成“每个提交的所有文本行都在条件块内”：Boost 的参数
传递脚手架、TensorFlow 的标准兼容源码修正及两处文件尾空行不属于该条件
块。它们不向 GCC 添加 libc++ 选项，且已有相应 GCC 验证。准确结论和每个
例外见 `COMPATIBILITY_AUDIT.md`。

LLVM 本地候选采用相同条件保护，armv7l GCC 实构建没有 libc++ 选项；由于
其余兼容格不完整，只记 PARTIAL。`bcc-tools`、`bpftrace` 没有适配提交，
记 NOT_AVAILABLE。

## 对外材料

本轮输入是上一轮已经审计的中英文状态版，副本及 SHA256 在 `source/` 与
`raw/002_external_material_inputs.md`。实际实施状态仍为 8/3；254 包清单仅
更新 `llvm` 一行的“当前实施状态”进度文字，前八列与输入逐字相同，另外
253 行的状态列也逐字相同。没有改写判定或理由。

中文 Base 报告与英文实施状态说明只更新 LLVM 的验证进度：三架构 libc++
与三种 MLGO 执行环境已过，兼容门禁仍未闭合。除此之外不改判定与理由。

## QuickBuild 输入

`QUICKBUILD_INPUT.md` 已在开头按要求明确标注尚未与实际 QuickBuild 界面
核对，并列出：

- READY：`libcxx-runtimes` 与八个已推送独立包；
- NOT_READY：`llvm`、`bcc-tools`、`bpftrace`；
- 顺序：`libcxx-runtimes → llvm → bcc-tools → bpftrace`；
- 其余八包在 libcxx-runtimes 可用后可独立安排；
- 动态库、模板库、静态归档、LLVM AOT 和五处头文件的不同产品判据。

由于 LLVM 未推送，本文不能直接作为 11 包完成态输入触发整轮验证。

## 未完成与未观测

- LLVM：aarch64、x86_64 的 GCC/不启用 libc++ 完整构建与产物核查；
  armv7l 不启用 libc++ 的同环境整包等价对照；
- bcc-tools、bpftrace：改动、六格构建和边界两端产品核查；
- QuickBuild：实际输入界面、调度行为与本轮产物均 `NOT_OBSERVED`。

## 自行判断与尚存疑问

- 自行判断：对“所有改动在条件内”作严格区分。只认定所有 libc++ 激活
  行为受保护，不掩盖提交内存在无条件脚手架、标准兼容源码修正和空行变化。
- 自行判断：由于 254 包实施状态未改变，清单只复制为本轮交付版，不制造
  无事实变化的改写；Base 报告的 LLVM 进度按最新证据更新。
- 尚存疑问：W1 中“不启用 libc++ 的整包等价”闭合口径仍待人工确认；已
  集中记录在 `BUILD_QUESTIONS_0909.md`。
