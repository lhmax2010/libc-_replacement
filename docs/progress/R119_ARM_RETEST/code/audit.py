#!/usr/bin/env python3
from common import *
import re,collections
gate('audit_gate')
frozen=json.loads((OUT/'PRIOR_FROZEN_SHA256.json').read_text())
for p,d in frozen.items():assert sha(ROOT/p)==d,p
reuse=json.loads((OUT/'PROBE_REUSE.json').read_text());assert sha(reuse['path'])==reuse['sha256']
for arch,folder,table in [('armv7l','measurements','ARMV7L_TYPES.tsv'),('aarch64','aarch64_measurements','AARCH64_TYPES.tsv')]:
 rows=list(csv.DictReader((OUT/table).open(),delimiter='\t'));assert len(rows)==13
 for r in rows:
  a,b=[json.loads((OUT/folder/f'{r["id"]}_{lib}.json').read_text()) for lib in ['gnu','cxx']]
  for m in [a,b]:
   assert len(m['runs'])==5 and sha(ROOT/m['source'])==m['source_sha256']
   for ref in m['runs']:assert int((ROOT/(ref+'.exitcode')).read_text())==0 and m['output'] in (ROOT/(ref+'.stdout')).read_text()
   assert m['size']==8 and m['align']==8
  if arch=='armv7l':assert a['token']==b['token']=='9TypeTokenIxE' and r['result']=='IDENTICAL'
  else:assert a['token']=='9TypeTokenIlE' and b['token']=='9TypeTokenIxE' and r['result']=='DIVERGENT'
signatures=json.loads((OUT/'SIGNATURES.json').read_text());assert len(signatures)==4
for s in signatures:
 assert sha(ROOT/s['source'])==s['source_sha256'] and len(s['repeats'])==5
 for r in s['repeats']:assert int((ROOT/(r['build']+'.exitcode')).read_text())==0 and s['symbol'] in (ROOT/(r['readelf']+'.stdout')).read_text()
for kind in ['dali','zypp']:
 a,b=[next(s['symbol'] for s in signatures if s['interface']==kind and s['library']==lib) for lib in ['gnu','cxx']]
 assert (a==b)==(kind=='zypp')
actual=json.loads((OUT/'ACTUAL_ARM_EDGES.json').read_text());assert len(actual)==2
headers=json.loads((OUT/'EDGE_HEADER_SHA256.json').read_text())
for p,d in headers.items():assert sha(p)==d,p
# 与 RPM 原包绑定，不只信任解包目录名。
base=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11')
materials=json.loads((OUT/'TARGET_PACKAGE_MATERIALS.json').read_text());bindings=[]
for m in materials:
 if m['arch']!='armv7l':continue
 files={p:d for p,d in headers.items() if p.startswith(m['extracted']+'/')}
 for e in actual:
  for side in ['provider','consumer']:
   v=e[side]
   if v['package']['rpm_sha256']==m['rpm_sha256']:files[v['path']]=v['sha256']
 if not files:continue
 rpm=base/'rpms'/m['repo']/m['location'];assert rpm.is_file(),rpm;assert sha(rpm)==m['rpm_sha256']
 rc,out,ref=record('rpm_dump_'+m['name'],['rpm','-qp','--dump',rpm]);assert rc==0
 dump={}
 for line in out.splitlines():
  parts=line.split()
  if len(parts)>4 and parts[0].startswith('/'):dump[parts[0]]=parts[3]
 for p,d in files.items():
  relative=p[len(m['extracted']):];assert relative in dump and dump[relative]==d,(relative,dump.get(relative),d)
 bindings.append(dict(package=m['name'],arch='armv7l',rpm=str(rpm),rpm_sha256=m['rpm_sha256'],record=ref,verified_files=files))
save(OUT/'RPM_BINDINGS.json',bindings)
for e in actual:
 for side in ['provider','consumer']:assert sha(e[side]['path'])==e[side]['sha256']
 q=e['provider']['record'];text=(ROOT/(q+'.stdout')).read_text()
 cxx=next(s['symbol'] for s in signatures if s['interface']==e['interface'] and s['library']=='cxx')
 if e['interface']=='dali':assert cxx not in text and e['symbol'] in text
start=json.loads((OUT/'raw/001_gate.time.json').read_text())['start'];now=datetime.datetime.now(datetime.timezone.utc);elapsed=(now-datetime.datetime.fromisoformat(start)).total_seconds();assert elapsed<10800
save(OUT/'SCRIPT_SHA256.json',{str(p.relative_to(OUT)):sha(p) for folder in ['code','probes'] for p in (OUT/folder).rglob('*') if p.is_file() and '__pycache__' not in p.parts})
save(OUT/'AUDIT.json',dict(at=now.isoformat(),elapsed_seconds=elapsed,prior_files_verified=len(frozen),prior_unchanged=True,probe_reused_unchanged=True,armv7l=dict(expressions=13,library_cells=26,runs=10,result='ALL_IDENTICAL',environment='QEMU_USER'),aarch64=dict(expressions=13,library_cells=26,runs=10,result='ALL_DIVERGENT',environment='QEMU_USER'),signature_compile_cells=4,signature_success_repeats=20,actual_arm_edges=2,rpm_bindings=len(bindings),rpm_bound_file_count=sum(len(x['verified_files']) for x in bindings),other_572_armv7l='NOT_OBSERVED',board_used=False,parallelism=1,rlimit_as_bytes=9921875558))
print((OUT/'AUDIT.json').read_text())
