"""Per-task disk, process and bounded I/O gates; never relax limits."""
import datetime,json,shlex,subprocess,sys,time
from pathlib import Path
out=Path('progress/BUILD_WEEKEND_0918'); stage=sys.argv[1]
assert stage in ('W2','W3','W4')
now=datetime.datetime.now().astimezone(); clocks=json.loads((out/'deadlines.json').read_text())
assert stage not in clocks
deadline=min(now+datetime.timedelta(hours={'W2':6,'W3':6,'W4':3}[stage]),datetime.datetime.fromisoformat(clocks['TOTAL']))
assert deadline>now
clocks[stage]=deadline.isoformat(); (out/'deadlines.json').write_text(json.dumps(clocks,indent=2))
records=[]
def run(cmd):
    started=time.monotonic(); r=subprocess.run(cmd,capture_output=True,text=True)
    row={'command':shlex.join(cmd),'exitcode':r.returncode,'seconds':time.monotonic()-started,'stdout':r.stdout,'stderr':r.stderr}
    records.append(row); (out/(stage+'-preflight-commands.json')).write_text(json.dumps(records,indent=2))
    print(row,flush=True); return r
r=run(['df','-h','.']); assert r.returncode==0
r=run(['df','-B1','--output=avail','.']); assert r.returncode==0
available=int(r.stdout.splitlines()[-1].strip())
if available<20*1024**3: raise SystemExit('STOP_DISK_BELOW_20_GIB')
r=run(['ps','-eo','pid,ppid,comm,etime']); assert r.returncode==0
unexpected=[line for line in r.stdout.splitlines()[1:] if line.split()[2] in ('ninja','ninja.real','make','gmake','rpmbuild','clang','clang++','clang-22','cc1','cc1plus') or line.split()[2].startswith('qemu-')]
assert not unexpected,('UNRESOLVED_BUILD_PROCESSES',unexpected)
for attempt in range(6):
    r=run(['bash','tools/resource_gate.sh','--level','medium'])
    if r.returncode==0: break
    if r.returncode!=21 or attempt==5:
        (out/(stage+'-preflight-result.json')).write_text(json.dumps({'status':'RED_STOP_RESOURCE','exitcode':r.returncode,'retries':attempt},indent=2))
        raise SystemExit(r.returncode)
    print('Resource gate 21; pause 600 seconds, retry',attempt+1,flush=True)
    for second in range(600):
        if datetime.datetime.now().astimezone()>=deadline: raise SystemExit(124)
        time.sleep(1)
for attempt in range(4):
    r=run(['timeout','-k','2s','30s','python3','progress/BUILD_STATIC_0917B/io_probe.py'])
    if r.returncode==0: break
    if attempt==3:
        (out/(stage+'-preflight-result.json')).write_text(json.dumps({'status':'NOT_OBSERVED','reason':'I/O timeout persisted after three ten-minute waits'},indent=2))
        raise SystemExit(124)
    print('I/O pause 600 seconds',attempt+1,flush=True)
    for second in range(600):
        if datetime.datetime.now().astimezone()>=deadline: raise SystemExit(124)
        time.sleep(1)
(out/(stage+'-preflight-result.json')).write_text(json.dumps({'status':'PASS','available_bytes':available,'started':now.isoformat(),'deadline':deadline.isoformat()},indent=2))
print(stage,'PREFLIGHT_PASS',flush=True)
