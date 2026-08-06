#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R11"
TMP="$REPO/tmp/R11"
RAW="$OUT/raw"
LEDGER="$OUT/commands/02_download_binary_rpms.log"
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
record_to assert_download_root_fresh "$RAW/assert_download_root_fresh.stdout.txt" test ! -e "$TMP/rpms" || exit 90
{
  printf 'LABEL=download_all_binary_rpms\nPWD=%s\nCOMMAND=' "$PWD"
  printf '%q ' python3 "$OUT/tools/download_rpms.py" \
    --manifest "$OUT/tables/binary_package_records.tsv" \
    --repositories "$OUT/inputs/repositories.tsv" \
    --output-root "$TMP/rpms" \
    --status "$OUT/tables/binary_download_status.tsv" \
    --ledger "$OUT/commands/02_binary_rpm_individual_downloads.log" \
    --jobs 24
  printf '\nOUTPUT_BEGIN\n'
} >>"$LEDGER"
python3 "$OUT/tools/download_rpms.py" \
  --manifest "$OUT/tables/binary_package_records.tsv" \
  --repositories "$OUT/inputs/repositories.tsv" \
  --output-root "$TMP/rpms" \
  --status "$OUT/tables/binary_download_status.tsv" \
  --ledger "$OUT/commands/02_binary_rpm_individual_downloads.log" \
  --jobs 24 2>&1 | tee -a "$LEDGER"
download_rc=${PIPESTATUS[0]}
printf 'OUTPUT_END\nEXIT_CODE=%s\n\n' "$download_rc" >>"$LEDGER"

record_to download_summary "$OUT/tables/binary_download_summary.tsv" python3 - "$OUT/tables/binary_download_status.tsv" <<'PY'
import csv, collections, sys
rows=list(csv.DictReader(open(sys.argv[1]), delimiter='\t'))
print('metric\tvalue\tunit_or_definition')
print(f'attempted\t{len(rows)}\tbinary package records')
print(f'passed\t{sum(r["result"]=="PASS" for r in rows)}\tbinary packages downloaded and checksum-verified')
print(f'failed\t{sum(r["result"]!="PASS" for r in rows)}\tbinary packages')
print(f'bytes_verified\t{sum(int(r["package_size"] or 0) for r in rows if r["result"]=="PASS")}\tbytes')
for arch,n in sorted(collections.Counter(r['arch'] for r in rows if r['result']=='PASS').items()):
    print(f'passed_arch_{arch}\t{n}\tbinary packages')
PY
summary_rc=$?
printf 'EXIT_CODE=%s\n\n' "$summary_rc" >>"$LEDGER"
record_to download_failures "$OUT/tables/binary_download_failures.tsv" python3 - "$OUT/tables/binary_download_status.tsv" <<'PY'
import csv, sys
rows=list(csv.DictReader(open(sys.argv[1]), delimiter='\t'))
fields=['repo_id','name','arch','epoch','version','release','location','exit_code','result','url']
print('\t'.join(fields))
for r in rows:
    if r['result']!='PASS': print('\t'.join(r[k] for k in fields))
PY
failure_table_rc=$?
printf 'EXIT_CODE=%s\n\n' "$failure_table_rc" >>"$LEDGER"
printf 'DOWNLOAD_COMMAND_EXIT=%s\n' "$download_rc" >>"$LEDGER"
printf 'OVERALL_EXIT_CODE=0\n' >>"$LEDGER"
exit 0
