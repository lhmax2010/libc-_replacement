import csv,json,pathlib,re
base=pathlib.Path('progress/ARM_LZMA_0921');work=pathlib.Path('tmp/ARM_LZMA_0921')
cal=json.loads((base/'CALIBRATION_MONOTONIC.json').read_text());out=[]
for r in cal:
    if not r['name'].endswith('-tap'):continue
    p=work/(r['name']+'.calls.tsv')
    lines=p.read_text().splitlines();rows=list(csv.DictReader(lines[1:],delimiter='\t'))
    expected=[re.search(r'call=(\d+) ret=(\d+) errno=(\d+)',s).groups() for s in r['summary'] if s.startswith('call=')]
    actual=[(a['seq'],a['ret'],a['errno_after']) for a in rows]
    assert actual==expected,(actual,expected)
    times=[int(a['monotonic_sec'])*1000000000+int(a['monotonic_nsec']) for a in rows]
    assert times==sorted(times) and min(times)>0
    out.append({'case':r['name'],'rows':len(rows),'returns':[int(a['ret']) for a in rows],'exact_log_vs_caller':True,'monotonic_nondecreasing':True})
(base/'MONOTONIC_LOG_CALIBRATION.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps(out))
