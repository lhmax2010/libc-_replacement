# 辅助显示命令补录

以下为工具侧已返回的命令与退出码；仅显示已生成的日志/脚本，不承担分类判据。此表事后补录，主流程原始输出均在 raw/，不伪称这些显示命令有独立原始输出文件。

| 命令原文 | 退出码 |
| --- | ---: |
| `cat docs/progress/R119_PROJECTION/raw/001_gate.stdout docs/progress/R119_PROJECTION/raw/002_context.stdout` | 0 |
| `cat docs/progress/R119_PROJECTION/raw/003_inventory.stdout` | 0 |
| `tail -5 docs/progress/R119_PROJECTION/raw/004_extract15.stdout` | 0 |
| `head -65 docs/progress/R119_PROJECTION/raw/005_expression_queue15.stdout` | 0 |
| `tail -65 docs/progress/R119_PROJECTION/raw/005_expression_queue15.stdout` | 0 |
| `tail -6 docs/progress/R119_PROJECTION/raw/006_measure15.stdout` | 0 |
| `sed -n '1,185p' docs/progress/R119_PROJECTION/raw/008_definition_rules.stdout` | 0 |
| `cat docs/progress/R119_PROJECTION/CHECKPOINT_15.json` | 1（当时尚未生成，不作为阴性证据） |
| `tail -4 docs/progress/R119_PROJECTION/raw/009_classify15.stdout` | 0（执行两次） |
| `tail -4 docs/progress/R119_PROJECTION/raw/010_refresh15_raw_args.stdout` | 0 |
| `tail -3 docs/progress/R119_PROJECTION/raw/010_refresh15_raw_args.stdout` | 0 |
| `cat docs/progress/R119_PROJECTION/raw/014_checkpoint15.stdout` | 0 |
| `cat docs/progress/R119_PROJECTION/CHECKPOINT_65.json docs/progress/R119_PROJECTION/EXTRACTION_STATUS.json` | 0 |
| `tail -45 docs/progress/R119_PROJECTION/raw/042_checkpoint65.stdout` | 0 |
| `cat docs/progress/R119_PROJECTION/raw/043_boundary_schema.stdout` | 0 |
| `head -8 docs/progress/R119_PROJECTION/raw/044_boundary_scan.stdout` | 0 |
| `sed -n '1,160p' docs/progress/R119_PROJECTION/code/boundary_scan.py` | 0 |
| `sed -n '1,160p' docs/progress/R119_PROJECTION/code/common.py` | 0 |
| `cat docs/progress/R119_PROJECTION/raw/045_boundary_digest.stdout` | 0 |
| `sed -n '1,200p' docs/progress/R119_PROJECTION/code/classify.py` | 0 |
| `head -65 docs/progress/R119_PROJECTION/raw/046_inspect_object_proof.stdout` | 0 |
| `cat docs/progress/R119_PROJECTION/raw/047_final_inputs.stdout` | 0 |
| `cat docs/progress/R119_PROJECTION/raw/048_layout_inspect.stdout` | 0 |
| `tail -100 docs/progress/R119_PROJECTION/raw/049_object_sources.stdout` | 0 |
| `tail -8 docs/progress/R119_PROJECTION/raw/050_final_audit.stderr` | 0 |
| `head -65 docs/progress/R119_PROJECTION/raw/050_final_audit.stdout` | 0 |
| `sed -n '1,140p' docs/progress/R119_PROJECTION/code/checkpoint.py` | 0 |

文件编辑使用 apply_patch，其内容及返回由会话工具记录；可执行 Python/探针快照另有 SHA256。
