#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R11"
META="$REPO/tmp/R11/repodata"
RAW="$OUT/raw"
LEDGER="$OUT/commands/03_metadata_relations_and_denominators.log"
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
record_to parse_binary_relations "$RAW/parse_binary_relations.stdout.txt" python3 "$OUT/tools/parse_relations.py" \
  --output "$OUT/tables/binary_relations.tsv" \
  "base_binary:binary:$META/base_binary.primary.xml.gz" \
  "unified_binary:binary:$META/unified_binary.primary.xml.gz" || exit 90
record_to parse_source_relations "$RAW/parse_source_relations.stdout.txt" python3 "$OUT/tools/parse_relations.py" \
  --output "$OUT/tables/source_relations.tsv" \
  "base_source:source:$META/base_source.primary.xml.gz" \
  "unified_source:source:$META/unified_source.primary.xml.gz" || exit 91

record_to count_units "$OUT/tables/counting_units.tsv" python3 - "$OUT/tables/binary_package_records.tsv" "$OUT/tables/source_package_records.tsv" <<'PY'
import csv, collections, sys
b=list(csv.DictReader(open(sys.argv[1]),delimiter='\t'))
s=list(csv.DictReader(open(sys.argv[2]),delimiter='\t'))
mapped=collections.defaultdict(set)
for r in b: mapped[r['sourcerpm']].add((r['name'],r['arch'],r['version'],r['release']))
print('unit\ttotal\tdefinition\tmapping_rule')
print(f'source_package_repo_record\t{len(s)}\tall src architecture records in frozen Base+Unified source primary metadata\tsource NEVRA')
print(f'binary_package_repo_record\t{len(b)}\tall unique binary NEVRA records in frozen Base+Unified binary primary metadata\teach record maps through rpm:sourcerpm')
print('ELF_file\tPENDING_AFTER_EXTRACTION\tregular files accepted by readelf -h after checksum-verified RPM extraction\towned by exactly one scanned binary RPM record in the extraction corpus')
print(f'distinct_sourcerpm_referenced_by_binaries\t{len(mapped)}\tdistinct rpm:sourcerpm strings among binary records\tone source RPM to one-or-more binary package records')
print(f'binary_records_with_sourcerpm\t{sum(bool(r["sourcerpm"]) for r in b)}\tbinary package records\texact metadata field')
print(f'binary_records_without_sourcerpm\t{sum(not r["sourcerpm"] for r in b)}\tbinary package records\texact metadata field')
for arch,n in sorted(collections.Counter(r['arch'] for r in b).items()):
    print(f'binary_arch_{arch}\t{n}\tbinary package records\texact metadata arch')
PY
count_rc=$?
printf 'EXIT_CODE=%s\n\n' "$count_rc" >>"$LEDGER"
[[ $count_rc -eq 0 ]] || exit 92

record_to libunwind_metadata_packages "$OUT/tables/platform_libunwind_packages.tsv" python3 - "$OUT/tables/binary_package_records.tsv" <<'PY'
import csv,sys
r=list(csv.DictReader(open(sys.argv[1]),delimiter='\t'))
print('repo_id\tname\tarch\tepoch\tversion\trelease\tsourcerpm\tlocation\tchecksum')
for x in r:
    if x['name'] in {'libunwind','libunwind-devel'}:
        print('\t'.join(x[k] for k in ('repo_id','name','arch','epoch','version','release','sourcerpm','location','checksum')))
PY
libpkg_rc=$?
printf 'EXIT_CODE=%s\n\n' "$libpkg_rc" >>"$LEDGER"

record_to libunwind_dependency_metadata "$OUT/tables/libunwind_dependency_metadata.tsv" python3 - "$OUT/tables/binary_relations.tsv" "$OUT/tables/source_relations.tsv" <<'PY'
import csv,sys
print('dependency_scope\trepo_id\tpackage_name\tpackage_arch\tpackage_nevra\trelation_name\tclassification')
for path,scope in ((sys.argv[1],'binary_Requires'),(sys.argv[2],'source_BuildRequires')):
    for r in csv.DictReader(open(path),delimiter='\t'):
        if r['relation_type']!='requires': continue
        name=r['relation_name']
        if name in {'libunwind','libunwind-devel'}:
            cls='EXACT_PACKAGE_NAME'
        elif name.startswith('libunwind.so'):
            cls='RUNTIME_SONAME_REQUIREMENT'
        else:
            continue
        print('\t'.join([scope,r['repo_id'],r['package_name'],r['package_arch'],r['package_nevra'],name,cls]))
PY
libdep_rc=$?
printf 'EXIT_CODE=%s\n\n' "$libdep_rc" >>"$LEDGER"
record_to relation_table_sha256 "$OUT/evidence/relation_table_sha256.txt" sha256sum \
  "$OUT/tables/binary_relations.tsv" "$OUT/tables/source_relations.tsv" \
  "$OUT/tables/counting_units.tsv" "$OUT/tables/platform_libunwind_packages.tsv" \
  "$OUT/tables/libunwind_dependency_metadata.tsv" || exit 93
printf 'OVERALL_EXIT_CODE=0\n' >>"$LEDGER"
