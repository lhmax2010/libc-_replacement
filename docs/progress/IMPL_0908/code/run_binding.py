#!/usr/bin/env python3
"""按实际入口版本、帧清理、原生锁与竞争者状态判定，不只看退出码。"""
import csv,pathlib,re,subprocess,sys
r=pathlib.Path.cwd();out=r/'tmp/IMPL_0908/binding-native';lib=r/'tmp/IMPL_0908/build-native/lib';rows=[]
cases=[('old_elf',86,'LLVM_22'),('old_relinked',0,'LLVM_22_TIZEN_1'),('new_object',0,'LLVM_22_TIZEN_1'),('new_prelinked_old',86,'LLVM_22'),('timed_old',86,'LLVM_22'),('timed_new',0,'LLVM_22_TIZEN_1')]
for name,expected,version in cases:
 syms=subprocess.check_output(['readelf','--dyn-syms','--wide',str(out/name)]).decode()
 symbol='15__do_timed_wait' if name.startswith('timed') else '4wait'
 assert any('condition_variable'+symbol in line and '@'+version+' ' in line for line in syms.splitlines()),name
 for repeat in range(1,6):
  stem=r/f'docs/progress/IMPL_0908/W2/raw/binding_runs/{name}_{repeat}'
  cmd=['nice','-n','15','ionice','-c','3','timeout','35','env','LD_LIBRARY_PATH='+str(lib),'EXPECTED_RUNTIME='+str(lib),str(out/name)]
  if name.startswith('timed'):cmd+=['system_clock']
  subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(stem),*cmd])
  rc=int(pathlib.Path(str(stem)+'.exitcode').read_text());text=pathlib.Path(str(stem)+'.stdout').read_text()
  assert rc==expected and 'RUNTIME_IDENTITY=PASS' in text,(name,rc)
  if expected==86:assert 'event=terminate' in text
  elif name=='timed_new':assert 'canceled=1 cleanup_count=1' in text
  else:
   assert 'event=assert expected=1' in text and 'canceled=1' in text
   if name=='old_relinked':assert 'frame_cleanup=0 target_unlock=0' in text and 'errno_busy=' in text
   else:assert 'frame_cleanup=1 target_unlock=1' in text and 'contender_rc=0' in text
  rows.append([name,version,repeat,rc,'PASS',str(stem)])
  with (r/'docs/progress/IMPL_0908/W2/bindings_native.tsv').open('w') as f:
   w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['case','version','repeat','exit','value_assert','evidence']);w.writerows(rows)
print('BINDING_MATRIX=PASS cells=6 runs='+str(len(rows)))
