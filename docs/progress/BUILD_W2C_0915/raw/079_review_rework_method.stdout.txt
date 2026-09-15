"""Compare observed target labels and recorded Ninja durations, not a counterfactual."""
import collections
import csv
import json
import re
from pathlib import Path

p = Path('progress/BUILD_W2C_0915')
old = Path('progress/BUILD_W2B_0915/raw/009_rebuild_llvm_armv7l.stdout.txt')
new = p/'build-w2c-llvm-confirmed.build.log'
def labels(path, total):
    values = []
    for line in path.read_text(errors='replace').splitlines():
        m = re.search(r'\[(\d+)/' + str(total) + r'\] (.*)', line)
        if m:
            values.append((int(m[1]), m[2]))
    return values
before = labels(old, 7147)
after = labels(new, 7146)
old_counts = collections.Counter(label for _, label in before)
new_counts = collections.Counter(label for _, label in after)
common = old_counts & new_counts
with (p/'reworked_target_labels.tsv').open('w') as f:
    writer = csv.writer(f, delimiter='\t')
    writer.writerow(['target_label', 'old_completions', 'new_completions', 'matched_rework'])
    for label in sorted(common):
        writer.writerow([label, old_counts[label], new_counts[label], common[label]])
root = Path('tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/BUILD/llvm-22.1.8/build')
recorded = {}
for line in (root/'.ninja_log').read_text().splitlines():
    parts = line.split('\t')
    if len(parts) == 5:
        recorded[parts[3]] = (int(parts[0]), int(parts[1]))
durations = []
for label in common:
    m = re.match(r'Building (?:CXX|C|ASM) object (.*)', label)
    if m and m[1] in recorded:
        start, end = recorded[m[1]]
        durations.append((m[1], start, end, end-start))
with (p/'reworked_object_durations.tsv').open('w') as f:
    writer = csv.writer(f, delimiter='\t')
    writer.writerow(['object', 'ninja_start_ms', 'ninja_end_ms', 'elapsed_ms'])
    writer.writerows(durations)
result = dict(old_observed_target_lines=len(before), new_observed_target_lines=len(after),
              matched_target_completions=sum(common.values()),
              object_duration_records=len(durations),
              recorded_object_elapsed_seconds=sum(x[3] for x in durations)/1000,
              qualification='Only completed matching target labels are counted. Object durations are recorded task elapsed times, not exact counterfactual additional wall-clock time; generation/link targets and configure recovery time are not included in this sum.')
(p/'rework_summary.json').write_text(json.dumps(result, ensure_ascii=False, indent=2)+'\n')
print(json.dumps(result, ensure_ascii=False))
