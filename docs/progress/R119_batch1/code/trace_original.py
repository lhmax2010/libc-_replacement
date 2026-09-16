from common import *
dest=TMP/'W0/x86_64';base=OLD/'tmp/R11/extracted/f2/f2a206acdb684eae2183aff41e9b53351cebb50e0f2635b633a89e6ee155f3d7/usr/lib64/lwe/mobile'
_,_,b,_=configuration('x86_64','gnu')
args=['nice','-n','19','ionice','-c','3','timeout','40','gdb','-q','-batch','-ex','set pagination off','-ex','set debuginfod enabled off','-ex','catch throw','-ex','run','-ex','bt 14','-ex','info proc mappings','-ex','frame 2','-ex','x/24i $pc-70','--args',SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(dest)+':'+str(ROOT/'tmp/R118/function/lwe/closure')+':'+str(base)+':'+str(b/'lib')+':'+str(SDK/'closure_ewk'),dest/'callback_gnu',base/'liblightweight-web-engine.mobile-impl.so',dest/'storage_gnu','original']
record('lwe_original_trace',args)
