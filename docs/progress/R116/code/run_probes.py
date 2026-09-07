#!/usr/bin/env python3
import csv,pathlib,shlex,subprocess,sys
r=pathlib.Path.cwd();t=r/'tmp/R116';libs=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib');rows=[]
cases=[]
for provider in ['gnu','llvm']:
 for spec in ['retained','removed']:
  for mode in ['wait','system_clock','steady_clock','custom_clock']:
   cases.append((provider,'patched',f'wait_{spec}',mode,[],86 if spec=='retained' else 0))
 for variant in ['control','rollback']:
  for path in ['plain','timed']:
   cases.append((provider,'patched',variant+'/shared',path,[variant,'cancel'],0))
 cases.append((provider,'patched','api','api',[],0))
 for abi in ['patched','unpatched']:cases.append((provider,abi,'api','rethrow',[],None))
for provider,abi,binary,mode,args,expected in cases:
 for repeat in range(1,6):
  paths=([t/'install-native/lib'] if provider=='llvm' else [])+([t/'abi-unpatched'] if abi=='unpatched' else [])+[t/'probes',t/'probes/control',t/'probes/rollback',libs]
  # Shared rollback must resolve its own libw4.so before the control copy.
  if binary=='rollback/shared':paths.insert(0,t/'probes/rollback')
  cmd=['nice','-n','15','ionice','-c','3','timeout','20','env',f'R116_PROVIDER={provider}',
       'LD_LIBRARY_PATH='+':'.join(map(str,paths)),str(t/'probes'/binary),mode,*args]
  stem=pathlib.Path(f'docs/progress/R116/raw/runs/{provider}_{abi}_{binary.replace("/","_")}_{mode}_{repeat}')
  subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(stem),'bash','-c','ulimit -c 0; exec '+shlex.join(cmd)])
  rc=int(pathlib.Path(str(stem)+'.exitcode').read_text());text=pathlib.Path(str(stem)+'.stdout').read_text()
  identity='PROVIDER_ASSERT=PASS' in text
  if expected==86:ok=rc==86 and 'event=terminate cleanup_count=0' in text
  elif expected==0 and binary.startswith('wait_'):ok=rc==0 and 'canceled=1 cleanup_count=1' in text
  elif expected==0:ok=rc==0 and 'ASSERTIONS=PASS' in text
  else:ok=None
  status=('MATCH' if ok else 'MISMATCH') if expected is not None else 'OBSERVED'
  if not identity:status='PROVIDER_NOT_CONFIRMED'
  rows.append([provider,abi,binary,mode,repeat,rc,identity,status,'canceled=1' in text,'ASSERTIONS=PASS' in text,str(stem)])
  with open('docs/progress/R116/measurements.tsv','w') as f:
   w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['provider','abi_pair','probe','mode','repeat','actual_exit','provider_assert','status','canceled_marker','value_assert','evidence']);w.writerows(rows)
print(f'runs={len(rows)} mismatches={sum(x[7] in ["MISMATCH","PROVIDER_NOT_CONFIRMED"] for x in rows)}')
