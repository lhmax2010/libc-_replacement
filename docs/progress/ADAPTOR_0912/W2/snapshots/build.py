#!/usr/bin/env python3
import pathlib,sys,json,hashlib
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from common import ROOT as r,EXT as e,record
o=r/'docs/progress/ADAPTOR_0912/W2';t=r/'tmp/ADAPTOR_0912/W2';old=r/'tmp/API_0911/W1_CONT';prev=r/'docs/progress/API_0911/W1_CONT'
rows=[];ids=[]
version=sys.argv[1] if len(sys.argv)>1 else ''
if version:t=t/version
for arch,build in [('x86_64','build-native'),('armv7l','build-armv7l')]:
 d=t/arch;d.mkdir(parents=True,exist_ok=True);b=r/'tmp/IMPL_0908'/build
 cc=[e/'progress/R33/tools/tizen-clang++']
 if arch=='armv7l':cc+=['--target=armv7l-tizen-linux-gnueabi','--sysroot='+str(e/'tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0'),'--ld-path='+str(e/'progress/R36/tools/armv7l-ld'),'-march=armv7-a','-mfpu=neon','-mfloat-abi=softfp','-mthumb']
 identity=json.loads((prev/('arm_header_identity.json' if arch=='armv7l' else 'ewk_header_identity.json')).read_text())
 inc=['-I'+p for p in sorted({str(pathlib.Path(x['path']).parent) for x in identity if 'path' in x})]
 for x in identity:
  if 'path' in x:assert hashlib.sha256(pathlib.Path(x['path']).read_bytes()).hexdigest()==x['sha256']
 sdk=old/'arm/sdk/libstdc++-devel' if arch=='armv7l' else old/'gnu14_sdk'
 heads=[p for p in sdk.rglob('vector') if p.parent.name=='c++'];assert len(heads)==1,heads
 gh=heads[0].parent;configs=list(sdk.rglob('c++config.h'));assert len(configs)==1
 gflags=['-nostdinc++','-I'+str(gh),'-I'+str(configs[0].parent.parent),'-nostdlib++']
 cflags=['-nostdinc++','-I'+str(b/'include/c++/v1'),'-nostdlib++']
 glib=[str(old/'arm/sdk/libstdc++/usr/lib/libstdc++.so.6')] if arch=='armv7l' else ['-lstdc++']
 common=['-std=c++17','-O0','-g',*inc];link=['-L'+str(b/'lib'),'-Wl,--no-as-needed','-lc++','-lc++abi',*glib,'-ldl']
 steps=[('front',[*cc,*cflags,*common,'-fPIC','-fvisibility=hidden','-c',o/'frontend.cpp','-o',d/'frontend.o'])]
 for mode in ['state','drop']:
  steps.append((mode+'_backend',[*cc,*gflags,*common,'-fPIC','-fvisibility=hidden',*(['-DADAPTOR_DROP_INITIAL'] if mode=='drop' else []),'-c',o/'backend.cpp','-o',d/(mode+'.o')]))
  steps.append((mode+'_link',[*cc,'-nostdlib++','-shared',d/'frontend.o',d/(mode+'.o'),'-Wl,-z,defs',*link,'-o',d/(mode+'.so')]))
 for kind,flags in [('gnu',gflags),('cxx',cflags)]:
  steps.append(('probe_'+kind,[*cc,*flags,*common,'-include',prev/'arm_monitor.h',o/'state_probe.cpp',*link,'-o',d/('probe_'+kind)]))
 for tag,cmd in steps:
  rc,_=record('W2',version+arch+'_build_'+tag,cmd);rows.append(dict(arch=arch,step=tag,exit=rc))
  if rc:raise SystemExit('构建失败，先检查原始错误：'+tag)
 record('W2',version+arch+'_symbols',['bash','-c','readelf -d '+str(d/'state.so')+'; nm -D -C '+str(d/'state.so')+'; readelf -Ws '+str(d/'frontend.o')+'; readelf -Ws '+str(d/'state.o')])
 for p in [*d.glob('*.so'),d/'probe_gnu',d/'probe_cxx',b/'lib/libc++.so.1.0',b/'lib/libc++abi.so.1.0']:
  ids.append(dict(path=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
(o/(version+'build_results.json')).write_text(json.dumps(rows,indent=2)+'\n');(o/(version+'binary_identity.json')).write_text(json.dumps(ids,indent=2)+'\n')
