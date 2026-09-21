"""Summarize recorded observations; never alter the build inputs."""
import collections,hashlib,json,pathlib,re
base=pathlib.Path(__file__).resolve().parent
root=base.parent.parent
rows=[json.loads(s) for s in (base/'cells/w5-original/threads.jsonl').read_text().splitlines()]
observed=[]
for row in rows:
    for proc in row['processes']:
        match=re.search(r'^Threads:\s+(\d+)',proc['status'],re.M)
        observed.append(dict(time=row['time'],pid=proc['pid'],wrote=row['wrote'],threads=int(match[1]),tasks=len(proc['tasks']),environment=proc['selected_environment']))
previous=[]
for name in ('rpmbuild-host-snapshot-125730.json','rpmbuild-host-snapshot-132743.json'):
    p=root/'progress/ARM_LZMA_0921'/name
    row=json.loads(p.read_text())
    status=row['host_proc_status']
    previous.append(dict(source=str(p.relative_to(root)),threads=int(re.search(r'^Threads:\s+(\d+)',status,re.M)[1])))
result=dict(samples=len(observed),counts=dict(collections.Counter(r['threads'] for r in observed)),writing_samples=sum(r['wrote']>0 for r in observed),previous=previous,observations=observed)
(base/'THREAD_SUMMARY.json').write_text(json.dumps(result,indent=2)+'\n')
before=json.loads((base/'INPUT_SHA256.json').read_text())
after={}
for name,expected in before.items():
    h=hashlib.sha256()
    with (root/name).open('rb') as f:
        for block in iter(lambda:f.read(1024*1024),b''):h.update(block)
    after[name]=dict(before=expected,after=h.hexdigest(),unchanged=h.hexdigest()==expected)
(base/'INPUT_RECHECK.json').write_text(json.dumps(after,indent=2)+'\n')
assert all(r['unchanged'] for r in after.values())
scripts={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(base.glob('*.py'))}
(base/'SCRIPT_SHA256.json').write_text(json.dumps(scripts,indent=2)+'\n')
print(json.dumps(dict(samples=result['samples'],counts=result['counts'],writing_samples=result['writing_samples'],previous=previous,inputs_unchanged=True),ensure_ascii=False))
