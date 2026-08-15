#!/bin/bash
set -u

workspace=/home/toolchain/development/libc++_replacement
cd "$workspace" || exit 2
curated=docs/progress/R41
log=progress/R41/commands/053_finalize_evidence.log
{
  printf 'COMMAND=progress/R41/tools/finalize_evidence.sh\n'
  printf 'COMMAND=cp -a progress/R41/commands/. %s/code/evidence/commands/\n' "$curated"
  cp -a progress/R41/commands/. "$curated/code/evidence/commands/"
  rc1=$?; printf 'COPY_COMMANDS_EXIT_CODE=%d\n' "$rc1"
  printf 'COMMAND=cp -a progress/R41/resources/. %s/code/evidence/resources/\n' "$curated"
  cp -a progress/R41/resources/. "$curated/code/evidence/resources/"
  rc2=$?; printf 'COPY_RESOURCES_EXIT_CODE=%d\n' "$rc2"
  printf 'COMMAND=cp -a progress/R41/tools/. %s/code/tools/\n' "$curated"
  cp -a progress/R41/tools/. "$curated/code/tools/"
  rc3=$?; printf 'COPY_TOOLS_EXIT_CODE=%d\n' "$rc3"
  printf 'COMMAND=cp -a progress/R41/R41_report.md %s/R41_report.md\n' "$curated"
  cp -a progress/R41/R41_report.md "$curated/R41_report.md"
  rc4=$?; printf 'COPY_REPORT_EXIT_CODE=%d\n' "$rc4"
  printf 'COMMAND=cp -a progress/R41/tables/. %s/tables/\n' "$curated"
  cp -a progress/R41/tables/. "$curated/tables/"
  rc5=$?; printf 'COPY_TABLES_EXIT_CODE=%d\n' "$rc5"
  printf 'EXIT_CODE=%d\n' "$((rc1 || rc2 || rc3 || rc4 || rc5))"
} >"$log" 2>&1
rc=$?
cp -a "$log" "$curated/code/evidence/commands/053_finalize_evidence.log"

printf 'upload_path\tsize_bytes\tsha256\n' > "$curated/code/INDEX.tsv"
find "$curated/code" -type f ! -path "$curated/code/INDEX.tsv" -print0 | sort -z |
  while IFS= read -r -d '' file; do
    size=$(stat -c %s "$file")
    sha=$(sha256sum "$file" | cut -d' ' -f1)
    printf '%s\t%s\t%s\n' "$file" "$size" "$sha"
  done >> "$curated/code/INDEX.tsv"

find "$curated" -type f ! -name MANIFEST.sha256 -print0 | sort -z |
  xargs -0 sha256sum > "$curated/MANIFEST.sha256"
find progress/R41 -type f ! -name MANIFEST.sha256 -print0 | sort -z |
  xargs -0 sha256sum > progress/R41/MANIFEST.sha256

exit "$rc"

