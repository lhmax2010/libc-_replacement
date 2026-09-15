#!/usr/bin/env python3
import datetime,hashlib,json,pathlib,re,subprocess
ROOT=pathlib.Path(__file__).resolve().parents[2];base=ROOT/'docs/progress/P13_0917'
checks={}
expected={'W1/PINNED_ALLOCATION.json':10,'W1/W1_ERRORS.json':120,'W2/EWK_RESULTS.json':20,'W2/BOUNDARY_RESULTS.json':15,'W2/EXCEPTIONS.json':80,'W2/ALLOCATIONS.json':180,'W2/UNIT_RESULTS.json':30,'W2/VECTOR_RESULTS.json':25,'W3/full_CONTRACT_COMPARISON.json':105,'W3/repeated_CONTRACT_COMPARISON.json':15,'W3/RELEASE_RESULTS.json':20}
for name,count in expected.items():
 rows=json.loads((base/name).read_text());assert len(rows)==count,(name,len(rows));checks[name]=count
 for row in rows:
  if 'stdout_sha256' in row:
   for stream in ['stdout','stderr']:
    p=base/name.split('/')[0]/'raw'/(row['raw']+'.'+stream)
    assert hashlib.sha256(p.read_bytes()).hexdigest()==row[stream+'_sha256']
exceptions=json.loads((base/'W2/EXCEPTIONS.json').read_text());assert sum(r['exit']==-6 for r in exceptions)==10 and all(r['exit']==0 for r in exceptions if r['kind']!=7)
retention=json.loads((base/'W3/repeated_CONTRACT_COMPARISON.json').read_text())
for row in retention:
 line=next(s for s in row['value_lines']if s.startswith('REPEATED_FAILURES'))
 assert ('tracked_outstanding=30 tracked_bytes=8340' if row['mode']=='original' else 'tracked_outstanding=20 tracked_bytes=7700') in line
for stage in ['W1','W2','W3']:
 for p,h in json.loads((base/stage/'SNAPSHOT_SHA256.json').read_text()).items():assert hashlib.sha256((base/stage/p).read_bytes()).hexdigest()==h,p
files=list(base.rglob('*'));assert not [p for p in files if p.is_file() and p.stat().st_size>=100*1024**2]
for p in files:
 if p.is_file():
  with p.open('rb') as f:assert f.read(4)!=b'\x7fELF',p
  assert p.suffix not in ['.o','.rpm','.pyc'],p
for p in base.rglob('*.command.txt'):
 stem=str(p)[:-12]
 if 'p13_audit.py' in p.read_text() and not pathlib.Path(stem+'.exitcode').exists():continue
 for suffix in ['.stdout','.stderr','.exitcode','.time.json']:assert pathlib.Path(stem+suffix).exists(),stem+suffix
timings={}
for stage,receipt in [('W1','004_w1_commit'),('W2','007_w2_commit'),('W3',None)]:
 start=json.loads((base/stage/'raw/001_gate.time.json').read_text())['start']
 end=json.loads((ROOT/'tmp/P12/P13_delivery'/(receipt+'.time.json')).read_text())['end'] if receipt else datetime.datetime.now(datetime.timezone.utc).isoformat()
 duration=(datetime.datetime.fromisoformat(end)-datetime.datetime.fromisoformat(start)).total_seconds()
 assert duration<({'W1':3,'W2':4,'W3':3}[stage])*3600
 timings[stage]={'start':start,'end':end,'seconds':duration,'end_kind':'commit' if receipt else 'audit_snapshot'}
(base/'TIMINGS.json').write_text(json.dumps(timings,indent=2)+'\n')
assert sum(t['seconds']for t in timings.values())<10*3600
checks.update(hashes='PASS',raw_records='PASS',scope_no_binaries='PASS',limits='PASS',normal_regression='PASS',original_abort='FIXED_IN_MEASURED_CONFIGURATION',double_destructor_escape='NOT_SATISFIED',provider_full_cleanup='NOT_SATISFIED',release='BLOCKED')
(base/'SELF_CHECK.json').write_text(json.dumps(checks,indent=2)+'\n')
print(json.dumps(checks,indent=2))
