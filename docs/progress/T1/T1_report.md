# T1 平台级 libstdc++ 专有用法语料扫描报告

状态：`DONE`（下载、解包或扫描失败按规格逐包登记，不阻断其余包）。

## 语料身份与覆盖率

输入仓为工作根 `gbs_llvm.conf` 实际引用的 Base/Unified Toolchain `reference` standard 仓。二进制 repomd revision 分别为 `1784726077`、`1785222917`，源码 repomd revision 分别为 `1784726069`、`1785222882`；URL、repomd SHA256、primary href/SHA256/时间戳均已固化（`progress/T1/repo_identity.tsv:1`；`progress/T1/source_repo_identity.tsv:1`）。解析全部架构的 primary 元数据后，得到 2,132 个依赖 libstdc++ 的二进制 NEVRA/arch 行，映射并去重为 372 个 SOURCERPM，其中 Base 59 个、Unified 313 个（`progress/T1/final_counts.tsv:1`；`progress/T1/source_packages.tsv:1`）。

372 个 SRPM 共 `18,191,086,818` 字节，均按元数据期望尺寸和 SHA256 验证；Chromium 初次非续传下载被安全终止后从同一 partial 续传并重新做完整尺寸/SHA 校验，其最终状态为 `DOWNLOADED_VERIFIED_AFTER_RESUME`（`progress/T1/download_results.tsv:1`；`progress/T1/06a_chromium_nonresuming_curl_terminated.log:1`；`progress/T1/06b_chromium_resume_and_verify.log:1`）。

解包结果为 371 个 `EXTRACTED_OK`、1 个 `EXTRACTION_FAILED_OR_INCOMPLETE`。失败项是 `chromium-efl-1.1.144-1.src.rpm`：`rpm2cpio` 明示 cpio 不支持超过 4 GB 文件，退出码为 `1`，下游 cpio 退出 `2`；该包不进入扫描分母（`progress/T1/extraction_summary.tsv:1`；`progress/T1/extraction_failure_logs/chromium-efl-1.1.144-1.src.rpm.log:1`；`progress/T1/coverage_failures.tsv:1`）。

覆盖率口径如下（`progress/T1/coverage_and_occurrence.tsv:1`）：

- 应扫源码包：372；
- 实际扫描包：371；
- 失败包：1；
- 覆盖率：`371 / 372 = 99.731183%`。

`EXTRACTED_OK` 集、`SCANNED` 集与逐包汇总集两次 `cmp` 均退出 0；最终所有断言为 PASS（`progress/T1/10_final_selfcheck.log:1`）。前两次自检分别因汇总脚本误读下载状态列、未接受 `DOWNLOADED_VERIFIED_AFTER_RESUME` 而 fail-closed，失败原文保留，未覆盖为绿（`progress/T1/10a_final_selfcheck_failed.log:1`；`progress/T1/10b_final_selfcheck_failed.log:1`）。

## 扫描结果

每个成功解包包均以同一模式集只读扫描；逐命中保留源码包、模式、`file:line`、命中行前一行/本行/后一行。完整逐命中表位于 `progress/T1/corpus_hits.tsv`（116,533 行命中数据，因体积约 56 MiB 不在本报告作逐行展开）；逐包结果见 `progress/T1/per_package_summary.tsv:1`，逐模式总次数见 `progress/T1/pattern_occurrences.tsv:1`。

模式命中总次数：

| 模式 | 次数 |
|---|---:|
| `__gnu_cxx` | 47,500 |
| `_GLIBCXX_` | 38,151 |
| `std::__cxx11` | 8,293 |
| 构建系统文件内 `libstdc++` | 7,384 |
| `__cxxabiv1` | 6,848 |
| `#include <bits/` | 2,934 |
| `#include <ext/` | 2,166 |
| `-lstdc++` | 927 |
| `__GLIBCXX__` | 644 |
| `abi::` | 551 |
| `pthread_cancel` | 315 |
| `__forced_unwind` | 309 |
| `<cxxabi.h>` | 275 |
| `pthread_setcancelstate` | 155 |
| `pthread_testcancel` | 81 |

“确定需改”采用保守、可复核的机械分类：GNU 专有命名空间/扩展头/实现宏、`std::__cxx11`、显式 `-lstdc++` 及构建系统文件内显式 `libstdc++` 计入；`__forced_unwind`、Itanium ABI 通用名称/头和 pthread 取消 API 仅计风险命中，不据此计入。逐模式分类依据见 `progress/T1/pattern_classification.tsv:1`。

命中至少一个“确定需改”模式的包为 123 个；发生率严格以实际扫描包为分母：`123 / 371 = 33.153639%`（`progress/T1/coverage_and_occurrence.tsv:1`）。其余 248 个实际扫描包未命中该机械分类。此处只陈述静态命中与分类口径，不作修复建议或良恶判断。

任务开始磁盘可用 `1,186,365,976,576` 字节，结束时可用 `1,116,251,492,352` 字节，均高于 80 GiB 守卫（`progress/T1/00_disk_guard.log:1`；`progress/T1/11_disk_finish.log:1`）。SRPM 与展开语料仅位于 `tmp/corpus/`，未进入 `codes/`。
