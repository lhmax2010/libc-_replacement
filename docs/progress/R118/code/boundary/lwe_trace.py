#!/usr/bin/env python3
import pathlib,sys
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from run import *
dest=TMP/'function/lwe';base=OLD/'tmp/R11/extracted/f2/f2a206acdb684eae2183aff41e9b53351cebb50e0f2635b633a89e6ee155f3d7/usr/lib64/lwe/mobile';_,_,b,_=configuration('x86_64','gnu')
rc,out,raw=record('function_lwe_control_trace',['nice','-n','19','ionice','-c','3','timeout','30','gdb','-q','-batch','-ex','set pagination off','-ex','set debuginfod enabled off','-ex','catch throw','-ex','run','-ex','bt 18','--args',SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(dest)+':'+str(dest/'closure')+':'+str(base)+':'+str(b/'lib')+':'+str(SDK/'closure_ewk'),dest/'callback_gnu',base/'liblightweight-web-engine.mobile-impl.so',dest/'storage_gnu'])
print('diagnostic_exit',rc,'raw',raw)
