#!/bin/bash
set -eu

python3 -m py_compile \
  docs/progress/BUILD_W4_0909/code/extract_log_evidence.py \
  docs/progress/BUILD_W4_0909/code/summarize_trace_probe.py

test "$(awk 'END { print NR - 1 }' docs/progress/BUILD_W4_0909/tables/package_failure_classification.tsv)" -eq 17
test "$(awk 'END { print NR - 1 }' docs/progress/BUILD_W4_0909/tables/trace_option_probe.tsv)" -eq 30
test "$(awk 'END { print NR - 1 }' docs/progress/BUILD_W4_0909/evidence/log_inventory.tsv)" -eq 32
test "$(grep -c '^## ' docs/progress/BUILD_W4_0909/evidence/log_excerpts.md)" -eq 17
test "$(grep -c 'NOT_OBSERVED' docs/progress/BUILD_W4_0909/tables/trace_option_probe.tsv)" -eq 5

counts=$(awk -F '\t' 'NR > 1 { c[$3]++ } END {
  print c["INJECTED_OPTION_TO_GCC"], c["SOURCE_COMPATIBILITY"],
        c["MIGRATION_DEPENDENCY_ABI"], c["ENVIRONMENT_OR_DEPENDENCY"],
        c["OTHER_UNRESOLVED"]
}' docs/progress/BUILD_W4_0909/tables/package_failure_classification.tsv)
test "$counts" = "4 6 5 1 1"

if find docs/progress/BUILD_W4_0909 -type f -size +2M -print | grep -q .; then
  echo "unexpected_large_file"
  exit 1
fi

sha256sum docs/progress/BUILD_W4_0909/code/*.sh \
  docs/progress/BUILD_W4_0909/code/*.py
# raw/ 保存命令与输出的逐字记录，其中的尾随空白属于原始证据，
# 不应由格式检查改写；其余待提交材料仍执行严格检查。
git diff --cached --check -- docs/progress/BUILD_W4_0909 \
  docs/progress/BUILD_STATUS_0909.md docs/progress/BUILD_QUESTIONS_0909.md \
  ':(exclude)docs/progress/BUILD_W4_0909/raw/**'
echo "SELF_CHECK=PASS"
