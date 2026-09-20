# 辅助只读命令补录

以下仅用于定位与查看已落盘结果，未启动新分析/编译。命令与退出码据本次工具返回补录；没有为它们伪造独立时间戳。主要分析、探针、资源闸门与 Git 命令的原文、输出、退出码和时间均在 `raw/`。

```text
command: pwd
exit: 0
command: rg --files -g AGENTS.md -g '*record*' -g '*limited*' -g '*review_notes*' docs/progress/R119_batch1 docs/progress/R119_batch2 docs/progress/API_0911
exit: 0
command: ls -la AGENTS.md docs/progress/R119_batch1/code docs/progress/R119_batch2/code
exit: 2
command: sed -n '1,220p' docs/progress/API_0911/record.py
exit: 0
command: sed -n '1,220p' docs/progress/R119_batch1/code/resume_limited.py
exit: 0
command: ls -l /home/AGENTS.md /home/toolchain/AGENTS.md /home/toolchain/development/AGENTS.md docs/AGENTS.md docs/progress/AGENTS.md
exit: 2
command: cat docs/progress/R119_RECLASSIFY/raw/001_gate.stdout docs/progress/R119_RECLASSIFY/raw/002_inputs.stdout
exit: 0
command: cat docs/progress/R119_RECLASSIFY/raw/003_unknowns.stdout
exit: 0
command: cat docs/progress/R119_RECLASSIFY/raw/004_probe_inputs.stdout
exit: 0
command: cat docs/progress/R119_RECLASSIFY/raw/005_compilers.stdout
exit: 0
command: cat docs/progress/R119_RECLASSIFY/raw/006_causes.stdout
exit: 0
command: tail -5 docs/progress/R119_RECLASSIFY/raw/010_structural.stdout
exit: 0
command: rg '^((12|38|48|49|52|58|121|175|192)\t|VERSION)' docs/progress/R119_RECLASSIFY/raw/a_gnu_run1.stdout docs/progress/R119_RECLASSIFY/raw/a_cxx_run1.stdout
exit: 0
command: cat docs/progress/R119_RECLASSIFY/raw/011_b_contexts.stdout
exit: 0
command: tail -8 docs/progress/R119_RECLASSIFY/raw/012_validate_a.stderr
exit: 0
command: cat docs/progress/R119_RECLASSIFY/raw/013_b_secondary.stdout
exit: 0
command: tail -6 docs/progress/R119_RECLASSIFY/raw/014_finalize_b.stderr
exit: 0
command: cat docs/progress/R119_RECLASSIFY/raw/016_merge_ab.stdout
exit: 0
command: cat docs/progress/R119_RECLASSIFY/raw/024_edge_schema.stdout
exit: 0
command: cat docs/progress/R119_RECLASSIFY/raw/025_edge_counts.stdout docs/progress/R119_RECLASSIFY/A_EXECUTION_TIME.json docs/progress/R119_RECLASSIFY/A_VALIDATION_TIME.json docs/progress/R119_RECLASSIFY/B_EXECUTION_TIME.json
exit: 0
command: sed -n '1,100p' docs/progress/R119_RECLASSIFY/CROSS_SURFACE.md
exit: 0
command: cat docs/progress/R119_RECLASSIFY/raw/026_timing_inputs.stdout
exit: 0
command: cat docs/progress/R119_RECLASSIFY/raw/037_delivery_check.stdout
exit: 0
command: cat docs/progress/R119_RECLASSIFY/raw/038_tracked_cache.stdout
exit: 0
```

两次 `ls` 退出 2 因所询问 AGENTS.md 不存在；不是候选源码/符号零命中，未用于定界。一次 STATUS.md 补丁应用因上下文不匹配未生效，随即以精确上下文重试成功；没有改动原始两张表。辅助命令先在会话中执行、后补录，是本次记录流程的偏差，已显式披露。
