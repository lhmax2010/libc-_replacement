#!/usr/bin/env python3
import subprocess,pathlib,sys,re,csv
arch=sys.argv[1];rows=[]
for name in ['old_retained','new_noisy','new_quiet']:
 for repeat in range(1,6):
  stem=f'docs/progress/R115/W3/raw/supplement_runs_{arch}/{name}_{repeat}'
  if arch=='armv7l':
   cmd=['timeout','15','/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101','shell',f'ulimit -c 0; cd /var/tmp/r115_20260907; LD_LIBRARY_PATH=/var/tmp/r115_20260907 ./{name}_bounded; rc=$?; printf "REMOTE_EXIT=%d\\n" "$rc"']
  else:
   cmd=['nice','-n','15','ionice','-c','3','env','LD_LIBRARY_PATH=tmp/R115-W3/x86_64:/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib',f'tmp/R115-W3/x86_64/{name}_bounded']
  host=subprocess.run([sys.executable,'docs/progress/R114/code/record.py',stem,*cmd]).returncode
  output=pathlib.Path(stem+'.stdout').read_text()
  if arch=='armv7l':
   m=re.search('REMOTE_EXIT=(\d+)',output)
   if host or not m:raise SystemExit('板卡状态不明，停止')
   rc=int(m[1])
  else:rc=host
  ok=rc==86 and 'event=terminate' in output if name=='old_retained' else rc==0 and 'event=assert expected=1' in output and 'frame_cleanup=1 target_unlock=1' in output
  rows.append([arch,'物理板' if arch=='armv7l' else '原生',name,repeat,rc,int(ok),stem])
  with open(f'docs/progress/R115/W3/supplement_{arch}.tsv','w') as f:
   w=csv.writer(f,delimiter='\t');w.writerow(['架构','环境','模式','轮次','退出码','断言通过','证据']);w.writerows(rows)
  if not ok:raise SystemExit('出现不同结果，已保留证据')
print('ASSERTIONS=PASS runs=15')
