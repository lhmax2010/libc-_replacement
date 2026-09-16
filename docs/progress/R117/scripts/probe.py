from common import *
for lib,header in [('libcxx',LLVM/'vector'),('libstdcxx',GCC/'std/vector')]:
 row=run('extractapi_'+lib,[CC,*FLAGS[lib],'-std=c++23','-x','c++-header','-extract-api',header,'-o',TMP/(lib+'_vector.json')],allowed=(1,))
 print(lib,row)
