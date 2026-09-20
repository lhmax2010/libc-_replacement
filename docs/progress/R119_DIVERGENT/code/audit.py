#!/usr/bin/env python3
from common import *
import collections,re,sqlite3
gate('audit_gate')
db=ROOT/'tmp/R119_batch1/full_elf/symbols.sqlite3';before=db.stat();db_sha=sha(db)
con=sqlite3.connect('file:'+str(db)+'?mode=ro',uri=True);elf_count=con.execute('select count(*) from elf').fetchone()[0];con.close()
after=db.stat();assert (before.st_size,before.st_mtime_ns)==(after.st_size,after.st_mtime_ns)
save(OUT/'ELF_INDEX_IDENTITY.json',dict(path=str(db),sha256=db_sha,size=after.st_size,mtime_ns=after.st_mtime_ns,elf_rows=elf_count,note='索引用于发现；最终阳性另由 RPM/SHA 绑定的真实 ELF 原始条目复核'))
frozen=json.loads((OUT/'PROJECTION_FROZEN_SHA256.json').read_text())
for p,digest in frozen.items():assert sha(ROOT/p)==digest,p
rows=json.loads((OUT/'TYPE_RESULTS.json').read_text());assert len(rows)==585
counts=collections.Counter(r['result'] for r in rows);assert counts==dict(IDENTICAL=283,DIVERGENT=242,NOT_AVAILABLE=60)
measurements=[];runrecords=set();sources=set();allruns=0
for p in sorted((OUT/'measurements').glob('*.json')):
 m=json.loads(p.read_text());assert m['probe_version']==2,p
 if 'source' in m:
  assert sha(ROOT/m['source'])==m['source_sha256'];sources.add(m['source'])
 if 'runs' in m:
  assert len(m['runs'])==5
  for run in m['runs']:
   assert int((ROOT/(run+'.exitcode')).read_text())==0
   assert m['output'] in (ROOT/(run+'.stdout')).read_text()
   runrecords.add(run);allruns+=1
 if m['status']=='MEASURED':assert m['wrapper_encoding'] and m['type_encoding']
 measurements.append(m)
assert len(measurements)==1170
for r in rows:
 if r['result']=='IDENTICAL':assert r['gnu_wrapper']==r['cxx_wrapper']
 if r['result']=='DIVERGENT':assert r['gnu_wrapper']!=r['cxx_wrapper']
control=json.loads((OUT/'LINK_CONTROL.json').read_text())['results'];assert len(control)==4
for c in control:
 if c['consumer']==c['provider']:assert len(c['runs'])==5 and c['observed']=='VALUE_20_IN_5_OF_5'
 else:assert len(c['link_attempts'])==5 and all(int((ROOT/(r+'.exitcode')).read_text())==1 for r in c['link_attempts'])
assert json.loads((OUT/'BOUNDARY_POSITIVE_CONTROL.json').read_text())['passed']
proofs=json.loads((OUT/'CONFIRMED_EDGE_PROOFS.json').read_text())
for p in proofs:
 for side in ['provider','consumer']:assert sha(p[side]['path'])==p[side]['sha256']
for chain in json.loads((OUT/'EDGE_STRUCTURAL_HEADERS.json').read_text()).values():
 for h in chain:assert sha(h['path'])==h['sha256']
exposure=list(csv.DictReader((OUT/'DIVERGENT_EXPOSURE.tsv').open(),delimiter='\t'));assert len(exposure)==242
excounts=collections.Counter(r['result'] for r in exposure)
summary=json.loads((OUT/'EXPOSURE_SUMMARY.json').read_text());assert summary['status_counts']==excounts
changed=list(csv.DictReader((OUT/'RECLASSIFIED_CHANGES.tsv').open(),delimiter='\t'));assert len(changed)==10
start=json.loads((OUT/'raw/001_gate.time.json').read_text())['start'];now=datetime.datetime.now(datetime.timezone.utc);elapsed=(now-datetime.datetime.fromisoformat(start)).total_seconds()
assert elapsed<8*3600
save(OUT/'AUDIT.json',dict(at=now.isoformat(),elapsed_seconds=elapsed,old_projection_files_sha_verified=len(frozen),old_projection_unchanged=True,type_counts=dict(counts),measurement_library_cells=len(measurements),measurement_status_counts=dict(collections.Counter(m['status'] for m in measurements)),unique_probe_sources_sha_verified=len(sources),five_repeat_cell_observations=allruns,unique_success_run_records=len(runrecords),projection_changed_positions=len(changed),exposure_status_counts=dict(excounts),two_library_controls='PASS',bundle_add_control='PASS',confirmed_edge_source_hashes='PASS',scope='x86_64 / C++17 / S_R119_20260918 + S_A finite extension',armv7l='NOT_OBSERVED',parallelism=1,rlimit_as_bytes=9921875558))
md=['# 两套库类型身份不同的实测清单','','仅及本次 x86_64 / C++17 配置；不是 242 个新的标量选择问题。完整身份比较使用 wrapper；DIRECT 编码不保留顶层 cv/ref。','', '| ID | 表达式 | GNU 展开 / DIRECT | libc++ 展开 / DIRECT | sizeof | 分类 | 测量记录 |','| --- | --- | --- | --- | --- | --- | --- |']
for r in rows:
 if r['result']!='DIVERGENT':continue
 md.append(f'| {r["id"]} | `{r["expression"]}` | `{r["gnu_canonical"]}` / `{r["gnu_encoding"]}` | `{r["cxx_canonical"]}` / `{r["cxx_encoding"]}` | {r["gnu_size"]} / {r["cxx_size"]} | {r["mechanism"]} | [GNU]({r["gnu_evidence"]}) / [libc++]({r["cxx_evidence"]}) |')
(OUT/'DIVERGENT_TYPES.md').write_text('\n'.join(md)+'\n')
scalar=[r for r in rows if r.get('mechanism')=='内建底层类型选择不同'];assert len(scalar)==13
tsv(OUT/'BUILTIN_DIVERGENT.tsv',scalar)
print((OUT/'AUDIT.json').read_text())
