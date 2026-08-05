# T4 三架构打包期门禁报告

执行日期：2026-08-05（Asia/Shanghai）

## 1. 结论

T4 总结论为 `RED_WITH_AARCH64_NOT_AVAILABLE`：

- x86_64：`PASS`，14 个门项全部实测达到判据。
- aarch64：`NOT_AVAILABLE`；P01-P08、P14 静态门项通过，宿主没有可用的
  qemu-aarch64 载体，P09-P13 严格记为 `NOT_AVAILABLE`，未以静态检查冒充。
- armv7l：14 个门项全部实测通过，但制品输入身份出现
  `RED_UNEXPECTED_R6_GIT_EXPORT_DELTA`，因此架构总状态为 `RED`。

制品本身没有发生门禁前漂移：三架构 24 个 RPM（每架构 7 binary + 1
SRPM）的当前 SHA256 与 T3R/T3R2/R6 归档值逐项 `MATCH`。

## 2. 门项清单与冻结工具映射

清单不是重新构想所得。先从 21.1.1 线的
`multiarch_packaging_report.md`、三架构 `gate_verdict.tsv` 及其引用的
`run_static_gates.sh`、`run_runtime_gates.sh` 逐字取证，再形成 P01-P13；
P14 是本任务明列的头文件布局门项。完整 14 项、判据、`file:line` 出处、
T5 工具映射和本次执行方法见 `gate_inventory.tsv`。

T5 的 `gates/FREEZE_MANIFEST.sha256` 共 145 项，本轮重新执行
`sha256sum -c`，145/145 通过。六项由新冻结 LLVM_22 静态工具直接实现；
其余八项明确列为 `NO_TOOL_MAPPING`，并按旧门禁原实现或本任务规定的等价
取证执行，见 `no_tool_mapping.tsv`，没有缩减门项。

新冻结静态工具 SHA256 为
`d24f7c234d266bf970dbfd9c24939052681eba2105c186d0002009891c119885`。
该工具的符号判据已是 `@@LLVM_22`，但输入文件选择器仍固定匹配
`21.1.1-2` 文件名。本轮在 `tmp/T4/static/` 创建逐字节相同的临时别名载体
后实际调用该工具；别名和原件 SHA 均逐项 `MATCH`，RPM 内部 NEVRA 仍为
22.1.8-1。此适配不修改归档制品，详见 `static_tool_adapter.tsv`。

## 3. 逐架构结果

完整逐门项表见 `gate_results.tsv`，三方差异见
`three_arch_gate_comparison.tsv`。

| 门项组 | armv7l | aarch64 | x86_64 |
|---|---|---|---|
| P01-P08 静态/身份 | PASS | PASS | PASS |
| P09 `ldd -r` 闭包 | PASS | NOT_AVAILABLE | PASS |
| P10 provider family | PASS | NOT_AVAILABLE | PASS |
| P11 绿例 | PASS_GREEN | NOT_AVAILABLE | PASS_GREEN |
| P12 红例 | EXPECTED_RED_DETECTED | NOT_AVAILABLE | EXPECTED_RED_DETECTED |
| P13 smoke | PASS | NOT_AVAILABLE | PASS |
| P14 头布局 | PASS | PASS | PASS |

LLVM_22 版本门禁实际结果：armv7l 的 libc++.so.1.0 为 1968/1968、
libc++abi.so.1.0 为 371/371；aarch64、x86_64 分别为 1969/1969 和
369/369。六个 DSO 均为 100% 默认版本 `@@LLVM_22`，单 `@LLVM_22`
和未版本化/其他导出均为 0。

闭包实测中，armv7l、x86_64 的强未解析符号、libstdc++、libunwind
命中均为 0。provider family 均实测为：`__cxa_finalize` → glibc，
`_Unwind_RaiseException` → libgcc_s，`__cxa_throw`、
`__cxa_allocate_exception`、`__gxx_personality_v0` → 本轮候选
libc++abi。

## 4. `ldd -r` 执行载体

- armv7l：使用宿主静态 wrapper 调用
  `/usr/bin/qemu-arm-static -L <R6 退役目标根> <目标 ld-linux.so.3>`，
  仅把目标根 glibc `ldd` 的 `RTLDLIST` 改指该 wrapper，再对本轮从 RPM
  提取的两个 DSO 执行 `ldd -r`。wrapper 源码、二进制、适配 diff、
  SHA256 和两次完整输出均在 `probes/`、`armv7l/static/evidence/`。
- x86_64：在 x86_64 宿主上以原生 x86_64 GBS 目标根执行 `ldd -r`。
- aarch64：PATH、`whereis`、`/usr/{bin,sbin}`、`/usr/local/{bin,sbin}`、
  `/opt` 和已安装 RPM 清单均未找到可执行 qemu-aarch64；故为
  `NOT_AVAILABLE`。

## 5. 制品与来源身份

`artifact_identity.tsv` 的 24 行全部 `MATCH`，各 RPM 还通过
`rpm -K --nosignature` 可读性检查。三个 RPM 内的
`SOURCE_PROVENANCE` 内容逐字一致，SHA256 都是
`1ab73051dc45ee72e681ec0393889bdf527c4d7407df13eb3effa79c7dd4be74`。

但“内容一致”不等于“内容与实际输入一致”：

- aarch64、x86_64 SRPM 的 Source0 SHA256 都是平台 tarball 锚
  `16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e`，
  且两树逐字一致。
- armv7l SRPM 的 Source0 SHA256 是
  `7d7f2105f1c745754e1318b3d73b632a642e426883fb2d6fa6a5b46f55b5d314`，
  与其 `SOURCE_PROVENANCE` 所声明的“verbatim platform SRPM tarball”
  不符。
- backport commit `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b` 本身仍严格是
  `EXPECTED_BACKPORT_DELTA`：仅改
  `libcxxabi/src/cxa_personality.cpp`，2 行新增，声明位于 ARM EHABI
  分支，因此该提交本身只影响 ARM EHABI 路径。
- 实际 R6 Git 导出 Source0 另有 clang、compiler-rt、llvm 文件差异，
  并多出 `packaging/`、`r6-packaging/` 目录；这部分不能归入
  `EXPECTED_BACKPORT_DELTA`，单列
  `RED_UNEXPECTED_R6_GIT_EXPORT_DELTA`。全量目录 diff 原文在
  `raw/armv7l_vs_x86_source_tree_retry.diff`。

## 6. 三架构差异与覆盖边界

P01-P08、P14 三方一致通过。P09-P13 的唯一门项结果差异是 aarch64
载体不可得；这是 `NOT_AVAILABLE`，不是实测失败。armv7l 与 x86_64
在所有可执行门项上结论一致。

本轮覆盖仅限构建期和静态/加载期检查。ARM EHABI 与 Itanium/DWARF
两条路径的运行时异常展开正确性均未经板测，缺口登记为
`PENDING_BOARD_VERIFICATION`。

## 7. 自动化失败留痕

所有首轮失败记录均保留，未覆盖命令或退出码：首次 SRPM cpio 成员名
选择错误、对 arm Source0 只允许一个文件差异的过严断言、DWARF 把正常
GNU AS 编译单元误判为失败、DWARF 校正表列号错误、无界 QEMU 搜索被
SIGINT、`strings | rg -q` 的 SIGPIPE 141。对应 disclosure 与 retry
脚本逐一说明修正依据；最终结论只引用通过登记判据的实测结果。

本任务到此停止，未组建 candidate repo，未进入晋级台账。
