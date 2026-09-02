# R95 关键证据索引

## 身份与资源

- `raw/001_startup_and_resource_gate.*`：工作区、资源门禁原始结果，`gate_result=PASS`。
- `inputs/actual_repositories.tsv`：本轮使用的实际 Tizen-Base 与 Tizen-Unified 冻结快照 URL。
- `raw/010_verify_r11_repository_identity.*`、`raw/011_confirm_r11_base_identity.*`：确认历史 R11/R22 语料不是本任务所指的实际 `Tizen-Base`，因此重新取数。

## Base 全量盘点

- `raw/060`–`raw/087`：1,710 个 Base RPM 的下载、SHA256 校验、分批解包、ELF 扫描和最终汇总。
- `raw/093_validate_complete_base_cpp_tables.*`：`FULL_EXTRACTION_RECORDS=1710`、`FULL_EXTRACTION_NONPASS=0`、源码名闭合检查为 0 缺口。
- `tables/base_cpp_source_records_exact.tsv`：完整 C++ 源码记录清单。
- `tables/base_cpp_binary_records_exact.tsv`：完整 C++ 二进制 RPM 清单。
- `tables/base_cpp_runtime_elfs_exact.tsv`：2,292 个 C++ runtime ELF 清单。

## 设备镜像

- `inputs/image_packages/`：14 份标准镜像 `.packages` 原文。
- `raw/030_map_base_cpp_packages_to_images.*`、`raw/087_summarize_complete_base_elf_scan.*`：镜像映射与精确重算。
- `tables/base_cpp_image_membership_exact.tsv`：每个镜像、包、架构、版本、源码包、RPM SHA256 与 ELF 路径。
- `raw/094_check_benchmark_and_function_test_tools_in_images.*`：测试/基准工具名称核查。

## Unified → Base 边界

- `raw/048`–`raw/059`：相关 RPM 下载、SHA256 校验、1,934 个 ELF 扫描、SONAME 解析、动态符号交集和聚合。
- `raw/091_verify_cross_edge_input_completeness.*`：用完整 Base 扫描复核 provider 输入，核心集合逐字节一致，`CMP_EXIT_CODE=0`。
- `tables/actual_cross_elf_edge_evidence.tsv`：逐 ELF 的 UND/DEF 交集、mangled 与 demangled 符号。
- `tables/actual_cross_source_edge_classification.tsv`：逐 source-RPM 边分类。
- `tables/actual_cross_logical_edge_classification.tsv`：逐去重源码名边分类。
- `tables/actual_cross_edge_unresolved.tsv`：表头之外无未解析记录。

## 历史 483 口径

- `raw/043`–`raw/045`、`raw/072`、`raw/089`：历史集合到实际两快照的映射和最终闭合。
- `tables/migration_483_actual_project_mapping.tsv`：483 条历史记录逐条映射。

## 技术性非零与恢复

- `raw/002`：过宽只读检索主动终止，退出 143；恢复说明同名前缀的 `recovery_note.txt`。
- `raw/022`：两个猜测的镜像路径返回 404，退出 22；随后改用已确认的 `images/standard/<image>/.packages`。
- `raw/037`、`raw/039`：Python CSV 字段上限导致退出 1；增加解析器字段上限后分别在 `raw/038`、`raw/040` 成功重跑，未改变判定逻辑。
- `raw/075`、`raw/097`：日志包装器参数顺序错误，退出 127；分别在 `raw/076`、`raw/098` 按正确接口重跑。
- `raw/050`：校验命令中的 awk 写法有误，但外层命令退出码被后续命令遮蔽；`raw/051` 用独立、可检查退出码的校验重跑通过。

这些均为命令或日志包装的技术性问题，不是调查判据不成立；失败原文保留，后续成功重跑可逐一对应。
