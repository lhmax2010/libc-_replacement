# LLVM 门禁矩阵闭合与推送报告

## 结论

结论为 **`LLVM_ADAPTED`**。十二格门禁已全部闭合：本次续跑补齐
x86_64 与 armv7l 的“不启用 libc++”同环境基线对照，两格的 RPM 集合、
包内文件记录、标准库 Requires、ELF `DT_NEEDED` 和动态符号集合均为
0 差异。

验证全部通过后，候选以普通快进方式推送到
`sandbox/lhmax2025/libcxx-ehabi-backport`。新提交为
`f203923a1508c9344f5fc6b17bd8822f011655c4`，直接父提交为
`c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13`；推送后远端 SHA 与本地一致。

## 输入、资源与范围

- 开工时间：2026-09-11 18:12:55 +08:00；十小时上限未触及；
- 资源门禁：`tools/resource_gate.sh --level medium`，退出码 0；
- 构建参数：`nice -n 15`、`ionice -c 3`、`_smp_mflags -j2`；
- 候选工作树：`tmp/BUILD_W1_0908/llvm-worktree`，基线 HEAD 为
  `2d23367d74afbf2bb1e9e4013fce072b3a154109`；
- 基线工作树：`tmp/BUILD_W1B_0909/llvm-baseline`，同一 HEAD；
- x86_64 使用此前保留的 12 个候选 RPM；armv7l 成功构建的 12 个
  release 109.3 RPM 仍保留在旧构建根，导出 spec 与当前候选只差 GBS
  自动插入的 `VCS: ...-dirty` 行；
- 没有修改上游 C++ 源码。

开工时 aarch64 构建根仍有 `abuild` 所有权的残留内容，但没有相关活动
进程，且本次 x86_64 与 armv7l 使用不同的明确构建根，因此未影响新构建；
本次没有提权或尝试绕过权限。

## 完整十二格矩阵

机器可读结果见 `MATRIX.tsv`。libc++ 构建、MLGO 实际执行、三架构 GCC
格及 aarch64 同环境格沿用前两轮已闭合结果；本次没有重跑这些格。

| 架构 | libc++ 构建与产品 | MLGO 实际执行 | GCC 构建 | 不启用 libc++ 同环境对照 |
| --- | --- | --- | --- | --- |
| armv7l | PASS（沿用） | PASS，物理板（沿用） | PASS（沿用） | PASS（本次） |
| aarch64 | PASS（沿用） | PASS，QEMU 用户态（沿用） | PASS（沿用） | PASS（沿用） |
| x86_64 | PASS（沿用） | PASS，原生（沿用） | PASS（沿用） | PASS（本次） |

## x86_64 同环境对照

候选 RPM 位于 `tmp/BUILD_W1B_0909/rpms/x86_64_candidate/`。本次用未带
候选改动的基线源码、相同 `gbs.conf`、release `111.1` 和相同资源参数
完成基线完整构建：`7634/7634`，12 个 RPM 全部写出，GBS 退出码 0，
耗时 10270 秒。

| 比较项 | 候选行数 | 基线行数 | diff 行数 |
| --- | ---: | ---: | ---: |
| RPM 集合 | 12 | 12 | 0 |
| 包内文件记录 | 5720 | 5720 | 0 |
| RPM 标准库 Requires | 164 | 164 | 0 |
| ELF `DT_NEEDED` | 742 | 742 | 0 |
| 动态符号集合 | 656205 | 656205 | 0 |

比较脚本输出 `SAME_ENVIRONMENT_RPM_COMPARE=PASS`，退出码 0。

## armv7l 同环境对照

保留候选的成功日志记录了 GCC/G++ 14.2.0、原
`mlgo_arm_model.tar.gz`、无 libc++ BuildRequires/参数，12 个 RPM 均为
release `109.3`。本次在同一个构建根以未带改动的基线源码做 `--clean`
完整构建：`7148/7148`，12 个 RPM 全部写出，GBS 退出码 0，耗时
7188 秒。基线同样使用 GCC/G++ 14.2.0，以下字符串在完整日志中的命中数
均为 0：`-stdlib=libc++`、`-lc++abi`、`libc++-devel`、
`libc++abi-devel`。

第一次比较直接指向构建仓目录，误把该目录中旧 release 108.2 的 10 个
debuginfo RPM 纳入基线，得到 packages diff 24 行、file lists diff
11654 行，退出码 20；标准库 Requires、ELF 依赖和动态符号当时已是
0 差异。确认差异全部来自旧 RPM 后，将本次 release 109.3 的 12 个 RPM
隔离到独立目录，保持脚本判定逻辑不变并重新比较：

| 比较项 | 候选行数 | 基线行数 | diff 行数 |
| --- | ---: | ---: | ---: |
| RPM 集合 | 12 | 12 | 0 |
| 包内文件记录 | 5644 | 5644 | 0 |
| RPM 标准库 Requires | 149 | 149 | 0 |
| ELF `DT_NEEDED` | 718 | 718 | 0 |
| 动态符号集合 | 654705 | 654705 | 0 |

修正输入集合后的比较输出 `SAME_ENVIRONMENT_RPM_COMPARE=PASS`，退出码 0。
旧 debuginfo 被纳入的第一次结果仍原样归档，没有删除或覆盖。

## 目标分支冲突与提交核对

直接从远端浅取目标分支后确认：

- 推送前远端 HEAD 为用户给定的 `c68f376f...`；
- 目标分支 `packaging/llvm.spec` 与验证基线 spec 的 SHA256 均为
  `7962d176484920de6138d70c7e8a8e1764dfada6655abf0332e9748167e889fd`；
- 因此目标分支五个运行时实施提交没有造成 spec 重叠；
- 重放后的 spec 与已验证候选逐字节一致；
- 三份 TF 2.18/libc++ 资产的 SHA256 分别为：
  - aarch64：`d50d874bab843ea96c3a282d34f172b87a16aefe5aa8309871e4d328c8b2f17c`；
  - armv7l：`3c0a04d193eb4b3a601036a6ee7217303fddb44bca5adcd13c67066c37a1c87c`；
  - x86_64：`858031bbfddf385ac859320f40f91dd0cf51bd1d4414b5cd55e36308ab616636`。

提交相对目标分支只修改 `packaging/llvm.spec` 并新增上述三份资产。
普通 push 返回退出码 0；远端从 `c68f376f...` 快进到 `f203923a...`。
服务端只报告 commit message 存在超过 72 字符的行这一警告，没有拒绝。

## 技术性非零与修正

- 首次启动 x86_64 基线时，日志目录按错误的相对工作目录创建，`script`
  在构建开始前退出 1；改为绝对路径后原参数重跑。
- 常规 fetch 会要求约 681 万对象，接收约 160 MiB 时人工中止，退出 1；
  删除两个明确的未完成 `tmp_pack_*` 文件后，改用 `--depth=1 --no-tags`
  单分支 fetch，成功取得 `c68f376f...`。
- armv7l 第一次比较的退出 20 是输入目录混入旧 debuginfo RPM，不是候选
  与基线产品差异；隔离同 release 12 个 RPM 后同一脚本退出 0。
- 一次只读收尾命令末尾的路径被错误截断为 `MultiN`，在已取得完整构建
  终态后额外返回 `command not found`；随后用独立命令取得 RPM 计数与
  编译器证据。它不影响构建或比较。

## 归档

- 完整构建输出：`build_logs/*_baseline_0911.typescript.gz`；
- 两次有效比较与一次未过滤比较：`build_logs/*compare*0911.log`；
- 实际比较脚本快照：`code/compare_same_environment_rpms.sh`；
- 命令原文、退出码和推送核对：`raw/017_*` 至 `raw/022_*`。

## 自行判断与尚存疑问

- 自行判断：armv7l 构建根中保留的 release 109.3 RPM 可作为候选，因为
  成功日志、仓地址、编译器、资产选择和禁用项均可核对，且导出 spec 与
  当前候选唯一差异是 GBS 自动生成的 VCS 行。
- 自行判断：旧 release 108.2 debuginfo RPM 不属于本轮基线集合；隔离
  release 109.3 输入后重跑，而不修改扫描器。
- 尚存疑问：无影响本次门禁或推送结论的未决问题。
