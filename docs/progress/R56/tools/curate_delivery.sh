#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
source_root="$workspace/progress/R56"
target_root="$workspace/docs/progress/R56"

mkdir -p "$target_root" "$target_root/code/commands" \
  "$target_root/code/output" "$target_root/code/config"

cp -a "$source_root/REPORT.md" "$source_root/RESULT.txt" \
  "$source_root/NOT_OBSERVED.md" "$target_root/"
cp -a "$source_root/tables" "$source_root/commands" \
  "$source_root/raw" "$source_root/tools" "$target_root/"
cp -a "$source_root/code/tests" "$source_root/code/NO_IMPLEMENTATION.md" \
  "$target_root/code/"

cp -a "$source_root/commands/003_compiler_version.command.txt" \
  "$source_root/commands/003_compiler_version.exit.txt" \
  "$source_root/commands/004_forward_libcxx.command.txt" \
  "$source_root/commands/004_forward_libcxx.exit.txt" \
  "$source_root/commands/005_forward_libstdcxx.command.txt" \
  "$source_root/commands/005_forward_libstdcxx.exit.txt" \
  "$source_root/commands/006_include_libcxx.command.txt" \
  "$source_root/commands/006_include_libcxx.exit.txt" \
  "$source_root/commands/007_include_libstdcxx.command.txt" \
  "$source_root/commands/007_include_libstdcxx.exit.txt" \
  "$target_root/code/commands/"
cp -a "$source_root/raw/feasibility"/*.log "$target_root/code/output/"
cp -a "$source_root/raw/identity/upstream_baseline.log" \
  "$source_root/raw/identity/source_sha256.log" \
  "$source_root/raw/identity/control_object_sha256.log" \
  "$source_root/raw/identity/control_symbols.log" \
  "$source_root/raw/identity/control_symbols_libstdcxx.log" \
  "$target_root/code/config/"

printf 'path\tsize_bytes\tsha256\treason\n' > "$target_root/EXCLUDED.tsv"
rm -f "$target_root/files_over_5MiB.txt"

oversized=$(find "$target_root" -type f -size +5M -print -quit)
if [[ -n $oversized ]]; then
  printf '%s\n' "$oversized" > "$target_root/files_over_5MiB.txt"
  exit 3
fi

{
  printf 'source_path\tupload_path\tsize_bytes\tsha256\n'
  find "$target_root" -type f ! -name INDEX.tsv -print0 |
    sort -z |
    while IFS= read -r -d '' file; do
    relative=${file#"$workspace/"}
    original=${relative#docs/}
    case "$relative" in
      docs/progress/R56/REPORT.md|docs/progress/R56/RESULT.txt|docs/progress/R56/NOT_OBSERVED.md|docs/progress/R56/tables/*|docs/progress/R56/commands/*|docs/progress/R56/raw/*|docs/progress/R56/tools/*)
        original="progress/R56/${relative#docs/progress/R56/}"
        ;;
      docs/progress/R56/code/tests/*|docs/progress/R56/code/NO_IMPLEMENTATION.md)
        original="progress/R56/code/${relative#docs/progress/R56/code/}"
        ;;
      docs/progress/R56/code/commands/*)
        original="progress/R56/commands/${relative#docs/progress/R56/code/commands/}"
        ;;
      docs/progress/R56/code/output/*)
        original="progress/R56/raw/feasibility/${relative#docs/progress/R56/code/output/}"
        ;;
      docs/progress/R56/code/config/*)
        original="progress/R56/raw/identity/${relative#docs/progress/R56/code/config/}"
        ;;
      docs/progress/R56/EXCLUDED.tsv)
        original='generated_for_R56_curation'
        ;;
    esac
    size=$(stat -c '%s' "$file")
    sha=$(sha256sum "$file" | awk '{print $1}')
      printf '%s\t%s\t%s\t%s\n' "$original" "$relative" "$size" "$sha"
    done
} > "$target_root/INDEX.tsv"
