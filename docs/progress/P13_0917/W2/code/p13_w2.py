#!/usr/bin/env python3
import os,sys,json,subprocess
os.environ['P13_STAGE']='W2';os.environ['P13_PIN_ABI']='1';os.environ['P13_TESTING']='1'
import p13_driver as d
b=d.b
if sys.argv[1]=='regression':
 b.vectors();b.cookies();b.forwarder()
 for script in ['run_ewk.py','run_boundary.py','run_vectors.py','run_units.py']:
  b.run('regression_'+script,[sys.executable,b.TMP/script])
if sys.argv[1]=='exceptions':
 b.run('build_p13_error_app_w2',[b.CC,*b.flags['cxx'],*b.common,b.SRC/'p13_error_app.cpp',*b.link,'-o',b.BUILD/'p13_error_app'])
 rows=[]
 for mode in ['separate','embedded']:
  for kind,expected in [(2,75),(3,22),(4,5),(5,5),(6,12),(7,None),(8,5),(9,22)]:
   for n in range(1,6):
    row=d.run_case('exception_'+mode+'_'+str(kind)+'_'+str(n),b.BUILD/'p13_error_app',[b.BUILD/('forwarder_'+mode+'.so'),b.PROVIDER,kind,5,expected or 5]);rows.append(dict(mode=mode,kind=kind,repeat=n,expected_errno=expected,**row))
    (b.OUT/'EXCEPTIONS.json').write_text(json.dumps(rows,indent=2)+'\n')
    assert 'MAP_ASSERT gnu=1 cxx=1 entry=1 provider=1' in row['stdout'],row
    if kind!=7:assert row['exit']==0 and 'OUTPUT_UNCHANGED=PASS' in row['stdout'],row
    else:assert row['exit']==-6,'Do not pretend destructor double-escape is contained'
if sys.argv[1]=='allocations':
 observer=b.BUILD/'p13_alloc_observer.so'
 b.run('build_allocation_observer',[b.CC,'-x','c','-std=c11','-O2','-fPIC','-shared','-nostdlib++','-I'+str(b.SRC),b.SRC/'p13_alloc_observer.c','-o',observer])
 b.run('build_observed_error_app',[b.CC,*b.flags['cxx'],*b.common,b.SRC/'p13_error_app.cpp','-Wl,--no-as-needed',observer,*b.link,'-o',b.BUILD/'p13_observed_error_app'])
 rows=[]
 for mode in ['separate','embedded']:
  for kind,expected in [(1,12),(2,75),(3,22),(4,5),(5,5),(6,12)]:
   for phase in [2,5,10]:
    for n in range(1,6):
     row=d.run_case('alloc_'+mode+'_'+str(kind)+'_'+str(phase)+'_'+str(n),b.BUILD/'p13_observed_error_app',[b.BUILD/('forwarder_'+mode+'.so'),b.PROVIDER,kind,phase,expected]);rows.append(dict(mode=mode,kind=kind,phase=phase,repeat=n,**row))
     (b.OUT/'ALLOCATIONS.json').write_text(json.dumps(rows,indent=2)+'\n')
     assert row['exit']==0 and 'live=0 bytes=0 overflow=0' in row['stdout'],row
