#!/usr/bin/env python3
"""按需只读已冻结证据；逐文件读取，避免重建全局索引。"""
import csv
import gzip
import json
import pathlib
import re
import sys
import subprocess
import shlex

base = pathlib.Path(__file__).resolve().parents[1]
mode, *args = sys.argv[1:]
if mode == 'proof':
    for key in args:
        obj = json.loads((base / 'W1/positive' / (key + '.json')).read_text())
        print('\nPROOF', key, 'SPEC', obj['spec'])
        for h in obj['header_matches']:
            print('HEADER', h['metadata']['cache_path'], h['metadata']['path'], h['context'])
        for e in obj['elf_matches']:
            print('ELF', e['symbol']['demangled'])
            print('CONSUMER', e['consumer']['path'], e['consumer_lines'])
            print('PROVIDER', e['provider']['path'], e['provider_lines'])
        for inc in obj['source_includes']:
            print('INCLUDE', inc['exit'], *inc['matches'][:10], sep='\n')
elif mode == 'includes':
    obj = json.loads((base / 'W1/positive' / (args[0] + '.json')).read_text())
    for item in obj['source_includes']:
        cmd = ['rg', '-n', '-uuu', args[1], str(pathlib.Path(item['root']) / 'unpacked')]
        print('command: ' + shlex.join(cmd), flush=True)
        result = subprocess.run(cmd)
        print('exitcode=' + str(result.returncode), flush=True)
        if result.returncode not in (0, 1):
            raise SystemExit(result.returncode)
elif mode == 'context':
    name = args[0]
    pattern = re.compile(args[1]) if len(args) > 1 else None
    p = base / 'W1/entity_full' / (name.replace('::', '_') + '.json.gz')
    with gzip.open(p, 'rt') as f:
        rows = json.load(f)
    seen = set()
    for r in rows:
        identity = (r['file_sha256'], r['line']) if 'file_sha256' in r else (r['installed_path'], r['line'], r['context'])
        if identity in seen:
            continue
        seen.add(identity)
        if pattern and not pattern.search(str(r)):
            continue
        print(json.dumps(r, ensure_ascii=False))
    print('total_occurrences=', len(rows), 'unique_contexts=', len(seen))
elif mode == 'order':
    with (base / 'W1/ORDER_495.tsv').open() as f:
        rows = list(csv.DictReader(f, delimiter='\t'))
    for r in rows[int(args[0])-1:int(args[1])]:
        print(r['rank'], r['entity'], r['skeleton_family_count'])
else:
    raise SystemExit('unknown mode')
