#!/usr/bin/env python3
from common import *
from run_types import configuration,CC,ARMROOT,TMP
import re
gate('edges_gate')
materials=json.loads((OUT/'TARGET_PACKAGE_MATERIALS.json').read_text())
def package(name):
 a=[r for r in materials if r['name']==name and r['arch']=='armv7l'];assert len(a)==1 and a[0]['exists'];return a[0]
def pkgroot(name):return pathlib.Path(package(name)['extracted'])
sources={
 'dali':('#include <dali/devel-api/threading/conditional-wait.h>\nvoid probe(Dali::ConditionalWait& w, const Dali::ConditionalWait::ScopedLock& l, Dali::ConditionalWait::TimePoint t) { w.WaitUntil(l,t); }\n',['dali2-integration-devel','dali2-devel'],'ConditionalWait9WaitUntil'),
 'zypp':('#include <zypp/parser/xml/Reader.h>\nvoid probe(const zypp::InputStream& in, const zypp::xml::Validate& v) { zypp::xml::Reader r(in,v); }\n',['libzypp-devel'],'ReaderC1ERKNS_11InputStream')}
results=[];headers={}
for kind,(code,pkgs,needle) in sources.items():
 src=OUT/'probes'/f'{kind}_signature.cpp';src.write_text(code)
 incs=['-I'+str(pkgroot(p)/'usr/include') for p in pkgs]
 if kind=='zypp':incs+=['-I'+str(pkgroot('boost-devel')/'usr/include'),'-I'+str(pkgroot('libxml2-devel')/'usr/include/libxml2')]
 for lib in ['gnu','cxx']:
  opts,_=configuration(lib);runs=[];symbols=[]
  for i in range(1,6):
   obj=TMP/f'{kind}_{lib}_{i}.o'
   rc,out,build=record(f'edge_build_{kind}_{lib}_{i}',[CC,*opts,*incs,'-H','-c',src,'-o',obj])
   if rc:
    save(OUT/'EDGE_BUILD_BLOCK.json',dict(kind=kind,library=lib,record=build));raise SystemExit(rc)
   for line in (ROOT/(build+'.stderr')).read_text().splitlines():
    m=re.match(r'^\.+ (/.+)$',line)
    if m and pathlib.Path(m[1]).is_file():headers[m[1]]=sha(m[1])
   rc,out,symref=record(f'edge_symbols_{kind}_{lib}_{i}',['readelf','-h','-Ws','--wide',obj]);assert rc==0 and 'ARM' in out
   matches=[s.split()[-1] for s in out.splitlines() if ' UND ' in s and needle in s];assert len(matches)==1,matches
   symbols.append(matches[0]);runs.append(dict(build=build,readelf=symref,object_sha256=sha(obj)))
  assert len(set(symbols))==1
  results.append(dict(interface=kind,library=lib,arch='armv7l',kind='COMPILE_ONLY_REAL_ARM_DEVEL_HEADER',symbol=symbols[0],source=str(src.relative_to(ROOT)),source_sha256=sha(src),header_packages=[package(p) for p in pkgs],repeats=runs))
save(OUT/'SIGNATURES.json',results);save(OUT/'EDGE_HEADER_SHA256.json',headers)
# 真实 armv7l GNU 提供方与消费方，验证边仍存在；不构建或运行业务包。
actual=[]
for kind,provider,consumer,ppath,cpath in [('dali','dali2','dali2-ui-foundation','usr/lib/libdali2-core.so.2.0.0','usr/lib/libdali2-ui-foundation.so.2.0.0'),('zypp','libzypp','zypper','usr/lib/libzypp.so.1722.1.7','usr/bin/zypper')]:
 symbol=next(r['symbol'] for r in results if r['interface']==kind and r['library']=='gnu')
 e=dict(interface=kind,symbol=symbol)
 for side,pkg,path in [('provider',provider,ppath),('consumer',consumer,cpath)]:
  p=pkgroot(pkg)/path;assert p.is_file(),p
  rc,out,ref=record('actual_'+kind+'_'+side,['readelf','-h','-d','-Ws','--wide',p]);assert rc==0 and 'ARM' in out
  lines=[s for s in out.splitlines() if len(s.split())>=8 and s.split()[7].split('@')[0]==symbol and ((' UND ' in s)==(side=='consumer'))];assert lines,(kind,side,symbol)
  e[side]=dict(package=package(pkg),path=str(p),sha256=sha(p),record=ref,symbol_lines=lines)
  if side=='provider':soname=re.findall(r'\(SONAME\).*?\[([^]]+)\]',out);assert len(soname)==1;e['soname']=soname[0]
  else:assert e['soname'] in re.findall(r'\(NEEDED\).*?\[([^]]+)\]',out)
 actual.append(e)
save(OUT/'ACTUAL_ARM_EDGES.json',actual)
print(json.dumps([dict(interface=r['interface'],library=r['library'],symbol=r['symbol']) for r in results],indent=2));print('actual_edges',len(actual))
