# W3：Base 层 11 包最终状态汇总

## 结论

汇总任务为 `COMPLETE`；包实施本身仍为 **8/11 完成、3/11 暂停**。
本任务没有改变任何包的“需要改/不需要改/当前不能改”判定、理由或证据，
没有修改平台源码或配置，也没有触发 QuickBuild。

## 输入身份

中文 254 包表来自
`docs/progress/R109/tables/base_all_source_packages_updated.tsv`；英文 254 包表
及中文 Base 报告来自
`docs/progress/R114/deliverable/hq_20260904/`。输入副本及 SHA256 保存在
`source/`，完整哈希见 `raw/006_validate_status_update.stdout.txt`。

远端核对确认八个已完成包的 sandbox HEAD 与既有记录完全一致；LLVM、
`bcc-tools`、`bpftrace` 的同名 sandbox 分支仍分别停在
`2d23367d…`、`dd26ba03…`、`30e51cd…`，没有适配提交。完整结果见
`PACKAGE_STATUS.tsv` 与 `raw/003_remote_branch_heads.stdout.txt`。

## 11 包状态

| 状态 | 数量 | 包 |
| --- | ---: | --- |
| 已完成并推送 | 8 | `abseil-cpp`、`boost`、`icu`、`jsoncpp`、`libsigc++`、`pcre`、`taglib`、`tensorflow2` |
| 暂停，未推送适配提交 | 1 | `llvm` |
| 前置不可得，未实施 | 2 | `bcc-tools`、`bpftrace` |

七个常规包的三架构 × libc++/GCC 六格完整通过；`tensorflow2` 沿用已经
人工确认并实施的相对基线判据，不能改写为六格完整构建全成功。各 SHA、
格子结论及本地 push 日志时间见 `PACKAGE_STATUS.tsv`。Gerrit 的
`ls-remote` 不提供服务端 ref 更新时间，因此表中“推送时间”是对应成功
push 输出文件的本地写入时间；它是本次可核实的直接时间证据，不冒充服务端
审计时间。

LLVM 的 TF 2.18 候选已经完成三架构静态/配置核查、armv7l 与 x86_64
libc++ 全量构建及产物核查，以及 x86_64 原生 MLGO 执行；aarch64 在
7,376/7,545 中止，其余硬门禁未闭合，所以没有提交或推送。W2 按明确前置
条件没有启动后两个包。

## 对外材料更新

`updated/base_packages_zh.tsv` 与 `updated/base_packages_en.tsv` 仅改变
`llvm`、`bcc-tools`、`bpftrace` 三行的“当前实施状态”列。校验确认：

- 两份表均为 254 行数据加表头；
- 每行前八列与各自输入逐字节一致；
- 状态列恰好改变三行；
- 判定计数仍为需要改 11、不需要改 237、当前不能改 6、未定 0；
- 两份 Markdown 均由更新后的 TSV 机械生成，数据行均为 254。

中文 Base 报告只更新“实施进度/当前阻断状态”段；英文侧提供同内容的独立
状态说明 `updated/base_adaptation_status_en.md`。仓内没有一份可识别为该
中文报告逐段对应的既有英文全文，因此没有自行新译整份报告；英文 254 包
清单已经完整更新。这一处理列入自行判断，供人工决定是否还需把状态段并入
仓外的英文报告版本。

## QuickBuild 输入

`QUICKBUILD_INPUT.md` 列出当前 READY 的 `libcxx-runtimes` 和八个独立包，
以及仍为 NOT_READY 的 LLVM 链三包。依赖顺序保持：

```text
libcxx-runtimes → llvm → bcc-tools → bpftrace
```

条文称“其余 7 个无依赖”，但 11 包清单扣除三包 LLVM 链后实际为八个。
本报告保留全部八包，没有自行删除一个来凑数，并把疑问写入共享疑问文件。

## 未覆盖范围

- 未触发 QuickBuild，因而没有 QuickBuild 产物或调度结果；
- LLVM 的 aarch64 完整产物、armv7l/aarch64 模型执行及三架构兼容构建
  均未观测；
- Gerrit 服务端精确 ref 更新时间 `NOT_AVAILABLE`，仅保留本地成功 push
  日志的写入时间；
- 仓内未找到既有 Base 报告英文全文，只产出英文状态说明和更新后的英文
  254 包清单。

## 自行判断与尚存疑问

1. 自行判断：把成功 push 输出文件的本地写入时间作为“推送时间证据”，
   并明确限定其性质；没有把 commit 时间冒充 push 时间。
2. 自行判断：由于没有既有英文 Base 报告全文，只新增英文状态说明，不重译
   或改动判定内容。
3. 尚存疑问：任务书的“其余 7 个”与实际八个独立包不一致；QuickBuild
   清单应否对其中某一包采用另行的前置顺序，需要人工确认。
