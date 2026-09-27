"""交付前证据一致性检查；不运行目标、不修改历史结果。"""
from common import *
import re,csv
gate()
selected=json.loads((OUT/'SELECTED_RUNS.json').read_text())
counts=json.loads((OUT/'COUNTS.json').read_text())
assert len(selected)==23 and counts['baseline_rounds']==80
checked=0;binding_checks=[]
for r in selected:
 if r.get('fixture'):
  assert sha(OUT/r['fixture'])==r['fixture_sha256']
  assert sha(OUT/'code/run_fixtures.py')==r['driver_sha256']
  assert sha(OUT/'code/common.py')==r['common_sha256']
  assert r['support_sha256']=={p.name:sha(p) for p in (OUT/'fixtures').glob('*.h')}
  assert sha(pathlib.Path(r['provider']))==r['provider_sha256']
 for x in r['rounds']:
  for key in ('und_record','record'):
   if key not in x:continue
   p=ROOT/x[key]
   for suffix in ('.command.txt','.stdout','.stderr','.exitcode','.time.json'):assert p.with_suffix(suffix).is_file()
  assert x['und_exact']
  nm=(ROOT/(x['und_record']+'.stdout')).read_text()
  assert any(s.split() and s.split()[-1].split('@')[0]==r['symbol'] for s in nm.splitlines())
  assert int((ROOT/(x['record']+'.exitcode')).read_text())==x['exit']
  for line in x['target_bindings']:
   target=re.search(r'\] to (.*?) \[',line).group(1)
   assert pathlib.Path(target).resolve()==pathlib.Path(r['provider']).resolve(),(r['edge'],target)
   binding_checks.append(dict(edge=r['edge'],round=x['round'],target=target,provider_sha256=r['provider_sha256']))
  if x['valid']:
   out=(ROOT/(x['record']+'.stdout')).read_text()
   assert 'VALUES ' in out and 'LIFETIME ' in out and 'STDLIB GNU release=14' in out
   assert x['maps'][r['provider']]==r['provider_sha256']
   assert x['exit']==0 and x['target_bindings'] and not x['cxx_mapped']
   checked+=1
assert checked==80
# Verify exact provider target, not merely that some DSO has the symbol.
save('BINDING_AUDIT.json',binding_checks)
assert len(binding_checks)==85 # 80 valid + five Delta error-path calls
raw_incomplete=[]
for p in RAW.glob('*.command.txt'):
 base=str(p)[:-len('.command.txt')]
 # The current outer audit command is legitimately still open.
 if '_final_audit' in p.name or '_seal_manifest' in p.name:continue
 if not all(pathlib.Path(base+s).exists() for s in ('.stdout','.stderr','.exitcode','.time.json')):raw_incomplete.append(p.name)
assert not raw_incomplete,raw_incomplete
intervals=[]
for p in RAW.glob('*.time.json'):
 if re.search(r'_(?:build_edge\d+|run_\d+_\d+)\.time\.json$',p.name):
  t=json.loads(p.read_text());start=datetime.datetime.fromisoformat(t['start_utc']).timestamp()
  assert t['parallelism']==1 and t['rlimit_as']==MEM*30//100
  intervals.append((start,start+t['elapsed'],p.name))
intervals.sort()
assert all(a[1]<=b[0] for a,b in zip(intervals,intervals[1:])), 'overlapping fixture compile/run'
assert all(int(p.read_text())==0 for p in RAW.glob('*_resource_gate.exitcode'))
rpms=json.loads((OUT/'RPMS.json').read_text())
for number,r in enumerate(rpms,1):
 p=ROOT/r['rpm_local'];assert sha(p)==r['checksum']
 if number%10==0 or number==len(rpms):save('AUDIT_PROGRESS.json',dict(rpm_hashes_checked=number,total=len(rpms),status='IN_PROGRESS'))
assert len(rpms)==262
for n in ('EDGES.tsv','NEXT_STAGE.tsv'):
 assert sha(OUT/n)==sha(ROOT/'docs/progress/RUNTIME_PHASE_SUMMARY_0921'/n)
for h in json.loads((OUT/'DECLARATIONS.json').read_text()):
 assert sha(TMP/'root'/h['path'])==h['sha256']
# Provider-internal includes are forbidden; public integration/devel API remains public installed API.
includes={p.name:re.findall(r'^#include\s+[<"]([^>"]+)',p.read_text(),re.M) for p in (OUT/'fixtures').glob('edge*.cpp')}
assert all('/internal/' not in inc and not inc.startswith('src/') for incs in includes.values() for inc in incs)
attempt=OUT/'fixtures/attempts/edge08_incorrect_update_thread.cpp'
old_runs=[]
for p in (OUT/'runs').glob('*/results.json'):
 for r in json.loads(p.read_text()):
  if r['edge']==8 and r.get('fixture_sha256')==sha(attempt):old_runs.append(str(p.relative_to(OUT)))
assert old_runs,'incorrect-thread snapshot not tied to any actual run'
links=[];missing=[]
for p in [OUT/'FINAL_RESULT.md',OUT/'README.md',OUT/'DATA_FOLLOWUP.md',ROOT/'docs/LINE_STATUS.md',ROOT/'docs/LINE_STATUS_INDEX.md']:
 for target in re.findall(r'\]\(([^)]+)\)',p.read_text()):
  if '://' in target or target.startswith('#'):continue
  q=(p.parent/target.split('#')[0]).resolve();links.append(str(q))
  if not q.exists() and q not in {OUT/'DELIVERY.md',OUT/'AUDIT.json',OUT/'SHA256SUMS'}:missing.append(str(q))
assert not missing,missing
times=[json.loads(p.read_text()) for p in RAW.glob('*.time.json')]
start=min(t['start_utc'] for t in times)
save('AUDIT.json',dict(result='PASS',architecture='x86_64 native',edges=23,valid_rounds=checked,
 exact_binding_checks=len(binding_checks),rpm_hashes_checked=len(rpms),fixture_run_intervals=len(intervals),
 serial_intervals_nonoverlapping=True,resource_gate_failures=0,raw_incomplete=raw_incomplete,
 public_fixture_includes=includes,incorrect_thread_snapshot_matches=old_runs,
 local_document_links_checked=len(links),deferred_generated_links=['DELIVERY.md','AUDIT.json','SHA256SUMS'],
 task_first_record_utc=start,audit_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),
 script_sha256={str(p.relative_to(OUT)):sha(p) for p in sorted((OUT/'code').glob('*.py'))},
 fixture_sha256={str(p.relative_to(OUT)):sha(p) for p in sorted((OUT/'fixtures').rglob('*')) if p.is_file()}))
print('PASS: 23 edges, 80 valid rounds, 85 exact binding targets, 262 RPM hashes, no overlapping compile/run')
