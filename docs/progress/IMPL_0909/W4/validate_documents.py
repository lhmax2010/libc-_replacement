#!/usr/bin/env python3
"""交付双语文档、当前统计与 R81 来源核对；不改旧记录。"""
import csv,hashlib,json,pathlib,re
base=pathlib.Path('docs/progress/IMPL_0909/W4');rows=[]
for name in ['DELIVERY_ZH.md','DELIVERY_EN.md']:
    p=base/name;text=p.read_text()
    for token in ['11402','10070','129','1176','27','7848','6657','136','1027','28','3554','Change-Id','NOT_OBSERVED','NOT_AVAILABLE']:
        assert token in text,(name,token)
    for link in re.findall(r'\]\(([^)]+)\)',text):
        if link.startswith(('http://','https://')):continue
        q=(p.parent/link.split('#',1)[0]).resolve();assert q.exists(),(name,link)
        rows.append([name,link,hashlib.sha256(q.read_bytes()).hexdigest() if q.is_file() else 'DIRECTORY'])
    print('DOCUMENT_SHA256',hashlib.sha256(p.read_bytes()).hexdigest(),p)
assert 10070+129+1176+27==11402 and 6657+136+1027+28==7848 and 7848+3554==11402
summary=json.loads(pathlib.Path('docs/progress/R81/tables/final_summary.json').read_text())
assert summary['libcxx']['owner_normalized_differences']==0 and summary['libcxxabi']['differences']==0
historical=list(csv.DictReader(open('docs/progress/IMPL_0908/W3/historical_baseline.tsv'),delimiter='\t'))
for suite,file in [('libcxx','docs/progress/R81/raw/phase2/results/baseline_armv7l/libcxx/aggregate_result_owner_normalized.json'),
                   ('libcxxabi','docs/progress/R81/raw/phase2/results/baseline_armv7l/libcxxabi/batch-001/result.json')]:
    p=pathlib.Path(file);data=json.loads(p.read_text())
    expected={e['name'].split(' :: ',1)[-1]:e['code'] for e in data['tests']}
    actual={e['test']:e['historical_code'] for e in historical if e['arch']=='armv7l' and e['suite']==suite}
    assert expected==actual,(suite,len(expected),len(actual))
    print('R81_BASELINE_EXACT_MATCH',suite,len(actual),'SHA256',hashlib.sha256(p.read_bytes()).hexdigest())
with (base/'EVIDENCE_INDEX.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['document','reference','sha256']);w.writerows(rows)
print('BILINGUAL_NUMBERS_AND_LOCAL_LINKS=PASS',len(rows))
print('SCRIPT_SHA256',hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest())
