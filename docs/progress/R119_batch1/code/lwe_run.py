from common import *
import shutil, sys
dest=TMP/'W0/x86_64';dest.mkdir(parents=True,exist_ok=True)
base=OLD/'tmp/R11/extracted/f2/f2a206acdb684eae2183aff41e9b53351cebb50e0f2635b633a89e6ee155f3d7/usr/lib64/lwe/mobile'
front=base/'liblightweight-web-engine.mobile.so';provider=base/'liblightweight-web-engine.mobile-impl.so'
alias=dest/'liblightweight-web-engine.so.1'
if not alias.exists():alias.symlink_to(front)
headers=ROOT/'tmp/R118/function/lwe/include'
source=OUT/'code/lwe_callback.cpp';mode=sys.argv[1] if len(sys.argv)>1 else 'no-render'
results=[]
for lib in ['gnu','cxx']:
    flags,links,b,g=configuration('x86_64',lib);exe=dest/('callback_'+lib)
    rc,_,raw=record('lwe_'+mode+'_'+lib+'_build',['nice','-n','19','ionice','-c','3',CC,*flags,'-I'+str(headers),source,front,*links,'-ldl','-pthread','-o',exe])
    row=dict(library=lib,mode=mode,build_exit=rc,build_raw=raw,source_sha256=digest(source),provider_sha256=digest(provider),runs=[]);results.append(row)
    if rc:continue
    row['executable_sha256']=digest(exe)
    storage=dest/('storage_'+lib);storage.mkdir(exist_ok=True)
    for i in range(1,6):
        rc,out,raw=record('lwe_'+mode+'_'+lib+f'_run{i}', ['bash','-c','ulimit -c 0; exec "$@"','r119','nice','-n','19','ionice','-c','3','timeout','25',SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(dest)+':'+str(ROOT/'tmp/R118/function/lwe/closure')+':'+str(base)+':'+str(b/'lib')+':'+str(SDK/'closure_ewk'),exe,provider,storage,mode])
        row['runs'].append(dict(exit=rc,raw=raw));save(OUT/'W0'/('x86_64_'+mode+'.json'),results)
        if rc:break
save(OUT/'W0'/('x86_64_'+mode+'.json'),results)
