#!/bin/bash
set -eu
workspace=/home/toolchain/development/libc++_replacement
cd "$workspace"

# Refresh command/resource evidence after all task actions.
cp progress/R42/commands/*.log docs/progress/R42/code/evidence/
cp progress/R42/resources/*.log docs/progress/R42/code/evidence/ 2>/dev/null || true
cp progress/R42/tools/*.sh docs/progress/R42/code/tools/

index=docs/progress/R42/code/INDEX.tsv
printf 'source_task\tsource_or_role\tupload_path\tsha256\n' >"$index"
while IFS= read -r path; do
  sha=$(sha256sum "$path" | awk '{print $1}')
  case "$path" in
    */libcxxabi/*) role='R42 experimental libc++abi complete source' ;;
    */tests/*) role='R42 exact test source' ;;
    */tools/*) role='R42 task script' ;;
    */build/*) role='R42 CMake configuration' ;;
    */evidence/*) role='R42 raw command/output' ;;
    */reference/*) role='Arm official EHABI fixed source' ;;
    *.diff) role='R42 source diff' ;;
    *) role='R42 curated material' ;;
  esac
  printf 'R42\t%s\t%s\t%s\n' "$role" "$path" "$sha" >>"$index"
done < <(find docs/progress/R42/code -type f ! -name INDEX.tsv ! -name MANIFEST.sha256 | sort)

find docs/progress/R42 -type f ! -name MANIFEST.sha256 -print0 | sort -z | xargs -0 sha256sum > docs/progress/R42/MANIFEST.sha256
find docs/upstream/libcxxabi-forced-unwind -type f ! -name MANIFEST.sha256 -print0 | sort -z | xargs -0 sha256sum > docs/upstream/libcxxabi-forced-unwind/MANIFEST.sha256
find progress/R42 -type f ! -name MANIFEST.sha256 -print0 | sort -z | xargs -0 sha256sum > progress/R42/MANIFEST.sha256

printf 'FILES_OVER_5MB_BEGIN\n'
find docs/progress/R42 docs/upstream/libcxxabi-forced-unwind -type f -size +5M -print
printf 'FILES_OVER_5MB_END\n'
sha256sum docs/progress/R42/MANIFEST.sha256 docs/progress/R42/code/INDEX.tsv \
  docs/upstream/libcxxabi-forced-unwind/MANIFEST.sha256 progress/R42/MANIFEST.sha256
