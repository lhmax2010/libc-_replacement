# 方法、执行记录与可复核范围

## 范围冻结

人工批准逐族分析 Dali/NUI 8 族与 Web 3 族；Native 12 族只作快速确认并辨别例外；其余四族仅介绍。
第一阶段提交为 `31a92b1af1ca702fd24e3875b16d48a8dbff69ae`。不改变其历史报告、统计或 SHA256 清单。
本批结果仅位于 `docs/progress/P7_0909/stage2/`，不修改平台源码、配置、运行库或其他分支。

## 原生头及 SDK 关联

复用第一阶段已验证的代表 RPM、逐头摘要及 public/internal rootstrap 交集。
本轮再读内容重算 SHA256，沿用修正过的形态规则；只将已人工读到的真实签名/实现用于最终分类。
`tables/headers.tsv` 的 2,923 条记录包含原建议范围的可见头，仅作定位母表；
其中已批准的 11 族有 1,690 条（代表架构全为 x86_64），其余四族没有逐接口分析。
Native 快速扫描只取 12 族中 public 配方必要路径相交的 287 条记录。

Native 排除路径名明确出现 internal/private/implementation/integration-api/devel-api 的 67 条后，
剩余 220 条：100 条 C 形态、105 条 C++ 信号、15 条未定。12 族中 11 族有 C++ 信号，另一族剩余 6 条为 C 形态。
这个按名字排除的方法已经申报；不是对“全部真实公开 API”作完备识别。public 配方的通配带入与实际 SDK 产品白名单仍需区分。

`boundary_headers.tsv` 是受检头的分层表；`boundary_signal_index.tsv` 保留候选非内部头中标准库/throw/生命周期词法行号。
后者不是 AST、链接图或完整函数目录：私有成员、头内实现、类型别名及真实跨库传递不能按词频混算。
最终 `API_CLASSIFICATION.tsv` 的 22 行是解释边界的 API 组/代表路径，不是平台接口总数，也不是测试格数。

正向对照包括 C guard、标准库 C++ 类、注释伪命中、C/C++ 混合四个小样本，结果保留在 `tables/scan_controls.json`；
真实代表 `inputmethod.h`、`app.hpp`、bundle_cpp.h 和混合的 ewk_cookie_parser.h 另有人读原文。
不从源码搜索零命中得出全平台“不存在”。

## 源码与产物身份

- 既有源码缓存只读。缺少已解包 TizenFX 时，从本地已缓存、匹配冻结 SHA256 的 230,118,341 字节源码 RPM
  读取 NUI 相关归档成员至 `tmp/P7_0909/csapi/`；没有下载、安装或构建新运行库。
- 选读源码全文快照由 source_view.py 留存；其 stdout 同时记录原始位置、SHA256、展示行号。
  verify_identity.py 核对“选读文件 → tar 成员 → 源码 RPM 文件摘要 → 冻结仓 SHA256”，而非只比同名版本。
  见 `tables/source_identity.tsv` 与 `tables/evidence_files.tsv`。
- binary_evidence.py 只读 x86_64 ELF，核对 RPM SHA256、RPM 载荷文件摘要后读取符号表和动态依赖。
  14 + 3 = 17 个 ELF（含顺带在所选产物内的辅助库），**不是 17 个混库测试**。
  主 binder 位于 `dali2-csharp-binder` 包，最先查询的 profile_common 包没有列出文件；没有将该空包误报为 binder 不存在。
- .pc 为已交付的链接元数据，不执行 pkg-config 生成构建，也不据此断言最终 SDK 开放。
  `tables/pkgconfig.tsv` 记录来源包/路径和快照摘要；第一阶段 RPM 查询记录可用于复核。

## 探查失败与修正（原始输出保留）

- raw/005 探查时使用了错误的补充表文件名，退出 1；随后使用实际存在的 managed_sdk_files.tsv 等材料。
- raw/007 调用已有形态函数漏传 path，退出 1；raw/008 修正参数后完成，摘要和形态复核通过。
- raw/021、028、047、083 的候选源码文件名/后缀未匹配，退出 1；分别改用已枚举的 common.h、utils_api.js、cscl-ui-nui.h、实际 C# 源文件。
  没有将这些缺省文件名错误当作接口不存在。
- 初版查看助手名为 inspect.py，异常诊断时遮蔽 Python 同名标准模块；已原样改名 source_view.py。
  旧调用的命令和错误保留；生成的本任务字节码缓存不作为证据提交。该问题不改变被读源码。

所有受检命令的原文、stdout、stderr、退出码和 UTC 起止时刻落 `raw/`。
报告/脚本通过 apply_patch 编写，主报告的新增 diff 与脚本 SHA256 一并封存。diff 命令退出 1 表示存在差异，不是核查失败。
旧探查结果不覆盖为成功结果。
用于生成材料自身提交/推送的回执放本地 `delivery/`，不递归装进它们所记录的同一个提交。

## 未做的事

不编译、不跑开发板、不运行混库探针、不设置平台环境或部署策略、不推 Gerrit。
本批未观测任何新 API 运行成功/失败，也没有新的跨架构结论。
报告中的 C 化难点是明确标记的技术推断；没有给实施建议、迁移顺序或开放资格裁决。
