#!/usr/bin/env bash
set -euo pipefail

root=${1:?R39 root required}
index="$root/CODE_INDEX.tsv"

printf 'uploaded_path\tsource_task\tsource_path\tsha256\n' >"$index"
while IFS= read -r -d '' uploaded; do
  rel=${uploaded#"$root/"}
  case "$rel" in
    code/r36/tests/*)
      source_task=R36
      source_path="progress/R36/src/${rel#code/r36/tests/}"
      ;;
    code/r36/evidence/*)
      source_task=R36
      source_path="progress/R36/commands/${rel#code/r36/evidence/}"
      ;;
    code/r36/build/*)
      source_task=R36
      source_path='tmp/R36/build-armv7l/CMakeCache.txt'
      ;;
    code/r38/tests/*)
      source_task=R38
      source_path="progress/R38/src/${rel#code/r38/tests/}"
      ;;
    code/r38/evidence/*)
      source_task=R38
      source_path="progress/R38/commands/${rel#code/r38/evidence/}"
      ;;
    code/r38/build/*)
      source_task=R38
      source_path='tmp/R38/unpatched-build-armv7l/CMakeCache.txt'
      ;;
    code/libcxxabi/*)
      source_task=R36
      source_path="docs/progress/R36/code/${rel#code/}"
      ;;
    code/r39/*)
      source_task=R39
      source_path="progress/R39/tools/${rel#code/r39/}"
      ;;
    *)
      source_task=UNKNOWN
      source_path=NOT_AVAILABLE
      ;;
  esac
  sha=$(sha256sum "$uploaded" | awk '{print $1}')
  printf '%s\t%s\t%s\t%s\n' "$rel" "$source_task" "$source_path" "$sha" >>"$index"
done < <(find "$root/code" -type f -print0 | sort -z)
