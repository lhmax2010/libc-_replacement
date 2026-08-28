# R77：LLVM 官方测试套件 armv7l 全量运行

## 结论

`PARTIAL`

补丁版完成 libc++ 11,321/11,321 和 libc++abi 81/81；基线版 libc++ 完成
7,000/11,321 后，batch-015 发生 `SDB_CONNECTION_FAILURE`，批次驱动以 90
退出。依照周末任务包的开发板异常纪律，本任务未重试、未继续启动基线
libc++abi，随即清理并释放开发板。因此不能给出完整基线对照，也不能宣称
`FULL_SUITE_PASSED` 或完整判定补丁是否引入失败。

实际运行时间：2026-08-28T18:34:34+08:00 至
2026-08-28T20:58:25+08:00，约 2 小时 24 分，未触及 7 小时上限。

## 执行约束与身份

- 资源门禁：`tools/resource_gate.sh --level medium`，退出 0；运行期间约每
  5 分钟采样，未触发 1 GiB 可用内存停止线。
- 并发与优先级：lit `-j 2`，`nice -n 15`，`ionice -c 3`。
- 每项板端运行硬超时：110 秒（小于任务要求的 120 秒）。
- 补丁源码：`tmp/R69/replay`，HEAD
  `ae3471c26c66489439354bb1e89df6af8b56da3e`。
- 基线源码：`tmp/R68/source-baseline`，HEAD
  `5ed6c77278dfa7a470667cf1a137723d3c96fe60`。
- 配置快照与编译包装器保存在 `code/config/` 和
  `code/armv7l_lit_host_clangxx.sh`；原始命令和退出码分别在
  `commands/` 与 `raw/`。

## SDB 执行器修正与验证

旧执行器把子进程 stdin 固定为 `DEVNULL`，导致依赖管道输入及 EOF 的官方
测试卡住。`code/sdb_executor.diff` 是完整修正 diff；修正后的执行器把 stdin
内容随 tar 包上传为 `.stdin`，板端从该文件重定向，从而同时保留内容与 EOF。
超时后终止该次运行的远端进程组并返回 124。

验证结果：

| 门禁 | 结果 | 证据 |
|---|---:|---|
| `stdin-is-piped.sh.cpp` 与 `cin.sh.cpp` | lit 退出 0 | `commands/029_executor_stuck_tests_validation_rerun.log` |
| 2 秒超时探针 | 退出 124 | `commands/031_executor_timeout_probe.log` |
| 超时后进程与运行目录残留 | 无，检查退出 0 | `raw/032_timeout_probe_cleanup_verification.stdout` |
| Python 语法检查 | 退出 0 | `commands/027_executor_syntax_after_helper_fix.log` |

第一次门禁暴露了 SDB 直连 stdin 不能可靠传递 EOF，后续还暴露一个
bytes/str 辅助函数类型错误；两次均只修正任务执行器，失败现象、修正前后 diff
和重跑输出均保留。没有改动 libc++、libc++abi 或定稿补丁。

lit 自带 `--timeout 120` 因当前环境缺少 `psutil` 在配置阶段退出 2；未安装
依赖，移除该选项，继续使用执行器的逐项 110 秒硬超时。原始失败在
`commands/044_patched_resume_batches.log` 及对应 raw 输出中。

## 旧 5,877 项有效性

重新发现 11,321 项测试，并把旧结果的完成序号与五个 stdin 依赖测试逐一
核对。五项均不在旧的 5,877 条已完成结果中：

| 测试 | 词典序位置 | 旧结果 |
|---|---:|---|
| `selftest/stdin-is-piped.sh.cpp` | 2,882 | `NOT_COMPLETED` |
| `narrow.stream.objects/cin.sh.cpp` | 5,879 | `NOT_COMPLETED` |
| `wcin-imbue.sh.cpp` | 5,885 | `NOT_COMPLETED` |
| `wcin-wide-mode.sh.cpp` | 5,886 | `NOT_COMPLETED` |
| `wcin.sh.cpp` | 5,887 | `NOT_COMPLETED` |

因此旧 5,877 项就已知 stdin 缺陷而言有效，没有重跑。注意：发现列表中
`selftest` 的词典序位置小于 5,877，但旧运行的完成集合不是简单的词典序前缀；
判定依据是逐条结果集合，而不是只比较位置。完整机器可读结果见
`raw/036_completed_prefix_validity.stdout`。

R68 的原始已完成输出和结果文件复制到 `raw/prior_R68/`，17 个文件经修正后的
校验器逐字节核对一致。第一次校验脚本错误地只选择了 16 个源文件，技术性
错误和修正重跑均保留，没有改变原证据。

## 实际覆盖与结果

| 组合 | 观察数 / 计划数 | PASS | FAIL | UNSUPPORTED | XFAIL | 状态 |
|---|---:|---:|---:|---:|---:|---|
| patched armv7l libc++ | 11,321 / 11,321 | 10,017 | 214 | 1,061 | 29 | 完成 |
| patched armv7l libc++abi | 81 / 81 | 60 | 0 | 21 | 0 | 复用并逐字节归档 R68 完整结果 |
| baseline armv7l libc++ | 7,000 / 11,321 | 5,787 | 163 | 1,022 | 28 | batch-015 连接失败后停止 |
| baseline armv7l libc++abi | 0 / 81 | `NOT_OBSERVED` | `NOT_OBSERVED` | `NOT_OBSERVED` | `NOT_OBSERVED` | 未启动 |

补丁版 11 个续跑批次全部完成，聚合后无缺失、无重复。三个随机分布评估测试
达到执行器的 110 秒上限并返回 124；它们仍按失败保留，没有提高阈值或重试。
补丁运行库在板端的 SHA256 与主机一致，且一个退出 0 的探针通过
`LD_DEBUG=libs` 确认从本任务精确 runtime 目录加载 libc++ 和 libc++abi。

基线 14 个批次的 7,000 条结果有完整检查点；batch-015 未生成 COMPLETE，
不计入观察集合。基线运行库部署前后 SHA256 一致。基线 loader 核验与
libc++abi 基线因板卡异常均为 `NOT_OBSERVED`。

## 失败项逐项分类

机器可读逐项表为 `tables/failure_classification.tsv`，共 214 条补丁版失败：

| 分类 | 数量 | 判定依据 |
|---|---:|---|
| 本方案引入 | 0 | 已观察的 7,000 项中没有“基线 PASS、补丁失败” |
| 上游已有 | 135 | 基线与补丁版均为失败状态 |
| 环境限制 | 0（未单列） | 本次不以推断代替成对证据；可能的环境因素仍保留在原输出中 |
| 无法判断 | 79 | 对应基线批次在断连前未运行 |

在 7,000 项重叠集合中，6,972 项状态相同；另有 28 项基线失败而补丁版 PASS，
列在 `tables/baseline_only_failures.tsv`。这些数据只说明已观察集合，不外推到
缺失的 4,321 项。135 项“上游已有”表示失败在未打补丁基线上也存在，不等于
断言其根因一定是 LLVM 上游代码；详细 stdout/stderr 是最终根因判断依据。

## 与已知 25 项发现的对照和盲区

- D-01：`forced_unwind1/2/3.pass.cpp` 在补丁版 libc++abi 均 PASS；
  `forced_unwind4.pass.cpp` 是 aarch64 专用，在 armv7l 为 UNSUPPORTED。官方
  libc++ 套件未发现 `pthread_cancel` 用例，因此五个已知取消边界没有被
  官方套件逐一动态命中；只能确认相邻设施的普通路径被广泛运行。
- D-02：异常禁用相关 `noexception1..4` 在该 arm 配置为 UNSUPPORTED。
- D-03、D-05、D-21：fallback allocator、future、stream、容器、buffer、
  locale/time 等官方测试被发现并按其所在批次执行；这验证的是单套 libc++
  路径，不是 libstdc++ 对照或共存期语义。
- D-10：RTTI/dynamic_cast 类测试属于单运行时官方测试，不能证明两套运行时
  之间可跨界传递 C++ 异常。
- D-22：armv7l 全体异常测试与 forced-unwind 1..3 覆盖 ARM EHABI 路径，
  但基线对照未完成。
- D-24：补丁版 armv7l 的全套缺口已经关闭；基线仍缺 4,321 项。
- D-07~09、D-11~20、D-23、D-25 多数是符号、ABI、部署或共存期差异，
  单运行时官方套件不能直接验证。

本轮没有覆盖 aarch64、LLVM 官方套件的其他配置、两套标准库共存期场景，
也没有覆盖异步取消恰落在无取消点忙循环的机制路径。

## 异常、中止与清理

`raw/149_baseline_arm_libcxx_batches.stdout` 最后一行是
`SDB_CONNECTION_FAILURE\tbatch-015`，命令退出 90。诊断时设备随后恢复可见，
但按任务纪律没有继续或重试。清理只针对
`/opt/usr/home/owner/share/tmp/r77_lit_20260829`；清理脚本报告
`ROOT_STATE=ABSENT`、`PROCESS_STATE=ABSENT`、`RESULT=PASS`，随后再次确认目录
ABSENT。证据见 `raw/212_capture_sdb_failure_and_cleanup.stdout`。

## 自行判断与未决问题

- 自行判断：对完成的 7,000 项进行部分基线对照仍有证据价值，因此在中止后
  只做离线聚合与分类；没有恢复板端工作。
- 自行判断：相同测试在基线和补丁版均处于失败状态，分类为“上游已有”；
  报告同时限定该标签不代表已查明根因。
- 未决问题：无须人工裁决的问题；剩余 4,321 项和基线 libc++abi 只能在新的
  授权任务中从检查点继续，不能由本报告推断。

## 材料导航

- `commands/`：每条命令原文、开始/结束信息和退出码；
- `raw/`：完整 stdout/stderr、lit JSON、遥测、清单和复制的 R68 原始证据；
- `code/`：执行器修正前后源码、diff、批处理、聚合、分类与清理工具；
- `tables/`：逐项失败分类和部分基线差异；
- `INDEX.tsv`：最终全材料 SHA256 索引。

未推送 Gerrit、未推送任何外部源码仓、未修改 `codes/`。
