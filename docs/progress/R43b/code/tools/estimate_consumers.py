#!/usr/bin/env python3
import csv
import hashlib
import json
import pathlib
import re
import subprocess

WORKSPACE = pathlib.Path('/home/toolchain/development/libc++_replacement')
EXTRACTED = WORKSPACE / 'tmp/corpus/extracted'
EXTRACTION = WORKSPACE / 'progress/T1/extraction_results.tsv'
R32 = WORKSPACE / 'progress/R32/tables/impact_package_classification.tsv'
OUT = WORKSPACE / 'progress/R43b/tables'

facility_patterns = {
    'iostream': re.compile(r'(?:#\s*include\s*[<"](?:iostream|istream|ostream)[>"]|std::(?:cout|cin|cerr|clog|getline))'),
    'future': re.compile(r'(?:#\s*include\s*[<"]future[>"]|std::(?:future|promise|packaged_task|async))'),
    'basic_string': re.compile(r'(?:#\s*include\s*[<"]string[>"]|std::(?:basic_string|string|wstring|u8string))'),
    'iomanip': re.compile(r'#\s*include\s*[<"]iomanip[>"]'),
    'condition_variable': re.compile(r'(?:#\s*include\s*[<"]condition_variable[>"]|std::condition_variable)'),
    'bitset': re.compile(r'(?:#\s*include\s*[<"]bitset[>"]|std::bitset)'),
    'extension_facilities': re.compile(r'#\s*include\s*[<"](?:experimental/executor|tr2/dynamic_bitset|ext/vstring)[>"]'),
}
combined = '|'.join(f'(?:{pattern.pattern})' for pattern in facility_patterns.values())
globs = ['*.h', '*.hh', '*.hpp', '*.hxx', '*.c', '*.cc', '*.cpp', '*.cxx', '*.tcc', '*.ipp']

with EXTRACTION.open(encoding='utf-8') as f:
    rows = list(csv.DictReader(f, delimiter='\t'))
success = {r['source_rpm'] for r in rows if r['status'] == 'EXTRACTED_OK'}

cmd = ['rg', '--json', '--no-messages', '--no-ignore', '--pcre2']
for glob in globs:
    cmd += ['-g', glob]
cmd += [combined, str(EXTRACTED)]
print('COMMAND=' + ' '.join(cmd))
package_files = {}
package_facilities = {}
proc = subprocess.Popen(
    cmd,
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE,
    text=True,
    encoding='utf-8',
    errors='replace',
)
assert proc.stdout is not None
for raw in proc.stdout:
    if not raw:
        continue
    record = json.loads(raw)
    if record.get('type') != 'match':
        continue
    data = record['data']
    path_text = data['path'].get('text')
    line_text = data['lines'].get('text', '')
    if path_text is None:
        continue
    path = pathlib.Path(path_text)
    rel = path.relative_to(EXTRACTED)
    package = rel.parts[0]
    if package not in success:
        continue
    found = {name for name, pattern in facility_patterns.items() if pattern.search(line_text)}
    if found:
        package_files.setdefault(package, set()).add(str(rel))
        package_facilities.setdefault(package, set()).update(found)
stderr = proc.stderr.read() if proc.stderr is not None else ''
returncode = proc.wait()
print(f'RG_EXIT_CODE={returncode}')
print(stderr)
if returncode not in (0, 1):
    raise SystemExit(returncode)

thread_evidence = {}
with R32.open(encoding='utf-8') as f:
    for row in csv.DictReader(f, delimiter='\t'):
        total = sum(int(row[name]) for name in (
            'thread_cancel_locations_nondoc', 'pthread_cancel_locations_nondoc',
            'pthread_create_locations_nondoc', 'pthread_setcancelstate_locations_nondoc',
            'pthread_testcancel_locations_nondoc', 'std_thread_locations_nondoc'))
        thread_evidence[row['source_rpm']] = total

OUT.mkdir(parents=True, exist_ok=True)
with (OUT / 'facility_consumers.tsv').open('w', encoding='utf-8', newline='') as f:
    writer = csv.writer(f, delimiter='\t', lineterminator='\n')
    writer.writerow(['source_rpm', 'facilities', 'matching_source_files', 'thread_cancel_evidence_locations', 'has_thread_cancel_evidence'])
    for package in sorted(package_facilities):
        evidence = thread_evidence.get(package, 0)
        writer.writerow([package, ','.join(sorted(package_facilities[package])), len(package_files[package]), evidence, 'YES' if evidence else 'NO'])

consumer_count = len(package_facilities)
thread_count = sum(1 for p in package_facilities if thread_evidence.get(p, 0))
with (OUT / 'facility_consumer_summary.tsv').open('w', encoding='utf-8', newline='') as f:
    writer = csv.writer(f, delimiter='\t', lineterminator='\n')
    writer.writerow(['metric', 'numerator', 'denominator', 'unit', 'scope'])
    writer.writerow(['packages_using_affected_facilities', consumer_count, len(success), 'source_packages', 'EXTRACTED_OK corpus; source-token approximation'])
    writer.writerow(['facility_users_with_thread_cancel_evidence', thread_count, consumer_count, 'source_packages', 'intersection with R32 thread/cancel evidence'])
    for facility in sorted(facility_patterns):
        count = sum(1 for values in package_facilities.values() if facility in values)
        writer.writerow([f'packages_using_{facility}', count, len(success), 'source_packages', 'EXTRACTED_OK corpus; source-token approximation'])

for path in (EXTRACTION, R32):
    digest = hashlib.sha256(path.read_bytes()).hexdigest()
    print(f'INPUT_SHA256={digest}  {path}')
print(f'EXTRACTED_OK_DENOMINATOR={len(success)}')
print(f'FACILITY_CONSUMER_PACKAGES={consumer_count}')
print(f'FACILITY_CONSUMERS_WITH_THREAD_CANCEL_EVIDENCE={thread_count}')
