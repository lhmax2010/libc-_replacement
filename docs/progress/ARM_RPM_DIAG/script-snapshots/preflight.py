import datetime,json,subprocess,time
from pathlib import Path
out=Path('progress/ARM_RPM_DIAG'); end=datetime.datetime.fromisoformat('2026-09-19T01:46:15+08:00')
def run(label,command):
    r=subprocess.run(['python3',str(out/'log.py'),label,*command]); return r.returncode
assert run('001_disk',['df','-h','.'])==0
assert run('002_inodes',['df','-i','.'])==0
assert run('003_disk_bytes',['df','-B1','--output=avail','.'])==0
available=int((out/'raw/003_disk_bytes.stdout.txt').read_text().splitlines()[-1])
if available<20*1024**3: raise SystemExit('STOP_DISK_BELOW_20_GIB')
def wait():
    for _ in range(600):
        if datetime.datetime.now().astimezone()>=end: raise SystemExit(124)
        time.sleep(1)
for n in range(4):
    code=run('004_io_'+str(n),['timeout','-k','2s','30s','python3','progress/BUILD_STATIC_0917B/io_probe.py'])
    if code==0: break
    if n==3: raise SystemExit('STOP_IO_AFTER_THREE_RETRIES')
    print('I/O failed or exceeded limit; waiting 600s',flush=True); wait()
for n in range(6):
    code=run('005_resource_'+str(n),['bash','tools/resource_gate.sh','--level','medium'])
    if code==0: break
    if code!=21 or n==5: raise SystemExit(code)
    print('Resource gate 21; waiting 600s',flush=True); wait()
assert run('006_processes',['ps','-eo','pid,ppid,ni,stat,etime,comm'])==0
(out/'preflight.json').write_text(json.dumps({'status':'PASS','available_bytes':available,'deadline':end.isoformat(),'checked_at':datetime.datetime.now().astimezone().isoformat()},indent=2))
print('PREFLIGHT_PASS',flush=True)
