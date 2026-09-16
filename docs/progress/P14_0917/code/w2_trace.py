from common import *
observer=TMP/'alloc_trace.so'
run('W2','004_build_observer',[CC,'-x','c','-std=c11','-O1','-g','-fPIC','-shared','-nostdlib++',TMP/'alloc_trace.c','-lgcc_s','-o',observer])
includes=[x for x in CONFIG['common'] if x.startswith('-I')]
run('W2','005_build_reproducer',[CC,*GNU,'-std=c++17','-O1','-g','-fexceptions',*includes,TMP/'reproduce.cpp','-Wl,--no-as-needed',observer,*LINK,'-o',TMP/'reproduce'])
rows=[]
for nth,reps in [(0,1),(4,1),(4,10)]:
 for n in range(1,6):
  rows.append(execute('W2',f'original_{nth}_{reps}_{n}',TMP/'reproduce',[PROVIDER,nth,reps,0]))
save('W2','ORIGINAL_RESULTS.json',rows)
