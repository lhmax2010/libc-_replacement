#!/usr/bin/env python3
import pathlib,sys,shutil
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from run import *
dest=TMP/'function/lwe';headers=dest/'include';headers.mkdir(exist_ok=True)
ev=OLD/'tmp/R11/extracted/22/228ad9d126fef9ef438b629a2c690a02fa637eaf703f367190fdf16b7fd88e94/usr/include/lightweight-web-engine'
for name in ['LWEWebView.h','PlatformIntegrationData.h']:
 assert (ev/name).is_file();shutil.copyfile(ev/name,headers/name)
base=OLD/'tmp/R11/extracted/f2/f2a206acdb684eae2183aff41e9b53351cebb50e0f2635b633a89e6ee155f3d7/usr/lib64/lwe/mobile'
front=base/'liblightweight-web-engine.mobile.so';provider=base/'liblightweight-web-engine.mobile-impl.so';alias=dest/'liblightweight-web-engine.so.1'
if not alias.exists():alias.symlink_to(front)
results=[]
source=CODE/'boundary/lwe_callback.cpp';snap=OUT/'function/lwe_snapshots';snap.mkdir(exist_ok=True);shutil.copyfile(source,snap/(digest(source)+'.cpp'))
for lib in ['gnu','cxx']:
 flags,links,b,g=configuration('x86_64',lib);exe=dest/('callback_'+lib)
 rc,out,raw=record('function_lwe_callback_'+lib+'_build',['nice','-n','19','ionice','-c','3',CC,*flags,'-I'+str(headers),CODE/'boundary/lwe_callback.cpp',front,*links,'-L'+str(b/'lib'),'-Wl,--no-as-needed','-lc++','-lc++abi',g,'-ldl','-o',exe])
 results.append(dict(library=lib,build_exit=rc,build_raw=raw,runs=[]))
 if rc:continue
 storage=dest/('storage_'+lib);storage.mkdir(exist_ok=True)
 rc,out,raw=record('function_lwe_callback_'+lib+'_run1',['bash','-c','ulimit -c 0; exec "$@"','r118','nice','-n','19','ionice','-c','3','timeout','20',SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(dest)+':'+str(dest/'closure')+':'+str(base)+':'+str(b/'lib')+':'+str(SDK/'closure_ewk'),exe,provider,storage])
 results[-1]['runs'].append(dict(exit=rc,raw=raw,result=out))
 # 环境前置失败不靠重复变成差异样本；成功时才扩展固定格。
 if rc==0:
  for i in range(2,6):
   rc,out,raw=record('function_lwe_callback_'+lib+f'_run{i}',['nice','-n','19','ionice','-c','3','timeout','20',SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(dest)+':'+str(dest/'closure')+':'+str(base)+':'+str(b/'lib')+':'+str(SDK/'closure_ewk'),exe,provider,storage]);results[-1]['runs'].append(dict(exit=rc,raw=raw,result=out))
save(OUT/'function/lwe_callback_results.json',results)
