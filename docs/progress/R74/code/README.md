# R74 代码与命令材料

- `extracted/`：从报告逐字节提取的全部 11 个 C++ 代码块；`INDEX.tsv` 给出报告行号和 SHA256。
- `supplemented/`：问题二、问题三为完成编译所作的补全副本；原提取件未改动。
- `tools/`：提取、原样编译、x86_64 构建运行、armv7l 构建运行及外链采集脚本。
- `commands/`：每次检查、编译、链接、运行、板端部署与清理的命令原文及退出码。

实际运行时身份位于上级 `raw/`：x86_64 的 `042_libcxx_ldd_with_path.txt`、`043_libcxx_ld_debug.*`、`047_x86_baseline_runtime_sha256.txt`；armv7l 的 `arm_runs/011_remote_sha256.log`、`012_ldd_libstdcxx.log`、`013_ldd_libcxx.log`。
