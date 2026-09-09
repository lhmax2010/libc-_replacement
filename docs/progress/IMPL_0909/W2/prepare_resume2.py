#!/usr/bin/env python3
"""保留原 7848 和首轮补测 60 个正式 PASS；基础设施不完整项单列。"""
import csv,json,pathlib,re
p=pathlib.Path('docs/progress/IMPL_0909/W2');old=pathlib.Path('docs/progress/IMPL_0908/W3')
good=[x['test'] for x in csv.DictReader((old/'ARM_RESULT_SELECTION.tsv').open(),delimiter='\t')]
data=json.loads((p/'raw/lit_resume/result.json').read_text());bad=[];new=[]
for x in data['tests']:
    if x['code']=='PASS':new.append(x['name'])
    else:
        assert x['code']=='FAIL' and ('CONNECTION_FAILURE already recorded' in x.get('output','') or 'RuntimeError: remote exit marker absent' in x.get('output','')),x['name']
        bad.append([x['name'],'FAIL','INFRASTRUCTURE_INCOMPLETE','cleanup receipt absent after test exit 0' if 'RuntimeError:' in x['output'] else 'refused before board work'])
assert len(new)==60 and len(bad)==60
assert not set(new)&set(good);good+=new
pattern='^(?:'+'|'.join(map(re.escape,good))+')$';rx=re.compile(pattern)
assert all(rx.search(x) for x in good) and not any(rx.search(x[0]) for x in bad)
(p/'resume2.args').write_text('--filter-out\n'+pattern+'\n')
with (p/'INTERRUPTION_CLASSIFICATION.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['test','raw_lit_code','classification','reason']);w.writerows(bad)
print('ORIGINAL_PRESERVED=7848 NEW_VALID_PRESERVED=60 REMAINING=3494; original FAIL records unchanged')
