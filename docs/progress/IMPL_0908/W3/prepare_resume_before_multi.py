#!/usr/bin/env python3
"""按原始逐项输出保留有效结果，只重跑传输无结果项及未执行项。"""
import collections,csv,json,pathlib,re
r=pathlib.Path('docs/progress/IMPL_0908/W3');p=r/'raw/lit_armv7l_new_full/result.json'
data=json.loads(p.read_text());bad=[];good=[]
for entry in data['tests']:
 output=entry.get('output','')
 if 'EXECUTOR_ERROR: remote exit marker absent' in output or 'CONNECTION_FAILURE already recorded; refusing new board work' in output:
  assert entry['code']=='FAIL';bad.append(entry)
 else:good.append(entry)
assert len(bad)==9 and len(good)==6306
out=r/'arm_resume1.args';pattern='^(?:'+'|'.join(re.escape(e['name']) for e in good)+')$'
rx=re.compile(pattern);assert all(rx.search(e['name']) for e in good) and not any(rx.search(e['name']) for e in bad)
out.write_text('--filter-out\n'+pattern+'\n')
with (r/'ARM_INTERRUPTION_CLASSIFICATION.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['test','original_code','classification','reason'])
 for e in bad:w.writerow([e['name'],e['code'],'NO_VALID_BOARD_EXECUTION','missing exit marker before launch' if 'EXECUTOR_ERROR:' in e['output'] else 'refused after connection flag'])
plan={'retry_number':1,'original_records':len(data['tests']),'preserve_valid_records':len(good),'infrastructure_records_to_rerun':len(bad),'previously_not_observed':11402-len(data['tests']),'expected_resume_tests':11402-len(good),'args_file':str(out),'original_result':str(p)}
(r/'ARM_RESUME_PLAN.json').write_text(json.dumps(plan,indent=2)+'\n');print(json.dumps(plan,indent=2))
