"""Record downloads, source lines and selected supporting excerpts, not session data."""
import csv, hashlib, json, pathlib, re
out=pathlib.Path(__file__).resolve().parent
work=pathlib.Path('tmp/QB_1194606')
data=work/'downloads'
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def numbered(lines,low,high):
    return ''.join(f'{i+1}\t{lines[i]}\n' for i in range(low-1,min(high,len(lines))))
rows=[]
for p in sorted(data.rglob('*')):
    if not p.is_file(): continue
    rows.append(dict(path=str(p),bytes=p.stat().st_size,sha256=sha(p)))
with (out/'LOCAL_DOWNLOAD_IDENTITIES.tsv').open('w') as f:
    w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t');w.writeheader();w.writerows(rows)
jobs=[]
for p in sorted((work/'commands').glob('*.json')):
    result=work/'results'/p.name
    jobs.append(dict(id=p.name,command=json.loads(p.read_text()),result=json.loads(result.read_text()) if result.exists() else 'NOT_OBSERVED'))
(out/'BROWSER_ACTIONS.json').write_text(json.dumps(jobs,ensure_ascii=False,indent=2)+'\n')
support=[]
for p in sorted((data/'logs').rglob('*.log')):
    lines=p.read_text().splitlines()
    package=p.parent.name; arch=p.stem
    if package in ('llvm','libcxx-runtimes','bpftrace'):
        hits=[i+1 for i,s in enumerate(lines) if ('Wrote:' in s or 'build succeeded' in s or 'finished "build' in s)]
        if not hits: hits=list(range(max(1,len(lines)-15),len(lines)+1))
        support.append(f'## {package}/{arch}: {p}\n')
        support.extend(numbered(lines,i,i) for i in hits)
    elif package=='cereal':
        support.append(f'## {package}/{arch}: flags and inputs\n')
        support.extend(numbered(lines,i+1,i+1) for i,s in enumerate(lines) if ('CXXFLAGS=' in s or 'cmake ..' in s or 'CXX compiler identification' in s or 'cumulate boost-serialization' in s or 'boost-serialization-1.83.0' in s))
    elif package=='icu':
        support.append(f'## {package}/{arch}: tz2icu command\n')
        support.extend(numbered(lines,i+1,i+1) for i,s in enumerate(lines) if 'tz2icu.cpp -o' in s)
    elif package=='qemu-accel-armv7l':
        support.append(f'## {package}/{arch}: auxiliary diagnostics (not the terminal cause)\n')
        support.extend(numbered(lines,i+1,i+1) for i,s in enumerate(lines) if ('Unable to open' in s or 'printf %sn' in s or 'libc++.so.1n/' in s or 'libc++abi.so.1n/' in s))
(out/'evidence/supporting-log-lines.txt').write_text(''.join(support))
report_counts=json.loads((out/'STATUS_COUNTS.json').read_text())
assert sum(sum(v.values()) for v in report_counts.values())==774
assert sum(v.get('failed',0) for v in report_counts.values())==9
assert len(list(csv.DictReader((out/'INPUT_COMPARISON.tsv').open(),delimiter='\t')))==12
assert len(list(csv.DictReader((out/'FAILURES.tsv').open(),delimiter='\t')))==9
assert len(list(csv.DictReader((out/'DOWNLOADS.tsv').open(),delimiter='\t')))==17
print(json.dumps({'download_files':len(rows),'browser_actions':len(jobs),'all_action_results_ok':all(x['result'].get('ok') for x in jobs if isinstance(x['result'],dict)), 'counts':report_counts},indent=2))
