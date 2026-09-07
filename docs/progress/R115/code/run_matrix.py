#!/usr/bin/env python3
import pathlib,subprocess,sys,re,csv
arch=sys.argv[1];rows=[]
cases=[(f'wait_{spec}',mode) for spec in ['retained','removed'] for mode in ['wait','system_clock','steady_clock','custom_clock']]
cases += [(n,'') for n in ['old_noisy','old_quiet','old_fno','old_fno_quiet']]
cases += [('facility',n) for n in ['timed_mutex','shared_mutex','shared_timed_mutex','future_wait','cv_any_wait','cv_any_throw']]
for binary,mode in cases:
 for repeat in range(1,6):
  stem=pathlib.Path(f'docs/progress/R115/W3/raw/matrix_{arch}/{binary}_{mode}_{repeat}')
  if arch=='armv7l':
   command=f'ulimit -c 0; cd /var/tmp/r115_20260907 && LD_LIBRARY_PATH=/var/tmp/r115_20260907 ./{binary}_bounded {mode}; rc=$?; printf "REMOTE_EXIT=%d\\n" "$rc"'
   cmd=['timeout','15','/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101','shell',command]
  else:
   cmd=['nice','-n','15','ionice','-c','3','timeout','10','env','LD_LIBRARY_PATH=tmp/R115-W3/x86_64:/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib',f'tmp/R115-W3/x86_64/{binary}',*([mode] if mode else [])]
  host=subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(stem),*cmd]).returncode
  output=pathlib.Path(str(stem)+'.stdout').read_text()
  if arch=='armv7l':
   match=re.search(r'REMOTE_EXIT=(\d+)',output)
   if host!=0 or not match: raise SystemExit('连接/执行状态异常，停止板上矩阵；不得用 host exit=0 代替远端退出码')
   rc=int(match[1])
  else:rc=host
  rows.append([arch,'物理板' if arch=='armv7l' else '原生',binary,mode,repeat,rc,
   int('canceled=1' in output),int('event=terminate' in output),int('event=cleanup' in output or 'event=frame_cleanup' in output),
   int('reusable=1' in output),int('event=assert expected=1' in output),str(stem)])
  # 逐次保存断点；不把不同结果强制变成预期的 PASS。
  with open(f'docs/progress/R115/W3/matrix_{arch}.tsv','w') as f:
   w=csv.writer(f,delimiter='\t');w.writerow(['架构','环境','二进制','模式','轮次','实际退出码','canceled','terminate','cleanup','reusable','oldcaller_assert','证据']);w.writerows(rows)
print(f'measured_runs={len(rows)} cells={len(cases)} repeats=5')
