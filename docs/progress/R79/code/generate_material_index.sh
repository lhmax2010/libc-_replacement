#!/usr/bin/env bash
set -euo pipefail
root="${1:?usage: generate_material_index.sh DOCS_R79_ROOT}"
index="$root/INDEX.tsv"
temporary="$root/.INDEX.tsv.tmp"
printf 'source\toriginal_path\tupload_path\tsha256\n' > "$temporary"
while IFS= read -r -d '' file; do
  relative="${file#$root/}"
  if [[ "$relative" == INDEX.tsv || "$relative" == .INDEX.tsv.tmp ]]; then continue; fi
  digest=$(sha256sum "$file"); digest=${digest%% *}
  printf 'R79_GENERATED_OR_CAPTURED\tprogress/R79/%s\t%s\t%s\n' "$relative" "$file" "$digest" >> "$temporary"
done < <(find "$root" -type f -print0 | sort -z)
mv "$temporary" "$index"
