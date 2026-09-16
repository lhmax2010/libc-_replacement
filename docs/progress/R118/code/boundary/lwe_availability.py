#!/usr/bin/env python3
import pathlib,sys
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from run import *
arch='x86_64';flags,links,build,gnu_lib=configuration(arch,'gnu');dest=TMP/'function/lwe';dest.mkdir(parents=True,exist_ok=True);exe=dest/'load_real'
provider=OLD/'tmp/R11/extracted/f2/f2a206acdb684eae2183aff41e9b53351cebb50e0f2635b633a89e6ee155f3d7/usr/lib64/lwe/mobile/liblightweight-web-engine.mobile-impl.so'
checked('function_lwe_loader_build',['nice','-n','19','ionice','-c','3',CC,*flags,CODE/'boundary/load_real.cpp',*links,'-L'+str(build/'lib'),'-Wl,--no-as-needed','-lc++','-lc++abi','-ldl','-o',exe])
rc,out,raw=record('function_lwe_actual_load',['nice','-n','19','ionice','-c','3','timeout','30',SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(provider.parent)+':'+str(build/'lib')+':'+str(SDK/'closure_ewk'),exe,provider])
save(OUT/'function/lwe_availability.json',dict(provider=str(provider),sha256=digest(provider),exit=rc,raw=raw,stdout=out,callback_execution='NOT_OBSERVED'))
