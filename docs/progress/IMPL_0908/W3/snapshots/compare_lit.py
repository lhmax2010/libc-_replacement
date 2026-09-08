#!/usr/bin/env python3
"""逐项比较当前/历史状态；不把新增失败自动豁免成通过。"""
import collections,csv,json,os,pathlib,sys
r=pathlib.Path('docs/progress/IMPL_0908/W3');arch,variant,label=sys.argv[1:4]
def canon(name):
 name=name.split(' :: ',1)[-1]
 for root in ['/libcxx/test/','/libcxxabi/test/']:
  if root in name:return name.split(root,1)[1]
 return name
historical={}
for x in csv.DictReader((r/'historical_baseline.tsv').open(),delimiter='\t'):
 if x['arch']==arch:historical[(x['suite'],canon(x['test']))]=x['historical_code']
raw=r/'raw'/('lit_'+arch+'_'+variant+'_'+label)
input_path=pathlib.Path(os.environ['IMPL_LIT_RESULT_FILE']) if os.environ.get('IMPL_LIT_RESULT_FILE') else raw/'result.json'
data=json.loads(input_path.read_text());rows=[];seen=set();counts=collections.defaultdict(collections.Counter)
details=r/'failure_details'/('_'.join([arch,variant,label]));details.mkdir(parents=True,exist_ok=True)
for entry in data['tests']:
 suite='libcxxabi' if 'libc++abi' in entry['name'].split(' :: ',1)[0] else 'libcxx'
 name=canon(entry['name']);key=(suite,name);assert key not in seen,key;seen.add(key)
 code=entry['code'];old=historical.get(key,'NOT_AVAILABLE');counts[suite][code]+=1
 if code==old:kind='SAME_AS_HISTORICAL'
 elif code in ('FAIL','TIMEOUT','UNRESOLVED','XPASS'):kind='NEW_FAILURE_REQUIRES_REVIEW'
 else:kind='CHANGED_STATUS'
 evidence=''
 if code in ('FAIL','TIMEOUT','UNRESOLVED','XPASS'):
  import hashlib
  p=details/(hashlib.sha256((suite+'/'+name).encode()).hexdigest()[:16]+'.txt')
  p.write_text('TEST='+suite+'/'+name+'\nHISTORICAL='+old+'\nCURRENT='+code+'\n'+entry.get('output',''))
  evidence=str(p)
 rows.append([suite,name,old,code,kind,evidence])
table=r/('_'.join(['comparison',arch,variant,label])+'.tsv')
with table.open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['suite','test','historical_code','current_code','comparison','details']);w.writerows(rows)
summary={'arch':arch,'variant':variant,'label':label,'input_path':str(input_path),'counts':dict(counts),'tests_recorded':len(rows),
 'historical_tests':len(historical),'historical_not_in_result':len(set(historical)-seen),
 'result_not_in_historical':len(seen-set(historical)),
 'new_failure_candidates':[x[0]+'/'+x[1] for x in rows if x[4]=='NEW_FAILURE_REQUIRES_REVIEW']}
(r/('_'.join(['summary',arch,variant,label])+'.json')).write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps(summary,indent=2))
