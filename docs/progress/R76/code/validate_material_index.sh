#!/usr/bin/env bash
set -euo pipefail

root="${1:?usage: validate_material_index.sh DOCS_R76_ROOT}"
index="${root}/INDEX.tsv"
expected=$(find "${root}" -type f ! -name INDEX.tsv | wc -l)
actual=$(awk 'END{print NR-1}' "${index}")
test "${actual}" -eq "${expected}"

tail -n +2 "${index}" | while IFS=$'\t' read -r source original upload digest; do
  test "${source}" = R76_GENERATED_OR_CAPTURED
  test -f "${upload}"
  observed=$(sha256sum "${upload}")
  observed=${observed%% *}
  test "${observed}" = "${digest}"
  test "${original}" = "progress/R76/${upload#${root}/}"
done

printf 'R76_INDEX_VALIDATION=PASS files=%s\n' "${actual}"
