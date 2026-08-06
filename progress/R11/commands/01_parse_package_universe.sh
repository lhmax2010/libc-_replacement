#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R11"
META="$REPO/tmp/R11/repodata"
RAW="$OUT/raw"
LEDGER="$OUT/commands/01_parse_package_universe.log"
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
record_to parse_binary_primary "$RAW/parse_binary_primary.stdout.txt" python3 "$OUT/tools/parse_primary.py" \
  --output "$OUT/tables/binary_package_records.tsv" \
  "base_binary:binary:$META/base_binary.primary.xml.gz" \
  "unified_binary:binary:$META/unified_binary.primary.xml.gz" || exit 90
record_to parse_source_primary "$RAW/parse_source_primary.stdout.txt" python3 "$OUT/tools/parse_primary.py" \
  --output "$OUT/tables/source_package_records.tsv" \
  "base_source:source:$META/base_source.primary.xml.gz" \
  "unified_source:source:$META/unified_source.primary.xml.gz" || exit 91

record_to package_universe_summary "$OUT/tables/package_universe_summary.tsv" python3 - "$OUT/tables/binary_package_records.tsv" "$OUT/tables/source_package_records.tsv" <<'PY'
import csv, collections, pathlib, sys
binary = list(csv.DictReader(open(sys.argv[1]), delimiter='\t'))
source = list(csv.DictReader(open(sys.argv[2]), delimiter='\t'))
print('metric\tvalue\tunit_or_definition')
print(f'binary_repo_records\t{len(binary)}\trepodata package records across Base+Unified')
print(f'source_repo_records\t{len(source)}\trepodata source package records across Base+Unified')
for arch, n in sorted(collections.Counter(r['arch'] for r in binary).items()):
    print(f'binary_arch_{arch}\t{n}\tbinary repo records')
identity = lambda r: (r['name'], r['epoch'], r['version'], r['release'], r['arch'])
nevra = collections.defaultdict(list)
checksums = collections.defaultdict(list)
for r in binary:
    nevra[identity(r)].append(r)
    checksums[r['checksum']].append(r)
dup_nevra = {k:v for k,v in nevra.items() if len(v)>1}
dup_checksum = {k:v for k,v in checksums.items() if len(v)>1}
print(f'unique_binary_nevra\t{len(nevra)}\tname+epoch+version+release+arch')
print(f'unique_binary_checksum\t{len(checksums)}\tRPM payload checksum')
print(f'duplicate_nevra_groups\t{len(dup_nevra)}\tNEVRA occurring in more than one repo record')
print(f'duplicate_checksum_groups\t{len(dup_checksum)}\tchecksum occurring in more than one repo record')
print(f'binary_download_bytes_all_records\t{sum(int(r["package_size"] or 0) for r in binary)}\tbytes')
print(f'binary_download_bytes_unique_checksum\t{sum(int(v[0]["package_size"] or 0) for v in checksums.values())}\tbytes')
PY
summary_rc=$?
printf 'EXIT_CODE=%s\n\n' "$summary_rc" >>"$LEDGER"
[[ $summary_rc -eq 0 ]] || exit 92

record_to duplicate_nevra "$OUT/tables/duplicate_binary_nevra.tsv" python3 - "$OUT/tables/binary_package_records.tsv" <<'PY'
import csv, collections, sys
rows=list(csv.DictReader(open(sys.argv[1]), delimiter='\t'))
groups=collections.defaultdict(list)
for r in rows: groups[(r['name'],r['epoch'],r['version'],r['release'],r['arch'])].append(r)
print('name\tepoch\tversion\trelease\tarch\trecord_count\trepos\tchecksums\tlocations')
for k,v in sorted(groups.items()):
    if len(v)>1:
        print('\t'.join((*k,str(len(v)),','.join(r['repo_id'] for r in v),','.join(r['checksum'] for r in v),','.join(r['location'] for r in v))))
PY
dup_rc=$?
printf 'EXIT_CODE=%s\n\n' "$dup_rc" >>"$LEDGER"
[[ $dup_rc -eq 0 ]] || exit 93

record_to input_table_sha256 "$OUT/evidence/package_table_sha256.txt" sha256sum \
  "$OUT/tables/binary_package_records.tsv" "$OUT/tables/source_package_records.tsv" \
  "$OUT/tables/package_universe_summary.tsv" "$OUT/tables/duplicate_binary_nevra.tsv" || exit 94
printf 'OVERALL_EXIT_CODE=0\n' >>"$LEDGER"
