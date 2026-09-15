#!/usr/bin/env python3
import errno,json,pathlib,subprocess,sys,os
import build as b
b.run('build_c_header',[b.CC,'-x','c','-std=c11','-I'+str(b.SRC),'-c',b.SRC/'protocol_header.c','-o',b.BUILD/'protocol_header.o'])
b.run('build_protocol_unit',[b.CC,*b.flags['gnu'],*b.common,b.SRC/'protocol_unit.cpp',b.BUILD/'ewk_back_reverse.o',*b.link,'-o',b.BUILD/'protocol_unit'])
for side in ['gnu','cxx']:
 back=b.BUILD/('vector_cxx_to_gnu_back.o' if side=='gnu' else 'vector_gnu_to_cxx_back.o')
 b.run('build_vector_protocol_'+side,[b.CC,*b.flags[side],*b.common,b.SRC/'vector_protocol_unit.cpp',back,*b.link,'-o',b.BUILD/('vector_protocol_'+side)])
variants=[('missing',str(b.BUILD/'no-such-provider.so'),errno.ENOENT),
          ('missing_symbol',str(b.LIBCXX/'lib/libc++.so.1'),errno.ENOSYS),
          ('self',str(b.BUILD/'ewk_error_self.so'),errno.ELOOP)]
for name,path,expected in variants:
 obj=b.compile('ewk_error_'+name,b.SRC/'ewk_back.cpp','gnu',['-DP12_PROVIDER_PATH="'+path+'"'])
 b.shared('ewk_error_'+name,[b.BUILD/'ewk_front_reverse.o',obj],b.SRC/'ewk.exports')
rows=[]
cases=[('cookie_protocol',b.BUILD/'protocol_unit',[],0),
       *[('vector_protocol_'+side,b.BUILD/('vector_protocol_'+side),[],0)for side in ['gnu','cxx']],
       *[('loader_'+name,b.BUILD/'ewk_fault_cxx',[b.BUILD/('ewk_error_'+name+'.so'),str(expected)],0)for name,path,expected in variants]]
for name,exe,args,expected in cases:
 for n in range(1,6):
  tag=os.environ.get('P12_RUN_TAG','')+'unit_'+name+'_'+str(n);stem=b.OUT/'raw'/tag
  assert not pathlib.Path(str(stem)+'.command.txt').exists()
  cmd=[b.SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(b.SDK/'closure_ewk')+':'+str(b.LIBCXX/'lib')+':'+str(b.BUILD),exe,*args]
  subprocess.run([sys.executable,str(b.REC),str(stem),'nice','-n','19','ionice','-c','3','prlimit','--core=0','--as=4294967296','--','timeout','40',*map(str,cmd)])
  rc=int(pathlib.Path(str(stem)+'.exitcode').read_text());text=pathlib.Path(str(stem)+'.stdout').read_text()
  assert rc==expected,(tag,rc,text)
  rows.append({'unit':name,'repeat':n,'exit':rc,'stdout':text,'raw':tag})
  (b.OUT/'UNIT_RESULTS.json').write_text(json.dumps(rows,indent=2)+'\n')
print('UNIT_RUNS',len(rows))
