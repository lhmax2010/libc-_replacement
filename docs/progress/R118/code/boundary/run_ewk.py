#!/usr/bin/env python3
import pathlib,sys,json,shutil
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from run import *
arch=sys.argv[1];dest=TMP/'string'/arch/'ewk';dest.mkdir(parents=True,exist_ok=True)
old=ROOT/'docs/progress/API_0911/W1_CONT/ewk_probe.cpp';src=CODE/'boundary/ewk_probe_snapshot.cpp';shutil.copyfile(old,src)
cfg=json.loads((ROOT/'docs/progress/P13_0917/W1/BUILD_CONFIG.json').read_text())
if arch=='x86_64':inc=[x for x in cfg['common'] if x.startswith('-I') and '/tmp/P12/' not in x];provider=pathlib.Path(cfg['real_gnu_provider'])
else:
 inc=['-I'+str(SDK/'arm/sdk'/x) for x in ['chromium-efl-devel/usr/include/chromium-ewk','ecore-core-devel/usr/include/ecore-1','eina-devel/usr/include/eina-1','eina-devel/usr/include/eina-1/eina','eo-devel/usr/include/eo-1','efl-devel/usr/include/efl-1','capi-base-common-devel/usr/include']]
 provider='/usr/share/chromium-efl/lib/libchromium-impl.so'
rows=[];built=[]
for lib in ['gnu','cxx']:
 flags,links,build,gnu_lib=configuration(arch,lib);exe=dest/('ewk_'+lib);tag=f'boundary_ewk_{arch}_{lib}'
 checked(tag+'_build',['nice','-n','19','ionice','-c','3',CC,*flags,*inc,src,'-L'+str(build/'lib'),'-Wl,--no-as-needed','-lc++','-lc++abi',gnu_lib,'-ldl','-o',exe]);built.append((lib,exe,tag))
if arch=='armv7l':
 before,_=remote_checked('ewk_diagnostics_before','ls -1 /opt/usr/share/crash/dump')
 remote_checked('ewk_board_precheck',f'test ! -e {REMOTE} && mkdir {REMOTE}')
 upload=[(build/'lib/libc++.so.1.0','libc++.so.1'),(build/'lib/libc++abi.so.1.0','libc++abi.so.1'),(gnu_lib,'libstdc++.so.6')]+[(exe,exe.name)for _,exe,_ in built]
 for p,n in upload:checked('ewk_upload_'+n,['timeout','45',*SDB,'push',p,REMOTE+'/'+n])
 remote_checked('ewk_board_mode','chmod 755 '+' '.join(REMOTE+'/'+exe.name for _,exe,_ in built))
 remote_checked('ewk_board_provider_hash',['sha256sum',str(provider)][0]+' '+str(provider)+' /usr/lib/libstdc++.so.6')
for lib,exe,tag in built:
 for i in range(1,6):
  if arch=='x86_64':rc,out,raw=record(tag+f'_run{i}',['bash','-c','ulimit -c 0; exec "$@"','r118','nice','-n','19','ionice','-c','3',cfg['loader'],'--library-path',str(build/'lib')+':'+str(SDK/'closure_ewk'),exe,provider])
  else:rc,out,raw=remote(tag+f'_run{i}',f'ulimit -c 0; cd {REMOTE} && LD_LIBRARY_PATH={REMOTE}:/usr/share/chromium-efl/lib nice -n 19 ionice -c 3 ./{exe.name} {provider}')
  rows.append(dict(arch=arch,consumer=lib,provider='gnu',repeat=i,exit=rc,raw=raw,maps='MAP_ASSERT cxx=1 gnu=1 provider=1' in out,exact='EXACT_FIELDS=1' in out,destroyed='CONSUMER_OBJECTS_DESTROYED=1' in out))
  save(OUT/'string'/(arch+'_ewk.json'),rows)
  if lib=='gnu' and rc:raise RuntimeError('GNU 正向控制失败 '+raw)
  if 'MAP_ASSERT cxx=1 gnu=1 provider=1' not in out:raise RuntimeError('实际加载身份未确认 '+raw)
if arch=='armv7l':
 remote_checked('ewk_board_cleanup','rm -- '+' '.join(REMOTE+'/'+name for _,name in upload)+f' && rmdir {REMOTE} && test ! -e {REMOTE}')
 after,_=remote_checked('ewk_diagnostics_after','ls -1 /opt/usr/share/crash/dump')
 clean=lambda s:{x.strip() for x in s.splitlines() if x.strip() and not x.startswith('R118_TARGET_EXIT=')}
 save(OUT/'string'/'ewk_board_cleanup.json',dict(directory_absent=True,new_diagnostics=sorted(clean(after)-clean(before))))
