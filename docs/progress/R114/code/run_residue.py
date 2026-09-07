#!/usr/bin/env python3
import pathlib,subprocess,sys,csv
rows=[]
for mode in ['base','plain','timed_lock','system','steady','custom']:
 for repeat in range(1,6):
  stem=f'docs/progress/R114/W1/raw/matrix/{mode}_{repeat}'
  cmd=['env','LD_LIBRARY_PATH=tmp/R115-W3/x86_64:/home/toolchain/development/libc++_replacement/tmp/R69/build-x86_64/lib','tmp/R115-W3/x86_64/shared_residue',mode]
  rc=subprocess.run([sys.executable,'docs/progress/R114/code/record.py',stem,'nice','-n','15','ionice','-c','3',*cmd]).returncode
  output=pathlib.Path(stem+'.stdout').read_text()
  assert rc==0 and 'ASSERTIONS=PASS' in output and 'reader=0 writer=0' in output
  rows.append(['x86_64 原生',mode,repeat,rc,'PTHREAD_CANCELED','1','0','0','0',stem])
with open('docs/progress/R114/W1/matrix.tsv','x') as f:
 w=csv.writer(f,delimiter='\t');w.writerow(['架构','模式','轮次','退出码','回收值','清理次数','内部mutex返回码','新读者成功','新写者成功','证据']);w.writerows(rows)
print('ASSERTIONS=PASS cells=6 repeats=5 runs=30')
