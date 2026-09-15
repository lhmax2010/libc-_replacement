#!/usr/bin/env python3
import hashlib,json,pathlib,re
import build as b
checks={}
for name,count in [('v2_EWK_RESULTS.json',20),('RELEASE_RESULTS.json',20),('v2_BOUNDARY_RESULTS.json',15),('UNIT_RESULTS.json',30),('v2_VECTOR_RESULTS.json',25),('ALLOCATION_CONTRACT_RESULTS.json',5),('ALLOCATION_CONTROL_RESULTS.json',5)]:
 rows=json.loads((b.OUT/name).read_text());assert len(rows)==count,(name,len(rows));checks[name]=count
assert all(x['exit']==-6 for x in json.loads((b.OUT/'ALLOCATION_CONTRACT_RESULTS.json').read_text()))
assert all(x['exit']==0 and 'ACTUAL_RETURN=-12' in x['stdout'] for x in json.loads((b.OUT/'ALLOCATION_CONTROL_RESULTS.json').read_text()))
for f,h in json.loads((b.OUT/'SNAPSHOT_SHA256.json').read_text()).items():assert hashlib.sha256((b.OUT/f).read_bytes()).hexdigest()==h,f
incomplete=[]
for p in (b.OUT/'raw').glob('*.command.txt'):
 stem=str(p)[:-len('.command.txt')]
 # This audit's own recorder has not finished yet; check all earlier records.
 if 'audit.py' in p.read_text() and not pathlib.Path(stem+'.exitcode').exists():continue
 for suffix in ['.stdout','.stderr','.exitcode','.time.json']:
  if not pathlib.Path(stem+suffix).exists():incomplete.append(stem+suffix)
assert not incomplete,incomplete
broken=[]
for md in b.OUT.glob('*.md'):
 for link in re.findall(r'\]\(([^)]+)\)',md.read_text()):
  if '://' not in link and not (md.parent/link.split('#')[0]).exists():broken.append((md.name,link))
assert not broken,broken
elf=[]
for p in b.OUT.rglob('*'):
 if p.is_file():
  assert p.suffix not in ['.rpm','.o','.pyc'],p
  with p.open('rb') as f:
   if f.read(4)==b'\x7fELF':elf.append(str(p))
assert not elf,elf
checks.update(snapshot_hashes='PASS',raw_records='PASS',local_links='PASS',no_elf_in_delivery='PASS',overall='RELEASE_BLOCKED')
(b.OUT/'SELF_CHECK.json').write_text(json.dumps(checks,indent=2)+'\n')
print(json.dumps(checks,indent=2))
