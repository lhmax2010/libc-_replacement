#!/usr/bin/env python3
import pathlib,sys
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from run import *
flags,links,b,g=configuration('x86_64','gnu');dest=TMP/'function/lwe';exe=dest/'load_front'
base=OLD/'tmp/R11/extracted/f2/f2a206acdb684eae2183aff41e9b53351cebb50e0f2635b633a89e6ee155f3d7/usr/lib64/lwe/mobile'
provider=base/'liblightweight-web-engine.mobile-impl.so';front=base/'liblightweight-web-engine.mobile.so'
checked('function_lwe_front_build',['nice','-n','19','ionice','-c','3',CC,*flags,CODE/'boundary/load_real.cpp',*links,'-L'+str(b/'lib'),'-Wl,--no-as-needed','-lc++','-lc++abi','-ldl','-o',exe])
rc,out,raw=record('function_lwe_front_load',['nice','-n','19','ionice','-c','3','timeout','30',SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(dest/'closure')+':'+str(base)+':'+str(b/'lib')+':'+str(SDK/'closure_ewk'),exe,provider,front])
save(OUT/'function/lwe_front_result.json',dict(exit=rc,raw=raw,output=out,front_sha256=digest(front),provider_sha256=digest(provider),callback_execution='NOT_OBSERVED'))
