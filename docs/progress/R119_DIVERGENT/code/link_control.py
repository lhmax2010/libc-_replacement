#!/usr/bin/env python3
"""机制正向/反向对照，不冒充平台包边。"""
from common import *
from measure import CC,SDK,BUILD,TMP,flags
import re
gate('link_control_gate');srcdir=OUT/'controls';srcdir.mkdir(exist_ok=True)
provider=srcdir/'offset_provider.cpp';consumer=srcdir/'offset_consumer.cpp'
provider.write_text('#include <ios>\nstd::streamoff accept_offset(std::streamoff v){return v+7;}\nstd::streamoff offset_return(){return 12345;}\n')
consumer.write_text('#include <ios>\n#include <cstdio>\nstd::streamoff accept_offset(std::streamoff);\nint main(){auto actual=accept_offset(13);std::printf("input=13 actual=%lld expected=20\\n",(long long)actual);return actual==20?0:1;}\n')
objects={};symbols={};builds={}
for lib in ['gnu','cxx']:
 opts,links=flags(lib)
 for role,src in [('provider',provider),('consumer',consumer)]:
  obj=TMP/f'offset_{lib}_{role}.o';rc,out,ref=record(f'offset_compile_{lib}_{role}',[CC,*opts,'-c',src,'-o',obj]);assert rc==0
  objects[(lib,role)]=obj;builds[(lib,role)]=ref
  rc,out,ref=record(f'offset_symbols_{lib}_{role}',['readelf','-Ws','--wide',obj]);assert rc==0;symbols[(lib,role)]=dict(record=ref,text=out)
assert '_Z13accept_offsetl' in symbols[('gnu','provider')]['text']
assert '_Z13accept_offsetx' in symbols[('cxx','provider')]['text']
assert all('_Z13offset_returnv' in symbols[(lib,'provider')]['text'] for lib in ['gnu','cxx'])
results=[]
for app,prov in [('gnu','gnu'),('cxx','cxx'),('gnu','cxx'),('cxx','gnu')]:
 opts,links=flags(app);exe=TMP/f'offset_{app}_{prov}';rc,out,ref=record(f'offset_link_{app}_{prov}',[CC,*opts,objects[(app,'consumer')],objects[(prov,'provider')],*links,'-o',exe])
 r=dict(consumer=app,provider=prov,link_exit=rc,link_record=ref,runs=[])
 if app!=prov:
  error=(ROOT/(ref+'.stderr')).read_text();assert rc!=0 and 'accept_offset' in error;r['observed']='LINK_REJECTED_TYPE_ENCODING';r['link_attempts']=[ref]
  for n in range(2,6):
   rc,out,more=record(f'offset_link_{app}_{prov}_{n}',[CC,*opts,objects[(app,'consumer')],objects[(prov,'provider')],*links,'-o',exe]);assert rc!=0 and 'accept_offset' in (ROOT/(more+'.stderr')).read_text();r['link_attempts'].append(more)
 else:
  assert rc==0
  for n in range(1,6):
   rc,out,run=record(f'offset_run_{app}_{prov}_{n}',[SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(BUILD/'lib')+':'+str(SDK/'closure_ewk'),exe]);assert rc==0 and 'input=13 actual=20 expected=20' in out;r['runs'].append(run)
  r['observed']='VALUE_20_IN_5_OF_5'
 results.append(r)
save(OUT/'LINK_CONTROL.json',dict(results=results,parameter_symbols={'gnu':'_Z13accept_offsetl','cxx':'_Z13accept_offsetx'},return_only_symbol='_Z13offset_returnv',interpretation='参数出现底层类型时，本最小对照两向均链接拒绝；普通非模板函数仅返回类型不同仍可同名。不是实际平台部署测试。',source_sha256={str(p.relative_to(ROOT)):sha(p) for p in [provider,consumer]}))
print(json.dumps(results,ensure_ascii=False,indent=2))
