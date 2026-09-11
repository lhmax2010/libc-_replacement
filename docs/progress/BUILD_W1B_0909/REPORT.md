# LLVM 剩余门禁矩阵补测报告

## 结论

结论为 **`GATE_NOT_CLOSED`**。本轮把 aarch64 的 GCC 产品格和同环境
非 libc++ 对照格闭合为 PASS；x86_64 干净候选构建结果见后文。由于
armv7l 的新判据要求同环境双构建，而它无法在十小时时限内与本轮其余
大构建全部完成，十二格矩阵仍有未观测项。按门禁纪律，**没有提交或推送
LLVM package 分支**。

## 输入与纪律

- 项目分支：`codex/r5-ehabi-diagnosis`；
- LLVM 候选工作树：`tmp/BUILD_W1_0908/llvm-worktree`；Git HEAD
  `2d23367d74afbf2bb1e9e4013fce072b3a154109`；
- 同环境基线：从 `codes/R104/llvm` 本地共享克隆到临时目录，并检出同一
  HEAD；未写入 `codes/`；
- 开工资源门禁退出码 0；构建均使用 `nice -n 15`、`ionice -c 3`、
  `_smp_mflags -j2`；
- 十小时硬截止为 2026-09-11 19:58（Asia/Shanghai）；
- 没有修改上游 C++ 源码。

## 完整十二格矩阵

机器可读结果见 `MATRIX.tsv`。上一轮已闭合的 libc++ 构建、三架构 MLGO
执行和 armv7l GCC 格直接沿用 `docs/progress/BUILD_W1_0909/`，本轮未重跑。

| 架构 | libc++ 构建与产品 | MLGO 实际执行 | GCC 构建 | 不启用 libc++ 同环境对照 |
| --- | --- | --- | --- | --- |
| armv7l | PASS（沿用） | PASS，物理板（沿用） | PASS（沿用） | NOT_OBSERVED |
| aarch64 | PASS（沿用） | PASS，QEMU 用户态（沿用） | PASS | PASS |
| x86_64 | PASS（沿用） | PASS，原生（沿用） | PASS | NOT_OBSERVED |

## aarch64 GCC 产品格

候选在新的独立构建根完成 `7546/7546` 并写出 12 个架构 RPM，GBS 退出码
为 0。C/C++ 编译器均为目标前缀 GCC/G++ 14.2.0。产品核查结果：

- `-stdlib=libc++` 和 `-lc++abi` 在生成的构建文件中均为 0 命中；
- 源码 RPM 的 BuildRequires 中 libc++ / libc++abi 为 0 命中；
- `libLLVM.so.22.1` 依赖 `libstdc++.so.6`，不依赖 libc++ / libc++abi；
- 产品核查脚本退出码 0。

完整构建日志、脚本和逐项输出分别位于 `build_logs/`、`code/` 与
`evidence/aarch64_candidate/`。

## aarch64 不启用 libc++ 的同环境对照

基线和候选在同一天、同一 `gbs.conf`、同一 release `111.1`、同一滚动
仓时间窗口及相同资源参数下分别做了干净完整构建。基线也完成
`7546/7546`，GBS 退出码 0。

两侧各 12 个 RPM 的全量比较结果：

| 比较项 | 行数 | diff 行数 |
| --- | ---: | ---: |
| RPM 包集合 | 12 | 0 |
| 包内文件清单 | 5691 | 0 |
| RPM 标准库依赖 | 158 | 0 |
| ELF `DT_NEEDED` | 731 | 0 |
| 动态符号集合 | 653085 | 0 |

比较脚本退出码 0。候选日志还直接显示该路径解包原
`mlgo_aarch_model.tar.gz`；候选与基线的这个输入归档 SHA256 相同，
`cmp` 退出码 0。因此该格按新判据记 PASS。

## x86_64

旧断点的增量尝试在 51 秒即暴露缓存污染：C/C++ 选择 x86_64 GCC，
但 CMake 同时残留不存在的 aarch64 Clang ASM 编译器，并把 `-lc++abi`
带入 GCC 链接探针。该尝试退出码 1，未用作判据。

随后新的独立构建根完成 `7634/7634` 并写出 12 个 RPM，GBS 退出码 0，
耗时 9149 秒。实际编译器为目标前缀 GCC/G++ 14.2.0；禁止的
`-stdlib=libc++`、`-lc++abi` 和 libc++ 相关 BuildRequires 均为 0；
`libLLVM.so.22.1` 依赖 libstdc++，不依赖 libc++ / libc++abi。产品核查
退出码 0，因此 GCC 格记 PASS。

候选 12 个 RPM 及其 SHA256 已保留在
`tmp/BUILD_W1B_0909/rpms/x86_64_candidate/`，下一轮只需在同一输入窗口
完成基线构建再运行比较脚本。17:36 时距硬截止仅约 2 小时 22 分，少于
本轮同类基线的实测 2 小时 26 分，且还需留出比较和材料收口时间，因此
没有启动必然跨过截止的基线构建；该格记 `NOT_OBSERVED`。

## armv7l 新判据格未闭合

上一轮已证明 armv7l 未启用 libc++ 时选择的 10,866 个资产文件与原
TF 2.15 输入逐文件 SHA256 一致，但当时使用的是跨日期固定 RPM 对照，
不满足本轮确认的同环境基线判据。本轮先执行 aarch64 两轮完整构建，再
处理 x86_64；按实际单轮 LLVM 构建耗时，已没有足够时间在十小时截止前
再完成 armv7l 候选与基线两轮。因此该格记 `NOT_OBSERVED`，不复用跨日期
结果冒充同环境证据。

## 技术性失败与输入局限

- aarch64、x86_64 的旧增量根都出现缓存自相矛盾，均在 CMake 配置阶段
  被拒用并转为独立新根干净构建；这些退出码 1 不作产品失败判据。
- 候选 GBS 导出包含一个工作树根目录的未跟踪 `llvm.manifest`。它与
  `packaging/llvm.manifest` 字节相同、不由 spec 安装；aarch64 的五类
  最终 RPM 对照均为 0 差异。该输入瑕疵仍显式保留在记录中。
- aarch64 大型临时 RPM 与 171 MiB 中间比对表在摘要和空 diff 归档后已
  清理。构建根中部分 `abuild` 所有权内容无法由当前用户删除，普通清理
  报权限不足，提权清理又因无交互凭据不可用；这项清理限制如实登记。

## 推送状态

十二格未全部闭合，故未创建 LLVM 提交、未推送
`sandbox/lhmax2025/libcxx-migration`。推送前远端该分支与 `tizen_base`
均为 `2d23367d74afbf2bb1e9e4013fce072b3a154109`；收口时再次核验远端未变。

## 自行判断与尚存疑问

- 自行判断：优先完成任务明确列出的 aarch64/x86_64 四格；根据每轮 LLVM
  全构建的实测耗时，armv7l 同环境双构建确定无法同时落在十小时内，故
  不启动必然跨过截止的构建。
- 自行判断：增量根同时出现错误架构 ASM 编译器缓存和与当前工具链矛盾的
  链接参数时，不把其非零视作候选产品失败，改用干净根复测。
- 尚存问题：armv7l 的同环境候选/基线双构建仍需约 4.5 小时；x86_64
  候选已经保留，单独补基线、比较及收口按本轮实测约需 2.7 小时。
