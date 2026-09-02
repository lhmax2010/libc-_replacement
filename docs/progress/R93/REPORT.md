# R93：过夜任务材料收口

## 结论

本任务只修正 R82/R92 的材料、状态和证据对应关系，没有重跑实验、没有修改
已有原始输出。

- R92 状态从 `COMPLETE` 改为 `PARTIAL`。
- R92 原始覆盖口径修正为 130 包完整扫描 / 8 包部分覆盖 / 2 包源码不可得。
- 对 8 包的 1,003 个排除成员完成归档目录元数据分类：全部为符号链接，
  994 个目标已在扫描根内，9 个目标不在归档，无法分类的成员为 0。
- 据此 6 包完成收口；bcc-tools 与 chromium-efl 保守保持部分覆盖。最终口径为
  136 包完整 / 2 包部分 / 2 包源码不可得。
- R87 的 ELF 准确口径为 39,166 条中 1 条未解析；失败项属于
  `u-boot-rubikpi3`，不在本次 140 包/4,374 条范围，故 R92 的具体结论不变。
- R92 的退出码 130 已标注为恢复记录。
- R82 增加 raw/034 对应的两用例执行等价重建脚本，并明确不能声称逐字节恢复。

## 1. 状态与 R92 三档修正

`docs/progress/OVERNIGHT_STATUS_0901.md` 的 R92 已改为 `PARTIAL`，同时记录：

| 阶段 | 完整 | 部分 | 源码不可得 |
| --- | ---: | ---: | ---: |
| R92 原始执行后 | 130 | 8 | 2 |
| R93 元数据收口后 | 136 | 2 | 2 |

`docs/progress/R92/tables/source_completeness.tsv` 已逐包改为：

- `AVAILABLE_AND_SCANNED`：130；
- `AVAILABLE_AND_SCANNED_R93_LINKS_CLOSED`：6；
- `PARTIAL_DANGLING_LINK_TARGETS_NOT_AVAILABLE`：2；
- `NOT_AVAILABLE`：2。

同步修正 `docs/progress/R92/tables/final_summary.tsv` 与 R92 报告。原候选、扫描
状态、归档状态和 raw 输出未改。

## 2. 8 包排除成员分类

### 2.1 方法

没有重做源码扫描，也没有展开源码树。只从现有冻结 SRPM 流式读取对应源码
归档的 tar 目录头，逐条复用 R92 当时的纳入规则：安全路径的普通文件或目录
纳入，其余排除。记录成员类型、链接目标及目标是否解析到归档内已纳入的普通
文件/目录。

分类总数必须与 R92 `archive_status.tsv` 的 1,003 精确一致，否则辅助脚本返回
非零。实际为 1,003/1,003。

### 2.2 类型统计

| 类型 | 数量 |
| --- | ---: |
| 符号链接 | 1,003 |
| 硬链接 | 0 |
| 普通文件 | 0 |
| 目录 | 0 |
| FIFO/设备/其他 | 0 |
| 不安全路径成员 | 0 |
| 无法分类 | 0 |

链接目标分类：

| 目标状态 | 数量 | 判定 |
| --- | ---: | --- |
| 归档内普通文件/目录，已位于扫描根 | 994 | 链接没有独有文件内容，目标内容已纳入原扫描根 |
| 目标不在归档 | 9 | 链接本身没有文件内容，但不能证明缺失目标源码已扫描，保守保持缺口 |

对于 994 个内部目标，原扫描器仍只读取其既定 C/C++ 文件 glob；这里的“已在
扫描范围”表示目标作为安全普通文件/目录存在于传给 R87 的根中，不扩张原脚本
的文件类型口径。

### 2.3 逐包更新

| source RPM | 排除链接 | 内部目标已纳入 | 目标不在归档 | 更新后判定 |
| --- | ---: | ---: | ---: | --- |
| bcc-tools-0.35.0-3.1 | 70 | 69 | 1 | 部分覆盖 |
| chromium-efl-1.1.144-1 | 870 | 862 | 8 | 部分覆盖 |
| dotnet-build-tools-8.0.404-1 | 36 | 36 | 0 | 完整收口 |
| elfutils-0.189-1.14 | 2 | 2 | 0 | 完整收口 |
| elfutils-0.189-1.16 | 2 | 2 | 0 | 完整收口 |
| heaptrack-1.4.5-1 | 1 | 1 | 0 | 完整收口 |
| llvm-22.1.8-19.1 | 19 | 19 | 0 | 完整收口 |
| org.tizen.vc-engine-default-1.90.1-1 | 3 | 3 | 0 | 完整收口 |

无法分类成员为 0。9 个悬空链接逐项保存在
`tables/dangling_symlinks.tsv`；完整 1,003 行位于
`tables/excluded_archive_members.tsv`。

## 3. R87 ELF 表述修正

R87 表中的实际数字是：

- ELF records：39,166；
- parse failures：1；
- 失败 source RPM：`u-boot-rubikpi3-2025.10-0.src.rpm`；
- 失败路径：`/boot/u-boot.mbn`；
- 原因：扫描器未实现该文件使用的扩展 section count。

机械连接 `cpp_gap_binary_coverage.tsv` 后确认：失败 source RPM 不在 R92 的
140 包清单，失败路径也不在 4,374 条明细中。因此应表述为“本次 4,374 条均
被 R87 成功解析”，不能再表述为“R87 39,166/39,166 全部完成”。证据见
`tables/r87_elf_failure_scope.tsv`。

## 4. 退出码 130 的恢复记录

`docs/progress/R92/raw/004_scan_cpp_gap_sources.exitcode` 的 130 不是日志包装器
自动写入。包装器在中断时尚未来得及生成 exitcode 文件；该值后来依据统一执行
会话实际返回的 130 补写。已有 stdout/stderr 没有修改。

现已增加
`docs/progress/R92/raw/004_scan_cpp_gap_sources.exitcode.recovery-note.txt`，并在
R92 报告中写明来源。

## 5. R82 脚本版本收口

`raw/034_run_arm_local_matrix.stdout` 只含 filesystem、iostreams 两个用例，
但归档的 `run_arm_local_matrix.sh` 后来已扩展为三个用例。执行当时没有保存脚本
SHA256 或不可变快照，因而无法证明任何重建文件与当时文件逐字节相同。

本次增加：

- `docs/progress/R82/code/run_arm_local_matrix.executed-034.reconstructed.sh`：依据
  原始输出、当时路径配置和后续增量修改重建的执行等价两用例版本；
- `docs/progress/R82/code/EXECUTED_SCRIPT_VERSIONS.md`：说明当前脚本、重建脚本
  与 raw/034 的关系和限制；
- R82 报告中的版本说明。

验证只比较静态执行集合：raw/034 的唯一 `CASE=` 为 filesystem、iostreams；
重建脚本循环同两项；当前脚本循环三项。没有执行其中任何二进制。

以后每次执行前，应把脚本复制为编号化不可变快照、记录 SHA256，并让命令引用
快照；多文件脚本同时保存依赖清单和各文件 SHA256。

## 6. 技术性非零

- `raw/003_*`：已有目录定位命令最后一个 glob 未命中，shell 返回 1；命令仍
  找到 5 个相关路径，不是材料判据失败。
- `raw/005_*`：限时的只读文件名查找在 30 秒到期，返回 124；未得到所需大包
  归档，随后改用 SRPM 流式目录元数据，不依赖该结果。
- `raw/008_*`：首次从大 SRPM 外层 tar 选择成员时遗漏路径前缀，返回 1；改为
  basename 通配后 `raw/010_*` 成功，分类总数严格为 1,003。
- `raw/018_*`、`raw/019_*`：材料一致性检查把不交付的临时 `work/` 或校验
  命令自身刚生成、尚未复制的日志也纳入目录比较，分别返回 1；内容计数门禁
  本身已通过。`raw/020_*` 收敛比较范围后全部通过。

## 7. 修改范围与纪律

修改/新增范围：

- `docs/progress/OVERNIGHT_STATUS_0901.md`；
- `docs/progress/R92/REPORT.md`、完整性/覆盖汇总与映射表、清单生成辅助代码及退出码恢复说明；
- `docs/progress/R82/REPORT.md`、执行等价重建脚本和版本说明；
- `docs/progress/R93/` 本报告、分类表、辅助代码和完整命令输出。

没有修改 R82/R92 已有原始 stdout/stderr/command/exitcode 文件，没有重跑实验，
没有修改平台源码或补丁，没有向 Gerrit、正式分支或 LLVM 上游推送；仅按任务
要求向本项目仓提交交付材料。
