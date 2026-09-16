#!/usr/bin/env python3
"""只使用冻结平台清单，唯一候选才接入；歧义留缺口，不挑产品配置。"""
import pathlib,sys,csv,re,collections
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from run import *
base=OLD/'progress/R11/tables';rows=[];index=collections.defaultdict(list);edges=collections.defaultdict(list)
with (base/'elf_inventory.tsv').open() as f:
 for x in csv.DictReader(f,delimiter='\t'):
  if x['arch']=='x86_64' and x['debug_payload']=='NO' and x['path'].startswith(('/usr/lib64/','/lib64/')):index[pathlib.Path(x['path']).name.split('.so')[0]].append(x)
with (base/'dt_needed_edges.tsv').open() as f:
 for x in csv.DictReader(f,delimiter='\t'):
  if x['arch']=='x86_64':edges[(x['rpm_sha256'],x['path'])].append(x['soname'])
provider=OLD/'tmp/R11/extracted/f2/f2a206acdb684eae2183aff41e9b53351cebb50e0f2635b633a89e6ee155f3d7/usr/lib64/lwe/mobile/liblightweight-web-engine.mobile-impl.so'
text,raw=checked('function_lwe_readelf',['readelf','-dW',provider]);queue=re.findall(r'\(NEEDED\).*\[(.*?)\]',text);seen=set();gaps=[]
stage=TMP/'function/lwe/closure';stage.mkdir(parents=True,exist_ok=True)
while queue:
 name=queue.pop(0)
 if name in seen:continue
 seen.add(name);matches=[]
 for x in index[name.split('.so')[0]]:
  h=x['rpm_sha256'];p=OLD/'tmp/R11/extracted'/h[:2]/h/x['path'].lstrip('/');alias=p.parent/name
  if p.is_file() and (p.name==name or p.name.startswith(name+'.') or (alias.exists() and alias.resolve()==p.resolve())):matches.append((x,p))
 primary=[y for y in matches if str(pathlib.Path(y[0]['path']).parent)in ['/usr/lib64','/lib64']]
 if primary:matches=primary
 if name=='libelementary.so.1':matches=[y for y in matches if y[0]['name']=='elementary-tizen']
 if len(matches)!=1:gaps.append(dict(soname=name,candidates=[dict(package=x['name'],path=str(p))for x,p in matches]));continue
 x,p=matches[0];link=stage/name
 if not link.exists():link.symlink_to(p)
 rows.append(dict(soname=name,package=x['name'],path=str(p),sha256=digest(p)));queue.extend(edges.get((x['rpm_sha256'],x['path']),[]))
save(OUT/'function/lwe_closure.json',dict(staged=rows,gaps=gaps));print('resolved',len(rows),'gaps',[(x['soname'],len(x['candidates']))for x in gaps])
_,_,b,_=configuration('x86_64','gnu');exe=TMP/'function/lwe/load_real'
rc,out,raw=record('function_lwe_closure_load',['nice','-n','19','ionice','-c','3','timeout','30',SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(stage)+':'+str(provider.parent)+':'+str(b/'lib')+':'+str(SDK/'closure_ewk'),exe,provider])
save(OUT/'function/lwe_closure_result.json',dict(exit=rc,raw=raw,output=out,callback_execution='NOT_OBSERVED'))
