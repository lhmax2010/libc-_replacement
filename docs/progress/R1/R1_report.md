# R1：T1 人工裁决后重分类报告

状态：`DONE`。本任务没有重扫语料；唯一聚合输入仍是既有 `progress/T1/corpus_hits.tsv`（`LOCAL_ONLY`，58,590,448 字节，SHA256 `cd2a91832f5a548b4e762c9cc42c4349c50d4f82e559058760f1cf6f83a6c7c2`）。原 T1 文件未修改，身份登记见 `progress/R1/input_identity.tsv:1`，旧结论的取代关系见 `progress/R1/supersession_registry.tsv:1`。

## 新分档

规则全文见 `progress/R1/classification_rules.tsv:1`。`__forced_unwind`、`__gnu_cxx`、`#include <ext/`、`std::__cxx11` 以及经 RPM 文件清单实证的 `LIBSTDCXX_BITS` 进入 `T1_HARD`；ABI、GLIBCXX 与 pthread cancel 系进入 `T2_JUDGE`；显式 `-lstdc++` 和构建系统文件中的 libstdc++ 进入 `T3_BUILD`。逐包 371 行、允许一包多档的结果见 `progress/R1/package_three_tiers.tsv:1`。

三个发生率见 `progress/R1/occurrence_rates.tsv:1`：

| 指标 | 分子/分母 | 发生率 | 口径 |
|---|---:|---:|---|
| T1_HARD | 45/371 | 12.129380% | 硬模式加 RPM 实证的 LIBSTDCXX_BITS |
| 含 T2_JUDGE 的 fail-closed 上界 | 112/371 | 30.188679% | `T1_HARD ∪ T2_JUDGE ∪ UNRESOLVED_BITS` |
| T3_BUILD | 90/371 | 24.258760% | `-lstdc++` 或构建系统 libstdc++ |

上界把 23 个含不可归属 bits 头的包并入，避免把未知项当作无影响；它不是“确定需改”率。

## bits 实测拆分

实际所有权来自现存 armv7l target 缓存的 `glibc-devel-2.40-2.8` 与 `libstdc++-devel-14.2.0-1.14` 的 `rpm -qlp` 清单，提取命令、SHA 和退出码见 `progress/R1/commands/06_freeze_ownership_inputs.log:1`；具体方法见 `progress/R1/bits_methodology.tsv:1`。199 个实际头文件名、命中和所有权见 `progress/R1/bits_header_classification.tsv:1`：

- `GLIBC_BITS`：6 个头名、44 行、17 包，一律不计入需改档；包括实际命中的 `types.h`、`wordsize.h` 等。
- `LIBSTDCXX_BITS`：178 个头名、2,771 行、12 包，计入 `T1_HARD`。
- `UNDETERMINED_NOT_IN_EITHER_RPM`：15 个头名、119 行、23 包；没有双 RPM 同名项。它们不机械定性，只 fail-closed 并入上界。完整列表在同一表中。

2,934 个原命中均成功提取目标头名，解析失败为 0。逐命中重分类表 `progress/R1/bits_hit_reclassification.tsv` 是批量原始输出，不上传策展副本，SHA256 `accfeced3123eedd28daeb7544779aee26ee0a902abc8467d06edc53d79d8e37`，标记 `LOCAL_ONLY`。

## GLIBCXX 用法形态

逐形态统计见 `progress/R1/glibcxx_usage_shapes.tsv:1`，合并表见 `progress/R1/glibcxx_combined_summary.tsv:1`，每种形态最多五条三行原文样例见 `progress/R1/glibcxx_usage_samples.tsv:1`。

- `__GLIBCXX__` 共 644 行：`#ifdef/#ifndef/#if defined` 三类 361，其他预处理条件 7，其他用法 276。
- `_GLIBCXX_` 共 38,151 行：上述三类 5,605，其他预处理条件 2,169，其他用法 30,377。

因此不能再把两类 GLIBCXX 命中整体机械判为硬缺陷；条件保护只占一部分，其他用法仍须人工判读。

## 波 1 交叉验证

逐包对照见 `progress/R1/wave1_cross_validation.tsv:1`，结果为 5/5 `MATCH`：askuser-notification、cynara、libcynara-creds-dbus 不在 T1_HARD；libcynara-commons 与 security-manager 均因 `__forced_unwind` 进入 T1_HARD，与 T2 两个实证失败锚完全一致。强制锚失败数为 0，故不触发停报。

## Chromium 缺口

`chromium-efl-1.1.144-1.src.rpm` 仍因提取失败未扫描，见 `progress/R1/chromium_gap.tsv:1`。它是最大 C++ 包，三个发生率的分子均存在低估风险；现有语料不能判定它属于哪一档，因此影响不可量化，分母继续明确使用实际扫描的 371，而不是 372。

## 闭环

聚合命令原文和退出码见 `progress/R1/commands/07_reclassification.log:1`；全部行数、bits 解析和强制锚自检见 `progress/R1/aggregation_self_check.tsv:1`，均为 PASS。旧 `pattern_classification.tsv`、T1 报告与结论只在新登记中标为 `SUPERSEDED_BY_ADJUDICATION_20260801`，原文件字节保持不变。
