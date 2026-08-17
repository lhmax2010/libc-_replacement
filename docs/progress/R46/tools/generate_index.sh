#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
root="$workspace/progress/R46"
output="$root/INDEX.tsv"
manifest="$root/MANIFEST.sha256"

printf 'relative_path\tsha256\tbytes\tcategory\n' > "$output"
while IFS= read -r -d '' file; do
  relative=${file#"$root/"}
  case "$relative" in
    code/*) category=CODE ;;
    raw/*) category=RAW_OUTPUT ;;
    commands/*) category=COMMAND_EVIDENCE ;;
    tables/*) category=TABLE ;;
    resources/*) category=RESOURCE_GATE ;;
    REPORT.md) category=REPORT ;;
    *) category=OTHER ;;
  esac
  sha=$(sha256sum "$file" | awk '{print $1}')
  bytes=$(stat -c '%s' "$file")
  printf '%s\t%s\t%s\t%s\n' "$relative" "$sha" "$bytes" "$category" >> "$output"
done < <(find "$root" -type f ! -name INDEX.tsv ! -name MANIFEST.sha256 -print0 | sort -z)

(cd "$root" && find . -type f ! -name MANIFEST.sha256 -print0 | sort -z | xargs -0 sha256sum > MANIFEST.sha256)
