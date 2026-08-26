# R72 材料说明

## 代码

- `code/enumerate_noexcept_cancel.py`：Universal Ctags + 词法调用图候选生成器。输出必须经人工消歧，不能直接作为结论。
- `code/scan_facility_usage.py`：对 R32 的 371 个成功扫描 source RPM 做设施词法命中；最多并发 2 个 `rg`，逐包超时 300 秒。
- `code/noexcept_cancel_samples.cpp`：`wait`、`wait_for`、自定义 clock 三种样本；工作线程非 `noexcept`，使用裸 `pthread_create`，应用代码无 catch。

## 结论表

- `tables/verified_boundaries.tsv`：人工复核后的边界、两库对照、可达性和样本状态。
- `tables/facility_paths.tsv`：实际差异向标准库设施的传播路径。
- `tables/platform_extension_cancel_points.tsv`：glibc 2.40 源码中、POSIX-required 名字集之外的 Linux 取消 wrapper 名字。
- `tables/portable_only_candidates.tsv`：POSIX-may 或其他配置下可能成立、但当前 glibc/Linux 口径未证实的候选。
- `tables/sample_results.tsv`：三组动态样本结果。

## 原始材料

- `raw/cancellation_points.tsv`：POSIX required/may 及初始 glibc 扩展种子。
- `raw/direct_cancel_calls.tsv`、`raw/noexcept_candidates*.tsv`：自动候选原始输出；其中含同名函数导致的假阳性。
- `raw/glibc240_*`、`raw/platform_libc_disassembly.txt`：同版本 glibc 源码与目标 sysroot libc 的核查材料。
- `raw/facility_usage_hits.tsv`、`raw/facility_usage_summary.tsv`：消费面逐文件命中与汇总。
- `raw/tests/`：六格未经整理的完整 stdout/stderr/退出码。
- `commands/`：命令、输出和退出码原文。失败/终止的探索命令也保留，没有从材料中删除。
- `research/upstream_sources.tsv`：POSIX/glibc/GCC/libc++ 政策和 issue 检索来源。

## 身份与未观测项

- libc++/libstdc++ 源码 commit、关键文件 SHA256、编译器与 sysroot 配置见 `commands/021_toolchain_config_identity.log`。
- 运行时加载身份见 `commands/015_runtime_identity.log`。
- D02（system-clock timed wait）和 D05（`wbuffer_convert` 析构）本轮只取得源码可达证据，运行样本为 `NOT_OBSERVED`；动态抽样已按任务要求选满 3 项。
- armv7l 动态运行未做，记 `NOT_OBSERVED`；任务明确允许抽样仅做 x86_64 原生执行。
- 平台发行补丁后的 glibc 2.40 源码未取得；使用 GNU 官方同版本源码作旁证，并以目标 sysroot `libc.so.6` 做二进制身份核查，不冒充平台源码原件。
