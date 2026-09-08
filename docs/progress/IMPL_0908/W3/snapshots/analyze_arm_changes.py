#!/usr/bin/env python3
"""核查全部状态变化，包括失败转通过；不把时间环境差异归功于实施改动。"""
import csv,json,pathlib,re
r=pathlib.Path('docs/progress/IMPL_0908/W3');rows=list(csv.DictReader((r/'comparison_armv7l_new_combined.tsv').open(),delimiter='\t'))
changes=[x for x in rows if x['comparison']!='SAME_AS_HISTORICAL'];new=[x for x in changes if x['comparison']=='NEW_FAILURE_REQUIRES_REVIEW'];improved=[x for x in changes if x['historical_code']=='FAIL' and x['current_code']=='PASS']
assert len(rows)==7848 and len(changes)==4 and len(new)==1 and len(improved)==3
oldpath=pathlib.Path('docs/progress/R81/raw/phase2/results/baseline_armv7l/libcxx/aggregate_result_owner_normalized.json')
old=json.loads(oldpath.read_text())['tests'];current=json.loads((r/'ARM_COMBINED_RESULT.json').read_text())['tests']
dest=r/'changed_to_pass_details';dest.mkdir(exist_ok=True)
for index,row in enumerate(improved,1):
 a=[e for e in old if e['name'].endswith(row['test'])];b=[e for e in current if e['name'].endswith(row['test'])]
 assert len(a)==len(b)==1
 (dest/f'{index}.txt').write_text('TEST='+row['test']+'\nHISTORICAL_SOURCE='+str(oldpath)+'\nHISTORICAL\n'+json.dumps(a[0],indent=2)+'\nCURRENT\n'+json.dumps(b[0],indent=2)+'\n')
 print('TEST',row['test'])
 for label,e in [('historical',a[0]),('current',b[0])]:
  print(label,'code',e['code'],'elapsed',e.get('elapsed','NOT_AVAILABLE'))
  for line in e.get('output','').splitlines():
   if re.search(r'Exit Code|TIMEOUT|timed out|timeout=|timeout [0-9]|exit status: 124',line,re.I):print(label,line)
pending=list(csv.DictReader((r/'ARM_UNOBSERVED.tsv').open(),delimiter='\t'))
assert len(pending)==3554 and sum(x['reason']=='NO_VALID_BOARD_EXECUTION' for x in pending)==39
print('ALL_CHANGES_CLASSIFIED=PASS new_failure=1 changed_to_pass=3 unobserved=3554 infrastructure_pending=39')
