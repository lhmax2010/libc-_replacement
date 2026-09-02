# R97 关键证据索引

- `raw/001_preflight_and_resource.*`：既有材料复核与 light 资源门禁，退出码 0。
- `raw/002_inventory_project_configs.*`：`codes/qb/` 全部 `project_config` 盘点。
- `raw/003_identify_r96_config_mapping.*`：R96 的 GCC/LLVM GBS 仓映射。
- `raw/004_locate_arch_flags_in_four_configs.*`：四个实际配置文件中的匹配行。
- `raw/005_inspect_config_structure_and_effective_sources.*`：带行号的配置原文、
  Optflags 全行与复合 `-mtune` 的全仓当前配置命中。
- `raw/006_inventory_compiler_test_environments.*`：过宽只读盘点的部分输出；
  因文件系统遍历持续阻塞而终止，恢复说明见同名前缀
  `.recovery_note.txt`。包装器随进程被终止，因此没有自动 `.exitcode`。
- `raw/007_inspect_known_roots_and_prior_probe.*`：限定到已知构建根后的工具盘点。
- `raw/008_qemu_target_clang_smoke.*`、`009_*`、`010_*`：目标 Clang 执行方式
  核验；aarch64 最终使用可执行的历史 LLVM 构建根进行独立探针。
- `raw/011_probe_aarch64_gcc_flags.*`：GCC/aarch64 逐参数与整组探针。
- `raw/012_probe_aarch64_llvm_flags.*`：LLVM/aarch64 逐参数与整组探针。
- `raw/013_probe_armv7l_gcc_flags.*`、`014_probe_armv7l_llvm_flags.*`：armv7l。
- `raw/015_probe_x86_64_gcc_flags.*`、`016_probe_x86_64_llvm_flags.*`：x86_64。
- `raw/017_summarize_probe_results.*`：所有探针结果汇总及所有拒绝项。
- `raw/018_correlate_config_with_actual_build_logs.*`：配置与实际历史构建参数
  对照，包括 R96 aarch64/GCC 失败、历史 aarch64/LLVM 和 R96 armv7l 两格。
- `raw/019_source_integrity_and_scope_audit.*`：四配置 SHA256、Git 无差异、
  Toolchain 配置精确检索和 R96 候选 spec 未变核验。

每组命令的原文、完整 stdout、stderr、退出码分别存于 `.command.txt`、
`.stdout`、`.stderr`、`.exitcode`。
