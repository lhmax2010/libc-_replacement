#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R11"
TMP="$REPO/tmp/R11"
META="$TMP/repodata"
RAW="$OUT/raw"
LEDGER="$OUT/commands/00_freeze_repodata.log"
mkdir -p "$OUT/commands" "$RAW" "$OUT/tables" "$OUT/evidence" "$META"
: >"$LEDGER"

record_to() {
  local label=$1 outfile=$2
  shift 2
  {
    printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
    printf '%q ' "$@"
    printf '\n'
  } >>"$LEDGER"
  "$@" >"$outfile" 2>"$RAW/$label.stderr.txt"
  local rc=$?
  printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
  return "$rc"
}

cd "$REPO" || exit 125
record_to project_identity "$OUT/evidence/project_identity.txt" bash -o pipefail -c \
  'printf "pwd=%s\nbranch=%s\nhead=%s\n" "$PWD" "$(git branch --show-current)" "$(git rev-parse HEAD)"; git status --short' || exit 90
record_to capacity "$OUT/evidence/capacity.txt" df -h "$REPO" "$TMP" || exit 91

tail -n +2 "$OUT/inputs/repositories.tsv" | while IFS=$'\t' read -r repo_id kind base_url; do
  record_to "${repo_id}_repomd_headers" "$META/${repo_id}.headers" curl -fsSIL --retry 3 --retry-delay 2 "$base_url/repodata/repomd.xml" || exit 92
  record_to "${repo_id}_repomd" "$META/${repo_id}.repomd.xml" curl -fsSL --retry 3 --retry-delay 2 "$base_url/repodata/repomd.xml" || exit 93
done || exit $?

record_to parse_repomd "$OUT/tables/repodata_inventory.tsv" python3 - "$OUT/inputs/repositories.tsv" "$META" <<'PY'
import csv, hashlib, pathlib, sys, xml.etree.ElementTree as ET
repos = pathlib.Path(sys.argv[1])
meta = pathlib.Path(sys.argv[2])
ns = {'r': 'http://linux.duke.edu/metadata/repo'}
print('repo_id\tkind\trevision\trepomd_sha256\tdata_type\tdeclared_sha256\thref\ttimestamp\tsize')
with repos.open() as f:
    for row in csv.DictReader(f, delimiter='\t'):
        p = meta / f"{row['repo_id']}.repomd.xml"
        root = ET.parse(p).getroot()
        revision = root.findtext('r:revision', namespaces=ns) or ''
        repohash = hashlib.sha256(p.read_bytes()).hexdigest()
        for data in root.findall('r:data', ns):
            dtype = data.attrib['type']
            if dtype not in {'primary', 'filelists'}:
                continue
            checksum = data.findtext('r:checksum', namespaces=ns) or ''
            loc = data.find('r:location', ns).attrib['href']
            timestamp = data.findtext('r:timestamp', namespaces=ns) or ''
            size = data.findtext('r:size', namespaces=ns) or ''
            print('\t'.join([row['repo_id'], row['kind'], revision, repohash, dtype, checksum, loc, timestamp, size]))
PY
parse_rc=$?
printf 'EXIT_CODE=%s\n\n' "$parse_rc" >>"$LEDGER"
[[ $parse_rc -eq 0 ]] || exit 94

tail -n +2 "$OUT/inputs/repositories.tsv" | while IFS=$'\t' read -r repo_id kind base_url; do
  while IFS=$'\t' read -r rid rkind revision repomd_sha dtype declared href timestamp size; do
    [[ $rid == "$repo_id" ]] || continue
    target="$META/${repo_id}.${dtype}.xml.gz"
    record_to "download_${repo_id}_${dtype}" "$RAW/download_${repo_id}_${dtype}.stdout.txt" curl -fsSL --retry 3 --retry-delay 2 -o "$target" "$base_url/$href" || exit 95
    record_to "verify_${repo_id}_${dtype}" "$RAW/verify_${repo_id}_${dtype}.stdout.txt" bash -o pipefail -c \
      'printf "actual_sha256="; sha256sum "$1" | cut -d" " -f1; printf "declared_sha256=%s\n" "$2"; test "$(sha256sum "$1" | cut -d" " -f1)" = "$2"; gzip -t "$1"' _ "$target" "$declared" || exit 96
  done < <(tail -n +2 "$OUT/tables/repodata_inventory.tsv")
done || exit $?

record_to metadata_sha256 "$OUT/evidence/metadata_sha256.txt" bash -o pipefail -c \
  'find "$1" -maxdepth 1 -type f -print0 | LC_ALL=C sort -z | xargs -0 sha256sum' _ "$META" || exit 97
record_to authoritative_source_unchanged "$OUT/evidence/llvm_source_identity.txt" bash -o pipefail -c \
  'printf "branch=%s\nhead=%s\n" "$(git -C "$1" branch --show-current)" "$(git -C "$1" rev-parse HEAD)"; git -C "$1" status --porcelain=v1; test "$(git -C "$1" branch --show-current)" = tizen_base; test "$(git -C "$1" rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b; test -z "$(git -C "$1" status --porcelain=v1)"' _ "$REPO/codes/llvm" || exit 98
printf 'OVERALL_EXIT_CODE=0\n' >>"$LEDGER"
