#!/usr/bin/env python3
import pathlib,sys,json,shutil
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from run import *

arch=sys.argv[1];typ=sys.argv[2] if len(sys.argv)>2 else 'string';dest=TMP/typ/arch/'boundary';dest.mkdir(parents=True,exist_ok=True)
base=OLD/'progress/R105/artifacts'
versions={'x86_64':{'gnu':('boost_x86_64_gcc','105.10.8'),'cxx':('boost_x86_64_libcxx','105.10.7')},'armv7l':{'gnu':('boost_armv7l_gcc','105.10.4'),'cxx':('boost_armv7l_libcxx_retry','105.10.3')}}
headers=base/'boost_x86_64_libcxx/boost-devel-1.83.0-105.10.7.x86_64/usr/include'
rows=[];built=[]
for kind,pkg,soname in [('program','program-options','program_options'),('test','test','unit_test_framework')]:
 if typ=='vector' and kind=='test':continue
 for caller in ['gnu','cxx']:
  flags,links,build,gnu_lib=configuration(arch,caller)
  for provider in ['gnu','cxx']:
   d,v=versions[arch][provider];sub='lib64' if arch=='x86_64' else 'lib'
   so=base/d/f'boost-{pkg}-1.83.0-{v}.{arch}/usr'/sub/f'libboost_{soname}.so.1.83.0'
   tag=f'boundary_{typ}_{arch}_{kind}_{caller}_{provider}';exe=dest/tag
   rc,out,raw=record(tag+'_build',['nice','-n','19','ionice','-c','3',CC,*flags,'-I'+str(headers),'-DBOOST_ALL_DYN_LINK',*(['-DTEST_BOOST_TEST'] if kind=='test' else []),CODE/f'boundary/boost_{typ}.cpp',so,*links,'-L'+str(build/'lib'),'-Wl,--no-as-needed','-lc++','-lc++abi',gnu_lib,'-ldl','-o',exe])
   row=dict(arch=arch,interface=kind,caller=caller,provider=provider,provider_path=str(so),provider_sha256=digest(so),build_exit=rc,build_raw=raw,runs=[]);rows.append(row)
   if rc==0:built.append((row,exe,so,tag))
   elif caller==provider:save(OUT/typ/(arch+'_boundaries.json'),rows);raise RuntimeError('同侧控制编译失败 '+tag)
if arch=='armv7l':
 remote_checked('string_boundary_precheck',f'test ! -e {REMOTE} && mkdir {REMOTE}')
 _,_,build,gnu_lib=configuration(arch,'cxx');upload=[(build/'lib/libc++.so.1.0','libc++.so.1'),(build/'lib/libc++abi.so.1.0','libc++abi.so.1'),(gnu_lib,'libstdc++.so.6')]+[(exe,exe.name)for _,exe,_,_ in built]
 for src,name in upload:checked('string_boundary_upload_'+name,['timeout','45',*SDB,'push',src,REMOTE+'/'+name])
 remote_checked('string_boundary_mode','chmod 755 '+' '.join(REMOTE+'/'+exe.name for _,exe,_,_ in built))
for row,exe,so,tag in built:
 if arch=='armv7l':checked(tag+'_upload_provider',['timeout','45',*SDB,'push',so,REMOTE+'/'+so.name])
 for i in range(1,6):
  if arch=='x86_64':
   rc,out,raw=record(tag+f'_run{i}',['nice','-n','19','ionice','-c','3',SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(so.parent)+':'+str(build/'lib')+':'+str(SDK/'closure_ewk'),exe,so])
  else:rc,out,raw=remote(tag+f'_run{i}',f'cd {REMOTE} && LD_LIBRARY_PATH={REMOTE} nice -n 19 ionice -c 3 ./{exe.name} {REMOTE}/{so.name}')
  row['runs'].append(dict(repeat=i,exit=rc,raw=raw,result=clean_output(out)))
  save(OUT/typ/(arch+'_boundaries.json'),rows)
  if rc:raise RuntimeError('控制运行失败 '+raw)
 if arch=='armv7l':remote_checked(tag+'_remove_provider',f'rm -- {REMOTE}/{so.name}')
if arch=='armv7l':remote_checked('string_boundary_cleanup','rm -- '+' '.join(REMOTE+'/'+name for _,name in upload)+f' && rmdir {REMOTE} && test ! -e {REMOTE}')
save(OUT/typ/(arch+'_boundaries.json'),rows)
