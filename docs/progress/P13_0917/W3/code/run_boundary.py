#!/usr/bin/env python3
import json,os,pathlib,re,subprocess,sys
import build as b
prefix=os.environ.get('P12_RUN_TAG','')
for kind in ['boundary','fault']:
 for side in ['gnu','cxx']:
  b.run('build_ewk_'+kind+'_'+side,[b.CC,*b.flags[side],*b.common,b.SRC/('ewk_'+kind+'_app.cpp'),*b.link,'-o',b.BUILD/('ewk_'+kind+'_'+side)])
rows=[];reference={}
for side,mode in [('gnu','control'),('cxx','separate'),('cxx','embedded')]:
 for n in range(1,6):
  tag=prefix+'boundary_'+side+'_'+mode+'_'+str(n);stem=b.OUT/'raw'/tag
  assert not pathlib.Path(str(stem)+'.command.txt').exists(),tag
  cmd=[b.SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(b.SDK/'closure_ewk')+':'+str(b.LIBCXX/'lib')+':'+str(b.BUILD),b.BUILD/('ewk_boundary_'+side),b.BUILD/('forwarder_'+mode+'.so'),b.PROVIDER]
  subprocess.run([sys.executable,str(b.REC),str(stem),'nice','-n','19','ionice','-c','3','prlimit','--core=0','--as=4294967296','--','timeout','40',*map(str,cmd)])
  rc=int(pathlib.Path(str(stem)+'.exitcode').read_text());out=pathlib.Path(str(stem)+'.stdout').read_text()
  assert rc==0 and 'BOUNDARY_COMPLETE=17 DESTROYED=17' in out,(tag,rc)
  cases=[]
  for line in out.splitlines():
   if not line.startswith('BOUNDARY='):continue
   case=dict(x.split('=',1) for x in line.split());i=int(case['BOUNDARY'])
   expiry=int(case['expiry']);before=int(case['before']);after=int(case['after'])
   if i in [4,7]:
    delta=0 if i==4 else 60;assert before+delta<=expiry<=after+delta;case['expiry']='CLOCK_WINDOW_'+str(delta)
   elif i==9:
    # Observe original overflow behaviour, without asserting it is valid semantics.
    case['expiry']='DELTA_'+str(expiry-before)
   del case['before'];del case['after'];cases.append(case)
  assert len(cases)==17
  if mode=='control':reference[n]=cases
  else:assert cases==reference[n],(tag,[(x,y)for x,y in zip(cases,reference[n])if x!=y])
  rows.append({'mode':mode,'consumer':side,'repeat':n,'matched':True,'cases':cases,'raw':tag})
  (b.OUT/(prefix+'BOUNDARY_RESULTS.json')).write_text(json.dumps(rows,indent=2)+'\n')
for mode in ['separate','embedded']:
 for n in range(1,6):
  tag=prefix+'fault_'+mode+'_'+str(n);stem=b.OUT/'raw'/tag
  assert not pathlib.Path(str(stem)+'.command.txt').exists(),tag
  cmd=[b.SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(b.SDK/'closure_ewk')+':'+str(b.LIBCXX/'lib')+':'+str(b.BUILD),b.BUILD/'ewk_fault_cxx',b.BUILD/('forwarder_'+mode+'.so'),'0']
  subprocess.run([sys.executable,str(b.REC),str(stem),'nice','-n','19','ionice','-c','3','prlimit','--core=0','--as=4294967296','--','timeout','40',*map(str,cmd)])
  assert int(pathlib.Path(str(stem)+'.exitcode').read_text())==0
  assert 'FAULT_SUITE=PASS' in pathlib.Path(str(stem)+'.stdout').read_text()
print('BOUNDARY_ROWS',len(rows),'FAULT_RUNS',10)
