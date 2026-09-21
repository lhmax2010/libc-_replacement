# 本轮写包与载荷核验

更新时间：2026-09-21T22:51:38.579743+08:00

## 范围与事实边界

只使用已有候选 spec，不改上游 C++、不新增 spec 改动、不推包仓、不覆盖 Source1002、不上板。编译线项目分支为 codex/r5-ehabi-diagnosis。

对账材料原样推送的远端提交为 `c7aa2c5e72f78516e40b7a5423a4b83673608bad`。

六格是两架构 × 三路径；现有 spec 每格写 main/common/static 三份 RPM。`RPM_INVENTORY.tsv` 列每份 RPM 的完整 SHA256、大小、压缩格式，`ELF_COMPARISON.tsv` 列 ELF 依赖与字符串痕迹。

截至本次汇总，已核验本轮 RPM 9 份；未执行项明确留在 STATUS.md 中。

RPM 摘要通过、整轮写包通过、静态候选验收通过是三件事。aarch64 libc++ 产物额外依赖 libbpf.so.1 的缺口不会被写包退出 0 覆盖。

所有源码/工具脚本执行记录均在 cells、raw 与命令 JSON 中；脚本快照和 SHA256 随记录保存。

## aarch64 / libcxx

实际写出 3 个 RPM，摘要校验已执行。综合状态：`RPM_COMPLETE_GATES_NOT_ALL_CLOSED`。

证据：`verify-bpf-aarch64-libcxx/result.json`、`verify-bpf-aarch64-libcxx/commands.json`、`verify-bpf-aarch64-libcxx/all-main-elfs.json`、`audit-bpf-aarch64-libcxx/result.json`、`cells/bpf-aarch64-libcxx/build.log`。

| 核验项 | 结果 |
|---|---|
| three_packages | PASS |
| payload_no_T | PASS |
| original_matches_this_cell_source1002 | PASS |
| elf_arch | PASS |
| libgcc_dynamic | PASS |
| no_dynamic_cxx_llvm | PASS |
| main_libcxx | PASS |
| static_goal_no_extra_dynamic_deps | 未闭合 |
| code_sections_match | PASS |
| unstripped_libcxx_markers | PASS |
| rpm_literal_std1_marker | PASS |
| rpm_cxx11_absent | PASS |

候选 static ELF 136127672 字节；原资产 129681256 字节。
static 的 DT_NEEDED：`libbpf.so.1, libm.so.6, libgcc_s.so.1, libc.so.6, ld-linux-aarch64.so.1`。
Source1002 中本架构普通文件 1 个；static RPM 解包后普通文件 2 个，完整列表在 result.json，计数不混用 RPM 文件记录（其还可能含目录）。

## aarch64 / gcc

实际写出 3 个 RPM，摘要校验已执行。综合状态：`PASS`。

证据：`verify-bpf-aarch64-gcc/result.json`、`verify-bpf-aarch64-gcc/commands.json`、`verify-bpf-aarch64-gcc/all-main-elfs.json`、`audit-bpf-aarch64-gcc/result.json`、`cells/bpf-aarch64-gcc/build.log`。

| 核验项 | 结果 |
|---|---|
| three_packages | PASS |
| payload_no_T | PASS |
| original_matches_this_cell_source1002 | PASS |
| elf_arch | PASS |
| source1002_byte_identical | PASS |
| main_gcc_runtime | PASS |

候选 static ELF 129681256 字节；原资产 129681256 字节。
static 的 DT_NEEDED：`libm.so.6, libc.so.6, ld-linux-aarch64.so.1`。
Source1002 中本架构普通文件 1 个；static RPM 解包后普通文件 2 个，完整列表在 result.json，计数不混用 RPM 文件记录（其还可能含目录）。

## aarch64 / undefined

实际写出 3 个 RPM，摘要校验已执行。综合状态：`PASS`。

证据：`verify-bpf-aarch64-undefined/result.json`、`verify-bpf-aarch64-undefined/commands.json`、`verify-bpf-aarch64-undefined/all-main-elfs.json`、`audit-bpf-aarch64-undefined/result.json`、`cells/bpf-aarch64-undefined/build.log`。

| 核验项 | 结果 |
|---|---|
| three_packages | PASS |
| payload_no_T | PASS |
| original_matches_this_cell_source1002 | PASS |
| elf_arch | PASS |
| source1002_byte_identical | PASS |
| main_gcc_runtime | PASS |

候选 static ELF 129681256 字节；原资产 129681256 字节。
static 的 DT_NEEDED：`libm.so.6, libc.so.6, ld-linux-aarch64.so.1`。
Source1002 中本架构普通文件 1 个；static RPM 解包后普通文件 2 个，完整列表在 result.json，计数不混用 RPM 文件记录（其还可能含目录）。

## 判断与未闭合项

- ARM 新旧同 NEVRA 的输入替换需要人工确认；当前进展以 INPUT_STATUS.md 与事务原始退出码为准，不能由拟安装清单推定已安装。
- 既有 libbpf 动态依赖问题未授权修改，本轮只记录实际产物。
- 精确字符串前缀与扩展检索分别报告，零命中不等于原资产不含某套运行时；尤其原 aarch64 资产的扩展检索已有命中。
- 功能等价、取消探针、物理板安装均未在本轮执行。
- 本轮尚不能据写包结果授权替换发布资产或启动 QuickBuild；人工已明确后者等 static 子包问题解决。
