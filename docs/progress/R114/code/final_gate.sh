#!/bin/bash
set -euo pipefail
root=/home/toolchain/development/libc++_replacement
cd "$root"

python3 docs/progress/R114/code/validate_translation.py
python3 docs/progress/R114/code/validate_bundle.py
diff -qr /tmp/r114_output_20260904/hq_20260904 \
  docs/progress/R114/deliverable/hq_20260904

cmp -s docs/progress/R114/source/base_packages_final_ZH_original.tsv \
  /tmp/r114_input_20260904/hq_20260904/base_packages_final.tsv
cmp -s docs/progress/R114/source/base_packages_final_ZH_original.md \
  /tmp/r114_input_20260904/hq_20260904/base_packages_final.md

actual_archive_sha=$(sha256sum docs/progress/R114/hq_deliverables_20260904_corrected.tar.gz | awk '{print $1}')
recorded_archive_sha=$(awk -F '\t' '$2 == "hq_deliverables_20260904_corrected.tar.gz" {print $1}' docs/progress/R114/OUTPUT_SHA256.tsv)
test "$actual_archive_sha" = "$recorded_archive_sha"

verify=$(mktemp -d /tmp/r114-final-gate.XXXXXX)
trap 'rm -r -- "$verify"' EXIT
tar xzf docs/progress/R114/hq_deliverables_20260904_corrected.tar.gz -C "$verify"
diff -qr docs/progress/R114/deliverable/hq_20260904 "$verify/hq_20260904"

test "$(find "$verify/hq_20260904" -maxdepth 1 -type f | wc -l)" -eq 8
test "$(wc -l < "$verify/hq_20260904/base_packages_final.tsv")" -eq 255
test "$(tail -n +7 "$verify/hq_20260904/base_packages_final.md" | wc -l)" -eq 254

git diff --check -- docs/progress/R114

printf 'source_chinese_unchanged=PASS\n'
printf 'archive_sha256=%s\n' "$actual_archive_sha"
printf 'archive_roundtrip=PASS\n'
printf 'archive_file_count=8\n'
printf 'final_gate=PASS\n'
