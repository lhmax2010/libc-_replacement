#!/usr/bin/env python3
"""逐格保存精确值结果；SDB 丢失真实退出标记立即停止，不自动重试。"""
import csv,pathlib,re,shlex,subprocess,sys
r=pathlib.Path.cwd();arch=sys.argv[1];attempt=sys.argv[2];rows=[]
out=r/'tmp/IMPL_0908'/('probes-'+arch);suffix='native' if arch=='x86_64' else 'armv7l'
cases=[]
for mode in ['wait','system_clock','steady_clock','custom_clock']:cases.append(('wait',[mode],'new','wait',0))
for mode in ['timed_mutex','future_wait','cv_any_wait']:cases.append(('facility',[mode],'new','facility',0))
for name,expected in [('old_elf',86),('old_relinked',0),('new_object',0),('new_prelinked_old',86)]:cases.append((name,[],'new',name,expected))
for variant in ['old','new']:
 cases.append((variant+'_buffer_cancel',[],variant,'buffer',86 if variant=='old' else 0))
 for mode in ['cv','pubsync','destructor']:cases.append((variant+'_normal',[mode],variant,'normal',0))
 for path in ['plain','timed']:
  for mode in ['immediate','gate1_success','gate2_success','stress']+(['timeout_gate1','timeout_gate2'] if path=='timed' else []):
   cases.append((variant+'_shared',[path,'rollback' if variant=='new' else 'control',mode],variant,'shared_normal',0))
for path in ['plain','timed']:
 for mode in ['cancel','wake_reader','wake_writer']:cases.append(('new_shared',[path,'rollback',mode],'new','shared_cancel',0))
def valid(text,kind,expected):
 if 'RUNTIME_IDENTITY=PASS' not in text:return False
 if expected==86:return 'event=terminate' in text
 if kind=='wait':return 'canceled=1 cleanup_count=1' in text
 if kind=='facility':return 'canceled=1 cleanup=1' in text and 'reusable=1' in text
 if kind in ['old_relinked','new_object']:
  return 'event=assert expected=1' in text and 'canceled=1' in text and ('frame_cleanup=0 target_unlock=0' if kind=='old_relinked' else 'frame_cleanup=1 target_unlock=1') in text
 if kind=='buffer':return 'BUFFER_CANCEL canceled=1 destructor_returned=1 blocked_flush=0 cleanup=1 ASSERTIONS=PASS' in text
 if kind=='shared_cancel':return 'canceled=1 cleanup=1 internal_mutex_rc=0' in text and 'new_reader=1 new_writer=1 final_state=0' in text and 'ASSERTIONS=PASS' in text
 return 'ASSERTIONS=PASS' in text
for index,(binary,args,variant,kind,expected) in enumerate(cases,1):
 for repeat in range(1,6):
  stem=r/f'docs/progress/IMPL_0908/W3/raw/matrix_{arch}_{attempt}/{index:02}_{binary}_{repeat}'
  if arch=='armv7l':
   lib='/var/tmp/impl_0908/'+variant
   leaf=['env','LD_LIBRARY_PATH='+lib,'EXPECTED_RUNTIME='+lib,'nice','-n','15','ionice','-c','3','/var/tmp/impl_0908/probes/'+binary,*args]
   shell='ulimit -c 0; '+shlex.join(leaf)+'; rc=$?; printf "REMOTE_EXIT=%d\\n" "$rc"'
   cmd=['timeout','40','/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101','shell',shell]
  else:
   lib=r/'tmp/IMPL_0908'/('build-'+('baseline-' if variant=='old' else '')+suffix)/'lib'
   cmd=['nice','-n','15','ionice','-c','3','timeout','35','env','LD_LIBRARY_PATH='+str(lib),'EXPECTED_RUNTIME='+str(lib),str(out/binary),*args]
  subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(stem),*cmd])
  host=int(pathlib.Path(str(stem)+'.exitcode').read_text());text=pathlib.Path(str(stem)+'.stdout').read_text()
  if arch=='armv7l':
   m=re.search(r'REMOTE_EXIT=(\d+)',text)
   if host!=0 or not m:raise SystemExit('CONNECTION_FAILURE: 停止矩阵；人工可核查后重试，次数不得超过 3')
   rc=int(m[1])
  else:rc=host
  passed=rc==expected and valid(text,kind,expected)
  rows.append([arch,'物理板' if arch=='armv7l' else '原生',index,binary,' '.join(args),variant,repeat,rc,expected,'PASS' if passed else 'FAIL',str(stem)])
  with (r/f'docs/progress/IMPL_0908/W3/matrix_{arch}_{attempt}.tsv').open('w') as f:
   w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['arch','environment','cell','binary','mode','runtime','repeat','exit','expected_exit','value_assert','evidence']);w.writerows(rows)
  if not passed:raise SystemExit('VALUE_MISMATCH: '+str(rows[-1]))
print('MATRIX=PASS cells='+str(len(cases))+' runs='+str(len(rows)))
