"""Read-only comparison of the two completed runs; no new experiment."""
import datetime
import hashlib
import json
import pathlib

b = pathlib.Path('progress/ARM_REPRO_0920')
old = json.loads((b / 'original-argv.json').read_text())
one = json.loads((b / 'cell_commands/original-argv-repro.json').read_text())
two = json.loads((b / 'cell_commands/vv-only-repro.json').read_text())
normalized_one = one.copy()
normalized_one[normalized_one.index('-o') + 1] = old[old.index('-o') + 1]
assert normalized_one == old
normalized_two = two.copy()
assert normalized_two.count('-vv') == 1
normalized_two.remove('-vv')
normalized_two[normalized_two.index('-o') + 1] = one[one.index('-o') + 1]
assert normalized_two == one
assert hashlib.sha256(pathlib.Path('tmp/WEEKEND_0918/specs/llvm.spec').read_bytes()).hexdigest() == 'cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68'

records = []
for label in ('original-argv-repro', 'vv-only-repro'):
    p = b / 'cells' / label
    events = [json.loads(s) for s in (p / 'events.jsonl').read_text().splitlines()]
    start = next(e for e in events if e['event'] == 'start')
    end = next(e for e in reversed(events) if e['event'] == 'finished')
    lines = (p / 'build.log').read_text().splitlines()
    wrote = [{'line': i, 'name': pathlib.Path(s[7:]).name, 'text': s} for i, s in enumerate(lines, 1) if s.startswith('Wrote: ')]
    records.append({
        'label': label, 'start': start['time'], 'finish': end['time'],
        'seconds': (datetime.datetime.fromisoformat(end['time']) - datetime.datetime.fromisoformat(start['time'])).total_seconds(),
        'exitcode': end['exitcode'], 'completed_targets': end['completed'],
        'wrote': wrote,
        'error_lines': [{'line': i, 'text': s} for i, s in enumerate(lines, 1) if s.startswith('error:') or 'cpio: write failed' in s],
        'log_sha256': hashlib.sha256((p / 'build.log').read_bytes()).hexdigest(),
    })

first_names = [x['name'] for x in records[0]['wrote']]
vv_names = [x['name'] for x in records[1]['wrote']]
vv = json.loads((b / 'VV_RESULT.json').read_text())
assert vv['decision'] == 'STOP_VV_FAILURE_DISAPPEARED'
assert len(vv['rpm_inventory']) == 22
assert all(x['exitcode'] == 0 for x in vv['rpm_inventory'])
assert set(vv_names) == {pathlib.Path(x['path']).name for x in vv['rpm_inventory']}
out = {'decision': vv['decision'], 'original_argv_only_trace_output_diff': True,
       'vv_argv_only_vv_and_trace_output_diff': True,
       'first_18_order_equal': vv_names[:18] == first_names,
       'additional_rpms': [n for n in vv_names if n not in first_names],
       'runs': records, 'archive_local_values': 'NOT_OBSERVED',
       'no_further_instrumentation_started': True}
(b / 'FINAL_AUDIT.json').write_text(json.dumps(out, ensure_ascii=False, indent=2) + '\n')
with (b / 'RPM_ORDER_COMPARISON.tsv').open('w') as f:
    f.write('order\tname\toriginal_log_line\tvv_log_line\tstatus\n')
    for i, r in enumerate(records[1]['wrote'], 1):
        before = next((x for x in records[0]['wrote'] if x['name'] == r['name']), None)
        f.write(f"{i}\t{r['name']}\t{before['line'] if before else 'NOT_WRITTEN'}\t{r['line']}\t{'both_written' if before else 'vv_only_written'}\n")
print(json.dumps(out, ensure_ascii=False, indent=2))
