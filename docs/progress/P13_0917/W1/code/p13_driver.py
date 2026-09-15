#!/usr/bin/env python3
"""P13 小目标驱动；完整 argv/输出/退出码交给既有 recorder。"""
import hashlib,json,os,pathlib,subprocess,sys,time
ROOT=pathlib.Path(__file__).resolve().parents[2]
stage=os.environ.get('P13_STAGE','W1');out=ROOT/'docs/progress/P13_0917'/stage
os.environ['P12_OUT_DIR']=str(out)
os.environ['P12_BUILD_DIR']=str(ROOT/'tmp/P12/build_P13')
os.environ['P12_WORKSPACE_ROOT']=str(ROOT)
import build as b
def run_case(tag,exe,args=(),library_path=None,debug=False):
 stem=b.OUT/'raw'/tag;assert not pathlib.Path(str(stem)+'.command.txt').exists()
 cmd=['nice','-n','19','ionice','-c','3','prlimit','--core=0','--as=4294967296','--','timeout','40',str(b.SDK/'closure_ewk/ld-linux-x86-64.so.2'),'--library-path',library_path or str(b.SDK/'closure_ewk')+':'+str(b.LIBCXX/'lib')+':'+str(b.BUILD),str(exe),*map(str,args)]
 if debug:cmd=['env','LD_DEBUG=bindings',*cmd]
 subprocess.run([sys.executable,str(b.REC),str(stem),*cmd])
 result=dict(raw=tag,exit=int(pathlib.Path(str(stem)+'.exitcode').read_text()),stdout=pathlib.Path(str(stem)+'.stdout').read_text(),stderr=pathlib.Path(str(stem)+'.stderr').read_text())
 return result
if __name__=='__main__':
 action=sys.argv[1]
 if action=='catch_only':
  b.cookies()
  b.run('allocation_catch_only_build',[b.CC,*b.flags['gnu'],*b.common,b.SRC/'allocation_contract.cpp',b.BUILD/'ewk_back_reverse.o',*b.link,'-o',b.BUILD/'allocation_catch_only'])
  rows=[run_case('catch_only_'+str(n),b.BUILD/'allocation_catch_only')for n in range(1,6)]
  (b.OUT/'CATCH_ONLY.json').write_text(json.dumps(rows,indent=2)+'\n')
  print([(r['exit'],r['stdout'])for r in rows])
 if action=='pinned':
  os.environ['P13_PIN_ABI']='1'
  b.common+=['-Wa,-mrelax-relocations=no']
  b.link+=['-fuse-ld=/usr/bin/ld.gold']
  b.cookies();b.forwarder()
  for side in ['mixed','gnu_only']:
   link=b.link if side=='mixed' else ['-nostdlib++',str(b.SDK/'closure_ewk/libstdc++.so.6'),'-ldl','-pthread']
   b.run('allocation_pinned_'+side,[b.CC,*b.flags['gnu'],*b.common,b.SRC/'allocation_contract.cpp',b.BUILD/'ewk_back_reverse.o',*link,'-o',b.BUILD/('allocation_pinned_'+side)])
  rows=[]
  for side in ['mixed','gnu_only']:
   for n in range(1,6):
    row=run_case('pinned_'+side+'_'+str(n),b.BUILD/('allocation_pinned_'+side));rows.append(dict(side=side,**row))
    assert row['exit']==0 and 'ACTUAL_RETURN=-12' in row['stdout'],row
  (b.OUT/'PINNED_ALLOCATION.json').write_text(json.dumps(rows,indent=2)+'\n')
 if action=='w1_faults':
  os.environ['P13_PIN_ABI']='1';os.environ['P13_TESTING']='1'
  b.common+=['-Wa,-mrelax-relocations=no'];b.link+=['-fuse-ld=/usr/bin/ld.gold']
  b.cookies();b.forwarder()
  b.run('build_p13_error_app',[b.CC,*b.flags['cxx'],*b.common,b.SRC/'p13_error_app.cpp',*b.link,'-o',b.BUILD/'p13_error_app'])
  rows=[]
  for mode in ['separate','embedded']:
   for kind,phase in [(1,p)for p in range(11)]+[(10,-1)]:
    for n in range(1,6):
     row=run_case('w1_'+mode+'_'+str(kind)+'_'+str(phase)+'_'+str(n),b.BUILD/'p13_error_app',[b.BUILD/('forwarder_'+mode+'.so'),b.PROVIDER,kind,phase,12]);rows.append(dict(mode=mode,kind=kind,phase=phase,repeat=n,**row))
     (b.OUT/'W1_ERRORS.json').write_text(json.dumps(rows,indent=2)+'\n')
     assert row['exit']==0 and 'OUTPUT_UNCHANGED=PASS' in row['stdout'],row
