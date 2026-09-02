# R92：未扫源码包的补扫

## 结论

R87 的零命中结论在本轮新增的可得源码范围内仍然成立。

R90 严格按 R11 冻结快照核算出的 1,105 条源码缺口中，有 140 条产出至少一个带 C++ 指标的已发布 ELF。本轮：

- 140/140 个本地 SRPM 均通过既有 SHA256 复核；
- R87 原版 `scan_noexcept_type_uses.py` 对 140/140 个展开根正常完成，脚本内容未修改；
- 138 个 SRPM 含可展开的上游源码，完成源码层补扫；
- 2 个 SRPM 只有打包文件、没有上游 C/C++ 源码，源码内容记 `NOT_AVAILABLE`；
- 原脚本产生 150 条宽口径候选，去重为 100 个位置；全部分类完毕，真实 D01–D05 函数地址、函数类型、模板参数、traits/noexcept 用法为 0；
- 140 个包对应的 4,374 条 C++ 指标 ELF 全部属于 R87 已完成的 39,166/39,166 ELF 扫描范围，没有新增二进制层缺口。

本任务没有修改 R87 的材料、脚本或判定逻辑，没有做修复或处置建议。

## 1. 覆盖关系

使用与 R90 相同的三个冻结输入机械重建集合：

- `progress/R11/tables/source_download_status.tsv`：R11 的 1,450 条源码记录；
- `docs/progress/R32/tables/scan_status.tsv`：R87 使用的 371 个 `SCAN_OK` 根；
- `progress/R11/tables/elf_inventory.tsv`：已发布 ELF 与 source RPM 的映射。

结果见 `tables/coverage_summary.tsv`：

| 指标 | 数量 |
| --- | ---: |
| R11 冻结源码记录 | 1,450 |
| R87 原源码语料 `SCAN_OK` | 371 |
| 与 R11 文件名精确相同的已扫记录 | 345 |
| R11 精确源码缺口 | 1,105 |
| 缺口中产出 C++ 指标 ELF 的源码记录 | 140 |
| 140 包对应的已发布 C++ 指标 ELF 行 | 4,374 |
| 已包含在 R87 ELF 扫描范围 | 4,374/4,374 |

R87 的二进制扫描输入是 R11 的全部 ELF shard，结果为 39,166 个 ELF 记录全部处理。`tables/cpp_gap_binary_coverage.tsv` 逐行把 140 个 source RPM 映射回这 4,374 条 ELF。因此本轮主要补的是可能只存在于源码语义中的形态，例如 traits、`decltype`、bool 模板参数、别名和重载选择；ABI 中仍带目标编码的用法此前已经有二进制层兜底。

## 2. 补扫口径与执行方法

### 2.1 清单与来源

`tables/cpp_gap_140.tsv` 列出全部 140 条，包含包名、压缩大小、本地路径、记录 SHA256、文件存在性和关联 ELF 数。140 个文件全部已经由 R11 下载，不进行网络下载。

### 2.2 展开与资源控制

逐包串行执行：

1. 重新计算 SRPM SHA256；
2. 临时展开当前 SRPM；
3. 安全展开内嵌 tar/zip 源码归档；
4. 为当前根生成单条 `SCAN_OK` 输入；
5. 调用 R87 原脚本，`--workers 1`；
6. 持久化状态、候选与命令 ledger；
7. 删除当前包临时树，再进入下一包。

执行使用 `nice -n 15` 与 `ionice -c 3`，没有构建，并行度为 1。设置 80 GiB 可用磁盘保护线；低于该值会停止。峰值最大的 chromium 包也只在当前包生命周期内展开，完成后缓存和临时树已清除；最终可用磁盘恢复到约 335 GiB。

共识别并处理 195 个内嵌源码归档：187 个普通完成，8 个按安全策略跳过链接、非普通或不安全成员后完成。合计跳过 1,003 个此类成员；该计数没有进一步区分链接与不安全路径，是本轮局限之一。实际源文件目标仍由普通成员扫描，但若某源码只存在为被跳过的硬链接，则可能遗漏。

R87 扫描器 SHA256：

`e81bd6f7afec0fa932430819f3f2f292c4f0bdc1230a9e8efeba9ebdb92629a9`

`git status -- docs/progress/R87` 为空，证明既有 R87 材料未修改。

## 3. 源码可得性

机械执行层面，140/140 个根都被原脚本正常处理。内容完整性层面分为：

| 状态 | 数量 | 说明 |
| --- | ---: | --- |
| `AVAILABLE_AND_SCANNED` | 138 | SRPM 含上游源码归档，展开并扫描 |
| `NOT_AVAILABLE` | 2 | SRPM 只有打包文件，没有上游 C/C++ 源码 |

两个 `NOT_AVAILABLE` 项逐项如下：

1. `mic-bootstrap-1.0-1.src.rpm`：仅有 `baselibs.conf`、`mic-bootstrap.spec`、`rpmlintrc`；spec 只有 `Source100/Source101` 指向这两个打包辅助文件，没有上游源码 Source。
2. `qemu-accel-0.4-1.4.src.rpm`：仅有 `qemu-accel-aarch64.spec`，没有上游源码 Source。

这两个根上运行扫描器得到零候选，只能说明其 SRPM 自带打包文本中没有命中；不能冒充上游 C++ 源码已经扫过。两者的已发布 C++ 指标 ELF 仍包含于 R87 二进制扫描范围。逐项证据见 `raw/011_*` 与 `tables/source_completeness.tsv`。

## 4. 候选复核

原脚本输出 150 条宽候选、100 个去重位置，涉及 chromium、LLVM 和 10 条 GCC/GCC 交叉工具链 source RPM 记录。`tables/candidate_classification.tsv` 保留每一行原文、位置、分类与理由；分类脚本遇到未知形态会返回非零，本轮未知形态为 0。

| 分类 | 原始候选行 | 去重位置 | 真实目标用法 |
| --- | ---: | ---: | ---: |
| condition_variable 内部 `__condvar` **数据成员**指针，不是 `wait` 函数指针 | 100 | 50 | 0 |
| `__do_timed_wait` 自身的声明、定义或普通直接调用 | 34 | 34 | 0 |
| `wbuffer_convert` 析构函数自身的声明或定义 | 14 | 14 | 0 |
| 无关的 `atomic::wait` noexcept 查询 | 2 | 2 | 0 |
| 合计 | 150 | 100 | 0 |

其中 100 行数据成员指针候选是同一批 50 个位置被 `MEMBER_POINTER_TYPE` 与 alias 分支各保留一次。它们的类型是 `std::__condvar std::condition_variable::*`，没有取得 D01 `wait` 的地址，也不携带 `wait` 函数类型的异常规格。其余三类同样不是任务所查的地址、函数类型、非类型模板参数或 traits 使用。

`tables/true_hits.tsv` 仅有表头，真实命中为 0。

## 5. 原结论是否变化

不变化，但边界必须精确表述：

> 在 R87 原 371 个源码根、本轮新增 138 个具有上游源码内容的精确 R11 快照记录，以及 R87 已覆盖的 39,166 个发布 ELF和 8,911 个静态归档中，未发现平台以会受异常规格变化影响的函数指针、函数类型、模板参数或 traits 等形式使用 D01–D05。

不能表述为“1,450 个源码包源码绝对全部零命中”，因为本轮 2 个 packaging-only SRPM 没有携带其已发布 C++ ELF 所对应的上游源码，且任务刻意没有补扫 965 个无 C++ ELF 指标的源码记录。

## 6. 剩余盲区

1. 上述 2 个 packaging-only SRPM 的上游 C/C++ 源码来源没有在包内提供，源码层为 `NOT_AVAILABLE`；其二进制产物已覆盖。
2. R90 缺口中的另 965 条没有已发布 C++ 指标 ELF，按本任务范围未补扫。它们仍可能包含仅用于静态库、构建工具、测试、生成代码或最终被剥离/内联的 C++。
3. 源码正则扫描不能展开编译期宏、生成器输出或外部拉取内容，也不能证明语义已完全擦除的 bool/SFINAE 结果来自哪个表达式。
4. 安全解包跳过了 1,003 个链接、非普通或不安全成员；未对该合并计数逐个恢复链接语义。
5. 结论绑定 R11 冻结快照；当前仓若已有版本漂移，不属于这次补扫。

## 7. 执行异常与恢复

所有命令原文、输出和退出码均在 `raw/`；逐包提取与扫描子命令在 `raw/source_scan_command_ledger.txt`。

1. 第一次处理大于 4 GiB 的 SRPM 时，辅助脚本错误地捕获了 `rpm2archive` 的标准输出，导致内存增长。发现后立即精确中止，命令退出码记录为 130；已完成 5 包断点未丢失。
2. 修正为标准输出直接流式写磁盘后，导出成功；随即发现辅助脚本预先创建了本应由安全解包函数创建的目录，以 `FileExistsError` 退出。判据未执行、断点未变化。
3. 仅修正 R92 辅助脚本的流向与目录前置条件，保留已成功生成的磁盘流缓存，再从断点续跑。最终 140 个根全部完成；没有修改 R87 扫描器。
4. 执行异常退出类运行实验时环境曾连续出现保护性中止提示；W2 后续只进行本地静态处理，并减少工具回显。没有尝试绕过保护。

这些均为命令自身技术性错误，不是扫描判据失败。

## 8. 自行判断与尚存疑问

自行判断：

1. 以 R11 文件名精确差集 1,105 为冻结快照分母，不使用简单算术差 1,079；这沿用 R90 已说明的口径。
2. 用 `cpp_indicator=YES` 把任务指定的 140 条从差集中机械选出。
3. 将原扫描器的宽候选逐项分类，只有实际取得目标函数地址/类型、把它们用于模板/traits，或其他会携带异常规格的形态才算真实命中。
4. 对两个 packaging-only SRPM，区分“扫描器正常扫描了现有根”和“上游 C++ 源码不可得”，后者如实记 `NOT_AVAILABLE`。

尚存疑问：若人工要求对两个 packaging-only 项取得源码层结论，需要确定其发布 ELF 的真正上游源码/构建来源；现有 SRPM 与本地冻结材料不能回答。该问题不阻塞对其二进制层的既有结论，也不阻塞本轮其余 138 项收口。

## 9. 证据索引

- 140 条清单：`tables/cpp_gap_140.tsv`
- 4,374 条二进制覆盖映射：`tables/cpp_gap_binary_coverage.tsv`
- 覆盖统计：`tables/coverage_summary.tsv`
- SRPM 内容清单：`tables/srpm_payload_inventory.tsv`
- 逐包扫描状态：`tables/source_scan_status.tsv`
- 内容完整性：`tables/source_completeness.tsv`
- 归档处理：`tables/archive_status.tsv`
- 原始宽候选：`tables/source_candidates.tsv`
- 全候选分类：`tables/candidate_classification.tsv`
- 分类汇总：`tables/candidate_classification_summary.tsv`
- 真实命中表：`tables/true_hits.tsv`
- 最终统计：`tables/final_summary.tsv`
- 命令与完整输出：`raw/`

## 10. 纪律确认

- 未修改平台源码或任何补丁。
- 未修改 R87 扫描脚本和既有结论材料。
- 未做修复或处置建议。
- 未向 Gerrit 或任何外部源码仓推送。
