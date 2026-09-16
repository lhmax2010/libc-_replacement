from common import *
import re
results={}
snap=json.loads((OUT/'SNAPSHOT_SHA256.json').read_text())
for rel,digest in snap.items():assert hashlib.sha256((OUT/rel).read_bytes()).hexdigest()==digest,rel
results['snapshots']='PASS'
rows=json.loads((OUT/'W1/RESULTS.json').read_text());assert len(rows)==25
for row in rows:
 stdout=ROOT/(row['raw']+'.stdout');s=stdout.read_text()
 assert hashlib.sha256(stdout.read_bytes()).hexdigest()==row['stdout_sha256']
 assert row['exit']==(-6 if row['kind']==7 else 0)
 assert ('MAP_ASSERT gnu=1 cxx='+str(int(row['kind']==0))) in s
 if row['kind'] in [6,8,9]:assert 'created=1 destroyed=1' in s
results['gnu_four_cells_plus_map_positive']='20 + 5 processes PASS'
values=json.loads((OUT/'W2/ASSERTED_ORIGINAL_RESULTS.json').read_text());assert len(values)==15
assert sum(v['bytes']==834 for v in values)==5 and sum(v['bytes']==8340 for v in values)==5 and sum(v['bytes']==0 for v in values)==5
for row in json.loads((OUT/'W2/NORMAL_ORIGINAL_RESULTS.json').read_text()):
 p=ROOT/(row['raw']+'.stdout');assert hashlib.sha256(p.read_bytes()).hexdigest()==row['stdout_sha256'];assert 'SUMMARY EXACT=12/12 DESTROYED=12/12' in p.read_text()
results['original_leak_reproduction']='15 processes PASS';results['original_normal']='12 scenarios x 5 PASS'
identity=json.loads((OUT/'W2/PATCH_IDENTITY.json').read_text())
assert hashlib.sha256((OUT/'W2/provider-exception-cleanup.CANDIDATE.patch').read_bytes()).hexdigest()==identity['patch_sha256']
for item in identity['files']:
 for mode,key in [('baseline','before_sha256'),('candidate','after_sha256')]:assert hashlib.sha256((OUT/'W2'/mode/item['path']).read_bytes()).hexdigest()==item[key]
assert identity['status']=='DRAFT_NOT_BUILT_NOT_RUNTIME_VALIDATED'
results['candidate']='APPLY_CHECK_ONLY; build and runtime NOT_OBSERVED'
for item in json.loads((OUT/'W3/DOCUMENT_CHANGES.json').read_text()):assert hashlib.sha256((ROOT/item['path']).read_bytes()).hexdigest()==item['after_sha256']
results['document_identities']='PASS'
for p in OUT.rglob('*'):
 if not p.is_file():continue
 assert p.stat().st_size<100*1024*1024,p
 with p.open('rb') as f:assert f.read(4)!=b'\x7fELF',p
 if p.name.endswith('.command.txt'):
  stem=str(p)[:-len('.command.txt')]
  if pathlib.Path(stem).name=='005_audit':continue # current outer receipt closes after this program
  for suffix in ['.stdout','.stderr','.exitcode','.time.json']:assert pathlib.Path(stem+suffix).exists(),stem+suffix
results['raw_receipts_and_no_elf']='PASS'
for file,digest in [('033_angle_files.stdout','dde4c0b37000f6e3d01bd5096bba7ee331a31b216bd43dcbee24f7f2a9a9cf7c'),('033_angle_files.stderr','94f75a90010d49a676335c1caca88ff04cb1b0a1294bdbb655f38d865eb7a18c')]:assert hashlib.sha256((ROOT/'docs/progress/R115/W3/raw'/file).read_bytes()).hexdigest()==digest
results['unrelated_user_edits']='UNCHANGED'
results['overall']='PARTIAL: candidate provider build/verification unavailable'
(OUT/'SELF_CHECK.json').write_text(json.dumps(results,indent=2)+'\n')
print(json.dumps(results))
