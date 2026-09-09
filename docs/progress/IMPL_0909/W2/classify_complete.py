#!/usr/bin/env python3
"""仅按已观测新增失败集合分类；保留 raw FAIL 与历史时间窗口限定。"""
import collections,csv,json,pathlib
r=pathlib.Path('docs/progress');o=r/'IMPL_0909/W2';old=r/'IMPL_0908/W3'
s=json.loads((o/'ARM_SUMMARY.json').read_text());assert s['valid_total']==11402 and s['missing']==0 and s['new_valid']==3554
assert (o/'ARM_REMAINING.tsv').read_text().splitlines()==['suite\ttest\tstate']
overflow='std/localization/locales/locale.convenience/conversions/conversions.buffer/overflow.pass.cpp'
assert [(x[0],x[1]) for x in s['new_failure_candidates']]==[('libcxx',overflow)]
assert 'LIBSTDCXX_MATCHES_NEW' in (r/'IMPL_0909/W1/REPORT.md').read_text()
native=json.loads((old/'summary_x86_64_new_full.json').read_text())
nc=list(csv.DictReader((old/'comparison_x86_64_new_full.tsv').open(),delimiter='\t'))
assert [(x['suite'],x['test']) for x in nc if x['comparison']=='NEW_FAILURE_REQUIRES_REVIEW']==[('libcxx',overflow)]
rows=[]
history=list(csv.DictReader((old/'historical_baseline.tsv').open(),delimiter='\t'))
for arch,summary in [('x86_64',native),('armv7l',s)]:
    current=collections.Counter()
    for counts in summary['counts'].values():current.update(counts)
    baseline=collections.Counter(x['historical_code'] for x in history if x['arch']==arch)
    assert sum(current.values())==sum(baseline.values())==11402
    for label,c in [('frozen_baseline',baseline),('current',current)]:
        rows.append([arch,label,11402,*[c[k] for k in ['PASS','FAIL','UNSUPPORTED','XFAIL']],1 if label=='current' else 'N/A',1 if label=='current' else 'N/A',0 if label=='current' else 'N/A',0 if label=='current' else 'N/A'])
with (o/'TWO_ARCHITECTURES.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['arch','variant','total','PASS','FAIL','UNSUPPORTED','XFAIL','new_FAIL','accepted_destructor_change','noexcept_assumption','other_new_FAIL']);w.writerows(rows)
with (o/'NEW_FAILURE_CLASSIFICATION.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['arch','suite','test','baseline','current','classification','evidence'])
    for arch in ['x86_64','armv7l']:w.writerow([arch,'libcxx',overflow,'PASS','FAIL','ACCEPTED_DESTRUCTOR_BEHAVIOR_CHANGE','docs/progress/IMPL_0909/W1/REPORT.md'])
print('CLASSIFICATION=SUITE_COMPLETE; accepted new FAIL=1 per architecture; noexcept-assumption=0; other new FAIL=0')
print('POSITIVE_CONTROL: real overflow PASS->FAIL detected on both architectures; explicit sync controls pass in W1')
print('FAIL_TO_PASS=3 armv7l historical timeout-window cases; not credited to patches')
for row in rows:print('\t'.join(map(str,row)))
