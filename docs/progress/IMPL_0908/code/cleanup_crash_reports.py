#!/usr/bin/env python3
"""由 ZIP 中的进程 cwd 认领本批崩溃诊断，保存原件后逐个清理；不读环境字段。"""
import csv,datetime,hashlib,json,pathlib,re,shlex,subprocess,sys,zipfile
r=pathlib.Path.cwd();base=r/'docs/progress/IMPL_0908/W3';raw=base/'raw';sdb=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101'];n=0
for d in raw.glob('lit_armv7l_*'):assert (d/'exitcode').exists(),('tests still running',d)
def run(label,cmd,remote=False):
 global n
 n+=1;stem=raw/f'crashes_{n:03}_{label}'
 if remote:cmd=[*sdb,'shell',cmd+'; impl_rc=$?; printf "\\nREMOTE_EXIT=%d\\n" "$impl_rc"']
 done=subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(stem),'timeout','40',*cmd])
 text=pathlib.Path(str(stem)+'.stdout').read_text()
 assert done.returncode==0,('carrier failure; STOP',stem)
 if remote:assert re.search(r'REMOTE_EXIT=0\s*$',text),('remote failure; STOP',stem)
 return text
text=run('inventory',"find /opt/usr/share/crash/dump -maxdepth 1 -type f -name 't.tmp.exe_*.zip' -printf '%T@ %p\\n'",True)
start=datetime.datetime.fromisoformat(json.loads((raw/'001_resource.time.json').read_text())['start']).timestamp()-300
files=[];prior=[]
for line in text.splitlines():
 m=re.fullmatch(r'([0-9.]+) (/opt/usr/share/crash/dump/t\.tmp\.exe_[0-9]+_[0-9]{14}\.zip)',line)
 if m:(files if float(m[1])>=start else prior).append(m[2])
assert files and prior,'candidate and historical positive controls required'
out=r/'tmp/IMPL_0908/board-crash-preserved';out.mkdir(exist_ok=False);rows=[];owned=[]
for path in sorted(files):
 cwd=run('cwd',"unzip -p "+shlex.quote(path)+" '*/proc/*/cwd'",True).split('REMOTE_EXIT=',1)[0].strip('\x00 \r\n')
 if not (cwd.startswith('/opt/usr/impl_0908_lit/') or cwd.startswith('/var/tmp/impl_0908/')):
  rows.append([path,cwd,'NOT_OWNED_PRESERVED','','']);continue
 local=out/pathlib.Path(path).name
 run('preserve',[*sdb,'pull',path,str(local)])
 digest=hashlib.sha256(local.read_bytes()).hexdigest()
 with zipfile.ZipFile(local) as archive:
  cwd_members=[z for z in archive.infolist() if re.search(r'/proc/[0-9]+/cwd$',z.filename)]
  assert len(cwd_members)==1 and cwd_members[0].file_size<4096
  assert archive.read(cwd_members[0]).decode().strip('\x00 \r\n')==cwd
 actual=run('hash','sha256sum '+shlex.quote(path),True)
 assert actual.split()[0]==digest
 owned.append(path);rows.append([path,cwd,'OWNED_BACKED_UP',digest,str(local)])
assert owned,'ownership positive control required'
with (base/'BOARD_CRASH_CLEANUP.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['board_file','process_cwd','classification','sha256','host_backup']);w.writerows(rows)
guards=' && '.join('test ! -L '+shlex.quote(p) for p in owned)
run('remove',guards+' && rm -f -- '+' '.join(map(shlex.quote,owned)),True)
run('verify',' && '.join('test ! -e '+shlex.quote(p) for p in owned)+' && test -f '+shlex.quote(prior[0])+' && printf "TASK_CRASHES_ABSENT=PASS HISTORICAL_FILE_RETAINED=PASS\\n"',True)
print('CRASH_REPORT_CLEANUP=PASS removed='+str(len(owned))+' non_task_candidates_preserved='+str(len(files)-len(owned)))
print('RECOVERABLE_FROM_HOST='+str(out)+'; ZIP environment fields were not printed or committed')
