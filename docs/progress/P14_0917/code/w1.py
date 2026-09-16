from common import *
import shutil
src=ROOT/'tmp/P12/src/p13_backend_test.h'
shutil.copy2(src,TMP/src.name)
save('W1','FIXTURE_IDENTITY.json',{'source':str(src),'source_sha256':hashlib.sha256(src.read_bytes()).hexdigest(),'copy_sha256':hashlib.sha256((TMP/src.name).read_bytes()).hexdigest()})
exe=TMP/'gnu_exceptions'
run('W1','003_build',[CC,*GNU,'-std=c++17','-O2','-g','-fexceptions',TMP/'gnu_exceptions.cpp',*LINK,'-o',exe])
rows=[]
for kind in [0,6,7,8,9]:
 for n in range(1,6):
  row=execute('W1',f'case_{kind}_{n}',exe,[kind],allow=(-6,) if kind==7 else ())
  assert row['exit']==(-6 if kind==7 else 0),row
  rows.append(dict(kind=kind,repeat=n,**row))
save('W1','RESULTS.json',rows)
