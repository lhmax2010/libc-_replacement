# R3：22.1.8 打包 spec 来源查证与迁移盘点（第一段）

状态：`DONE_FIRST_SEGMENT_WAITING_ADJUDICATION`。本段只读取平台 repodata、21.1.1 基线 worktree、既有记录与 22.1.8 SRPM 源码；没有构建，没有修改或还原任何源码/spec，也没有决定 dirty 版取舍或实际迁移范围。输入身份见 `progress/R3/input_identity.tsv:1`。

## 平台侧优先结论

结论为 `PLATFORM_DOES_NOT_PROVIDE`，见 `progress/R3/platform_conclusion.tsv:1`。检索范围是冻结的 Base/Unified 二进制和 source primary 四份元数据，共 12,776 个 package 记录；逐份路径、SHA256、记录数及 0 命中结果见 `progress/R3/platform_search_scope.tsv:1`。

检索对象为包名或 Provides 以 `libc++`、`libc++abi`、`libcxx`、`libcxxabi`、`llvm-libc++` 开头的记录。XML 解析结果表 `progress/R3/platform_repo_hits.tsv:1` 只有表头，四个 dataset 的命中数均为 0；精确原文检索逐项返回 `NOT_FOUND`，见 `progress/R3/commands/01b_platform_repodata_exact_name_search.log:1`，XML 解析与退出码见 `progress/R3/commands/02_platform_repodata_xml_parse.log:1`。因此没有可列出的平台 NEVRA、Provides、Requires 或子包构成，与我方 21.1.1 形态的差异摘要按条件为 `NOT_APPLICABLE_NO_PLATFORM_HIT`。

一次更宽的子串检索把 `GLIBCXX` 错当成 `libcxx` 命中；该尝试未用于结论，失败证据原样保留。最终规则使用 XML package 边界与名称前缀，避免该假阳性。失败探查及其替代证据登记在 `progress/R3/execution_incidents.tsv:1`。

## 21.1.1 基线 spec 身份

唯一 spec 位于 `/home/toolchain/plan_evaluation/p1/libcxx_packaging/source/libcxx-runtimes-21.1.1/packaging/libcxx-runtimes.spec`。Git HEAD 为 `67e4b73a5f6a40545555ba6b1318807967f86678`，工作树仅有该 spec 一项修改。完整身份表见 `progress/R3/baseline_spec_identity.tsv:1`：

- HEAD 版 SHA256：`6cd1c5218d8b5b97e36ca841ccbcfb76dbf3d6f93b855e5fa1e19c4cd45d78ee`；冻结全文为 `progress/R3/evidence/libcxx-runtimes.spec.HEAD:1`。
- 工作树版 SHA256：`25db13b28204be28d21bab4a54cfbfc96b2abfc9e3f114f1041175a967f13f4f`；冻结全文为 `progress/R3/evidence/libcxx-runtimes.spec.WORKTREE:1`。
- HEAD 到工作树的完整 full-index diff SHA256：`18a25afa18af2e1840893eb6293e6623bd0d4dc037798965f5bb3b05d7d0363f`，原文见 `progress/R3/evidence/baseline_spec_head_vs_worktree.diff:1`；生成命令与退出码见 `progress/R3/commands/04_baseline_spec_identity_and_full_diff.log:1`。

dirty 修改事实是：Release 1→2；增加 `ExclusiveArch`；增加 `LIBCXX_INSTALL_LIBRARY_DIR` 与 `LIBCXXABI_INSTALL_LIBRARY_DIR`；增加 21.1.1-2 changelog。当前短 index diff 与既有 `spec_actual_v3.diff` 逐字 `cmp=0`。对应留痕不是 `NO_ADJUDICATION_FOUND`：

- 基线、预先冻结提案及最终处置见 `progress/R3/evidence/record_spec_review.md:3`、`progress/R3/evidence/record_spec_review.md:23`、`progress/R3/evidence/record_spec_review.md:30`；
- 64 位安装目录触发证据与 v3 提案边界见 `progress/R3/evidence/record_spec_review_followup_v3.md:3`；
- 最终 spec SHA、逐项 diff、三架构产物门结果见 `progress/R3/evidence/record_multiarch_packaging_report.md:9`、`progress/R3/evidence/record_multiarch_packaging_report.md:29`、`progress/R3/evidence/record_multiarch_packaging_report.md:77`。

逐项性质和原始记录位置汇总于 `progress/R3/dirty_trace.tsv:1`。广域记录搜索覆盖完整 plan_evaluation、plan_evaluation_probe 与 `/home/toolchain/development/libc++_replacement_bak`，见 `progress/R3/commands/05_dirty_adjudication_search.log:1`；其中备份报告与 plan_evaluation 权威报告字节相同，见 `progress/R3/commands/06b_backup_record_identity.log:1`。另以裁决/报告文件类型和明确备份子根复核，范围与各根命中/NOT_FOUND 见 `progress/R3/commands/05b_dirty_adjudication_scoped_search.log:1`。本报告只确认修改性质与留痕，不判断其良恶或迁移时应选 HEAD/dirty 哪一版。

## spec 迁移改动面

工作树 spec 中 19 个版本相关位置的 file:line、完整原文和分类见 `progress/R3/spec_version_surface.tsv:1`：8 项标为“可机械替换”，11 项标为“需人工确认”。可机械项仅包括数值版本宏/RPM Version 及由 `%{version}`、`%{release}` 展开的依赖；下列内容不得当成源码版本机械替换：

- `libcxx-llvm21.map`、`LLVM_21` 与 version-script 是 ABI 符号版本决策；
- `c++/v1`、DSO `.so.1*` 是安装布局或 SONAME，不是 LLVM 21 的字面版本；
- 既有 changelog 是历史记录；
- `Source0` 虽使用版本宏，但现 spec 预期 `libcxx-runtimes-%{version}.tar.gz` 和同名 `%setup` 顶层，平台输入实际为 `llvm-22.1.8.tar.gz` / `llvm-22.1.8`。

源码布局、构建参数、runtimes 组合、安装目录、build target、四个子包及 `%files` 的 13 个强耦合面全部标为“需人工确认”，见 `progress/R3/spec_layout_coupling.tsv:1`。本段没有把“同名仍存在”等同于“语义已验证”，也没有产生迁移 spec。

## 22.1.8 三锚

裁决指定的三锚和数值版本全部精确通过，见 `progress/R3/three_anchor.tsv:1`：

- tarball SHA256 为 `16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e`；
- 平台 spec VCS 为 `platform/upstream/llvm#c12cbe887ff08f04628e1b1165f8d02ce69a9146`；
- `llvmorg-22.1.8^{}` 为 `ca7933e47d3a3451d81e72ac174dcb5aa28b59d1`，远端原文见 `progress/R3/evidence/upstream_llvmorg_22_1_8_ls_remote.raw.log:1`；
- `LLVMVersion.cmake` 数值为 22.1.8、suffix 空。

完整命令、原文和硬断言在 `progress/R3/commands/08_three_anchor_and_numeric_version.log:1`。源码 tarball 没有 `.git`，按本次裁决记为 `ACCEPTED_NOT_A_RED_STOP`；旧 T3 对树内 Git 元数据的要求及输入缺口结论只在 `progress/R3/supersession_registry.tsv:1` 指定范围内被取代，原 T3 文件未改。

## 21.1.1 与 22.1.8 源码静态差异

目录差异见 `progress/R3/source_layout_diff.tsv:1` 和排除 test/docs/benchmarks/fuzz/www 后的全深度打包相关视图 `progress/R3/source_layout_packaging_relevant_diff.tsv:1`：

- 两版 `runtimes` 与 `libcxxabi` 在上述打包相关目录集合内无增删；
- 22.1.8 的 `libcxx` 增加 `utils/ci/docker`、`utils/ci/images`，测试树另增加 `test/libcxx-03`；
- 21.1.1 基线没有 `libunwind` 组件目录，22.1.8 有完整的 `cmake/include/src` 等目录。现 spec 的 runtimes 列表仍只写 `libcxxabi;libcxx`，并显式关闭 LLVM unwinder；是否维持这一组合留待人工裁决。

CMake 选项静态对照见 `progress/R3/cmake_option_comparison.tsv:1`：spec 传入的 40 个变量中，26 个 LLVM runtime 项目选项在两版均可定位且同名，14 个是标准 CMake 变量；没有发现该 spec 使用的项目选项被删除或改名。相关定义集合的版本差异仅为 22.1.8 新增四项 availability/assertion 选项，见 `progress/R3/cmake_option_name_delta.tsv:1`。`cxxabi`、`cxx`、`cxx_experimental` target 在两版源码中均可静态定位；原文见 `progress/R3/commands/09_key_source_coupling_evidence.log:1`。这些结论均为只读静态存在性检查，不替代配置或构建验证。

## 停止边界

第一段查证完成：平台未提供同名 libc++ 包；三锚闭合；21.1.1 HEAD/dirty 两版及 dirty 留痕已冻结；迁移改动面与源码差异已列全。本轮到此停止，等待人工裁决 dirty 修改取舍与第二段迁移范围；没有 checkout、stash、还原、spec 改写、补丁或构建。
