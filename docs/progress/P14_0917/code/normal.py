from common import *
includes=[x for x in CONFIG['common'] if x.startswith('-I')]
run('W2','018_build_normal',[CC,*GNU,'-std=c++17','-O2','-g','-fexceptions',*includes,TMP/'normal_app.cpp',*LINK,'-o',TMP/'normal_app'])
rows=[]
for n in range(1,6):
 row=execute('W2',f'normal_original_{n}',TMP/'normal_app',[PROVIDER,PROVIDER])
 s=(ROOT/(row['raw']+'.stdout')).read_text();assert 'SUMMARY EXACT=12/12 DESTROYED=12/12' in s
 rows.append(row)
save('W2','NORMAL_ORIGINAL_RESULTS.json',rows)
