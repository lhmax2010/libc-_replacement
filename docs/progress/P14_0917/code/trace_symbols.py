from common import *
run('W2','006_trace_with_symbols',[CC,'-x','c','-std=c11','-O1','-g','-fPIC','-shared','-nostdlib++',TMP/'alloc_trace.c','-lgcc_s','-ldl','-o',TMP/'alloc_trace.so'])
execute('W2','symbol_original_4',TMP/'reproduce',[PROVIDER,4,1,0])
