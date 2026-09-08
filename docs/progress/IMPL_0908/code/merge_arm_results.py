#!/usr/bin/env python3
"""派生汇总：明确选择首轮有效结果及续跑结果，原始中断结果不改动。"""
import csv,hashlib,json,pathlib
r=pathlib.Path('docs/progress/IMPL_0908/W3');sources=[];selected={};provenance=[];discarded=[];per_run=[];no_result=[]
for label in ['full','resume1','resume2','resume3']:
 p=r/('raw/lit_armv7l_new_'+label+'/result.json')
 if not p.parent.exists():continue
 assert (p.parent/'exitcode').exists(),'run still active'
 if not p.exists():
  timing=p.parent/'time.json';reason=json.loads(timing.read_text())['reason']
  assert reason in ['CONNECTION_FAILURE','TASK_TIME_LIMIT','RESOURCE_STOP']
  no_result.append({'run':label,'reason':reason,'time_record':str(timing),'sha256':hashlib.sha256(timing.read_bytes()).hexdigest()});continue
 sources.append(p);data=json.loads(p.read_text());valid=0;infra=0
 for e in data['tests']:
  if 'EXECUTOR_ERROR: remote exit marker absent' in e.get('output','') or 'CONNECTION_FAILURE already recorded; refusing new board work' in e.get('output',''):
   discarded.append({'test':e['name'],'source':str(p)});infra+=1;continue
  assert e['name'] not in selected,('unexpected valid overlap',e['name'])
  selected[e['name']]=e;valid+=1
  reason='RERUN_AFTER_NO_VALID_EXECUTION' if any(x['test']==e['name'] for x in discarded) else 'FIRST_VALID_RESULT'
  provenance.append([e['name'],e['code'],str(p),reason])
 per_run.append({'run':label,'valid_records':valid,'infrastructure_records':infra})
assert len(selected)<=11402
with (r/'ARM_RESULT_SELECTION.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['test','selected_code','original_source','selection_reason']);w.writerows(provenance)
data={'kind':'DERIVED_UNION_NOT_SINGLE_INVOCATION','complete':len(selected)==11402,'sources':[{'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in sources],'no_result_runs':no_result,'per_run':per_run,'original_infrastructure_failures_retained_in_source':discarded,'tests':[selected[k] for k in sorted(selected)]}
(r/'ARM_COMBINED_RESULT.json').write_text(json.dumps(data,indent=2)+'\n')
def canon(name):
 name=name.split(' :: ',1)[-1]
 for root in ['/libcxx/test/','/libcxxabi/test/']:
  if root in name:return name.split(root,1)[1]
 return name
def key(name):return ('libcxxabi' if 'libc++abi' in name.split(' :: ',1)[0] else 'libcxx',canon(name))
present={key(name) for name in selected};invalid={key(x['test']) for x in discarded}-present
history=[row for row in csv.DictReader((r/'historical_baseline.tsv').open(),delimiter='\t') if row['arch']=='armv7l']
missing=[row for row in history if (row['suite'],canon(row['test'])) not in present]
assert len(missing)+len(present)==11402
with (r/'ARM_UNOBSERVED.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['suite','test','state','reason'])
 for row in missing:w.writerow([row['suite'],canon(row['test']),'NOT_OBSERVED','NO_VALID_BOARD_EXECUTION' if (row['suite'],canon(row['test'])) in invalid else 'NOT_RUN_BEFORE_CONNECTION_STOP'])
print('ARM_DERIVED_UNION unique_valid_tests='+str(len(selected))+' complete='+str(data['complete']),per_run)
print('UNOBSERVED',len(missing),'INFRASTRUCTURE_PENDING',len(invalid))
print('INFRASTRUCTURE_RERUN_RESULTS',[(x['test'],selected.get(x['test'],{}).get('code','NOT_OBSERVED')) for x in discarded])
