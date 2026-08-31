# R81：armv7l 基线补跑与测试夹具隔离

状态：`FAILURES_FOUND`（全量执行完成；213 项为基线与补丁共同失败，方案引入 0 项）

## 结论

R81 两阶段均已完成。armv7l libc++ 的 owner 标准化对照覆盖
11,321/11,321 项，libc++abi 覆盖 81/81 项。标准化后两侧状态逐项完全一致：

| 组合 | PASS | FAIL | UNSUPPORTED | XFAIL | 总数 |
|---|---:|---:|---:|---:|---:|
| armv7l 基线 libc++（owner） | 10,018 | 213 | 1,061 | 29 | 11,321 |
| armv7l 补丁 libc++（owner 标准化） | 10,018 | 213 | 1,061 | 29 | 11,321 |
| armv7l 基线 libc++abi | 60 | 0 | 21 | 0 | 81 |
| armv7l 补丁 libc++abi | 60 | 0 | 21 | 0 | 81 |

213 项 libc++ 失败全部是两侧共同失败，沿用任务口径分类为
`UPSTREAM_EXISTING`；这里表示“未打补丁基线也失败”，不表示已经逐项证明根因
一定在 LLVM 上游。`SCHEME_INTRODUCED=0`、`INDETERMINATE=0`。

结合此前已经完成的 x86_64 全量对照，两架构的选定测试配置均未发现补丁引入
的回归。结论限定在本文“未覆盖范围”内。

机器可读汇总见 `tables/final_summary.json`，213 项逐项分类见
`tables/full_failure_classification.tsv`。

## 执行身份和夹具隔离

统一使用 `owner(5001)`。理由是产品应用通常不是以特权 root 运行，owner 更接近
实际产品身份；同时它与既有补丁侧预期身份一致，并且不会绕过权限和
`RLIMIT_NPROC` 测试夹具。

判断身份敏感测试的方法分两层：

1. 对测试源码机械扫描权限变更、权限错误断言、UID/EUID、`setrlimit`、
   `RLIMIT_NPROC` 等身份相关机制；
2. 对候选项在同一补丁运行库上做 root/owner 动态对照，并对原差异项做两侧
   owner 成对复验。不能只靠目录名或历史状态推断。

最终确认 28 项身份敏感测试：27 项 filesystem 权限夹具，以及
`thread_create_failure.pass.cpp`。完整清单和机械依据见
`tables/identity_sensitive_tests_final.tsv`。

filesystem 测试通过 `scoped_test_env`/`static_test_env` 在每个测试进程内创建
目录、普通文件、软/硬链接、FIFO、socket 和无权限路径。异常中止会跳过 C++
析构，留下专属临时根；正常运行则由析构和执行器清理。隔离方案为两侧使用
互不相干、运行前重建的 `/opt/usr/r81_phase2/owner_baseline/tmp` 与
`owner_patched/tmp`，固定 `TMPDIR/TMP/TEMP`，每项仍使用独立远端工作目录。

27 项 filesystem 在 owner 下两侧均 PASS。第 28 项
`thread_create_failure.pass.cpp` 通过 `setrlimit(RLIMIT_NPROC, 1)` 强制线程创建
失败；root 可绕过该限制。R77 原补丁结果由未执行 `su owner` 的旧执行器得到，
退出 134 且断言 `assert(false)`，本轮同一补丁二进制在 owner 下退出 0；基线
owner 也退出 0。因此它是执行身份造成的环境差异，不是 future 补丁回归。

第一阶段的详细夹具结构、四项 root/owner 双向抽样及残留分析仍保留在本目录
原始材料中。

## `offset_range.pass.cpp`

运行前板上 `/opt/usr` 可用约 92.4 GB；测试写入量为 4,294,967,362 字节，
高于所需空间并另保留 8 GiB 安全余量，因此允许成对复验。

lit 的单项运行受 120 秒执行器上限影响，故最终直接执行 lit 已构建的同一测试
二进制，并继续使用同一 owner SDB 执行器、独立目录和目标运行库：

- 基线：退出 0，约 117.94 秒；
- 补丁：退出 0，约 119.59 秒。

R77 的基线 TIMEOUT 与补丁 PASS 是 120 秒边界附近的时间环境差异；owner 成对
结果相同。没有把超时推断成库行为差异。

## 基线补跑

从 R77 已完成的 7,000 项断点继续，批次 015--022 按原纪律以 `-j2`、
`nice -n 15`、`ionice -c 3` 执行；批次 023 和 libc++abi 在系统异常后降为
`-j1`。完整 result JSON、lit stdout/stderr、遥测和每批 COMPLETE 标记均在
`raw/phase2/results/`。

- libc++：原 7,000 项 + 本轮 4,321 项 = 11,321/11,321；23 个批次标记齐全；
- libc++abi：81/81，lit 退出 0；
- 基线运行库板端 SHA256：libc++
  `d293b1a3bed8a4114b5a7c30d3732a4299a52221fd7cc7d5aada9a369899a737`，
  libc++abi
  `58b8bcc8ffe95b78bb1a3d5951efe9507ddf44400fe3e6ad8debeed09a93b374`。

设备连接在本轮发生两次中断，均立即停止当前运行、保存已完成批次并检查板端
进程、夹具、运行库 SHA256 和空间；总连接恢复次数为 2/3。未观察到系统崩溃
的可证明根因，记 `NOT_OBSERVED`，不归因于某个命令或补丁。

第二次恢复后，为响应“控制 CPU 和内存消耗”的明确要求，采取以下保护：

- 只运行剩余单批和 libc++abi，`-j1`；
- 每 60 秒记录负载和可用内存；
- 可用内存低于 8 GiB 或 load1 高于 20 即主动停止，比原 1 GiB 门槛保守；
- 一次 batch-023 重试在 load1=23.33 时主动终止，保存为
  `interrupted_attempt_2_high_load`；等待连续两个低负载样本后重跑成功。

最终成功的 batch-023 期间 MemAvailable 约 18.7--19.6 GiB、load1
9.93--11.97；libc++abi 期间约 17.5--18.4 GiB、load1 10.93--11.81。
第二阶段从 15:27:37 开始，在 22:27:37 的七小时截止点前完成。

## 79 项和原 28 项的重新分类

原 79 项 `INDETERMINATE` 已全部定性，见
`tables/r77_79_indeterminate_resolved.tsv`：

- 78 项：基线与补丁均 FAIL，`UPSTREAM_EXISTING`；
- 1 项 `thread_create_failure.pass.cpp`：owner 下两侧均 PASS，
  `ENVIRONMENT_CAUSED`；
- 方案引入 0，无法判断 0。

R77 原 28 项“基线失败、补丁通过”在隔离后均无差异，见
`tables/r77_28_resolved.tsv`：27 项由 root/owner 身份漂移造成，1 项
`offset_range` 由超时边界造成，全部归为 `ENVIRONMENT_CAUSED`。

## 五处 `noexcept` 边界和 25 类差异

五处边界逐项记录在 `tables/noexcept_boundary_coverage.tsv`。官方套件没有
`pthread_cancel` 用例，因此 D01--D05 五个精确取消触发均未直接命中；只能确认
condition_variable、future、stream 等普通路径被运行。不得把普通路径通过写成
取消边界已验证。

25 类差异逐项记录在 `tables/known_25_coverage.tsv`。主要结果：

- D01：libc++abi `forced_unwind1/2/3` 两侧 PASS，`forced_unwind4` 在 armv7l
  UNSUPPORTED；精确 noexcept 取消边界未命中；
- D02：`noexception1..4` 两侧均 UNSUPPORTED；
- D03、D05、D10、D20、D21、D22：只获得表中所列的部分覆盖；
- D24：选定 x86_64/armv7l 配置的全量对照缺口已关闭；
- D04、D06--D09、D11--D19、D23、D25：本轮未直接验证。

## 清理

收尾前确认无 `t.tmp.exe` 或 R77 run 进程，记录目录清单后删除两个精确测试根：

- `/opt/usr/home/owner/share/tmp/r77_lit_20260829`
- `/opt/usr/r81_phase2`

最终核验为 `R77_ABSENT`、`R81_ABSENT`、测试进程 ABSENT，`/opt/usr` 可用
90,200,176 KiB。删除的是可由归档脚本和本地构建产物重建的板端临时夹具，板上
原目录本身不可恢复。未安装软件、未改系统配置、未重启。

## 技术性错误与自行判断

技术性错误均保留原始失败记录，未改判定对象：

- 批次包装首次将 `batch-023` 写成 `batch-23`，修正路径格式后重跑；
- 一次 SDB 输出带 CRLF，严格字符串核验误报，去除 CR 后复核；
- 候选证据初次使用过宽检索，产生较大输出；后续改为 JSON 定点提取；
- 最终分析脚本首次只读入 R77 新增 11 个批次而漏掉复用的 R68 前缀，命中
  test-set mismatch 并停止；改为读取 R77 的 11,321 项聚合文件后通过；
- 一次记录包装参数少传标签导致脚本权限错误；一次板端只读进程检查的 grep
  引号错误；均落盘并使用明确命令复核；
- 一次 runtime 部署脚本因任务根已存在拒绝部署，按既有清理脚本清理精确 R77
  根后正常部署，没有绕过门禁。

自行判断：

1. 将 owner 选为统一身份，理由见上文；
2. 身份机械扫描采用保守候选集，再用动态对照确认；没有因静态扫描漏掉
   `RLIMIT_NPROC` 就扩大为全套重跑，只补做该单项；
3. `offset_range` 在空间足够时使用已由 lit 构建的精确二进制成对执行，以避免
   discovery 时间计入 120 秒测试上限；
4. 系统两次异常后把剩余并行度从任务书的 2 降为 1，并采用更严格资源停止线；
5. 两侧共同 FAIL 按任务口径记 `UPSTREAM_EXISTING`，不声称已查明每项根因。

尚存疑问：无阻塞性疑问。系统两次崩溃的根因未观测到；精确五处取消边界、
aarch64、其他编译配置、两套标准库共存期和产品包级 ABI/部署场景仍是未覆盖
范围。

## 纪律与未覆盖范围

- 未修改平台源码、libc++/libc++abi 源码或任何定稿补丁；
- 未向 Gerrit、LLVM 上游或任何外部源码仓推送；本次只提交本项目证据仓；
- 原始命令、stdout/stderr、退出码、批次 JSON 和遥测完整保存在本目录；
- 未覆盖：aarch64、LLVM 套件其他标准/编译器/异常配置、共存期、产品包级构建
  与 ABI、异步取消精确落入五处 noexcept 边界。

## 材料导航

- `tables/final_summary.json`：最终计数；
- `tables/full_failure_classification.tsv`：213 项共同失败逐项分类；
- `tables/r77_79_indeterminate_resolved.tsv`：原 79 项逐项结论；
- `tables/r77_28_resolved.tsv`：原 28 项逐项结论；
- `tables/identity_sensitive_tests_final.tsv`：28 项身份敏感清单和依据；
- `tables/libcxxabi_owner_comparison.tsv`：81 项 libc++abi 对照；
- `tables/noexcept_boundary_coverage.tsv`、`known_25_coverage.tsv`：已知发现对照；
- `raw/phase2/results/`：完整补跑结果；`raw/*.command.txt`、`*.stdout`、
  `*.stderr`、`*.exitcode`：命令原文和退出码；
- `code/`：执行包装、资源保护和离线分析脚本。
