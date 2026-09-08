#!/usr/bin/env python3
"""保留历次有效结果，仅重跑传输无结果项及未执行项；最多三次。"""
import csv,json,pathlib,re,sys
r=pathlib.Path('docs/progress/IMPL_0908/W3');retry=int(sys.argv[1]) if len(sys.argv)>1 else 1
assert 1<=retry<=3
labels=['full']+['resume'+str(i) for i in range(1,retry)];good={};all_names=set();bad=[];sources=[]
for label in labels:
 p=r/('raw/lit_armv7l_new_'+label+'/result.json');assert (p.parent/'exitcode').exists()
 sources.append(str(p));data=json.loads(p.read_text())
 for e in data['tests']:
  all_names.add(e['name']);output=e.get('output','')
  if 'EXECUTOR_ERROR: remote exit marker absent' in output or 'CONNECTION_FAILURE already recorded; refusing new board work' in output:
   assert e['code']=='FAIL';bad.append((label,e));continue
  assert e['name'] not in good,('unexpected valid overlap',e['name']);good[e['name']]=e
pending={e['name'] for _,e in bad}-set(good)
assert pending and good
out=r/f'arm_resume{retry}.args';pattern='^(?:'+'|'.join(re.escape(name) for name in good)+')$';rx=re.compile(pattern)
assert all(rx.search(name) for name in good) and not any(rx.search(name) for name in pending)
out.write_text('--filter-out\n'+pattern+'\n')
classification=r/('ARM_INTERRUPTION_CLASSIFICATION.tsv' if retry==1 else f'ARM_INTERRUPTION{retry}_CLASSIFICATION.tsv')
with classification.open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['run','test','original_code','classification','reason'])
 for label,e in bad:w.writerow([label,e['name'],e['code'],'NO_VALID_BOARD_EXECUTION','missing exit marker before launch' if 'EXECUTOR_ERROR:' in e['output'] else 'refused after connection flag'])
plan={'retry_number':retry,'preserve_valid_records':len(good),'outstanding_infrastructure_records_to_rerun':len(pending),'historical_infrastructure_records_preserved':len(bad),'previously_not_observed':11402-len(all_names),'expected_resume_tests':11402-len(good),'args_file':str(out),'source_results':sources}
dest=r/('ARM_RESUME_PLAN.json' if retry==1 else f'ARM_RESUME{retry}_PLAN.json');assert not dest.exists()
dest.write_text(json.dumps(plan,indent=2)+'\n');print(json.dumps(plan,indent=2))
