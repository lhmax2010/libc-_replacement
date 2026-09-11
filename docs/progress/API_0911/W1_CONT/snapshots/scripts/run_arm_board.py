#!/usr/bin/env python3
"""物理 ARM 板：仅上传本任务独占目录，核对摘要，执行后逐文件清理。"""
import pathlib,subprocess,sys,hashlib,json,shlex,re
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';t=r/'tmp/API_0911/W1_CONT/arm';b=r/'tmp/IMPL_0908/build-armv7l/lib'
serial='192.168.108.26:26101';remote_dir='/opt/usr/home/owner/share/tmp/api_w1_0911_1050';sdb=['sdb','-s',serial];rows=[];uploads=[]
def record(stem,cmd):
 rc=subprocess.run([sys.executable,str(r/'docs/progress/API_0911/record.py'),str(o/'raw'/stem),'nice','-n','15','ionice','-c','3',*cmd]).returncode
 return rc,(o/'raw'/(stem+'.stdout')).read_text(errors='replace')
def remote(stem,cmd):
 wrapped='{ '+cmd+'; }; api_w1_rc=$?; printf "\\nAPI_TARGET_EXIT=%s\\n" "$api_w1_rc"; exit "$api_w1_rc"'
 rc,out=record(stem,['timeout','50',*sdb,'shell',wrapped]);m=re.findall(r'API_TARGET_EXIT=(\d+)',out)
 return (int(m[-1]) if m else None),out,rc
rc,out,transport=remote('arm_board_create','test ! -e '+shlex.quote(remote_dir)+' && mkdir '+shlex.quote(remote_dir));assert rc==0,(rc,transport,out)
files=[(b/'libc++.so.1.0','libc++.so.1'),(b/'libc++abi.so.1.0','libc++abi.so.1')]+[(t/name,name) for name in ['ewk_gnu','ewk_cxx','dali_probe','binder_property_gnu.so','binder_property_cxx.so'] if (t/name).is_file()]
if (t/'dali_core_cxx/libdali2-core.so.2.0.0').is_file():files.append((t/'dali_core_cxx/libdali2-core.so.2.0.0','dali_core_cxx.so'))
cleanup={}
try:
 for local,name in files:
  target=remote_dir+'/'+name;expected=hashlib.sha256(local.read_bytes()).hexdigest();uploads.append(dict(local=str(local),remote=target,sha256=expected))
  rc,_=record('arm_upload_'+name,[*sdb,'push',str(local),target]);assert rc==0
 rc,out,_=remote('arm_uploaded_hashes','sha256sum '+' '.join(shlex.quote(x['remote']) for x in uploads));assert rc==0
 for x in uploads:assert x['sha256']+'  '+x['remote'] in out,x
 remote('arm_provider_hashes','sha256sum /usr/share/chromium-efl/lib/libchromium-impl.so /usr/lib/libdali2-core.so.2.0.0 /usr/lib/libdali2-csharp-binder.so.2.0.0 /usr/lib/libstdc++.so.6; rpm -q elementary-tizen')
 rc,out,_=remote('arm_probe_modes','chmod 755 '+' '.join(shlex.quote(remote_dir+'/'+name) for local,name in files if name in ['ewk_gnu','ewk_cxx','dali_probe']));assert rc==0
 cases=[('ewk_gnu','ewk_gnu','/usr/share/chromium-efl/lib/libchromium-impl.so'),('ewk_cxx','ewk_cxx','/usr/share/chromium-efl/lib/libchromium-impl.so'),('dali_full_gnu_to_gnu','dali_probe','/usr/lib/libdali2-csharp-binder.so.2.0.0 /usr/lib/libdali2-core.so.2.0.0')]
 if (t/'dali_core_cxx/libdali2-core.so.2.0.0').is_file():cases.append(('dali_full_gnu_to_cxx','dali_probe','/usr/lib/libdali2-csharp-binder.so.2.0.0 '+remote_dir+'/dali_core_cxx.so'))
 for consumer in ['gnu','cxx']:
  if not (t/('binder_property_'+consumer+'.so')).is_file():continue
  for provider in ['gnu','cxx']:
   core='/usr/lib/libdali2-core.so.2.0.0' if provider=='gnu' else remote_dir+'/dali_core_cxx.so'
   cases.append(('dali_subset_'+consumer+'_to_'+provider,'dali_probe',remote_dir+'/binder_property_'+consumer+'.so '+core))
 for case,name,args in cases:
  if not (t/name).is_file():rows.append(dict(target=name,status='NOT_OBSERVED',reason='build unavailable'));continue
  for repeat in range(1,6):
   stem=f'arm_physical_{case}_{repeat}'
   command='cd '+shlex.quote(remote_dir)+' && LD_LIBRARY_PATH='+shlex.quote(remote_dir+':/usr/share/chromium-efl/lib')+' nice -n 15 ionice -c 3 ./'+name+' '+args
   rc,out,transport=remote(stem,command)
   rows.append(dict(target=case,repeat=repeat,environment='armv7l physical rpi4',target_exit=rc,transport_exit=transport,maps=('MAP_ASSERT cxx=1 gnu=1 provider=1' in out if name.startswith('ewk') else 'MAP_ASSERT cxx=1 gnu=1 binder=1 core=1' in out),exact=('EXACT_FIELDS=1' in out if name.startswith('ewk') else 'EXACT_VALUES=7 CONSUMER_DELETE_COMPLETED=7 CALLBACK_EXCEPTIONS=0' in out),fault_signal=re.findall(r'OBSERVED_SIGNAL=(\w+)',out),raw=stem))
finally:
 (o/'arm_board_uploads.json').write_text(json.dumps(uploads,indent=2)+'\n');(o/'arm_board_matrix.json').write_text(json.dumps(rows,indent=2)+'\n')
 # 只删除确切上传清单；rmdir 在有任何未列文件时会失败，不扩大范围。
 rc,out,_=remote('arm_board_cleanup','rm -f -- '+' '.join(shlex.quote(x['remote']) for x in uploads)+' && rmdir '+shlex.quote(remote_dir)+' && test ! -e '+shlex.quote(remote_dir))
 cleanup=dict(directory=remote_dir,target_exit=rc,removed_only_upload_manifest=True)
 (o/'arm_board_cleanup.json').write_text(json.dumps(cleanup,indent=2)+'\n')
 assert rc==0,cleanup
