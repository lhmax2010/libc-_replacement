#!/usr/bin/env python3
import pathlib,sys,json,re
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from common import ROOT as r,record
o=r/'docs/progress/ADAPTOR_0912/W2';t=r/'tmp/ADAPTOR_0912/W2/x86_64';old=r/'tmp/API_0911/W1_CONT';b=r/'tmp/IMPL_0908/build-native'
provider=r/'tmp/API_0911/W1/chromium_image/lib/libchromium-impl.so';rows=[]
version=sys.argv[1] if len(sys.argv)>1 else ''
if version:t=r/'tmp/ADAPTOR_0912/W2'/version/'x86_64'
for kind,mode in [('gnu','direct'),('cxx','state'),('cxx','drop')]:
 for n in range(1,6):
  tag=f'{version}native_{mode}_{n}';entry=provider if mode=='direct' else t/(mode+'.so')
  rc,out=record('W2',tag,['prlimit','--core=0','--','timeout','40',old/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(old/'closure_ewk')+':'+str(b/'lib'),t/('probe_'+kind),entry,provider])
  rows.append(dict(arch='x86_64',environment='原生',path=mode,repeat=n,exit=rc,maps='MAP_ASSERT cxx=1 gnu=1 entry=1 provider=1' in out,summary=re.findall(r'SUMMARY EXACT=(\d+)/12 DESTROYED=(\d+)/12',out),cases=[dict(case=int(k),valid=int(v),exact=int(x)) for k,v,x in re.findall(r'CASE=(\d+) VALID=(\d+) EXACT=(\d+)',out)],raw=tag))
(o/(version+'native_matrix.json')).write_text(json.dumps(rows,ensure_ascii=False,indent=2)+'\n')
