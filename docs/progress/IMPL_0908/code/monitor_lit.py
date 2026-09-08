#!/usr/bin/env python3
"""只读进度与历史差异候选；运行中结果不替代最终 JSON。"""
import collections,csv,json,pathlib,re
r=pathlib.Path('docs/progress/IMPL_0908/W3')
def canon(s):
 s=s.split(' :: ',1)[-1]
 for p in ['/libcxx/test/','/libcxxabi/test/']:
  if p in s:return s.split(p,1)[1]
 return s
old={}
for row in csv.DictReader((r/'historical_baseline.tsv').open(),delimiter='\t'):old[(row['arch'],row['suite'],canon(row['test']))]=row['historical_code']
for arch in ['x86_64','armv7l']:
 p=r/'raw'/('lit_'+arch+'_new_full');status=[];candidates=[]
 for code,name,position,total in re.findall(r'^(PASS|FAIL|XFAIL|XPASS|UNSUPPORTED|TIMEOUT|UNRESOLVED): (.+) \((\d+) of (\d+)\)$',(p/'stdout').read_text(errors='replace'),re.M):
  status.append(code);suite='libcxxabi' if 'libc++abi' in name.split(' :: ',1)[0] else 'libcxx'
  prior=old.get((arch,suite,canon(name)),'NOT_AVAILABLE')
  if code in ['FAIL','XPASS','TIMEOUT','UNRESOLVED'] and prior!=code:candidates.append([canon(name),prior,code])
 print(json.dumps({'arch':arch,'completed_records':len(status),'counts':dict(collections.Counter(status)),'new_failure_candidates':candidates,'json_ready':(p/'result.json').exists(),'connection_failure':(p/'executor/CONNECTION_FAILURE').exists()},ensure_ascii=False))
