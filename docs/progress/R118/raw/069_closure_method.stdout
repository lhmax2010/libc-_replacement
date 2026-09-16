#!/usr/bin/env python3
"""按冻结 DT_NEEDED 闭包使用真实平台库，避免误混主机同 SONAME 库。"""
import csv,pathlib,json,subprocess,sys,re,hashlib,os
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';t=r/'tmp/API_0911/W1_CONT';c=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11/extracted')
rows=[x for x in csv.DictReader(open('/home/toolchain/development/libc++_replacement/progress/R11/tables/elf_inventory.tsv'),delimiter='\t') if x['arch']=='x86_64' and x['debug_payload']=='NO' and x['path'].startswith(('/usr/lib64/','/lib64/'))]
edges={}
for x in csv.DictReader(open('/home/toolchain/development/libc++_replacement/progress/R11/tables/dt_needed_edges.tsv'),delimiter='\t'):
 if x['arch']=='x86_64':edges.setdefault((x['rpm_sha256'],x['path']),[]).append(x['soname'])
def file(x):h=x['rpm_sha256'];return c/h[:2]/h/x['path'].lstrip('/')
def resolve(name):
 matches=[]
 for x in rows:
  if pathlib.Path(x['path']).name.split('.so')[0]!=name.split('.so')[0]:continue
  p=file(x);alias=p.parent/name
  if p.name==name or p.name.startswith(name+'.') or (alias.exists() and alias.resolve()==p.resolve()):matches.append(x)
 if name=='libelementary.so.1':matches=[x for x in matches if x['name']=='elementary-tizen']
 primary=[x for x in matches if str(pathlib.Path(x['path']).parent) in ['/usr/lib64','/lib64']]
 if primary:matches=primary
 if len(matches)!=1:return None,matches
 return matches[0],matches
stage=t/'closure_ewk';stage.mkdir(exist_ok=True)
provider=r/'tmp/API_0911/W1/chromium_image/lib/libchromium-impl.so'
stem=o/('raw/ewk_complete_dynamic'+os.environ.get('API_CLOSURE_ATTEMPT',''))
subprocess.run([sys.executable,str(r/'docs/progress/API_0911/record.py'),str(stem),'readelf','-dW',str(provider)],check=True)
queue=re.findall(r'\(NEEDED\).*\[(.*?)\]',stem.with_suffix('.stdout').read_text());seen=set();results=[];gaps=[]
while queue:
 name=queue.pop(0)
 if name in seen:continue
 seen.add(name);x,matches=resolve(name)
 if x is None:gaps.append(dict(soname=name,candidates=matches));continue
 p=file(x);link=stage/name
 if not link.exists():link.symlink_to(p)
 results.append(dict(soname=name,package=x['name'],rpm_sha256=x['rpm_sha256'],path=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
 queue.extend(edges.get((x['rpm_sha256'],x['path']),[]))
 print('CLOSURE',len(results),name,flush=True)
(o/'closure.json').write_text(json.dumps(dict(staged=results,gaps=gaps),indent=2)+'\n')
print('CLOSURE_DONE',len(results),'GAPS',[(g['soname'],len(g['candidates'])) for g in gaps])
