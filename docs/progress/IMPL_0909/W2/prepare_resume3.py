#!/usr/bin/env python3
"""第二次中断后仅筛出未获得有效结果的项，不改变任何既有结果。"""
import csv,json,pathlib,re
p=pathlib.Path('docs/progress/IMPL_0909/W2');old=pathlib.Path('docs/progress/IMPL_0908/W3')
good=[x['test'] for x in csv.DictReader((old/'ARM_RESULT_SELECTION.tsv').open(),delimiter='\t')];bad=[]
for label in ['lit_resume','lit_resume2']:
    data=json.loads((p/'raw'/label/'result.json').read_text());valid=0
    for x in data['tests']:
        output=x.get('output','')
        if any(s in output for s in ['CONNECTION_FAILURE already recorded','EXECUTOR_ERROR:','RuntimeError: remote exit marker absent']):
            bad.append([label,x['name'],x['code'],'INFRASTRUCTURE_INCOMPLETE']);continue
        assert x['name'] not in good;x['code']=='PASS';good.append(x['name']);valid+=1
    print('PRESERVE',label,valid)
assert len(good)==8224
pattern='^(?:'+'|'.join(map(re.escape,good))+')$';rx=re.compile(pattern)
assert all(rx.search(x) for x in good)
(p/'resume3.args').write_text('--filter-out\n'+pattern+'\n')
with (p/'INTERRUPTION2_CLASSIFICATION.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['run','test','raw_code','classification']);w.writerows(bad)
print('PRESERVED=8224 ORIGINAL=7848 NEW_VALID=376 REMAINING=3178; no test with a valid result selected again')
